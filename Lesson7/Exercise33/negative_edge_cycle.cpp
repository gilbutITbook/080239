#include <vector>
#include <iostream>
#include <climits>

using namespace std;

struct Edge
{
	int src;
	int dst;
	int weight;
};

const int UNKNOWN = INT_MAX;

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
			cout << "음수 가중치 사이클 발견!" << endl;
			return {};
		}
	}

	return distance;
}

int main()
{
	int V = 6;              // 정점 개수
	vector<Edge> edges;     // 에지 포인터의 벡터

	vector<vector<int>> edge_map { // {시작 정점, 목표 정점, 가중치}
		{0, 1, 3},
		{1, 3, -8},
		{2, 1, 3},
		{2, 5, 5},
		{3, 2, 3},
		{2, 4, 2},
		{4, 5, -1},
		{5, 1, 8}
	};

	for (auto& e : edge_map)
	{
		edges.emplace_back(Edge {e[0], e[1], e[2]});
	}

	int start = 0;
	vector<int> distance = BellmanFord(edges, V, start);

	if (!distance.empty())
	{
		cout << "[" << start << "번 정점으로부터 최소 거리]" << endl;

		for (int i = 0; i < distance.size(); i++)
		{
			if (distance[i] == UNKNOWN)
				cout << i << "번 정점: 방문하지 않음!" << endl;
			else
				cout << i << "번 정점: " << distance[i] << endl;
		}
	}
}
