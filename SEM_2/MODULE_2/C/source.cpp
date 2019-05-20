/*Задача A. Восьминашки
Имя входного файла: puzzle.in
Имя выходного файла: puzzle.out
Ограничение по времени: 1 секунда
Ограничение по памяти: 256 мегабайт
«Восьминашки» – упрощенный вариант известной головоломки «Пятнашки». 
Восемь костяшек,
пронумерованных от 1 до 8, расставлены по ячейкам игровой доски 3 на 3, 
одна ячейка при этом
остается пустой. За один ход разрешается передвинуть одну из костяшек, 
расположенных рядом с
пустой ячейкой, на свободное место. 
Цель игры – для заданной начальной конфигурации игровой
доски за минимальное число ходов получить выигрышную конфигурацию 
(пустая ячейка обозначена
нулем):
1 2 3
4 5 6
7 8 0
Формат входного файла
Во входном файле содержится начальная конфигурация головоломки – 3 строки по 3 числа в
каждой.
Формат выходного файла
Если решение существует, то в первой строке выходного файла выведите минимальное число
перемещений костяшек, которое нужно сделать, чтобы достичь выигрышной конфигурации, а во
второй строке выведите соответствующую последовательность ходов: L означает, что в результате
перемещения костяшки пустая ячейка сдвинулась влево, R – вправо, U – вверх, D – вниз. Если таких
последовательностей несколько, то выведите любую из них. Если же выигрышная конфигурация
недостижима, то выведите в выходной файл одно число −1.


INPUT:
0 1 6
4 3 2
7 5 8

OUTPUT:
8
RDRULDDR

*/



#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <algorithm>

std::vector<int> tens{ 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

class State
{
public:
	State() : _status(8765321) {};
	State(int stat) : _status(stat) {}
	bool operator==(const State &other) { return _status == other._status; }
	bool IsNeeded() { return _status == 87654321; }

	std::vector<std::pair<State, char>> GetNextStates() const;
private:
	int _status;
	State _MoveUp() const;
	State _MoveDown() const;
	State _MoveLeft() const;
	State _MoveRight() const;
	int _MoveZero(int number_to_replace) const;
	int _GetZeroPosition() const;
	State _ReversedMove(char c) const;

	friend std::string GetAnswer(std::map<State, char> &known);
	friend bool operator<(const State &left, const State &right);
};

bool operator<(const State &left, const State &right)
{
	return left._status < right._status;
}

int State::_MoveZero(int number_to_replace) const
{
	int new_status = 0, temp_status = _status;
	for (int i = 0; i < 9; i++)
	{
		int new_number = temp_status % 10;
		if (new_number == 0)
			new_number = number_to_replace;
		else
			if (new_number == number_to_replace)
				new_number = 0;
		new_status += tens[i] * new_number;
		temp_status /= 10;
	}
	return new_status;
}

int State::_GetZeroPosition() const
{
	int temp_status = _status, zero_position = 0;
	for (int i = 0; i < 9; i++)
	{
		if (temp_status % 10 == 0)
			break;
		temp_status /= 10;
		++zero_position;
	}
	return zero_position;
}

std::vector<std::pair<State, char>> State::GetNextStates() const
{
	int zero_position = _GetZeroPosition();
	std::vector<std::pair<State, char>> next;
	if ((zero_position + 1) % 3 != 0)
		next.push_back({ _MoveRight(), 'R' });
	if (zero_position % 3 != 0)
		next.push_back({ _MoveLeft(), 'L' });
	if (zero_position >= 3)
		next.push_back({ _MoveUp(), 'U' });
	if (zero_position <= 5)
		next.push_back({ _MoveDown(), 'D' });
	return next;
}

State State::_MoveUp() const
{
	int zero_position = _GetZeroPosition();
	int number_to_replace = (_status / tens[zero_position - 3]) % 10;
	int new_status = _MoveZero(number_to_replace);
	return State(new_status);
}

State State::_MoveDown() const
{
	int zero_position = _GetZeroPosition();
	int number_to_replace = (_status / tens[zero_position + 3]) % 10;
	int new_status = _MoveZero(number_to_replace);
	return State(new_status);
}

State State::_MoveLeft() const
{
	int zero_position = _GetZeroPosition();
	int number_to_replace = (_status / tens[zero_position - 1]) % 10;
	int new_status = _MoveZero(number_to_replace);
	return State(new_status);
}

State State::_MoveRight() const
{
	int zero_position = _GetZeroPosition();
	int number_to_replace = (_status / tens[zero_position + 1]) % 10;
	int new_status = _MoveZero(number_to_replace);
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

std::string GetAnswer(std::map<State, char> &known)
{
	State curState(87654321);
	std::string answer;
	while (known[curState] != '-')
	{
		char step = known[curState];
		answer += step;
		curState = curState._ReversedMove(step);
	}
	std::reverse(answer.begin(), answer.end());
	return answer;
}

std::string FindWay(const State &startState)
{
	std::queue<State> q;
	std::map<State, char> known;
	known[startState] = '-';
	q.push(startState);
	State curState;
	std::pair<State, char> final;
	while (!q.empty())
	{
		curState = q.front();
		if (curState.IsNeeded())
			break;
		q.pop();
		auto next = curState.GetNextStates();
		for (int i = 0; i < next.size(); i++)
		{
			if (known.find(next[i].first) == known.end())
			{
				known[next[i].first] = next[i].second;
				q.push(next[i].first);
			}
		}
	}
	if (q.empty())
		return "NOANSWER";
	return GetAnswer(known);
}

int main()
{
	std::ifstream T("puzzle.in");
	std::ofstream To("puzzle.out");

	int input, suminp = 0;
	for (int i = 0; i < 9; i++)
	{
		T >> input;
		suminp += tens[i] * input;
	}

	std::string way = FindWay(State(suminp));
	if (way == "NOANSWER")
		To << -1 << std::endl;
	else
	{
		To << way.size() << std::endl;
		To << way << std::endl;
	}
	return 0;
}