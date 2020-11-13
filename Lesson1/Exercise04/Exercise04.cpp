#include <iostream>
#include <forward_list>
#include <vector>

int main()
{
	std::vector<std::string> vec = {
		"Lewis Hamilton", "Lewis Hamilton", "Nico Roseberg",
		"Sebastian Vettel", "Lewis Hamilton", "Sebastian Vettel",
		"Sebastian Vettel", "Sebastian Vettel", "Fernando Alonso"
	};

	auto it = vec.begin(); // 상수 시간
	std::cout << "가장 최근 우승자: " << *it << std::endl;

	it += 8; // 상수 시간
	std::cout << "8년전 우승자: " << *it << std::endl;

	advance(it, -3); // 상수 시간
	std::cout << "그후 3년 뒤 우승자: " << *it << std::endl;

	std::forward_list<std::string> fwd(vec.begin(), vec.end());

	auto it1 = fwd.begin();
	std::cout << "가장 최근 우승자: " << *it1 << std::endl;

	advance(it1, 5); // 선형 시간
	std::cout << "5년전 우승자: " << *it1 << std::endl;

	// std::forward_list는 순방향으로만 이동할 수 있으므로
	// 아래 코드는 에러가 발생합니다. 
	// advance(it1, -2);
}
