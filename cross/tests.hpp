#ifndef TESTS_HPP
#define TESTS_HPP

#include <sstream>
#include <exception>

#include "IDictionary.h"

using namespace std;

template<typename K, typename V>
ostream& operator<< (ostream & out, AVL_tree<K, V>& tree)
{
	tree.traversal(tree.LRtR, [&out] (K& key, V& val) {
		out << val << " ";
	});
	return out;
}

template<typename K, typename V>
ostream& operator<< (ostream & out, const AVL_tree<K, V>& tree)
{
	tree.const_traversal(tree.LRtR, [&out] (K& key, V& val) {
		out << val << " ";
	});
	return out;
}



template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint)
{
  if (t != u) {
	ostringstream os;
	os << "Assertion failed: " << t << " != " << u << " hint: " << hint;
	throw runtime_error(os.str());
  }
}


class TestRunner
{
public:
	template <class TestFunc>
	void RunTest(TestFunc func, const string& test_name) {
		try {
			func();
			cout << test_name << " OK" << endl;
		} catch (runtime_error& e) {
			++fail_count;
			cout << test_name << " fail: " << e.what() << endl;
		}
	}

	~TestRunner() {
		if (fail_count > 0) {
			cout << fail_count << " unit tests failed. Terminate" << endl;
			exit(1);
		}
	}
private:
	int fail_count = 0;
};



int f (int key, int val) {return val;}
void tests_base_part_AVL_tree ()
{
	AVL_tree<int, int> tree;

	for (int i = 1; i < 6; i++)
		tree.insert(i, i);

	AssertEqual(tree.size(), 5, "size");
	AssertEqual(tree.get(2), 2, "get");

	tree.erase(2);
	AssertEqual(tree.size(), 4, "erase");

	AVL_tree<int, int> nov;
	nov.insert(1, 1);
	for (int i = 3; i < 6; i++)
		nov.insert(i, i);

	AssertEqual(tree, nov, "erase, operator== and operator!=");
}




int func (int val) {return 5 * val;}
bool func_where (int val) {
	if (val % 10 == 0)
		return true;
	else
		return false;
}
void tests_map_where_reduce ()
{
	AVL_tree<int, int> tree;
	for (int i = 1; i < 6; i++)
		tree.insert(i, i);

	AVL_tree<int, int> r = map(tree, func);
	AssertEqual(r.get(4), 20, "map");

	AVL_tree<int, int> nov;
	for (int i = 1; i < 6; i++)
		nov.insert(i, 5*i);

	AssertEqual (r, nov, "map, operator== and operator!=");

	int start_val = 0;
	int res = reduce(r, start_val, std::plus<>());
	AssertEqual(res, 75, "reduce");

	AVL_tree<int, int> tr = where(r, func_where);

	AVL_tree<int, int> wher;
	wher.insert(2, 10);
	wher.insert(4, 20);

	AssertEqual(tr, wher, "where, operator== and operator!=");
}

void tests_IDictionary()
{
	IDictionary<int, int> dict;
	for (int i = 1; i < 6; i++)
		dict.insert(i, i);
	bool tru = dict.get(3) == 3;

	AssertEqual(dict.containsKey(3), tru, "containsKey");
}


#endif // TESTS_HPP
