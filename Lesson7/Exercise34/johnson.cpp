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

bool HasNegativeCycle(const vector<Edge>& edges, vector<int> distance)
{
	for (auto& e : edges)
	{
		if (distance[e.src] == UNKNOWN)
			continue;

		if (distance[e.dst] > distance[e.src] + e.weight)
			return true;
	}

	return false;
}

vector<int> BellmanFord(vector<Edge> edges, int V)
{
	vector<int> distance(V + 1, UNKNOWN);

	int s = V;
	for (int i = 0; i < V; i++)
	{
		edges.push_back(Edge {s, i, 0});
	}

	distance[s] = 0;

	// 정점 개수가 V + 1개 이므로 V번 반복
	for (int i = 0; i < V; i++)
	{
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
	if (HasNegativeCycle(edges, distance))
	{
		cout << "음수 가중치 사이클 발견!" << endl;
		return {};
	}

	return distance;
}

int GetMinDistance(vector<int>& distance, vector<bool>& visited)
{
	int minDistance = UNKNOWN;
	int minIndex = -1;

	for (int i = 0; i < distance.size(); i++)
	{
		if (!visited[i] && distance[i] <= minDistance)
		{
			minDistance = distance[i];
			minIndex = i;
		}
	}

	return minIndex;
}

vector<int> Dijkstra(vector<Edge> edges, int V, int start)
{
	vector<int> distance(V, UNKNOWN);
	vector<bool> visited(V, false);

	distance[start] = 0;

	for (int i = 0; i < V - 1; i++)
	{
		// 방문하지 않은 정점 중에서 최소 거리 정점을 찾음
		int curr = GetMinDistance(distance, visited);

		visited[curr] = true;

		for (auto& e : edges)
		{
			// 인접한 정점만 고려
			if (e.src != curr)
				continue;

			// 이미 방문했으면 무시
			if (visited[e.dst])
				continue;

			if (distance[curr] != UNKNOWN &&
				distance[e.dst] > distance[curr] + e.weight)
			{
				distance[e.dst] = distance[curr] + e.weight;
			}
		}
	}

	return distance;
}

void Johnson(vector<Edge> edges, int V)
{
	// 더미 정점을 추가한 그래프에서 최단 거리를 계산
	vector<int> h = BellmanFord(edges, V);

	if (h.empty())
		return;

	// 에지 가중치 재설정
	for (auto& e : edges)
	{
		e.weight += (h[e.src] - h[e.dst]);
	}

	// 모든 정점들 사이의 최단 거리를 저장
	vector<vector<int>> shortest(V);

	for (int i = 0; i < V; i++)
	{
		shortest[i] = Dijkstra(edges, V, i);
	}

	// 가중치 변환 수식을 역으로 적용하여 최단 거리를 출력
	for (int i = 0; i < V; i++)
	{
		cout << i << ":\n";

		for (int j = 0; j < V; j++)
		{
			if (shortest[i][j] != UNKNOWN)
			{
				shortest[i][j] += h[j] - h[i];

				cout << "\t" << j << ": " << shortest[i][j] << endl;
			}
		}
	}
}

int main()
{
	int V = 5;              // 정점 개수
	vector<Edge> edges;     // 에지 포인터의 벡터

	vector<vector<int>> edge_map { // {시작 정점, 목표 정점, 가중치}
		{0, 1, -7},
		{1, 2, -2},
		{2, 0, 10},
		{0, 3, -5},
		{0, 4, 2},
		{3, 4, 4}
	};

	for (auto& e : edge_map)
	{
		edges.emplace_back(Edge {e[0], e[1], e[2]});
	}

	Johnson(edges, V);
}
