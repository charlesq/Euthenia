#include <node.h>
/* recursive implementation of BST traversals */
void traverse_in_order(const struct _node <int> *n);
void traverse_pre_order(const struct _node <int> *n);
void traverse_post_order(const struct _node <int> *n);
/* iterative implementation of traversals */
void traverse_in_order_it(const struct _node<int> *n);
void traverse_pre_order_it(const struct _node <int> *n);
void traverse_post_order_it(const struct _node <int> *n);
void traverse_by_level(const struct _node <int> *n);
void traverse_contour(const struct _node <int> *root, const bool no_bottom = false);
int main()
{
    struct _node <int> *r = build_tree(10, 0);
    std::cout << "recursive in order traversal \n";
    traverse_in_order(r);
    std::cout << "\niterative in order traversal \n";
    traverse_in_order_it(r);
    std::cout << "\nrecursive pre order traversal \n";
    traverse_pre_order(r);
    std::cout << "\niterative pre order traversal\n";
    traverse_pre_order_it(r);
    std::cout << "\nrecursive post order traversal \n";
    traverse_post_order(r);
    std::cout << "\niterative post order travsersal\n";
    traverse_post_order_it(r);
    std::cout <<"\ntraverse by level\n";
    traverse_by_level(r);
    std::cout << std::endl;
    std::cout <<"print contour without bottom\n";
    traverse_contour(r, true);
    std::cout << std::endl;
    std::cout << "print contour with bottom\n";
    traverse_contour(r, false);
    std::cout << std::endl;
    release_tree(r);
    return 0;
}










