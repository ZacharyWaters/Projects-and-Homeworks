/**
 * represents a single chess square
 *
 * @author zwaters3
 * @version 1.0
 */
public class Square {
    private char rank;
    private char file;
    private String both;

    /**
     *Creates a Square object with the String
     *name consisting of rank and file combined
     * @param name  takes in a strng of both rank and file
     */
    public Square(String name) {
        this(name.charAt(0), name.charAt(1));
    }
    /**
     *Creates a Square object with the chars
     *consisting of rank and file seperatly
     * @param   file a character that is a letter
     * @param   rank a character that is a number
     */
    public Square(char file, char rank) {
        if (!((file == 'a') || (file == 'b')
            || (file == 'c') || (file == 'd')
            || (file == 'e') || (file == 'f')
            || (file == 'g') || (file == 'h'))
            || !((rank == '1') || (rank == '2')
            || (rank == '3') || (rank == '4')
            || (rank == '5') || (rank == '6')
            || (rank == '7') || (rank == '8'))) {
            throw new InvalidSquareException("" + file + rank
                + "-is a Square that doesn't exist on the chess board.");
        }
        this.file = file;
        this.rank = rank;
        both = "" + file + rank;
    }
    /**
     *returns a string of both rank and file
     *consisting of rank and file seperatly
     * @return String both which is both rank and file
     */
    @Override
    public String toString() {
        return both;
    }
    /**
     *Returns file
     *@return char file which is a letter
     */
    public char getFile() {
        return this.file;
    }
    /**
     *Returns rank
     *@return char rank which is a number
     */
    public char getRank() {
        return this.rank;
    }
    /**
     *Returns the int value of file
     *@return int value fo file
     */
    public int getFileInt() {
        return getFile() - 'a' + 1;
    }
    /**
     *Returns the int value of rank
     *@return int value of rank
     */
    public int getRankInt() {
        return getRank() - '0';
    }
    /**
     *Returns an hash value of the
     *Square given to it by concatenating
     *rank and fle
     *@return int sum which is rannk and file together
     */
    @Override
    public int hashCode() {
        int sum = 0;
        String concat = "";
        concat = concat + this.getFileInt() + this.getRankInt();
        sum = Integer.parseInt(concat);
        return sum;
    }
    /**
     *Returns true if both the char and file
     *of two squares are equal
     *@param given square to compare
     *@return boolean, true if they are equal, else false
     */
    @Override
    public boolean equals(Object given) {
        if (this == given) {
            return true;
        }
        if (given == null) {
            return false;
        }
        if ((given.toString().equals(this.toString()))) {
            return true;
        }
        return false;
    }
}