import java.util.Set;
import java.util.HashSet;
import java.util.Arrays;

public class CSPZebraDetermine extends CSP {

  static Set<Object> varCol = new HashSet<Object>(
    Arrays.asList(new String[] {"blue", "green", "ivory", "red", "yellow"}));
  static Set<Object> varDri = new HashSet<Object>(
    Arrays.asList(new String[] {"coffee", "orange-juice", "tea", "water"}));
  static Set<Object> varMilk = new HashSet<Object>(
    Arrays.asList(new String[] {"milk"}));
  static Set<Object> varNat = new HashSet<Object>(
    Arrays.asList(new String[] {"englishman", "japanese", "spaniard", "ukrainian"}));
  static Set<Object> varNor = new HashSet<Object>(
    Arrays.asList(new String[] {"norwegian"}));
  static Set<Object> varPet = new HashSet<Object>(
    Arrays.asList(new String[] {"dog", "fox", "horse", "snails", "zebra"}));
  static Set<Object> varCig = new HashSet<Object>(
    Arrays.asList(new String[] {"chesterfield", "kools", "lucky-strike", "old-gold", "parliament"}));

	public boolean isGood(Object X, Object Y, Object x, Object y) {
		//if X is not even mentioned in by the constraints, just return true
		//as nothing can be violated
		if(!C.containsKey(X))
			return true;

    if(X.equals("englishman") && Y.equals("red") && !x.equals(y))
      return false;
    if(X.equals("spaniard") && Y.equals("dog") && !x.equals(y))
      return false;
    if(X.equals("green") && Y.equals("coffee") && !x.equals(y))
      return false;
    if(X.equals("ukrainian") && Y.equals("tea") && !x.equals(y))
      return false;
    if(X.equals("old-gold") && Y.equals("snails") && !x.equals(y))
      return false;
    if(X.equals("kools") && Y.equals("yellow") && !x.equals(y))
      return false;
    if(X.equals("lucky-strike") && Y.equals("orange-juice") && !x.equals(y))
      return false;
    if(X.equals("japanese") && Y.equals("parliament") && !x.equals(y))
      return false;

    if(X.equals("green") && Y.equals("ivory") && (int)x != (int)y+1)
      return false;
    if(X.equals("chesterfield") && Y.equals("fox") && Math.abs((int)x - (int)y) != 1)
      return false;
    if(X.equals("kools") && Y.equals("horse") && Math.abs((int)x - (int)y) != 1)
      return false;
    if(X.equals("norwegian") && Y.equals("blue") && Math.abs((int)x - (int)y) != 1)
      return false;

    if(varCol.contains(X) && varCol.contains(Y) && !X.equals(Y) && x.equals(y))
        return false;
    if(varDri.contains(X) && varDri.contains(Y) && !X.equals(Y) && x.equals(y))
        return false;
    if(varNat.contains(X) && varNat.contains(Y) && !X.equals(Y) && x.equals(y))
        return false;
    if(varPet.contains(X) && varPet.contains(Y) && !X.equals(Y) && x.equals(y))
        return false;
    if(varCig.contains(X) && varCig.contains(Y) && !X.equals(Y) && x.equals(y))
        return false;
    if(varMilk.contains(X) && varMilk.contains(Y) && !X.equals(Y) && x.equals(y))
        return false;
    if(varNor.contains(X) && varNor.contains(Y) && !X.equals(Y) && x.equals(y))
        return false;

		//check to see if there is an arc between X and Y
		//if there isn't an arc, then no constraint, i.e. it is good
		if(!C.get(X).contains(Y))
			return true;

		return true;
	}

	public static void main(String[] args) throws Exception {
		CSPZebraDetermine csp = new CSPZebraDetermine();

    Integer[] dom = {1,2,3,4,5};
    Integer[] norDom = {1};
    Integer[] norDom2 = {2,3,4,5};
    Integer[] milkDom = {3};
    Integer[] milkDom2 = {1,2,4,5};

    for(Object X: varCol)
      csp.addDomain(X, dom);
    for(Object X: varDri)
      csp.addDomain(X, milkDom2);
    for(Object X: varNat)
      csp.addDomain(X, norDom2);
    for(Object X: varPet)
      csp.addDomain(X, dom);
    for(Object X: varCig)
      csp.addDomain(X, dom);
    csp.addDomain(varNor.iterator().next(), norDom);
    csp.addDomain(varMilk.iterator().next(), milkDom);

    String[][] pairs = {{"englishman","red"},
                      {"spaniard","dog"},
                      {"green","coffee"},
                      {"ukrainian","tea"},
                      {"old-gold","snails"},
                      {"kools","yellow"},
                      {"lucky-strike","orange-juice"},
                      {"japanese","parliament"},

                      {"green","ivory"},
                      {"chesterfield","fox"},
                      {"kools","horse"},
                      {"norwegian","blue"}
                    };

    for(Object X: varCol){
      for(Object Y: varCol){
        csp.addBidirectionalArc(X, Y);
      }
    }
    for(Object X: varCig){
      for(Object Y: varCig){
        csp.addBidirectionalArc(X, Y);
      }
    }
    for(Object X: varPet){
      for(Object Y: varPet){
        csp.addBidirectionalArc(X, Y);
      }
    }
    for(Object X: varNat){
      for(Object Y: varNat){
        csp.addBidirectionalArc(X, Y);
      }
    }
    for(Object X: varDri){
      for(Object Y: varDri){
        csp.addBidirectionalArc(X, Y);
      }
    }
    csp.addBidirectionalArc(varMilk.iterator().next(), varMilk.iterator().next());
    csp.addBidirectionalArc(varNor.iterator().next(), varNor.iterator().next());

    for(Object[] p : pairs)
      csp.addBidirectionalArc(p[0], p[1]);

		Search search = new Search(csp);
		System.out.println(search.BacktrackingSearch());
	}
}
