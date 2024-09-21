// COP 3503, Fall 2022

import java.lang.*;
import java.util.*;

public class SneakyKnights
{
	// Position object is used to hold the row and column of a knight.
	static class Position
	{
		int row, column;
		// Default constructor
		Position()
		{
			column = 0;
			row = 0;
		}
		// Overloaded constructor
		Position(int column, int row)
		{
			this.row = row;
			this.column = column;
		}

		// Change in equals method for Position object to ensure that not only the hashCodes
		// the same but also each row and column is the same.
		@Override
		public boolean equals(Object obj)
		{
			if (this == obj)
				return true;

			if (obj == null || obj.getClass() != this.getClass())
				return false;

			// After proving that both are the same class I am able to make obj a Position object
			Position temp = (Position) obj;
			if (temp.hashCode() != this.hashCode())
				return false;

			if (temp.column != this.column || temp.row != this.row)
				return false;

			return true;
		}

		// Change in hashCode based of Integer.hashCode where it just returns the int.
		// Multiplies the row and column after using spread to cause less colisions.
		@Override
		public int hashCode()
		{
			int spread = 3;
			return (row - spread) * (column + spread);
		}
	}

	// Checks if the HashSet set contains a knight in any position that curKnight can attack.
	// If there is a knight in an attackable position return true otherwise return false.
	public static boolean checkUnsafe(HashSet<Position> set, Position curKnight)
	{
		return (set.contains(new Position(curKnight.column + 2, curKnight.row + 1))||
		set.contains(new Position(curKnight.column + 2, curKnight.row - 1))||
		set.contains(new Position(curKnight.column - 2, curKnight.row + 1))||
		set.contains(new Position(curKnight.column - 2, curKnight.row - 1))||
		set.contains(new Position(curKnight.column + 1, curKnight.row + 2))||
		set.contains(new Position(curKnight.column + 1, curKnight.row - 2))||
		set.contains(new Position(curKnight.column - 1, curKnight.row + 2))||
		set.contains(new Position(curKnight.column - 1, curKnight.row - 2)));
	}

	// Checks is any knight, with positions given by coordinateStrings, is able to attack another.
	// Uses HashSet in order to hold the position of each knight.
	public static boolean
	allTheKnightsAreSafe(ArrayList<String> coordinateStrings, int boardSize)
	{
		// If only one or no knights then all knights are safe
		if (coordinateStrings.size() <= 1)
			return true;

		// Creates HashSet based on size of coordinateStrings to do least ammount of rehashing
		float loadFactor = (float)0.75;
		int initalCapacity = (int)Math.ceil(coordinateStrings.size() * 8 / loadFactor);
		HashSet<Position> set = new HashSet<Position>(initalCapacity, loadFactor);

		// Loop through the coordinateStrings to get each knight and see if they are safe
		for (int i = 0; i < coordinateStrings.size(); i++)
		{
			String curString = coordinateStrings.get(i);
			Position curKnight = new Position();

			// Changes string to column and row in a Position object
			for (int j = 0; j < curString.length(); j++)
			{
				Character tempChar = curString.charAt(j);
				if (Character.isAlphabetic​(tempChar))
					curKnight.column = curKnight.column * 26 + tempChar % 'a' + 1;
				else
					curKnight.row = curKnight.row * 10 + tempChar % '0';
			}
			// Checks if any knights are in the HashSet that can be attacked by curKnight
			if (checkUnsafe(set, curKnight))
			{
				return false;
			}
			// If no unsafe knight in the HashSet then adds curKnight
			set.add(curKnight);
		}
		return true;
	}


	public static double difficultyRating()
	{
		return 4.05;
	}

	public static double hoursSpent()
	{
		return 22.5;
	}
}
