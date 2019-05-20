/*2_4 На числовой прямой окрасили N отрезков.
Известны координаты левого и правого концов каждого отрезка [Li, Ri].
Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.
N ≤ 10000. Li, Ri — целые числа в диапазоне [0, 10^9].\

INPUT:
3
1 4 
7 8
2 5

OUTPUT:
3

*/


#include <iostream>



struct Point
{
	int x;
	bool closing;
	const bool operator<(const Point &r)
	{
		return (this->x < r.x || (this->x == r.x && !this->closing && r.closing));
	}

};

void swp(Point &l, Point &r)
{
	Point temp = l;
	l = r;
	r = temp;
}

void SiftDown(Point *arr, int heapsize, int ind)
{
	while (2 * ind + 1 < heapsize)
	{
		int left = 2 * ind + 1;
		int right = 2 * ind + 2;

		if (arr[ind] < arr[left])
		{
			if (right < heapsize)
				if (arr[right] < arr[left])
				{
					swp(arr[left], arr[ind]);
					ind = left;
				}
				else
				{
					swp(arr[right], arr[ind]);
					ind = right;
				}
			else
			{
				swp(arr[left], arr[ind]);
				ind = left;
			}
		}
		else
		{
			if (right < heapsize && arr[ind] < arr[right])
			{
				swp(arr[ind], arr[right]);
				ind = right;
			}
			else
				ind = heapsize;
		}

	}
}

void BuildHeap(Point *arr, int heapsize)
{
	for (int i = heapsize / 2; i >= 0; i--)
		SiftDown(arr, heapsize, i);
}

void Heapsort(Point *arr, int heapsize)
{
	BuildHeap(arr, heapsize);
	int theapsize = heapsize;
	for (int i = 0; i < heapsize; i++)
	{
		swp(arr[0], arr[theapsize - 1]);
		--theapsize;
		SiftDown(arr, theapsize, 0);
	}
}

int CalculateMixColored(Point *arr, int N)
{
	int prevPos = arr[0].x;
	int colors = 1;
	int toAdd = 0;
	int count = 0;
	for (int i = 1; i < 2 * N; i++)
	{
		if (arr[i].x == prevPos)
		{
			if (arr[i].closing)
				--toAdd;
			else
				++toAdd;
		}
		else
		{
			colors += toAdd;
			if (colors == 1)
			{
				count += arr[i].x - prevPos;
			}
			toAdd = 0;
			if (arr[i].closing)
				--toAdd;
			else
				++toAdd;
			prevPos = arr[i].x;
		}
	}

	return count;
}

int main()
{
	int N;
	std::cin >> N;
	Point *arr = new Point[2 * N];

	for (int i = 0; i < N; i++)
	{
		int x1, x2;
		std::cin >> x1 >> x2;
		arr[2 * i].x = x1;
		arr[2 * i].closing = false;
		arr[2 * i + 1].x = x2;
		arr[2 * i + 1].closing = true;
	}
	
	Heapsort(arr, 2*N);
	int answer = CalculateMixColored(arr, N);
    delete[] arr;
	std::cout << answer << std::endl;
	system("pause");
}