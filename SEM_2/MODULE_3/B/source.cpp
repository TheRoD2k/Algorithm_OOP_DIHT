/*Задан ориентированный граф, каждое ребро которого обладает целочисленной пропускной способностью. 
Найдите максимальный поток из вершины с номером 1 в вершину с номером n.

Формат ввода
Первая строка входного файла содержит 
n и m - количество вершин и количество ребер графа 
(2 <= n <= 100, 1 <= m <= 1000) 
Следующие m строк содержат по три числа: 
номера вершин, которые соединяет соответствующее ребро графа и его пропускную способность. 
Пропускные способности не превосходят 105.
Формат вывода
В выходной файл выведите одно число — 
величину максимального потока из вершины с номером 1 в вершину с номером n.


INPUT:
4 5
1 2 1
1 3 2
3 2 1
2 4 2
3 4 1

OUTPUT:
3

*/



#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <set>
#include <algorithm>
#include <map>

template <typename T>
struct IGraph
{
	virtual ~IGraph() {}

	virtual void AddEdge(int from, int to, int weight) = 0;

	virtual int VerticesCount() const = 0;

	virtual void GetNextVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const = 0;
	virtual void GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const = 0;
};

template <typename T>
struct ListGraph : public IGraph<T>
{
public:
	~ListGraph() = default;


	ListGraph(const IGraph<T>* g);
	ListGraph(int n) { Graph.resize(n); }
	ListGraph() { Graph.resize(8); }


	void AddEdge(int from, int to, int weight);
	int VerticesCount() const { return Graph.size(); };
	void GetNextVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const;
	void GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const;

private:
	std::vector<std::vector<std::pair<int, int>>> Graph;
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
void ListGraph<T>::AddEdge(int from, int to, int weight)
{
	bool checker = false;
	/*for (auto vx : Graph[from])
		if (vx.first == to)
		{
			checker = true;
			break;
		}*/
	if (!checker)
		Graph[from].push_back({ to, weight });
	checker = false;


	/*for (auto vx.first : Graph[to])
		if (vx.first == from)
		{
			checker = true;
			break;
		}*/
	//if (!checker)
	//	Graph[to].push_back({ from, weight });
}

template <typename T>
void ListGraph<T>::GetNextVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const
{
	vertices = Graph[vertex];
}

template <typename T>
void ListGraph<T>::GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const
{
	vertices.clear();
	for (int i = 0; i < Graph.size(); i++)
		for (auto vx : Graph[i])
			if (vx.first == vertex)
			{
				vertices.push_back({ i, vx.second });
				break;
			}
}

void printD(std::vector<int> d)
{
	std::cout << "d ";
	for (int i = 0; i < d.size(); i++)
		std::cout << d[i] << " ";
	std::cout << std::endl;
}

bool bfs(IGraph<int> &G, std::map<std::pair<int, int>, int> &flow, std::map<std::pair<int, int>, int> &capacity, std::vector<int> &d, int s, int t)
{
	d = std::vector<int>(G.VerticesCount(), -1);
	std::queue<int> q;
	d[s] = 0;
	q.push(s);
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		std::vector<std::pair<int, int>> next;
		G.GetNextVertices(u, next);
		for (int j = 0; j < next.size(); j++)
		{
			if (flow[{u, next[j].first}] < capacity[{u, next[j].first}] && d[next[j].first] == -1)
			{
				d[next[j].first] = d[u] + 1;
				q.push(next[j].first);
			}
		}
		G.GetPrevVertices(u, next);
		for (int j = 0; j < next.size(); j++)
		{
			if (flow[{u, next[j].first}] < capacity[{u, next[j].first}] && d[next[j].first] == -1)
			{
				d[next[j].first] = d[u] + 1;
				q.push(next[j].first);
			}
		}
	}
	return d[t] != -1;
}

int dfs(IGraph<int> &G, std::map<std::pair<int, int>, int> &flow, std::map<std::pair<int, int>, int> &capacity, std::vector<int> &d, std::vector<int> &p, int u, int t, int minC)
{
	if (u == t || minC == 0)
		return minC;
	for (int v = p[u]; v < G.VerticesCount(); v++)
	{
		if (d[v] == d[u] + 1)
		{
			int delta = dfs(G, flow, capacity, d, p, v, t, std::min(minC, capacity[{u, v}] - flow[{u, v}]));
			if (delta != 0)
			{
				flow[{u, v}] += delta;
				flow[{v, u}] -= delta;
				return delta;
			}
		}
		++p[u];
	}
	return 0;
}

void MakeCapacity(IGraph<int> &G, std::map<std::pair<int, int>, int> &capacity)
{
	for (int i = 0; i < G.VerticesCount(); i++)
	{
		std::vector<std::pair<int, int>> next;
		G.GetNextVertices(i, next);
		for (int j = 0; j < next.size(); j++)
		{
			capacity[{i, next[j].first}] = next[j].second;
		}
	}
}

int findMaxFlow(IGraph<int> &G, int s, int t)
{
	int maxFlow = 0;
	std::map<std::pair<int, int>, int> flow, capacity;
	MakeCapacity(G, capacity);
	std::vector<int> d(G.VerticesCount(), -1);
	
	
	while (bfs(G, flow, capacity, d, s, t))
	{
		std::vector<int> p(G.VerticesCount(), 0);
		int flownum = dfs(G, flow, capacity, d, p, s, t, 9000000);
		while (flownum != 0)
		{
			maxFlow += flownum;
			flownum = dfs(G, flow, capacity, d, p, s, t, 9000000);
		}
	}
	return maxFlow;
}

int main()
{
	int n, m;
	std::cin >> n >> m;
	ListGraph<int> G(n);
	for (int i = 0; i < m; i++)
	{
		int from, to, capacity;
		std::cin >> from >> to >> capacity;
		G.AddEdge(from-1, to-1, capacity);
	}
	std::cout << findMaxFlow(G, 0, n - 1) << std::endl;
	system("pause");
}