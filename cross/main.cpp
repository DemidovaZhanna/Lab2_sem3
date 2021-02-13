#include <iostream>
#include "tests.hpp"
#include "interface.h"

using namespace std;

int main()
{
	TestRunner runner;
	runner.RunTest(tests_base_part_AVL_tree, "tests_base_part_AVL_tree");
	runner.RunTest(tests_map_where_reduce, "tests_map_where_reduce");
	runner.RunTest(tests_IDictionary, "tests_IDictionary");

	cout << endl;
	interface();

	return 0;
}
