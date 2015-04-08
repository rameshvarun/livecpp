#include <iostream>
using namespace std;

struct node {
	int value;
	node* left;
	node* right;
};
bool isbalanced(node* root);

node* readTree() {
	node* n = new node();
	n->value = 0;
	n->left = NULL;
	n->right = NULL;
}

int main() {
	node* tree = readTree();
	cout << boolalpha << isbalanced(tree) << noboolalpha;
}

// Return whether or not the tree is balanced.
bool isbalanced(node* root) {
	return true;
}