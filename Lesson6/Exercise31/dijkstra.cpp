#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

template <typename T>
struct Edge
{
	unsigned src;
	unsigned dst;
	T weight;
};

template <typename T>
class Graph
{
public:
	// N개의 정점으로 구성된 그래프
	Graph(unsigned N) : V(N) {}

	// 그래프의 정점 개수 반환
	auto vertices() const { return V; }

	// 전체 에지 리스트 반환
	auto& edges() const { return edge_list; }

	// 정점 v에서 나가는 모든 에지를 반환
	auto edges(unsigned v) const
	{
		vector<Edge<T>> edges_from_v;
		for (auto& e : edge_list)
		{
			if (e.src == v)
				edges_from_v.emplace_back(e);
		}

		return edges_from_v;
	}

	void add_edge(Edge<T>&& e)
	{
		// 에지 양 끝 정점 ID가 유효한지 검사
		if (e.src >= 1 && e.src <= V && e.dst >= 1 && e.dst <= V)
			edge_list.emplace_back(e);
		else
			cerr << "에러: 유효 범위를 벗어난 정점!" << endl;
	}

	// 표준 출력 스트림 지원
	template <typename U>
	friend ostream& operator<< (ostream& os, const Graph<U>& G);

private:
	unsigned V;		// 정점 개수
	vector<Edge<T>> edge_list;
};

template <typename U>
ostream& operator<< (ostream& os, const Graph<U>& G)
{
	for (unsigned i = 1; i < G.vertices(); i++)
	{
		os << i << ":\t";

		auto edges = G.edges(i);
		for (auto& e : edges)
			os << "{" << e.dst << ": " << e.weight << "}, ";

		os << endl;
	}

	return os;
}

template <typename T>
auto create_reference_graph()
{
	Graph<T> G(9);

	map<unsigned, vector<pair<unsigned, T>>> edge_map;
	edge_map[1] = { {2, 2}, {5, 3} };
	edge_map[2] = { {1, 2}, {5, 5}, {4, 1} };
	edge_map[3] = { {4, 2}, {7, 3} };
	edge_map[4] = { {2, 1}, {3, 2}, {5, 2}, {6, 4}, {8, 5} };
	edge_map[5] = { {1, 3}, {2, 5}, {4, 2}, {8, 3} };
	edge_map[6] = { {4, 4}, {7, 4}, {8, 1} };
	edge_map[7] = { {3, 3}, {6, 4} };
	edge_map[8] = { {4, 5}, {5, 3}, {6, 1} };

	for (auto& i : edge_map)
		for (auto& j : i.second)
			G.add_edge(Edge<T>{ i.first, j.first, j.second });

	return G;
}

template <typename T>
struct Label
{
	unsigned ID;
	T distance;

	// Label 객체 비교는 거리(distance) 값을 이용
	inline bool operator> (const Label<T>& l) const
	{
		return this->distance > l.distance;
	}
};

template <typename T>
auto dijkstra_shortest_path(const Graph<T>& G, unsigned src, unsigned dst)
{
	// 최소 힙
	priority_queue<Label<T>, vector<Label<T>>, greater<Label<T>>> heap;

	// 모든 정점에서 거리 값을 최대로 설정
	vector<T> distance(G.vertices(), numeric_limits<T>::max());

	set<unsigned> visited;					// 방문한 정점
	vector<unsigned> parent(G.vertices());	// 이동 경로를 기억을 위한 벡터

	heap.emplace(Label<T>{src, 0});
	parent[src] = src;

	while (!heap.empty())
	{
		auto current_vertex = heap.top();
		heap.pop();

		// 목적지 정점에 도착했다면 종료
		if (current_vertex.ID == dst)
		{
			cout << current_vertex.ID << "번 정점(목적 정점)에 도착!" << endl;
			break;
		}

		// 현재 정점을 이전에 방문하지 않았다면
		if (visited.find(current_vertex.ID) == visited.end())
		{
			cout << current_vertex.ID << "번 정점에 안착!" << endl;

			// 현재 정점과 연결된 모든 에지에 대해
			for (auto& e : G.edges(current_vertex.ID))
			{
				auto neighbor = e.dst;
				auto new_distance = current_vertex.distance + e.weight;

				// 인접한 정점의 거리 값이 새로운 경로에 의한 거리 값보다 크면
				// 힙에 추가하고, 거리 값을 업데이트함.
				if (new_distance < distance[neighbor])
				{
					heap.emplace(Label<T>{neighbor, new_distance});

					parent[neighbor] = current_vertex.ID;
					distance[neighbor] = new_distance;
				}
			}

			visited.insert(current_vertex.ID);
		}
	}

	// 백트래킹 방식으로 시작 정점부터 목적 정점까지의 경로 구성
	vector<unsigned> shortest_path;
	auto current_vertex = dst;

	while (current_vertex != src)
	{
		shortest_path.push_back(current_vertex);
		current_vertex = parent[current_vertex];
	}

	shortest_path.push_back(src);
	reverse(shortest_path.begin(), shortest_path.end());

	return shortest_path;
}

int main()
{
	using T = unsigned;

	// 그래프 객체 생성
	auto G = create_reference_graph<T>();
	cout << "[입력 그래프]" << endl;
	cout << G << endl;

	auto shortest_path = dijkstra_shortest_path<T>(G, 1, 6);

	cout << endl << "[1번과 6번 정점 사이의 최단 경로]" << endl;
	for (auto v : shortest_path)
		cout << v << " ";
	cout << endl;
}
