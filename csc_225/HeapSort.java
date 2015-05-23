/* HeapSort.java
   CSC 225 - Fall 2014
   Assignment 3 - Template for Heap Sort

   This template includes some testing code to help verify the implementation.
   To interactively provide test inputs, run the program with
	java HeapSort

   To conveniently test the algorithm with a large input, create a
   text file containing space-separated integer values and run the program with
	java HeapSort file.txt
   where file.txt is replaced by the name of the text file.
*/

import java.util.Scanner;
import java.util.Vector;
import java.util.Arrays;
import java.io.File;
import java.util.*;
import java.io.*;

//Do not change the name of the HeapSort class
public class HeapSort{
	/* HeapSort(A)
		Sort the array A using heap sort.
		You may add additional functions (or classes) if needed, but the entire program must be
		contained in this file.

		Do not change the function signature (name/parameters).
	*/
	
	/*
	Swaps 2 integer values within the array
	*/
	public static void swap(int i, int j, int[] heap)
	{
	int temp = heap[i];
	heap[i] = heap[j];
	heap[j] = temp;
	}
	
	/*
	Takes 2 addresses of an array and returns the one with the smallest value
	if the address is less than the index
	*/
	public static int mincompare(int i, int j, int[] heap, int index)
	{
	if(i > index)
	{
	return j;
	} else if(j > index)
	{
	return i;
	}
	else if(heap[i] > heap[j])
	{
	return j;
	} else 
	{
	return i;
	}
	}
	
	/*
	This inserts each element of array A into the min heap array
	*/
	public static void HeapSort(int[] A)
	{
		int n = A.length;
		int[] heap = new int[n];
		int k, temp;
		
		for(k = 0; k < A.length; k++)
		{
			int m = k;
			heap[k] = A[k];
			while(heap[m] < heap[(m-1)/2])
			{
			temp = heap[m];
			heap[m] = heap[(m-1)/2];
			heap[(m-1)/2] = temp;
			m = (m-1)/2;
			}
		} //for
		
		/*
		This first assigns the root value of the min heap to the array
		before swapping the min value and the last value of the min heap
		which is then bubbled down to it's correct position.
		*/
		int index = heap.length-1;
		for(int j = 0; j < heap.length; j++)
		{
		A[j] = heap[0];
		swap(0, index, heap);
		index--;
		int m = 0;
		int left = 2*m+1;
		int right = 2*m+2;
		if(left > index && right > index)
		{
		A[j+1] = heap[0];
		return;
		}
		int min = mincompare(left, right, heap, index);
			while(heap[m] > heap[min])
			{
			swap(m, min, heap);
			m = min;
			right = 2*m+2;
			left = 2*m+1;
			if(left > index && right > index)
			{
			break;
			}
			min = mincompare(left, right, heap, index);
			} //while
		} //for
		
	} //heapsort

	/* main()
	   Contains code to test the HeapSort function. Nothing in this function
	   will be marked. You are free to change the provided code to test your
	   implementation, but only the contents of the HeapSort() function above
	   will be considered during marking.
	*/
	public static void main(String[] args){
		Scanner s;
		if (args.length > 0){
			try{
				s = new Scanner(new File(args[0]));
			} catch(java.io.FileNotFoundException e){
				System.out.printf("Unable to open %s\n",args[0]);
				return;
			}
			System.out.printf("Reading input values from %s.\n",args[0]);
		}else{
			s = new Scanner(System.in);
			System.out.printf("Enter a list of non-negative integers. Enter a negative value to end the list.\n");
		}
		Vector<Integer> inputVector = new Vector<Integer>();

		int v;
		while(s.hasNextInt() && (v = s.nextInt()) >= 0)
			inputVector.add(v);

		int[] array = new int[inputVector.size()];

		for (int i = 0; i < array.length; i++)
			array[i] = inputVector.get(i);

		System.out.printf("Read %d values.\n",array.length);


		long startTime = System.currentTimeMillis();

		HeapSort(array);

		long endTime = System.currentTimeMillis();

		double totalTimeSeconds = (endTime-startTime)/1000.0;

		//Don't print out the values if there are more than 100 of them
		if (array.length <= 100){
			System.out.println("Sorted values:");
			for (int i = 0; i < array.length; i++)
				System.out.printf("%d ",array[i]);
			System.out.println();
		}

		//Check whether the sort was successful
		boolean isSorted = true;
		for (int i = 0; i < array.length-1; i++)
			if (array[i] > array[i+1])
				isSorted = false;

		System.out.printf("Array %s sorted.\n",isSorted? "is":"is not");
		System.out.printf("Total Time (seconds): %.2f\n",totalTimeSeconds);
	}
}
