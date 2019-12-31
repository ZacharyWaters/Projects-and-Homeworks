/**
* represents the Queen piece
*
* @author zwaters3
*/
public class Queen extends Piece {
    public Queen(Color color) {
        super(color);
    }
    /**
    *Returns a string containing the
    *algebraic letter for Queen
    */
    public String algebraicName() {
        return "Q";
    }
    /**
    *Returns a string containing the fen
    *name of Queen for both white and black
    */
    public String fenName() {
        if (color == Color.BLACK) {
            return "q";
        } else {
            return "Q";
        }
    }

    /**
    *Returns an array of Squares containing
    *the squares where a Queen could move to
    *from a given square location, with nulls
    */
    public Square[] cleanupqueen(int[] loc) {
        Square[][] board = getboard();
        int counter = 0;
        Square[] nulls = new Square[29];
        int row = loc[0];
        int col = loc[1];
        //Diag Right Up
        for (int i = 1;
            (row - i) >= 0 && (col + i) <= 7; i++) {
            nulls[counter] = board[row - i][col + i];
            counter++;
        }
        //Diag Right Down
        for (int i = 1;
            (row + i) <= 7 && (col + i) <= 7; i++) {
            nulls[counter] = board[row + i][col + i];
            counter++;
        }
        //Diag Left Down
        for (int i = 1;
            (row + i) <= 7 && (col - i) >= 0; i++) {
            nulls[counter] = board[row + i][col - i];
            counter++;
        }
        //Diag Left Up
        for (int i = 1;
            (row - i) >= 0 && (col - i) >= 0; i++) {
            nulls[counter] = board[row - i][col - i];
            counter++;
        }
        //up
        for (int i = row - 1; i >= 0; i--) {
            nulls[counter] = board[i][col];
            counter++;
        }
        //Right
        for (int i = col + 1; i <= 7; i++) {
            nulls[counter] = board[row][i];
            counter++;
        }
        //Down
        for (int i = row + 1; i <= 7; i++) {
            nulls[counter] = board[i][col];
            counter++;
        }
        //Left
        for (int i = col - 1; i >= 0; i--) {
            nulls[counter] = board[row][i];
            counter++;
        }
        return nulls;
    }

    /**
    *Returns an array of Squares containing
    *the squares where a Queen could move to
    *from a given square location, without nulls
    */
    public Square[] movesFrom(Square square) {
        return shorten(cleanupqueen(findLoc(square)));
    }
}