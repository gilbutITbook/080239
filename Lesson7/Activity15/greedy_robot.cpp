#include <vector>
#include <iostream>
#include <climits>
#include <fstream>

using namespace std;

struct Edge
{
	int src;
	int dst;
	int weight;
};

const int UNKNOWN = INT_MAX;

bool ReadTestCase(string filename, int& N, vector<Edge>& edges)
{
	ifstream infile(filename);

	if (!infile.is_open())
	{
		cout << "테스트 케이스 파일을 열 수 없습니다!" << endl;
		return false;
	}

	infile >> N;

	for (int i = 0; i < N * N - 1; i++)
	{
		string directions;
		int power;

		infile >> directions >> power;

		int next = i;

		for (auto d : directions)
		{

			switch (d)
			{
			case 'N': next = i - N; break;
			case 'E': next = i + 1; break;
			case 'S': next = i + N; break;
			case 'W': next = i - 1; break;
			}

			// power 값의 부호를 바꿔서 에지 가중치로 사용
			edges.push_back(Edge {i, next, -power});
		}
	}

	return true;
}

vector<int> BellmanFord(vector<Edge> edges, int V, int start)
{
	vector<int> distance(V, UNKNOWN);
	distance[start] = 0;

	// (V - 1)번 반복
	for (int i = 0; i < V - 1; i++)
	{
		// 전체 에지에 대해 반복
		for (auto& e : edges)
		{
			// 에지의 시작 정점의 거리 값이 UNKNOWN이면 스킵
			if (distance[e.src] == UNKNOWN)
				continue;

			// 인접한 정점의 거리 값이 새로운 경로에 의한 거리 값보다 크면
			// 거리 값을 업데이트함.
			if (distance[e.dst] > distance[e.src] + e.weight)
			{
				distance[e.dst] = distance[e.src] + e.weight;
			}
		}
	}

	// 음수 가중치 사이클이 있는 지 검사
	for (auto& e : edges)
	{
		if (distance[e.src] == UNKNOWN)
			continue;

		if (distance[e.dst] > distance[e.src] + e.weight)
		{
			//cout << "음수 가중치 사이클 발견!" << endl;
			return {};
		}
	}

	return distance;
}

int main()
{
	int N;
	vector<Edge> edges;     // 에지 리스트

	// testcase1~5.txt 파일로부터 테스트 입력을 받아 결과 확인
	if (ReadTestCase("testcase1.txt", N, edges))
	{
		vector<int> distance = BellmanFord(edges, N * N, 0);

		if (distance.empty() || distance[N * N - 1] == UNKNOWN)
			cout << "탐색 중단" << endl;
		else
			cout << -1 * distance[N * N - 1] << endl;
	}
}
