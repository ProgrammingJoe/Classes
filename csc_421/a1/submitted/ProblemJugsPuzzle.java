import java.util.HashSet;
import java.util.Set;

public class ProblemJugsPuzzle extends Problem {

    static final int jug3 = 0;
    static final int jug8 = 1;
    static final int jug12 = 2;

	boolean goal_test(Object state) {
        StateJugsPuzzle can_state = (StateJugsPuzzle) state;

        if (can_state.canArray[jug3]==1 || can_state.canArray[jug8]==1 || can_state.canArray[jug12]==1)
            return true;
        else return false;
	}

  private int intoJug(int more_water, int current_water, int size){
    int space_available = size - current_water;
    if (space_available > more_water){
      return current_water + more_water;
    } else {
      return size;
    }
  }

  private int outOfJug(int more_water, int current_water, int size){
    int space_available = size - current_water;
    if (space_available > more_water){
      return 0;
    } else {
      return more_water - space_available;
    }
  }

  Set<Object> getSuccessors(Object state) {

      Set<Object> set = new HashSet<Object>();
      StateJugsPuzzle can_state = (StateJugsPuzzle) state;

      //Let's create without any constraint, then remove the illegal ones
      StateJugsPuzzle successor_state;

      //jug3 into jug8
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug3] = outOfJug(successor_state.canArray[jug3], successor_state.canArray[jug8], 8);
      successor_state.canArray[jug8] = intoJug(successor_state.canArray[jug3], successor_state.canArray[jug8], 8);
      if (isValid(successor_state)) set.add(successor_state);

      //jug3 into jug12
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug3] = outOfJug(successor_state.canArray[jug3], successor_state.canArray[jug8], 12);
      successor_state.canArray[jug12] = intoJug(successor_state.canArray[jug3], successor_state.canArray[jug12], 12);
      if (isValid(successor_state)) set.add(successor_state);

      //jug3 onto ground
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug3] = 0;
      if (isValid(successor_state)) set.add(successor_state);

      //fill jug3
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug3] = 3;
      if (isValid(successor_state)) set.add(successor_state);

      //jug8 into jug12
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug8] = outOfJug(successor_state.canArray[jug3], successor_state.canArray[jug8], 12);
      successor_state.canArray[jug12] = intoJug(successor_state.canArray[jug8], successor_state.canArray[jug12], 12);
      if (isValid(successor_state)) set.add(successor_state);

      //jug8 into jug3
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug8] = outOfJug(successor_state.canArray[jug3], successor_state.canArray[jug8], 3);
      successor_state.canArray[jug3] = intoJug(successor_state.canArray[jug8], successor_state.canArray[jug3], 3);
      if (isValid(successor_state)) set.add(successor_state);

      //jug8 onto ground
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug8] = 0;
      if (isValid(successor_state)) set.add(successor_state);

      //fill jug8
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug8] = 8;
      if (isValid(successor_state)) set.add(successor_state);

      //jug12 into jug3
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug12] = outOfJug(successor_state.canArray[jug3], successor_state.canArray[jug8], 3);
      successor_state.canArray[jug3] = intoJug(successor_state.canArray[jug12], successor_state.canArray[jug3], 3);
      if (isValid(successor_state)) set.add(successor_state);

      //jug12 into jug8
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug12] = outOfJug(successor_state.canArray[jug3], successor_state.canArray[jug8], 8);
      successor_state.canArray[jug8] = intoJug(successor_state.canArray[jug12], successor_state.canArray[jug8], 8);
      if (isValid(successor_state)) set.add(successor_state);

      //jug12 onto ground
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug12] = 0;
      if (isValid(successor_state)) set.add(successor_state);

      //fill jug12
      successor_state = new StateJugsPuzzle(can_state);
      successor_state.canArray[jug12] = 12;
      if (isValid(successor_state)) set.add(successor_state);

      return set;
  }

  private boolean isValid(StateJugsPuzzle state)
  {
      //Checking to see if any element of the array is negative
      for (int i=0; i<2; i++)
          if (state.canArray[i] < 0) return false;

			if(state.canArray[jug3] > 3 || state.canArray[jug8] > 8 || state.canArray[jug12] > 12){
				return false;
			}

      return true;
  }

	double step_cost(Object fromState, Object toState) { return 1; }

	public double h(Object state) { return 0; }


	public static void main(String[] args) throws Exception {
		ProblemJugsPuzzle problem = new ProblemJugsPuzzle();
		int[] canArray = {0,0,0};
		problem.initialState = new StateJugsPuzzle(canArray);

		Search search  = new Search(problem);

		System.out.println("TreeSearch------------------------");
		System.out.println("BreadthFirstTreeSearch:\t\t" + search.BreadthFirstTreeSearch());
		System.out.println("UniformCostTreeSearch:\t\t" + search.UniformCostTreeSearch());
		System.out.println("DepthFirstTreeSearch:\t\t" + search.DepthFirstTreeSearch());
		System.out.println("GreedyBestFirstTreeSearch:\t" + search.GreedyBestFirstTreeSearch());
		System.out.println("AstarTreeSearch:\t\t" + search.AstarTreeSearch());

		System.out.println("\n\nGraphSearch----------------------");
		System.out.println("BreadthFirstGraphSearch:\t" + search.BreadthFirstGraphSearch());
		System.out.println("UniformCostGraphSearch:\t\t" + search.UniformCostGraphSearch());
		System.out.println("DepthFirstGraphSearch:\t\t" + search.DepthFirstGraphSearch());
		System.out.println("GreedyBestGraphSearch:\t\t" + search.GreedyBestFirstGraphSearch());
		System.out.println("AstarGraphSearch:\t\t" + search.AstarGraphSearch());

		System.out.println("\n\nIterativeDeepening----------------------");
		System.out.println("IterativeDeepeningTreeSearch:\t" + search.IterativeDeepeningTreeSearch());
		System.out.println("IterativeDeepeningGraphSearch:\t" + search.IterativeDeepeningGraphSearch());
	}
}
