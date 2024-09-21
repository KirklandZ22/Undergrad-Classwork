// COP 3503, Fall 2022

import java.io.*;
import java.util.*;

public class SneakyQueens
{

    // Creates the queen object that stores the location of a specfic queen.
    // Runs in O(k) where k is the length of the string
    public static Queens createQueen(String str)
    {
        int i = 0;
        Queens cur_queen = new Queens();
        // Loops through the letter part of the string to get the numeric value of the column.
        while (Character.isAlphabetic(str.charAt(i)))
        {
            cur_queen.column_pos = (int)(cur_queen.column_pos * 26 + str.charAt(i) % 'a' + 1);
            i++;
        }
        // Remain part string is all numbers.
        cur_queen.row_pos = Integer.parseInt(str.substring(i));

        // Top down is found by equation y = -x + c => y + x = c where y = row and x = column.
        cur_queen.top_down_diagonal = cur_queen.row_pos + cur_queen.column_pos;
        // Bottom up is found by equation y = x + c => y - x = c where y = row and x = column.
        cur_queen.bottom_up_diagonal = cur_queen.row_pos - cur_queen.column_pos;
        return cur_queen;
    }

    // Takes the values of all the queens and sees if there are any repeating.
    // This is done by placing each value (row, column, and diagonals) into unique Hash Tables.
    // Runs in O(n) where n is the number of strings.
    public static boolean compareQueenLocations(Queens[] queen, int num_queens)
    {
        int hash_size = (int)(num_queens / 0.75 + 1);
        // Using Hash Table as the add function is O(1) and checks for repeating values.
        HashSet<Integer> all_rows = new HashSet<Integer>(hash_size);
        HashSet<Integer> all_columns = new HashSet<Integer>(hash_size);
        HashSet<Integer> all_top_down = new HashSet<Integer>(hash_size);
        HashSet<Integer> all_bottom_up = new HashSet<Integer>(hash_size);

        // Loop to add each value the queen has into the appropiate Hash Table.
        // If add fails then it is a repeating value meaning that not all queens are safe.
        // If after adding all non repeat then all are safe.
        for (int i = 0; i < num_queens; i++)
        {
            if (!all_rows.add(queen[i].row_pos) || !all_columns.add(queen[i].column_pos) ||
               !all_top_down.add(queen[i].top_down_diagonal) ||
               !all_bottom_up.add(queen[i].bottom_up_diagonal))
                return false;
        }
        return true;
    }

    // Being passed a ArrayList of locations of queens will determine if any queens can
    // attack one another.
    // Runs in O(kn) where k is the length of the string and n is the number of strings.
    public static boolean
    allTheQueensAreSafe(ArrayList<String> CoordinateStrings, int boardSize)
    {
        // If only 1 or 0 queens then they are all safe
        if (CoordinateStrings.size() < 2)
            return true;

        // Creates array of queens to hold positions of each queens
        Queens[] queen = new Queens[CoordinateStrings.size()];

        // Makes each queen with its unique values based on the string
        for (int i = 0; i < CoordinateStrings.size(); i++)
        {
            queen[i] = createQueen(CoordinateStrings.get(i));
        }

        // Compares each queen in the Queens array to see if any could attack each other.
        return compareQueenLocations(queen, CoordinateStrings.size());
    }

    public static double difficultyRating()
    {
        return 2.5;
    }

    public static double hoursSpent()
    {
        return 9;
    }
}

// Class to hold each unique position of a specfic queen.
class Queens
{
    // column_pos intialized to 0 as base value for any math done
    int column_pos = 0;
    int row_pos;
    int top_down_diagonal;
    int bottom_up_diagonal;


}
