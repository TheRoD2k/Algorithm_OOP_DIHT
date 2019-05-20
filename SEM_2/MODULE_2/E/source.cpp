/*Полный ориентированный взвешенный граф задан матрицей смежности. 
Постройте матрицу кратчайших путей между его вершинами. 
Гарантируется, что в графе нет циклов отрицательного веса.

Формат ввода
В первой строке вводится единственное число N (1 < N < 100) — количество вершин графа. 
В следующих N строках по N чисел задается матрица смежности графа 
(j-ое число в i-ой строке — вес ребра из вершины i в вершину j). 
Все числа по модулю не превышают 100.
На главной диагонали матрицы — всегда нули.

Формат вывода
Выведите N строк по N чисел — матрицу расстояний между парами вершин, 
где j-ое число в i-ой строке равно весу кратчайшего пути из вершины i в j.


INPUT:
4
0 5 9 100
100 0 2 8
100 100 0 7
4 100 100 0


OUTPUT:
0 5 7 13 
12 0 2 8 
11 16 0 7 
4 9 11 0 

*/

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <utility>
#include <set>
#include <algorithm>


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



std::vector<std::vector<std::pair<int, int>>> Floyd(IGraph<int> *G)
{
    std::vector<std::vector<std::pair<int, int>>> d;
    for (int i = 0; i < G->VerticesCount(); i++)
    {
        std::vector<std::pair<int, int>> next;
        G->GetNextVertices(i, next);
        d.push_back(next);
    }

    std::vector<std::vector<int>> next(G->VerticesCount(),std::vector<int>(G->VerticesCount()));

    
    for (int i = 0; i < G->VerticesCount(); i++)
        for (int u = 0; u < G->VerticesCount(); u++)
            for (int v = 0; v < G->VerticesCount(); v++)
            {
                if (d[u][i].second + d[i][v].second < d[u][v].second)
                {
                    d[u][v].second = d[u][i].second + d[i][v].second;
                    next[u][v] = next[u][i];
                }
    }
    return d;
}

int main()
{
    int N;
    std::cin >> N;
    ListGraph<int> Graph(N);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            int weight;
            std::cin >> weight;
            Graph.AddEdge(i,j,weight);
        }
    auto answer = Floyd(&Graph);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            std::cout << answer[i][j].second << " ";
        std::cout << std::endl;
    }
    return 0;

}