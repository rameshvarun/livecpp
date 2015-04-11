#include <iostream>
#include <cmath>
using namespace std;

struct node {
	int value;
	node* left;
	node* right;
};

bool isbalanced(node* root);

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

int main() {
	node* tree = readTree();
	cout << boolalpha << isbalanced(tree) << noboolalpha;
}

// Return whether or not the tree is balanced.
bool isbalanced(node* root) {
	return true;
}