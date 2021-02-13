#ifndef IDICTIONARY_H
#define IDICTIONARY_H

#include "binary_search_tree.h"

template <typename K, typename V>
class IDictionary : public AVL_tree<K, V>
{
public:
	IDictionary(){}
	IDictionary(const IDictionary& dict);

	bool containsKey(K key) {return (this->get(key) != NULL);}

	bool operator==(const IDictionary& dict) const;
	bool operator!=(const IDictionary& dict) const {return !(dict == *this);}
};


template<typename K, typename V>
IDictionary<K, V>::IDictionary(const IDictionary &dict)
{
	if (this->root != nullptr)
		delete this->root;
	this->root = new typename AVL_tree<K,V>::node(dict.root);
}


template<typename K, typename V>
bool IDictionary<K, V>::operator==(const IDictionary &dict) const
{
	int t = 0;
	if (this->size() == dict.size())
	{
		dict.const_traversal(dict.LRtR, [&dict, this, &t] (K& key, V& val) {
			if (this->get(key) != val)
				t++;
		});
		return (t == 0);
	}
	else return false;
}

#endif // IDICTIONARY_H


