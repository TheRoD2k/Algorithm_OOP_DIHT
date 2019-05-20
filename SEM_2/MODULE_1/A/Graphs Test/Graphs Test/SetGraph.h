#pragma once
#include <vector>
#include <unordered_set>
#include "IGraph.h"



template <typename T>
struct SetGraph : public IGraph<T>
{
public:
	~SetGraph() = default;


	SetGraph(const IGraph<T>* g);
	SetGraph(int n) { Graph.resize(n); }
	SetGraph() { Graph.resize(8); }


	void AddEdge(int from, int to);
	int VerticesCount() const { return Graph.size(); };
	void GetNextVertices(int vertex, std::vector<int>& vertices) const;
	void GetPrevVertices(int vertex, std::vector<int>& vertices) const;

private:
	std::vector<std::unordered_set<int>> Graph;
};