/*Дан невзвешенный неориентированный граф. 
Определить, является ли он двудольным. 
Требуемая сложность O(V+E).

Ввод: v:кол-во вершин(макс. 50000), n:кол-во ребер(макс. 200000), 
n пар реберных вершин.

Вывод: YES если граф является двудольным, NO - если не является.



INPUT:
3
3
0 1
1 2
0 2

OUTPUT:
NO

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

template <typename T>
bool IsTwodBFS(IGraph<T> const *G)
{
	std::queue<int> vertices;
	std::vector<Vert> visited(G->VerticesCount());
	for (int i = 0; i < G->VerticesCount(); i++)
	{
		if (!visited[i].visited)
		{
			vertices.push(i);
			visited[i].visited = true;
			visited[i].color = 1;
			visited[i].range = 0;
			std::vector<int> temp_vertices;
			while (!vertices.empty())
			{
				G->GetNextVertices(vertices.front(), temp_vertices);
				for (int j = 0; j < temp_vertices.size(); j++)
				{
					if (!visited[temp_vertices[j]].visited || visited[temp_vertices[j]].range >= visited[vertices.front()].range)
					{
						if (visited[vertices.front()].color == visited[temp_vertices[j]].color)
							return false;
						if (!visited[temp_vertices[j]].visited)
							vertices.push(temp_vertices[j]);
						visited[temp_vertices[j]].visited = true;
						visited[temp_vertices[j]].range = visited[vertices.front()].range + 1;
						visited[temp_vertices[j]].color = 3 - visited[vertices.front()].color;
					}
				}
				vertices.pop();
			}
		}

	}
	return true;
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
		if (isTwod == false)
			return true;
		if (visited.size() == 0)
			initialize(G, vertex);
		vertices.push(vertex);
		visited[vertex].visited = true;
		visited[vertex].color = 1;
		visited[vertex].range = 0;
		std::vector<int> temp_vertices;
		while (!vertices.empty())
		{
			G->GetNextVertices(vertices.front(), temp_vertices);
			for (int j = 0; j < temp_vertices.size(); j++)
			{
				if (!visited[temp_vertices[j]].visited || visited[temp_vertices[j]].range >= visited[vertices.front()].range)
				{
					if (visited[vertices.front()].color == visited[temp_vertices[j]].color)
						isTwod = false;
					if (!visited[temp_vertices[j]].visited)
						vertices.push(temp_vertices[j]);
					visited[temp_vertices[j]].visited = true;
					visited[temp_vertices[j]].range = visited[vertices.front()].range + 1;
					visited[temp_vertices[j]].color = 3 - visited[vertices.front()].color;
				}
			}
			vertices.pop();
		}
		return (vertices.empty());
	};

	bool is_visited(int vertex) { return (visited.size() == 0) ? false : visited[vertex].visited; }
	bool is_twod() { return isTwod; }

private:
	void initialize(const IGraph<int> *G, int vertex);
	struct Vert
	{
		Vert() : visited(false), color(0), range(0) {};
		Vert(bool v, char c, int r) : visited(v), color(c), range(r) {}
		bool visited;
		char color;
		int range;
	};
	std::vector<Vert> visited;
	std::queue<int> vertices;
	bool isTwod = true;
};

void Visitor::initialize(const IGraph<int> *G, int vertex)
{
	visited = std::vector<Vert>(G->VerticesCount());
}

bool IsTwod(IGraph<int> *G)
{
	std::queue<int> vertices;
	Visitor visitor;
	for (int i = 0; i < G->VerticesCount(); i++)
	{
		if (!visitor.is_visited(i))
		{
			BFS(G, i, visitor);
		}
	}
	return visitor.is_twod();
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
	if (((N + 1)*(N + 1)) / 4 < M || !IsTwod(&G))
		std::cout << "NO" << std::endl;
	else
			std::cout << "YES" << std::endl;
	system("pause");
	return 0;
}