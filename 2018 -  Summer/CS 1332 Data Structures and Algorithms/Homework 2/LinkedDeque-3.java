import java.util.NoSuchElementException;

/**
 * Your implementation of a linked deque.
 *
 * @author Zachary Waters
 * @userid zwaters3
 * @GTID 903116871
 * @version 1.0
 */
public class LinkedDeque<T> {
    // Do not add new instance variables.
    private LinkedNode<T> head;
    private LinkedNode<T> tail;
    private int size;

    /**
     * Adds the data to the front of the deque.
     *
     * This method must run in O(1) time.
     *
     * @param data the data to add to the deque
     * @throws java.lang.IllegalArgumentException if data is null
     */
    public void addFirst(T data) {
        if (data == null) {
            throw new IllegalArgumentException("input argument cannot be null");
        }
        LinkedNode<T> newNode = new LinkedNode<>(data);
        size = size + 1;
        if (getHead() == null) {
            head = newNode;
            tail = newNode;
        } else {
            getHead().setPrevious(newNode);
            newNode.setNext(getHead());
            head = newNode;
        }
    }

    /**
     * Adds the data to the back of the deque.
     *
     * This method must run in O(1) time.
     *
     * @param data the data to add to the deque
     * @throws java.lang.IllegalArgumentException if data is null
     */
    public void addLast(T data) {
        if (data == null) {
            throw new IllegalArgumentException("input argument cannot be null");
        }
        LinkedNode<T> newNode = new LinkedNode<>(data);
        size = size + 1;
        if (getHead() == null) {
            head = newNode;
            tail = newNode;
        } else {
            newNode.setPrevious(getTail());
            getTail().setNext(newNode);
            tail = newNode;
        }
    }

    /**
     * Removes the data at the front of the deque.
     *
     * This method must run in O(1) time.
     *
     * @return the data formerly at the front of the deque
     * @throws java.util.NoSuchElementException if the deque is empty
     */
    public T removeFirst() {
        if (getHead() == null) {
            throw new NoSuchElementException("Array is empty");
        }
        size = size - 1;
        T value = getHead().getData();
        if (getHead().getNext() == null) {
            head = null;
            tail = null;
            return value;
        }
        getHead().getNext().setPrevious(null);
        head = (getHead().getNext());
        return value;
    }

    /**
     * Removes the data at the back of the deque.
     *
     * This method must run in O(1) time.
     *
     * @return the data formerly at the back of the deque
     * @throws java.util.NoSuchElementException if the deque is empty
     */
    public T removeLast() {
        if (getTail() == null) {
            throw new NoSuchElementException("Array is empty");
        }
        size = size - 1;
        T returnData = getTail().getData();
        if (getTail().getPrevious() == null) {
            head = null;
            tail = null;
            return returnData;
        }
        tail = getTail().getPrevious();
        tail.setNext(null);
        return returnData;
    }

    /**
     * Returns the number of elements in the list.
     *
     * Runs in O(1) for all cases.
     *
     * DO NOT USE THIS METHOD IN YOUR CODE.
     *
     * @return the size of the list
     */
    public int size() {
        // DO NOT MODIFY!
        return size;
    }

    /**
     * Returns the head node of the linked deque.
     * Normally, you would not do this, but it's necessary for testing purposes.
     *
     * DO NOT USE THIS METHOD IN YOUR CODE.
     *
     * @return node at the head of the linked deque
     */
    public LinkedNode<T> getHead() {
        // DO NOT MODIFY!
        return head;
    }

    /**
     * Returns the tail node of the linked deque.
     * Normally, you would not do this, but it's necessary for testing purposes.
     *
     * DO NOT USE THIS METHOD IN YOUR CODE.
     *
     * @return node at the tail of the linked deque
     */
    public LinkedNode<T> getTail() {
        // DO NOT MODIFY!
        return tail;
    }
}