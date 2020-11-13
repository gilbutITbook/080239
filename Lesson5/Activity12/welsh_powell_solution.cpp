#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>

using namespace std;

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

// 그래프 컬러링에 사용할 색상 번호
unordered_map<unsigned, string> color_map = {
	{1, "Red"},
	{2, "Blue"},
	{3, "Green"},
	{4, "Yellow"},
	{5, "Black"},
	{6, "White"}
};

template <typename T>
auto welsh_powell_coloring(const Graph<T>& G)
{
	auto size = G.vertices();
	vector<pair<unsigned, size_t>> degrees;

	// 각 정점의 차수를 <정점 ID, 차수>의 쌍으로 취합
	for (unsigned i = 1; i < size; i++)
		degrees.push_back(make_pair(i, G.edges(i).size()));

	// 정점의 차수 기준으로 내림차순 정렬
	sort(degrees.begin(), degrees.end(), [](const auto& a, const auto& b) {
		return a.second > b.second;
		});

	cout << "[색상 지정 순서 (괄호는 차수)]" << endl;
	for (auto const i : degrees)
		cout << "" << i.first << " (" << i.second << ")" << endl;

	vector<unsigned> assigned_colors(size);
	auto color_to_be_assigned = 1;

	while (true)
	{
		for (auto const i : degrees)
		{
			// 이미 색칠이 칠해져 있으면 다음 정점을 검사
			if (assigned_colors[i.first] != 0)
				continue;

			auto outgoing_edges = G.edges(i.first);

			// i번째 정점과 인접해있는 정점들의 현재 색상
			set<unsigned> neighbours;

			for (auto& e : outgoing_edges)
			{
				neighbours.insert(assigned_colors[e.dst]);
			}

			// i번째 정점과 인접한 정점이 color_to_be_assigned 색상을 가지고 있지 않다면
			// i번재 정점에 color_to_be_assigned 색상을 지정
			if (neighbours.find(color_to_be_assigned) == neighbours.end())
				assigned_colors[i.first] = color_to_be_assigned;
		}

		color_to_be_assigned++;

		// 모든 정점에 색칠이 칠해졌으면 종료
		if (find(assigned_colors.begin() + 1, assigned_colors.end(), 0) ==
			assigned_colors.end())
			break;
	}

	return assigned_colors;
}

template <typename T>
void print_colors(vector<T>& colors)
{
	for (auto i = 1; i < colors.size(); i++)
	{
		cout << i << ": " << color_map[colors[i]] << endl;
	}
}

int main()
{
	using T = unsigned;

	// 그래프 객체 생성
	Graph<T> G(9);

	map<unsigned, vector<pair<unsigned, T>>> edge_map;
	edge_map[1] = {{2, 0}, {5, 0}};
	edge_map[2] = {{1, 0}, {5, 0}, {4, 0}};
	edge_map[3] = {{4, 0}, {7, 0}};
	edge_map[4] = {{2, 0}, {3, 0}, {5, 0}, {6, 0}, {8, 0}};
	edge_map[5] = {{1, 0}, {2, 0}, {4, 0}, {8, 0}};
	edge_map[6] = {{4, 0}, {7, 0}, {8, 0}};
	edge_map[7] = {{3, 0}, {6, 0}};
	edge_map[8] = {{4, 0}, {5, 0}, {6, 0}};

	for (auto& i : edge_map)
		for (auto& j : i.second)
			G.add_edge(Edge<T>{ i.first, j.first, j.second });

	cout << "[입력 그래프]" << endl;
	cout << G << endl;

	auto colors = welsh_powell_coloring<T>(G);
	cout << endl << "[그래프 컬러링]" << endl;
	print_colors(colors);
}