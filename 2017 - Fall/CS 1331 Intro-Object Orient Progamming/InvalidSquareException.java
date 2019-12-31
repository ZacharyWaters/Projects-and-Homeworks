/**
 * is an exception that is thrown
 *when the user creates a square
 *that doesnt exist on a chess board
 *I chose for invalidsquare to be a
 *checked exception because in this
 *homework's case because it was the
 *exception I was most familiar with
 *in addition checked exceptions work
 *best when the error would be on the
 *client side, and would be easily
 *recoverable, all they would have to
 *do is enter a correct square.
 * @author zwaters3
 * @version 1.0
 */
public class InvalidSquareException extends RuntimeException {
    /**
     *Creates a InvalidSquareException object with a String
     * @param whatwasgiven string that is fed into the parent class
     */
    public InvalidSquareException(String whatwasgiven) {
        super(whatwasgiven);
    }
}