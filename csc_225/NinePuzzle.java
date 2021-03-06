/* NinePuzzle.java
   CSC 225 - Fall 2014
   Assignment 5 - Template for the 9-puzzle

   This template includes some testing code to help verify the implementation.
   Input boards can be provided with standard input or read from a file.

   To provide test inputs with standard input, run the program with
	java NinePuzzle
   To terminate the input, use Ctrl-D (which signals EOF).

   To read test inputs from a file (e.g. boards.txt), run the program with
    java NinePuzzle boards.txt

   The input format for both input methods is the same. Input consists
   of a series of 9-puzzle boards, with the '0' character representing the
   empty square. For example, a sample board with the middle square empty is

    1 2 3
    4 0 5
    6 7 8

   And a solved board is

    1 2 3
    4 5 6
    7 8 0

   An input file can contain an unlimited number of boards; each will be
   processed separately.

*/

import java.util.Scanner;
import java.io.File;

public class NinePuzzle{

	//The total number of possible boards is 9! = 1*2*3*4*5*6*7*8*9 = 362880
	public static final int NUM_BOARDS = 362880;


	/*  SolveNinePuzzle(B)
		Given a valid 9-puzzle board (with the empty space represented by the
		value 0),return true if the board is solvable and false otherwise.
		If the board is solvable, a sequence of moves which solves the board
		will be printed, using the printBoard function below.
	*/
	
	public static boolean isempty(int arr[]){
	int k = 0;
		for(k = 0; k < NUM_BOARDS; k++){
			if(arr[k] != -1){
				return false;
			} else {
				continue;
			}
		}
		return true;
	}
	
	public static boolean SolveNinePuzzle(int[][] B){

		int[][] adjacencyList=BuildNinePuzzleGraph();
		int[] discovered = new int[NUM_BOARDS];
		int[] myqueue = new int[NUM_BOARDS];
		int[] predge = new int[NUM_BOARDS];
		int[][] board = new int[3][3];
		
		int end = getIndexFromBoard(B);
		int qfront = 0;
		int qrear = 0;
		int vertex = 0;
		int found = 0;
		int n = 0;
		
		System.out.println("The testing board is: ");
		printBoard(B);
		
		if(end == 0){
			System.out.println("The starting board is the final board");
			printBoard(B);
			return true;
		} //for
		
		for(n = 0; n < NUM_BOARDS; n++){
			myqueue[n] = -1;
		} //for
		
		discovered[0] = 1;
		myqueue[qrear] = 0;
		
		while(isempty(myqueue) == false){
			vertex = myqueue[qfront];
			if(vertex == end){
				found = vertex;
				break;
			} //if
			myqueue[qfront] = -1;
			qfront++;
			int k = 0;
			while(k < 4 && adjacencyList[vertex][k] != -1){
				int nextvertex = adjacencyList[vertex][k];
				if(discovered[nextvertex] != 1){
					discovered[nextvertex] = 1;
					predge[nextvertex] = vertex;
					myqueue[++qrear] = nextvertex;
				} //if
			k++;
			} //while
		} //while
		
		if(found != 0){
			while(found != 0){
				board = getBoardFromIndex(found);
				found = predge[found];
				printBoard(board);
			}
			printBoard(getBoardFromIndex(0));
			return true;
		}
	return false;
	} //solver puzzle
	
	public static int[][] BuildNinePuzzleGraph(){
		// adjacency list
		int[][] adjacencyList=new int[NUM_BOARDS][4];
		
		// initialize the adjacency list with negative values
		for (int i=0;i<adjacencyList.length;i++){
			for (int j=0;j<adjacencyList[0].length;j++){
				adjacencyList[i][j]=-1;
			}
		}
		
		
		int k, i, j, index2, col;
		int row = 0;
		int column = 0;
		int board[][] = new int[3][3];
		int up[][] = new int[3][3];
		int down[][] = new int[3][3];
		int left[][] = new int[3][3];
		int right[][] = new int[3][3];
		
		for(k = 0; k < NUM_BOARDS; k++){
			col = 0;
			board = getBoardFromIndex(k);
			up = getBoardFromIndex(k);
			down = getBoardFromIndex(k);
			left = getBoardFromIndex(k);
			right = getBoardFromIndex(k);
			//find where the 0 is in this board
			for(i = 0; i <= 2; i++){
				for(j = 0; j <= 2; j++){
					if(board[i][j] == 0){
						column = i;
						row = j;
					} //if
				} //for
			} //for
			
			if (column > 0){
				left[column][row] = left[column-1][row];
				left[column-1][row] = 0;
				index2 = getIndexFromBoard(left);
				adjacencyList[k][col++] = index2;
			} //if
			if (column < 2){
				right[column][row] = right[column+1][row];
				right[column+1][row] = 0;
				index2 = getIndexFromBoard(right);
				adjacencyList[k][col++] = index2;
			} //if
			if (row > 0){
				up[column][row] = up[column][row-1];
				up[column][row-1] = 0;
				index2 = getIndexFromBoard(up);
				adjacencyList[k][col++] = index2;
			} //if
			if (row < 2){
				down[column][row] = down[column][row+1];
				down[column][row+1] = 0;
				index2 = getIndexFromBoard(down);
				adjacencyList[k][col++] = index2;
			} //if
		} //mainfor
	return adjacencyList;
	} //function
		
		

	/*  printBoard(B)
		Print the given 9-puzzle board. The SolveNinePuzzle method above should
		use this method when printing the sequence of moves which solves the input
		board. If any other method is used (e.g. printing the board manually), the
		submission may lose marks.
	*/
	public static void printBoard(int[][] B){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++)
				System.out.printf("%d ",B[i][j]);
			System.out.println();
		}
		System.out.println();
	}


	/* Board/Index conversion functions
	   These should be treated as black boxes (i.e. don't modify them, don't worry about
	   understanding them). The conversion scheme used here is adapted from
		 W. Myrvold and F. Ruskey, Ranking and Unranking Permutations in Linear Time,
		 Information Processing Letters, 79 (2001) 281-284.
	*/
	public static int getIndexFromBoard(int[][] B){
		int i,j,tmp,s,n;
		int[] P = new int[9];
		int[] PI = new int[9];
		for (i = 0; i < 9; i++){
			P[i] = B[i/3][i%3];
			PI[P[i]] = i;
		}
		int id = 0;
		int multiplier = 1;
		for(n = 9; n > 1; n--){
			s = P[n-1];
			P[n-1] = P[PI[n-1]];
			P[PI[n-1]] = s;

			tmp = PI[s];
			PI[s] = PI[n-1];
			PI[n-1] = tmp;
			id += multiplier*s;
			multiplier *= n;
		}
		return id;
	}

	public static int[][] getBoardFromIndex(int id){
		int[] P = new int[9];
		int i,n,tmp;
		for (i = 0; i < 9; i++)
			P[i] = i;
		for (n = 9; n > 0; n--){
			tmp = P[n-1];
			P[n-1] = P[id%n];
			P[id%n] = tmp;
			id /= n;
		}
		int[][] B = new int[3][3];
		for(i = 0; i < 9; i++)
			B[i/3][i%3] = P[i];
		return B;
	}


	public static void main(String[] args){
		/* Code to test your implementation */
		/* You may modify this, but nothing in this function will be marked */


		Scanner s;

		if (args.length > 0){
			//If a file argument was provided on the command line, read from the file
			try{
				s = new Scanner(new File(args[0]));
			} catch(java.io.FileNotFoundException e){
				System.out.printf("Unable to open %s\n",args[0]);
				return;
			}
			System.out.printf("Reading input values from %s.\n",args[0]);
		}else{
			//Otherwise, read from standard input
			s = new Scanner(System.in);
			System.out.printf("Reading input values from stdin.\n");
		}

		int graphNum = 0;
		double totalTimeSeconds = 0;

		//Read boards until EOF is encountered (or an error occurs)
		while(true){
			graphNum++;
			if(graphNum != 1 && !s.hasNextInt())
				break;
			System.out.printf("Reading board %d\n",graphNum);
			int[][] B = new int[3][3];
			int valuesRead = 0;
			for (int i = 0; i < 3 && s.hasNextInt(); i++){
				for (int j = 0; j < 3 && s.hasNextInt(); j++){
					B[i][j] = s.nextInt();
					valuesRead++;
				}
			}
			if (valuesRead < 9){
				System.out.printf("Board %d contains too few values.\n",graphNum);
				break;
			}
			System.out.printf("Attempting to solve board %d...\n",graphNum);
			long startTime = System.currentTimeMillis();
			boolean isSolvable = SolveNinePuzzle(B);
			long endTime = System.currentTimeMillis();
			totalTimeSeconds += (endTime-startTime)/1000.0;

			if (isSolvable)
				System.out.printf("Board %d: Solvable.\n",graphNum);
			else
				System.out.printf("Board %d: Not solvable.\n",graphNum);
		}
		graphNum--;
		System.out.printf("Processed %d board%s.\n Average Time (seconds): %.2f\n",graphNum,(graphNum != 1)?"s":"",(graphNum>1)?totalTimeSeconds/graphNum:0);

	}

}

