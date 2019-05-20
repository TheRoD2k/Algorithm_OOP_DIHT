/*Дан невзвешенный неориентированный граф. 
В графе может быть несколько кратчайших путей между какими-то вершинами. 
Найдите количество различных кратчайших путей между заданными вершинами. 
Требуемая сложность O(V+E).

Формат ввода
v: кол-во вершин (макс. 50000),
n: кол-во ребер(макс. 200000),
n пар реберных вершин,
пара вершин (u, w) для запроса.

Формат вывода
Количество кратчайших путей от v к w.


INPUT:
4
5
0 1
0 2
1 2
1 3
2 3

OUTPUT:
2

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



class Visitor {
public:
	Visitor(int f, int t) : from(f), to(t) {};

	bool operator()(const IGraph<int> *G, int vertex)
	{
		if (visited.size() == 0)
			initialize(G, vertex);
		int prev_amount_of_ways = visited[to].ways;
		std::vector<int> temp_vertices;
		G->GetNextVertices(vertices.front(), temp_vertices);
		for (int i = 0; i < temp_vertices.size(); i++)
		{
			if (temp_vertices[i] == to)
			{
				found = true;
				visited[to].ways += visited[vertices.front()].ways;
				currange = visited[vertices.front()].range;
			}
			else
				if (visited[temp_vertices[i]].visited == false || visited[vertices.front()].range < visited[temp_vertices[i]].range)
				{
					if (visited[temp_vertices[i]].visited == false)
						vertices.push(temp_vertices[i]);
					visited[temp_vertices[i]].visited = true;
					visited[temp_vertices[i]].ways += visited[vertices.front()].ways;
					visited[temp_vertices[i]].range = visited[vertices.front()].range + 1;
				}
		}
		vertices.pop();
		return !(!vertices.empty() && (!found || visited[vertices.front()].range == currange));
	};
	int get_min_ways_amount() { return visited[to].ways; }

private:
	struct Vert
	{
		Vert() : visited(false), ways(0), range(0) {};
		Vert(bool v, int w, int r) : visited(v), ways(w), range(r) {}
		bool visited;
		int ways;
		int range;
	};
	void initialize(const IGraph<int> *G, int vertex);
	int length;
	int count = 0;
	int currange = 0;
	int from, to;
	bool found = false;
	std::vector<Vert> visited;
	std::queue<int> vertices;
};

void Visitor::initialize(const IGraph<int> *G, int vertex)
{
	visited = std::vector<Vert>(G->VerticesCount());
	length = G->VerticesCount();
	visited[from] = { true,1,0 };
	vertices.push(from);
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

int FindAmountOfMinWaysBFS(IGraph<int> *G, int from, int to)
{
	Visitor visitor(from, to);
	BFS(G, from, visitor);
	return visitor.get_min_ways_amount();
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

	int from, to;
	std::cin >> from >> to;
	std::cout << FindAmountOfMinWaysBFS(&G, from, to) << std::endl;
	system("pause");
	return 0;
}