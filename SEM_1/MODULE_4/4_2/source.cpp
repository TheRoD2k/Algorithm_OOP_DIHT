/*Дано невзвешенное дерево. 
Расстоянием между двумя вершинами будем называть количество ребер в пути, 
соединяющем эти две вершины. 
Для каждой вершины определите сумму расстояний до всех остальных вершин. 
Время работы должно быть O(n).

Формат ввода
В первой строке записано количество вершин n ≤ 10000. 
Затем следует n - 1 строка, описывающая ребра дерева. 
Каждое ребро - это два различных целых числа - индексы вершин 
в диапазоне [0, n-1]. Индекс корня – 0. 
В каждом ребре родительской вершиной является та, чей номер меньше.

Формат вывода
Выход должен содержать n строк. 
В i-ой строке выводится сумма расстояний от i-ой вершины до всех остальных.


INPUT:
3
0 1
1 2

OUTPUT:
3
2
3

*/


#include <iostream>
#include <vector>


long long Min(int x, int y)
{
	if (x < y)
		return x;
	return y;
}

long long Max(int x, int y)
{
	if (x > y)
		return x;
	return y;
}

long long CalculateWaysAndChildren(std::vector<std::vector<int>> &Tree, int ind, long long curway, long long children, std::vector<int> &ways, std::vector<int> &chldrn)
{
	++curway;
	long long tempchildren = 0;
	for (int i = 0; i < Tree[ind].size(); i++)
	{
		tempchildren += CalculateWaysAndChildren(Tree, Tree[ind][i], curway, children, ways, chldrn);
		++tempchildren;
	}
	ways[ind] = curway;
	chldrn[ind] = tempchildren;
	return tempchildren;
}

void FormAnswer(std::vector<std::vector<int>> &Tree, int ind, int prevind, std::vector<int> &chldrn, std::vector<long long> &answer)
{
	answer[ind] = answer[prevind] - (chldrn[ind] + 1) + (Tree.size() - (chldrn[ind] + 1));
	for (int i = 0; i < Tree[ind].size(); i++)
	{
		FormAnswer(Tree, Tree[ind][i], ind, chldrn, answer);
	}
}

long long GetSumOfWays(std::vector<std::vector<int>> &Tree, std::vector<long long> &answer)
{
	int N = Tree.size();
	std::vector<int> ways(N), chldrn(N);
	long long temp = 0, sum = 0, children = 0;

	CalculateWaysAndChildren(Tree, 0, -1, children, ways, chldrn);

	for (int i = 0; i < N; i++)
		sum += ways[i];

	answer[0] = sum;
	for (int i = 0; i < Tree[0].size(); i++)
	{
		FormAnswer(Tree, Tree[0][i], 0, chldrn, answer);
	}

	return sum;
}

int main()
{
	int N;
	std::cin >> N;
	std::vector<std::vector<int>> Tree(N);
	std::vector<long long> answer(N);
	
	for (int i = 0; i < N - 1; i++)
	{
		int x, y;
		std::cin >> x >> y;
		Tree[Min(x, y)].push_back(Max(x, y));
	}

	GetSumOfWays(Tree, answer);
	for (int i = 0; i < N; i++)
		std::cout << answer[i] << std::endl;

	system("pause");
}