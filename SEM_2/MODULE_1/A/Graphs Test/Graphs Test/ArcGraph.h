#pragma once
#include <vector>
#include <map>
#include "IGraph.h"

template <typename  T>
struct ArcGraph : public IGraph<T>
{
public:
	~ArcGraph() = default;

	ArcGraph(const IGraph<T>* g);
	ArcGraph(int N);
	ArcGraph() = default;

	void AddEdge(int from, int to);
	int VerticesCount() const;
	void GetNextVertices(int vertex, std::vector<int>& vertices) const;
	void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
private:
	std::vector<std::pair<int, int>> Graph;
	std::map<int, int> unicVertexMap;
	int verticesCount;
};