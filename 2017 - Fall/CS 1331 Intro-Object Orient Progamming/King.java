/**
* represents the King piece
*
* @author zwaters3
*/
public class King extends Piece {
    public King(Color color) {
        super(color);
    }
    /**
    *Returns a string containing the
    *algebraic letter for King
    */
    public String algebraicName() {
        return "K";
    }
    /**
    *Returns a string containing the fen
    *name of King for both white and black
    */
    public String fenName() {
        if (color == Color.BLACK) {
            return "k";
        } else {
            return "K";
        }
    }

    /**
    *Returns an array of Squares containing
    *the squares where a King could move to
    *from a given square location, with nulls
    */
    public Square[] cleanupking(int[] loc) {
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
            break;
        }
        //Diag Right Down
        for (int i = 1;
            (row + i) <= 7 && (col + i) <= 7; i++) {
            nulls[counter] = board[row + i][col + i];
            counter++;
            break;
        }
        //Diag Left Down
        for (int i = 1;
            (row + i) <= 7 && (col - i) >= 0; i++) {
            nulls[counter] = board[row + i][col - i];
            counter++;
            break;
        }
        //Diag Left Up
        for (int i = 1;
            (row - i) >= 0 && (col - i) >= 0; i++) {
            nulls[counter] = board[row - i][col - i];
            counter++;
            break;
        }
        //up
        for (int i = row - 1; i >= 0; i--) {
            nulls[counter] = board[i][col];
            counter++;
            break;
        }
        //Right
        for (int i = col + 1; i <= 7; i++) {
            nulls[counter] = board[row][i];
            counter++;
            break;
        }
        //Down
        for (int i = row + 1; i <= 7; i++) {
            nulls[counter] = board[i][col];
            counter++;
            break;
        }
        //Left
        for (int i = col - 1; i >= 0; i--) {
            nulls[counter] = board[row][i];
            counter++;
            break;
        }
        return nulls;
    }

    /**
    *Returns an array of Squares containing
    *the squares where a King could move to
    *from a given square location, without nulls
    */
    public Square[] movesFrom(Square square) {
        return shorten(cleanupking(findLoc(square)));
    }
}