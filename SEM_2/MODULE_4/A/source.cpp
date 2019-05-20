/*Дано число N и последовательность из N целых чисел. 
Найти вторую порядковую статистику на заданных диапазонах.

Для решения задачи используйте структуру данных Sparse Table. 
Требуемое время обработки каждого диапазона O(1). 
Время подготовки структуры данных .

Формат ввода
В первой строке заданы 2 числа: размер последовательности N и количество диапазонов M.
Следующие N целых чисел задают последовательность. Далее вводятся M пар чисел - границ диапазонов.
Гарантируется, что каждый диапазон содержит как минимум 2 элемента.
Формат вывода
Для каждого из M диапазонов напечатать элемент последовательности - 2ю порядковую статистику. 
По одному числу в строке.


INPUT:
10 3
1 2 3 4 5 6 7 8 9 10
1 2
1 10
2 7

OUTPUT:
2
2
3

*/



#include <iostream>
#include <algorithm>


template <class T>
class  Vector
{
public:
	Vector();
	Vector(unsigned int size);
	Vector(unsigned int size, const T & initial);
	Vector(const Vector<T> & v);
	~Vector();

	unsigned int capacity() const;
	unsigned int size() const;
	bool empty() const;
	T & front();
	T & back();
	void push_back(const T & value);
	void pop_back();

	void reserve(unsigned int capacity);
	void resize(unsigned int size);

	T & operator[](unsigned int index);
	Vector<T> & operator=(const Vector<T> &);
	bool operator==(const Vector<T> &);
	void clear();
private:
	unsigned int _size;
	unsigned int _buffersize;
	T * buffer;
};

template<class T>
bool Vector<T>::operator==(const Vector<T> &vec)
{	
	if (size() != vec.size())
    	return false;
	for (int i = 0; i < vec.size() && i < size(); i++)
    	if (vec.buffer[i] != buffer[i])
    		return false;
}

template<class T>
Vector<T>::Vector()
{
	_buffersize = 0;
	_size = 0;
	buffer = 0;
}

template<class T>
Vector<T>::Vector(const Vector<T> & v)
{
	_size = v._size;
	_buffersize = v._buffersize;
	buffer = new T[_size];
	for (unsigned int i = 0; i < _size; i++)
		buffer[i] = v.buffer[i];
}

template<class T>
Vector<T>::Vector(unsigned int size)
{
	_buffersize = size;
	_size = size;
	buffer = new T[size];
}

template<class T>
Vector<T>::Vector(unsigned int size, const T & initial)
{
	_size = size;
	_buffersize = size;
	buffer = new T[size];
	for (unsigned int i = 0; i < size; i++)
		buffer[i] = initial;
}

template<class T>
Vector<T> & Vector<T>::operator = (const Vector<T> & v)
{
	delete[] buffer;
	_size = v._size;
	_buffersize = v._buffersize;
	buffer = new T[_size];
	for (unsigned int i = 0; i < _size; i++)
		buffer[i] = v.buffer[i];
	return *this;
}

template<class T>
T& Vector<T>::front()
{
	return buffer[0];
}

template<class T>
T& Vector<T>::back()
{
	return buffer[_size - 1];
}

template<class T>
void Vector<T>::push_back(const T & v)
{
	if (_size >= _buffersize)
		reserve((_buffersize + 1) * 2);
	buffer[_size++] = v;
}

template<class T>
void Vector<T>::pop_back()
{
	_size--;
}

template<class T>
void Vector<T>::reserve(unsigned int capacity)
{
	if (buffer == 0)
	{
		_size = 0;
		_buffersize = 0;
	}
	T * Newbuffer = new T[capacity];
	unsigned int l_Size = capacity < _size ? capacity : _size;

	for (unsigned int i = 0; i < l_Size; i++)
		Newbuffer[i] = buffer[i];

	_buffersize = capacity;
	delete[] buffer;
	buffer = Newbuffer;
}

template<class T>
unsigned int Vector<T>::size()const
{
	return _size;
}

template<class T>
void Vector<T>::resize(unsigned int size)
{
	reserve(size);
	_size = size;
}

template<class T>
T& Vector<T>::operator[](unsigned int index)
{
	return buffer[index];
}

template<class T>
unsigned int Vector<T>::capacity()const
{
	return _buffersize;
}

template<class T>
Vector<T>::~Vector()
{
	delete[] buffer;
}
template <class T>
void Vector<T>::clear()
{
	_buffersize = 0;
	_size = 0;
	buffer = nullptr;
}

template<typename T1, typename T2>
class Pair
{
public:
	Pair(T1 v1, T2 v2) : first(v1), second(v2) {}
	Pair() = default;
	T1 first;
	T2 second;
};

template <typename T1, typename T2>
Pair<T1, T2> MakePair(T1 v1, T2 v2) {  return Pair<T1, T2>(v1, v2);  }

class sparseTable
{
public:
	sparseTable(int N) : _size(N) { _data = Vector<Vector<Pair<Pair<int, int>,
		Pair<int, int>>>>(N, Vector<Pair<Pair<int, int>, Pair<int, int>>>(calculate_log(N))); };
	sparseTable(Vector<int> vals);

	int get_min(int from, int to);
private:
	int calculate_log(int N) const;
	int power(int base, int deg) const;
	int _size = 0;
	Vector<Vector<Pair<Pair<int, int>, Pair<int, int>>>> _data;
};


int sparseTable::power(int base, int deg) const
{
	int temp = 1;
	while (deg > 0)
	{
		if (deg % 2 == 0)
		{
			deg /= 2;
			base *= base;
		}
		else
		{
			--deg;
			temp *= base;
		}
	}
	return temp;
}


Pair<Pair<int, int>, Pair<int, int>> FindNeededStat(Pair<int, int> min1, Pair<int, int> min2, Pair<int, int> min3, Pair<int, int> min4)
{
	Vector<Pair<int, int>> vec;
	vec.push_back(min1);
	vec.push_back(min2);
	vec.push_back(min3);
	vec.push_back(min4);
	for (int i = 0; i < vec.size(); i++)
		for (int j = i+1; j < vec.size(); j++)
		{
			if (vec[i].first > vec[j].first)
			{
				auto temp = vec[i];
				vec[i] = vec[j];
				vec[j] = temp;
			}
		}
	Vector<Pair<int, int>> needed;
	needed.push_back(vec[0]);
	for (int i = 1; i < vec.size(); i++)
		if (vec[i - 1].second != vec[i].second)
			needed.push_back(vec[i]);
	return { needed[0], needed[1] };
}

sparseTable::sparseTable(Vector<int> vals)
{
	_size = vals.size();
	int x_size = vals.size(), y_size = calculate_log(vals.size()) + 1;
	_data = Vector<Vector<Pair<Pair<int, int>, Pair<int, int>>>>(x_size, Vector<Pair<Pair<int, int>, Pair<int, int>>>(y_size+1));
	for (int i = 0; i < _data.size(); i++)
		_data[i][0] = MakePair(MakePair( vals[i], i ), MakePair(900000000, -1) );
	for (int j = 1; j < y_size+1; j++)
		for (int i = 0; i < x_size; i++)
		{	
			if (i + power(2, j - 1) < x_size)
			{	
				auto left = _data[i][j - 1], right = _data[i + power(2, j - 1)][j - 1];
				Pair<int, int> min1 = left.first, min2 = left.second, min3 = right.first, min4 = right.second;
				auto ans = FindNeededStat(min1, min2, min3, min4);

				_data[i][j] = { ans.first, ans.second };
			}
			else _data[i][j] = _data[i][j - 1];
		}
}

int sparseTable::calculate_log(int N) const
{
	if (N == 1)
		return 0;
	else
		return calculate_log(N/2) + 1;
}

int sparseTable::get_min(int from, int to)
{
	int log = calculate_log(to - from + 1);
	auto left = _data[from][log], right = _data[to - power(2, log) + 1][log];
	Pair<int, int> min1 = left.first, min2 = left.second, min3 = right.first, min4 = right.second;
	auto ans = FindNeededStat(min1, min2, min3, min4);
	return ans.second.first;
}

int main()
{	
	int N, M;
	std::cin >> N >> M;
	Vector<int> vals(N);
	for (int i = 0; i < N; i++)
	{
		std::cin >> vals[i];
	}
	sparseTable temp(vals);
	
	for (int i = 0; i < M; i++)
	{
		int from, to;
		std::cin >> from >> to;
		std::cout << temp.get_min(from - 1, to - 1) << std::endl;
	}
	
	system("pause");
}