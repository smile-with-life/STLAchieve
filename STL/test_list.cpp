#include<iostream>
#include<vector>
#include<format>
#include<thread>
#include"cwq_list.h"
using namespace cwq;
template<class T>
void printList(const List<T>& list)
{
    std::cout << std::format("list size = {} \n", list.size());
    int i = 0;
    for (auto iter : list)
    {
        std::cout << std::format("List[{}] = {}\t", i, iter);
        i++;
    }
    std::cout << '\n' << '\n';
}

void test_List_1()
{
    List<int> list_1;
    std::cout << "list default:" << std::endl;
    printList(list_1);

    List<int> list_2{ std::allocator<int>() };
    std::cout << "list alloc:" << std::endl;
    printList(list_2);

    List<int> list_3(10);
    std::cout << "list init count:" << std::endl;
    printList(list_3);

    List<int> list_4(10,5);
    std::cout << "list init count and value:" << std::endl;
    printList(list_4);

    List<int> list_5 = { 1,2,3,4,5,6,7,8,9 };
    std::cout << "list init std::initializer_list<T>:" << std::endl;
    printList(list_5);

    List<int> list_6 = list_5;
    std::cout << "list init copy" << std::endl;
    printList(list_6);

    List<int> list_7 = std::move(list_6);
    std::cout << "list init move" << std::endl;
    printList(list_7);

    List<int> list_8;
    list_8 = list_7;
    std::cout << "list init copy=" << std::endl;
    printList(list_8);

    List<int> list_9;
    list_9 = std::move(list_8);
    std::cout << "list init move=" << std::endl;
    printList(list_9);

    List<int> list_10;
    list_10.assign(5, 1);
    std::cout << "list init assign count and value" << std::endl;
    printList(list_10);

    List<int> list_11;
    list_11.assign({1,2,3,4,5});
    std::cout << "list init assign std::initializer_list<T>" << std::endl;
    printList(list_11);

}

void test_List_2()
{
    List<int> list{1,2,3,4,5};
    std::cout << "list:" << std::endl;
    printList(list);

    std::cout << "list front: " << list.front() << std::endl;
    std::cout << "list back: " << list.back() << std::endl;
    std::cout << "list iterator: " << std::endl;
    auto iter = list.begin();
    std::cout << "list *iter: " << *iter << std::endl;
    std::cout << "list *iter++: " << *iter++ << std::endl;
    std::cout << "list *++iter: " << *++iter << std::endl;

    std::cout << "list begin: " << *list.begin() << std::endl;
    std::cout << "list rbegin: " << *list.rbegin() << std::endl;

    std::cout << "list empty: " << list.empty() << std::endl;
    std::cout << "list size: " << list.size() << std::endl;
    std::cout << "list max_size: " << list.max_size() << std::endl; 
}

void test_List_3()
{
    List<int> list{ 1,2,3,4,5 };
    std::cout << "list:" << std::endl;
    printList(list);

    std::cout << "list clear:" << std::endl;
    list.clear();
    printList(list);

    std::cout << "list emplace:" << std::endl;
    list.emplace(list.begin(), 1);
    printList(list);

    std::cout << "list emplace_front:" << std::endl;
    list.emplace_front(0);
    printList(list);

    std::cout << "list emplace_back:" << std::endl;
    list.emplace_back(2);
    printList(list);

    std::cout << "list insert:" << std::endl;
    list.insert(list.end(),3);
    printList(list);

    std::cout << "list insert std::initializer_list<T>:" << std::endl;
    list.insert(list.begin(),{9,9,9});
    printList(list);

    std::cout << "list earse:" << std::endl;
    list.erase(list.begin());
    printList(list);

    std::cout << "list earse range:" << std::endl;
    list.erase(list.begin(),list.end());
    printList(list);

    std::cout << "list push_front:" << std::endl;
    list.push_front(1);
    printList(list);

    std::cout << "list push_back:" << std::endl;
    list.push_back(2);
    printList(list);

    std::cout << "list pop_front:" << std::endl;
    list.pop_front();
    printList(list);

    std::cout << "list pop_back:" << std::endl;
    list.pop_back();
    printList(list);

    std::cout << "list resize count:" << std::endl;
    list.resize(5);
    printList(list);

    std::cout << "list resize count and value:" << std::endl;
    list.resize(10,1);
    printList(list);

    std::cout << "list resize count :" << std::endl;
    list.resize(3);
    printList(list);

    List<int> list_1{ 1,2,3,4,5 };
    std::cout << "list_1 :" << std::endl;
    printList(list_1);

    List<int> list_2{ 5,4,3,2,1 };
    std::cout << "list_2 :" << std::endl;
    printList(list_2);
    std::cout << "list_1 and list_2 swap :" << std::endl;
    list_1.swap(list_2);
    std::cout << "list_1 :" << std::endl;
    printList(list_1);
    std::cout << "list_2 :" << std::endl;
    printList(list_2);
}

void test_List_4()
{
    List<int> list_1{ 1,2,3,4,5 };
    std::cout << "list_1:" << std::endl;
    printList(list_1);

    List<int> list_2{ 2,3,5,7,9 };
    std::cout << "list_2:" << std::endl;
    printList(list_2);

    list_1.merge(list_2);
    std::cout << "list_1 and list_2 merge:" << std::endl;
    printList(list_1);

    List<int> list_3{ 1,2,3,4,5 };
    std::cout << "list_3:" << std::endl;
    printList(list_3);

    List<int> list_4{ 2,3,5,7,9 };
    std::cout << "list_4:" << std::endl;
    printList(list_4);

    list_3.splice(list_3.begin(),list_4);
    std::cout << "list_3 splice list_4:" << std::endl;
    std::cout << "list_3:" << std::endl;
    printList(list_3);
    std::cout << "list_4:" << std::endl;
    printList(list_4);

    List<int> list_5{ 1,2,3,4,5 };
    std::cout << "list_5:" << std::endl;
    printList(list_5);

    List<int> list_6{ 2,3,5,7,9 };
    std::cout << "list_6:" << std::endl;
    printList(list_6);

    list_5.splice(list_5.begin(), list_6, list_6.begin(),list_6.end());
    std::cout << "list_5 splice list_6 range:" << std::endl;
    std::cout << "list_5:" << std::endl;
    printList(list_5);
    std::cout << "list_6:" << std::endl;
    printList(list_6);

    List<int> list_7{ 1,2,3,4,5 };
    std::cout << "list_7:" << std::endl;
    printList(list_7);

    List<int> list_8{ 2,3,5,7,9 };
    std::cout << "list_8:" << std::endl;
    printList(list_8);

    list_7.splice(list_7.begin(), list_8,list_8.begin());
    std::cout << "list_7 splice list_8 iter:" << std::endl;
    std::cout << "list_7:" << std::endl;
    printList(list_7);
    std::cout << "list_8:" << std::endl;
    printList(list_8);
}

void test_List_5()
{
    List<int> list{ 1,2,3,4,5,5,5,3,2,1,9,8,7,3,2,4,6,7,9,10,5,8,6,6,5 };
    std::cout << "list:" << std::endl;
    printList(list);

    std::cout << "list remove:" << std::endl;
    list.remove(1);
    printList(list);

    std::cout << "list remove_if:" << std::endl;
    list.remove_if([](int n) { return n < 5; });
    printList(list);

    std::cout << "list reverse:" << std::endl;
    list.reverse();
    printList(list);

    std::cout << "list sort:" << std::endl;
    list.sort();
    printList(list);



    std::cout << "list unique:" << std::endl;
    list.unique();
    printList(list);
}