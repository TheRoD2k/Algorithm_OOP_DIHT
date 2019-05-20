#include <vector>
#include <map>
#include "ArcGraph.h"

template <typename T>
ArcGraph<T>::ArcGraph(const IGraph<T> *g)
{
	for (int i = 0; i < g->VerticesCount(); i++)
	{
		std::vector<int> temp;
		g->GetNextVertices(i, temp);
		for (int j = 0; j < temp.size(); j++)
			AddEdge(i, temp[j]);
	}
}

template <typename T>
ArcGraph<T>::ArcGraph(int N)
{
}

template <typename T>
int ArcGraph<T>::VerticesCount() const { return verticesCount; }

template <typename T>
void ArcGraph<T>::AddEdge(int from, int to)
{
	if (unicVertexMap[from] != 1)
		++verticesCount;
	if (unicVertexMap[to] != 1)
		++verticesCount;
	unicVertexMap[from] = unicVertexMap[to] = 1;

	Graph.push_back({ from,to });
}

template <typename T>
void ArcGraph<T>::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
	for (int i = 0; i < Graph.size(); i++)
		if (Graph[i].first == vertex)
			vertices.push_back(Graph[i].second);
}

template <typename T>
void ArcGraph<T>::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
	for (int i = 0; i < Graph.size(); i++)
		if (Graph[i].second == vertex)
			vertices.push_back(Graph[i].first);
}