#include <iostream>

#include "skip_list.hpp"

int main()
{
	mtl::skip_list<int, int> list;

	std::cout << "empty: " << list.empty() << std::endl;

	for (int i = 0; i < 60; i++)
	{
		list.insert(i, i+1);
	}

	std::cout << "empty: " << list.empty() << std::endl;

	list.show();
	std::cout << list.size() << std::endl;

	list.erase(1);
	list.erase(14);
	list.erase(19);

	list.show();
	std::cout << list.size() << std::endl;

	std::cout << "使用迭代器遍历:" << std::endl;
	for (auto it = list.begin(); it != list.end(); it++)
	{
		std::cout << it->second() << " ";
	}
	std::cout << std::endl;


	// 查找
	std::cout << "查找后遍历:" << std::endl;
	for (auto it = list.find(10); it != list.end(); it++)
	{
		std::cout << it->second() << " ";
	}
	std::cout << std::endl;


	// 删除
	std::cout << "删除所有" << std::endl;
	list.clear();
	std::cout << "empty: " << list.empty() << std::endl;

	std::cout << "打印:" << std::endl;
	list.show();

	return 0;
}
