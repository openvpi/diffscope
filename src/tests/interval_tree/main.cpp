#include "interval_tree.h"

using namespace lib_interval_tree;

int main(int argc, char **argv) {
    interval_tree_t<int, right_open> tree;
    tree.insert(make_safe_interval<int, right_open>(21, 16)); // make_safe_interval swaps low and high if not in right order.
    tree.insert({8, 9});
    tree.insert({25, 30});
    tree.insert({5, 8});
    tree.insert({15, 23});
    tree.insert({17, 19});
    tree.insert({26, 26});
    tree.insert({0, 3});
    tree.insert({6, 10});
    tree.insert({19, 20});

    tree.overlap_find_all({7, 12}, [](const decltype(tree)::iterator& it) {
        std::cout << it.interval().low() << ", " << it.interval().high() << std::endl;
        return true;
    });

    return 0;
}
