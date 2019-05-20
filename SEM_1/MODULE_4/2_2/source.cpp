/*В большой IT-фирме есть только одна переговорная комната. 
Желающие посовещаться заполняют заявки с желаемым временем начала и конца. 
Ваша задача определить максимальное количество заявок, 
которое может быть удовлетворено. Число заявок ≤ 100000.

Формат ввода
Вход содержит только пары целых чисел — начала и концы заявок.

Формат вывода
Выход должен содержать натуральное число — максимальное число заявок.


INPUT:
1 2
2 5
7 10
2 3
6 7
4 7
3 6
3 5
1 2
4 5

OUTPUT:
5

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct request
{
	request() = default;
	request(int b, int e) : beg(b), end(e), dif(e-b) {}
	int beg, end, dif;
};

bool IsBetter(request const &l, request const &r)
{
	if (l.end < r.end)
		return true;
	if (l.end == r.end && l.dif < r.dif)
		return true;
	return false;
}

int CountMaxRequests(std::vector<request> &rvec)
{
	int prev = 0, count = 0;
	std::sort(rvec.begin(),rvec.end(), IsBetter);
	for (auto i = rvec.begin(); i != rvec.end(); i++)
	{
		if (i->beg >= prev)
		{
			++count;
			prev = i->end;
			//std::cout << i->beg << " " << i->end << std::endl;
		}
	}
	return count;
}

int main()
{
	std::ifstream input("input.txt");
	std::ofstream output("output.txt");
	
	int temp;
	std::vector<request> rvec;
	while (input >> temp)
	{
		int beg = temp;
		int end;
		input >> end;
		rvec.push_back(request(beg, end));
	}
	
	output << CountMaxRequests(rvec) << std::endl;

	system("pause");
}