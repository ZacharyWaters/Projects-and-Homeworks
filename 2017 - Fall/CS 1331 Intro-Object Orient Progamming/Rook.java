/**
* represents the Rook piece
*
* @author zwaters3
*/
public class Rook extends Piece {
    public Rook(Color color) {
        super(color);
    }
    /**
    *Returns a string containing the
    *algebraic letter for Rook
    */
    public String algebraicName() {
        return "R";
    }
    /**
    *Returns a string containing the fen
    *name of Rook for both white and black
    */
    public String fenName() {
        if (color == Color.BLACK) {
            return "r";
        } else {
            return "R";
        }
    }

    /**
    *Returns an array of Squares containing
    *the squares where a Rook could move to
    *from a given square location, with nulls
    */
    public Square[] cleanuprook(int[] loc) {
        Square[][] board = getboard();
        int counter = 0;
        Square[] nulls = new Square[29];
        int row = loc[0];
        int col = loc[1];
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
    *the squares where a Rook could move to
    *from a given square location, without nulls
    */
    public Square[] movesFrom(Square square) {
        return shorten(cleanuprook(findLoc(square)));
    }
}