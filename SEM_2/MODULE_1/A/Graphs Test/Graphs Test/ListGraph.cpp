#include <iostream>
#include <vector>
#include "ListGraph.h"



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
	/*checker = false;


	for (auto vx : Graph[to])
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