#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>

#define DEBUG 1

#if DEBUG
#define PRINT(x) cerr << x
#else
#define PRINT(x)
#endif

using namespace std;

vector<vector<pair<int, int>>> found;

int LCS_BruteForce(string A, string B, int i, int j, 
	               vector<pair<int, int>> subsequence)
{
	// 만약 i가 A의 길이보다 커지거나 또는 j가 B의 길이보다 커지면:
	if (i >= A.size() || j >= B.size())
	{
		found.push_back(subsequence);

		// 재귀를 종료하고 부분 시퀀스의 길이를 반환합니다.
		return subsequence.size();
	}

	// 만약 A[i] = B[j] 이면:
	if (A[i] == B[j])
	{
		// 부분 시퀀스 길이를 1만큼 증가합니다.
		subsequence.push_back({i, j});

		// i와 j를 각각 1씩 증가합니다.
		return LCS_BruteForce(A, B, i + 1, j + 1, subsequence);
	}

	/* 그렇지 않으면:
	
	   옵션 1) (i + 1)번째와 j번째 문자에 대해 검사를 진행합니다.
	   옵션 2) i번째와 (j + 1)번째 문자에 대해 검사를 진행합니다.

	   이 상태의 LCS는 옵션 1 및 옵션 2의 최댓값과 같습니다.
	 */

	return max(LCS_BruteForce(A, B, i + 1, j, subsequence),
		LCS_BruteForce(A, B, i, j + 1, subsequence));
}

void PrintSubsequences(string A, string B)
{
	// 람다 함수를 이용한 정렬
	sort(found.begin(), found.end(), [](auto a, auto b)
		{
			// 부분 시퀀스 길이를 기준으로 정렬
			if (a.size() != b.size())
			{
				return a.size() < b.size();
			}

			// 두 부분 시퀀스 길이가 같다면 인덱스의 사전 순서로 정렬
			return a < b;
		});

	// 중복 제거
	found.erase(unique(found.begin(), found.end()), found.end());

	int previousSize = 0;

	for (auto subsequence : found)
	{
		if (subsequence.size() != previousSize)
		{
			previousSize = subsequence.size();
			PRINT("SIZE = " << previousSize << endl);
		}

		// 밑줄을 이용하여 문자의 자리를 표현
		string a_seq(A.size(), '_');
		string b_seq(B.size(), '_');

		for (auto pair : subsequence)
		{
			// 빈칸을 부분 문자열 문자로 채우기
			a_seq[pair.first] = A[pair.first];
			b_seq[pair.second] = B[pair.second];
		}

		PRINT("\t" << a_seq << " " << b_seq << endl);
	}
}

int main()
{
	string A, B;
	cin >> A >> B;

	int LCS = LCS_BruteForce(A, B, 0, 0, {});
	cout << A << "와 " << B << "의 최장 공통 부분 시퀀스 길이: " << LCS << endl;

#if DEBUG
	PrintSubsequences(A, B);
#endif
}

/*
ABCX ACYXB
*/