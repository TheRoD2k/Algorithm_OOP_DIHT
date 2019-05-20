#include <vector>
#include <unordered_set>
#include "SetGraph.h"

template <typename T>
SetGraph<T>::SetGraph(const IGraph<T>* g)
{
	Graph.clear();
	for (int i = 0; i < g->VerticesCount(); i++)
	{
		Graph.push_back({});
		std::vector<int> temp;
		g->GetNextVertices(i, temp);
		for (int j = 0; j < temp.size(); j++)
			Graph[i].insert(temp[j]);
	}
}

template <typename T>
void SetGraph<T>::AddEdge(int from, int to)
{
	Graph[from].insert(to);
}

template <typename T>
void SetGraph<T>::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
	vertices.clear();
	for (auto i : Graph[vertex])
		vertices.push_back(i);
}

template <typename T>
void SetGraph<T>::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
	vertices.clear();
	for (int i = 0; i < Graph.size(); i++)
	{
		if (Graph[i].find(vertex) != Graph[i].end())
			vertices.push_back(i);
	}
}