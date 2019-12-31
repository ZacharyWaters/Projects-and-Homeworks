import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Your implementations of various string searching algorithms.
 *
 * @author Zachary Waters
 * @userid zwaters3
 * @GTID 903116871
 * @version 1.0
 */
public class PatternMatching {

    /**
     * Knuth-Morris-Pratt (KMP) algorithm that relies on the failure table (also
     * called failure function). Works better with small alphabets.
     *
     * Make sure to implement the failure table before implementing this method.
     *
     * @throws IllegalArgumentException if the pattern is null or of length 0
     * @throws IllegalArgumentException if text or comparator is null
     * @param pattern the pattern you are searching for in a body of text
     * @param text the body of text where you search for pattern
     * @param comparator you MUST use this for checking character equality
     * @return list containing the starting index for each match found
     */
    public static List<Integer> kmp(CharSequence pattern, CharSequence text,
                                    CharacterComparator comparator) {
        //handling Exceptions
        if (pattern == null) {
            throw new IllegalArgumentException("the Pattern cannot be null");
        }
        if (text == null) {
            throw new IllegalArgumentException("the text cannot be null");
        }
        if (comparator == null) {
            throw new IllegalArgumentException("the comparator cannot be null");
        }
        if (pattern.length() == 0) {
            throw new IllegalArgumentException("the Pattern can't be length 0");
        }
        //making the list of found indexes
        List<Integer> list = new ArrayList<>();
        //handling an immediate end case
        if (pattern.length() > text.length()) {
            return list;
        }
        //getting the failure table
        int[] failTable = buildFailureTable(pattern, comparator);
        //Setting up variables
        int jPattern = 0;
        int matches = 0;
        for (int iText = 0;
             iText <= text.length() - pattern.length();
             iText++) {
            while (jPattern < pattern.length()
                    && comparator.compare(text.charAt(iText + jPattern),
                    pattern.charAt(jPattern)) == 0) {
                jPattern++;
                matches++;
            }
            if (matches == pattern.length()) {
                list.add(iText);
            }
            if (jPattern > 0) {
                int failValue = failTable[jPattern - 1];
                int shiftValue = jPattern - failTable[jPattern - 1];
                iText =  iText + shiftValue;
                //this offsets the for loops auto-add
                iText--;
                jPattern = failValue;
                matches = jPattern;
            }
        }
        return list;
    }



    /**
     * Builds failure table that will be used to run the Knuth-Morris-Pratt
     * (KMP) algorithm.
     *
     * The table built should be the length of the input text.
     *
     * Note that a given index i will be the largest prefix of the pattern
     * indices [0..i] that is also a suffix of the pattern indices [1..i].
     * This means that index 0 of the returned table will always be equal to 0
     *
     * Ex. ababac
     *
     * table[0] = 0
     * table[1] = 0
     * table[2] = 1
     * table[3] = 2
     * table[4] = 3
     * table[5] = 0
     *
     * If the pattern is empty, return an empty array.
     *
     * @throws IllegalArgumentException if the pattern or comparator is null
     * @param pattern a {@code CharSequence} you're building a failure table for
     * @param comparator you MUST use this for checking character equality
     * @return integer array holding your failure table
     */
    public static int[] buildFailureTable(CharSequence pattern,
                                          CharacterComparator comparator) {
        if (pattern == null) {
            throw new IllegalArgumentException("the Pattern cannot be null");
        }
        if (comparator == null) {
            throw new IllegalArgumentException("the comparator cannot be null");
        }
        //MAKING THE FAILURE TABLE
        int[] table = new int[pattern.length()];
        if (pattern.length() == 0) {
            return table;
        }
        int i = 0;
        int j = 1;
        while (j < pattern.length()) {
            //If the characters pointed to by i and j match,
            if (comparator.compare(pattern.charAt(i), pattern.charAt(j)) == 0) {
                table[j] = i + 1;
                j++;
                i++;
                //If the characters pointed to by i and j do not match:
                //If i is not at 0.
            } else if (i != 0) {
                i = table[i - 1];
                //If i is at 0.
            } else {
                table[j] = i;
                j++;
            }
        }
        return table;
    }

    /**
     * Boyer Moore algorithm that relies on last occurrence table. Works better
     * with large alphabets.
     *
     * Make sure to implement the last occurrence table before implementing this
     * method.
     *
     * @throws IllegalArgumentException if the pattern is null or of length 0
     * @throws IllegalArgumentException if text or comparator is null
     * @param pattern the pattern you are searching for in a body of text
     * @param text the body of text where you search for the pattern
     * @param comparator you MUST use this for checking character equality
     * @return list containing the starting index for each match found
     */

    public static List<Integer> boyerMoore(CharSequence pattern,
                                           CharSequence text,
                                           CharacterComparator comparator) {
        //handling Exceptions
        if (pattern == null) {
            throw new IllegalArgumentException("the Pattern cannot be null");
        }
        if (text == null) {
            throw new IllegalArgumentException("the text cannot be null");
        }
        if (comparator == null) {
            throw new IllegalArgumentException("the comparator cannot be null");
        }
        if (pattern.length() == 0) {
            throw new IllegalArgumentException("the Pattern can't be length 0");
        }
        //making the list of found indexes
        List<Integer> list = new ArrayList<>();
        //handling an immediate end case
        if (pattern.length() > text.length()) {
            return list;
        }
        //Getting the Last table
        Map<Character, Integer> lastTable = buildLastTable(pattern);
        int iTextFront = 0;
        int jPattern = pattern.length() - 1;
        while (iTextFront <= text.length() - pattern.length()) {
            int matches = 0;
            int iTextBack = iTextFront + jPattern;
            while (jPattern >= 0
                    && comparator.compare(text.charAt(iTextBack),
                    pattern.charAt(jPattern)) == 0) {
                jPattern--;
                iTextBack = iTextFront + jPattern;
                matches++;
            }
            if (matches == pattern.length()) {
                list.add(iTextFront);
                iTextFront++;
                jPattern = pattern.length() - 1;
            } else {
                int lastValue = lastTable.getOrDefault(text.charAt(iTextBack),
                        -1);
                int shiftValue = jPattern - lastValue;
                if (shiftValue <= 0) {
                    iTextFront++;
                    jPattern = pattern.length() - 1;
                } else {
                    iTextFront = iTextFront + shiftValue;
                    jPattern = pattern.length() - 1;
                }
            }
        }
        return list;
    }


    /**
     * Builds last occurrence table that will be used to run the Boyer Moore
     * algorithm.
     *
     * Note that each char x will have an entry at table.get(x).
     * Each entry should be the last index of x where x is a particular
     * character in your pattern.
     * If x is not in the pattern, then the table will not contain the key x,
     * and you will have to check for that in your Boyer Moore implementation.
     *
     * Ex. octocat
     *
     * table.get(o) = 3
     * table.get(c) = 4
     * table.get(t) = 6
     * table.get(a) = 5
     * table.get(everything else) = null, which you will interpret in
     * Boyer-Moore as -1
     *
     * If the pattern is empty, return an empty map.
     *
     * @throws IllegalArgumentException if the pattern is null
     * @param pattern a {@code CharSequence} you are building last table for
     * @return a Map with keys of all of the characters in the pattern mapping
     *         to their last occurrence in the pattern
     */
    public static Map<Character, Integer> buildLastTable(CharSequence pattern) {
        if (pattern == null) {
            throw new IllegalArgumentException("the pattern can't be null");
        }
        Map<Character, Integer> map = new HashMap<>();
        if (pattern.length() == 0) {
            return map;
        }
        for (int i = 0; i < pattern.length(); i++) {
            map.put(pattern.charAt(i), i);
        }
        return map;
    }

    /**
     * Prime base used for Rabin-Karp hashing.
     * DO NOT EDIT!
     */
    private static final int BASE = 137;

    /**
     * Runs the Rabin-Karp algorithm. This algorithms generates hashes for the
     * pattern and compares this hash to substrings of the text before doing
     * character by character comparisons.
     *
     * When the hashes are equal and you do character comparisons, compare
     * starting from the beginning of the pattern to the end, not from the end
     * to the beginning.
     *
     * You must use the Rabin-Karp Rolling Hash for this implementation. The
     * formula for it is:
     *
     * sum of: c * BASE ^ (pattern.length - 1 - i), where c is the integer
     * value of the current character, and i is the index of the character
     *
     * For example: Hashing "bunn" as a substring of "bunny" with base 137 hash
     * = b * 137 ^ 3 + u * 137 ^ 2 + n * 137 ^ 1 + n * 137 ^ 0 = 98 * 137 ^ 3 +
     * 117 * 137 ^ 2 + 110 * 137 ^ 1 + 110 * 137 ^ 0 = 254203747
     *
     * Note that since you are dealing with very large numbers here, your hash
     * will likely overflow, and that is fine for this implementation.
     *
     * Another key step for this algorithm is that updating the hashcode from
     * one substring to the next one must be O(1). To update the hash:
     *
     *  remove the oldChar times BASE raised to the length - 1, multiply by
     *  BASE, and add the newChar.
     *
     * For example: Shifting from "bunn" to "unny" in "bunny" with base 137
     * hash("unny") = (hash("bunn") - b * 137 ^ 3) * 137 + y * 137 ^ 0 =
     * (254203747 - 98 * 137 ^ 3) * 137 + 121 * 137 ^ 0 = 302928082
     *
     * Keep in mind that calculating exponents is not O(1) in general, so you'll
     * need to keep track of what BASE^{m - 1} is for updating the hash.
     *
     * @throws IllegalArgumentException if the pattern is null or of length 0
     * @throws IllegalArgumentException if text or comparator is null
     * @param pattern a string you're searching for in a body of text
     * @param text the body of text where you search for pattern
     * @param comparator the comparator to use when checking character equality
     * @return list containing the starting index for each match found
     */
    public static List<Integer> rabinKarp(CharSequence pattern,
                                          CharSequence text,
                                          CharacterComparator comparator) {
        if (pattern == null) {
            throw new IllegalArgumentException("the Pattern cannot be null");
        }
        if (text == null) {
            throw new IllegalArgumentException("the text cannot be null");
        }
        if (comparator == null) {
            throw new IllegalArgumentException("the comparator cannot be null");
        }
        if (pattern.length() == 0) {
            throw new IllegalArgumentException("the Pattern can't be length 0");
        }
        List<Integer> list = new ArrayList<>();
        if (pattern.length() > text.length()) {
            return list;
        }
        //GETTING THE HASH
        int hashPattern = 0;
        int hashText = 0;
        //GETTING THE HASH OF THE PATTERN AND TEXT
        int multiplyExponent = 1;
        hashPattern = 0;
        hashText = 0;
        int realExponent = 0;
        for (int i = pattern.length() - 1; i >= 0; i--) {
            char o = pattern.charAt(i);
            char q = text.charAt(i);
            hashPattern = hashPattern + (o * multiplyExponent);
            hashText = hashText + (q * multiplyExponent);
            realExponent = multiplyExponent;
            if (i < pattern.length()) {
                multiplyExponent = multiplyExponent * BASE;
            }
        }
        multiplyExponent = realExponent;
        //DOING OUR COMPARISONS
        for (int i = 0; i < text.length() - pattern.length(); i++) {
            //A MATCH FOUND
            if (hashPattern == hashText) {
                int matches = 0;
                int startIndex = i;
                int jnew = 0;
                //Check the characters 1 by 1
                while (jnew < pattern.length()
                        && comparator.compare(text.charAt(i + jnew),
                        pattern.charAt(jnew)) == 0) {
                    jnew++;
                    matches++;
                }
                //NOT A FALSE ALARM
                if (matches == pattern.length()) {
                    list.add(startIndex);
                }
            }
            //SHIFTING HASH TO NEXT CHARACTER
            hashText = BASE
                    * (hashText - text.charAt(i)
                    *  multiplyExponent)
                    + text.charAt(i + pattern.length());
        }
        if (hashPattern == hashText)  {
            list.add(text.length() - pattern.length());
        }
        return list;
    }

}
