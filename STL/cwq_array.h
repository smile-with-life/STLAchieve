#pragma once
#include <cstddef>
#include<string>
#include<iterator>
#include <stdexcept> 

#if defined(_MSC_VER)
#define  CWQSTL_UNREACHABLE() __assume(0)
#elif defined(__GNUC__)
#define  CWQSTL_UNREACHABLE() _builtin_unreachable()
#elde
#define CWQSTL_UNREACHABLE() do{}while(1)
#endif
namespace cwq
{
template<class T,std::size_t N>
class Array
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reverse_iterator = std::reverse_iterator<value_type*>;
    using const_reverse_iterator = std::reverse_iterator<const value_type*>;
public:
    T m_data[N];

public://元素访问
    reference at(size_type pos)
    {
        if (pos >= N)[[unlikely]] throw std::out_of_range("cwq::Array::at error:" + std::to_string(pos) + " out of range");
        return m_data[pos];
    }

    const_reference at(size_type pos) const
    {
        if (pos >= N) [[unlikely]] throw std::out_of_range("cwq::Array::at error:" + std::to_string(pos) + " out of range");
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
        return m_data[0];
    }

    const_reference front() const noexcept
    {
        return m_data[0];
    }

    reference back() noexcept
    {
        return m_data[N - 1];
    }

    const_reference back() const noexcept
    {
        return m_data[N - 1];
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
        return m_data + N;
    }

    const_iterator end() const noexcept
    {
        return m_data + N;
    }

    const_iterator cend() const noexcept
    {
        return m_data + N;
    }

    reverse_iterator rbegin() noexcept
    {
        return std::make_reverse_iterator(m_data);
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return std::make_reverse_iterator(m_data);
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return std::make_reverse_iterator(m_data);
    }

    reverse_iterator rend() noexcept
    {
        return std::make_reverse_iterator(m_data + N);
    }

    const_reverse_iterator rend() const noexcept
    {
        return std::make_reverse_iterator(m_data + N);
    }

    const_reverse_iterator crend() const noexcept
    {
        return std::make_reverse_iterator(m_data + N);
    }
public://容量
    constexpr bool empty() const noexcept
    {
        return false;
    }

    constexpr size_type size() const noexcept
    {
        return N;
    }

    constexpr size_type max_size() const noexcept
    {
        return N;
    }
public://操作
    void fill(const T& value) noexcept(std::is_nothrow_copy_assignable_v<T>)
    {
        for (size_type i = 0; i < N; i++) 
        {
            m_data[i] = value;
        }
    }

    void swap(Array& other) noexcept(std::is_nothrow_swappable_v<T>)
    {
        for (size_type i = 0; i < N; i++)
        {
            std::swap(m_data[i], other.m_data[i]);
        }
    }
};
template<class T>
class Array<T,0>
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
        using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reverse_iterator = std::reverse_iterator<value_type*>;
    using const_reverse_iterator = std::reverse_iterator<const value_type*>;
public:

public://元素访问
    reference at(size_type pos)
    {
        throw std::out_of_range("cwq::Array::at error: Array size is 0");
    }

    const_reference at(size_type pos) const
    {
        throw std::out_of_range("cwq::Array::at error: Array size is 0");
    }

    reference operator[](size_type pos) noexcept
    {
        CWQSTL_UNREACHABLE();
    }

    const_reference operator[](size_type pos) const noexcept
    {
        CWQSTL_UNREACHABLE();
    }

    reference front() noexcept
    {
        CWQSTL_UNREACHABLE();
    }

    const_reference front() const noexcept
    {
        CWQSTL_UNREACHABLE();
    }

    reference back() noexcept
    {
        CWQSTL_UNREACHABLE();
    }

    const_reference back() const noexcept
    {
        CWQSTL_UNREACHABLE();
    }

    T* data() noexcept
    {
        return nullptr;
    }

    const T* data() const noexcept
    {
        return nullptr;
    }
public://迭代器
    iterator begin() noexcept
    {
        return nullptr;
    }

    const_iterator begin() const noexcept
    {
        return nullptr;
    }

    const_iterator cbegin() const noexcept
    {
        return nullptr;
    }

    iterator end() noexcept
    {
        return nullptr;
    }

    const_iterator end() const noexcept
    {
        return nullptr;
    }

    const_iterator cend() const noexcept
    {
        return nullptr;
    }

    reverse_iterator rbegin() noexcept
    {
        return nullptr;
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return nullptr;
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return nullptr;
    }

    reverse_iterator rend() noexcept
    {
        return nullptr;
    }

    const_reverse_iterator rend() const noexcept
    {
        return nullptr;
    }

    const_reverse_iterator crend() const noexcept
    {
        return nullptr;
    }
public://容量
    constexpr bool empty() const noexcept
    {
        return true;
    }

    constexpr size_type size() const noexcept
    {
        return 0;
    }

    constexpr size_type max_size() const noexcept
    {
        return 0;
    }
public://操作
    void fill(const T& value) noexcept(std::is_nothrow_copy_assignable_v<T>)
    {

    }

    void swap(Array& other) noexcept(std::is_nothrow_swappable_v<T>)
    {

    }
};
template< class T, class... U >
Array(T, U...) -> Array<T, 1 + sizeof...(U)>;

template< class T, std::size_t N >
bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}


};//namespace cwq