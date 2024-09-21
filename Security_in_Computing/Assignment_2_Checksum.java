/*=============================================================================
|	Assignment: pa02 - Calculating an 8, 16, or 32 bit
|						checksum on an ASCII input file
|
|
|	Language: java
|
|	To Compile: javac Assignment_2_Checksum.java
|
|	To Execute:	java	-> java Assignment_2_Checksum inputFile.txt 8
|
|
|	Note: All input files are simple 8 bit ASCII input
|
|	Class: CIS3360 - Security in Computing - Fall 2022
|
+=============================================================================*/

import java.io.*;
import java.util.*;

class Assignment_2_Checksum
{
	private static final int MAX_LINE = 80;

	public static void printChecksum(StringBuilder hexFinal, StringBuilder hexString, int checksumSize)
	{
		StringBuilder hexOutput = new StringBuilder("        ");
		StringBuilder checksumOutput = new StringBuilder("  ");
		StringBuilder stringSizeOutput = new StringBuilder("    ");

		// Setup checksumOutput
		if (checksumSize == 8)
		{
			checksumOutput.replace(1, 2, Integer.toString(checksumSize));
		}
		else
		{
			checksumOutput.replace(0, 2, Integer.toString(checksumSize));
		}

		// Setup hexOutput
		hexOutput.replace(8 - hexFinal.length(), 8, hexFinal.toString());

		// Setup stringSizeOutput
		String numchars = Integer.toString((hexString.length() / 2));
		stringSizeOutput.replace(4 - numchars.length(), 4, numchars);

		System.out.println(checksumOutput.toString() + " bit checksum is " + hexOutput.toString() +
							" for all " + stringSizeOutput.toString() + " chars");
	}

	public static String findHex(StringBuilder hexString, int checksumSize)
	{
		int size = checksumSize / 4;
		long temp1 = 0, temp2;
		for (int i = size; i <= hexString.length(); i += size)
		{
			temp2 = Integer.parseInt(hexString.substring(i - size, i), 16);
			temp1 = temp2 + temp1;
		}
		String ret = String.format("%08x", temp1);
		ret = ret.substring(ret.length() - size, ret.length());
		return ret;
	}

	private static StringBuilder checkPadding(StringBuilder hex, int checksumSize)
	{
		if (checksumSize == 16)
		{
			if (hex.length() % 4 != 0)
			{
				hex.append(Integer.toHexString((int) 'X'));
				System.out.printf("X");
			}
		}
		else
		{
			if (hex.length() % 8 != 0)
			{
				if (hex.length() % 8 == 2)
				{
					hex.append(Integer.toHexString((int) 'X'));
					System.out.printf("X");
				}
				if (hex.length() % 8 == 4)
				{
					hex.append(Integer.toHexString((int) 'X'));
					System.out.printf("X");
				}
				if (hex.length() % 8 == 6)
				{
					hex.append(Integer.toHexString((int) 'X'));
					System.out.printf("X");
				}
			}
		}
		return hex;
	}

	public static StringBuilder findChecksum(StringBuilder input, int checksumSize)
	{
		StringBuilder hex = new StringBuilder();
		for (int i = 0; i < input.length(); i++)
			hex.append(Integer.toHexString((int) input.charAt(i)));
		hex.append("0a");

		if (checksumSize != 8)
		{
			hex = checkPadding(hex, checksumSize);
		}
		System.out.println("");

		return hex;
	}

	public static void main(String [] args) throws IOException
	{
		// Checks if sufficent arguments were passed
		if (args.length != 2)
		{
			System.err.println("Program not executed properly. Need inputfile and checksum size");
			return;
		}


		// Checks that a proper checksum was passed
		int checksumSize = Integer.parseInt(args[1]);
		if (checksumSize % 8 != 0 || checksumSize < 8 || checksumSize > 32)
		{
			System.err.println("Valid checksum sizes are 8, 16, or 32");
			return;
		}

		// Reads in the file to input string
		Scanner in = new Scanner(new File(args[0]));
		StringBuilder input = new StringBuilder();
		while (in.hasNext())
			input.append(in.nextLine());
		in.close();
		// Breaks up the input string into 80 char substrings in an arraylist
		ArrayList<String> list = new ArrayList<>();
		int i = MAX_LINE, temp = input.length();
		while (true)
		{
			if (i > temp)
			{
				list.add(input.substring(i - MAX_LINE, temp));
				break;
			}
			else
			{
				list.add(input.substring(i - MAX_LINE, i));
				i += MAX_LINE;
			}
		}

		// Prints out the broken up string
		System.out.println("");
		for (i = 0; i < list.size(); i++)
			System.out.println(list.get(i));


		// calculate the checksum
		StringBuilder hexString = findChecksum(input, checksumSize);
		StringBuilder hexFinal = new StringBuilder(findHex(hexString, checksumSize));
		while (hexFinal.charAt(0) == '0')
			hexFinal = hexFinal.deleteCharAt(0);


		// Print out the checksum using proper format
		printChecksum(hexFinal, hexString, checksumSize);
		// System.out.printf("%s bit checksum is %s for all %s chars \n", )

	}

}
