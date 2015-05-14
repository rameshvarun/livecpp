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

		void preorder_traversal();
		void inorder_traversal();
		void postorder_traversal();

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
	tree.preorder_traversal();
	tree.inorder_traversal();
	tree.postorder_traversal();
}

BinaryTree::BinaryTree(TreeNode* root){
	this->root = root;
}

/*
 * ^^ Implementation (not important) ^^
 * ************************************
 * vv Section problem (important!)   vv
 */

void BinaryTree::preorder_traversal(){
	cout << "Pre-order traversal:  ";

	cout << endl;
}

void BinaryTree::inorder_traversal(){
	cout << "In-order traversal:   ";

	cout << endl;
}

void BinaryTree::postorder_traversal(){
	cout << "Post-order traversal: ";

	cout << endl;
}