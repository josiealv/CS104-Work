#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "../bst/bst.h"

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus 
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
	// Constructor/destructor.
	AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
	virtual ~AVLNode();

	// Getter/setter for the node's height.
	int getHeight() const;
	void setHeight(int height);

	// Getters for parent, left, and right. These need to be redefined since they 
	// return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
	// for more information.
	virtual AVLNode<Key, Value>* getParent() const override;
	virtual AVLNode<Key, Value>* getLeft() const override;
	virtual AVLNode<Key, Value>* getRight() const override;

protected:
	int mHeight;
};

/*
--------------------------------------------
Begin implementations for the AVLNode class.
--------------------------------------------
*/

/**
* Constructor for an AVLNode. Nodes are initialized with a height of 0.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
	: Node<Key, Value>(key, value, parent)
	, mHeight(0)
{

}

/**
* Destructor.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* Getter function for the height. 
*/
template<typename Key, typename Value>
int AVLNode<Key, Value>::getHeight() const
{
	return mHeight;
}

/**
* Setter function for the height. 
*/
template<typename Key, typename Value>
void AVLNode<Key, Value>::setHeight(int height)
{
	mHeight = height;
}

/**
* Getter function for the parent. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mParent);
}

/**
* Getter function for the left child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mLeft);
}

/**
* Getter function for the right child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mRight);
}

/*
------------------------------------------
End implementations for the AVLNode class.
------------------------------------------
*/

/**
* A templated balanced binary search tree implemented as an AVL tree.
*/
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods. 
	virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
	void remove(const Key& key);

private:
	/* Helper functions are strongly encouraged to help separate the problem
	   into smaller pieces. You should not need additional data members. */
	   void calc_height (AVLNode<Key,Value>* curr);
	   AVLNode<Key, Value>* r_rotate (AVLNode<Key,Value>* node); //right rotate
	   AVLNode<Key, Value>* l_rotate (AVLNode<Key,Value>* node); //left rotate
	   int Balancefactor (AVLNode<Key,Value>* node); //gets the height difference of a node's children
	   AVLNode<Key, Value>* Node_unbal( AVLNode<Key, Value>* node); //find unbalanced node
	   void rebalance (AVLNode<Key, Value>* node);//helper function that balances tree
	   AVLNode<Key, Value>* getLeaf(AVLNode<Key, Value>* leaf);
	   AVLNode<Key, Value>* findSuccessor (AVLNode<Key, Value>* node); 
	   AVLNode<Key, Value>* remove1(AVLNode<Key, Value>* node); //for removing node with 1 child or no child
	   AVLNode<Key, Value>* remove2(AVLNode<Key, Value>* node); //for removing node with 2 children
};

/*
--------------------------------------------
Begin implementations for the AVLTree class.
--------------------------------------------
*/

/**
* Insert function for a key value pair. Finds location to insert the node and then balances the tree. 
*/

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::r_rotate(AVLNode<Key,Value>* node)
{
	AVLNode<Key, Value>* T0 = node->getLeft(); //get the smaller child of the current node
	AVLNode<Key, Value>* T1 = T0 ->getRight(); //get the larger child of the smaller child of the current node
	
	//if node is the root, since we are going to rotate it to the right, make the root node's left child
	if (this->mRoot == node) 
	{
	  this->mRoot = T0;
	}
	else
	{
		if (node->getParent()->getLeft()==node) //if node is in a left subtree, swap parent's left child & node's left child
		{
			node->getParent()->setLeft(T0);
		}
		else if (node->getParent()->getRight()==node) //if node is in right subtree, swap parent's right child 
		{
			node->getParent()->setRight(T0);
		}
	}
	T0->setParent(node->getParent()); //reset the parents
	node->setParent(T0);
	T0->setRight(node); //make the smaller child the new parent of the current node
	node->setLeft(T1); //make the larger child of the previous smaller child the new left child of the node we rotate
	if(T1!=NULL) //reset the parent of the original grandchild of the node as the node
	{
		T1->setParent(node);
	}

	//re-calculate heights
	AVLNode<Key, Value>* new_root = static_cast <AVLNode<Key, Value>*> (this->mRoot);
	(calc_height(new_root));

	return T0;
}
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::l_rotate(AVLNode<Key,Value>* node)
{
	AVLNode<Key, Value>* T0 = node->getRight();	//get the larger child of the current node
	AVLNode<Key, Value>* T1 = T0 ->getLeft();	//get the smaller child of the larger child

	//if node is the root, since we are going to rotate it to the left, make the root node's right child
	if (this->mRoot == node)
	{
		this->mRoot = T0; 
	}
	else
	{
		if (node->getParent()->getLeft()==node) //if node is in a left subtree, swap parent's left child & node's left child
		{
			node->getParent()->setLeft(T0);
		}
		else if (node->getParent()->getRight()==node) //if node is in right subtree, swap parent's right child 
		{
			node->getParent()->setRight(T0);
		}
	}
	T0->setParent(node->getParent()); //reset parents
	node->setParent(T0);
	T0->setLeft(node); //make the larger child the new parent of the current node
	node->setRight(T1); //make the smaller child of the previous larger child the new right child of the node we rotate

	if (T1!=NULL) //reset the parent of the original grandchild of the node as the node
	{
		T1->setParent(node); 
	}
	//re-calculate heights
	AVLNode<Key, Value>* new_root = static_cast <AVLNode<Key, Value>*> (this->mRoot);
	(calc_height(new_root));

	return T0;
}
template<typename Key, typename Value>
int AVLTree<Key, Value>::Balancefactor(AVLNode<Key,Value>* node)
{
	if (node->getLeft()==NULL && node->getRight()==NULL) //if node is leaf, it is automatically balanced
	{
		return 0;
	}
	else if (node->getLeft()==NULL) //if left child DNE, then make height of right child negative, NULL's have height -1
	{
		return -1-node->getRight()->getHeight(); 
	}
	else if (node->getRight()==NULL) //same as above but for left but make positive
	{
		return node->getLeft()->getHeight()+1;
	}
	else
	{
		return (node->getLeft()->getHeight() - node->getRight()->getHeight()); //bf for when both children exists
	}
}
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::Node_unbal(AVLNode<Key, Value>* node)
{
	while (node!=NULL)
	{
		int bal = Balancefactor(node);
		if (bal>1 || bal <-1) //if height difference is greater than 1, then unbalance node has been found
		{
			return node;
		}
		node=node->getParent(); //keep going up 
	}
	return node;
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::calc_height(AVLNode<Key,Value>* curr)
{
	if (curr->getRight()== NULL && curr->getLeft()==NULL) //if leaf or root, height = 0
	{
		curr->setHeight(0);
		return;
	}
	else if (curr->getLeft()==NULL) //current node has only right child, go down that subtree & calc height
	{
		calc_height(curr->getRight());
		curr->setHeight(curr->getRight()->getHeight()+1);
	}
	else if (curr->getRight()==NULL) //same as above but for the left child
	{
		calc_height(curr->getLeft());
		curr->setHeight(curr->getLeft()->getHeight()+1);
	}
	//if both left & right child exists, go down both their subtrees and get the max height of the 2 & set height = max+1
	else 
	{
		calc_height(curr->getRight());
		calc_height(curr->getLeft());
		curr->setHeight((std::max(curr->getLeft()->getHeight(), curr->getRight()->getHeight()))+1);
	}
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::rebalance (AVLNode<Key, Value>* node)
{
	int h_diff = Balancefactor(node); //get the balance factor of unbalanced node

	//check if balance factors of left/right child 
	//of unbalanced node have a balance factor > 0 || <0
	//and then decide which rotations to do
	if (h_diff>1)
	{
			//Left Left: The insertion was in the left subtree of the left child of the unbalanced node.
			if (Balancefactor(node->getLeft())>0)
			{
				node = r_rotate(node);
			}
			//Left Right: The insertion was in the right subtree of the left child of the unbalanced node
			else 
			{
				node->setLeft(l_rotate(node->getLeft()));
				node =  r_rotate(node);
			}
	}	
	else if (h_diff <-1)
	{
		//Right right: The insertion was in the right subtree of the right child of the unbalanced node
		if (Balancefactor(node->getRight())<0)
		{
			node = l_rotate(node);
		}
		//Right Left: The insertion was in the left subtree of the right child of the unbalanced node
		else 
		{
			node->setRight(r_rotate(node->getRight()));
			node = l_rotate(node);
		}
	}	

}
template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
	if(this->mRoot==NULL) //if root hasn't been created, create it with current pair
	{
		AVLNode<Key, Value>* node = new AVLNode<Key, Value> (keyValuePair.first, keyValuePair.second, NULL);
		this->mRoot= node; 
	}
	else if (keyValuePair.first==this->mRoot->getKey()) //if key is equal key of root, update value
	{
		AVLNode<Key, Value>* root = static_cast <AVLNode<Key, Value>*> (this->mRoot);
		root->setValue(keyValuePair.second);
	}
	else
	{
		AVLNode<Key, Value>* traverse = static_cast <AVLNode<Key, Value>*> (this->mRoot);
		AVLNode<Key, Value>* node = NULL;
		while (traverse != NULL) //regular BST insert with some minor changes
		{
			//if either the left or right child of current node is null
			//compare the key with the key of the current node and insert accordingly
			if (traverse->getRight()==NULL || traverse->getLeft()==NULL)
			{
				if (keyValuePair.first > traverse->getKey()&&
				traverse->getRight()==NULL)	
				{
					node = 
					new AVLNode<Key, Value> (keyValuePair.first, keyValuePair.second, traverse);
					traverse->setRight(node);
					break;
				}
				else if (keyValuePair.first < traverse->getKey()&&
				traverse->getLeft()==NULL)	
				{
					node = 
					new AVLNode<Key, Value> (keyValuePair.first, keyValuePair.second, traverse);
					traverse->setLeft(node);
					break;
				}
			}
			//compare current node's key with the key of node we want to insert
			//to decide to go left or right
			if (keyValuePair.first>traverse->getKey())
			{
				traverse = traverse->getRight();
			}
			else if (keyValuePair.first<traverse->getKey())
			{
				traverse = traverse->getLeft();
			}
			else if (keyValuePair.first == traverse->getKey()) //if key is equal key of current node, update value
			{
				traverse->setValue(keyValuePair.second);
				break;
			}
		}
		AVLNode<Key, Value>* root = static_cast <AVLNode<Key, Value>*> (this->mRoot);
		calc_height(root); //calculate heights after inserting 

		if (node!=NULL) //if key didn't already exist, check if inserting new node caused an inbalnce
		{
			AVLNode<Key, Value>* unbalnode = Node_unbal (node); //try to find an unbalanced node
			if (unbalnode!=NULL) //if unbalanced node found, call helper function
			{
				rebalance(unbalnode);
			}
		}
	}
}

/**
* Remove function for a given key. Finds the node, reattaches pointers, and then balances when finished. 
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::findSuccessor (AVLNode<Key, Value>* node)
{
	AVLNode<Key, Value>* traverse = node->getRight(); //go right
	while (traverse->getLeft()!=NULL) //go all the way left to get it's successor
	{
		traverse=traverse->getLeft();
	}
	return traverse;
}
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::remove1(AVLNode<Key, Value>* node)
{
	AVLNode <Key, Value>* parent = node->getParent();
	AVLNode<Key, Value>* temp;

	if (node==(this->mRoot)) //if node is root, set the new root to be node's child that exists
	{
		if (node->getLeft()==NULL && node->getRight()==NULL) //if root is a single node in the tree, set it equal to NULL
		{
			this->mRoot=NULL;
			return NULL;
		}
	    if (node->getLeft()!=NULL) //if only left child exists, set it as the new root
		{
			node->getLeft()->setParent(NULL);
			this->mRoot = node->getLeft();
		}
		else if (node->getRight()!=NULL) //same as above but for right child
		{
			node->getRight()->setParent(NULL);
			this->mRoot = node->getRight();
	    }
		delete node;
		AVLNode<Key, Value>* root = static_cast <AVLNode<Key, Value>*>(this->mRoot);
		return root; 
	}
	if (parent->getLeft()==node) //node is in a left sub tree
	{
		if(node->getLeft()==NULL && node->getRight()==NULL) //if leaf, set parent's left child as null
		{
			parent->setLeft(NULL);
		}
		else if (node->getLeft()==NULL) //if only right child exists, have the right child take place of node
		{
			temp = node->getRight();
			parent->setLeft(temp); //reset pointers
			temp->setParent(parent);
		}
		else //same as above but for when only left child exists
		{
			temp = node->getLeft();
			parent->setLeft(temp);
			temp->setParent(parent);
		}
	}
	else if (parent->getRight()==node) //node is in a right subtree
	{
		//all same as above, but for when node is in right subtree
		if(node->getLeft()==NULL && node->getRight()==NULL) 
		{
			parent->setRight(NULL);
		}
		else if (node->getLeft()==NULL) 
		{
			temp = node->getRight();
			parent->setRight(temp);
			temp->setParent(parent);
		}
		else
		{
			temp = node->getLeft();
			parent->setRight(temp);
			temp->setParent(parent);
		}
	}	
	delete node;
	return parent;
}
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::remove2(AVLNode<Key, Value>* node)
{
	AVLNode <Key, Value>* parent = node->getParent(); //get the parent of the node we want to delete
	AVLNode<Key, Value>* succ = findSuccessor(node);
	if (node==this->mRoot)
	{
		node->getLeft()->setParent(succ);
		succ->setLeft(node->getLeft());
		if (succ->getParent()!= node) //if the successor is not the node's right child 
		{
			if (succ->getRight()!=NULL) //if the successor has a right child, it will be the new parent of the node's right child
			{
				node->getRight()->setParent(succ->getRight());
				succ->getRight()->setRight(node->getRight());
			}
			else
			{
				node->getRight()->setParent(succ); //have the successor become the new parent of the right child
				succ->setRight(node->getRight()); //make sure the succesor's new right child points to the node's right child
			}
			succ->getParent()->setLeft(NULL); //reset successor's parent's left child
		}
		succ->setParent(NULL); //since succ will be the new root, set the parent equal to null
		this -> mRoot = succ; 
		delete node;
		return succ; 
	}
	//same as above but for when node isn't a root
	if (parent->getLeft()==node) //node is in a left sub tree
	{
		parent->setLeft(succ); 
		node->getLeft()->setParent(succ);
		succ->setLeft(node->getLeft());
		if (succ->getParent()!=node) 
		{
			if (succ->getRight()!=NULL)
			{
				node->getRight()->setParent(succ->getRight());
				succ->getRight()->setRight(node->getRight());
			}
			else
			{
				node->getRight()->setParent(succ); //have the successor become the new parent of the right child
				succ->setRight(node->getRight()); //make sure the succesor's new right child points to the node's right child
			}
			succ->getParent()->setLeft(NULL); //reset successor's parent's left child
		}
		succ->setParent(parent);
	}
	else if (parent->getRight()==node) //node is in a right sub tree
	{
		parent->setRight(succ);
		node->getLeft()->setParent(succ);
		succ->setLeft(node->getLeft());
		if (succ->getParent()!=node) 
		{
			if (succ->getRight()!=NULL)
			{
				node->getRight()->setParent(succ->getRight());
				succ->getRight()->setRight(node->getRight());
			}
			else
			{
				node->getRight()->setParent(succ); //have the successor become the new parent of the right child
				succ->setRight(node->getRight()); //make sure the succesor's new right child points to the node's right child
			}
			succ->getParent()->setLeft(NULL); //reset successor's parent's left child
		}
		succ->setParent(parent);
	}
	delete node;
	return parent; //return the parent so we can use it to rebalance the tree
}
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getLeaf(AVLNode<Key, Value>* leaf)
{
	if (leaf->getRight()==NULL && leaf->getLeft()==NULL) //found leaf, return it
	{
		return leaf;
	}
	else
	{
		if (leaf->getRight()==NULL) //node only has left child
		{
			return getLeaf(leaf->getLeft()); //go down the node's right subtree
		}
		else if (leaf->getLeft()==NULL) //node only has right child
		{
			return getLeaf(leaf->getRight()); //go down the node's left subtree
		}
		else if (leaf->getRight()!=NULL && leaf->getLeft()!=NULL) //node has 2 children
		{
			if (leaf->getRight()->getHeight() >
			leaf->getLeft()->getHeight()) //if right child's height is greater than height of left child, go down that subtree
			{
				return getLeaf(leaf->getRight());
			}
			else //same as above but for left child 
			{
				return getLeaf(leaf->getLeft());
			}
		}
	}
	return leaf;
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
	AVLNode<Key, Value>* deletenode = static_cast <AVLNode<Key, Value>*> (this->internalFind(key));
	AVLNode<Key, Value>* parent=NULL;
	if(deletenode==NULL) //node is not in the tree
	{
		return;
	}
	if (deletenode->getRight()!=NULL && deletenode->getLeft()!=NULL) //node has 2 children 
	{
		parent = remove2(deletenode);
	}
	else //node has 1 or no children
	{
		parent = remove1(deletenode);
	}
	AVLNode<Key, Value>* root = static_cast <AVLNode<Key, Value>*>(this->mRoot);
	if (root!=NULL)
	{
		calc_height(root); //recalculate heights after deleting
		if (parent==this->mRoot)
		{
			AVLNode<Key, Value>* leaf = getLeaf(parent);
			AVLNode<Key, Value>* unbal  = Node_unbal (leaf); //try to find an unbalanced node
			if (unbal!=NULL) //if unbalanced node found, call helper function
			{
				rebalance(unbal);
			}
		}
		else
		{
			AVLNode<Key, Value>* unbal  = Node_unbal (parent); //try to find unbalanced node from parent of deleted node
			if (unbal!=NULL) //if unbalanced node found, call helper function
			{
				rebalance(unbal);
			}
		}
	}
		
}

/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif
