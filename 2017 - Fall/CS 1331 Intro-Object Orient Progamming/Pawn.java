/**
* represents the Pawn piece
*
* @author zwaters3
*/
public class Pawn extends Piece {
    public Pawn(Color color) {
        super(color);
    }
    /**
    *Returns a string containing the
    *algebraic letter for Pawn
    */
    public String algebraicName() {
        return "";
    }
    /**
    *Returns a string containing the fen
    *name of Pawn for both white and black
    */
    public String fenName() {
        if (color == Color.BLACK) {
            return "p";
        } else {
            return "P";
        }
    }

    /**
    *Returns an array of Squares containing
    *the squares where a white Pawn could move to
    *from a given square location, with nulls
    */
    public Square[] cleanupblackpawn(int[] loc) {
        Square[][] board = getboard();
        int counter = 0;
        Square[] nulls = new Square[29];
        int row = loc[0];
        int col = loc[1];
        //Down
        for (int i = row + 1; i <= 7; i++) {
            nulls[counter] = board[i][col];
            counter++;
            break;
        }
        if (row == 1) {
            //Down Opening
            for (int i = row + 2; i <= 7; i++) {
                nulls[counter] = board[i][col];
                counter++;
                break;
            }
        }
        return nulls;
    }
    /**
    *Returns an array of Squares containing
    *the squares where a black pawn could move to
    *from a given square location, with nulls
    */
    public Square[] cleanupwhitepawn(int[] loc) {
        Square[][] board = getboard();
        int counter = 0;
        Square[] nulls = new Square[29];
        int row = loc[0];
        int col = loc[1];
        //up
        for (int i = row - 1; i >= 0; i--) {
            nulls[counter] = board[i][col];
            counter++;
            break;
        }
        if (row == 6) {
            for (int i = row - 2; i >= 0; i--) {
                nulls[counter] = board[i][col];
                counter++;
                break;
            }
        }
        return nulls;
    }

    /**
    *Returns an array of Squares containing
    *the squares where a Pawn could move to
    *from a given square location, without nulls
    */
    public Square[] movesFrom(Square square) {
        if (color == Color.BLACK) {
            return shorten(cleanupblackpawn(findLoc(square)));
        } else {
            return shorten(cleanupwhitepawn(findLoc(square)));
        }
    }
}