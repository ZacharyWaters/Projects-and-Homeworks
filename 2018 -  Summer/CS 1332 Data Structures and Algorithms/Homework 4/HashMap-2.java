import java.util.List;
import java.util.Set;
import java.util.HashSet;
import java.util.ArrayList;
import java.util.NoSuchElementException;

/**
 * Your implementation of HashMap.
 *
 * @author Zachary Waters
 * @userid zwaters3
 * @GTID 903116871
 * @version 1.0
 */
public class HashMap<K, V> {

    // DO NOT MODIFY OR ADD NEW GLOBAL/INSTANCE VARIABLES
    public static final int INITIAL_CAPACITY = 13;
    public static final double MAX_LOAD_FACTOR = 0.67;
    private MapEntry<K, V>[] table;
    private int size;

    /**
     * Create a hash map with no entries. The backing array has an initial
     * capacity of {@code INITIAL_CAPACITY}.
     *
     * Do not use magic numbers!
     *
     * Use constructor chaining.
     */
    public HashMap() {
        this(INITIAL_CAPACITY);
    }

    /**
     * Create a hash map with no entries. The backing array has an initial
     * capacity of {@code initialCapacity}.
     *
     * You may assume {@code initialCapacity} will always be positive.
     *
     * @param initialCapacity initial capacity of the backing array
     */
    public HashMap(int initialCapacity) {
        size = 0;
        table = new MapEntry[initialCapacity];
    }

    /**
     * Adds the given key-value pair to the HashMap.
     * If an entry in the HashMap already has this key, replace the entry's
     * value with the new one passed in.
     *
     * In the case of a collision, use linear probing as your resolution
     * strategy.
     *
     * At the start of the method, you should check to see if the array
     * violates the max load factor. For example, let's say the array is of
     * length 5 and the current size is 3 (LF = 0.6). For this example, assume
     * that no elements are removed in between steps. If a non-duplicate key is
     * added, the size will increase to 4 (LF = 0.8), but the resize shouldn't
     * occur until the next put operation.
     *
     * When regrowing, resize the length of the backing table to
     * 2 * old length + 1. You must use the resizeBackingTable method to do so.
     *
     * Return null if the key was not already in the map. If it was in the map,
     * return the old value associated with it.
     *
     * @param key key to add into the HashMap
     * @param value value to add into the HashMap
     * @throws IllegalArgumentException if key or value is null
     * @return null if the key was not already in the map. If it was in the
     * map, return the old value associated with it
     */
    public V put(K key, V value) {
        //CHECKING IF KEY IS NULL FOR EXCEPTION
        if (key == null) {
            throw new IllegalArgumentException("The key cannot be null");
        }
        //CHECKING IF VALUE IS NULL FOR EXCEPTION
        if (value == null) {
            throw new IllegalArgumentException("The value cannot be null");
        }
        //CHECKING IF NEED TO RESIZE
        double currentLoadFactor =  ((double) size / (double) table.length);
        if (currentLoadFactor > MAX_LOAD_FACTOR) {
            resizeBackingTable((2 * table.length) + 1);
        }
        //CREATING THE NEW MAP ENTRY TO BE ADDED
        MapEntry<K, V> newEntry = new MapEntry<>(key, value);
        //GETTING THE HASH VALUE FROM THE KEY
        int hashValue = key.hashCode();
        if (hashValue <= 0) {
            hashValue = hashValue * -1;
            hashValue = hashValue % table.length;
        }
        hashValue = hashValue % table.length;
        //GETTING THE VALUE AT THE HASH CODE'S INDEX
        MapEntry<K, V> oldEntry = table[hashValue];
        //CHECKING IF IMMEDIATE EMPTY SPOT
        if (oldEntry == null) {
            table[hashValue] = newEntry;
            size = size + 1;
            return null;
        }
        //CHECKING IF IMMEDIATE DUPLICATE
        if (oldEntry.getKey().equals(newEntry.getKey())) {
            V oldValue = oldEntry.getValue();
            table[hashValue].setValue(newEntry.getValue());
            return oldValue;
        }
        //PROBE TO NEXT EMPTY SPOT OR DUPLICATE
        boolean flagBrakes = false;
        boolean brakes = false;
        int firstFoundRemoved = -1;
        int fullLoopCounter = 0;
        while (!(brakes)) {
            //PROBE FINDS A NULL SPOT, SO PROBE STOPS
            if (oldEntry == null) {
                brakes = true;
                //PROBE FINDS ITS FIRST REMOVED VALUE REMEMBERS IT.
            } else if (oldEntry.isRemoved() && !flagBrakes) {
                firstFoundRemoved = hashValue;
                flagBrakes = true;
                hashValue = (hashValue + 1) % table.length;
                oldEntry = table[hashValue];
                //PROBE FINDS AN ACTIVE DUPLICATE
            } else if (oldEntry.getKey().equals(newEntry.getKey())
                    && !(oldEntry.isRemoved())) {
                brakes = true;
                //PROBE MAKES A FULL CYCLE AROUND ARRAY.
            }  else if (fullLoopCounter == table.length - 1) {
                brakes = true;
            } else {
                //PROBE ITERATES TO NEXT INDEX
                hashValue = (hashValue + 1) % table.length;
                oldEntry = table[hashValue];
                fullLoopCounter = fullLoopCounter + 1;
            }
        }
        //CHECKING IF POST PROBE EMPTY SPOT and no earlier flag
        if (oldEntry == null && firstFoundRemoved == -1) {
            table[hashValue] = newEntry;
            size = size + 1;
            return null;
        }
        //CHECKING IF POST PROBE EMPTY SPOT and earlier flag
        if (oldEntry == null && firstFoundRemoved != -1) {
            table[firstFoundRemoved] = newEntry;
            size = size + 1;
            return null;
        }
        //CIRCLED WHOLE ARRAY EDGE CASE
        if (fullLoopCounter == table.length - 1
                && firstFoundRemoved != -1) {
            table[firstFoundRemoved] = newEntry;
            size = size + 1;
            return null;
        }
        //CHECKING IF POST PROBE DUPLICATE
        if (oldEntry.getKey().equals(newEntry.getKey())) {
            V oldValue = oldEntry.getValue();
            table[hashValue].setValue(newEntry.getValue());
            return oldValue;
        }
        return null;
    }

    /**
     * Removes the entry with a matching key from the HashMap.
     *
     * @param key the key to remove
     * @throws IllegalArgumentException if key is null
     * @throws java.util.NoSuchElementException if the key does not exist
     * @return the value previously associated with the key
     */
    public V remove(K key) {
        if (key == null) {
            throw new IllegalArgumentException("The key cannot be null");
        }
        //GETTING THE HASH VALUE FROM THE KEY
        int hashValue = key.hashCode();
        if (hashValue <= 0) {
            hashValue = hashValue * -1;
            hashValue = hashValue % table.length;
        }
        hashValue = hashValue % table.length;
        //IMMEDIATE NO FIND
        if (table[hashValue] == null) {
            throw new NoSuchElementException(
                    "The element needs to be in the HashMap to remove it");
        }
        //IMMEDIATE FIND
        if (table[hashValue].getKey().equals(key)
                && !(table[hashValue].isRemoved())) {
            table[hashValue].setRemoved(true);
            size = size - 1;
            return table[hashValue].getValue();
        }
        //PROBE TO NEXT EMPTY SPOT OR VALUE WITHOUT REMOVAL FLAG
        boolean brakes = false;
        while (!(brakes)) {
            if (table[hashValue] == null) {
                brakes = true;
            } else if (table[hashValue].getKey().equals(key)
                    && !(table[hashValue].isRemoved())) {
                brakes = true;
            } else {
                hashValue = (hashValue + 1) % table.length;
            }
        }
        //POST PROBE NO FIND
        if (table[hashValue] == null) {
            throw new NoSuchElementException(
                    "The element needs to be in the HashMap to remove it");
        }
        //POST PROBE FIND
        if (table[hashValue].getKey().equals(key)
                && !(table[hashValue].isRemoved())) {
            table[hashValue].setRemoved(true);
            size = size - 1;
            return table[hashValue].getValue();
        }
        return null;
    }

    /**
     * Gets the value associated with the given key.
     *
     * @param key the key to search for
     * @throws IllegalArgumentException if key is null
     * @throws java.util.NoSuchElementException if the key is not in the map
     * @return the value associated with the given key
     */
    public V get(K key) {
        if (key == null) {
            throw new IllegalArgumentException("The key cannot be null");
        }
        //GETTING THE HASH VALUE FROM THE KEY
        int hashValue = key.hashCode();
        if (hashValue <= 0) {
            hashValue = hashValue * -1;
            hashValue = hashValue % table.length;
        }
        hashValue = hashValue % table.length;
        //IMMEDIATE NO FIND
        if (table[hashValue] == null) {
            throw new NoSuchElementException(
                    "The element needs to be in the HashMap to get it");
        }
        //IMMEDIATE FIND
        if (table[hashValue].getKey().equals(key)
                && !(table[hashValue].isRemoved())) {
            return table[hashValue].getValue();
        }
        //PROBE TO NEXT EMPTY SPOT OR DUPLICATE
        boolean brakes = false;
        while (!(brakes)) {
            if (table[hashValue] == null) {
                brakes = true;
            } else if ((table[hashValue].getKey().equals(key))
                    && !(table[hashValue].isRemoved())) {
                brakes = true;
            } else {
                hashValue = (hashValue + 1) % table.length;
            }
        }
        //POST PROBE NO FIND
        if (table[hashValue] == null) {
            throw new NoSuchElementException(
                    "The element needs to be in the HashMap to get it");
        }
        //POST PROBE FIND
        if (table[hashValue].getKey().equals(key)) {
            return table[hashValue].getValue();
        }
        //Should not get here
        return null;
    }

    /**
     * Returns whether or not the key is in the map.
     *
     * @param key the key to search for
     * @throws IllegalArgumentException if key is null
     * @return whether or not the key is in the map
     */
    public boolean containsKey(K key) {
        if (key == null) {
            throw new IllegalArgumentException("The key cannot be null");
        }
        //GETTING THE HASH VALUE FROM THE KEY
        int hashValue = key.hashCode();
        if (hashValue <= 0) {
            hashValue = hashValue * -1;
            hashValue = hashValue % table.length;
        }
        hashValue = hashValue % table.length;
        //IMMEDIATE NO FIND
        if (table[hashValue] == null) {
            return false;
        }
        //IMMEDIATE FIND
        if (table[hashValue].getKey().equals(key)
                && !(table[hashValue].isRemoved())) {
            return true;
        }
        //PROBE TO NEXT EMPTY SPOT OR DUPLICATE
        boolean brakes = false;
        while (!(brakes)) {
            if (table[hashValue] == null) {
                brakes = true;
            } else if (table[hashValue].getKey() == (key)) {
                brakes = true;
            } else {
                hashValue = (hashValue + 1) % table.length;
            }
        }
        //POST PROBE NO FIND
        if (table[hashValue] == null) {
            return false;
        }
        //POST PROBE FIND
        if (table[hashValue].getKey().equals(key)
                && !(table[hashValue].isRemoved())) {
            return true;
        }
        return false;
    }

    /**
     * Returns a Set view of the keys contained in this map.
     * Use {@code java.util.HashSet}.
     *
     * @return set of keys in this map
     */
    public Set<K> keySet() {
        Set<K> keySet = new HashSet<>();
        for (int i = 0; i < table.length; i++) {
            if (table[i] != null) {
                if (!(table[i].isRemoved())) {
                    K keyValue = table[i].getKey();
                    keySet.add(keyValue);
                }
            }
        }
        return keySet;
    }

    /**
     * Returns a List view of the values contained in this map.
     *
     * Use {@code java.util.ArrayList} or {@code java.util.LinkedList}.
     *
     * You should iterate over the table in order of increasing index and add 
     * entries to the List in the order in which they are traversed.
     *
     * @return list of values in this map
     */
    public List<V> values() {
        ArrayList<V> valueList = new ArrayList<>();
        for (int i = 0; i < table.length; i++) {
            if (table[i] != null) {
                if (!(table[i].isRemoved())) {
                    V keyValue = table[i].getValue();
                    valueList.add(keyValue);
                }
            }
        }
        return valueList;
    }

    /**
     * Resize the backing table to {@code length}.
     *
     * Disregard the load factor for this method. So, if the passed in length is
     * smaller than the current capacity, and this new length causes the table's
     * load factor to exceed MAX_LOAD_FACTOR, you should still resize the table
     * to the specified length and leave it at that capacity.
     *
     * You should iterate over the old table in order of increasing index and
     * add entries to the new table in the order in which they are traversed.
     *
     * Remember that you cannot just simply copy the entries over to the new
     * array.
     *
     * Also, since resizing the backing table is working with the non-duplicate
     * data already in the table, you shouldn't need to check for duplicates.
     *
     * @param length new length of the backing table
     * @throws IllegalArgumentException if length is non-positive or less than
     * the number of items in the hash map.
     */
    public void resizeBackingTable(int length) {
        if (length < 0) {
            throw new IllegalArgumentException("length can't be negative");
        }
        if (length < size) {
            throw new IllegalArgumentException(
                    "length can't be less than size");
        }
        MapEntry[] newTable = new MapEntry[length];
        MapEntry[] oldTable = table;
        table = newTable;
        for (int i = 0; i < oldTable.length; i++) {
            if (oldTable[i] != null) {
                if (!(oldTable[i].isRemoved())) {
                    //GETTING THE HASH VALUE FROM THE KEY
                    int hashValue = oldTable[i].getKey().hashCode();
                    if (hashValue <= 0) {
                        hashValue = hashValue * -1;
                        hashValue = hashValue % table.length;
                    }
                    hashValue = hashValue % table.length;
                    //CHECKING IF IMMEDIATE EMPTY SPOT
                    if (newTable[hashValue] == null) {
                        table[hashValue] = oldTable[i];
                    } else {
                        //PROBE TO NEXT EMPTY SPOT
                        while (newTable[hashValue] != null) {
                            hashValue = (hashValue + 1) % newTable.length;
                        }
                        //PUTTING VALUE IN POST_PROBE EMPTY SPOT
                        table[hashValue] = oldTable[i];
                    }
                }
            }
        }
    }

    /**
     * Clears the table and resets it to the default length.
     */
    public void clear() {
        table = new MapEntry[INITIAL_CAPACITY];
        size = 0;
    }

    /**
     * Returns the number of elements in the map.
     *
     * DO NOT USE OR MODIFY THIS METHOD!
     *
     * @return number of elements in the HashMap
     */
    public int size() {
        // DO NOT MODIFY THIS METHOD!
        return size;
    }

    /**
     * DO NOT USE THIS METHOD IN YOUR CODE. IT IS FOR TESTING ONLY.
     *
     * @return the backing array of the data structure, not a copy.
     */
    public MapEntry<K, V>[] getTable() {
        // DO NOT MODIFY THIS METHOD!
        return table;
    }

}