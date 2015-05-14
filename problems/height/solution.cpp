#include <iostream>
#include <cmath>
using namespace std;

struct TreeNode {
	int data;
	TreeNode* left;
	TreeNode* right;
};

class BinaryTree {
	public:
		BinaryTree(TreeNode* root);

		int height();
		int height_helper(TreeNode* node);

	private:
		TreeNode* root; //NULL if empty
};

TreeNode* readTree() {

	int inputValue = -1;
	cin >> inputValue;

	if(inputValue == -1) return NULL;

	TreeNode* n = new TreeNode();
	n->data = inputValue;
	n->left = readTree();
	n->right = readTree();
	return n;
}

int main() {
	BinaryTree tree(readTree());
	cout << tree.height() << endl;
}

BinaryTree::BinaryTree(TreeNode* root){
	this->root = root;
}

/*
 * ^^ Implementation (not important) ^^
 * ************************************
 * vv Section problem (important!)   vv
 */

int BinaryTree::height(){
	return height_helper(root);
}

int BinaryTree::height_helper(TreeNode* node){
    if(node == NULL) return 0;
    return 1 + max(height_helper(node->left), height_helper(node->right));
}