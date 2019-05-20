/*Дан невзвешенный неориентированный граф. 
Найдите цикл минимальной длины.

Формат ввода
В первой строке вводятся два натуральных числа N и M, 
не превосходящих 10000. 
Далее идут M строк по 2 числа (от 0 до N-1) - индексы вершин, 
между которыми есть ребро.

Формат вывода
Требуется вывести одно целое число, 
равное длине минимального цикла. 
Если цикла нет, то вывести -1.

INPUT:
6 6
0 1
0 2
2 3
2 4
3 5
4 5

OUTPUT:
4

*/

#include <vector>
#include <iostream>
#include <queue>
#include <utility>
#include <unordered_set>

template <typename T>
struct IGraph
{
	virtual ~IGraph() {}

	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const = 0;

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
};

template <typename T>
struct ListGraph : public IGraph<T>
{
public:
	~ListGraph() = default;


	ListGraph(const IGraph<T>* g);
	ListGraph(int n) { Graph.resize(n); }
	ListGraph() { Graph.resize(8); }


	void AddEdge(int from, int to);
	int VerticesCount() const { return Graph.size(); };
	void GetNextVertices(int vertex, std::vector<int>& vertices) const;
	void GetPrevVertices(int vertex, std::vector<int>& vertices) const;

private:
	std::vector<std::vector<int>> Graph;
};

template <typename T>
ListGraph<T>::ListGraph(const IGraph<T>* g)
{
	Graph.clear();
	for (int i = 0; i < g->VerticesCount(); i++)
	{
		Graph.push_back({});
		g->GetNextVertices(i, Graph[i]);
	}
}

template <typename T>
void ListGraph<T>::AddEdge(int from, int to)
{
	bool checker = false;
	for (auto vx : Graph[from])
		if (vx == to)
		{
			checker = true;
			break;
		}
	if (!checker)
		Graph[from].push_back(to);
	checker = false;


	for (auto vx : Graph[to])
		if (vx == from)
		{
			checker = true;
			break;
		}
	if (!checker)
		Graph[to].push_back(from);
}

template <typename T>
void ListGraph<T>::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
	vertices = Graph[vertex];
}

template <typename T>
void ListGraph<T>::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
	vertices.clear();
	for (int i = 0; i < Graph.size(); i++)
		for (auto vx : Graph[i])
			if (vx == vertex)
			{
				vertices.push_back(i);
				break;
			}
}


template <class T, class TVisitor>
void BFS(IGraph<T> const *G, int start, TVisitor &visitor)
{
	std::queue<int> vertices;
	std::unordered_set<int> visited;
	vertices.push(start);
	while (!vertices.empty() && !visitor(G, vertices.front()))
	{
		std::vector<int> next;
		G->GetNextVertices(vertices.front(), next);
		for (int i = 0; i < next.size(); i++)
			if (visited.count(next[i]) == 0)
			{
				visited.insert(next[i]);
				vertices.push(next[i]);
			}
		vertices.pop();
	}
}

class Visitor {
public:
    bool operator()(const IGraph<int> *G, int vertex) {
		if (vertices.size() == 0)
			initialize(G, vertex);
		std::vector<int> temp_vertices;
		G->GetNextVertices(vertices.front().ind, temp_vertices);
		int prevcurmin = curmin;
		curmin = find_cur_min(G, found, temp_vertices, vertices, visited);
		vertices.pop();
		if (vertices.empty())
			curmin = prevcurmin;
		return vertices.empty();
    };

	int get_cur_min() { return curmin; }

private:
	struct Vert
	{
		Vert() = default;
		Vert(int i, int p, int r) : ind(i), prev(p), range(r) {}
		int ind;
		int prev;
		int range;
	};
	void initialize(const IGraph<int> *G, int vertex);
	int find_cur_min(IGraph<int> const *G, bool &found, std::vector<int> &temp_vertices,
		std::queue<Vert> &vertices, std::vector<std::pair<char, int>> &visited);
	std::queue<Vert> vertices;
	std::vector<std::pair<char, int>> visited;
	bool found = false;
	int curmin = -1;
};

void Visitor::initialize(const IGraph<int> *G, int vertex)
{
	curmin = G->VerticesCount() + 2;
	vertices.push({ vertex, -1, 0 });
	visited = std::vector<std::pair<char, int>>(G->VerticesCount(), { 0,0 });
	visited[vertex] = { 1,0 };
	found = false;
}

int Visitor::find_cur_min(IGraph<int> const *G, bool &found, std::vector<int> &temp_vertices,
	std::queue<Vert> &vertices, std::vector<std::pair<char, int>> &visited)
{
	if (temp_vertices.size() > 1)
	{
		for (int j = 0; j < temp_vertices.size() && !found; j++)
			if (visited[temp_vertices[j]].first == 1)
			{
				if (visited[temp_vertices[j]].second >= vertices.front().range
					&& temp_vertices[j] != vertices.front().ind)
				{
					found = true;
					return visited[temp_vertices[j]].second + vertices.front().range + 1;
				}
			}
			else
			{
				visited[temp_vertices[j]] = { 1,vertices.front().range + 1 };
				vertices.push({ temp_vertices[j], vertices.front().ind, vertices.front().range + 1 });
			}
	}
	return G->VerticesCount() + 2;
}

int FindMinCycle(const IGraph<int> *G)
{
	int min = G->VerticesCount() + 2;
	for (int i = 0; i < G->VerticesCount(); i++)
	{
		Visitor visitor;
		BFS(G, i, visitor);
		int curmin = visitor.get_cur_min();
		if (min > curmin)
			min = curmin;
	}
	return min < G->VerticesCount() + 2 ? min : -1;
}

int main()
{
	int N, M;
	std::cin >> N >> M;
	ListGraph<int> G(N);
	for (int i = 0; i < M; i++)
	{
		int from, to;
		std::cin >> from >> to;
		G.AddEdge(from, to);
	}
	std::cout << FindMinCycle(&G) << std::endl;
	system("pause");
	return 0;
}