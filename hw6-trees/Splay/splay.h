#ifndef SPLAY_H
#define SPLAY_H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include "../bst/bst.h"

/**
* A templated binary search tree implemented as a Splay tree.
*/
template <class Key, class Value>
class SplayTree : public BinarySearchTree<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods.
	SplayTree();
	virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
	void remove(const Key& key);
	int report() const;

private:
	/* You'll need this for problem 5. Stores the total number of inserts where the
	   node was added at level strictly worse than 2*log n (n is the number of nodes
	   including the added node. The root is at level 0). */
	int badInserts;

	/* Helper functions are encouraged. */
	void Splay (Node<Key, Value>* splay);
	Node<Key, Value>* findSuccessor (Node<Key, Value>* node);
	Node<Key, Value>* r_rotate(Node<Key,Value>* node);
	Node<Key, Value>* l_rotate(Node<Key,Value>* node);
	Node<Key, Value>* remove1(Node<Key,Value>* node); //remove a node with 1 or no children
	Node<Key, Value>* remove2(Node<Key,Value>* node); //remove a node with 2 children

};

/*
--------------------------------------------
Begin implementations for the SplayTree class.
--------------------------------------------
*/

template<typename Key, typename Value>
SplayTree<Key, Value>::SplayTree() : badInserts(0) { }

template<typename Key, typename Value>
int SplayTree<Key, Value>::report() const {
	return badInserts;
}

/**
* Insert function for a key value pair. Finds location to insert the node and then splays it to the top.
*/
template<typename Key, typename Value>
Node<Key, Value>* SplayTree<Key, Value>::r_rotate(Node<Key,Value>* node)
{
	Node<Key, Value>* T0 = node->getLeft(); //get the smaller child of the current node
	Node<Key, Value>* T1 = T0 ->getRight(); //get the larger child of the smaller child of the current node
	
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
	
	return T0;
}
template<typename Key, typename Value>
Node<Key, Value>* SplayTree<Key, Value>::l_rotate(Node<Key,Value>* node)
{
	Node<Key, Value>* T0 = node->getRight();	//get the larger child of the current node
	Node<Key, Value>* T1 = T0 ->getLeft();	//get the smaller child of the larger child

	//if node is the root, since we are going to rotate it to the left, make the root be node's right child
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
	return T0;
}
template<typename Key, typename Value>
void SplayTree<Key, Value>::Splay (Node<Key, Value>* splay) //will return new root
{
	while (splay->getParent()!=NULL)
	{
		Node<Key, Value>* parent = splay->getParent();
		Node<Key, Value>* grandparent = parent->getParent();

		if (grandparent==NULL) //single rotations 
		{
			if (parent->getLeft()==splay) //node is in left subtree
			{
				splay = r_rotate(parent);
			}
			else if (parent->getRight()==splay)//node is in right subtree
			{
				splay = l_rotate(parent);
			}
		}
		else if (grandparent->getLeft()==parent) //node is in *a* left sub tree
		{
			if(parent->getLeft()==splay) //node is a left child, do left left
			{
				parent = (r_rotate(grandparent));//do right rotation on grandparent
				splay = (r_rotate(parent));
			}
			else if (parent->getRight()==splay) //node is a right child, do left right
			{
				splay = (l_rotate(parent)); //rotate on parent
				parent = (r_rotate(grandparent)); //rotate on grandparent
			}
		}
		else if (grandparent->getRight()==parent) //node is in *a* right subtree
		{
			if(parent->getLeft()==splay) //node is a left child, do right left
			{
				splay = (r_rotate(parent));//do right rotation on grandparent
				parent = (l_rotate(grandparent));
			}
			else if (parent->getRight()==splay) //node is a right child, do right right
			{
				parent = (l_rotate(grandparent)); //rotate on grandparent
				splay = (l_rotate(parent)); //rotate on parent
			}	
		}
	}
	this->mRoot=splay;
}
template<typename Key, typename Value>
void SplayTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
	BinarySearchTree<Key, Value>::insert(keyValuePair); //regular bst insert
	Node<Key, Value>* node = BinarySearchTree<Key, Value>::internalFind(keyValuePair.first);
	if (node==(this->mRoot))
	{
		badInserts=1; //if root was inserted (empty tree) bad insert bc h <= 2log(1)
	}
	if (this->mRoot->getKey()==keyValuePair.first)
	{
		return;
	}
	else
	{
		if (!this->value_update)
		{
			if (this->recent_level<=2*log(this->num_nodes))
			{
				badInserts++;
			}
		}
		if (node!=NULL)
		{
			Splay(node);
		}
	}
}

/**
* Remove function for a given key. Finds the node, reattaches pointers, and then splays the parent
* of the deleted node to the top.
*/
template<typename Key, typename Value>
Node<Key, Value>* SplayTree<Key, Value>::findSuccessor (Node<Key, Value>* node)
{
	Node<Key, Value>* traverse = node->getRight(); //go right
	while (traverse->getLeft()!=NULL) //go all the way left to get it's successor
	{
		traverse=traverse->getLeft();
	}
	return traverse;
}
template<typename Key, typename Value>
Node<Key, Value>* SplayTree<Key, Value>::remove1(Node<Key, Value>* node)
{
	Node <Key, Value>* parent = node->getParent();
	Node<Key, Value>* temp;

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
		return (this->mRoot); 
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
Node<Key, Value>* SplayTree<Key, Value>::remove2(Node<Key, Value>* node)
{
	Node <Key, Value>* parent = node->getParent(); //get the parent of the node we want to delete
	Node<Key, Value>* succ = findSuccessor(node);
	if (node==this->mRoot) //node is the root
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
			Node<Key, Value>* succ_old_parent = succ->getParent();
			succ->getParent()->setLeft(NULL); //reset successor's parent's left child
			succ->setParent(NULL);
			delete node;
			this->mRoot=succ; //reset root as successor
			return (succ_old_parent); //return successor's old parent since we *swapped* deleted node with successor
		}
		succ->setParent(NULL); //since succ will be the new root, set the parent equal to null
		this -> mRoot = succ; 
		delete node;
		return (succ); 
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
void SplayTree<Key, Value>::remove(const Key& key)
{
	Node<Key, Value>* find = this->mRoot; //start at the root
	Node <Key, Value>* last; //used to get last accessed node if find is NULL (i.e. node not in tree)

	while (find!=NULL) //regular BST find
	{
		if (find->getKey()==key) //check first if key is at current location & return pointer
		{
			break;
		}
		else
		{
			if (key > find->getKey()) //if key is greater than current key, go right
			{
				last=find;
				find =find->getRight();
			}
			else if (key < find->getKey())	//if key is less than current key, go left
			{
				last=find;
				find = find->getLeft();
			}
		}
	}
	if (find==NULL)
	{
		Splay(last);
	}
	else
	{
		Node<Key, Value>* parent=NULL;
		if (find->getRight()!=NULL && find->getLeft()!=NULL) //node has 2 children 
		{
			parent = remove2(find);
			Splay(parent);
		}
		else //node has 1 or no children
		{
			parent = remove1(find);
			if(parent!=NULL) //if the node isn't null (i.e. deleted node isn't root with no children)
			{
				Splay(find);
			}
		}
	}

}

/*
------------------------------------------
End implementations for the SplayTree class.
------------------------------------------
*/

#endif
