/* TripleSum.java
   CSC 225 - Summer 2014
   Assignment 1 - Template for TripleSum
   
   This template includes some testing code to help verify the implementation.
   To interactively provide test inputs, run the program with
	java TripleSum

   To conveniently test the algorithm with a large input, create a 
   text file containing space-separated integer values and run the program with
	java TripleSum file.txt
   where file.txt is replaced by the name of the text file.

   B. Bird - 04/30/2014
*/

import java.util.Scanner;
import java.util.Vector;
import java.util.Arrays;
import java.io.File;

//Do not change the name of the TripleSum class
public class TripleSum{

	/* TripleSum225()
		The input array A will contain non-negative integers. The function
		will search the input array A for three elements which sum to 225.
		If such a triple is found, return true. Otherwise, return false.
		The triple may contain the same element more than once.
		The function may modify the array A.

		Do not change the function signature (name/parameters).
	*/
	
	public static boolean TripleSum225(int[] A){
		int[] my225 = new int[226];
		int count;
		
		/*
		This creates an array that has the number 1 at each location
		that Array A has that number. Ex: Array A has number 224 in it,
		then my225 has the number 1 at location 224.
		*/
		for(count = 0; count<A.length; count++){
			if(A[count]<=225){
				my225[A[count]] = 1;
			}
		}
		
		/*
		The next 3 for loops search through numbers 0 to 225 looking for all combinations 
		of three numbers that add up to 225. For each combination it checks if Array A has 
		those numbers. If so it returns true, otherwise false.
		*/
		for(int a=0; a < 225; a++){
			int b = a+1;
			int c = 225;
			while(b < c){
				if((a + b + c) == 225 && my225[a] == 1 && my225[b] == 1 && my225[c] == 1){
				return true;
				} else if((a + b + c) < 225){ 
					b++;
				} else if((a + b + c) > 225){ 
					c--;
				} else c--;
			}
		}
		
		for(int d = 0; d < 225; d++){
			int e = d + 1;
			while(e <= 113){
				if((d + e + e) == 225 & my225[d] == 1 && my225[e] == 1){ 
				return true; 
				} else e++;
			} 
		}
		
		for(int d = 225; d > 0; d--){
			int e = d - 1;
			while(e >= 0){
				if((d + e + e) == 225 & my225[d] == 1 && my225[e] == 1){ 
				return true; 
				} else e--;
			} 
		}
		
		return false;
		}
		
		/* main()
	   Contains code to test the TripleSum225 function. Nothing in this function 
	   will be marked. You are free to change the provided code to test your 
	   implementation, but only the contents of the TripleSum225() function above 
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

		boolean tripleExists = TripleSum225(array);

		long endTime = System.currentTimeMillis();

		double totalTimeSeconds = (endTime-startTime)/1000.0;

		System.out.printf("Array %s a triple of values which add to 225.\n",tripleExists? "contains":"does not contain");
		System.out.printf("Total Time (seconds): %.2f\n",totalTimeSeconds);
	}
		
	}

	

