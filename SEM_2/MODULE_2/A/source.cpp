/*Требуется отыскать самый короткий маршрут между городами. 
Из города может выходить дорога, которая возвращается в этот же город.

Требуемое время работы O((N + M)log N), 
где N – количество городов, M – известных дорог между ними.
N ≤ 10000, M ≤ 250000.
Длина каждой дороги ≤ 10000.

Формат ввода
Первая строка содержит число N – количество городов.

Вторая строка содержит число M - количество дорог.

Каждая следующая строка содержит описание дороги (откуда, куда, время в пути). 
Все указанные дороги двусторонние. 
ежду любыми двумя городами может быть больше одной дороги.

Последняя строка содержит маршрут (откуда и куда нужно доехать).

Формат вывода
Вывести длину самого короткого маршрута.


INPUT:
6
9
0 3 1
0 4 2
1 2 7
1 3 2
1 4 3
1 5 3
2 5 3
3 4 4
3 5 6
0 2

OUTPUT:
9

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
    if (from == to)
    return;

    Graph[from].push_back({to, weight});

    Graph[to].push_back({from, weight});
}

template <typename T>
void ListGraph<T>::GetNextVertices(int vertex, std::vector<std::pair<int, T>>& vertices) const
{
    vertices = Graph[vertex];
}

/*template <typename T>
void ListGraph<T>::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
    vertices.clear();
    for (int i = 0; i < Graph.size(); i++)
        for (auto vx : Graph[i])
            if (vx.first == vertex)
            {
                vertices.push_back(i);
                break;
            }
}*/




bool Relax(std::vector<int> &pi, std::vector<int> &d, int first, std::pair<int, int> &second)
{
    if (d[second.first] > d[first] + second.second)
    {

        d[second.first] = d[first] + second.second;
        pi[second.first] = first;
        return true;
    }
    return false;
}

int Dijkstra(IGraph<int> *G, int s, int b)
{
    auto compare = [] (std::pair<int, int> const &left, std::pair<int, int> const &right)
    {
    	if (left.second == right.second)
    		return (left.first < right.first);
   		return left.second < right.second;
    };
    std::vector<int> pi (G->VerticesCount(), -1);
    std::vector<int> d (G->VerticesCount(), 30000000);
    d[s] = 0;
    std::set<std::pair<int, int>, decltype(compare)> q(compare);
    q.insert({s, d[s]});
    while( !q.empty() )
    {
        int u = q.begin()->first;
        q.erase(q.begin());

        std::vector<std::pair<int, int>> next;
        G->GetNextVertices(u, next);
        for(int i = 0; i < next.size(); i++)
        {
            if( d[next[i].first] == 30000000 )
            {
                d[next[i].first] = d[u] + next[i].second;
                pi[next[i].first] = u;
                q.insert( {next[i].first, d[next[i].first]} );
            }
            else
            {
                std::pair<int, int> temp = {next[i].first, d[next[i].first]};
                if (Relax(pi, d, u, next[i]))
                {
                    if (q.find(temp) != q.end())
                    {q.erase(q.find(temp));
                     q.insert({next[i].first, d[next[i].first]});}
                }
            }
        }
    }
    return d[b];
}

int main()
{
    int N, M;
    std::cin >> N >> M;
    ListGraph<int> Graph(N);
    for (int i = 0; i < M; i++)
    {
        int a,b,c;
        std::cin >> a >> b >> c;
        Graph.AddEdge(a,b,c);
    }
    int a,b;
    std::cin >> a >> b;
    std::cout << Dijkstra(&Graph, a, b);
    return 0;
}