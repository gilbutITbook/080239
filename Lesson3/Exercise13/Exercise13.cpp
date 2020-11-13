#include <iostream>
#include <vector>

using uint = unsigned int;

class hash_map
{
	std::vector<int> data;

public:
	hash_map(size_t n)
	{
		data = std::vector<int>(n, -1);
	}

	void insert(uint value)
	{
		int n = data.size();
		data[value % n] = value;
		std::cout << value << "을(를) 삽입했습니다." << std::endl;
	}

	bool find(uint value)
	{
		int n = data.size();
		return (data[value % n] == value);
	}

	void erase(uint value)
	{
		int n = data.size();
		if (data[value % n] == value)
		{
			data[value % n] = -1;
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
	print(25);

	map.insert(100);
	print(100);
	print(2);

	map.erase(25);
}
