/**
* represents the abstration of all pieces in the game of chess
*
* @author zwaters3
*/
abstract class Piece {
    protected Color color;

    /**
    *takes color, WHITE or BLACK
    *and sets it to the color variable
    */
    public Piece(Color color) {
        this.color = color;
    }
    /**
    *Returns the color of peice
    */
    public Color getColor() {
        return color;
    }

    /**
    *Creates a board consisting of 64
    *square objects in order
    */
    public Square[][] getboard() {
        Square[][] board = new Square[8][8];
        int counter = 0;
        for (int col = 0; col < board.length; col++) {
            for (int row = board[col].length - 1; row >= 0; row--) {
                int z = counter + 1;
                int a = (z / 8);
                int b = (z % 8);
                if (b == 0) {
                    a = a - 1;
                    b = 8;
                }
                char y = (char) (a + 'a');
                String both = "" + y + b;
                board[row][col] = new Square(both);
                counter++;
            }
        }
        return board;
    }

    /**
    *Returns an int array containing the
    *x and y position on the getboard array
    *that contains the the square object
    */
    public int[] findLoc(Square square) {
        int[] loc = new int[2];
        Square[][] board = getboard();
        for (int row = 0; row < board.length; row++) {
            for (int col = 0; col < board[row].length; col++) {
                if (board[row][col].equals(square)) {
                    loc[0] = row;
                    loc[1] = col;
                }
            }
        }
        return loc;
    }

    /**
    *given an array of squares it will remove the nulls in it
    */
    public Square[] shorten(Square[] loc) {
        int k = 0;
        for (int i = 0; i < loc.length; i++) {
            if (loc[i] == null) {
                break;
            }
            k++;
        }
        Square[] shorty = new Square[k];
        for (int j = 0; j < shorty.length; j++) {
            shorty[j] = loc[j];
        }
        return shorty;
    }

    abstract String algebraicName();
    abstract String fenName();
    abstract Square[] movesFrom(Square square);
}