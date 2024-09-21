// COP 3503, Fall 2022

import java.util.*;
import java.lang.*;

class RunLikeHell
{
	// Runtime: O(n) where n is blocks.length. Space: O(1).
	public static int maxGain(int [] blocks)
	{
		// Checks on size of blocks array. Ensures no out of bounds issues.
		if (blocks == null || blocks.length == 0)
			return 0;
		if (blocks.length == 1)
			return blocks[0];

		// Sets up the matrix and fills base values
		int matrix[] = new int [2];
		matrix[0] = blocks[0];
		matrix[1] = Integer.max(blocks[0], blocks[1]);
		// Loops through block and updates the matrix to hold the value to be accessed in future
		// calls.
		for (int i = 2; i < blocks.length; i++)
			matrix[i % 2] = Integer.max(blocks[i] + matrix[(i - 2) % 2], matrix[(i - 1) % 2]);
		return matrix[(blocks.length - 1) % 2];
	}

	public static double difficultyRating()
	{
		return 2.0;
	}

	public static double hoursSpent()
	{
		return 5.0;
	}
}
