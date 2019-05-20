/*Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N. 
Требуется построить бинарное дерево, заданное наивным порядком вставки. 
Т.е., при добавлении очередного числа K в дерево с корнем root, 
если root→Key ≤ K, то узел K добавляется в правое поддерево root; 
иначе в левое поддерево root. 
Выведите элементы в порядке pre-order (сверху вниз).

Рекурсия запрещена.

INPUT:
10
2
5
6
4
7
8
9
3
1
10

OUTPUT:
2 1 5 4 3 6 7 8 9 10

*/


#include <iostream>

template <typename T>
struct Node
{
	Node() = default;
	Node(T &v) : val(v) {left = nullptr; right = nullptr; }
	Node *left;
	Node *right;
	T val;
};

template <typename T>
class BinTree
{
public:
	BinTree() = default;
	~BinTree() { Clear(root); }

	void Insert(T&);
	void PreOrder(Node<T> *);
	void Visit(Node<T> *);
	void Clear(Node<T> *);
	Node<T> *Root() { return root; }

private:
	Node<T> *root;
};

template <typename T>
void BinTree<T>::Insert(T &val)
{
	Node<T> *newNode = new Node<T>(val);
	Node<T> *cur = root, *prevcur = nullptr;
	if (root == nullptr)
		root = newNode;
	else
	{
		while (cur != nullptr)
		{
			prevcur = cur;
			if (val < cur->val)
				cur = cur->left;
			else
				cur = cur->right;
		}
		if (val < prevcur->val)
			prevcur->left = newNode;
		else
			prevcur->right = newNode;
	}
}

template <typename T>
void BinTree<T>::Visit(Node<T> *node)
{
	std::cout << node->val << std::endl;
}

template <typename T>
void BinTree<T>::PreOrder(Node<T> *cur)
{
	if (cur != nullptr)
	{
		Visit(cur);
		PreOrder(cur->left);
		PreOrder(cur->right);
	}
}

template <typename T>
void BinTree<T>::Clear(Node<T> *cur)
{
	if (cur != nullptr)
	{
		Clear(cur->left);
		Clear(cur->right);
		delete cur;
	}
}
int main()
{
	int N;
	std::cin >> N;
	BinTree<int> tree;
	for (int i = 0; i < N; i++)
	{
		int temp;
		std::cin >> temp;
		tree.Insert(temp);
	}

	tree.PreOrder(tree.Root());
	system("pause");
}