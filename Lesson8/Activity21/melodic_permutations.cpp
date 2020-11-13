#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

int CountMelodicPermutations(vector<int> melody, vector<int> set)
{
	unsigned int target = 0;

	for (auto note : set)
	{
		target |= note;
	}

	vector<vector<int>> DP(melody.size() + 1, vector<int>(4096, 0));

	// 기저 조건 -> 공집합
	DP[0][0] = 1;

	for (int i = 1; i <= melody.size(); i++)
	{
		for (unsigned int subset = 0; subset < 4096; subset++)
		{
			// (i-1)에서의 결과 더하기
			DP[i][subset] += DP[i - 1][subset];

			// melody[i-1]를 포함한 부분집합을 고려
			DP[i][subset | melody[i - 1]] += DP[i - 1][subset];
		}
	}

	// 최종 해답
	return DP[melody.size()][target];
}

vector<int> ConvertNotes(vector<string> notes)
{
	map<string, int> M =
	{
		{"A", 0},
		{"A#", 1},
		{"Bb", 1},
		{"B", 2},
		{"Cb", 2},
		{"B#", 3},
		{"C", 3},
		{"C#", 4},
		{"Db", 4},
		{"D", 5},
		{"D#", 6},
		{"Eb", 6},
		{"E", 7},
		{"Fb", 7},
		{"E#", 8},
		{"F", 8},
		{"F#", 9},
		{"Gb", 9},
		{"G", 10},
		{"G#", 11},
		{"Ab", 11}
	};

	vector<int> converted;

	for (auto note : notes)
	{
		converted.push_back(1 << M[note]); // 2의 승수로 매핑
	}

	return converted;
}

int main()
{
	int melodyLength;
	int setLength;

	cin >> melodyLength;

	vector<string> melody(melodyLength);

	for (int i = 0; i < melodyLength; i++)
	{
		cin >> melody[i];
	}

	cin >> setLength;

	vector<string> set(setLength);

	for (int i = 0; i < setLength; i++)
	{
		cin >> set[i];
	}

	int count = CountMelodicPermutations(ConvertNotes(melody), ConvertNotes(set));

	cout << count << endl;

	return 0;
}

/*
 7
 A B C C E C A
 3
 A C E
*/