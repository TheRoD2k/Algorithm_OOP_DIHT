/*В одном из отделов крупной организации работает n человек. 
Как практически все сотрудники этой организации, они любят пить чай в перерывах между работой. 
При этом они достаточно дисциплинированы и делают в день ровно один перерыв, во время которого пьют чай. 
Для того, чтобы этот перерыв был максимально приятным, 
каждый из сотрудников этого отдела обязательно пьет чай одного из своих любимых сортов. 
В разные дни сотрудник может пить чай разных сортов. 
Для удобства пронумеруем сорта чая числами от 1 до m. 
Недавно сотрудники отдела купили себе большой набор чайных пакетиков, 
который содержит a1 пакетиков чая сорта номер 1, a2 пакетиков чая сорта номер 2, ..., am пакетиков чая сорта номер m. 
Теперь они хотят знать, на какое максимальное число дней им может хватить купленного набора так, 
чтобы в каждый из дней каждому из сотрудников доставался пакетик чая одного из его любимых сортов. 
Каждый сотрудник отдела пьет в день ровно одну чашку чая, которую заваривает из одного пакетика. 
При этом пакетики чая не завариваются повторно.

Формат ввода
Первая строка содержит два целых числа n и m (1 ≤ n, m ≤ 50). 
Вторая строка содержит m целых чисел a1, ..., am (1 ≤ ai ≤ 1000000 для всех i от 1 до m). 
Далее следуют n строк — i-я из этих строк описывает любимые сорта i-го сотрудника отдела и имеет следующий формат: 
сначала следует положительное число ki — количество любимых сортов чая этого сотрудника, 
а затем идут ki различных чисел от 1 до m — номера этих сортов.

Формат вывода
Выведите одно целое число — искомое максимальное количество дней.


INPUT:
2 3
3 2 1
2 1 2
2 1 3

OUTPUT:
3

*/



#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <set>
#include <algorithm>
#include <map>

template <typename T>
struct IGraph
{
	virtual ~IGraph() {}

	virtual void AddEdge(int from, int to, int weight) = 0;

	virtual int VerticesCount() const = 0;

	virtual void GetNextVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const = 0;
	virtual void GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const = 0;
};

template <typename T>
struct ListGraph : public IGraph<T>
{
public:
	~ListGraph() = default;


	ListGraph(const IGraph<T>* g);
	ListGraph(int n) { Graph.resize(n); }
	ListGraph() { Graph.resize(8); }


	void AddEdge(int from, int to, int weight);
	int VerticesCount() const { return Graph.size(); };
	void GetNextVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const;
	void GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const;

private:
	std::vector<std::vector<std::pair<int, int>>> Graph;
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
void ListGraph<T>::AddEdge(int from, int to, int weight)
{
	bool checker = false;
	/*for (auto vx : Graph[from])
		if (vx.first == to)
		{
			checker = true;
			break;
		}*/
	if (!checker)
		Graph[from].push_back({ to, weight });
	checker = false;


	/*for (auto vx.first : Graph[to])
		if (vx.first == from)
		{
			checker = true;
			break;
		}*/
		//if (!checker)
		//	Graph[to].push_back({ from, weight });
}

template <typename T>
void ListGraph<T>::GetNextVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const
{
	vertices = Graph[vertex];
}

template <typename T>
void ListGraph<T>::GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const
{
	vertices.clear();
	for (int i = 0; i < Graph.size(); i++)
		for (auto vx : Graph[i])
			if (vx.first == vertex)
			{
				vertices.push_back({ i, vx.second });
				break;
			}
}

void printD(std::vector<int> d)
{
	std::cout << "d ";
	for (int i = 0; i < d.size(); i++)
		std::cout << d[i] << " ";
	std::cout << std::endl;
}

bool bfs(IGraph<int> &G, std::map<std::pair<int, int>, int> &flow, std::map<std::pair<int, int>, int> &capacity, std::vector<int> &d, int s, int t)
{
	d = std::vector<int>(G.VerticesCount(), -1);
	std::queue<int> q;
	d[s] = 0;
	q.push(s);
	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		std::vector<std::pair<int, int>> next;
		G.GetNextVertices(u, next);
		for (int j = 0; j < next.size(); j++)
		{
			if (flow[{u, next[j].first}] < capacity[{u, next[j].first}] && d[next[j].first] == -1)
			{
				d[next[j].first] = d[u] + 1;
				q.push(next[j].first);
			}
		}
		G.GetPrevVertices(u, next);
		for (int j = 0; j < next.size(); j++)
		{
			if (flow[{u, next[j].first}] < capacity[{u, next[j].first}] && d[next[j].first] == -1)
			{
				d[next[j].first] = d[u] + 1;
				q.push(next[j].first);
			}
		}
	}
	return d[t] != -1;
}

int dfs(IGraph<int> &G, std::map<std::pair<int, int>, int> &flow, std::map<std::pair<int, int>, int> &capacity, std::vector<int> &d, std::vector<int> &p, int u, int t, int minC)
{
	if (u == t || minC == 0)
		return minC;
	for (int v = p[u]; v < G.VerticesCount(); v++)
	{
		if (d[v] == d[u] + 1)
		{
			int delta = dfs(G, flow, capacity, d, p, v, t, std::min(minC, capacity[{u, v}] - flow[{u, v}]));
			if (delta != 0)
			{
				flow[{u, v}] += delta;
				flow[{v, u}] -= delta;
				return delta;
			}
		}
		++p[u];
	}
	return 0;
}


int findMaxFlow(IGraph<int> &G, int s, int t, std::map<std::pair<int, int>, int> &capacity, std::map<std::pair<int, int>, int> &flow)
{
	int maxFlow = 0;
	std::vector<int> d(G.VerticesCount(), -1);


	while (bfs(G, flow, capacity, d, s, t))
	{
		std::vector<int> p(G.VerticesCount(), 0);
		int flownum = dfs(G, flow, capacity, d, p, s, t, 9000000);
		while (flownum != 0)
		{
			maxFlow += flownum;
			flownum = dfs(G, flow, capacity, d, p, s, t, 9000000);
		}
	}
	return maxFlow;
}

void MakeCapacity(IGraph<int> &G, std::map<std::pair<int, int>, int> &capacity, int days_amount)
{
	for (int i = 0; i < G.VerticesCount(); i++)
	{
		std::vector<std::pair<int, int>> next;
		G.GetNextVertices(i, next);
		for (int j = 0; j < next.size(); j++)
		{
			if (i == 0)
				capacity[{i, next[j].first}] = days_amount;
			else
				capacity[{i, next[j].first}] = next[j].second;
		}
	}
}

bool CheckIfCorrect(IGraph<int> &G, std::map<std::pair<int, int>, int> &flow, int days_amount)
{
	std::vector<std::pair<int, int>> next;
	G.GetNextVertices(0, next);
	for (int i = 0; i < next.size(); i++)
	{
		if (flow[{0, next[i].first}] != days_amount)
			return false;
	}
	return true;
}

int FindAmountOfDays(IGraph<int> &G, int tea_types_amount, int employee_amount)
{
	int max_days_amount = 0, min_days_amount = 0;
	for (int i = 1; i <= tea_types_amount; i++)
	{
		std::vector<std::pair<int, int>> next;
		G.GetNextVertices(i, next);
		max_days_amount += next[0].second;
	}
	std::map<std::pair<int, int>, int> flow, capacity;
	int cur_days_amount = max_days_amount / 2, answer = 0;
	bool correct = true;
	while (correct)
	{
		if (min_days_amount == max_days_amount)
			correct = false;
		capacity.clear();
		flow.clear();
		MakeCapacity(G, capacity, cur_days_amount);
		findMaxFlow(G, 0, G.VerticesCount() - 1, capacity, flow);
		if (CheckIfCorrect(G, flow, cur_days_amount))
		{
			answer = cur_days_amount;
			min_days_amount = cur_days_amount + 1;
		}
		else
		{
			max_days_amount = cur_days_amount;
		}
		cur_days_amount = (max_days_amount + min_days_amount) / 2;
	}
	return answer;
}

int main()
{
	int n, m;
	std::cin >> n >> m;
	int size = n + m + 2;
	ListGraph<int> G(size);
	for (int i = 1; i <= m; i++)
	{
		int tea_amount;
		std::cin >> tea_amount;
		G.AddEdge(i, n + m + 1, tea_amount);
	}
	for (int i = 1; i <= n; i++)
	{
		int temp_size, tea_number;
		std::cin >> temp_size;
		for (int j = 0; j < temp_size; j++)
		{
			std::cin >> tea_number;
			G.AddEdge(m + i, tea_number, 9000000);
		}
		G.AddEdge(0, m + i, 0);
	}
	std::cout << FindAmountOfDays(G, m, n) << std::endl;
	system("pause");
}