/*Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе. 
Вариант 1. С помощью алгоритма Прима.
Вариант 2. С помощью алгоритма Крускала.
Вариант 3. С помощью алгоритма Борувки.
Ваш номер варианта прописан в ведомости.
Формат ввода
Первая строка содержит два натуральных числа n и m — количество вершин и ребер графа соответственно (1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000). 
Следующие m строк содержат описание ребер по одному на строке. 
Ребро номер i описывается тремя натуральными числами bi, ei и wi — номера концов ребра и его вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100000).

Формат вывода
Выведите единственное целое число - вес минимального остовного дерева.


INPUT:
4 4
1 2 1
2 3 2
3 4 5
4 1 4

OUTPUT:
7

*/


#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <algorithm>

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
	if (!checker)
		Graph[to].push_back({ from, weight });
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

template <typename T>
class DSU
{
public:
	void make_set(T v);
	int find_set(T v);
	void union_sets(T a, T b, int val);
	DSU(int N);
	int size();
	int get_value(int v) { return values[find_set(v)]; }

private:
	std::vector<int> parent;
	std::vector<int> rank;
	std::vector<int> values;
};

template <typename T>
DSU<T>::DSU(int N) 
{
	parent.resize(N);
	rank.resize(N);
	values.resize(N);
	for (int i = 0; i < N; i++)
		parent[i] = i;
}

template <typename T>
void DSU<T>::make_set(T v)
{
	parent[v] = v;
	rank[v] = 0;
}

template <typename T>
int DSU<T>::find_set(T v)
{
	if (v == parent[v])
		return v;
	return parent[v] = find_set(parent[v]);
}

template <typename T>
void DSU<T>::union_sets(T a, T b, int val)
{
	a = find_set(a);
	b = find_set(b);
	if (a != b) 
	{
		if (rank[a] < rank[b])
			std::swap(a, b);
		parent[b] = a;
		if (rank[a] == rank[b])
			++rank[a];
		values[a] = values[a] + values[b] + val;
	}
 }

template <typename T>
int DSU<T>::size()
{
	int size = 0;
	for (int i = 0; i < parent.size(); i++)
		if (i == parent[i])
			++size;
	return size;
}

int Boruvka(const IGraph<int> &G)
{
	DSU<int> dsu(G.VerticesCount());

	while (dsu.size() > 1)
	{
		std::vector<std::pair<int, int>> best_edges(G.VerticesCount(), { -1, 9000000 });
		std::vector<std::pair<int, int>> next;
		for (int i = 0; i < G.VerticesCount(); i++)
		{
			G.GetNextVertices(i, next);
			for (int j = 0; j < next.size(); j++)
			{
				if (dsu.find_set(next[j].first) == dsu.find_set(i))
					continue;
				if (best_edges[dsu.find_set(i)].second >= next[j].second)
					best_edges[dsu.find_set(i)] = next[j];
			}
		}
		for (int i = 0; i < best_edges.size(); i++)
		{
			if (best_edges[i].first == -1 || dsu.find_set(best_edges[i].first) == i)
				continue;
			dsu.union_sets(best_edges[i].first, i, best_edges[i].second);
		}
	}
	return dsu.get_value(0);
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	int N, M;
	std::cin >> N >> M;
	ListGraph<int> G(N);
	for (int i = 0; i < M; i++)
	{
		int from, to, weight;
		std::cin >> from >> to >> weight;
		G.AddEdge(from-1, to-1, weight);
	}

	std::cout << Boruvka(G) << std::endl;
	system("pause");
}