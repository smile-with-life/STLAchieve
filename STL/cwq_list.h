#pragma once
#include <cstddef>
#include <iterator>
#include <memory>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <compare>
#include <initializer_list>

namespace cwq
{
template <class T>
struct ListBaseNode 
{
    ListBaseNode* m_next;
    ListBaseNode* m_prev;

    inline T& value();
    inline T const& value() const;
};

template <class T>
struct ListValueNode : ListBaseNode<T>
{
    union 
    {
        T m_value;
    };
};

template <class T>
inline T& ListBaseNode<T>::value() 
{
    return static_cast<ListValueNode<T>&>(*this).m_value;
}

template <class T>
inline T const& ListBaseNode<T>::value() const 
{
    return static_cast<const ListValueNode<T>&>(*this).m_value;
}

template<class T,class Allocator = std::allocator<T>>
class List
{
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type &;
    using pointer = std::allocator_traits<Allocator>::pointer;
    using const_pointer = std::allocator_traits<Allocator>::const_pointer;
    using ListNode = ListBaseNode<T>;
    using AllocNode = std::allocator_traits<Allocator>::template rebind_alloc<ListValueNode<T>>;
    struct iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
    private:
        ListNode* m_current;

        friend List;

        explicit iterator(ListNode* current) noexcept
            : m_current(current) 
        {}
    public:
        iterator() = default;

        iterator& operator++() noexcept 
        { 
            m_current = m_current->m_next;
            return *this;
        }

        iterator operator++(int) noexcept 
        {
            auto tmp = *this;
            m_current = m_current->m_next;
            return tmp;
        }

        iterator& operator--() noexcept 
        {
            m_current = m_current->m_prev;
            return *this;
        }

        iterator operator--(int) noexcept 
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        T& operator*() const noexcept 
        {
            return m_current->value();
        }

        bool operator!=(iterator const& other) const noexcept 
        {
            return m_current != other.m_current;
        }

        bool operator==(iterator const& other) const noexcept 
        {
            return !(*this != other);
        }
    };
    struct const_iterator
    {
        friend List;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
    private:
        const ListNode* m_current;      

        explicit const_iterator(const ListNode * current) noexcept
            : m_current(current) 
        {}
    public:
        const_iterator() = default;

        const_iterator(iterator other) noexcept 
            : m_current(other.m_current) 
        {
        }

        explicit operator iterator() noexcept 
        {
            return iterator{ const_cast<ListNode*>(m_current) };
        }

        const_iterator& operator++() noexcept
        {
            m_current = m_current->m_next;
            return *this;
        }

        const_iterator operator++(int) noexcept
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        const_iterator& operator--() noexcept
        {
            m_current = m_current->m_prev;
            return *this;
        }

        const_iterator operator--(int) noexcept
        {
            auto tmp = *this;
            m_current = m_current->m_prev;
            return tmp;
        }

        const T& operator*() const noexcept
        {
            return m_current->value();
        }

        bool operator!=(const_iterator& other) const noexcept
        {
            return m_current != other.m_current;
        }

        bool operator==(const_iterator& other) const noexcept
        {
            return !(*this != other);
        }
    };
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
    ListNode m_dummy;//无效节点,next指向头节点,prev指向尾节点
    size_type m_size{0};
    [[no_unique_address]] Allocator m_alloc;
public:
    List() noexcept
        : m_size(0)
    {
        m_dummy.m_prev = m_dummy.m_next = &m_dummy;
    }
    explicit List(const Allocator& alloc)
        : m_size(0)
        , m_alloc(alloc)
    {
        m_dummy.m_prev = m_dummy.m_next = &m_dummy;
    }
    explicit List(size_type count, const Allocator& alloc = Allocator())
        : m_alloc(alloc)
    {
        construct(count);
    }
    List(size_type count, const T& value, const Allocator& alloc = Allocator())
        : m_alloc(alloc)
    {
        construct(count, value);
    }

    template<std::input_iterator InputIt>
    List(InputIt first, InputIt last, const Allocator& alloc = Allocator())
        :m_alloc(alloc)
    {
        construct_range(first,last);
    }

    List(std::initializer_list<T> init, const Allocator& alloc = Allocator())
        : List(init.begin(),init.end(),alloc)
    {}      

    List(const List& other)
        : m_alloc(other.m_alloc)
    {
        construct_range(other.cbegin(), other.cend());
    }

    List(const List& other, const Allocator& alloc)
        : m_alloc(alloc)
    {
        construct_range(other.cbegin(), other.cend());
    }

    List(List&& other)  noexcept
        : m_alloc(other.m_alloc)
    {
        construct_move(std::move(other));
    }

    List(List&& other, const Allocator& alloc)
        : m_alloc(alloc)
    {
        construct_move(std::move(other));
    }

    ~List()
    {
        clear();
    }

    List& operator=(const List& other)
    {
        m_alloc = other.m_alloc;
        clear();
        construct_range(other.cbegin(), other.cend());
        return *this;
    }

    List& operator=(List&& other)  noexcept
    {
        m_alloc = std::move(other.m_alloc);
        clear();
        construct_move(std::move(other));
        return *this;
    }

    void assign(size_type count, const T& value)
    {
        clear();
        construct(count, value);
    }

    template<std::input_iterator InputIt>
    void assign(InputIt first, InputIt last)
    {
        clear();
        construct_range(first, last);
    }

    void assign(std::initializer_list<T> ilist)
    {
        clear();
        construct_range(ilist.begin(), ilist.end());
    }

    allocator_type get_allocator() const noexcept
    {
        return m_alloc;
    }
public://元素访问
    reference front() noexcept
    {
        return m_dummy.m_next->value();
    }

    const_reference front() const noexcept
    {
        return m_dummy.m_next->value();
    }

    reference back() noexcept
    {
        return m_dummy.m_prev->value();
    }

    const_reference back() const noexcept
    {
        return m_dummy.m_prev->value();
    }
public://迭代器
    iterator begin() noexcept
    {
        return iterator{ m_dummy.m_next };
    }

    const_iterator begin() const noexcept
    {
        return const_iterator{ m_dummy.m_next };
    }

    const_iterator cbegin() const noexcept
    {
        return const_iterator{ m_dummy.m_next };
    }

    iterator end() noexcept
    {
        return iterator{ &m_dummy };
    }

    const_iterator end() const noexcept
    {
        return const_iterator{ &m_dummy };
    }

    const_iterator cend() const noexcept
    {
        return const_iterator{ &m_dummy };
    }

    reverse_iterator rbegin() noexcept
    {
        return std::make_reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return std::make_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return std::make_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept
    {
        return std::make_reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept
    {
        return std::make_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept
    {
        return std::make_reverse_iterator(begin());
    }

public://容量
    bool empty() const noexcept
    {
        return m_dummy.m_prev == m_dummy.m_next;
    }

    size_type size() const noexcept
    {
        return m_size;
    }

    size_type max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }
public://修改器
    void clear() noexcept
    {
        ListNode* point = m_dummy.m_next;
        while (point != &m_dummy) 
        {
            std::destroy_at(&point->value());
            auto next = point->m_next;
            deleteNode(point);
            point = next;
        }
        m_dummy.m_prev = m_dummy.m_next = &m_dummy;
        m_size = 0;
    }

    template< class... Args >
    iterator emplace(const_iterator pos, Args&&... args)
    {
        ListNode* node = newNode();
        ListNode* right = const_cast<ListNode*>(pos.m_current);
        ListNode* left = right->m_prev;
        node->m_next = right;
        node->m_prev = left;
        right->m_prev = node;
        left->m_next = node;
        std::construct_at(&node->value(), std::forward<Args>(args)...);
        m_size++;
        return iterator{ node };
    }

    template<class... Args>
    reference emplace_front(Args&&... args)
    {
        ListNode* node = newNode();
        ListNode* head = m_dummy.m_next;
        head->m_prev = node;
        node->m_next = head;
        node->m_prev = &m_dummy;
        std::construct_at(&node->value(), std::forward<Args>(args)...);
        m_dummy.m_next = node;
        m_size++;
        return node->value();
    }

    template<class... Args>
    reference emplace_back(Args&&... args)
    {
        ListNode* node = newNode();
        ListNode* end = m_dummy.m_prev;
        end->m_next = node;
        node->m_prev = end;
        node->m_next = &m_dummy;
        std::construct_at(&node->value(), std::forward<Args>(args)...);
        m_dummy.m_prev = node;
        m_size++;
        return node->value();
    }
    
    iterator insert(const_iterator pos, const T& value)
    {
        return emplace(pos, value);
    }

    iterator insert(const_iterator pos, T&& value)
    {
        return emplace(pos, std::move(value));
    }

    iterator insert(const_iterator pos, size_type count, const T& value)
    {
        List<value_type, allocator_type> new_list(count, value, m_alloc);
        ListNode* head = new_list.m_dummy.m_next;
        ListNode* end = new_list.m_dummy.m_prev;
        m_size += new_list.m_size;
        new_list.m_dummy.m_next = new_list.m_dummy.m_prev = &new_list.m_dummy;
        ListNode* right = const_cast<ListNode*>(pos.m_current);
        ListNode* left = right->m_prev;
        head->m_prev = left;
        end->m_next = right;
        right->m_prev = end;
        left->m_next = head;
        
        return iterator{ head };
    }
    
    template<std::input_iterator InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last)
    {
        List<value_type, allocator_type> new_list(first, last, m_alloc);
        ListNode* head = new_list.m_dummy.m_next;
        ListNode* end = new_list.m_dummy.m_prev;
        m_size += new_list.m_size;
        new_list.m_dummy.m_next = new_list.m_dummy.m_prev = &new_list.m_dummy;
        ListNode* right = const_cast<ListNode*>(pos.m_current);
        ListNode* left = right->m_prev;
        head->m_prev = left;
        end->m_next = right;
        right->m_prev = end;
        left->m_next = head;
        return iterator{ head };
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist)
    {
        return insert(pos,ilist.begin(), ilist.end());
    }

    iterator erase(const_iterator pos)
    {
        ListNode* point = const_cast<ListNode*>(pos.m_current);
        ListNode* next = point->m_next;
        ListNode* prev = point->m_prev;
        prev->m_next = next;
        next->m_prev = prev;
        std::destroy_at(&point->value());
        deleteNode(point);
        m_size--;
        return iterator{ next };
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        while (first != last) 
        {
            first = erase(first);
        }
        return iterator(first);
    }

    void push_front(const T& value)
    {
        emplace_front(value);
    }

    void push_front(T&& value)
    {
        emplace_front(std::move(value));
    }

    void push_back(const T& value)
    {
        emplace_back(value);
    }

    void push_back(T&& value)
    {
        emplace_back(std::move(value));
    }

    void pop_front()
    {
        erase(begin());
    }


    void pop_back()
    {
        erase(std::prev(end()));
    }

    void resize(size_type count)
    {
        if (count < m_size)
        {
            size_type diff = m_size - count;
            for (size_type i = 0; i != diff; i++)
            {
                pop_back();
            }
        }
        else if (count > m_size)
        {
            size_type diff = count - m_size;
            for (size_type i = 0; i != diff; i++)
            {
                push_back(value_type());
            }
        }
        m_size = count;
    }

    void resize(size_type count, const value_type& value)
    {
        if (count < m_size)
        {
            size_type diff = m_size - count;
            for (size_type i = 0; i != diff; i++)
            {
                pop_back();
            }
        }
        else if (count > m_size)
        {
            size_type diff = count - m_size;
            for (size_type i = 0; i != diff; i++)
            {
                push_back(value);
            }
        }
        m_size = count;
    }

    void swap(List& other) noexcept
    {
        if (this == &other) [[unlikely]] return;
        auto left_head = m_dummy.m_next;
        auto left_end = m_dummy.m_prev;
        auto left_size = m_size;
        auto left_alloc = m_alloc;

        auto right_head = other.m_dummy.m_next;
        auto right_end = other.m_dummy.m_prev;
        auto right_size = other.m_size;
        auto right_alloc = other.m_alloc;

        m_dummy.m_next = right_head;
        right_head->m_prev = &m_dummy;
        m_dummy.m_prev = right_end;
        right_end->m_next = &m_dummy;
        m_size = right_size;
        m_alloc = right_alloc;

        other.m_dummy.m_next = left_head;
        left_head->m_prev = &other.m_dummy;
        other.m_dummy.m_prev = left_end;
        left_end->m_next = &other.m_dummy;
        other.m_size = left_size;
        other.m_alloc = left_alloc;
    }
public://操作
    void merge(List& other)
    {
        merge(other, std::less<value_type>{});
    }

    void merge(List&& other)
    {
        merge(other);
    }

    template<class Compare>
    void merge(List& other, Compare comp)
    {
        if (this == &other) [[unlikely]] return;
        if (other.size() == 0) [[unlikely]] return;
        auto first = begin();
        std::cout << *first;
        auto second = other.begin();
        std::cout << *second;
        while (true)
        {
            if (comp(*first,*second))
            {
                splice(first, other, second);
                second = other.begin();
                first++;
                first++;
                if (second == other.end())
                {
                    return;
                }
            }
            else
            {
                first++;
                if (first == end())
                {
                    splice(first, other, second, other.end());
                    return;
                }
            }
        }

    }

    template<class Compare>
    void merge(List&& other, Compare comp)
    {
        memrge(other, comp);
    }

    void splice(const_iterator pos, List& other)
    {
        insert(pos, other.begin(), other.end());
        other.clear();
    }

    void splice(const_iterator pos, List&& other)
    {
        insert(pos, std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()));
    }

    void splice(const_iterator pos, List& other, const_iterator it)
    {
        insert(pos, *it);
        other.erase(it);
    }

    void splice(const_iterator pos, List&& other, const_iterator it)
    {
        insert(pos, *it);
        other.clear();
    }

    void splice(const_iterator pos, List& other, const_iterator first, const_iterator last)
    {
        insert(pos, first, last);
        other.erase(first, last);
    }

    void splice(const_iterator pos, List&& other, const_iterator first, const_iterator last)
    {
        insert(pos, first, last);
    }

    size_type remove(const T& value) noexcept
    {
        auto first = begin();
        auto last = end();
        size_type count = 0;
        while (first != last) 
        {
            if (*first == value) 
            {
                first = erase(first);
                count++;
            }
            else 
            {
                first++;
            }
        }
        return count;
    }

    template<class UnaryPredicate>
    size_type remove_if(UnaryPredicate pred) noexcept
    {
        auto first = begin();
        auto last = end();
        size_type count = 0;
        while (first != last)
        {
            if (pred(*first))
            {
                first = erase(first);
                count++;
            }
            else
            {
                first++;
            }
        }
        return count;
    }
    
    void reverse()
    {
        if (m_size == 0 || m_size == 1) [[unlikely]] return;
        auto point = m_dummy.m_next;
        while (point != &m_dummy)
        {
            auto prev = point->m_prev;
            auto next = point->m_next;
            point->m_prev = next;
            point->m_next = prev;  
            point = next;
        }
        auto head = m_dummy.m_next;
        auto end = m_dummy.m_prev;
        m_dummy.m_next = end;
        m_dummy.m_prev = head;

    }

    size_type unique()
    {
        auto first = begin();
        auto second = std::next(first);
        size_type count = 0;
        while (second != end())
        {
            if (*first == *second)
            {
                second = erase(second);
                count++;
            }
            else
            {
                first++;
                second++;              
            }
        }
        return count;
    }

    template<class BinaryPredicate>
    size_type unique(BinaryPredicate pred)
    {
        auto first = begin();
        auto second = std::next(first);
        size_type count = 0;
        while (second != end())
        {
            if (pred(*first, *second))
            {
                second = earse(second);
                count++;
            }
            else
            {
                first++;
                second++;
            }
        }
    }

    void sort()
    {
        quick_sort(begin(), end(),std::less<>{});
    }

    template<class Compare>
    void sort(Compare comp)
    {
        quick_sort(begin(), end(), comp);
    }
private:
    ListNode* newNode() 
    {
        return AllocNode{ m_alloc }.allocate(1);
    }

    void deleteNode(ListNode* node) noexcept 
    {
        AllocNode{ m_alloc }.deallocate(static_cast<ListValueNode<T>*>(node), 1);
    }

    void construct(size_type count)
    {
        ListNode* point = &m_dummy;
        while (count)
        {
            ListNode* node = newNode();
            point->m_next = node;
            node->m_prev = point;
            std::construct_at(&node->value());
            point = node;
            count--;
            m_size++;
        }
        m_dummy.m_prev = point;
        point->m_next = &m_dummy;
    }

    void construct(size_type count, const T& value)
    {
        ListNode* point = &m_dummy;
        while (count)
        {
            ListNode* node = newNode();
            point->m_next = node;
            node->m_prev = point;
            std::construct_at(&node->value(), value);
            point = node;
            count--;
            m_size++;
        }
        m_dummy.m_prev = point;
        point->m_next = &m_dummy;
    }

    template <std::input_iterator InputIt>
    void construct_range(InputIt first, InputIt last)
    {
        m_size = 0;
        ListNode* point = &m_dummy;
        while (first != last)
        {
            ListNode* node = newNode();
            point->m_next = node;
            node->m_prev = point;
            std::construct_at(&node->value(), *first);
            point = node;
            first++;
            m_size++;
        }
        m_dummy.m_prev = point;
        point->m_next = &m_dummy;
    }

    void construct_move(List&& other) noexcept
    {
        auto end = other.m_dummy.m_prev;
        auto head = other.m_dummy.m_next;
        end->m_next = &m_dummy;
        head->m_prev = &m_dummy;
        m_dummy = other.m_dummy;
        other.m_dummy.m_prev = other.m_dummy.m_next = &other.m_dummy;
        m_size = other.m_size;
        other.m_size = 0;
    }
    
    template<std::input_iterator InputIt, class Compare>
    void quick_sort(InputIt first, InputIt last, Compare comp)
    {
        if (first == last)
            return;

        auto pivot = *std::next(first, std::distance(first, last) / 2);
        auto middle1 = std::partition(first, last, [pivot](const value_type& t)
            {
                return t < pivot;
            });
        auto middle2 = std::partition(middle1, last, [pivot](const value_type& t)
            {
                return !(pivot < t);
            });

        quick_sort(first, middle1,comp);
        quick_sort(middle2, last,comp);

    }
};
template<class T, class Allocator>
bool operator==(const List<T, Allocator>& lhs, const List<T, Allocator>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template< class T, class Allocator >
void swap(List<T, Allocator>& lhs, List<T, Allocator>& rhs) noexcept
{
    lhs.swap(rhs);
}
}//namespace cwq