#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iomanip>

#define DEBUG 0

#if DEBUG
#define PRINT(x) cerr << x
#else
#define PRINT(x)
#endif

using namespace std;

const int UNKNOWN = -1;

void GetAllSubsets(vector<int> set, vector<int> subset,
	int index, vector<vector<vector<int>>>& allSubsets)
{
	// 집합 set의 끝에 도달한 경우
	if (index == set.size())
	{
		// 부분집합 크기를 인덱스로 사용하여 부분집합을 allSubsets에 추가
		allSubsets[subset.size()].push_back(subset);
		return;
	}

	// 원소를 추가하지 않고 재귀 호출
	GetAllSubsets(set, subset, index + 1, allSubsets);

	// 원소를 부분집합에 추가한 후 재귀 호출
	subset.push_back(set[index]);
	GetAllSubsets(set, subset, index + 1, allSubsets);
}

bool SubsetSum_BruteForce(vector<int> set, int target)
{
	vector<vector<vector<int>>> allSubsets(set.size() + 1);

	GetAllSubsets(set, {}, 0, allSubsets);

	for (int size = 0; size <= set.size(); size++)
	{
		PRINT("부분집합의 원소 개수: " << size << endl);

		for (auto subset : allSubsets[size])
		{
			PRINT("\t{ ");

			int sum = 0;
			for (auto number : subset)
			{
				sum += number;
				PRINT(number << " ");
			}

			PRINT("} = " << sum << endl);

			if (sum == target)
				return true;
		}
	}

	return false;
}

bool SubsetSum_Backtracking(vector<int> set, int sum, int i)
{
	// 만약 현재 부분집합의 합이 target과 같다면
	if (sum == 0)
	{
		return true;
	}

	// 만약 현재 부분집합의 합이 target보다 크거나, 집합의 끝에 도달했다면
	if (i == set.size() || set[i] > sum)
	{
		return false;
	}

	// Case 1: sum에서 set[i]을 빼서 재귀 호출 (i번째 원소를 부분집합에 추가)
	// Case 2: sum을 그대로 전달하여 재귀 호출 (i번째 원소를 부분집합에 추가하지 않음)

	return SubsetSum_Backtracking(set, sum - set[i], i + 1)
		|| SubsetSum_Backtracking(set, sum, i + 1);
}

bool SubsetSum_Memoization(vector<int>& set, int sum, int i,
	vector<vector<int>>& memo)
{
	// 만약 현재 부분집합의 합이 target과 같다면
	if (sum == 0)
	{
		return true;
	}

	// 만약 현재 부분집합의 합이 target보다 크거나, 집합의 끝에 도달했다면
	if (i == set.size() || set[i] > sum)
	{
		return false;
	}

	// 현재 상태가 캐시에 있는지 확인
	if (memo[i][sum] == UNKNOWN)
	{
		// 현재 상태에 대한 솔루션을 구하여 캐시에 저장
		bool append = SubsetSum_Memoization(set, sum - set[i], i + 1, memo);
		bool ignore = SubsetSum_Memoization(set, sum, i + 1, memo);

		memo[i][sum] = append || ignore;
	}

	// 캐시에 저장된 값을 반환
	return memo[i][sum];
}

vector<vector<bool>> SubsetSum_Tabulation(vector<int>& set)
{
	int maxSum = 0;

	for (int i = 0; i < set.size(); i++)
	{
		maxSum += set[i];
	}

	vector<vector<bool>> DP(set.size() + 1, vector<bool>(maxSum + 1, 0));

	for (int i = 0; i < set.size(); i++)
	{
		DP[i][0] = true;
	}

	for (int i = 1; i <= set.size(); i++)
	{
		for (int sum = 1; sum <= maxSum; sum++)
		{
			if (sum < set[i - 1])
			{
				DP[i][sum] = DP[i - 1][sum];
			}
			else
			{
				DP[i][sum] = DP[i - 1][sum]
					|| DP[i - 1][sum - set[i - 1]];
			}
		}
	}

	return DP;
}

vector<string> types =
{
	"BRUTE FORCE",
	"BACKTRACKING",
	"MEMOIZATION",
	"TABULATION"
};

void GetTime(clock_t& timer, string type)
{
	// 현재 시간에서 timer를 빼서 경과 시간을 계산
	timer = clock() - timer;

	// 화면에 경과 시간 출력
	cout << type << " 방법 경과 시간: ";
	cout << fixed << setprecision(5) << (float)timer / CLOCKS_PER_SEC << endl;

	timer = clock(); // timer를 현재 시간으로 초기화
}

int main()
{
	vector<int> set = {16, 1058, 22, 13, 46, 55, 3, 92, 47, 7,
					   98, 367, 807, 106, 333, 85, 577, 9, 3059};
	int target = 6076;
	int tests = 4;

	sort(set.begin(), set.end());

	for (int i = 0; i < tests; i++)
	{
		bool found = false;

		clock_t timer = clock();

		switch (i)
		{
		case 0:
			found = SubsetSum_BruteForce(set, target);
			break;
		case 1:
			found = SubsetSum_Backtracking(set, target, 0);
			break;
		case 2:
		{
			// 메모이제이션 테이블 초기화
			vector<vector<int>> memo(set.size(), vector<int>(7000, UNKNOWN));

			found = SubsetSum_Memoization(set, target, 0, memo);
			break;
		}
		case 3:
		{
			int total = 0;
			for (auto number : set)
				total += number;

			vector<vector<bool>> DP = SubsetSum_Tabulation(set);
			found = DP[set.size()][target];

			vector<int> subsetSums;
			for (int sum = 1; sum <= total; sum++)
			{
				if (DP[set.size()][sum])
				{
					subsetSums.push_back(sum);
				}
			}

			cout << "다음과 같이 " << subsetSums.size() << "가지의 부분집합의 합이 가능합니다:" << endl;

			for (auto sum : subsetSums)
				cout << sum << " ";
			cout << endl;

			break;
		}
		}

		if (found)
		{
			cout << "원소 합이 " << target << "인 부분집합이 있습니다." << endl;
		}
		else
		{
			cout << "원소 합이 " << target << "인 부분집합이 없습니다." << endl;
		}

		GetTime(timer, types[i]);
		cout << endl;
	}
}

