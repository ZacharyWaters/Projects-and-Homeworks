import java.util.NoSuchElementException;
import java.util.Iterator;
import java.util.Set;
import java.util.Collection;
/**
 * represents a set of Squares
 *
 * @author zwaters3
 * @version  1.0
 */
public class SquareSet implements Set<Square> {
    private Square[] backEnd;
    private int bSize = 0;

    /**
     * the null constructor for the squareset
     *
     * @version  1.0
     */
    public SquareSet() { }
    /**
     * the parameterized constructor creates a set out the given squares
     *
     * @version  1.0
     * @param  e a collection of squares
     */
    public SquareSet(Collection<Square> e) {
        for (Square alpha: e) {
            this.add(alpha);
        }
    }
    /**
     * adds a square to a set
     *
     * @version  1.0
     * @param  e a square you are trying to add to the set
     * @return true if the square was added false otherwise
     */
    @Override
    public boolean add(Square e) {
        if (e == null) {
            throw new NullPointerException();
        }
        if (e.getRankInt() > 8 || e.getRankInt() < 1
            || e.getFileInt() > 8 || e.getFileInt() < 1) {
            throw new InvalidSquareException("error");
        }
        if (!(this.contains(e))) {
            bSize = bSize + 1;
            Square[] newarray = new Square[bSize];
            if (!(backEnd == null)) {
                for (int a = 0; a < backEnd.length; a++) {
                    newarray[a] = backEnd[a];
                }
                newarray[bSize - 1] = e;
                backEnd = newarray;
                return true;
            } else {
                newarray[0] = e;
                backEnd = newarray;
                return true;
            }
        }
        return false;
    }
    /**
     * checks if a square is contained in the set
     *
     * @version  1.0
     * @param  o is an object you want to see if in a set
     * @return true if contains, false otherwise
     */
    @Override
    public boolean contains(Object o) {
        if (!(o instanceof Square)) {
            return false;
        }
        boolean pairfound = false;
        if (!(backEnd == null)) {
            for (int i = 0; i < backEnd.length; i++) {
                if (o.equals(backEnd[i])) {
                    pairfound = true;
                }
            }
        }
        return pairfound;
    }
    /**
     * checks if a set contains all given squares
     *
     * @version  1.0
     * @param  c is a collection of square you want to see if in the set
     * @return true if all are contained, false otherwise
     */
    @Override
    public boolean containsAll(Collection<?> c) {
        if (!(c == null)) {
            for (Object value: c) {
                if (!(this.contains(value))) {
                    return false;
                }
            }
        }
        return true;
    }
    /**
     * checks if two sets are equivilant
     *
     * @version  1.0
     * @param  o
     * @return boolean true if equal, regardless of order
     * false otherwise
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null) {
            return false;
        }
        if (o instanceof Collection) {
            if (!(((Collection) o).size() == this.bSize)) {
                return false;
            }
        }
        if (o instanceof Collection) {
            if (this.containsAll((Collection) o)) {
                return true;
            }
        }
        return false;
    }
    /**
     * returns a hashvalue for the entire set
     *
     * @version  1.0
     * @return int returns the hashcode value of the square
     */
    @Override
    public int hashCode() {
        int sum = 0;
        if (!(this.backEnd == null)) {
            for (Square num: this.backEnd) {
                sum = sum + num.hashCode();
            }
        }
        return sum;
    }
    /**
     * checks if a set is empty
     *
     * @version  1.0
     * @return boolean true if empty, false otherwise
     */
    @Override
    public boolean isEmpty() {
        if (this.backEnd == null) {
            return true;
        }
        return false;
    }
      /**
     * returns a square at location i in the set
     *
     * @version  1.0
     * @param    i where i is an int
     * @return  Square at location i in the backEnd array
     */
    public Square get(int i) {
        if (i < 0 || i > size()) {
            throw new NoSuchElementException();
        }
        return (this.backEnd[i]);
    }
    /**
     * returns a new iterator
     *
     * @version  1.0
     * @return Iterator returns a new iterator for
     * moving through a square collection.
     */
    @Override
    public Iterator<Square> iterator() {
        return new SquareArrayIterator();
    }
    /**
    * the iterator class
    *
    * @author zwaters3
    * @version  1.0
    */
    private class SquareArrayIterator implements Iterator<Square> {
        private int cursor = 0;
        public boolean hasNext() {
            return cursor < size();
        }


        /**
        * returns the next value in the back end array
        *
        * @version  1.0
        * @return Square the next square int the collection
        */
        public Square next() {
            if (!(hasNext())) {
                throw new NoSuchElementException();
            }
            Square answer = get(cursor);
            cursor++;
            return answer;
        }
    }
    /**
     * returns the size of the set
     *
     * @version  1.0
     * @return int size of the back End Array
     */
    @Override
    public int size() {
        return bSize;
    }
    /**
     * returns the set in the form of an array
     *
     * @version  1.0
     * @return Object[] returns the back end array
     */
    @Override
    public Object[] toArray() {
        Object[] copies = new Object[bSize];
        for (int i = 0; i < backEnd.length; i++) {
            copies[i] = backEnd[i];
        }
        return copies;
    }
    /**
     * fills the set into a preexisting array
     *
     * @version  1.0
     * @param  a
     */
    @Override
    public <T> T[] toArray(T[] a) {
        if (this.size() > a.length) {
            return (T[]) backEnd;
        }
        for (int i = 0; i < this.backEnd.length; i++) {
            a[i] = (T) backEnd[i];
        }
        return (T[]) backEnd;
    }
    /**
     * adds all squares to a set
     *
     * @version  1.0
     * @param  c a collection of squares to add to a set
     * @return boolean true if all added, false otherwise
     */
    @Override
    public boolean addAll(Collection<? extends Square> c) {
        boolean addboolean = false;
        for (Object delta: c) {
            if (delta == null) {
                throw new NullPointerException();
            }
            if (delta instanceof Square) {
                if ((((Square) delta).getRankInt() > 8
                    || ((Square) delta).getRankInt() < 1
                    || ((Square) delta).getFileInt() > 8
                    || ((Square) delta).getFileInt() < 1)) {
                    throw new InvalidSquareException("error");
                }
                if (this.add((Square) delta)) {
                    addboolean = true;
                }

            }
        }
        return addboolean;
    }
    /**
     * removes a given square from a set
     *
     * @version  1.0
     * @param  o a square you are trying to remove from the set
     * @return boolean true if successfully removed, false otherwise
     */
    @Override
    public boolean remove(Object o) {
        if (o == null) {
            throw new NullPointerException();
        }
        if (!(o instanceof Square)) {
            throw new ClassCastException("error");
        }
        if (((Square) o).getRankInt() > 8
            || ((Square) o).getRankInt() < 1
            || ((Square) o).getFileInt() > 8
            || ((Square) o).getFileInt() < 1) {
            throw new ClassCastException("error");
        }
        if (bSize == 0) {
            return false;
        }
        if (!(this.contains(o))) {
            return false;
        }
        if (bSize == 1) {
            backEnd = null;
            return true;
        }
        bSize = bSize - 1;
        Square[] newarray = new Square[bSize];
        int newCounter = 0;
        for (int a = 0; a < backEnd.length; a++) {
            if (!(backEnd[a].equals(o))) {
                newarray[newCounter] = backEnd[a];
                newCounter++;
            }
        }
        backEnd = newarray;
        return true;
    }



    /**
     * @version  1.0
     */
    @Override
    public void clear() {
        throw new UnsupportedOperationException();
    }
    /**
     * @version 1.0
     * @param   c a collection
     */
    @Override
    public boolean removeAll(Collection<?> c) {
        throw new UnsupportedOperationException();
    }
    /**
     * @version  1.0
     * @param c a collection
     */
    @Override
    public boolean retainAll(Collection<?> c) {
        throw new UnsupportedOperationException();
    }
}