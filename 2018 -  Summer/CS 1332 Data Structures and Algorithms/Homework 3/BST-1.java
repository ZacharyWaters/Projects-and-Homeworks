import java.util.Collection;
import java.util.List;
import java.util.LinkedList;
import java.util.ArrayList;
import java.util.Queue;

/**
 * Your implementation of a binary search tree.
 *
 * @author Zachary Waters
 * @userid zwaters3
 * @GTID 903116871
 * @version 1.0
 */
public class BST<T extends Comparable<? super T>> {
    // DO NOT ADD OR MODIFY INSTANCE VARIABLES.
    private BSTNode<T> root;
    private int size;

    /**
     * A no-argument constructor that should initialize an empty BST.
     *
     * Since instance variables are initialized to their default values, there
     * is no need to do anything for this constructor.
     */
    public BST() {
        // DO NOT IMPLEMENT THIS CONSTRUCTOR!
    }

    /**
     * Initializes the BST with the data in the Collection. The data
     * should be added in the same order it is in the Collection.
     *
     * Hint: Not all Collections are indexable like Lists, so a regular
     * for loop will not work here. What other type of loop would work?
     *
     * @param data the data to add to the tree
     * @throws IllegalArgumentException if data or any element in data is null
     */
    public BST(Collection<T> data) {
        if (data == null) {
            throw new IllegalArgumentException(
                    "the collection parameter cannot be null");
        }
        size = 0;

        for (Object value: data) {
            if (value == null) {
                throw new IllegalArgumentException(
                        "no data inside the collection can be null");
            }
            add((T) value);
        }
    }

    /**
     * Add the data as a leaf in the BST. Should traverse the tree to find the
     * appropriate location. If the data is already in the tree, then nothing
     * should be done (the duplicate shouldn't get added, and size should not be
     * incremented).
     *
     * Should have a running time of O(log n) for a balanced tree, and a worst
     * case of O(n).
     *
     * @throws IllegalArgumentException if the data is null
     * @param data the data to be added
     */
    public void add(T data) {
        if (data == null) {
            throw new IllegalArgumentException(
                    "cannot add null data to the Binary Tree");
        }
        BSTNode<T> newNode = new BSTNode<>(data);
        if (root == null) {
            root = newNode;
            size = 1;
        } else {
            root = addHelper(newNode, root);
        }
    }

    /**
     * Helper method that recursively adds a node to the BST
     *
     * @param toAdd is the node containing the data to be added
     * @param currentNode is a pointer to the current node allowing recursion
     * @return the node you are currently at recreating the  new tree
     */
    private  BSTNode<T> addHelper(BSTNode<T> toAdd, BSTNode<T> currentNode) {
        if (currentNode == null) {
            size = size + 1;
            return toAdd;
        }
        if (toAdd.getData().compareTo(currentNode.getData()) > 0) {
            currentNode.setRight(addHelper(toAdd, currentNode.getRight()));
        }
        if (toAdd.getData().compareTo(currentNode.getData()) < 0) {
            currentNode.setLeft(addHelper(toAdd, currentNode.getLeft()));
        }
        return currentNode;
    }

    /**
     * Removes the data from the tree. There are 3 cases to consider:
     *
     * 1: the data is a leaf. In this case, simply remove it.
     * 2: the data has one child. In this case, simply replace it with its
     * child.
     * 3: the data has 2 children. Use the successor to replace the data.
     * You must use recursion to find and remove the successor (you will likely
     * need an additional helper method to handle this case efficiently).
     *
     * Should have a running time of O(log n) for a balanced tree, and a worst
     * case of O(n).
     *
     * @throws IllegalArgumentException if the data is null
     * @throws java.util.NoSuchElementException if the data is not found
     * @param data the data to remove from the tree.
     * @return the data removed from the tree. Do not return the same data
     * that was passed in.  Return the data that was stored in the tree.
     */
    public T remove(T data) {
        if (data == null) {
            throw new IllegalArgumentException(
                    "tree can't contain null values, so you can't remove null");
        }
        BSTNode<T> dummyNode = new BSTNode<>(null);
        root = removeHelper(root, dummyNode, data);
        return dummyNode.getData();
    }

    /**
     * Helper method that recursively removes a node from the BST
     *
     * @param currentNode is a pointer to the current node allowing recursion
     * @param dummyNode is a node that will contain the value removed
     * @param data is the data of the tree you want to remove
     * @return the node you are currently at recreating the  new tree
     */
    private BSTNode<T> removeHelper(BSTNode<T> currentNode,
                                       BSTNode<T> dummyNode, T data) {
        if (currentNode == null) {
            throw new java.util.NoSuchElementException("no element found");
        }
        if (currentNode.getData().compareTo(data) == 0) {
            //No Children
            if (currentNode.getLeft() == null
                    && currentNode.getRight() == null) {
                size = size - 1;
                dummyNode.setData(currentNode.getData());
                return null;
            }
            //One Child left
            if (currentNode.getLeft() != null
                    && currentNode.getRight() == null) {
                size = size - 1;
                dummyNode.setData(currentNode.getData());
                return currentNode.getLeft();
            }
            //One Child Right
            if (currentNode.getLeft() == null
                    && currentNode.getRight() != null) {
                size = size - 1;
                dummyNode.setData(currentNode.getData());
                return currentNode.getRight();
            }
            //Two Children
            if (currentNode.getLeft() != null
                    && currentNode.getRight() != null) {
                size = size - 1;
                dummyNode.setData(currentNode.getData());
                BSTNode<T> tempNode = new BSTNode<>(data);
                currentNode.setRight(successorHelper(currentNode.getRight(),
                        tempNode));
                currentNode.setData(tempNode.getData());
                return currentNode;
            }
        }
        if (data.compareTo(currentNode.getData()) > 0) {
            currentNode.setRight(removeHelper(currentNode.getRight(),
                    dummyNode, data));
        }
        if (data.compareTo(currentNode.getData()) < 0) {
            currentNode.setLeft(removeHelper(currentNode.getLeft(),
                    dummyNode, data));
        }
        return currentNode;
    }

    /**
     * Helper method that recursively removes a node from the BST
     *
     * @param currentNode is a pointer to the current node allowing recursion
     * @param tempNode is a node that will contain the value removed
     * @return the node you are currently at recreating the new tree
     * */
    private BSTNode<T> successorHelper(BSTNode<T> currentNode,
                                          BSTNode<T> tempNode) {
        if (currentNode.getLeft() == null) {
            tempNode.setData(currentNode.getData());
            return currentNode.getRight();
        } else {
            currentNode.setLeft(successorHelper(currentNode.getLeft(),
                    tempNode));
        }
        return currentNode;
    }

    /**
     * Returns the data in the tree matching the parameter passed in (think
     * carefully: should you use value equality or reference equality?).
     *
     * Should have a running time of O(log n) for a balanced tree, and a worst
     * case of O(n).
     *
     * @throws IllegalArgumentException if the data is null
     * @throws java.util.NoSuchElementException if the data is not found
     * @param data the data to search for in the tree.
     * @return the data in the tree equal to the parameter. Do not return the
     * same data that was passed in.  Return the data that was stored in the
     * tree.
     */
    public T get(T data) {
        if (data == null) {
            throw new IllegalArgumentException(
                    "tree cannot contain a null value, nor can you get one");
        }
        T dataFound = getHelper(data, root);
        if (dataFound == null) {
            throw new java.util.NoSuchElementException(
                    "no such element found in the array");
        }
        return dataFound;
    }

    /**
     * Helper method for the get method
     * @param currentNode is a pointer to the current node allowing recursion
     * @param data the data to search for in the tree.
     * @return the data inside the node, or null if not found
     */
    private T getHelper(T data, BSTNode<T> currentNode) {
        T datafound;
        if (currentNode == null) {
            return null;
        }
        if (currentNode.getData().equals(data)) {
            return currentNode.getData();
        } else if (data.compareTo(currentNode.getData()) > 0) {
            datafound = getHelper(data, currentNode.getRight());
        } else {
            datafound = getHelper(data, currentNode.getLeft());
        }
        return datafound;
    }

    /**
     * Returns whether or not data equivalent to the given parameter is
     * contained within the tree. The same type of equality should be used as
     * in the get method.
     *
     * Should have a running time of O(log n) for a balanced tree, and a worst
     * case of O(n).
     *
     * @throws IllegalArgumentException if the data is null
     * @param data the data to search for in the tree.
     * @return whether or not the parameter is contained within the tree.
     */
    public boolean contains(T data) {
        if (data == null) {
            throw new IllegalArgumentException(
                    "tree cannot contain a null value, nor can search for one");
        }
        return containsHelper(data, root);
    }

    /**
     * helper method for the contains method
     * @param currentNode is a pointer to the current node allowing recursion
     * @param data the data to search for in the tree.
     * @return whether or not the parameter is contained within the tree.
     */
    private boolean containsHelper(T data, BSTNode<T> currentNode) {
        boolean isfound;
        if (currentNode == null) {
            return false;
        }
        if (currentNode.getData().compareTo(data) == 0) {
            return true;
        } else if (data.compareTo(currentNode.getData()) > 0) {
            isfound = containsHelper(data, currentNode.getRight());
        } else {
            isfound = containsHelper(data, currentNode.getLeft());
        }
        return isfound;
    }

    /**
     * Should run in O(n).
     *
     * @return a preorder traversal of the tree
     */
    public List<T> preorder() {
        ArrayList<T> preorderList =  new ArrayList<>();
        preOrderHelper(root, preorderList);
        return preorderList;
    }

    /**
     * helper method for the preorder method
     * @param currentNode is a pointer to the current node allowing recursion
     * @param currentList is the list of the data in the specified order
     */
    private void preOrderHelper(BSTNode<T> currentNode, List<T> currentList) {
        if (currentNode != null) {
            currentList.add(currentNode.getData());
            preOrderHelper(currentNode.getLeft(), currentList);
            preOrderHelper(currentNode.getRight(), currentList);
        }
    }

    /**
     * Should run in O(n).
     *
     * @return an inorder traversal of the tree
     */
    public List<T> inorder() {
        ArrayList<T> inorderList =  new ArrayList<>();
        inOrderHelper(root, inorderList);
        return inorderList;
    }

    /**
     * helper method for the inorder method
     * @param currentNode is a pointer to the current node allowing recursion
     * @param currentList is the list of the data in the specified order
     */
    private void inOrderHelper(BSTNode<T> currentNode, List<T> currentList) {
        if (currentNode != null) {
            inOrderHelper(currentNode.getLeft(), currentList);
            currentList.add(currentNode.getData());
            inOrderHelper(currentNode.getRight(), currentList);
        }
    }

    /**
     * Should run in O(n).
     *
     * @return a postorder traversal of the tree
     */
    public List<T> postorder() {
        ArrayList<T> postorderList =  new ArrayList<>();
        postOrderHelper(root, postorderList);
        return postorderList;
    }

    /**
     * helper method for the postorder method
     * @param currentNode is a pointer to the current node allowing recursion
     * @param currentList is the list of the data in the specified order
     */
    private void postOrderHelper(BSTNode<T> currentNode, List<T> currentList) {
        if (currentNode != null) {
            postOrderHelper(currentNode.getLeft(), currentList);
            postOrderHelper(currentNode.getRight(), currentList);
            currentList.add(currentNode.getData());
        }
    }

    /**
     * Generate a level-order traversal of the tree.
     *
     * To do this, add the root node to a queue. Then, while the queue isn't
     * empty, remove one node, add its data to the list being returned, and add
     * its left and right child nodes to the queue. If what you just removed is
     * {@code null}, ignore it and continue with the rest of the nodes.
     *
     * Should run in O(n).
     *
     * @return a level order traversal of the tree
     */
    public List<T> levelorder() {
        Queue<BSTNode<T>> nodeList = new LinkedList<>();
        ArrayList<T> levelorderList =  new ArrayList<>();
        nodeList.add(root);
        while (!nodeList.isEmpty()) {
            BSTNode<T> tempNode = nodeList.poll();
            levelorderList.add(tempNode.getData());
            if (tempNode.getLeft() != null) {
                nodeList.add(tempNode.getLeft());
            }
            if (tempNode.getRight() != null) {
                nodeList.add(tempNode.getRight());
            }
        }
        return levelorderList;
    }



    /**
     * Clears the tree.
     *
     * Should run in O(1).
     */
    public void clear() {
        this.size = 0;
        root = null;
    }

    /**
     * Calculate and return the height of the root of the tree. A node's
     * height is defined as {@code max(left.height, right.height) + 1}. A leaf
     * node has a height of 0 and a null child should be -1.
     *
     * Should be calculated in O(n).
     *
     * @return the height of the root of the tree, -1 if the tree is empty
     */
    public int height() {
        if (root == null) {
            return -1;
        }
        return heightHelper(root) - 1;
    }

    /**
     * Helper method that recursively finds the height of a node
     *
     * @param currentNode is a pointer to the current node allowing recursion
     * @return the height of the current node
     */
    private int heightHelper(BSTNode<T> currentNode) {
        if (currentNode == null) {
            return 0;
        } else {
            int leftHeight = heightHelper(currentNode.getLeft());
            int rightHeight = heightHelper(currentNode.getRight());

            if (leftHeight > rightHeight) {
                return (leftHeight + 1);
            } else {
                return (rightHeight + 1);
            }
        }
    }

    /**
     * THIS METHOD IS ONLY FOR TESTING PURPOSES.
     *
     * DO NOT USE THIS METHOD IN YOUR CODE.
     *
     * @return the number of elements in the tree
     */
    public int size() {
        // DO NOT MODIFY THIS METHOD
        return size;
    }

    /**
     * THIS METHOD IS ONLY FOR TESTING PURPOSES.
     *
     * DO NOT USE THIS METHOD IN YOUR CODE.
     *
     * @return the root of the tree
     */
    public BSTNode<T> getRoot() {
        // DO NOT MODIFY THIS METHOD!
        return root;
    }
}
