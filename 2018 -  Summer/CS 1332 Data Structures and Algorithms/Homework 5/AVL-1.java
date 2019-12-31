import java.util.Collection;

/**
 * Your implementation of an AVL Tree.
 *
 * @author YOUR NAME HERE
 * @userid YOUR USER ID HERE (i.e. gburdell3)
 * @GTID YOUR GT ID HERE (i.e. 900000000)
 * @version 1.0
 */
public class AVL<T extends Comparable<? super T>> {
    // DO NOT ADD OR MODIFY INSTANCE VARIABLES.
    private AVLNode<T> root;
    private int size;

    /**
     * A no argument constructor that should initialize an empty AVL tree.
     */
    public AVL() {
        // DO NOT IMPLEMENT THIS CONSTRUCTOR!
    }

    /**
     * Initializes the AVL tree with the data in the Collection. The data
     * should be added in the same order it is in the Collection.
     *
     * @param data the data to add to the tree
     * @throws IllegalArgumentException if data or any element in data is null
     */
    public AVL(Collection<T> data) {
        if (data == null) {
            throw new IllegalArgumentException(
                    "the construction parameter cannot be null");
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
     * Add the data as a leaf to the AVL. Should traverse the tree to find the
     * appropriate location. If the data is already in the tree, then nothing
     * should be done (the duplicate shouldn't get added, and size should not be
     * incremented).
     *
     * Remember to recalculate heights going up the tree, rebalancing if
     * necessary.
     *
     * @throws java.lang.IllegalArgumentException if the data is null
     * @param data the data to be added
     */
    public void add(T data) {
        if (data == null) {
            throw new IllegalArgumentException(
                    "cannot add null data to the Binary Tree");
        }
        AVLNode<T> newNode = new AVLNode<>(data);
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
    private  AVLNode<T> addHelper(AVLNode<T> toAdd, AVLNode<T> currentNode) {
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
        updateHeight(currentNode);
        updateBalanceFactor(currentNode);
        return rotate(currentNode);
    }

    /**
     * Helper method that finds and stores the height of the node
     *
     * @param currentNode is a pointer to the current node allowing recursion
     */
    private void updateHeight(AVLNode<T> currentNode) {
        int leftHeight = -1;
        int rightHeight = -1;
        if (currentNode.getLeft() != null) {
            leftHeight = currentNode.getLeft().getHeight();
        }
        if (currentNode.getRight() != null) {
            rightHeight = currentNode.getRight().getHeight();
        }
        currentNode.setHeight((Math.max(leftHeight, rightHeight) + 1));
    }

    /**
     * Helper method that finds and stores the Balance Factor of the node
     *
     * @param currentNode is a pointer to the current node allowing recursion
     */
    private void updateBalanceFactor(AVLNode<T> currentNode) {
        int leftHeight = -1;
        int rightHeight = -1;
        if (currentNode.getLeft() != null) {
            leftHeight = currentNode.getLeft().getHeight();
        }
        if (currentNode.getRight() != null) {
            rightHeight = currentNode.getRight().getHeight();
        }
        int balanceFactor = leftHeight - rightHeight;
        currentNode.setBalanceFactor(balanceFactor);
    }

    /**
     * Helper method that returns the Balance Factor of the node
     *
     * @param currentNode is a the node you want to return balance factor on
     * @return the balance factor
     */
    private int getBF(AVLNode<T> currentNode) {
        if (currentNode == null) {
            return 0;
        } else {
            return currentNode.getBalanceFactor();
        }
    }

    /**
     * Helper method that rotates if the balance factor is off
     *
     * @param currentNode is a pointer to the current node allowing recursion
     * @return the node you are currently at recreating the  new tree
     */
    private AVLNode<T> rotate(AVLNode<T> currentNode) {
        int balanceFactor = currentNode.getBalanceFactor();
        if (balanceFactor > 1) {
            int leftBF = getBF(currentNode.getLeft());
            if (leftBF < 0) {
                return leftRightRotate(currentNode);
            } else {
                return rightRotate(currentNode);
            }
        } else if (balanceFactor < -1) {
            int rightBF = getBF(currentNode.getRight());
            if (rightBF > 0) {
                return rightLeftRotate(currentNode);
            } else {
                return leftRotate(currentNode);
            }
        } else {
            return currentNode;
        }
    }

    /**
     * Helper method that performs a left rotate
     *
     * @param currentNode is a pointer to the current node allowing recursion
     * @return the post rotate node recreating the  new tree
     */
    private AVLNode<T> leftRotate(AVLNode<T> currentNode) {
        AVLNode tempNode = currentNode.getRight();
        currentNode.setRight(tempNode.getLeft());
        tempNode.setLeft(currentNode);
        updateHeight(currentNode);
        updateBalanceFactor(currentNode);
        updateHeight(tempNode);
        updateBalanceFactor(tempNode);
        return tempNode;
    }

    /**
     * Helper method that performs a right rotate
     *
     * @param currentNode is a pointer to the current node allowing recursion
     * @return the post rotate node recreating the  new tree
     */
    private AVLNode<T> rightRotate(AVLNode<T> currentNode) {
        AVLNode tempNode = currentNode.getLeft();
        currentNode.setLeft(tempNode.getRight());
        tempNode.setRight(currentNode);
        updateHeight(currentNode);
        updateBalanceFactor(currentNode);
        updateHeight(tempNode);
        updateBalanceFactor(tempNode);
        return tempNode;
    }

    /**
     * Helper method that performs a leftRight rotate
     *
     * @param currentNode is a pointer to the current node allowing recursion
     * @return the post rotate node recreating the  new tree
     */
    private AVLNode<T> leftRightRotate(AVLNode<T> currentNode) {
        currentNode.setLeft(leftRotate(currentNode.getLeft()));
        return rightRotate(currentNode);
    }

    /**
     * Helper method that performs a Rightleft rotate
     *
     * @param currentNode is a pointer to the current node allowing recursion
     * @return the post rotate node recreating the  new tree
     */
    private AVLNode<T> rightLeftRotate(AVLNode<T> currentNode) {
        currentNode.setRight(rightRotate(currentNode.getRight()));
        return leftRotate(currentNode);
    }

    /**
     * Removes the data from the tree. There are 3 cases to consider:
     * 1: The data is a leaf. In this case, simply remove it.
     * 2: The data has one child. In this case, simply replace the node with
     * the child node.
     * 3: The data has 2 children. For this assignment, use the predecessor to
     * replace the data you are removing, not the sucessor.
     *
     * Remember to recalculate heights going up the tree, rebalancing if
     * necessary.
     *
     * @throws java.lang.IllegalArgumentException if the data is null
     * @throws java.util.NoSuchElementException if the data is not in the tree
     * @param data data to remove from the tree
     * @return the data removed from the tree.  Do not return the same data
     * that was passed in. Return the data that was stored in the tree.
     */
    public T remove(T data) {
        if (data == null) {
            throw new IllegalArgumentException(
                    "tree can't contain null values, so you can't remove null");
        }
        AVLNode<T> dummyNode = new AVLNode<>(null);
        root = removeHelper2(root, dummyNode, data);
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
    private AVLNode<T> removeHelper2(AVLNode<T> currentNode,
                                     AVLNode<T> dummyNode, T data) {
        if (currentNode == null) {
            throw new java.util.NoSuchElementException("no element found");
        } else if (currentNode.getData().compareTo(data) == 0) {
            return removeChildCases(currentNode, dummyNode);
        } else {
            if (data.compareTo(currentNode.getData()) > 0) {
                currentNode.setRight(
                        removeHelper2(currentNode.getRight(), dummyNode, data));
            } else {
                currentNode.setLeft(
                        removeHelper2(currentNode.getLeft(), dummyNode, data));
            }
        }
        updateHeight(currentNode);
        updateBalanceFactor(currentNode);
        return rotate(currentNode);
    }

    /**
     * Helper method for remove when there are children
     *
     * @param currentNode is a pointer to the current node allowing recursion
     * @param dummyNode is a node that will contain the value removed
     * @return the node you are currently at recreating the  new tree
     */
    private AVLNode<T> removeChildCases(AVLNode<T> currentNode,
                                        AVLNode<T> dummyNode) {
        //No Children
        if (currentNode.getLeft() == null
                && currentNode.getRight() == null) {
            size = size - 1;
            dummyNode.setData(currentNode.getData());
            return null;
            //One Child Left
        } else if (currentNode.getLeft() != null
                && currentNode.getRight() == null) {
            size = size - 1;
            dummyNode.setData(currentNode.getData());
            return currentNode.getLeft();
            //One Child Right
        } else if (currentNode.getLeft() == null
                && currentNode.getRight() != null) {
            size = size - 1;
            dummyNode.setData(currentNode.getData());
            return currentNode.getRight();
            //TWO CHILD CASE
        } else {
            dummyNode.setData(currentNode.getData());
            T predecessor = predecessor2(currentNode.getData());
            currentNode.setData(predecessor);
            AVLNode<T> dumbNode = new AVLNode<>(null);
            currentNode.setLeft(
                    removeHelper2(currentNode.getLeft(),
                            dumbNode, predecessor));
            updateHeight(currentNode);
            updateBalanceFactor(currentNode);
            return rotate(currentNode);
        }
    }

    /**
     * Helper method that recursively returns the predecessor's data
     *
     * @param data is the data of the tree you want to remove
     * @return the node you are currently at recreating the  new tree
     */
    private T predecessor2(T data) {
        if (data == null) {
            throw new IllegalArgumentException(
                    "tree cannot contain a null value, so no successor exists");
        }
        AVLNode<T> ancestorNode = new AVLNode<>(null);
        return predecessorHelper2(data, root, ancestorNode);
    }

    /**
     * helper method for the predecessor method
     * @param currentNode is a pointer to the current node allowing recursion
     * @param ancestorNode is a pointer to the ancestor node's value
     * @param data the data to search for in the tree.
     * @return the data of the successor or throws an error
     */
    private T predecessorHelper2(T data, AVLNode<T> currentNode,
                                 AVLNode<T> ancestorNode) {
        T isFound;
        if (currentNode == null) {
            throw new java.util.NoSuchElementException(
                    "The element wasn't found in the tree");
        }
        if (currentNode.getData().compareTo(data) < 0) {
            ancestorNode.setData(currentNode.getData());
        }
        if (currentNode.getData().compareTo(data) == 0) {
            if (currentNode.getLeft() != null) {
                return predecessorHelperHelper(currentNode.getLeft());
            } else {
                return ancestorNode.getData();
            }
        } else if (data.compareTo(currentNode.getData()) > 0) {
            isFound = predecessorHelper2(data, currentNode.getRight(),
                    ancestorNode);
        } else {
            isFound = predecessorHelper2(data, currentNode.getLeft(),
                    ancestorNode);
        }
        return isFound;
    }

    /**
     * Helper method that recursively gets the predecessor data from a node
     *
     * @param currentNode is a pointer to the current node allowing recursion
     * @return the node you are currently at recreating the new tree
     * */
    private T predecessorHelperHelper(AVLNode<T> currentNode) {
        if (currentNode.getRight() == null) {
            return currentNode.getData();
        } else {
            return predecessorHelperHelper(currentNode.getRight());
        }
    }


    /**
     * Returns the data in the tree matching the parameter passed in.
     *
     * @throws java.lang.IllegalArgumentException if the data is null
     * @throws java.util.NoSuchElementException if the data is not found
     * @param data data to get in the AVL tree
     * @return the data in the tree equal to the parameter.  Do not return the
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
    private T getHelper(T data, AVLNode<T> currentNode) {
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
     * Returns whether or not the parameter is contained within the tree.
     *
     * @throws java.lang.IllegalArgumentException if the data is null
     * @param data data to find in the AVL tree
     * @return whether or not the parameter is contained within the tree
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
    private boolean containsHelper(T data, AVLNode<T> currentNode) {
        boolean isFound;
        if (currentNode == null) {
            return false;
        }
        if (currentNode.getData().compareTo(data) == 0) {
            return true;
        } else if (data.compareTo(currentNode.getData()) > 0) {
            isFound = containsHelper(data, currentNode.getRight());
        } else {
            isFound = containsHelper(data, currentNode.getLeft());
        }
        return isFound;
    }

    /**
     * In your BST homework, you worked with the concept of the successor, the
     * smallest data that is larger than the current data. However, you only
     * saw it in the context of the 2-child remove case.
     *
     * This method should retrieve (but not remove) the successor of the data
     * passed in. There are 2 cases to consider:
     * 1: The right subtree is non-empty. In this case, the successor is the
     * leftmost node of the right subtree.
     * 2: The right subtree is empty. In this case, the successor is the lowest
     * ancestor of the node containing {@code data} whose left child is also
     * an ancestor of {@code data}.
     *
     * For example, in the tree below, the successor of 76 is 81, and the
     * successor of 40 is 76.
     *
     *                    76
     *                  /    \
     *                34      90
     *                  \    /
     *                  40  81
     *
     * @throws java.lang.IllegalArgumentException if the data is null
     * @throws java.util.NoSuchElementException if the data is not in the tree
     * @param data the data to find the successor of
     * @return the successor of {@code data}. If there is no larger data than
     * the one given, return null.
     */
    public T successor(T data) {
        if (data == null) {
            throw new IllegalArgumentException(
                    "tree cannot contain a null value, so no successor exists");
        }
        AVLNode<T> ancestorNode = new AVLNode<>(null);
        return successorHelper(data, root, ancestorNode);
    }

    /**
     * helper method for the successor method
     * @param currentNode is a pointer to the current node allowing recursion
     * @param ancestorNode is a pointer to the ancestor node's value
     * @param data the data to search for in the tree.
     * @return the data of the successor or throws an error
     */
    private T successorHelper(T data, AVLNode<T> currentNode,
                              AVLNode<T> ancestorNode) {
        T isFound;
        if (currentNode == null) {
            throw new java.util.NoSuchElementException(
                    "The element wasn't found in the tree");
        }
        if (currentNode.getData().compareTo(data) > 0) {
            ancestorNode.setData(currentNode.getData());
        }
        if (currentNode.getData().compareTo(data) == 0) {
            if (currentNode.getRight() != null) {
                return successorHelperHelper(currentNode.getRight());
            } else {
                return ancestorNode.getData();
            }
        } else if (data.compareTo(currentNode.getData()) > 0) {
            isFound = successorHelper(data, currentNode.getRight(),
                    ancestorNode);
        } else {
            isFound = successorHelper(data, currentNode.getLeft(),
                    ancestorNode);
        }
        return isFound;
    }

    /**
     * Helper method that recursively gets the successor data from a node
     *
     * @param currentNode is a pointer to the current node allowing recursion
     * @return the node you are currently at recreating the new tree
     * */
    private T successorHelperHelper(AVLNode<T> currentNode) {
        if (currentNode.getLeft() == null) {
            return currentNode.getData();
        } else {
            return successorHelperHelper(currentNode.getLeft());
        }
    }

    /**
     * Return the height of the root of the tree.
     *
     * This method does not need to traverse the tree since this is an AVL.
     *
     * @return the height of the root of the tree, -1 if the tree is empty
     */
    public int height() {
        if (root == null) {
            return -1;
        }
        return root.getHeight();
    }

    /**
     * Clears the tree.
     */
    public void clear() {
        this.size = 0;
        root = null;
    }

    /**
     * Get the number of elements in the tree.
     *
     * DO NOT USE OR MODIFY THIS METHOD!
     *
     * @return the number of elements in the tree
     */
    public int size() {
        // DO NOT MODIFY THIS METHOD!
        return size;
    }

    /**
     * Returns the root of the tree. Normally, you wouldn't do this, but it's
     * necessary to grade your code.
     *
     * DO NOT USE OR MODIFY THIS METHOD!
     *
     * @return the root of the tree
     */
    public AVLNode<T> getRoot() {
        // DO NOT MODIFY THIS METHOD!
        return root;
    }
}


