#include <iostream>
#include "ArcGraph.cpp"
#include "ListGraph.cpp"
#include "MatrixGraph.cpp"
#include "SetGraph.cpp"
#include "IGraph.h"
#include <random>
#include <fstream>
#include <time.h>

void TestGetNextVertices(int N, std::vector<double> tests)
{
	std::cout << "GetNextVertices: " << std::endl;
	for (int test = 0; test < tests.size(); test++)
	{
		ArcGraph<int> GArc(N);
		ListGraph<int> GList(N);
		MatrixGraph<int> GMatrix(N);
		SetGraph<int> GSet(N);
		std::cout << "V\E ratio: " << tests[test] << std::endl;
		for (int i = 0; i < N*tests[test]; i++)
		{
			int from = std::abs(rand() % N), to = std::abs(rand() % N);
			GArc.AddEdge(from, to);
			GList.AddEdge(from, to);
			GMatrix.AddEdge(from, to);
			GSet.AddEdge(from, to);
		}
		std::vector<int> next;
		clock_t t1 = clock();
		for (int i = 0; i < N; i++)
			GArc.GetNextVertices(i, next);
		clock_t t2 = clock();
		std::cout << "ArcGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;

		t1 = clock();
		for (int i = 0; i < N; i++)
			GList.GetNextVertices(i, next);
		t2 = clock();
		std::cout << "ListGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;

		t1 = clock();
		for (int i = 0; i < N; i++)
			GMatrix.GetNextVertices(i, next);
		t2 = clock();
		std::cout << "MatrixGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;

		t1 = clock();
		for (int i = 0; i < N; i++)
			GSet.GetNextVertices(i, next);
		t2 = clock();
		std::cout << "SetGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

void TestGetPrevVertices(int N, std::vector<double> tests)
{
	std::cout << "GetPrevVertices: " << std::endl;
	for (int test = 0; test < tests.size(); test++)
	{
		ArcGraph<int> GArc(N);
		ListGraph<int> GList(N);
		MatrixGraph<int> GMatrix(N);
		SetGraph<int> GSet(N);
		std::cout << "V\E ratio: " << tests[test] << std::endl;
		for (int i = 0; i < N*tests[test]; i++)
		{
			int from = std::abs(rand() % N), to = std::abs(rand() % N);
			GArc.AddEdge(from, to);
			GList.AddEdge(from, to);
			GMatrix.AddEdge(from, to);
			GSet.AddEdge(from, to);
		}
		std::vector<int> prev;
		clock_t t1 = clock();
		for (int i = 0; i < N; i++)
			GArc.GetPrevVertices(i, prev);
		clock_t t2 = clock();
		std::cout << "ArcGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;

		t1 = clock();
		for (int i = 0; i < N; i++)
			GList.GetPrevVertices(i, prev);
		t2 = clock();
		std::cout << "ListGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;

		t1 = clock();
		for (int i = 0; i < N; i++)
			GMatrix.GetPrevVertices(i, prev);
		t2 = clock();
		std::cout << "MatrixGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;

		t1 = clock();
		for (int i = 0; i < N; i++)
			GSet.GetPrevVertices(i, prev);
		t2 = clock();
		std::cout << "SetGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

void TestVerticesCount(int N, std::vector<double> tests)
{
	std::cout << "VerticesCount: " << std::endl;
	for (int test = 0; test < tests.size(); test++)
	{
		ArcGraph<int> GArc(N);
		ListGraph<int> GList(N);
		MatrixGraph<int> GMatrix(N);
		SetGraph<int> GSet(N);
		std::cout << "V\E ratio: " << tests[test] << std::endl;
		for (int i = 0; i < N*tests[test]; i++)
		{
			int from = std::abs(rand() % N), to = std::abs(rand() % N);
			GArc.AddEdge(from, to);
			GList.AddEdge(from, to);
			GMatrix.AddEdge(from, to);
			GSet.AddEdge(from, to);
		}
		clock_t t1 = clock();
		GArc.VerticesCount();
		clock_t t2 = clock();
		std::cout << "ArcGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;

		t1 = clock();
		GList.VerticesCount();
		t2 = clock();
		std::cout << "ListGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;

		t1 = clock();
		GMatrix.VerticesCount();
		t2 = clock();
		std::cout << "MatrixGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;

		t1 = clock();
		GSet.VerticesCount();
		t2 = clock();
		std::cout << "SetGraph: " << double(t2 - t1) / 1000 << "s" << std::endl;
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

void TestGraphs(int N)
{
	std::cout << "N = " << N << std::endl;
	std::vector<double> tests{ 0.1, 0.25, 0.5, 0.75, 1.0, double(N/10),  double(N/5), double(N) };
	TestGetNextVertices(N, tests);
	TestGetPrevVertices(N, tests);
	TestVerticesCount(N, tests);
}

int main()
{
	TestGraphs(5000);
	system("pause");
}