#include <vector>
#include "MatrixGraph.h"

template <typename T>
MatrixGraph<T>::MatrixGraph(const IGraph<T>* g)
{
	Graph.clear();
	for (int i = 0; i < g->VerticesCount(); i++)
	{
		Graph.push_back({});
		g->GetNextVertices(i, Graph[i]);
	}
}

template <typename T>
void MatrixGraph<T>::AddEdge(const int from, const int to)
{
	Graph[from][to] = 1;
}

template <typename T>
int MatrixGraph<T>::VerticesCount() const { return Graph.size(); }

template <typename T>
void MatrixGraph<T>::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
	vertices.clear();
	for (int i = 0; i < Graph.size(); i++)
		if (Graph[vertex][i] == 1)
			vertices.push_back(i);
}

template <typename T>
void MatrixGraph<T>::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
	vertices.clear();
	for (int i = 0; i < Graph.size(); i++)
		if (Graph[i][vertex] == 1)
			vertices.push_back(i);
}