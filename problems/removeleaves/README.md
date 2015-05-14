# Remove the Leaves of a Binary Tree

![](http://i.imgur.com/hXNlJw1.png)

A node is a _leaf_ that does not have a right or a left child (`node->left = NULL` and `node->right = NULL`). In this problem, we are going to explore removing nodes from a binary tree while paying attention to memory usage.

####Instructions
Write a function `removeLeaves` that will eliminate all leaves from the given tree. If the function is called on an empty tree, it should not change the tree because there are no leaves. You must free the memory of any removed node.