#include <iostream>
#include <cmath>
#include <unordered_map>
#include <list>
using namespace std;

struct node {
	int value;
	node* left;
	node* right;
};

node* readTree() {
	int inputValue = -1;
	cin >> inputValue;
	if(inputValue == -1) return NULL;

	node* n = new node();
	n->value = inputValue;
	n->left = readTree();
	n->right = readTree();
	return n;
}

// Helper tree equals function
bool tree_equal(node* a, node* b) {
	if(a == NULL && b == NULL) return true;
	if(a == NULL && b != NULL) return false;
	if(a != NULL && b == NULL) return false;
	if(a->value != b->value) return false;
	return tree_equal(a->left, b->left) && tree_equal(a->right, b->right);
}

// Return the size of largest common subtree
int largest_common_subtree(node* a, node* b) {
	return 0;
}

int main() {
	node* a = readTree();
	node* b = readTree();
	cout << largest_common_subtree(a, b);
}