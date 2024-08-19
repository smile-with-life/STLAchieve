#pragma once
#include <cstddef>
#include<string>
#include <memory>
#include <exception>
#include <limits>
#include <utility>
#include <iterator>
#include <stdexcept>
#include<initializer_list>
namespace cwq
{

template<class T,class Allocator = std::allocator<T>>
class Vector
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = std::allocator_traits<Allocator>::pointer;
    using const_pointer = std::allocator_traits<Allocator>::const_pointer;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reverse_iterator = std::reverse_iterator<value_type*>;
    using const_reverse_iterator = std::reverse_iterator<const value_type*>;
private: 
    T* m_data;
    size_type m_size;
    size_type m_capacity;
    [[no_unique_address]] Allocator m_alloc;
public:
    Vector() noexcept
        : m_alloc()
    {
        m_data = nullptr;
        m_capacity = m_size = 0;
    }

    explicit Vector(const Allocator& alloc) noexcept
        : m_alloc(alloc)
    {
        m_data = nullptr;
        m_capacity = m_size = 0;
    }

    explicit Vector(size_type count, const Allocator& alloc = Allocator())
        : m_alloc(alloc)
    {
        m_data = m_alloc.allocate(count);
        m_capacity = m_size = count;
        for (size_t i = 0; i != count; i++)
        {
            std::construct_at(&m_data[i]);
        }
    }

    Vector(size_type count,const T& value, const Allocator& alloc = Allocator())
        : m_alloc(alloc) 
    {
        m_data = m_alloc.allocate(count);
        m_capacity = m_size = count;
        for (size_type i = 0; i != count; i++)
        {
            std::construct_at(&m_data[i], value);
        }
    }

    template <std::random_access_iterator InputIt>//支持随机访问的的迭代器
    Vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
        : m_alloc(alloc) 
    {
        size_type count = last - first;
        m_data = m_alloc.allocate(count);
        m_capacity = m_size = count;
        for (size_type i = 0; i != count; i++)
        {
            std::construct_at(&m_data[i], *first);
            first++;
        }
    }

    Vector(const Vector& other)
        : m_alloc(other.m_alloc)
    {
        m_capacity = m_size = other.m_size;
        if (m_size != 0)
        {
            m_data = m_alloc.allocate(m_capacity);
            for (size_type i = 0; i != m_size; i++)
            {
                std::construct_at(&m_data[i], std::as_const(other.m_data[i]));
            }
        }
        else
        {
            m_data = nullptr;
        }
    }

    Vector(const Vector& other, const Allocator& alloc)
        : m_alloc(alloc)
    {
        m_capacity = m_size = other.m_size;
        if (m_size != 0)
        {
            m_data = m_alloc.allocate(m_capacity);
            for (size_type i = 0; i != m_size; i++)
            {
                std::construct_at(&m_data[i], std::as_const(other.m_data[i]));
            }
        }
        else
        {
            m_data = nullptr;
        }
    }

    Vector& operator=(const Vector& other)
    {
        if (&other == this) [[unlikely]] return *this;
        reserve(other.m_size);
        m_size = other.m_size;
        for (size_type i = 0; i != m_size; i++)
        {
            std::construct_at(&m_data[i], std::as_const(other.m_data[i]));
        }
        return *this;   
    }

    Vector(Vector&& other) noexcept
        : m_alloc(std::move(other.m_alloc))
    {
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    Vector(Vector&& other,const Allocator& alloc) noexcept
        : m_alloc(alloc)
    {
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    Vector& operator=(Vector&& other) noexcept
    {
        if (&other == this) [[unlikely]] return *this;
        for (size_type i = 0; i != m_size; i++)
        {
            std::destroy_at(&m_data[i]);
        }
        if (m_capacity != 0)
        {
            m_alloc.deallocate(m_data, m_capacity);
        }
        m_data = other.m_data;
        m_size = other.m_size;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        return *this;
    }

    Vector(std::initializer_list<T> ilist, Allocator const& alloc = Allocator())
        : Vector(ilist.begin(), ilist.end(), alloc)
    {}

    Vector& operator=(std::initializer_list<value_type> ilist)
    {
        assign(ilist.begin(), ilist.end());
        return *this;
    }

    ~Vector() noexcept
    {
        for (size_type i = 0; i != m_size; i++)
        {
            std::destroy_at(&m_data[i]);
        }
        if (m_capacity != 0) 
        {
            m_alloc.deallocate(m_data, m_capacity);
        }
    }

    void assign(size_type count, const T& value)
    {
        clear();
        reserve(count);
        m_size = count;
        for (size_type i = 0; i != count; i++)
        {
            std::construct_at(&m_data[i], value);
        }
    }

    template <std::random_access_iterator InputIt>
    void assign(InputIt first, InputIt last)
    {
        clear();
        size_type count = last - first;
        reserve(count);
        m_size = count;
        for (size_type i = 0; i != count; i++)
        {
            std::construct_at(&m_data[i], *first);
            first++;
        }
    }

    void assign(std::initializer_list<T> ilist)
    {
        assign(ilist.begin(), ilist.end());
    }

    Allocator get_allocator() const noexcept
    {
        return m_alloc;
    }
public://元素访问
    reference at(size_type pos)
    {
        if (pos >= m_size) [[unlikely]] throw std::out_of_range("cwq::Vector::at " + std::to_string(pos) + " out of range");
        return m_data[pos];
    }
    
    const_reference at(size_type pos) const
    {
        if (pos >= m_size) [[unlikely]] throw std::out_of_range("cwq::Vector::at " + std::to_string(pos) + " out of range");
        return m_data[pos];
    }

    reference operator[](size_type pos) noexcept
    {
        return m_data[pos];
    }

    const_reference operator[](size_type pos) const noexcept
    {
        return m_data[pos];
    }

    reference front() noexcept
    {
        return *m_data;
    }

    const_reference front() const noexcept
    {
        return *m_data;
    }

    reference back() noexcept
    {
        return m_data[m_size - 1];
    }

    const_reference back() const noexcept
    {
        return m_data[m_size - 1];
    }

    T* data() noexcept
    {
        return m_data;
    }

    const T* data() const noexcept
    {
        return m_data;
    }
public://迭代器
    iterator begin() noexcept
    {
        return m_data;
    }

    const_iterator begin() const noexcept
    {
        return m_data;
    }

    const_iterator cbegin() const noexcept
    {
        return m_data;
    }

    iterator end() noexcept
    {
        return m_data + m_size;
    }

    const_iterator end() const noexcept
    {
        return m_data + m_size;
    }

    const_iterator cend() const noexcept
    {
        return m_data + m_size;
    }

    reverse_iterator rbegin() noexcept
    {
        return std::make_reverse_iterator(m_data + m_size);
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return std::make_reverse_iterator(m_data + m_size);
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return std::make_reverse_iterator(m_data + m_size);
    }

    reverse_iterator rend() noexcept
    {
        return std::make_reverse_iterator(m_data);
    }

    const_reverse_iterator rend() const noexcept
    {
        return std::make_reverse_iterator(m_data);
    }

    const_reverse_iterator crend() const noexcept
    {
        return std::make_reverse_iterator(m_data);
    }
public://容量
    bool empty() noexcept
    {
        return m_size == 0;
    }

    size_type size() const noexcept
    {
        return m_size;
    }

    size_type max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }

    void reserve(size_type new_cap)
    {
        if (new_cap > max_size()) [[unlikely]] throw(std::length_error("cwq::Vector size reaches limit"));
        if (new_cap <= m_capacity) return;
        new_cap = std::max(new_cap, m_capacity * 2);
        auto old_data = m_data;
        auto old_capacity = m_capacity;
        if (new_cap == 0)
        {
            m_data = nullptr;
            m_capacity = 0;
            if (old_capacity != 0)
            {
                for (size_type i = 0; i < m_size; i++)
                {
                    std::destroy_at(&old_data[i]);
                }
                m_alloc.deallocate(old_data, old_capacity);
            }
        }
        else 
        {
            m_data = m_alloc.allocate(new_cap);
            m_capacity = new_cap;
            if (old_capacity != 0)
            {
                for (size_type i = 0; i < m_size; i++)
                {
                    std::construct_at(&m_data[i], std::move_if_noexcept(old_data[i]));
                }
                for (size_type i = 0; i < m_size; i++)
                {
                    std::destroy_at(&old_data[i]);
                }
                m_alloc.deallocate(old_data, old_capacity);
            }
        }
    }

    size_type capacity() const noexcept
    {
        return m_capacity;
    }

    void shrink_to_fit() noexcept
    {
        auto old_data = m_data;
        auto old_capacity = m_capacity;
        m_capacity = m_size;
        if (m_size == 0) 
        {
            m_data = nullptr;
        }
        else 
        {
            m_data = m_alloc.allocate(m_size);
        }
        if (old_capacity != 0) [[likely]] 
        {
            for (size_type i = 0; i != m_size; i++)
            {
                std::construct_at(&m_data[i], std::move_if_noexcept(old_data[i])); 
                std::destroy_at(&old_data[i]);
            }
            m_alloc.deallocate(old_data, old_capacity);
        }
    }
public://修改器
    void clear() noexcept
    {
        for (size_type i = 0; i != m_size; i++)
        {
            std::destroy_at(&m_data[i]);
        }
        m_size = 0;
    }

    iterator insert(const_iterator pos, const T& value)
    {
        size_type j = pos - m_data;
        reserve(m_size + 1);
        for (size_type i = m_size; i != j; i--)
        {
            std::construct_at(&m_data[i], std::move(m_data[i - 1]));
            std::destroy_at(&m_data[i - 1]);
        }
        m_size++;
        std::construct_at(&m_data[j], value);
        return m_data + j;
    }

    iterator insert(const_iterator pos, T&& value)
    {
        size_type j = pos - m_data;
        reserve(m_size + 1);
        for (size_type i = m_size; i != j; i--) 
        {
            std::construct_at(&m_data[i], std::move(m_data[i - 1]));
            std::destroy_at(&m_data[i - 1]);
        }
        m_size++;
        std::construct_at(&m_data[j], std::move(value));
        return m_data + j;
    }

    iterator insert(const_iterator pos, size_type count, const T& value)
    {
        size_t j = pos - m_data;
        if (count == 0) [[unlikely]] return const_cast<iterator>(pos);
        reserve(m_size + count);
        for (size_type i = m_size; i != j; i--) 
        {
            std::construct_at(&m_data[i + count - 1], std::move(m_data[i - 1]));
            std::destroy_at(&m_data[i - 1]);
        }
        m_size += count;
        for (size_type i = j; i != j + count; i++)
        {
            std::construct_at(&m_data[i], value);
        }
        return m_data + j;
    }

    template <std::random_access_iterator InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last)
    {
        size_type j = pos - m_data;
        size_type count = last - first;
        if (count == 0)[[unlikely]] return const_cast<iterator>(pos);
        reserve(m_size + count);
        for (size_type i = m_size; i != j; i--)
        {
            std::construct_at(&m_data[i + count - 1], std::move(m_data[i - 1]));
            std::destroy_at(&m_data[i - 1]);
        }
        m_size += count;
        for (size_type i = j; i != j + count; i++) 
        {
            std::construct_at(&m_data[i], *first);
            first++;
        }
        return m_data + j;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist)
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    template<class... Args >
    iterator emplace(const_iterator pos, Args&&... args)
    {
        size_t j = pos - m_data;
        reserve(m_size + 1);
        for (size_type i = m_size; i != j; i--) 
        {
            std::construct_at(&m_data[i], std::move(m_data[i - 1]));
            std::destroy_at(&m_data[i - 1]);
        }
        m_size += 1;
        std::construct_at(&m_data[j], std::forward<Args>(args)...);
        return m_data + j;
    }

    iterator erase(const_iterator pos) noexcept(std::is_nothrow_move_assignable_v<T>)
    {
        size_type i = pos - m_data;
        for (size_type j = i + 1; j != m_size; j++)
        {
            m_data[j - 1] = std::move(m_data[j]);
        }
        m_size--;
        std::destroy_at(&m_data[m_size - 1]);
        return const_cast<iterator>(pos);
    }

    iterator erase(const_iterator first, const_iterator last) noexcept(std::is_nothrow_move_assignable_v<T>)
    {
        size_type diff = last - first;
        for (size_type i = last - m_data; i != m_size; i++) 
        {
            m_data[i - diff] = std::move(m_data[i]);
        }
        m_size -= diff;
        for (size_type i = m_size; i != m_size + diff; i++) 
        {
            std::destroy_at(&m_data[i]);
        }
        return const_cast<iterator>(first);
    }

    void push_back(const T& value)
    {
        if (m_size + 1 >= m_capacity) [[unlikely]] reserve(m_size + 1);
        std::construct_at(&m_data[m_size], value);
        m_size++;
    }

    void push_back(T&& value)
    {
        if (m_size + 1 >= m_capacity) [[unlikely]] reserve(m_size + 1);
        std::construct_at(&m_data[m_size], std::move(value));
        m_size++;
    }

    template< class... Args >
    void emplace_back(Args&&... args)
    {
        if (m_size + 1 >= m_capacity) [[unlikely]] reserve(m_size + 1);
        std::construct_at(&m_data[m_size], std::forward<Args>(args)...);
        m_size++;
    }

    template< class... Args >
    reference emplace_back(Args&&... args)
    {
        if (m_size + 1 >= m_capacity) [[unlikely]] reserve(m_size + 1);
        std::construct_at(&m_data[m_size], std::forward<Args>(args)...);
        m_size++;
        return m_data[m_size];
    }

    void pop_back()
    {
        m_size--;
        std::destroy_at(&m_data[m_size - 1]);
    }

    void resize(size_type count)
    {
        if (count < m_size) 
        {
            for (size_type i = count; i != m_size; i++) 
            {
                std::destroy_at(&m_data[i]);
            }
            m_size = count;
        }
        else if (count > m_size) 
        {
            reserve(count);
            for (size_type i = m_size; i != count; i++)
            {
                std::construct_at(&m_data[i]);
            }
        }
        m_size = count;
    }

    void resize(size_type count, const value_type& value)
    {
        if (count < m_size)
        {
            for (size_type i = count; i != m_size; i++)
            {
                std::destroy_at(&m_data[i]);
            }
            m_size = count;
        }
        else if (count > m_size)
        {
            reserve(count);
            for (size_type i = m_size; i != count; i++)
            {
                std::construct_at(&m_data[i],value);
            }
        }
        m_size = count;
    }

    void swap(Vector& other) noexcept
    {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_alloc, other.m_alloc);
    }

    


};
template< class T, class Allocator >
bool operator==(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) noexcept
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

}//namespace cwq
