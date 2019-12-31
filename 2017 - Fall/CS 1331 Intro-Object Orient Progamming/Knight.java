/**
* represents the Knight piece
*
* @author zwaters3
*/
public class Knight extends Piece {
    public Knight(Color color) {
        super(color);
    }
    /**
    *Returns a string containing the
    *algebraic letter for Knight
    */
    public String algebraicName() {
        return "N";
    }
    /**
    *Returns a string containing the fen
    *name of Knight for both white and black
    */
    public String fenName() {
        if (color == Color.BLACK) {
            return "n";
        } else {
            return "N";
        }
    }

    /**
    *Returns an array of Squares containing
    *the squares where a Knight could move to
    *from a given square location, with nulls
    */
    public Square[] cleanupknight(int[] loc) {
        Square[][] board = getboard();
        int counter = 0;
        Square[] nulls = new Square[29];
        int row = loc[0];
        int col = loc[1];
        int[] a = {
            row + 2,
            col + 1
        };
        int[] b = {
            row + 1,
            col + 2
        };
        int[] c = {
            row + 1,
            col - 2
        };
        int[] d = {
            row + 2,
            col - 1
        };
        int[] e = {
            row - 2,
            col - 1
        };
        int[] f = {
            row - 1,
            col - 2
        };
        int[] g = {
            row - 2,
            col + 1
        };
        int[] h = {
            row - 1,
            col + 2
        };
        int[][] kmoves = {
            a,
            b,
            c,
            d,
            e,
            f,
            g,
            h
        };
        for (int i = 0; i < kmoves.length; i++) {
            if (((kmoves[i])[0]) >= 0 && ((kmoves[i])[0]) < 8
                && ((kmoves[i])[1]) >= 0 && ((kmoves[i])[1]) < 8) {
                nulls[counter] = board[kmoves[i][0]][kmoves[i][1]];
                counter++;
            }
        }
        return nulls;
    }

    /**
    *Returns an array of Squares containing
    *the squares where a Knight could move to
    *from a given square location, without nulls
    */
    public Square[] movesFrom(Square square) {
        return shorten(cleanupknight(findLoc(square)));
    }
}
