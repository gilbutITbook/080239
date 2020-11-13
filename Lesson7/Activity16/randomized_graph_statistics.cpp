#include <vector>
#include <iostream>
#include <climits>
#include <iomanip>
#include <algorithm>
#include <queue>

using namespace std;

struct Edge
{
	int src;
	int dst;
	int weight;
};

const int UNKNOWN = 1e9;

static unsigned long int next_random = 1;

int rand(void)
{
	next_random = next_random * 1103515245 + 12345;
	return (unsigned int)(next_random / 65536) % 32768;
}

void srand(unsigned int seed)
{
	next_random = seed;
}

enum RESULT
{
	VALID,
	INVALID,
	INTERESTING
};

struct Graph
{
	int V, E;
	int maxWeight = -UNKNOWN;

	vector<Edge> edges;
	vector<vector<int>> adj;
	vector<vector<int>> weight;

	Graph(int v, int e) : V(v), E(e)
	{
		vector<vector<bool>> used(V, vector<bool>(V, false));

		adj.resize(V);
		weight.resize(V, vector<int>(V, UNKNOWN));

		while (e)
		{
			// 에지 정보 생성
			int u = rand() % V;
			int v = rand() % V;
			int w = rand() % 100;

			if (rand() % 3 == 0)
			{
				w = -w;
			}

			// 유효한 에지인지 확인
			if (u == v || used[u][v])
				continue;

			// 에지 정보를 추가하고 used 배열 값을 설정
			edges.push_back(Edge {u, v, w});
			adj[u].push_back(v);
			weight[u][v] = w;
			maxWeight = max(maxWeight, w);

			used[u][u] = used[v][v] = used[u][v] = used[v][u] = true;
			e--;
		}

		for (int i = 0; i < V; i++)
		{
			// 유효하지 않은 그래프에 대해 V 값을 -1로 설정
			if (!used[i][i])
			{
				V = -1;
				break;
			}
		}
	}
};

vector<int> BellmanFord(Graph G)
{
	vector<int> distance(G.V + 1, UNKNOWN);

	int s = G.V;

	for (int i = 0; i < G.V; i++)
	{
		G.edges.push_back(Edge {s, i, 0});
	}

	distance[s] = 0;

	// 정점 개수가 V + 1개 이므로 V번 반복
	for (int i = 0; i <= G.V; i++)
	{
		for (auto& e : G.edges)
		{
			if (distance[e.src] == UNKNOWN)
				continue;

			if (distance[e.dst] > distance[e.src] + e.weight)
			{
				distance[e.dst] = distance[e.src] + e.weight;
			}
		}
	}

	for (auto& e : G.edges)
	{
		if (distance[e.src] == UNKNOWN)
			continue;

		if (distance[e.dst] > distance[e.src] + e.weight)
		{
			return {};
		}
	}

	return distance;
}

vector<int> Dijkstra(int src, Graph G)
{
	using State = pair<int, int>;  // {distance, id}

	priority_queue<State, vector<State>, greater<State>> heap;
	vector<bool> visited(G.V, false);
	vector<int> distance(G.V, UNKNOWN);

	heap.push({0, src});
	distance[src] = 0;

	while (!heap.empty())
	{
		State top = heap.top();
		heap.pop();

		int dist = top.first;
		int node = top.second;

		visited[node] = true;

		for (auto next : G.adj[node])
		{
			if (visited[next])
			{
				continue;
			}
			
			if (dist != UNKNOWN && distance[next] > dist + G.weight[node][next])
			{
				distance[next] = dist + G.weight[node][next];

				heap.push({distance[next], next});
			}
		}
	}
	
	return distance;
}

RESULT TestGraph(Graph G)
{
	if (G.V == -1)
		return INVALID;

	vector<int> distance = BellmanFord(G);

	if (distance.empty())
		return VALID;

	for (auto& e : G.edges)
	{
		G.weight[e.src][e.dst] += (distance[e.src] - distance[e.dst]);
	}

	double result = 0;

	for (int i = 0; i < G.V; i++)
	{
		vector<int> shortest = Dijkstra(i, G);

		double average = 0;
		int count = 0;

		for (int j = 0; j < G.V; j++)
		{
			if (i == j || shortest[j] == UNKNOWN)
				continue;

			shortest[j] += (distance[j] - distance[i]);
			average += shortest[j];
			count++;
		}

		average = average / count;
		result += average;
	}

	result = result / G.V;

	double ratio = result / G.maxWeight;

	return (ratio < 0.5) ? INTERESTING : VALID;
}

int main()
{
	long seed;
	int iterations, V, E;

	cin >> seed;
	cin >> iterations;
	cin >> V >> E;

	int invalid = 0;
	int valid = 0;
	int interesting = 0;

	srand(seed);

	while (iterations--)
	{
		Graph G(V, E);

		switch (TestGraph(G))
		{
		case INVALID: invalid++; break;
		case VALID: valid++; break;
		case INTERESTING:
		{
			valid++;
			interesting++;
			break;
		}
		}
	}

	double percentInteresting = (double)interesting / valid * 100;

	cout << "유효하지 않은 그래프 개수: " << invalid << endl;
	cout << "흥미로운 그래프 생성 비율: " << fixed << setprecision(2) << percentInteresting << "%" << endl;

	return 0;
}

/*
42 1000 15 10
유효하지 않은 그래프 개수: 996
흥미로운 그래프 생성 비율: 0.00%

11111 400 5 5
유효하지 않은 그래프 개수: 55
흥미로운 그래프 생성 비율: 0.58%
*/
