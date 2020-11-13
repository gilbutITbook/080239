#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

void transform_test(std::vector<int> S)
{
	std::vector<int> Tr;
	std::cout << "[맵 테스트]" << std::endl;
	std::cout << "입력 배열, S: ";
	for (auto i : S)
		std::cout << i << " ";
	std::cout << std::endl;

	// std::transform() 함수 사용
	std::transform(S.begin(), S.end(), std::back_inserter(Tr), 
		[](int x) { return std::pow(x, 2.0); });

	std::cout << "std::transform(), Tr: ";
	for (auto i : Tr)
		std::cout << i << " ";
	std::cout << std::endl;

	// std::for_each() 함수 사용
	std::for_each(S.begin(), S.end(), 
		[](int& x) { x = std::pow(x, 2.0); });

	std::cout << "std::for_each(), S: ";
	for (auto i : S)
		std::cout << i << " ";
	std::cout << std::endl;
}

void reduce_test(std::vector<int> S)
{
	std::cout << std::endl << "[리듀스 테스트]" << std::endl;
	std::cout << "입력 배열, S: ";
	for (auto i : S)
		std::cout << i << " ";
	std::cout << std::endl;

	// std::accumulate() 함수 사용
	auto ans = std::accumulate(S.begin(), S.end(), 0,
		[](int acc, int x) { return acc + x; });
	std::cout << "std::accumulate(), ans: " << ans << std::endl;
}

int main()
{
	std::vector<int> S {1, 10, 4, 7, 3, 5, 6, 9, 8, 2};

	transform_test(S);
	reduce_test(S);
}
