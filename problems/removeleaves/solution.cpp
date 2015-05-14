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
		void printTree();
		void printTree(TreeNode* node);

		void removeLeaves();
		void removeLeaves(TreeNode*& node);
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

void BinaryTree::printTree(){
	printTree(root);
}

void BinaryTree::printTree(TreeNode *node){
	if(node == NULL){
		cout << "-1 ";
	}else{
		cout << node->data << " ";
    	printTree(node->left);
    	printTree(node->right);
	}
}

int main() {
	BinaryTree tree(readTree());
	tree.removeLeaves();
	tree.printTree();
}

/*
 * ^^ Implementation (not important) ^^
 * ************************************
 * vv Section problem (important!)   vv
 */

// Return whether or not the tree is balanced.
void BinaryTree::removeLeaves(){
	removeLeaves(root);
}

void BinaryTree::removeLeaves(TreeNode*& node){
	if(node != NULL){
		if(node->left == NULL && node->right == NULL){
			delete node;
			node = NULL;
		} else {
			removeLeaves(node->left);
			removeLeaves(node->right);
		}
	}
}