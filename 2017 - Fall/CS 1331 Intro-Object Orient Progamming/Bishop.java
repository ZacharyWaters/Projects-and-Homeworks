/**
* represents the bishop piece
*
* @author zwaters3
*/
public class Bishop extends Piece {
    public Bishop(Color color) {
        super(color);
    }

    /**
    *Returns a string containing the
    *algebraic letter for bishop
    */
    public String algebraicName() {
        return "B";
    }

    /**
    *Returns a string containing the fen
    *name of bishop for both white and black
    */
    public String fenName() {
        if (color == Color.BLACK) {
            return "b";
        } else {
            return "B";
        }
    }

    /**
    *Returns an array of Squares containing
    *the squares where a bishop could move to
    *from a given square location, with nulls
    */
    public static Square[] cleanupbishop(int[] loc) {
        Square[][] board = getboard();
        int counter = 0;
        Square[] nulls = new Square[29];
        int row = loc[0];
        int col = loc[1];
        // Diag Right Up
        for (int i = 1;
            (row - i) >= 0 && (col + i) <= 7; i++) {
            nulls[counter] = board[row - i][col + i];
            counter++;
        }
        // Diag Right Down
        for (int i = 1;
            (row + i) <= 7 && (col + i) <= 7; i++) {
            nulls[counter] = board[row + i][col + i];
            counter++;
        }
        // Diag Left Down
        for (int i = 1;
            (row + i) <= 7 && (col - i) >= 0; i++) {
            nulls[counter] = board[row + i][col - i];
            counter++;
        }
        // Diag Left Up
        for (int i = 1;
            (row - i) >= 0 && (col - i) >= 0; i++) {
            nulls[counter] = board[row - i][col - i];
            counter++;
        }
        return nulls;
    }

    /**
    *Returns an array of Squares containing
    *the squares where a bishop could move to
    *from a given square location, without nulls
    */
    public Square[] movesFrom(Square square) {
        return shorten(cleanupbishop(findLoc(square)));
    }
}