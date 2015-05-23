/* ShortestPath.java
   CSC 226 - Fall 2014
   Assignment 3 - Template for Dijkstra's Algorithm
   
   This template includes some testing code to help verify the implementation.
   To interactively provide test inputs, run the program with
	java ShortestPath
	
   To conveniently test the algorithm with a large input, create a text file
   containing one or more test graphs (in the format described below) and run
   the program with
	java ShortestPath file.txt
   where file.txt is replaced by the name of the text file.
   
   The input consists of a series of graphs in the following format:
   
    <number of vertices>
	<adjacency matrix row 1>
	...
	<adjacency matrix row n>
	
   Entry A[i][j] of the adjacency matrix gives the weight of the edge from 
   vertex i to vertex j (if A[i][j] is 0, then the edge does not exist).
   Note that since the graph is undirected, it is assumed that A[i][j]
   is always equal to A[j][i].
	
   An input file can contain an unlimited number of graphs; each will be 
   processed separately.


   B. Bird - 08/02/2014
*/

import java.util.Arrays;
import java.util.Scanner;
import java.util.Vector;
import java.io.File;


class heap{
		int weight = 0;
		int parent1 = 0;
		int parent2 = 0;

	public heap(){
	}	
					
	public heap(int weight, int parent1, int parent2){
		this.weight = weight;
		this.parent1 = parent1;
		this.parent2 = parent2;
	}
	
	public int edgeweight(heap edge){
		return edge.weight;
	}
	
	public int edgep1(heap edge){
		return edge.parent1;
	}
	
	public int edgep2(heap edge){
		return edge.parent2;
	}


	public heap[] removemin(int size, heap[] myheap){
		myheap[1] = myheap[size];
		myheap[size] = null;
		
		if(myheap[1] == null){
			return myheap;
		}
		
		boolean sorted = false;
		heap temp = new heap();
		int location = 1;
		while(sorted == false){
			if(myheap[location*2] == null && myheap[location*2 +1] == null){
				sorted = true;
			} else if(myheap[location*2] == null && myheap[location*2 +1] != null){
				if(myheap[location].weight > myheap[location*2+1].weight){
					temp = myheap[location];
					myheap[location] = myheap[(location*2 + 1)];
					myheap[(location*2 + 1)] = temp;
					location = location*2+1;
				} else {
					sorted = true;
				}
			} else if(myheap[location*2] != null && myheap[location*2 +1] == null){
				if(myheap[location].weight > myheap[location*2].weight){
					temp = myheap[location];
					myheap[location] = myheap[location*2];
					myheap[location*2] = temp;
					location = location*2;
				} else {
					sorted = true;
				}
			} else if(myheap[location*2] != null && myheap[location*2 +1] != null){
				if(myheap[location].weight > myheap[location*2].weight || myheap[location].weight > myheap[location*2 + 1].weight){
					if(myheap[location*2].weight < myheap[(location*2 +1)].weight){
						temp = myheap[location];
						myheap[location] = myheap[location*2];
						myheap[location*2] = temp;
						location = location*2;
					} else if(myheap[location*2].weight > myheap[(location*2 +1)].weight){
						temp = myheap[location];
						myheap[location] = myheap[(location*2 + 1)];
						myheap[(location*2 + 1)] = temp;
						location = location*2;
					}
				} else {
					sorted = true;
				}
			}
			
		} //while
		return myheap;
	} //remove


	public heap[] insert(heap element, heap[] myheap, int location){
		myheap[location] = element;
		boolean sorted = false;
		heap temp = new heap();
	
		while(sorted == false){
			if(myheap[location/2] != null){
				if(myheap[location].weight < myheap[location/2].weight){
					temp = myheap[location];
					myheap[location] = myheap[location/2];
					myheap[location/2] = temp;
					location = location/2;
				} else if(myheap[location].weight >= myheap[location/2].weight){
					sorted = true;
				}
			}
			sorted = true;
		}
		return myheap;
	} //insert
	
	public boolean isempty(heap[] myheap){
		boolean empty = true;
		for(int k = 0; k< myheap.length; k++){
			if(myheap[k] != null){
				empty = false;
			}
		}
		return empty;
	} //isempty

} //heapclass

//Do not change the name of the ShortestPath class
public class ShortestPath{

	/* ShortestPath(G)
		Given an adjacency matrix for graph G, return the total weight
		of a minimum weight path from vertex 0 to vertex 1.
		
		If G[i][j] == 0, there is no edge between vertex i and vertex j
		If G[i][j] > 0, there is an edge between vertices i and j, and the
		value of G[i][j] gives the weight of the edge.
		No entries of G will be negative.
	*/
	
	
	static int ShortestPath(int[][] G){
		int numVerts = G.length;
		int totalWeight = 999999999;
		int k = 0;  
		heap[] myheap = new heap[numVerts*numVerts];
		int size = 1;
		heap tool = new heap();
		int[] relax = new int[numVerts];
		int[] parentedge = new int[numVerts];
		relax[0] = 1;
		
		myheap[0] = null;
		
		for(k = 0; k < numVerts; k++){
			if(G[0][k] > 0){
				heap newitem = new heap(G[0][k], 0, k);
				myheap = tool.insert(newitem, myheap, size);
				size++;
			}
		}
		
		outerloop:
		while(tool.isempty(myheap) == false){
			heap min = myheap[1];
			myheap = tool.removemin(size-1, myheap);
			
			if(myheap[1] == null){
				break outerloop;
			}
			size--;
			relax[min.parent2] = 1;
			
			if(min.parent2 == 1){
				if(min.weight < totalWeight){
					totalWeight = min.weight;
				}
			}
			
			for(k = 0; k < numVerts; k++){
				if(G[min.parent2][k] > 0){
					if(relax[k] == 0){
						heap newitem = new heap((G[min.parent2][k] + min.weight), min.parent2, k);
						myheap = tool.insert(newitem, myheap, size);
						size++;
					}
				}
			}
			
		}
		
		
		return totalWeight;
	
	}

	
		
	/* main()
	   Contains code to test the ShortestPath function. You may modify the
	   testing code if needed, but nothing in this function will be considered
	   during marking, and the testing process used for marking will not
	   execute any of the code below.
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
			System.out.printf("Reading input values from stdin.\n");
		}
		
		int graphNum = 0;
		double totalTimeSeconds = 0;
		
		//Read graphs until EOF is encountered (or an error occurs)
		while(true){
			graphNum++;
			if(graphNum != 1 && !s.hasNextInt())
				break;
			System.out.printf("Reading graph %d\n",graphNum);
			int n = s.nextInt();
			int[][] G = new int[n][n];
			int valuesRead = 0;
			for (int i = 0; i < n && s.hasNextInt(); i++){
				for (int j = 0; j < n && s.hasNextInt(); j++){
					G[i][j] = s.nextInt();
					valuesRead++;
				}
			}
			if (valuesRead < n*n){
				System.out.printf("Adjacency matrix for graph %d contains too few values.\n",graphNum);
				break;
			}
			long startTime = System.currentTimeMillis();
			
			int totalWeight = ShortestPath(G);
			long endTime = System.currentTimeMillis();
			totalTimeSeconds += (endTime-startTime)/1000.0;
			
			System.out.printf("Graph %d: Minimum weight of a 0-1 path is %d\n",graphNum,totalWeight);
		}
		graphNum--;
		System.out.printf("Processed %d graph%s.\nAverage Time (seconds): %.2f\n",graphNum,(graphNum != 1)?"s":"",(graphNum>0)?totalTimeSeconds/graphNum:0);
	}
}