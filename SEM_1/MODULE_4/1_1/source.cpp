/*Реализуйте структуру данных типа “множество строк” на основе 
динамической хеш-таблицы с открытой адресацией. 
Хранимые строки непустые и состоят из строчных латинских букв. 
Хеш-функция строки должна быть реализована с помощью вычисления 
значения многочлена методом Горнера. 
Начальный размер таблицы должен быть равным 8-ми. 
Перехеширование выполняйте при добавлении элементов в случае, 
когда коэффициент заполнения таблицы достигает 3/4. 
Структура данных должна поддерживать операции добавления строки в множество, 
удаления строки из множества и проверки принадлежности данной строки множеству. 1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки. 1_2. Для разрешения коллизий используйте двойное хеширование.

Формат ввода
Каждая строка входных данных задает одну операцию над множеством. 
Запись операции состоит из типа операции и следующей за ним через пробел строки, 
над которой проводится операция. 
Тип операции – один из трех символов: 
+ означает добавление данной строки в множество; 
- означает удаление строки из множества; 
? означает проверку принадлежности данной строки множеству. 
При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. 
При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.

Формат вывода
Программа должна вывести для каждой операции одну из двух строк OK или FAIL, 
в зависимости от того, встречается ли данное слово в нашем множестве.


INPUT:
+ hello
+ bye
? bye
+ bye
- bye
? bye
? hello

OUTPUT:
OK
OK
OK
FAIL
OK
FAIL
OK

ПЕРЕДЕЛАТЬ УДАЛЕНИЕ, В НЕМ ОШИБКА! (Исправленный код утрачен)*/

#include <iostream>
#include <string>
#include <cassert>

template <typename T>
struct StructType
{
	T element = "";
	bool wasDeleted = false;
};

template <typename T>
class HashTable
{
public:
	HashTable() = default;
	~HashTable() { delete[] table; }
	HashTable(HashTable<T> const &ht);
	HashTable(HashTable<T> const &&ht);

	bool Insert(T const &str);
	bool Delete(T const &str);
	bool Check(T const &str);

private:
	int64_t Hash(T const &str);
	void ReHash();
	double SizeStat() { return double(size) / double(buffsize); }

	int64_t a = 10001;
	int64_t buffsize = 8;
	int64_t size = 0;

	StructType<T> *table = new StructType<T>[8];
};

template <typename T>
HashTable<T>::HashTable(HashTable<T> const &ht)
{
	buffsize = ht.buffsize;
	size = ht.size;
	a = ht.a;
	table = new StructType<T>[buffsize];
	for (int64_t i = 0; i < size; i++)
		table[i] = ht.table[i];
}

template <typename T>
HashTable<T>::HashTable(HashTable<T> const &&ht)
{
	buffsize = ht.buffsize;
	size = ht.size;
	a = ht.a;
	table = new StructType<T>[buffsize];
	for (int64_t i = 0; i < size; i++)
		table[i] = ht.table[i];
}

template <typename T>
bool HashTable<T>::Insert(T const &str)
{
	int64_t temp = Hash(str);
	int64_t remembered = -1;
	for (int64_t i = 0; i < buffsize; i++)
	{
		temp = (temp + i) % buffsize;
		if (table[temp].element == "")
		{
			if (table[temp].wasDeleted == true)
			{
				if (remembered == -1)
					remembered = temp;
			}
			else
			{
				if (remembered != -1)
					temp = remembered;
				table[temp].element = str;
				table[temp].wasDeleted = false;
				++size;
				ReHash();
				return true;
			}
		}
		if (table[temp].element == str)
		{
			return false;
		}
	}

	if (remembered != -1)
	{
		table[remembered].element = str;
		table[remembered].wasDeleted = false;
		return true;
	}

	return false;
}

template <typename T>
bool HashTable<T>::Delete(T const &str)
{
	int64_t temp = Hash(str);
	for (int64_t i = 0; i < buffsize; i++)
	{
		temp = (temp + i) % buffsize;
		if (table[temp].element == "" && table[temp].wasDeleted == false)
		{
			return false;
		}
		if (table[temp].element == str)
		{
			table[temp].wasDeleted = true;
			table[temp].element = "";
			--size;
			return true;
		}
	}

	return false;
}

template <typename T>
bool HashTable<T>::Check(T const &str)
{
	int64_t temp = Hash(str);
	for (int64_t i = 0; i < buffsize; i++)
	{
		temp = (temp + i) % buffsize;
		if (table[temp].element == "" && table[temp].wasDeleted == false)
		{
			return false;
		}
		if (table[temp].element == str)
		{
			return true;
		}
	}

	return false;
}

template <typename T>
int64_t HashTable<T>::Hash(T const &str)
{
	int64_t temp = 0;
	for (int64_t i = 0; i < str.size(); i++)
	{
		temp *= a;
		temp += str[i];
		temp %= buffsize;
	}

	return temp;
}

template <typename T>
void HashTable<T>::ReHash()
{
	if (SizeStat() < 0.75)
		return;
	size = 0;
	StructType<T> *temp = table;
	table = new StructType<T>[buffsize * 2];

	buffsize *= 2;
	for (int64_t i = 0; i < buffsize/2; i++)
		if (temp[i].element != "" && temp[i].wasDeleted != true)
			Insert(temp[i].element);

	delete[] temp;
}

int main()
{
	HashTable<std::string> ht;
	
	std::string str;
	while (std::getline(std::cin,str))
	{
		char c = str[0];
		for (int64_t i = 0; i < str.size() - 2; i++)
		{
			str[i] = str[i + 2];
		}

		str.pop_back();
		str.pop_back();

		if (c == '+')
			std::cout << (ht.Insert(str) ? "OK" : "FAIL") << std::endl;

		if (c == '-')
			std::cout << (ht.Delete(str) ? "OK" : "FAIL") << std::endl;

		if (c == '?')
			std::cout << (ht.Check(str) ? "OK" : "FAIL") << std::endl;
	}
	
	system("pause");
}