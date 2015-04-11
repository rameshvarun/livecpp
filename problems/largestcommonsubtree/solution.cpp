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

#define PRIME 53 // Prime number used for hashing
#define NUM_BUCKETS 1000 // Number of buckets to hash into

pair<int, int> generate_hashes(list<pair<int, node*>> hashes[], node* start) {
    if(start == NULL) return pair<int,int>(0,0);
    
    pair<int, int> l = generate_hashes(hashes, start->left);
    pair<int, int> r = generate_hashes(hashes, start->right);
    
    unsigned int size = l.first + r.first + 1;
    unsigned int hash = start->value + PRIME*l.second + PRIME*PRIME*r.second;

    hashes[hash % NUM_BUCKETS].push_back(pair<int, node*>(size, start));
    return pair<int,int>(size, hash);
}

pair<int,int> check_hashes(list<pair<int, node*>> hashes[], node* start) {
	if(start == NULL) return pair<int,int>(0,0);

	pair<int, int> l = check_hashes(hashes, start->left);
    pair<int, int> r = check_hashes(hashes, start->right);

    unsigned int hash = start->value + PRIME*l.second + PRIME*PRIME*r.second;
    list<pair<int, node*>> bucket = hashes[hash % NUM_BUCKETS];
    if(bucket.size() == 0) { // No matching subtrees
    	return pair<int,int>(max(l.first, r.first), hash);
    } else if(bucket.size() == 1) { // One subtree with same hash
    	return pair<int, int>(bucket.front().first, hash);
    } else { // Rare but possible: two subtrees have same hash
    	for(auto& entry : bucket) {
    		if(tree_equal(start, entry.second))
    			return pair<int, int>(entry.first, hash);
    	}

    	cout << "Something has gone wrong." << endl;
    	abort(); 
    }
}

// Return the size of largest common subtree
int largest_common_subtree(node* a, node* b) {
	list<pair<int, node*>> a_hashes[NUM_BUCKETS];
	generate_hashes(a_hashes, a);
	return check_hashes(a_hashes, b).first;
}

int main() {
	node* a = readTree();
	node* b = readTree();
	cout << largest_common_subtree(a, b);
}