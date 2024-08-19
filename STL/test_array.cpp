#include<iostream>
#include<vector>
#include<format>
#include<thread>
#include"cwq_array.h"

template<class T,std::size_t N>
void printArray(const cwq::Array<T,N>& a)
{
    std::cout << std::format("array size = {} \n", a.size());
    int i = 0;
    for (auto iter : a)
    {
        std::cout << std::format("array[{}] = {}\n", i, iter);
        i++;
    }
}
//功能测试
void test_Array_1()
{
    cwq::Array<int, 5> a{};
    std::cout << "default a:" << std::endl;
    printArray(a);
}
void test_Array_2()
{
    cwq::Array a{ 1,2,3 };
    std::cout << "initlist a:" << std::endl;
    printArray(a);
}
void test_Array_3()
{
    auto a = cwq::Array{ 1,2,3 };
    auto b = cwq::Array{ 3,2,1 };
    std::cout << "swap before a:" << std::endl;
    printArray(a);
    std::cout << "swap before b:" << std::endl;
    printArray(b);
    std::cout << "a==b?" << std::boolalpha << (a == b) << std::endl;
    a.swap(b);
    std::cout << "swap after a:" << std::endl;
    printArray(a);
    std::cout << "swap after b:" << std::endl;
    printArray(b);
}
void tset_Array_4()
{
    cwq::Array<int, 5> a{};
    a.fill(10);
    printArray(a);
}