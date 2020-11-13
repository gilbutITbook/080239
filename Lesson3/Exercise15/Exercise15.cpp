#include <iostream>
#include <vector>

class hash_map
{
	std::vector<int> data1;
	std::vector<int> data2;
	int size;

	int hash1(int key) const
	{
		return key % size;
	}

	int hash2(int key) const
	{
		return (key / size) % size;
	}

public:
	hash_map(int n) : size(n)
	{
		data1 = std::vector<int>(size, -1);
		data2 = std::vector<int>(size, -1);
	}

	std::vector<int>::iterator lookup(int key)
	{
		auto hash_value1 = hash1(key);
		if (data1[hash_value1] == key)
		{
			std::cout << "1번 테이블에서 " << key << "을(를) 찾았습니다." << std::endl;
			return data1.begin() + hash_value1;
		}

		auto hash_value2 = hash2(key);
		if (data2[hash_value2] == key)
		{
			std::cout << "2번 테이블에서 " << key << "을(를) 찾았습니다." << std::endl;
			return data2.begin() + hash_value2;
		}

		return data2.end();
	}

	void erase(int key)
	{
		auto position = lookup(key);
		if (position != data2.end())
		{
			*position = -1;
			std::cout << key << "에 해당하는 원소를 삭제했습니다." << std::endl;
		}
		else
		{
			std::cout << key << "키를 찾지 못했습니다." << std::endl;
		}
	}

	void insert(int key)
	{
		insert_impl(key, 0, 1);
	}

	void insert_impl(int key, int cnt, int table)
	{
		if (cnt >= size)
		{
			std::cout << key << " 삽입 시 사이클 발생! 재해싱이 필요합니다!" << std::endl;
			return;
		}

		if (table == 1)
		{
			int hash = hash1(key);
			if (data1[hash] == -1)
			{
				std::cout << table << "번 테이블에 " << key << " 삽입" << std::endl;
				data1[hash] = key;
			}
			else
			{
				int old = data1[hash];
				data1[hash] = key;
				std::cout << table << "번 테이블에 " << key << " 삽입: 기존의 " << old << " 이동 -> ";
				insert_impl(old, cnt + 1, 2);
			}
		}
		else
		{
			int hash = hash2(key);
			if (data2[hash] == -1)
			{
				std::cout << table << "번 테이블에 " << key << " 삽입" << std::endl;
				data2[hash] = key;
			}
			else
			{
				int old = data2[hash];
				data2[hash] = key;
				std::cout << table << "번 테이블에 " << key << " 삽입: 기존의 " << old << " 이동 -> ";
				insert_impl(old, cnt + 1, 1);
			}
		}
	}

	void print()
	{
		std::cout << "Index: ";
		for (int i = 0; i < size; i++)
			std::cout << i << '\t';
		std::cout << std::endl;

		std::cout << "Data1: ";
		for (auto i : data1)
			std::cout << i << '\t';
		std::cout << std::endl;

		std::cout << "Data2: ";
		for (auto i : data2)
			std::cout << i << '\t';
		std::cout << std::endl;
	}
};

int main()
{
	hash_map map(7);
	map.print();
	std::cout << std::endl;

	map.insert(10);
	map.insert(20);
	map.insert(30);
	std::cout << std::endl;

	map.insert(104);
	map.insert(2);
	map.insert(70);
	map.insert(9);
	map.insert(90);
	map.insert(2);
	map.insert(7);
	std::cout << std::endl;

	map.print();
	std::cout << std::endl;

	map.insert(14); // 사이클 발생!
}
