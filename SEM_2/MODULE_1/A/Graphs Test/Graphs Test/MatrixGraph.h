#pragma once
#include <vector>
#include "IGraph.h"

template <typename  T>
struct MatrixGraph : public IGraph<T>
{
public:
	~MatrixGraph() = default;

	MatrixGraph(const IGraph<T>* g);
	MatrixGraph() { Graph = std::vector<std::vector<int>>(8, std::vector<int>(8, 0)); }
	MatrixGraph(int n) { Graph = std::vector<std::vector<int>>(n, std::vector<int>(n, 0)); }

	void AddEdge(const int from, const int to);
	int VerticesCount() const;
	void GetNextVertices(int vertex, std::vector<int>& vertices) const;
	void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
private:
	std::vector<std::vector<int>> Graph;
};