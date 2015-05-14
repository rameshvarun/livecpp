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
		void preorder_helper(TreeNode* node);
		void inorder_helper(TreeNode* node);
		void postorder_helper(TreeNode* node);
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
	cout << "Pre-order traversal: ";
    preorder_helper(root);
	cout << endl;
}

void BinaryTree::preorder_helper(TreeNode* node){
    if(node == NULL) return;
    cout << node->data << " ";
    preorder_helper(node->left);
    preorder_helper(node->right);
}

void BinaryTree::inorder_traversal(){
	cout << "In-order traversal: ";
    inorder_helper(root);
	cout << endl;
}

void BinaryTree::inorder_helper(TreeNode* node){
    if(node == NULL) return;
    inorder_helper(node->left);
    cout << node->data << " ";
    inorder_helper(node->right);
}

void BinaryTree::postorder_traversal(){
	cout << "Post-order traversal: ";
    postorder_helper(root);
	cout << endl;
}

void BinaryTree::postorder_helper(TreeNode* node){
    if(node == NULL) return;
    postorder_helper(node->left);
    postorder_helper(node->right);
    cout << node->data << " ";
}