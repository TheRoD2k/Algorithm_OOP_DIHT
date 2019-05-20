/* Дано N кубиков. 
Требуется определить каким количеством способов можно 
выстроить из этих кубиков пирамиду.

Формат ввода
На вход подается количество кубиков N.

Формат вывода
Вывести число различных пирамид из N кубиков.

INPUT:
3

OUTPUT:
3

*/


#include <iostream>
#include <vector>

long long GetPossibleAmount(long long K,long long N, std::vector<std::vector<long long>> &ivec)
{
	if (K <= 0 || N < 0)
		return 0;
	if (N == 0)
		return 1;
	if (ivec[K][N] != -1)
		return ivec[K][N];
	long long a = GetPossibleAmount(K - 1, N,ivec);
	long long b = GetPossibleAmount(K, N - K,ivec);
	ivec[K][N] = a + b;
	return a + b;
}

int main()
{
	long long N;
	std::cin >> N;
	long long count = 0;
	std::vector<std::vector<long long>> ivec(N+1,std::vector<long long> (N+1,-1));
	for (long long i = 1; i <= N; i++)
	{
		for (long long j = 1; j <= N; j++)
			GetPossibleAmount(i, j, ivec);
	}
	std::cout << ivec[N][N] << std::endl;
	system("pause");
}