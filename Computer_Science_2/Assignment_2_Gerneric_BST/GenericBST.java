// COP 3503, Fall 2022

// ====================
// GenericBST: BST.java
// ====================
// Basic binary search tree (BST) implementation that supports insert() and
// delete() operations. This framework is provided for you to modify as part of
// Programming Assignment #2.

import java.io.*;
import java.util.*;


// Node holds the data and the node address of the left and right subtree root node.
class Node<AnyType extends Comparable<AnyType>>
{
	AnyType data;
	Node<AnyType> left, right;

	Node(AnyType data)
	{
		this.data = data;
	}
}

// Code for a Binary Search Tree that uses generics with the restrictions of the comarable class.
public class GenericBST<T extends Comparable<T>>
{
	private Node<T> root;

	// Helper class for insert
	public void insert(T data)
	{
		root = insert(root, data);
	}

	// Uses recursion to insert new data in BST.
	// Takes the root of the subtree and the data to be inserted.
	// Returns new subtree with new Node added.
	private Node<T> insert(Node<T> root, T data)
	{
		// Base case: if no root then new node with data is the root.
		if (root == null)
		{
			return new Node<T>(data);
		}
		// Finds which subtree to go into to add the data.
		else if (data.compareTo(root.data) < 0)
		{
			root.left = insert(root.left, data);
		}
		else if (data.compareTo(root.data) > 0)
		{
			root.right = insert(root.right, data);
		}

		return root;
	}

	// Helper class for delete.
	public void delete(T data)
	{
		root = delete(root, data);
	}

	// Uses recursion to find the data in the BST to delete.
	// Takes in the root of the subtree the data might be in and the data to be deleted.
	// returns the new subtree with the node removed.
	private Node<T> delete(Node<T> root, T data)
	{
		// Base case: if no tree then the data is not there.
		if (root == null)
		{
			return null;
		}
		// Finds which subtree to search for the data in.
		else if (data.compareTo(root.data) < 0)
		{
			root.left = delete(root.left, data);
		}
		else if (data.compareTo(root.data) > 0)
		{
			root.right = delete(root.right, data);
		}
		// If the data is found
		else
		{
			// No children then it disapears.
			if (root.left == null && root.right == null)
			{
				return null;
			}
			// Only one child so the child moves up.
			else if (root.left == null)
			{
				return root.right;
			}
			else if (root.right == null)
			{
				return root.left;
			}
			// Two Children: Replaces the data to delete with the max value in the left subtree.
			// Then deletes the max value in the left subtree.
			else
			{
				root.data = findMax(root.left);
				root.left = delete(root.left, root.data);
			}
		}

		return root;
	}

	// This method assumes root is non-null, since this is only called by
	// delete() on the left subtree, and only when that subtree is not empty.
	// Goes as far right in the left subtree to find the value.
	private T findMax(Node<T> root)
	{
		while (root.right != null)
		{
			root = root.right;
		}

		return root.data;
	}

	// Helper class for contains.
	public boolean contains(T data)
	{
		return contains(root, data);
	}

	// Takes in the root of a subtree that might contain the data and the data.
	// Returns true if the data is found and false if the data is not in the BST.
	private boolean contains(Node<T> root, T data)
	{
		// Base case: if no tree then data not in tree.
		if (root == null)
		{
			return false;
		}
		// Finds which tree to look into next
		else if (data.compareTo(root.data) < 0)
		{
			return contains(root.left, data);
		}
		else if (data.compareTo(root.data) > 0)
		{
			return contains(root.right, data);
		}
		// The data is found therefore it is in the BST.
		else
		{
			return true;
		}
	}

	// Helper class for inorder
	public void inorder()
	{
		System.out.print("In-order Traversal:");
		inorder(root);
		System.out.println();
	}

	// Inorder traversal prints the left subtree, the root, then right subtree using recursion.
	// Takes the root of the subtree to print. Returns void.
	private void inorder(Node<T> root)
	{
		if (root == null)
			return;

		inorder(root.left);
		System.out.print(" " + root.data);
		inorder(root.right);
	}

	// Helper class for preorder
	public void preorder()
	{
		System.out.print("Pre-order Traversal:");
		preorder(root);
		System.out.println();
	}

	// Preorder traversal prints the root, left subtree, then right subtree using recursion.
	// Takes the root of the subtree to print. Returns void.
	private void preorder(Node<T> root)
	{
		if (root == null)
			return;

		System.out.print(" " + root.data);
		preorder(root.left);
		preorder(root.right);
	}

	// Helper class for postorder
	public void postorder()
	{
		System.out.print("Post-order Traversal:");
		postorder(root);
		System.out.println();
	}

	// Postorder traversal prints tthe left subtree, right subtree, then the root using recursion.
	// Takes the root of the subtree to print. Returns void.
	private void postorder(Node<T> root)
	{
		if (root == null)
			return;

		postorder(root.left);
		postorder(root.right);
		System.out.print(" " + root.data);
	}

	public static double difficultyRating()
	{
		return 2.89;
	}

	public static double hoursSpent()
	{
		return 6.25;
	}


}
