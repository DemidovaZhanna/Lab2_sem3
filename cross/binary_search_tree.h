#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>
#include <chrono>


template <typename K, typename V>
class AVL_tree
{
public:
	using traversal_type = void (*)(void*&, void*&, void*&);

public:
	AVL_tree () : root(nullptr), _size(0) {}
	AVL_tree(const AVL_tree<K,V> &Tree);

	void insert (K key,	V val);
	void erase (K key) {root = remove(root, key); _size--;}

	V& get (K key);
	const V& get (K key) const {return find_el(root, key)->val;}

	int size() const {return _size;}

	AVL_tree& operator= (const AVL_tree& tree);

	bool operator== (const AVL_tree& a) const;
	bool operator!= (const AVL_tree& a) const {return !(a == *this);}

	template<typename Func>
	void traversal (traversal_type t, Func f) {_traversal(root, nullptr, t, f);}

	template<typename Func>
	void const_traversal (traversal_type t, Func f) const {_const_traversal(root, nullptr, t, f);}

	~AVL_tree();

private:
	struct node {

		node (K key, V val) : key(key), val(val), height(1), left(nullptr), right(nullptr) {}
		K key;
		V val;
		int height;
		node* left;
		node* right;

		node(node *Node);
		~node();
	};

	int height (node* p) const;
	int balance_factor (node* p) const;
	void fixheight (node* p);
	node* rotate_right (node* p);
	node* rotate_left (node* q);
	node* balance (node* p);
	node* find_el (node* p, K k) const;
	node* _insert (node* p, K k, V val);
	node* find_min (node* p) const;
	node* remove_min_el (node* p);
	node* remove (node* p, K k);

	template<typename Func>
	void _traversal(node* p, node* parent, traversal_type t, Func f);

	template<typename Func>
	void _const_traversal(node* p, node* parent, traversal_type t, Func f) const;

private:
	node* root;
	int _size;

public:
	static void RtRL (void *&n1, void *&n2, void *&n3) {std::swap(n1, n3); std::swap(n2, n3);}
	static void RtLR (void *&n1, void *&n2, void *&n3) {std::swap(n1, n2);}
	static void RRtL (void *&n1, void *&n2, void *&n3) {std::swap(n1, n3);}
	static void LRtR (void *&n1, void *&n2, void *&n3) {}
	static void RLRt (void *&n1, void *&n2, void *&n3) {std::swap(n1, n3); std::swap(n1, n2);}
	static void LRRt (void *&n1, void *&n2, void *&n3) {std::swap(n2, n3);}
};


template<typename K, typename V>
AVL_tree<K, V>::AVL_tree(const AVL_tree<K, V> &Tree) :
	root(Tree.root ? new node(Tree.root) : nullptr),
	_size(Tree._size)
{}


template<typename K, typename V>
void AVL_tree<K, V>::insert(K key, V val)
{
	root = _insert(root, key, val);
	++_size;
}


template<typename K, typename V>
V &AVL_tree<K, V>::get(K key)
{
	node* k = find_el(root, key);
	return k->val;
}

template<typename K, typename V>
AVL_tree<K, V> &AVL_tree<K, V>::operator= (const AVL_tree &tree)
{
	if (root != nullptr)
		delete root;
	root = new node(tree.root);

	return *this;
}

template<typename K, typename V>
AVL_tree<K, V>::~AVL_tree()
{
	delete root;
}

template<typename K, typename V>
bool AVL_tree<K, V>::operator==(const AVL_tree<K, V> &a) const
{
	int t = 0;
	if (_size == a._size){
		a.const_traversal(a.LRtR, [&a, this, &t] (K& key, V& val) {
			if (this->get(key) != val)
				t++;
		});
		return (t == 0);
	}
	else
		return false;
}



template<typename K, typename V>
AVL_tree<K, V>::node::node(AVL_tree::node *Node) : key(Node->key), val(Node->val), height(Node->height)
{
	if (Node->left != nullptr)
		left = new node(Node->left);
	else
		left = nullptr;
	if (Node->right != nullptr)
		right = new node(Node->right);
	else
		right = nullptr;
}


template<typename K, typename V>
AVL_tree<K, V>::node::~node()
{
	delete left;
	delete right;
}


template<typename K, typename V>
int AVL_tree<K, V>::height(AVL_tree::node *p) const //может работать и с пустыми деревьями
	{
		return p ? p->height : 0;
	}

template<typename K, typename V>
int AVL_tree<K, V>::balance_factor(AVL_tree::node *p) const // работает только с ненулевыми указателями
{
	return height(p->right) - height(p->left);
}

template<typename K, typename V>
void AVL_tree<K, V>::fixheight(AVL_tree::node *p)
{ // восстанавливает корректное значение height заданного узла (при корректности значений дочерних узлов)
	int h_left = height(p->left);
	int h_right = height(p->right);

	p->height = (h_left > h_right ? h_left : h_right) + 1;
}

template<typename K, typename V>
typename AVL_tree<K, V>::node *AVL_tree<K, V>::rotate_right(AVL_tree::node *p)
{ //правый поворот вокруг р для поддержания балансировки (если разница между высотами равна +-2)
	node* q = p->left; //изначально q расположена левее p
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);

	return q;
}

template<typename K, typename V>
typename AVL_tree<K, V>::node *AVL_tree<K, V>::rotate_left(AVL_tree::node *q)
{
	node* p = q->right;
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);

	return p;
}

template<typename K, typename V>
typename AVL_tree<K, V>::node *AVL_tree<K, V>::balance(AVL_tree::node *p)  // балансировка узла р
{
	fixheight(p);

	if (balance_factor(p) == 2) {
		if (balance_factor(p->right) < 0)
			p->right = rotate_right(p->right);
		return rotate_left(p);
	}

	if (balance_factor(p) == -2) {
		if(balance_factor(p->left) > 0)
			p->left = rotate_left(p->left);
		return rotate_right(p);
	}

	return p;
}

template<typename K, typename V>
typename AVL_tree<K, V>::node *AVL_tree<K, V>::find_el(AVL_tree<K, V>::node *p, K k) const
{
	if (!p)
		return nullptr;
		//throw std::out_of_range("AVL_Tree out of range!"); //было return nullptr;
	if (k < p->key)
		return find_el(p->left, k);
	if (k > p->key)
		return find_el(p->right, k);
	else
		return p;
}

template<typename K, typename V>
typename AVL_tree<K, V>::node *AVL_tree<K, V>::_insert(AVL_tree<K, V>::node *p, K k, V val)
{
	if (!p)
		return new node(k, val);
	if (k < p->key)
		p->left = _insert(p->left, k, val);
	else
		p->right = _insert(p->right, k, val);
	return balance(p);
}

template<typename K, typename V>
typename AVL_tree<K, V>::node *AVL_tree<K, V>::find_min(AVL_tree<K, V>::node *p) const
{
	return p->left ? find_min(p->left) : p;
}

template<typename K, typename V>
typename AVL_tree<K, V>::node *AVL_tree<K, V>::remove_min_el(AVL_tree<K, V>::node *p)
{ //удаление узла с минимальным ключом
	if (p->left == nullptr)
		return p->right;
	p->left = remove_min_el(p->left);
	return balance(p);
}

template<typename K, typename V>
typename AVL_tree<K, V>::node *AVL_tree<K, V>::remove(AVL_tree<K, V>::node *p, K k)
{
	if (p == nullptr)
		throw std::out_of_range("AVL_Tree out of range!");
	if (k < p->key)
		p->left = remove(p->left,k);
	else if (k > p->key)
		p->right = remove(p->right,k);
	else
		if (k == p->key) {
		node* q = p->left;
		node* r = p->right;

		p->left = nullptr;
		p->right = nullptr;
		delete p;

		if(r == nullptr) return q;

		node* min = find_min(r);
		min->right = remove_min_el(r);
		min->left = q;
		return balance(min);
	}

	return balance(p);
}


template<typename K, typename V>
template <typename Func>
void AVL_tree<K, V>::_traversal(node *p, node *parent, traversal_type t, Func f)
{
	if (p == nullptr)
		return;

	if (p == parent)
		f(p->key, p->val);
	else {
		void *n1 = p->left,
			 *n2 = p,
			 *n3 = p->right;
		t(n1, n2, n3);

		_traversal((node*)n1, p, t, f);
		_traversal((node*)n2, p, t, f);
		_traversal((node*)n3, p, t, f);
	}
}


template<typename K, typename V>
template<typename Func>
void AVL_tree<K, V>::_const_traversal(node *p, node *parent, traversal_type t, Func f) const
{
	if (p == nullptr)
		return;

	if (p == parent)
		f(p->key, p->val);
	else {
		void *n1 = p->left,
			 *n2 = p,
			 *n3 = p->right;
		t(n1, n2, n3);

		_const_traversal((node*)n1, p, t, f);
		_const_traversal((node*)n2, p, t, f);
		_const_traversal((node*)n3, p, t, f);
	}
}



template<typename K, typename V, typename Func>
AVL_tree<K, V> map(const AVL_tree<K, V>& tree, Func f)
{
	AVL_tree<K, V> nov = tree;
	nov.traversal(nov.LRtR, [f](K& key, V& val) {
		val = f(val);
	});

	return nov;
}


template<typename K, typename V, typename Func>
AVL_tree<K, V> where(const AVL_tree<K, V>& tree, Func f)
{
	AVL_tree<K, V> nov;

	tree.const_traversal(tree.LRtR, [&f, &nov](const K& key, const V& val) {
		if (f(val))
			nov.insert(key, val);
	});

	return nov;
}


template<typename K, typename V, typename Func>
V reduce(const AVL_tree<K, V>& tree, const V& start_val, Func f
		 /*, typename AVL_Tree<K,V>::traversal_type t_type = AVL_Tree<K,V>::LRtR*/)
{
	V res = start_val;

	tree.const_traversal(tree.LRtR, [&f, &res] (const K& key, const V& val) {
		res = f(val, res);
	});

	return res;
}

#endif // BINARY_SEARCH_TREE_H
