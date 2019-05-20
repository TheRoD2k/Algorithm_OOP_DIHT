/*Дано число N и N строк. 
Каждая строка содержащит команду добавления или удаления натуральных чисел, 
а также запрос на получение k-ой порядковой статистики. 
Команда добавления числа A задается положительным числом A, 
команда удаления числа A задается отрицательным числом “-A”. 
Запрос на получение k-ой порядковой статистики задается числом k. 
Требуемая скорость выполнения запроса - O(log n).


INPUT:
3
1 0
2 0
-1 0

OUTPUT:
1 1 2

*/


#include <iostream>

template <typename T>
struct Node
{
	Node(T const &k) : key(k) { }
	T key = NULL;
	int height = 1;
	int children = 1;
	Node<T> *left = nullptr;
	Node<T> *right = nullptr;
	void FixHeight();
	int BalanceFactor();
};

template <typename T>
int Node<T>::BalanceFactor()
{
	if (this->left == nullptr && this->right == nullptr)
		return 0;
	if (this->left == nullptr)
		return this->right->height;
	if (this->right == nullptr)
		return -this->left->height;
	return this->right->height - this->left->height;
}

template <typename T>
void Node<T>::FixHeight()
{
	int leftChildren = (this->left ? this->left->children : 0);
	int rightChildren = (this->right ? this->right->children : 0);
	this->children = leftChildren + rightChildren + 1;

	int leftHeight = (this->left ? this->left->height : 0);
	int rightHeight = (this->right ? this->right->height : 0);
	this->height = (leftHeight>rightHeight ? leftHeight : rightHeight) + 1;
}

template <typename T>
class AVLTree
{
public:
	Node<T> *root = nullptr;
	Node<T> *RotateRight(Node<T> *);
	Node<T> *RotateLeft(Node<T> *);
	Node<T> *Balance(Node<T> *);
	Node<T> *Insert(Node<T> *, T const &);
	Node<T> *FindMin(Node<T> *);
	Node<T> *RemoveMin(Node<T> *);
	Node<T> *Remove(Node<T> *, T const &);
};

template <typename T>
Node<T> *AVLTree<T>::RotateRight(Node<T> *nodeA)
{
	Node<T> *nodeB = nodeA->left;
	nodeA->left = nodeB->right;
	nodeB->right = nodeA;
	nodeA->FixHeight();
	nodeB->FixHeight();
	return nodeB;
}

template <typename T>
Node<T> *AVLTree<T>::RotateLeft(Node<T> *nodeA)
{
	Node<T> *nodeB = nodeA->right;
	nodeA->right = nodeB->left;
	nodeB->left = nodeA;
	nodeA->FixHeight();
	nodeB->FixHeight();
	return nodeB;
}

template <typename T>
Node<T> *AVLTree<T>::Balance(Node<T> *cur)
{
	cur->FixHeight();
	if (cur->BalanceFactor() == 2)
	{
		if (cur->right->BalanceFactor() < 0)
			cur->right = RotateRight(cur->right);
		return RotateLeft(cur);
	}
	if (cur->BalanceFactor() == -2)
	{
		if (cur->left->BalanceFactor() > 0)
			cur->left = RotateLeft(cur->left);
		return RotateRight(cur);
	}
	return cur;
}

template <typename T>
Node<T> *AVLTree<T>::Insert(Node<T> *cur, T const &key)
{
	if (root == nullptr)
	{
		root = new Node<T>(key);
		return root;
	}
	if (cur == nullptr)
		return new Node<T>(key);
	if (key<cur->key)
		cur->left = Insert(cur->left, key);
	else
		cur->right = Insert(cur->right, key);
	if (cur == root)
	{
		root = Balance(cur);
		return root;
	}
	return Balance(cur);
}

template <typename T>
Node<T> *AVLTree<T>::FindMin(Node<T> *cur)
{
	return cur->left ? FindMin(cur->left) : cur;
}

template <typename T>
Node<T> *AVLTree<T>::RemoveMin(Node<T> *cur)
{
	if (cur->left == 0)
		return cur->right;
	cur->left = RemoveMin(cur->left);
	return Balance(cur);
}

template <typename T>
Node<T> *AVLTree<T>::Remove(Node<T> *cur, T const &key)
{
	if (cur == nullptr)
		return 0;
	if (key < cur->key)
		cur->left = Remove(cur->left, key);
	else if (key > cur->key)
		cur->right = Remove(cur->right, key);
	else
	{

		Node<T> *lTr = cur->left;
		Node<T> *rTr = cur->right;
		if (cur != root)
		{
			delete cur;
			if (rTr == nullptr)
				return lTr;
			Node<T> *min = FindMin(rTr);
			min->right = RemoveMin(rTr);
			min->left = lTr;
			return Balance(min);
		}
		else
		{
			if (rTr == nullptr)
			{
				root = lTr;
				return root;
			}
			Node<T> *min = FindMin(rTr);
			min->right = RemoveMin(rTr);
			min->left = lTr;
			root = Balance(min);
			return root;
		}
	}
	if (cur == root)
	{
		root = Balance(cur);
		return root;
	}
	return Balance(cur);
}

template <typename T>
T KStat(AVLTree<T> &Tr, int num)
{
	if (num > Tr.root->children)
		num = Tr.root->children;
	if (num < 1)
		num = 1;
	if (Tr.root == nullptr)
		return 0;
	Node<T> *cur = Tr.root;
	while (num != 0)
	{
		//std::cout << num << std::endl;
		if (cur->left != nullptr && cur->left->children >= num)
			cur = cur->left;
		else
			if (cur->right != nullptr && (cur->left ? cur->left->children : 0) + 1 < num)
			{
				num -= ((cur->left ? cur->left->children : 0) + 1);
				cur = cur->right;
			}
			else
				if (num == (cur->left ? cur->left->children : 0) + 1)
				{
					num = 0;
					return cur->key;
				}
	}
}

int main()
{
	AVLTree<int> Tr;
	int N;
	std::cin >> N;
	for (int i = 0; i < N; i++)
	{
		int temp, k;
		std::cin >> temp >> k;
		if (temp >= 0)
			Tr.Insert(Tr.root, temp);
		else
			Tr.Remove(Tr.root, -temp);
		std::cout << KStat(Tr, ++k) << std::endl;
	}
	system("pause");
}