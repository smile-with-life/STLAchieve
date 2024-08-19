#include<iostream>
#include<vector>
#include<format>
#include<thread>
#include"cwq_vector.h"
template<class T>
void printVector(const cwq::Vector<T>& a)
{
    std::cout << std::format("array size = {} capacity = {}\n", a.size(), a.capacity());
    int i = 0;
    for (auto iter : a)
    {
        std::cout << std::format("array[{}] = {}\n", i, iter);
        i++;
    }
}
//功能测试
void test_Vector_1()
{
    //测试 Vector() noexcept;
    cwq::Vector<int> a{};
    std::cout << "default a:" << std::endl;
    printVector(a);
}

void test_Vector_2()
{
    //测试 explicit Vector(const Allocator& alloc) noexcept;
    cwq::Vector<int> a{std::allocator<int>()};
    std::cout << "alloc a:" << std::endl;
    printVector(a);
}

void test_Vector_3()
{
    //测试 explicit Vector(size_type count, const Allocator& alloc = Allocator());
    cwq::Vector<int> a(5);
    std::cout << "count init a:" << std::endl;
    printVector(a);
}

void test_Vector_4()
{
    //测试 Vector(size_type count,const T& value, const Allocator& alloc = Allocator());
    cwq::Vector<int> a(5,1);
    std::cout << "count and value init a:" << std::endl;
    printVector(a);
}

void test_Vector_5()
{
    //测试 template <std::random_access_iterator InputIt> Vector(InputIt first, InputIt last, const Allocator& alloc = Allocator());
    cwq::Vector<int> a(5,1);
    std::cout << "a:" << std::endl;
    printVector(a);
    cwq::Vector<int> b(a.begin(),a.end());
    std::cout << "InputIt init b:" << std::endl;
    printVector(b);
}

void test_Vector_6()
{
    //测试 Vector(const Vector& other);
    cwq::Vector<int> a(5, 1);
    std::cout << "a:" << std::endl;
    printVector(a);
    cwq::Vector<int> b(a);
    std::cout << "copy init b:" << std::endl;
    printVector(b);
}
void test_Vector_7()
{
    //测试 Vector& operator=(const Vector& other);
    cwq::Vector<int> a(5, 1);
    std::cout << "a:" << std::endl;
    printVector(a);
    cwq::Vector<int> b;
    std::cout << "default b:" << std::endl;
    printVector(b);
    b = a;
    std::cout << "copy = b:" << std::endl;
    printVector(b);
}

void test_Vector_8()
{
    //测试 Vector(Vector&& other) noexcept;
    cwq::Vector<int> a(5, 1);
    std::cout << "a:" << std::endl;
    printVector(a);
    cwq::Vector<int> b(std::move(a));
    std::cout << "move init b:" << std::endl;
    printVector(b);

}

void test_Vector_9()
{
    //测试 Vector(Vector&& other,const Allocator& alloc) noexcept;
    cwq::Vector<int> a(5, 1);
    std::cout << "a:" << std::endl;
    printVector(a);
    cwq::Vector<int> b;
    b = std::move(a);
    std::cout << "move = b:" << std::endl;
    printVector(b);

}

void test_Vector_10()
{
    //测试 Vector(std::initializer_list<T> ilist, Allocator const& alloc = Allocator());
    cwq::Vector<int> a = { 1,2,3,4,5,6,7,8,9 };
    std::cout << "init_list a:" << std::endl;
    printVector(a);
    //测试 Vector& operator=(std::initializer_list<value_type> ilist);
    a = { 9,8,7,6,5,4,3,2,1,0 };
    std::cout << "init_list = a:" << std::endl;
    printVector(a);
    //测试 assign(size_type count, const T& value);
    a.assign(3, 3);
    std::cout << "assign a:" << std::endl;
    printVector(a);
    //测试 template <std::random_access_iterator InputIt> void assign(InputIt first, InputIt last);
    cwq::Vector<int> b;
    b.assign(a.begin(), a.end());
    std::cout << "assign InputIt b:" << std::endl;
    printVector(b);
    //测试 void assign(std::initializer_list<T> ilist);
    b.assign({5,5,5,5,5});
    std::cout << "assign init_list b:" << std::endl;
    printVector(b);
}
void test_Vector_11()
{
    cwq::Vector<int> a = { 1,2,3,4,5,6,7,8,9 };
    std::cout << "a:" << std::endl;
    printVector(a);
    std::cout << "a.at(8) = " << a.at(8) << std::endl;
    std::cout << "a[8] = " << a[8] << std::endl;
    std::cout << "a.front() = " << a.front() << std::endl;
    std::cout << "a.back() = " << a.back() << std::endl;
    a.clear();
    std::cout << "a.clear()" << std::endl;
    printVector(a);
    a.push_back(9);
    std::cout << "a.push_back(9)" << std::endl;
    printVector(a);
    a.push_back(8);
    std::cout << "a.push_back(8)" << std::endl;
    printVector(a);
    a.shrink_to_fit();
    std::cout << "a.shrink_to_fit()" << std::endl;
    printVector(a);
    a.insert(a.begin(),0);
    std::cout << "a.insert(a.begin(),0)" << std::endl;
    printVector(a);
    a.insert(a.end(), 1);
    std::cout << "a.insert(a.end(), 1)" << std::endl;
    printVector(a);
    a.resize(10);
    std::cout << "a.resize(10)" << std::endl;
    printVector(a);

}
void test_Vector_12()
{
    cwq::Vector<int> a = { 1,2,3,4,5,6,7,8,9 };
    cwq::Vector<int> b = { 9,8,7,6,5,4,3,2,1 };
    std::cout << "a:" << std::endl;
    printVector(a);
    std::cout << "b:" << std::endl;
    printVector(b);
    a.swap(b);
    std::cout << "swap after a:" << std::endl;
    printVector(a);
    std::cout << "swap after b:" << std::endl;
    printVector(b);
}

void test_Vector_13()
{
    cwq::Vector<int> a = { 1,2,3,4,5,6,7,8,9 };
    cwq::Vector<int> b = { 9,8,7,6,5,4,3,2,1 };
    cwq::Vector<int> c = { 1,2,3,4,5,6,7,8,9 };
    std::cout << "a:" << std::endl;
    printVector(a);
    std::cout << "b:" << std::endl;
    printVector(b);
    std::cout << "c:" << std::endl;
    printVector(c);
    std::cout << "a == b:"<<std::boolalpha << (a == b) << std::endl;
    std::cout << "a == c:" << std::boolalpha << (a == c) << std::endl;
}

//兼容性测试
void test_Vector_14()
{
    cwq::Vector<char> char_a;
    std::cout << "char_a:" << std::endl;
    printVector(char_a);

    cwq::Vector<short> short_a;
    std::cout << "short_a:" << std::endl;
    printVector(short_a);

    cwq::Vector<int> int_a;
    std::cout << "int_a:" << std::endl;
    printVector(int_a);

    cwq::Vector<long> long_a;
    std::cout << "long_a:" << std::endl;
    printVector(long_a);

    cwq::Vector<long long> long_long_a;
    std::cout << "long_long_a:" << std::endl;
    printVector(long_long_a);

    cwq::Vector<float> float_a;
    std::cout << "float_a:" << std::endl;
    printVector(float_a);

    cwq::Vector<double> double_a;
    std::cout << "double_a:" << std::endl;
    printVector(double_a);

    cwq::Vector<long double> long_double_a;
    std::cout << "long_double_a:" << std::endl;
    printVector(long_double_a);
}

void test_Vector_15()
{
    cwq::Vector<std::string> string_a;
    cwq::Vector<cwq::Vector<int>> Vector2_a;
    cwq::Vector<std::vector<int>> std_vector_a;
    cwq::Vector<std::thread> std_thread_a;
}




