/*Дан ориентированный граф. 
Определите, какое минимальное число ребер необходимо добавить, 
чтобы граф стал сильносвязным. В графе возможны петли.

Формат ввода
В первой строке указывается число вершин графа V, 
во второй – число ребер E, в последующих – E пар вершин, задающих ребра.

Формат вывода
Минимальное число ребер k.


INPUT:
4
4
1 2
2 3
3 1
3 4

OUTPUT:
1

*/

#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

template <typename T>
struct IGraph
{
	virtual ~IGraph() {}

	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const = 0;

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
	virtual void RemoveEdge(int form, int to) = 0;
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
	void RemoveEdge(int from, int to);
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


	/*for (auto vx : Graph[to])
		if (vx == from)
		{
			checker = true;
			break;
		}
	if (!checker)
		Graph[to].push_back(from);*/
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
void ListGraph<T>::RemoveEdge(int from, int to)
{
	for (int i = 0; i < Graph[from].size(); i++)
		if (Graph[from][i] == to)
		{
			std::swap(Graph[from][i], Graph[from][Graph[from].size() - 1]);
			Graph[from].pop_back();
			break;
		}

	for (int i = 0; i < Graph[to].size(); i++)
		if (Graph[to][i] == from)
		{
			std::swap(Graph[to][i], Graph[to][Graph[to].size() - 1]);
			Graph[to].pop_back();
			break;
		}
}

//======================
//======================


ListGraph<int> MakeInversed(IGraph<int> *G)
{
	ListGraph<int> InvG(G->VerticesCount());
	std::vector<int> temp_vertices;
	for (int i = 0; i < G->VerticesCount(); i++)
	{
		G->GetPrevVertices(i, temp_vertices);
		for (int j = 0; j < temp_vertices.size(); j++)
		{
			InvG.AddEdge(i, temp_vertices[j]);
		}
	}

	return InvG;
}

struct Vert
{
	bool visited = false, toPop = false;
};

struct Leave
{
	int ind = -1;
	int time = 0;
};

bool CompForDFS(Leave &l, Leave &r)
{
	return l.time > r.time;
}

void InvDFS(const IGraph<int> *G, std::vector<Leave> &leave)
{
	std::vector<Vert> visited(G->VerticesCount());
	int time = 0;
	for (int i = 0; i < G->VerticesCount(); i++)
	{
		std::vector<int> vertices;
		if (visited[i].visited)
			continue;
		vertices.push_back(i);
		visited[i].visited = true;
		std::vector<int> temp_vertices;
		while (!vertices.empty())
		{
			int cur = vertices.back();
			if (visited[vertices.back()].toPop == true)
			{
				vertices.pop_back();
				leave[cur].time = time;
				leave[cur].ind = cur;
				++time;
			}
			else
				visited[cur].toPop = true;

			visited[cur].visited = true;
			G->GetNextVertices(cur, temp_vertices);
			for (int j = 0; j < temp_vertices.size(); j++)
			{
				if (visited[temp_vertices[j]].visited == 0)
				{
					vertices.push_back(temp_vertices[j]);
				}
			}
		}
	}
}

std::vector<std::vector<int>> GetComponentsDFS(const IGraph<int> *G, std::vector<Leave> &leave)
{
	std::vector<std::vector<int>> components;
	std::vector<Vert> visited(G->VerticesCount());
	//int count = 0;
	for (int i = 0; i < leave.size(); i++)
	{
		std::vector<int> vertices;
		if (visited[leave[i].ind].visited)
			continue;

		vertices.push_back(leave[i].ind);
		components.push_back({});
		components.back().push_back(leave[i].ind);
		while (!vertices.empty())
		{
			int cur = vertices.back();
			vertices.pop_back();
			visited[cur].visited = true;
			std::vector<int> temp_vertices;
			G->GetNextVertices(cur, temp_vertices);
			for (int j = 0; j < temp_vertices.size(); j++)
			{
				if (!visited[temp_vertices[j]].visited)
				{
					vertices.push_back(temp_vertices[j]);
					components.back().push_back(temp_vertices[j]);
				}
			}
		}
	}
	return components;
}

struct Edges
{
	std::unordered_set<int> from, to, vertices;
};

std::vector<Edges> Compose(IGraph<int> *G, std::vector<std::vector<int>> &components)
{
	std::vector<Edges> composed(components.size());
	for (int i = 0; i < components.size(); i++)
	{
		std::vector<int> temp_vertices;
		for (int j = 0; j < components[i].size(); j++)
		{
			composed[i].vertices.insert(components[i][j]);
		}

		for (int j = 0; j < components[i].size(); j++)
		{
			G->GetNextVertices(components[i][j], temp_vertices);
			for (int k = 0; k < temp_vertices.size(); k++)
				if (composed[i].vertices.find(temp_vertices[k]) == composed[i].vertices.end())
					composed[i].to.insert(temp_vertices[k]);
			G->GetPrevVertices(components[i][j], temp_vertices);
			for (int k = 0; k < temp_vertices.size(); k++)
				if (composed[i].vertices.find(temp_vertices[k]) == composed[i].vertices.end())
					composed[i].from.insert(temp_vertices[k]);
		}
	}

	return composed;
}

int GetMinAddNumber(IGraph<int> *G)
{
	auto InvG = MakeInversed(G);
	std::vector<Leave> leave(G->VerticesCount());
	InvDFS(&InvG, leave);

	std::sort(leave.begin(), leave.end(), CompForDFS);
	std::vector<std::vector<int>> components = GetComponentsDFS(G, leave);
	std::vector<Edges> composed = Compose(G, components);
	int min=0, max=0, av;
	for (int i = 0; i < composed.size(); i++)
	{
		if (composed[i].to.empty())
			++min;
		if (composed[i].from.empty())
			++max;
	}

	if (components.size() == 1)
		return 0;
	return std::max(min,max);
}


int main()
{
	int V, E;
	std::cin >> V >> E;
	ListGraph<int> G(V);
	for (int i = 0; i < E; i++)
	{
		int from, to;
		std::cin >> from >> to;
		G.AddEdge(from-1, to-1);
	}
	std::cout << GetMinAddNumber(&G) << std::endl;
	system("pause");
}