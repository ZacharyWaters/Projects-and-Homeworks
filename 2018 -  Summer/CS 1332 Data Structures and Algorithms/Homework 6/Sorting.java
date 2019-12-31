import java.util.Comparator;
import java.util.Queue;
import java.util.LinkedList;

/**
 * Your implementation of various sorting algorithms.
 *
 * @author YOUR NAME HERE
 * @userid YOUR USER ID HERE (i.e. gburdell3)
 * @GTID YOUR GT ID HERE (i.e. 900000000)
 * @version 1.0
 */
public class Sorting {

    /**
     * Implement bubble sort.
     *
     * It should be:
     *  in-place
     *  stable
     *  adaptive
     *
     * Have a worst case running time of:
     *  O(n^2)
     *
     * And a best case running time of:
     *  O(n)
     *
     * You may assume that the array doesn't contain any null elements.
     *
     * @throws IllegalArgumentException if the array or comparator is null
     * @param <T> data type to sort
     * @param arr the array that must be sorted after the method runs
     * @param comparator the Comparator used to compare the data in arr
     */
    public static <T> void bubbleSort(T[] arr, Comparator<T> comparator) {
        if (arr == null) {
            throw new IllegalArgumentException("The array cannot be null");
        }
        if (comparator == null) {
            throw new IllegalArgumentException("The comparator cannot be null");
        }
        int endFlag = arr.length - 1;
        int startFlag = 0;
        boolean isSorted = false;
        int brakes;
        while (startFlag < arr.length && !isSorted) {
            brakes = 0;
            for (int j = 0; j < endFlag; j++) {
                if (comparator.compare(arr[j], arr[j + 1]) > 0) {
                    T temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                    brakes++;
                }
            }
            if (brakes == 0) {
                isSorted = true;
            }
            endFlag--;
        }
    }

    /**
     * Implement insertion sort.
     *
     * It should be:
     *  in-place
     *  stable
     *  adaptive
     *
     * Have a worst case running time of:
     *  O(n^2)
     *
     * And a best case running time of:
     *  O(n)
     *
     * You may assume that the array doesn't contain any null elements.
     *
     * @throws IllegalArgumentException if the array or comparator is null
     * @param <T> data type to sort
     * @param arr the array that must be sorted after the method runs
     * @param comparator the Comparator used to compare the data in arr
     */
    public static <T> void insertionSort(T[] arr, Comparator<T> comparator) {
        if (arr == null) {
            throw new IllegalArgumentException("The array cannot be null");
        }
        if (comparator == null) {
            throw new IllegalArgumentException("The comparator cannot be null");
        }
        for (int i = 1; i < arr.length; i++) {
            T value = arr[i];
            int marker = i - 1;
            while (marker >= 0 && comparator.compare(arr[marker], value) > 0) {
                int shiftedRight = marker + 1;
                arr[shiftedRight] = arr[marker];
                marker--;
            }
            int shiftedRight = marker + 1;
            arr[shiftedRight] = value;
        }
    }

    /**
     * Implement merge sort.
     *
     * It should be:
     *  stable
     *
     * Have a worst case running time of:
     *  O(n log n)
     *
     * And a best case running time of:
     *  O(n log n)
     *
     * You may assume that the array doesn't contain any null elements.
     *
     * You can create more arrays to run mergesort, but at the end,
     * everything should be merged back into the original T[]
     * which was passed in.
     *
     * When splitting the array, if there is an odd number of elements, put the
     * extra data on the right side.
     *
     * @throws IllegalArgumentException if the array or comparator is null
     * @param <T> data type to sort
     * @param arr the array to be sorted
     * @param comparator the Comparator used to compare the data in arr
     */
    public static <T> void mergeSort(T[] arr, Comparator<T> comparator) {
        if (arr == null) {
            throw new IllegalArgumentException("The array cannot be null");
        }
        if (comparator == null) {
            throw new IllegalArgumentException("The comparator cannot be null");
        }
        //END CONDITION FOR RECURSION
        if (arr.length <= 1) {
            return;
        }
        int leftArraySize = (int) (((double) arr.length) / 2.0);
        int rightArraySize = (arr.length / 2) + ((arr.length % 2 == 0) ? 0 : 1);
        //int rightArraySize = (int) Math.ceil(((double) arr.length) / 2.0);
        T[] leftArray = (T[]) new Object[leftArraySize];
        T[] rightArray = (T[]) new Object[rightArraySize];
        int midpoint = (int) (((double) arr.length) / 2.0);
        //FILLS LEFT ARRAY
        for (int tracker = midpoint - 1; tracker >= 0; tracker--) {
            leftArray[tracker] = arr[tracker];
        }
        //FILLS RIGHT ARRAY
        for (int tracker = midpoint, rightTracker = 0;
             tracker < arr.length; tracker++, rightTracker++) {
            rightArray[rightTracker] = arr[tracker];
        }
        mergeSort(leftArray, comparator);
        mergeSort(rightArray, comparator);
        merge(leftArray, rightArray, arr, comparator);
    }

    /**
     * helper method for mergeSort
     * @param <T> data type to sort
     * @param left one of the arrays to be combined
     * @param right one of the arrays to be combined
     * @param base the array the combined will be at
     * @param comparator the Comparator used to compare the data in arr
     */
    private static <T> void merge(T[] left, T[] right,
                                  T[] base, Comparator<T> comparator) {
        if (left.length == 1 && right.length == 1) {
            if (comparator.compare(left[0], right[0]) > 0) {
                base[0] = right[0];
                base[1] = left[0];
            } else {
                base[0] = left[0];
                base[1] = right[0];
            }
        } else {
            int baseCounter = 0;
            int leftCounter = 0;
            int rightCounter = 0;
            while (leftCounter < left.length && rightCounter < right.length) {
                if (comparator.compare(left[leftCounter],
                        right[rightCounter]) > 0) {
                    base[baseCounter] = right[rightCounter];
                    rightCounter++;
                } else {
                    base[baseCounter] = left[leftCounter];
                    leftCounter++;
                }
                baseCounter++;
            }
            while (leftCounter < left.length) {
                base[baseCounter] = left[leftCounter];
                leftCounter++;
                baseCounter++;
            }
            while (rightCounter < right.length) {
                base[baseCounter] = right[rightCounter];
                rightCounter++;
                baseCounter++;
            }
        }
    }

    /**
     * Implement LSD (least significant digit) radix sort.
     *
     * Remember you CANNOT convert the ints to strings at any point in your
     * code! Doing so may result in a 0 for the implementation.
     *
     * It should be:
     *  stable
     *
     * Have a worst case running time of:
     *  O(kn)
     *
     * And a best case running time of:
     *  O(kn)
     *
     * Refer to the PDF for more information on LSD Radix Sort.
     *
     * You may use {@code java.util.ArrayList} or {@code java.util.LinkedList}
     * if you wish, but it may only be used inside radix sort and any radix sort
     * helpers. Do NOT use these classes with other sorts.
     *
     * @throws IllegalArgumentException if the array is null
     * @param arr the array to be sorted
     */
    public static void lsdRadixSort(int[] arr) {
        if (arr == null) {
            throw new IllegalArgumentException("The array cannot be null");
        }
        //Queue<Integer>[] buckets = new Queue[10];
        //int queueInitializer = 0;
        //List<Queue<Integer>> buckets = new ArrayList<Queue<Integer>>(10);
        Queue<Integer>[] buckets = new Queue[10];
        buckets[0] = new LinkedList<>();
        buckets[1] = new LinkedList<>();
        buckets[2] = new LinkedList<>();
        buckets[3] = new LinkedList<>();
        buckets[4] = new LinkedList<>();
        buckets[5] = new LinkedList<>();
        buckets[6] = new LinkedList<>();
        buckets[7] = new LinkedList<>();
        buckets[8] = new LinkedList<>();
        buckets[9] = new LinkedList<>();
        int maxTensPlace = 1;
        int tensPlaceCounter = 1;
        for (int i = 0; i < arr.length; i++) {
            int digitValue = arr[i];
            int tensValue = digitValue % 10;
            int digitValueCopy = digitValue;
            int tensSubCounter = 0;
            while (digitValueCopy > 0) {
                digitValueCopy = digitValueCopy / 10;
                tensSubCounter++;
            }
            if (tensValue < 0) {
                tensValue = tensValue * -1;
            }
            buckets[tensValue].add(digitValue);
            if (tensSubCounter > maxTensPlace) {
                maxTensPlace = tensSubCounter;
            }
        }
        int tenPOW = 10;
        while (tensPlaceCounter < maxTensPlace) {
            int dividend = tenPOW;
            tenPOW = tenPOW * 10;
            //int dividend = (int) Math.pow(10, tensPlaceCounter);
            Queue<Integer>[] newBuckets = new Queue[10];
            newBuckets[0] = new LinkedList<>();
            newBuckets[1] = new LinkedList<>();
            newBuckets[2] = new LinkedList<>();
            newBuckets[3] = new LinkedList<>();
            newBuckets[4] = new LinkedList<>();
            newBuckets[5] = new LinkedList<>();
            newBuckets[6] = new LinkedList<>();
            newBuckets[7] = new LinkedList<>();
            newBuckets[8] = new LinkedList<>();
            newBuckets[9] = new LinkedList<>();
            for (int i = 0; i < 10; i++) {
                while (buckets[i].peek() != null) {
                    int digitValue = buckets[i].remove();
                    int tensValue = (digitValue / dividend) % 10;
                    if (tensValue < 0) {
                        tensValue = tensValue * -1;
                    }
                    newBuckets[tensValue].add(digitValue);
                }

            }
            tensPlaceCounter++;
            buckets = newBuckets;
        }
        int arrTracker = 0;
        for (int i = 0; i < 10; i++) {
            while (buckets[i].peek() != null) {
                arr[arrTracker] = buckets[i].remove();
                arrTracker++;
            }
        }
        //END OF METHOD
    }
    //END OF CLASS FILE HERE
}

