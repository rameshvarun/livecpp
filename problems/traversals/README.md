# Tree Traversals

A _traversal_ over a tree prints out each of its nodes (once). In 106B, you'll see three types of traversals: pre-order, in-order, and post-order.

#### Pre-Order Traversal

A pre-order traversal prints out the tree's root before any of its children, outputting 
`root` `left children` `right children`.

#### In-Order Traversal

An in-order traversal prints out the tree's root between its left and right children, outputting 
`left children` `root` `right children`.

#### Post-Order Traversal

A post-order traversal prints out the tree's root after all of its children, outputting 
`left children` `right children` `root`.

####Instructions
Write three member functions for the `BinaryTree` class: `preorder_traversal`, `inorder_traversal`, and `postorder_traversal`. Each function will take a pointer to the root of a tree as the parameter and prints the respective traversal of that tree.