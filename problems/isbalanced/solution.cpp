#include <iostream>
#include <cmath>
using namespace std;

struct node {
	int value;
	node* left;
	node* right;
};

bool isbalanced(node* root);
int height(node* root);

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
	return abs(height(root->left) - height(root->right)) <= 1;
}

int height(node* root){
	if(root == NULL) return 0;

	int left_height = height(root->left);
	int right_height = height(root->right);

	if(left_height > right_height) return left_height + 1;
	return right_height + 1;
}