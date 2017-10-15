public class StateNim extends State {

    public int coins = 13;

    public StateNim() {
      coins = 13;
      player = 1;
    }

    public StateNim(StateNim state) {
      this.coins = state.coins;
      player = state.player;
    }

    public String toString() {
      return Integer.toString(coins);
    }
}
