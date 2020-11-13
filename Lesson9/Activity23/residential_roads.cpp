#include <vector>
#include <iostream>

using namespace std;

const int UNKNOWN = 1e9;
const char EMPTY_SPACE = '.';
const string roads = "-|/\\";

struct Point
{
	int x;
	int y;

	Point() : x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}

	bool operator !=(const Point& other) const
	{
		return x != other.x || y != other.y;
	}
};

int H, W;
int N;

vector<string> grid;
vector<vector<int>> terrain;
vector<vector<int>> cost;
vector<Point> houses;

int GetDirection(int x_dir, int y_dir)
{
	if (y_dir == 0) return 0;
	if (x_dir == 0) return 1;
	if (x_dir == -1)
	{
		return (y_dir == 1) ? 2 : 3;
	}
	return (y_dir == 1) ? 3 : 2;
}

void DrawPath(int start, int end)
{
	Point a = houses[start];
	Point b = houses[end];

	int x_dir = 0;
	int y_dir = 0;

	if (a.x != b.x)
	{
		x_dir = (a.x < b.x) ? 1 : -1;
	}

	if (a.y != b.y)
	{
		y_dir = (a.y < b.y) ? 1 : -1;
	}

	int direction = GetDirection(x_dir, y_dir);
	char mark = roads[direction];

	do {
		a.x += x_dir;
		a.y += y_dir;

		if (grid[a.y][a.x] == EMPTY_SPACE)
		{
			grid[a.y][a.x] = mark;
		}
		else if (!isalpha(grid[a.y][a.x]))
		{
			// 만약 두 도로가 교차하면 '+' 문자로 대체
			grid[a.y][a.x] = (mark != grid[a.y][a.x]) ? '+' : mark;
		}
	} while (a != b);
}

vector<int> GetPath(int start, int end, vector<vector<int>>& next)
{
	vector<int> path = {start};

	do {
		start = next[start][end];

		path.push_back(start);
	} while (next[start][end] != end);

	return path;
}

void GetShortestPaths()
{
	vector<vector<int>> dist(N, vector<int>(N, UNKNOWN));
	vector<vector<int>> next(N, vector<int>(N, UNKNOWN));

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			dist[i][j] = cost[i][j];

			if (dist[i][j] != UNKNOWN)
			{
				next[i][j] = j;
			}
		}

		dist[i][i] = 0;
		next[i][i] = i;
	}

	for (int mid = 0; mid < N; mid++)
	{
		for (int start = 0; start < N; start++)
		{
			for (int end = 0; end < N; end++)
			{
				if (dist[start][end] > dist[start][mid] + dist[mid][end])
				{
					dist[start][end] = dist[start][mid] + dist[mid][end];
					next[start][end] = next[start][mid];
				}
			}
		}
	}

	for (int i = 0; i < N; i++)
	{
		vector<int> path = GetPath(i, N - 1, next);

		int curr = i;

		for (auto neighbor : path)
		{
			DrawPath(curr, neighbor);
			curr = neighbor;
		}
	}
}

bool DirectLine(Point a, Point b)
{
	if (a.x == b.x) return true;
	if (a.y == b.y) return true;
	if (abs(a.x - b.x) == abs(a.y - b.y)) return true;

	return false;
}

int GetCost(int start, int end)
{
	Point a = houses[start];
	Point b = houses[end];

	int x_dir = 0;
	int y_dir = 0;

	if (a.x != b.x)
	{
		x_dir = (a.x < b.x) ? 1 : -1;
	}

	if (a.y != b.y)
	{
		y_dir = (a.y < b.y) ? 1 : -1;
	}

	int cost = 0;

	do {
		a.x += x_dir;
		a.y += y_dir;

		cost += terrain[a.y][a.x];
	} while (grid[a.y][a.x] == EMPTY_SPACE);

	return (a != b) ? UNKNOWN : cost;
}

void BuildGraph()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (i == j) continue;

			// 두 집이 수평, 수직, 대각 위치에 있는지 확인
			if (DirectLine(houses[i], houses[j]))
			{
				// 두 집 사이의 도로 건설에 필요한 비용 계산
				cost[i][j] = cost[j][i] = GetCost(i, j);
			}
		}
	}
}

void Input()
{
	cin >> H >> W;
	cin >> N;

	grid.resize(H);
	houses.resize(N);
	terrain.resize(H, vector<int>(W, UNKNOWN));
	cost.resize(N, vector<int>(N, UNKNOWN));

	// 지도 정보
	for (int i = 0; i < H; i++)
	{
		cin >> grid[i];
	}

	// 지반 강도 정보
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			cin >> terrain[i][j];
		}
	}

	// 주택 좌표
	for (int i = 0; i < N; i++)
	{
		cin >> houses[i].x >> houses[i].y;

		// 주택 레이블 설정
		grid[houses[i].y][houses[i].x] = char(i + 'A');
	}
}

int main()
{
	Input();
	BuildGraph();
	GetShortestPaths();

	for (auto it : grid)
	{
		cout << it << "\n";
	}

	return 0;
}

/*
 5 5
 5
 .....
 .....
 .....
 .....
 .....
 0 42 68 35 0
 1 70 25 79 59
 63 65 0 6 46
 82 28 62 92 96
 0 43 28 37 9
 0 0
 4 0
 4 4
 2 2
 0 4
*/

/*
5 5
5
..#..
.###.
.....
..#..
..##.
0 42 0 35 0
1 0 0 0 59
63 65 0 6 46
82 28 0 92 96
0 43 0 0 0
0 0
4 0
4 4
2 2
0 4
*/