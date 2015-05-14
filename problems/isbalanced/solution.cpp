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

		bool isBalanced();
		bool isBalanced(TreeNode* node);
		int height();
		int height(TreeNode* node);

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

BinaryTree::BinaryTree(TreeNode* root){
	this->root = root;
}

int BinaryTree::height(){
	return height(root);
}

int BinaryTree::height(TreeNode* node){
    if(node == NULL) return 0;
    return 1 + max(height(node->left), height(node->right));
}

int main() {
	BinaryTree tree(readTree());
	cout << boolalpha << tree.isBalanced() << noboolalpha;
}

/*
 * ^^ Implementation (not important) ^^
 * ************************************
 * vv Section problem (important!)   vv
 */

// Return whether or not the tree is balanced.
bool BinaryTree::isBalanced(){
	return isBalanced(root);
}

bool BinaryTree::isBalanced(TreeNode* node) {
	if(node == NULL){
		return true;
	}else if(!isBalanced(node->left) || !isBalanced(node->right)){
		return false;
	}else{
		return abs(height(node->left) - height(node->right)) <= 1;
	}
}