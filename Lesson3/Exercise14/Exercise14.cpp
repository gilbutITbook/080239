#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using uint = unsigned int;

class hash_map
{
	std::vector<std::list<int>> data;

public:
	hash_map(size_t n)
	{
		data.resize(n);
	}

	void insert(uint value)
	{
		int n = data.size();
		data[value % n].push_back(value);
		std::cout << value << "을(를) 삽입했습니다." << std::endl;
	}

	bool find(uint value)
	{
		int n = data.size();
		auto& entries = data[value % n];
		return std::find(entries.begin(), entries.end(), value) != entries.end();
	}

	void erase(uint value)
	{
		int n = data.size();
		auto& entries = data[value % n];
		auto iter = std::find(entries.begin(), entries.end(), value);

		if (iter != entries.end())
		{
			entries.erase(iter);
			std::cout << value << "을(를) 삭제했습니다." << std::endl;
		}
	}
};

int main()
{
	hash_map map(7);

	auto print = [&](int value) {
		if (map.find(value))
			std::cout << "해시 맵에서 " << value << "을(를) 찾았습니다.";
		else
			std::cout << "해시 맵에서 " << value << "을(를) 찾지 못했습니다.";
		std::cout << std::endl;
	};

	map.insert(2);
	map.insert(25);
	map.insert(10);

	map.insert(100);
	map.insert(55);

	print(100);
	print(2);

	map.erase(2);
}
