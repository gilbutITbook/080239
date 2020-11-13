#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

class SimpleDisjointSet
{
private:
	struct Node
	{
		unsigned id;
		unsigned rank;
		unsigned parent;

		Node(unsigned _id) : id(_id), rank(0), parent(_id) {}

		bool operator!= (const Node& n) const
		{
			return this->id != n.id;
		}
	};

	// 디스조인트-셋 포레스트
	vector<Node> nodes; 

public:
	SimpleDisjointSet(unsigned N)
	{
		nodes.reserve(N);
	}

	void make_set(const unsigned& x)
	{
		nodes.emplace_back(x);
	}

	unsigned find(unsigned x)
	{
		auto node_it = find_if(nodes.begin(), nodes.end(),
			[x](auto n) { return n.id == x; });
		unsigned node_id = (*node_it).id;

		while (node_id != nodes[node_id].parent)
		{
			node_id = nodes[node_id].parent;
		}

		return node_id;
	}

	void union_sets(unsigned x, unsigned y)
	{
		auto root_x = find(x);
		auto root_y = find(y);

		// 만약 X와 Y가 같은 트리에 있다면 그대로 종료
		if (root_x == root_y)
			return;

		// 작은 랭크의 트리를 큰 랭크의 트리 쪽으로 병합
		if (nodes[root_x].rank > nodes[root_y].rank)
			swap(root_x, root_y);

		nodes[root_x].parent = nodes[root_y].parent;
		nodes[root_y].rank++;
	}
};

template <typename T>
struct Edge
{
	unsigned src;
	unsigned dst;
	T weight;

	// Edge 객체 비교는 가중치를 이용
	inline bool operator< (const Edge<T>& e) const
	{
		return this->weight < e.weight;
	}

	inline bool operator> (const Edge<T>& e) const
	{
		return this->weight > e.weight;
	}
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

// 트리도 그래프로 표현할 수 있으므로 최소 신장 트리도 Graph 객체로 반환합니다.
// 다만 여기에는 사이클이 있으면 안됩니다.
template <typename T>
Graph<T> minimum_spanning_tree(const Graph<T>& G)
{
	// 에지 가중치를 이용한 최소 힙 구성
	priority_queue<Edge<T>, vector<Edge<T>>, greater<Edge<T>>> edge_min_heap;

	// 모든 에지를 최소 힙에 추가
	for (auto& e : G.edges())
		edge_min_heap.push(e);

	// 정점 개수에 해당하는 크기의 디스조인트-셋 자료 구조 생성 및 초기화
	auto N = G.vertices();
	SimpleDisjointSet dset(N);
	for (unsigned i = 0; i < N; i++)
		dset.make_set(i);

	// 디스조인트-셋 자료 구조를 이용하여 최소 신장 트리 구하기
	Graph<T> MST(N);
	while (!edge_min_heap.empty())
	{
		// 최소 힙에서 최소 가중치 에지를 추출
		auto e = edge_min_heap.top();
		edge_min_heap.pop();

		// 선택한 에지가 사이클을 생성하지 않으면 해당 에지를 MST에 추가
		if (dset.find(e.src) != dset.find(e.dst))
		{
			MST.add_edge(Edge <T>{e.src, e.dst, e.weight});
			dset.union_sets(e.src, e.dst);
		}
	}

	return MST;
}

int main()
{
	using T = unsigned;

	// 그래프 객체 생성
	Graph<T> G(9);

	map<unsigned, vector<pair<unsigned, T>>> edge_map;
	edge_map[1] = {{2, 2}, {5, 3}};
	edge_map[2] = {{1, 2}, {5, 5}, {4, 1}};
	edge_map[3] = {{4, 2}, {7, 3}};
	edge_map[4] = {{2, 1}, {3, 2}, {5, 2}, {6, 4}, {8, 5}};
	edge_map[5] = {{1, 3}, {2, 5}, {4, 2}, {8, 3}};
	edge_map[6] = {{4, 4}, {7, 4}, {8, 1}};
	edge_map[7] = {{3, 3}, {6, 4}};
	edge_map[8] = {{4, 5}, {5, 3}, {6, 1}};

	for (auto& i : edge_map)
		for (auto& j : i.second)
			G.add_edge(Edge<T>{ i.first, j.first, j.second });

	cout << "[입력 그래프]" << endl;
	cout << G << endl;

	Graph<T> MST = minimum_spanning_tree(G);
	cout << "[최소 신장 트리]" << endl;
	cout << MST;
}