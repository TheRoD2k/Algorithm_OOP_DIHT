/*Задано N точек на плоскости.
Указать (N−1)-звенную несамопересекающуюся незамкнутую ломаную,
проходящую через все эти точки.
Указание: стройте ломаную в порядке возрастания x-координаты. 
Если имеются две точки с одинаковой x-координатой,
то расположите раньше ту точку, у которой y-координата меньше.
 
INPUT:
4
0 0
1 1
1 0
0 1

OUTPUT:
0 0
0 1
1 0
1 1
*/

#include <iostream>

struct Point
{
	int x, y;
};

void InsertionSort(Point *arr, int N)
{
	for (int i = 1; i < N; i++)
	{
		int j = i;
		Point temp = arr[j];
		for (; j > 0 && (arr[j - 1].x > temp.x || (arr[j - 1].x == temp.x && arr[j - 1].y > temp.y)); j--)
		{
			arr[j] = arr[j - 1];
		}
		arr[j] = temp;
	}
}

int main()
{
	int N;
	std::cin >> N;
	Point *arr = new Point[N];
	for (int i = 0; i < N; i++)
	{
		int x, y;
		std::cin >> x >> y;
		arr[i].x = x;
		arr[i].y = y;
	}

	InsertionSort(arr, N);
	for (int i = 0; i < N; i++)
	{
		std::cout << arr[i].x << "\t" << arr[i].y << std::endl;
	}
	delete[] arr;
	system("pause");
}