// COP 3503, Fall 2022

import java.io.*;
import java.util.*;

// Helper class that holds a vertex's information of incoming and outgoing edges.
class Node
{
	private int num_incoming;
	private boolean [] incoming;
	private boolean [] outgoing;

	// Initalizes the arrays for the node with the potential edges that it could have.
	public Node(int num_vertex)
	{
		incoming = new boolean [num_vertex];
		outgoing = new boolean [num_vertex];
	}

	// Accessor for num_incoming
	public int getNumIncoming()
	{
		return num_incoming;
	}

	// Returns true if the node calling this has an outgoing edge to the node passed in.
	public boolean isGoingTo(int node)
	{
		return outgoing[node];
	}

	// Indicates where an outgoing edge is point to.
	public void setOutgoing(int node)
	{
		outgoing[node] = true;
	}

	// Indicates the node that an edge is coming from and increases num_incoming.
	public void setIncoming(int node)
	{
		incoming[node] = true;
		num_incoming++;
	}

}

public class ConstrainedTopoSort
{
	private int num_vertex;
	private ArrayList<Node> graph;
	// hasValidTopo is set to false is basic cases of TopoSort fail in order to give an escape.
	private boolean hasValidTopo;

	public ConstrainedTopoSort(String filename) throws IOException
	{
		// Intializes class variables and opens the file.
		hasValidTopo = true;
		Scanner in = new Scanner(new File(filename));
		num_vertex = in.nextInt();
		graph = new ArrayList<Node>();

		// Fills the ArrayList<Node>.
		for (int i = 0; i < num_vertex; i++)
			graph.add(new Node(num_vertex));

		// Reads the file and edits the nodes edges
		for (int i = 0; i < num_vertex; i++)
		{
			int num_outgoing = in.nextInt();
			// For each edge indicate the outgoing node i and incoming node for the indicated edge
			for (int j = 0; j < num_outgoing; j++)
			{
				int going_to = in.nextInt();
				graph.get(i).setOutgoing(going_to - 1);
				graph.get(going_to - 1).setIncoming(i);
			}
		}

		in.close();

		// Checks if there are no nodes without an incoming edge. If all nodes have an incoming
		// edge then there is no valid TopoSort.
		boolean temp = false;
		for (int i = 0; i < num_vertex; i++)
		{
			if (graph.get(i).getNumIncoming() == 0)
			{
				temp = true;
				break;
			}
		}
		hasValidTopo = temp;

		// Checks if there are any direct loops. If there are such loops then there is no TopoSort.
		temp = true;
		for (int i = 0; i < num_vertex && temp; i++)
		{
			for (int j = 0; j < num_vertex && temp; j++)
			{
				if (graph.get(i).isGoingTo(j))
					if (graph.get(j).isGoingTo(i))
						temp = false;
			}
		}
		hasValidTopo = temp;
	}

	public boolean hasConstrainedTopoSort(int x, int y)
	{
		// Sets x and y down one as this code uses array based indicies.
		x--;
		y--;
		// Prevents repeated get calls.
		Node x_node = graph.get(x);
		Node y_node = graph.get(y);
		// Early escape based on how the graph was made.
		if (!hasValidTopo)
			return false;
		// Quick check to prove that y cannot come after x.
		if (y_node.isGoingTo(x))
			return false;
		// If looking for a node to come after itself then return false as that is not possible.
		if (x == y)
			return false;

		// Setup elements needed to do TopoSort in a generic sense.
		int [] incoming = new int [num_vertex];
		boolean [] visited = new boolean [num_vertex];
		for (int i = 0; i < num_vertex; i++)
			incoming[i] = graph.get(i).getNumIncoming();
		Queue<Integer> q = new ArrayDeque<Integer>();

		// if x has no incoming edges start with x as it has to be first in the TopoSort.
		if (incoming[x] == 0)
		{
			q.add(x);
			visited[x] = true;
		}

		// Adds everything else that has no incoming edges to the queue.
		for (int i = 0; i < num_vertex; i++)
		{
			if (incoming[i] == 0 && !visited[i])
			{
				q.add(i);
				visited[i] = true;
			}
		}

		// Preforms TopoSort based on what is in the queue.
		while (!q.isEmpty())
		{
			int temp = q.remove();
			// Because y has to come before x do not calculate anything once y is found.
			// This prevents anything that has an edge from y to be visted.
			if(temp == y)
			{
				continue;
			}

			// For all the nodes that have an edge from the temp node. Decrease the number of
			// incoming and if at 0 incoming add it to the queue.
			for (int i = 0; i < num_vertex; i++)
			{
				if (graph.get(temp).isGoingTo(i) && --incoming[i] == 0)
				{
					q.add(i);
					visited[i] = true;
				}
				// Checks if both x and y have been processed and put into the linked list.
				// If yes than there is a TopoSort where x can go before y.
				if (visited[x] && visited[y])
					return true;
			}
		}
		return false;
	}

	public static double difficultyRating()
	{
		return 2.75;
	}

	public static double hoursSpent()
	{
		return 10.5;
	}
}
