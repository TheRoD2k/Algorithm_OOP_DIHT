/*Дано число N < 106 и последовательность пар целых чисел из [-231, 231] 
длиной N. Построить декартово дерево из N узлов, 
парами чисел (Xi, Yi). Каждая пара чисел (Xi, Yi) определяет ключ Xi 
и приоритет Yi в декартовом дереве. 
Добавление узла в декартово дерево выполняйте второй версией алгоритма, 
рассказанного на лекции: 
При добавлении узла (x, y) выполняйте спуск по ключу 
до узла P с меньшим приоритетом. 
Затем разбейте найденное поддерево по ключу x так, 
чтобы в первом поддереве все ключи меньше x, 
а во втором больше или равны x. 
Получившиеся два дерева сделайте дочерними для нового узла (x, y). 
Новый узел вставьте на место узла P. 
Построить также наивное дерево поиска по ключам Xi. 
Равные ключи добавляйте в правое поддерево. 
Вычислить количество узлов в самом широком слое декартового дерева 
и количество узлов в самом широком слое наивного дерева поиска. 
Вывести их разницу. Разница может быть отрицательна.


INPUT:
10
5 11
18 8
25 7
50 12
30 30
15 15
20 10
22 5
40 20
45 9

OUTPUT:
1

*/



#include <iostream>
#include <deque>

template <typename T>
struct Node
{	
	Node() = default;
	Node(T &val, int pri) : key(val), priority(pri) { left = nullptr; right = nullptr; }
	Node(T &val) : key(val) { left = nullptr; right = nullptr; }
	Node *left;
	Node *right;
	T key;
	int priority=0;
};


template <typename T>
class Treap
{
public:
	void Split(Node<T> *, T &, Node<T> *&, Node <T> *&);
	void Insert(T &, int);
	Node<T> *root = nullptr;
	int GetWidest();
};

template <typename T>
class BinTree
{
public:
	void Insert(T &);
	Node<T> *root = nullptr;
	int GetWidest();
};

template <typename T>
void Treap<T>::Split(Node<T> *curNode, T &key, Node<T> *&left, Node <T> *&right)
{
	if (curNode == nullptr) {
		left = nullptr;
		right = nullptr;
	}
	else if (curNode->key < key) {
		Split(curNode->right, key, curNode->right, right);
		left = curNode;
	}
	else {
		Split(curNode->left, key, left, curNode->left);
		right = curNode;
	}
}

template <typename T>
void Treap<T>::Insert(T &key, int priority)
{
	Node<T> *cur = root;
	Node<T> *prevcur = nullptr;
	while (cur != nullptr && cur->priority >= priority)
	{
		if (key < cur->key)
		{
			prevcur = cur;
			cur = cur->left;
		}
		else
		{
			prevcur = cur;
			cur = cur->right;
		}
	}

	Node<T> *newNode = new Node<T>(key, priority);
	if (root == nullptr)
	{
		root = newNode;
		return;
	}

	if (cur == nullptr)
	{
		if (key < prevcur->key)
			prevcur->left = newNode;
		else
			prevcur->right = newNode;
		return;
	}

	Node<T> *left, *right;
	Node<T> *savedCur = cur;
	Split(cur, key, left, right);
	newNode->left = left; newNode->right = right;
	if (prevcur == nullptr)
	{
		root = newNode;
		return;
	}
	else
		if (prevcur->key > key)
			prevcur->left = newNode;
		else
			prevcur->right = newNode;
}

template <typename T>
void BinTree<T>::Insert(T &key)
{
	if (root == nullptr)
	{
		Node<T> *newNode = new Node<T>(key);
		root = newNode;
		return;
	}
	Node<T> *prevcur = nullptr;
	Node<T> *cur = root;
	while(cur != nullptr)
	{
		if (key < cur->key)
		{
			prevcur = cur;
			cur = cur->left;
		}
		else
		{
			prevcur = cur;
			cur = cur->right;
		}
	}
	
	Node<T> *newNode = new Node<T>(key);
	if (key < prevcur->key)
		prevcur->left = newNode;
	else
		prevcur->right = newNode;
}

template <typename T>
int Treap<T>::GetWidest()
{
	int width = 0;
	if (root == nullptr)
		return 0;
	std::deque<Node<T> *> nodes;
	nodes.push_back(root);
	int curSize = 1;
	int maxSize = 1;
	while (!nodes.empty())
	{
		int temp = 0;
		for (int i = 0; i < curSize; i++)
		{
			if (nodes.front()->left != nullptr)
			{
				nodes.push_back(nodes.front()->left);
				++temp;
			}
			if (nodes.front()->right != nullptr)
			{
				nodes.push_back(nodes.front()->right);
				++temp;
			}
			nodes.pop_front();
		}
		curSize = temp;
		if (curSize > maxSize)
			maxSize = curSize;
	}
	return maxSize;
}

template <typename T>
int BinTree<T>::GetWidest()
{
	if (root == nullptr)
		return 0;
	std::deque<Node<T> *> nodes;
	nodes.push_back(root);
	int curSize = 1;
	int maxSize = 1;
	while (!nodes.empty())
	{
		int temp = 0;
		for (int i = 0; i < curSize; i++)
		{
			if (nodes.front()->left != nullptr)
			{
				nodes.push_back(nodes.front()->left);
				++temp;
			}
			if (nodes.front()->right != nullptr)
			{
				nodes.push_back(nodes.front()->right);
				++temp;
			}
			nodes.pop_front();
		}
		curSize = temp;
		if (curSize > maxSize)
			maxSize = curSize;
	}
	return maxSize;
}

int main()
{
	int N;
	std::cin >> N;
	Treap<int> tr;
	BinTree<int> btr;

	for (int i = 0; i < N; i++)
	{
		int tempKey, tempPrio;
		std::cin >> tempKey >> tempPrio;
		tr.Insert(tempKey, tempPrio);
		btr.Insert(tempKey);
	}

	std::cout << tr.GetWidest() - btr.GetWidest() << std::endl;
	system("pause");
}