/*Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду: [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ], где 0 задает пустую ячейку. Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.

Формат ввода
Начальная расстановка.

Формат вывода
Если вам удалось найти решение, 
то в первой строке файла выведите число перемещений, 
которое требуется сделать в вашем решении. 
А во второй строке выведите соответствующую последовательность ходов: 
L означает, что в результате перемещения костяшки пустая ячейка сдвинулась влево, 
R – вправо, U – вверх, D – вниз. Если же выигрышная конфигурация недостижима, 
то выведите в выходной файл одно число −1.


INPUT:
1 2 3 0 5 6 7 4 9 10 11 8 13 14 15 12 

OUTPUT:
3

*/



#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <algorithm>
#include <set>

std::vector<unsigned long long> tens{ 0x1, 0x10, 0x100,
0x1000, 0x10000, 0x100000, 0x1000000, 0x10000000, 0x100000000,
0x1000000000, 0x10000000000, 0x100000000000, 0x1000000000000,
0x10000000000000, 0x100000000000000, 0x1000000000000000};
unsigned long long needed = 0x0FEDCBA987654321;

unsigned long long ABSdifference(int left, int right)
{
	int lefty = left / 4, leftx = left % 4, righty = right / 4, rightx = right % 4;
	if (lefty < righty)
		std::swap(lefty, righty);
	if (leftx < rightx)
		std::swap(leftx, rightx);
	return leftx - rightx + lefty - righty;
}
class State
{
public:
	State() = default;
	State(unsigned long long stat) : _status(stat) {}
	bool operator==(const State &other) { return _status == other._status; }
	bool IsNeeded() const;

	std::vector<std::pair<State, char>> GetNextStates() const;
	void Print()
	{
		unsigned long long temp_status = _status;
		for (int i = 0; i < 16; i++)
		{
			std::cout << temp_status % 16u << " ";
			temp_status /= 16u;
			if ((i + 1) % 4 == 0)
				std::cout << std::endl;
		}
		std::cout << std::endl << std::endl;
	}
private:
	unsigned long long _status;
	State _MoveUp() const;
	State _MoveDown() const;
	State _MoveLeft() const;
	State _MoveRight() const;
	unsigned long long _MoveZero(unsigned long long number_to_replace) const;
	unsigned long long _GetZeroPosition() const;
	State _ReversedMove(char c) const;

	friend std::string GetAnswer(std::map<State, char> &known);
	friend bool operator<(const State &left, const State &right);
	friend int Heuristic(const State &state);
	friend std::string FindWay(const State &startState);
};

bool State::IsNeeded() const
{
	return _status == needed;
}

bool operator<(const State &left, const State &right)
{
	/*if (Heuristic(left) == Heuristic(right))
		return Heuristic(left) < Heuristic(right);
	else
		return left._status < right._status;*/
	return left._status > right._status;
}

unsigned long long State::_MoveZero(unsigned long long number_to_replace) const
{
	unsigned long long new_status = 0u, temp_status = _status;
	for (int i = 0; i < 16; i++)
	{
		unsigned long long new_number = temp_status % 16u;
		if (new_number == 0u)
			new_number = number_to_replace;
		else
			if (new_number == number_to_replace)
				new_number = 0u;
		new_status += tens[i] * new_number;
		temp_status /= 16u;
	}
	return new_status;
}

unsigned long long State::_GetZeroPosition() const
{
	unsigned long long temp_status = _status, zero_position = 0;
	for (int i = 0; i < 16; i++)
	{
		if (temp_status % 16u == 0)
			break;
		temp_status /= 16u;
		++zero_position;
	}
	return zero_position;
}

std::vector<std::pair<State, char>> State::GetNextStates() const
{
	//std::cout << _status << std::endl;
	unsigned long long zero_position = _GetZeroPosition();
	std::vector<std::pair<State, char>> next;
	if ((zero_position + 1u) % 4u != 0u)
		next.push_back({ _MoveRight(), 'R' });
	if (zero_position % 4u != 0u)
		next.push_back({ _MoveLeft(), 'L' });
	if (zero_position >= 4u)
		next.push_back({ _MoveUp(), 'U' });
	if (zero_position <= 11u)
		next.push_back({ _MoveDown(), 'D' });
	return next;
}

State State::_MoveUp() const
{
	unsigned long long zero_position = _GetZeroPosition();
	unsigned long long number_to_replace = (_status / tens[zero_position - 4u]) % 16u;
	unsigned long long new_status = _MoveZero(number_to_replace);
	return State(new_status);
}

State State::_MoveDown() const
{
	unsigned long long zero_position = _GetZeroPosition();
	unsigned long long number_to_replace = (_status / tens[zero_position + 4u]) % 16u;
	unsigned long long new_status = _MoveZero(number_to_replace);
	return State(new_status);
}

State State::_MoveLeft() const
{
	unsigned long long zero_position = _GetZeroPosition();
	unsigned long long number_to_replace = (_status / tens[zero_position - 1u]) % 16u;
	unsigned long long new_status = _MoveZero(number_to_replace);
	return State(new_status);
}

State State::_MoveRight() const
{
	unsigned long long zero_position = _GetZeroPosition();
	unsigned long long number_to_replace = (_status / tens[zero_position + 1u]) % 16u;
	unsigned long long new_status = _MoveZero(number_to_replace);
	return State(new_status);
}

State State::_ReversedMove(char c) const
{
	if (c == 'L')
		return _MoveRight();
	if (c == 'R')
		return _MoveLeft();
	if (c == 'U')
		return _MoveDown();
	if (c == 'D')
		return _MoveUp();
}

int Heuristic(const State &state)
{
	State perfect(needed);

	unsigned long long temp_status = state._status, heuristic = 0u;
	for (int i = 0; i < 16; i++)
	{
		if (temp_status % 16u != 0u)
			heuristic += ABSdifference(i, (temp_status-1) % 16);//sraka;
		temp_status /= 16u;
	}
	return heuristic;
}

std::string GetAnswer(std::map<State, char> &known)
{
	State curState(needed);
	std::string answer;
	while (known[curState] != '-')
	{
		char step = known[curState];
		answer += step;
		curState = curState._ReversedMove(step);
	}
	std::reverse(answer.begin(), answer.end());
	//std::cout << "ANSWER ORIGINAL: " << answer << std::endl;
	std::string newAnswer = "";
	for (int i = 0; i < answer.size(); i++)
	{
		char c = answer[i];
		if (c == 'R')
			c = 'L';
		else
			if (c == 'L')
				c = 'R';
		if (c == 'U')
			c = 'D';
		else
			if (c == 'D')
				c = 'U';
		newAnswer += c;
	}
	//std::cout << "ANSWER: " << newAnswer << std::endl;
	return newAnswer;
}



std::string FindWay(const State &startState)
{
	std::priority_queue<std::pair<int, State>> q;
	std::map<State, char> known;
	known[startState] = '-';
	q.push({ -Heuristic(startState), startState });
	State curState;
	int ccc = 0;
	while (!q.empty())
	{
		curState = q.top().second;
		if (curState.IsNeeded())
			break;
		q.pop();
		auto next = curState.GetNextStates();
		/*if (ccc < 20)
		{
			++ccc;
			std::cout << Heuristic(curState) << std::endl;
			curState.Print();
		}*/
		//std::cout << std::endl;
		for (int i = 0; i < next.size(); i++)
		{
			//next[i].first.Print();
			if (known.count(next[i].first) == 0)
			{
				known[next[i].first] = next[i].second;
				q.push({ -Heuristic(next[i].first), next[i].first });
				//std::cout << "UNIQUE: ^" << std::endl;
			}
		}
		//std::cout << "=====================" << std::endl;
	}
	if (q.empty())
		return "NOANSWER";
	return GetAnswer(known);
}

int main()
{
	std::ifstream T("input.txt");
	std::ofstream To("output.txt");

	/*unsigned long long temp = 0;
	for (unsigned long long i = 1; i < 16; i++)
	{
		temp += i * tens[i - 1]; 
	}
	std::cout << temp << std::endl;*/
	unsigned long long input, suminp = 0;

	for (int i = 0; i < 16; i++)
	{
		T >> input;
		suminp += tens[i] * input;
	}
	//std::cout << Heuristic(State(suminp)) << std::endl;

	std::string way = FindWay(State(suminp));
	if (way == "NOANSWER")
		To << -1 << std::endl;
	else
	{
		To << way.size() << std::endl;
		To << way << std::endl;
	}
	system("pause");
	return 0;
}