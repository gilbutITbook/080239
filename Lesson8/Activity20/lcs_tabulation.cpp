#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <time.h>
#include <iomanip>
#include <iostream>

#define DEBUG 0

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

const int UNKNOWN = -1;

int LCS_Memoization(string A, string B, int i, int j, 
	                vector<vector<int>>& memo)
{
	// 기저 조건 - 빈 문자열에 대해서는 0을 반환
	if (i == 0 || j == 0)
	{
		return 0;
	}

	// 두 문자열의 부분 문자열에 대해 결과가 저장되어 있으면 반환
	// Have we found a result for the prefixes of the two strings?
	if (memo[i - 1][j - 1] != UNKNOWN)
	{
		return memo[i - 1][j - 1];
	}

	// A와 B의 두 부분 문자열에서 맨 마지막 문자가 같은지 확인
	if (A[i - 1] == B[j - 1])
	{
		// 이 경우 A와 B의 부분 문자열에서 구한 LCS 길이는 각 부분 문자열에서 
		// 마지막 한 문자를 제외한 문자열로부터 구한 LCS 길이에 1을 더한 것과 같음

		memo[i - 1][j - 1] = 1 + LCS_Memoization(A, B, i - 1, j - 1, memo);

		// 테이블에 저장된 결과를 반환
		return memo[i - 1][j - 1];
	}

	// A와 B의 두 부분 문자열에서 맨 마지막 문자가 같지 않다면
	// A의 부분 문자열에서 마지막 문자를 제외한 것과 B의 부분 문자열에서 구한 LCS 길이,
	// B의 부분 문자열에서 마지막 문자를 제외한 것과 A의 부분 문자열에서 구한 LCS 길이 중
	// 최댓값을 선택하여 지정

	memo[i - 1][j - 1] = max(LCS_Memoization(A, B, i - 1, j, memo),
		LCS_Memoization(A, B, i, j - 1, memo));

	return memo[i - 1][j - 1];
}

string ReconstructLCS(vector<vector<int>> DP, string A, string B, int i, int j)
{
	if (i == 0 || j == 0)
	{
		return "";
	}

	if (A[i - 1] == B[j - 1])
	{
		return ReconstructLCS(DP, A, B, i - 1, j - 1) + A[i - 1];
	}
	else if (DP[i - 1][j] > DP[i][j - 1])
	{
		return ReconstructLCS(DP, A, B, i - 1, j);
	}
	else
	{
		return ReconstructLCS(DP, A, B, i, j - 1);
	}
}

string LCS_Tabulation(string A, string B)
{
	vector<vector<int>> DP(A.size() + 1, vector<int>(B.size() + 1));

	for (int i = 0; i <= A.size(); i++)
	{
		for (int j = 0; j <= B.size(); j++)
		{
			if (i == 0 || j == 0)
			{
				DP[i][j] = 0;
			}
			else if (A[i - 1] == B[j - 1])
			{
				DP[i][j] = DP[i - 1][j - 1] + 1;
			}
			else
			{
				DP[i][j] = max(DP[i - 1][j], DP[i][j - 1]);
			}
		}
	}

	string lcs = ReconstructLCS(DP, A, B, A.size(), B.size());

	return lcs;
}

vector<string> types =
{
	"BRUTE FORCE",
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
	string A, B;
	cin >> A >> B;

	int tests = 3;


	for (int i = 0; i < tests; i++)
	{
		int LCS;

		clock_t timer = clock();

		switch (i)
		{
		case 0:
		{
			LCS = LCS_BruteForce(A, B, 0, 0, {});

#if DEBUG
			PrintSubsequences(A, B);
#endif
			break;
		}
		case 1:
		{
			vector<vector<int>> memo(A.size(), vector<int>(B.size(), UNKNOWN));
			LCS = LCS_Memoization(A, B, A.size(), B.size(), memo);

			break;
		}
		case 2:
		{
			string lcs = LCS_Tabulation(A, B);

			LCS = lcs.size();

			cout << A << "와 " << B << "의 최장 공통 부분 시퀀스: " << lcs << endl;
			break;
		}
		}

		cout << A << "와 " << B << "의 최장 공통 부분 시퀀스 길이: " << LCS << endl;

		GetTime(timer, types[i]);
		cout << endl;
	}
}

/*
A1B2C3D4E ABCDE
5
ABCDE
 
ABZCYDABAZADAEA YABAZADBBEAAECYACAZ
10
YABAZADAEA

QWJEHFBEMVJEIIFJEEVFBEHFJXAJXBE BVBQHEJEJCHEEHXBNEBCHHEHHFEHSBE
14
QHEJEJEEBEHFBE
 
AAA12AAA3AA4AA56AA7AAA8 AA1AA2AAA3A4A5A6AA7A89AAA
19
AA12AAA3A4A56AA7AAA

 */

