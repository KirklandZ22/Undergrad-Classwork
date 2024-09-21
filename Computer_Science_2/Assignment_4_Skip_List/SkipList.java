// COP 3503, Fall 2022

import java.io.*;
import java.util.*;


// Node class for SkipList. Holds the data of generic type that is comparable, the node height and
// pointers to next nodes.
class Node<AnyType extends Comparable<AnyType>>
{
	private AnyType data;
	private int height;
	private ArrayList<Node<AnyType>> next;

	// Makes node with no data stored (used for head nodes)
	Node(int height)
	{
		this.height = height;
		next = new ArrayList<Node<AnyType>>(height);
		for (int i = 0; i < height; i++)
			next.add(null);
	}

	// Makes node with data
	Node(AnyType data, int height)
	{
		this.data = data;
		this.height = height;
		next = new ArrayList<Node<AnyType>>(height);
		for (int i = 0; i < height; i++)
			next.add(null);
	}

	// data accesser
	public AnyType value()
	{
		return data;
	}

	// height accesser
	public int height()
	{
		return height;
	}

	// Gets next at specific level, if nothing there then return null
	public Node<AnyType> next(int level)
	{
		return (level >= 0 && level < height) ? next.get(level) : null;
	}

	// Saves the node in the ArrayList at the specific level
	public void setNext(int level, Node<AnyType> node)
	{
		next.set(level, node);
	}

	// Increases the height of the node by one
	public void grow()
	{
		next.add(null);
		height++;
	}

	// Checks to see if the node should grow based on probability and grow if needed
	public boolean maybeGrow()
	{
		double rand = Math.random();
		double prob = 1.0 / 2;
		if (rand < prob)
		{
			next.add(null);
			height++;
			return true;
		}
		else
		{
			return false;
		}
	}

	// Decreases the height of the current node to the height passed
	public void trim(int height)
	{

		for (int i = height; i < this.height; i++)
			next.set(i, null);
		this.height = height;
	}

}

// Creates a probabilistic SkipList using generic that are comparable
public class SkipList<AnyType extends Comparable<AnyType>>
{

	private Node<AnyType> head;
	private int size;

	// Creates empty skiplist with height of 1
	SkipList()
	{
		head = new Node<AnyType>(1);
	}

	// Creates empty skiplist with the head having the height passed in
	SkipList(int height)
	{
		if (height < 1)
			head = new Node<AnyType>(1);
		else
			head = new Node<AnyType>(height);
	}

	// size accesser
	public int size()
	{
		return size;
	}

	// height accesser
	public int height()
	{
		return head.height();
	}

	// head accesser
	public Node<AnyType> head()
	{
		return head;
	}

	// Finds the ceiling of the Log based 2 of x
	public static int ceilngLog2(int x)
	{
		return (int)Math.ceil(Math.log(x) / Math.log(2));
	}

	// Traverses through the ArrayList to find the node directly before the location of data
	// Stores each node when the level is decreased in an ArrayList to allow ease of access.
	public ArrayList<Node<AnyType>> findDroppedNodes(AnyType data)
	{
		int level = head.height() - 1;
		ArrayList<Node<AnyType>> dropped_nodes = new ArrayList<Node<AnyType>>(head.height());
		// Initalize the ArrayList to be empty
		for (int i = 0; i < head.height(); i++)
			dropped_nodes.add(i, null);

		Node<AnyType> temp = head;

		// traverse through the SkipList. Level is used to indicate which height is looked at
		while (level >= 0)
		{
			// If there is a next node at same level and value to insert goes after it then move
			if (temp.next(level) != null && temp.next(level).value().compareTo(data) < 0)
			{
				temp = temp.next(level);
			}
			// Add node that is dropped to the ArrayList and decrease level to look at
			else
			{
				dropped_nodes.set(level, temp);
				level--;
			}
		}
		return dropped_nodes;
	}

	// Connects the newly created temp node to the nodes that came before it.
	public void connectNodes(Node<AnyType> temp, ArrayList<Node<AnyType>> dropped_nodes)
	{
		Node<AnyType> dropped_from;
		// For each level put temps next to dropped next and connect temp to dropped
		for (int level = 0; level < temp.height(); level++)
		{
			dropped_from = dropped_nodes.get(level);
			temp.setNext(level, dropped_from.next(level));
			dropped_from.setNext(level, temp);
		}
		size++;
	}

	// Adds a node with data and a random height to the SkipList
	public void insert(AnyType data)
	{
		// Check if need to grow the SkipList
		if (ceilngLog2(size + 1) > head.height())
			growSkipList();

		ArrayList<Node<AnyType>> dropped_nodes = findDroppedNodes(data);
		Node<AnyType> created = new Node<AnyType>(data, generateRandomHeight(head.height()));
		connectNodes(created, dropped_nodes);
	}

	// Adds a node with data and a set height to the skiplist
	public void insert(AnyType data, int height)
	{
		// Check if need to grow the SkipList
		if (ceilngLog2(size + 1) > head.height())
			growSkipList();

		ArrayList<Node<AnyType>> dropped_nodes = findDroppedNodes(data);
		Node<AnyType> created = new Node<AnyType>(data, height);
		connectNodes(created, dropped_nodes);
	}

	// Deletes the first node with data in it. If no node has data then does nothing.
	public void delete(AnyType data)
	{
		ArrayList<Node<AnyType>> dropped_nodes = findDroppedNodes(data);

		// If the node does not exist in the SkipList then do nothing
		if (dropped_nodes.get(0).next(0) == null ||
			dropped_nodes.get(0).next(0).value().compareTo(data) != 0)
			return;

		Node<AnyType> temp = dropped_nodes.get(0).next(0);

		// Sets the pointers from the node to be deleted to the nodes that was dropped_from
		for (int i = 0; i < temp.height(); i++)
		{
			dropped_nodes.get(i).setNext(i, temp.next(i));
		}

		size--;
		// Checks if the SkipList needs to be resized
		if (ceilngLog2(size) < head.height())
			trimSkipList();
	}

	// Sees if the SkipList contains the data
	public boolean contains(AnyType data)
	{
		int level = head.height() - 1;
		Node<AnyType> temp = head;

		while (level >= 0)
		{
			// If there is nothing left at the level go down
			if (temp.next(level) == null)
			{
				level--;
				continue;
			}
			double compared_value = temp.next(level).value().compareTo(data);
			// If the next at the level is the data return true
			if (compared_value == 0)
				return true;
			// If the next at the level is less than the data go to next at same level
			else if (compared_value < 0)
				temp = temp.next(level);
			else
				level--;
		}
		return false;
	}
	// Sees if SkipList has a node that contains data and returns that node
	public Node<AnyType> get(AnyType data)
	{
		int level = head.height() - 1;
		Node<AnyType> temp = head;

		while (level >= 0)
		{
			// If there is nothing left at the level go down
			if (temp.next(level) == null)
			{
				level--;
				continue;
			}
			double compared_value = temp.next(level).value().compareTo(data);
			// If the next at the level is the data return true
			if (compared_value == 0)
				return temp.next(level);
			// If the next at the level is less than the data go to next at same level
			else if (compared_value < 0)
				temp = temp.next(level);
			else
				level--;
		}
		return null;
	}

	public static double difficultyRating()
	{
		return 4.1;
	}

	public static double hoursSpent()
	{
		return 13.5;
	}

	// Creates a random height using probability
	private static int generateRandomHeight(int maxHeight)
	{
		double rand = Math.random();
		double prob = 1.0 / 2;
		int count = 1;
		while (rand < prob && count < maxHeight)
		{
			prob /= 2;
			count++;
		}

		return count;
	}

	// Increases the max height of the skiplist by 1. If nodes were the old max height give them a
	// chance to grow.
	private void growSkipList()
	{
		int old_height = head.height();
		int new_height = old_height + 1;

		head.grow();

		Node<AnyType> temp_grown = head;
		Node<AnyType> temp_maybe = head.next(old_height);

		// Traverses the top level of the SkipList and sees if any of them get to grow
		while (temp_maybe != null)
		{
			if (temp_maybe.maybeGrow())
			{
				temp_grown.setNext(new_height, temp_maybe);
				temp_grown = temp_maybe;
			}
			temp_maybe = temp_maybe.next(old_height);
		}
	}

	// Decreases the height of the SkipList to fit the ceiling of the Log based 2 of the size
	private void trimSkipList()
	{
		Node<AnyType> temp = head, to_trim;
		int new_height;
		if (size == 1 || size == 0)
			new_height = 1;
		else
			new_height = ceilngLog2(size);

		// Traverses the SkipList and decreases everything to the new height if taller.
		while (temp != null)
		{
			if (temp.height() > new_height)
				temp.trim(new_height);
			temp = temp.next(new_height - 1);
		}
	}

}
