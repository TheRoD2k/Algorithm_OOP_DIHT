/*Арбитраж - это торговля по цепочке различных валют в 
надежде заработать на небольших различиях в коэффициентах. 
Например, есть следующие курсы валют:
GBP/USD: 0.67
RUB/GBP: 78.66
USD/RUB: 0.02
Имея 1$ и совершив цикл USD->GBP->RUB->USD, получим 1.054$. 
Таким образом заработав 5.4

Формат ввода
Первая строка содержит число N – количество возможных валют 
(определяет размер таблицы котировок). 
Далее следует построчное представление таблицы. 
Диагональные элементы (i, i) пропущены 
(подразумевается, что курс валюты к себе же 1.0). 
В элементе таблицы (i, j) содержится обменный курс i->j. 
Если обмен в данном направлении не производится, то -1.
Формат вывода
Выведите YES, если арбитраж есть, и NO, иначе.

INPUT:
2
      10.0
0.09

OUTPUT:
NO

*/

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <utility>
#include <set>
#include <algorithm>

//==================================================================================================================

template <typename T>
struct IGraph
{
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to, T weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector<std::pair<int, T>>& vertices) const = 0;
    //virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
};

template <typename T>
struct ListGraph : public IGraph<T>
{
public:
    ~ListGraph() = default;


    ListGraph(const IGraph<T>* g);
    ListGraph(int n) { Graph.resize(n); }
    ListGraph() { Graph.resize(8); }


    void AddEdge(int from, int to, T weight);
    int VerticesCount() const { return Graph.size(); };
    void GetNextVertices(int vertex, std::vector<std::pair<int, T>>& vertices) const;
    //void GetPrevVertices(int vertex, std::vector<int>& vertices) const;

private:
    std::vector<std::vector<std::pair<int, T>>> Graph;
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
void ListGraph<T>::AddEdge(int from, int to, T weight)
{
    Graph[from].push_back({to, weight});
}

template <typename T>
void ListGraph<T>::GetNextVertices(int vertex, std::vector<std::pair<int, T>>& vertices) const
{
    vertices = Graph[vertex];
}

bool FordBellman(IGraph<double> *G)
{
    std::vector<double> d(G->VerticesCount(), 0);
    d[0] = 1;
    bool changed = false;
    for (int i = 0; i < G->VerticesCount(); i++)
    {
        changed = false;
        for (int j = 0; j < G->VerticesCount(); j++)
        {
            std::vector<std::pair<int, double>> next;
            G->GetNextVertices(j, next);
            for (int k = 0; k < next.size(); k++)
                if (d[next[k].first] < d[j] * next[k].second)   
            	{
            		d[next[k].first] = d[j] * next[k].second;
             		changed = true;
            	}
        }
    }
    return changed;
}

int main()
{
    int N;
    std::cin >> N;
    ListGraph<double> Graph(N);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            double weight;
            if (i == j)
            {
            //std::cin >> weight;
            weight = 1;}
            else
                std::cin >> weight;
            if (weight != -1)
                Graph.AddEdge(i,j,weight);
        }
    if (FordBellman(&Graph))
        std::cout << "YES" << std::endl;
    else
        std::cout << "NO" << std::endl;
}
