import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class PgnReader {

    private static int[][] board = new int[8][8];

    public static int[] locationfrommove(String move) {
        int[] locA = new int[6];
        if (move.contains("O-O")) {
            locA = new int[] {0, 0, 0, 0, 0, 0 };
            return locA;
        }
        int i = 0;
        boolean brakes = false; // brakes are where for loops with break; used
                                // to be
        while (!brakes) { // The magic while loop
            if ((!(move.substring(move.length() - 1, move.length()))
                    .matches(".*\\d+.*"))) {
                if ((move.substring(move.length() - 1, move.length()))
                        .matches("[a-zA-Z]+")) {
                    locA[3] = stringtopiece((move.substring(move.length() - 1,
                            move.length())));
                } // pawnpromotion
                move = move.substring(0, move.length() - 1);
                i++;
            } else if ((move.substring(move.length() - 1, move.length()))
                    .matches(".*\\d+.*")) {
                brakes = true;
            }
        }
        locA[4] = 9;
        locA[5] = 9;
        String lead = move.substring(0, move.length() - 2);
        if (lead.matches(".*\\d+.*")) {
            if (lead.contains("1")) {
                locA[4] = (9 - 1) - 1;
            }
            if (lead.contains("2")) {
                locA[4] = (9 - 2) - 1;
            }
            if (lead.contains("3")) {
                locA[4] = (9 - 3) - 1;
            }
            if (lead.contains("4")) {
                locA[4] = (9 - 4) - 1;
            }
            if (lead.contains("5")) {
                locA[4] = (9 - 5) - 1;
            }
            if (lead.contains("6")) {
                locA[4] = (9 - 6) - 1;
            }
            if (lead.contains("7")) {
                locA[4] = (9 - 7) - 1;
            }
            if (lead.contains("8")) {
                locA[4] = (9 - 9) - 1;
            }
        }
        if ((lead.matches(".*[A-Z]+.*"))
                && (lead.matches(".*[a|b|c|d|e|f|g|h].*"))) {
            if (lead.contains("a")) {
                locA[5] = 0;
            }
            if (lead.contains("b")) {
                locA[5] = 1;
            }
            if (lead.contains("c")) {
                locA[5] = 2;
            }
            if (lead.contains("d")) {
                locA[5] = 3;
            }
            if (lead.contains("e")) {
                locA[5] = 4;
            }
            if (lead.contains("f")) {
                locA[5] = 5;
            }
            if (lead.contains("g")) {
                locA[5] = 6;
            }
            if (lead.contains("h")) {
                locA[5] = 7;
            }
        }

        String values = move.substring(move.length() - 2, move.length());
        // Its values that gets cut, not move
        char letter = values.charAt(0);
        int x = letter - 'a';
        locA[1] = x;
        int y = Integer.parseInt(values.substring(values.length() - 1));
        y = (9 - y) - 1;
        locA[0] = y;
        if (move.contains("x") && (piecefrommove(move, 0) == 1)) {
            char letter2 = move.charAt(0);
            int z = letter2 - 'a';
            locA[2] = z;
        } else {
            locA[2] = 9;
        }
        return locA;
    }

    public static int pinnedk(int piece, int row, int col) {
        // diagonal left up
        int kingspin = 0;
        int i = 1;
        int color;
        if (piece > 0) {
            color = 9;
        } else {
            color = -9;
        }
        boolean brakes = false;
        while (!brakes && i < (row + 1) && i < (col + 1)) {
            if (((board[row - i][col - i]) != 0)
                    && ((board[row - i][col - i]) != color)) {
                brakes = true;
            } else if ((board[row - i][col - i]) == color) {
                brakes = true;
                kingspin = 1;
            }
            i++;
        }
        // diagonal right down
        i = 1;
        brakes = false;
        while (!brakes && (i < (8 - row)) && (i < (8 - col))) {
            if (((board[row + i][col + i]) != 0)
                    && ((board[row + i][col + i]) != color)) {
                brakes = true;
            } else if ((board[row + i][col + i]) == color) {
                brakes = true;
                kingspin = 5;
            }
            i++;
        }
        // diagonal Right Up
        i = 1;
        brakes = false;
        while (!brakes && (i < row + 1) && (i < (8 - col))) {
            if (((board[row - i][col + i]) != 0)
                    && ((board[row - i][col + i]) != color)) {
                brakes = true;
            } else if ((board[row - i][col + i]) == color) {
                brakes = true;
                kingspin = 3;
            }
            i++;
        }
        // diagonal Left Down
        i = 1;
        brakes = false;
        while (!brakes && (i < (8 - row)) && (i < col + 1)) {
            if (((board[row + i][col - i]) != 0)
                    && ((board[row + i][col - i]) != color)) {
                brakes = true;
            } else if ((board[row + i][col - i]) == color) {
                brakes = true;
                kingspin = 7;
            }
            i++;
        }
        // right
        i = 1;
        brakes = false;
        while (!brakes && i < (8 - row)) {
            if (((board[row + i][col]) != 0)
                    && ((board[row + i][col]) != color)) {
                brakes = true;
            } else if ((board[row + i][col]) == color) {
                brakes = true;
                kingspin = 4;
            }
            i++;
        }
        // down
        i = 1;
        brakes = false;
        while (!brakes && (i < (8 - col))) {
            if (((board[row][col + i]) != 0)
                    && ((board[row][col + i]) != color)) {
                brakes = true;
            } else if ((board[row][col + i]) == color) {
                brakes = true;
                kingspin = 6;
            }
            i++;
        }
        // left
        i = 1;
        brakes = false;
        while (!brakes && (i < (row + 1))) {
            if (((board[row - i][col]) != 0)
                    && ((board[row - i][col]) != color)) {
                brakes = true;
            } else if ((board[row - i][col]) == color) {
                brakes = true;
                kingspin = 8;
            }
            i++;
        }
        // up
        i = 1;
        brakes = false;
        while (!brakes && (i < (col + 1))) {
            if (((board[row][col - i]) != 0)
                    && ((board[row][col - i]) != color)) {
                brakes = true;
            } else if ((board[row][col - i]) == color) {
                brakes = true;
                kingspin = 2;
            }
            i++;
        }
        return kingspin;
    }

    public static boolean ispinned(int piece, int row, int col) {
        boolean t = false;
        boolean brakes = false;
        int a, b, c, i;
        if (piece > 0) {
            a = -8;
            b = -4;
            c = -2;
        } else {
            a = 8;
            b = 4;
            c = 2;
        }
        i = 1;
        int k = pinnedk(piece, row, col);
        if (k == 0) { // None
            return false;
        }
        if (k == 1) { // diagonal Right Down
            i = 1;
            brakes = false;
            while (!brakes && (i < (8 - row)) && (i < (8 - col))) {
                int z = (board[row + i][col + i]);
                if ((z != 0) && (z != a) && (z != c)) {
                    return false;
                } else if ((z == a) || (z == c)) {
                    brakes = true;
                    return true;
                }
                i++;
            }
        }
        if (k == 2) { // down
            i = 1;
            brakes = false;
            while (!brakes && (i < (8 - col))) {
                int z = (board[row][col + i]);
                if ((z != 0) && (z != a) && (z != b)) {
                    return false;
                } else if ((z == a) || (z == b)) {
                    brakes = true;
                    return true;
                }
                i++;
            }
        }
        if (k == 3) { // diagonal Left Down
            i = 1;
            brakes = false;
            while (!brakes && (i < (8 - row)) && (i < (1 + col))) {
                int z = (board[row + i][col - i]);
                if ((z != 0) && (z != a) && (z != c)) {
                    return false;
                } else if ((z == a) || (z == c)) {
                    brakes = true;
                    return true;
                }
                i++;
            }
        }
        if (k == 4) { // Left
            i = 1;
            brakes = false;
            while (!brakes && (i < (1 + row))) {
                int z = (board[row - i][col]);
                if ((z != 0) && (z != a) && (z != b)) {
                    return false;
                } else if ((z == a) || (z == b)) {
                    brakes = true;
                    return true;
                }
                i++;
            }
        }
        if (k == 5) { // diagonal Left Up
            i = 1;
            brakes = false;
            while (!brakes && (i < (1 + row)) && (i < (1 + col))) {
                int z = (board[row - i][col - i]);
                if ((z != 0) && (z != a) && (z != c)) {
                    return false;
                } else if ((z == a) || (z == c)) {
                    brakes = true;
                    return true;
                }
                i++;
            }
        }
        if (k == 6) { // Up
            i = 1;
            brakes = false;
            while (!brakes && (i < (1 + col))) {
                int z = (board[row][col - i]);
                if ((z != 0) && (z != a) && (z != b)) {
                    return false;
                } else if ((z == a) || (z == b)) {
                    brakes = true;
                    return true;
                }
                i++;
            }
        }
        if (k == 7) { // diagonal Right Up
            i = 1;
            brakes = false;
            while (!brakes && (i < (1 + row)) && (i < (8 - col))) {
                int z = (board[row - i][col + i]);
                if ((z != 0) && (z != a) && (z != c)) {
                    return false;
                } else if ((z == a) || (z == c)) {
                    brakes = true;
                    return true;
                }
                i++;
            }
        }
        if (k == 8) { // Right
            i = 1;
            brakes = false;
            while (!brakes && (i < (8 - row))) {
                int z = (board[row + i][col]);
                if ((z != 0) && (z != a) && (z != b)) {
                    return false;
                } else if ((z == a) || (z == b)) {
                    brakes = true;
                    return true;
                }
                i++;
            }
        }
        return t;
    }

    public static void generateboard() {
        for (int col = 0; col < board.length; col++) {
            for (int row = 0; row < board[col].length; row++) {
                if ((col == 0 || col == 7) && row == 0) { // black rooks
                    board[row][col] = -4;
                } else if ((col == 1 || col == 6) && row == 0) { // black
                                                                    // knights
                    board[row][col] = -3;
                } else if ((col == 2 || col == 5) && row == 0) { // black
                                                                    // bishops
                    board[row][col] = -2;
                } else if (col == 3 && row == 0) { // black Queen
                    board[row][col] = -8;
                } else if (col == 4 && row == 0) { // black king
                    board[row][col] = -9;
                } else if (row == 1) { // black pawns
                    board[row][col] = -1;
                } else if (row == 6) { // white pawns
                    board[row][col] = 1;
                } else if ((col == 0 || col == 7) && row == 7) { // white rooks
                    board[row][col] = 4;
                } else if ((col == 1 || col == 6) && row == 7) { // white
                                                                    // knights
                    board[row][col] = 3;
                } else if ((col == 2 || col == 5) && row == 7) { // white
                                                                    // bishops
                    board[row][col] = 2;
                } else if (col == 3 && row == 7) { // white queen
                    board[row][col] = 8;
                } else if (col == 4 && row == 7) { // white king
                    board[row][col] = 9;
                } else { // Empty Space
                    board[row][col] = 0;
                }
            }
        }
    }

    public static String getfinalboardlayout() {
        String boardstring = "";
        int counter = 0;
        for (int row = 0; row < board.length; row++) {
            for (int col = 0; col < board[row].length; col++) {
                if (board[row][col] == 0) {
                    counter++;
                } else if (board[row][col] != 0) {
                    if (counter != 0) {
                        boardstring = boardstring + counter;
                        counter = 0;
                    }
                    boardstring = boardstring + piecetranslate(board[row][col]);
                }
            }
            if (counter != 0) {
                boardstring = boardstring + counter;
            }
            boardstring = boardstring + "/";
            counter = 0;
        }
        return boardstring;
    }

    public static String piecetranslate(int x) {
        if (x == 1) {
            return "P";
        } else if (x == -1) {
            return "p";
        } else if (x == -2) {
            return "b";
        } else if (x == -3) {
            return "n";
        } else if (x == -4) {
            return "r";
        } else if (x == -8) {
            return "q";
        } else if (x == -9) {
            return "k";
        } else if (x == 2) {
            return "B";
        } else if (x == 3) {
            return "N";
        } else if (x == 4) {
            return "R";
        } else if (x == 8) {
            return "Q";
        } else if (x == 9) {
            return "K";
        } else {
            return null;
        }
    }

    public static int stringtopiece(String value) { // pawn promotion
        if (value.contains("Q")) {
            return 8;
        } else if (value.contains("q")) {
            return 8;
        } else if (value.contains("R")) {
            return 4;
        } else if (value.contains("r")) {
            return 4;
        } else if (value.contains("N")) {
            return 3;
        } else if (value.contains("n")) {
            return 3;
        } else if (value.contains("B")) {
            return 2;
        } else if (value.contains("b")) {
            return 2;
        } else {
            return 0;
        }
    }

    public static String[] gathermoves(String game) {
        String[] arraylist1 = game.split("\n");
        boolean check = false;
        int blankline = -1;
        for (int i = 0; i < arraylist1.length; i++) {
            if (arraylist1[i].equals("")) {
                blankline = i;
            }
        }

        String stringlist2 = "";
        for (int i = blankline + 1; i < arraylist1.length; i++) {
            stringlist2 = stringlist2 + " " + arraylist1[i];
        }
        String[] arraylist3 = stringlist2.split("\\s+");
        String[] arraylist4 = new String[arraylist3.length - 1];
        for (int i = 1; i < arraylist3.length; i++) {
            arraylist4[i - 1] = arraylist3[i];
        }
        String stringlist5 = "";
        for (int i = 1; i < arraylist4.length; i++) {
            if ((i % 3) != 0) {
                stringlist5 = stringlist5 + (arraylist4[i]) + " ";
            }
        }
        String[] arraylist6 = stringlist5.split(" ");
        String stringlist7 = "";
        for (int i = 0; i < arraylist6.length; i++) {
            if ((arraylist6[i].matches(".*[a-z].*"))
                    || (arraylist6[i].matches(".*O.*"))) {
                stringlist7 = stringlist7 + arraylist6[i] + " ";
            }
        }
        String[] arraylist8 = stringlist7.split("\\s+");
        return arraylist8;
    }

    public static void placepiece(int piece, int[] location) {
        if (Math.abs(piece) == 5 || Math.abs(piece) == 6) {
            if (piece == 5) {
                board[7][5] = 4;
                board[7][6] = 9;
                board[7][4] = 0;
                board[7][7] = 0;
            } else if (piece == -5) {
                board[0][6] = -9;
                board[0][5] = -4;
                board[0][4] = 0;
                board[0][7] = 0;
            } else if (piece == 6) {
                board[7][3] = 4;
                board[7][2] = 9;
                board[7][4] = 0;
                board[7][0] = 0;
            } else if (piece == -6) {
                board[0][2] = -9;
                board[0][3] = -4;
                board[0][4] = 0;
                board[0][0] = 0;
            }
        } else {
            int a = location[0];
            int b = location[1];
            if (location[3] == 0) { // if not pawn promotion
                board[a][b] = piece;
            } else { // pawn promotion
                if (piece == 1) {
                    board[a][b] = location[3]; // white pawn promotion
                } else if (piece == -1) {
                    board[a][b] = -1 * location[3]; // black pawn promotion
                }
            }
        }
    }

    public static int piecefrommove(String movestring, int count) {
        int piece;
        char letter = movestring.charAt(0);
        int size = movestring.length();
        if (letter == 'B') {
            piece = 2;
        } else if (letter == 'N') {
            piece = 3;
        } else if (letter == 'R') {
            piece = 4;
        } else if (letter == 'O' && size == 3) {
            piece = 5;
        } else if (letter == 'O' && size == 5) {
            piece = 6;
        } else if (letter == 'Q') {
            piece = 8;
        } else if (letter == 'K') {
            piece = 9;
        } else {
            piece = 1;
        }
        if ((count % 2) == 1) {
            piece = piece * (-1);
        }
        return piece;
    }

    public static void cleanupbishop(int color, int[] loc) {
        boolean dis = false;
        if ((!(loc[4] == 9)) || (!(loc[5] == 9))) {
            dis = true;
        }
        int row = loc[0];
        int col = loc[1];
        int i = 1;
        // diagonal left up
        boolean brakes1 = false;
        while (!brakes1 && i < (row + 1) && i < (col + 1)) {
            if (((board[row - i][col - i]) != 0)
                    && ((board[row - i][col - i]) != color)) {
                brakes1 = true;
            } else if ((board[row - i][col - i]) == color) {
                if (!ispinned(color, (row - i), (col - i))) {
                    if ((dis)
                            && (((row - i) == loc[4]) || (col - i) == loc[5])) {
                        board[row - i][col - i] = 0;
                        brakes1 = true;
                    } else if (!dis) {
                        board[row - i][col - i] = 0;
                        brakes1 = true;
                    }
                }
            }
            i++;
        }
        // diagonal right down
        i = 1;
        brakes1 = false;
        while (!brakes1 && (i < (8 - row)) && (i < (8 - col))) {
            if (((board[row + i][col + i]) != 0)
                    && ((board[row + i][col + i]) != color)) {
                brakes1 = true;
            } else if ((board[row + i][col + i]) == color) {
                if (!ispinned(color, (row + i), (col + i))) {
                    if ((dis)
                            && (((row + i) == loc[4]) || (col + i) == loc[5])) {
                        board[row + i][col + i] = 0;
                        brakes1 = true;
                    } else if (!dis) {
                        board[row + i][col + i] = 0;
                        brakes1 = true;
                    }
                }
            }
            i++;
        }
        // diagonal Right Up
        i = 1;
        brakes1 = false;
        while (!brakes1 && (i < row + 1) && (i < (8 - col))) {
            if (((board[row - i][col + i]) != 0)
                    && ((board[row - i][col + i]) != color)) {
                brakes1 = true;
            } else if ((board[row - i][col + i]) == color) {
                if (!ispinned(color, (row - i), (col + i))) {
                    if ((dis)
                            && (((row - i) == loc[4]) || (col + i) == loc[5])) {
                        board[row - i][col + i] = 0;
                        brakes1 = true;
                    } else if (!dis) {
                        board[row - i][col + i] = 0;
                        brakes1 = true;
                    }
                }
            }
            i++;
        }
        // diagonal Left Down
        i = 1;
        brakes1 = false;
        while (!brakes1 && (i < (8 - row)) && (i < col + 1)) {
            if (((board[row + i][col - i]) != 0)
                    && ((board[row + i][col - i]) != color)) {
                brakes1 = true;
            } else if ((board[row + i][col - i]) == color) {
                if (!ispinned(color, (row + i), (col - i))) {
                    if ((dis)
                            && (((row + i) == loc[4]) || (col - i) == loc[5])) {
                        board[row + i][col - i] = 0;
                        brakes1 = true;
                    } else if (!dis) {
                        board[row + i][col - i] = 0;
                        brakes1 = true;
                    }
                }
            }
            i++;
        }
    }

    public static void cleanuprook(int color, int[] loc) {
        boolean dis = false;
        if ((!(loc[4] == 9)) || (!(loc[5] == 9))) {
            dis = true;
        }
        int row = loc[0];
        int col = loc[1];
        // right
        int i = 1;
        boolean brakes = false;
        while (!brakes && i < (8 - row)) {
            if (((board[row + i][col]) != 0)
                    && ((board[row + i][col]) != color)) {
                brakes = true;
            } else if ((board[row + i][col]) == color) {
                if (!ispinned(color, (row + i), (col))) {
                    if ((dis)
                            && (((row + i) == loc[4]) || (col) == loc[5])) {
                        board[row + i][col] = 0;
                        brakes = true;
                    } else if (!dis) {
                        board[row + i][col] = 0;
                        brakes = true;
                    }
                }
            }
            i++;
        }
        // down
        i = 1;
        brakes = false;
        while (!brakes && (i < (8 - col))) {
            if (((board[row][col + i]) != 0)
                    && ((board[row][col + i]) != color)) {
                brakes = true;
            } else if ((board[row][col + i]) == color) {
                if (!ispinned(color, (row), (col + i))) {
                    if ((dis)
                            && (((row) == loc[4]) || (col + i) == loc[5])) {
                        board[row][col + i] = 0;
                        brakes = true;
                    } else if (!dis) {
                        board[row][col + i] = 0;
                        brakes = true;
                    }
                }
            }
            i++;
        }
        // left
        i = 1;
        brakes = false;
        while (!brakes && (i < (row + 1))) {
            if (((board[row - i][col]) != 0)
                    && ((board[row - i][col]) != color)) {
                brakes = true;
            } else if ((board[row - i][col]) == color) {
                if (!ispinned(color, (row - i), (col))) {
                    if ((dis)
                            && (((row - i) == loc[4]) || (col) == loc[5])) {
                        board[row - i][col] = 0;
                        brakes = true;
                    } else if (!dis) {
                        board[row - i][col] = 0;
                        brakes = true;
                    }
                }
            }
            i++;
        }
        // up
        i = 1;
        brakes = false;
        while (!brakes && (i < (col + 1))) {
            if (((board[row][col - i]) != 0)
                    && ((board[row][col - i]) != color)) {
                brakes = true;
            } else if ((board[row][col - i]) == color) {
                if (!ispinned(color, (row), (col - i))) {
                    if ((dis)
                            && (((row) == loc[4]) || (col - i) == loc[5])) {
                        board[row][col - i] = 0;
                        brakes = true;
                    } else if (!dis) {
                        board[row][col - i] = 0;
                        brakes = true;
                    }
                }
            }
            i++;
        }
    }

    public static void cleanupknight(int color, int[] loc) {
        boolean dis = false;
        if ((!(loc[4] == 9)) || (!(loc[5] == 9))) {
            dis = true;
        }
        boolean brakes = false;
        int i;
        int row = loc[0];
        int col = loc[1];
        int[] a = {row + 2, col + 1 };
        int[] b = {row + 1, col + 2 };
        int[] c = {row + 1, col - 2 };
        int[] d = {row + 2, col - 1 };
        int[] e = {row - 2, col - 1 };
        int[] f = {row - 1, col - 2 };
        int[] g = {row - 2, col + 1 };
        int[] h = {row - 1, col + 2 };
        int[][] kmoves = {a, b, c, d, e, f, g, h };
        if (!dis) {
            i = 0;
            brakes = false;
            while (!brakes) {
                if (((kmoves[i])[0]) >= 0 && ((kmoves[i])[0]) < 8
                        && ((kmoves[i])[1]) >= 0 && ((kmoves[i])[1]) < 8) {
                    if ((board[(kmoves[i][0])][(kmoves[i][1])]) == color) {
                        if (!ispinned(color, (kmoves[i][0]), (kmoves[i][1]))) {
                            board[kmoves[i][0]][kmoves[i][1]] = 0;
                            brakes = true;
                        }
                    }
                }
                i++;
            }
        }
        if (dis) {
            i = 0;
            brakes = false;
            while (!brakes) {
                if (((kmoves[i])[0]) >= 0 && ((kmoves[i])[0]) < 8
                        && ((kmoves[i])[1]) >= 0 && ((kmoves[i])[1]) < 8) {
                    if ((board[(kmoves[i][0])][(kmoves[i][1])]) == color) {
                        if (kmoves[i][0] == loc[4] || kmoves[i][1] == loc[5]) {
                            if (!ispinned(color, kmoves[i][0], kmoves[i][1])) {
                                board[kmoves[i][0]][kmoves[i][1]] = 0;
                                brakes = true;
                            }
                        }
                    }
                }
                i++;
            }
        }
    }

    public static void cleanupking(int color, int[] loc) {
        for (int row = 0; row < board.length; row++) {
            for (int col = 0; col < board[row].length; col++) {
                if ((board[row][col] == color)
                        && !(row == loc[0] && col == loc[1])) {
                    board[row][col] = 0;
                }
            }
        }
    }

    public static void cleanupqueen(int color, int[] loc) {
        cleanuprook(color, loc);
        cleanupbishop(color, loc);
    }

    public static void cleanuppawn(int color, int[] loc) {
        boolean isblack = (color < 0);
        boolean iscapture = (loc[2] != 9); // remember that iscapture represents
                                            // the column from where the pawn
                                            // came from: 9 is nonexstance
        int row = loc[0];
        int col = loc[1];
        int cap = loc[2];
        boolean brakes1 = false;
        boolean brakes2 = false;
        int i = 1;
        int j = 1;
        if (isblack) {
            if (iscapture) {
                board[(row - 1)][cap] = 0;
                if ((board[row - 1][col] == (-1 * color)) && (row - 1 == 4)) {
                    board[row - 1][col] = 0;
                }
            } else {
                while (!brakes1 && i < 3) {
                    if ((board[(row - i)][col]) == color) {
                        board[row - i][col] = 0;
                        brakes1 = true;
                    }
                    i++;
                }
            }
        } else {
            if (iscapture) {
                board[(row + 1)][cap] = 0;
                if ((board[row + 1][col] == (-1 * color)) && (row + 1 == 3)) {
                    board[row + 1][col] = 0;
                }
            } else {
                while (!brakes2 && j < 3) {
                    if ((board[(row + j)][col]) == color) {
                        board[row + j][col] = 0;
                        brakes2 = true;
                    }
                    j++;
                }
            }
        }
    }

    public static String tagValue(String tagName, String game) {
        String[] tags = game.split("\n");
        for (int i = 0; i < tags.length; i++) {
            if (tags[i].contains(tagName)) {
                return tags[i].substring(tags[i].indexOf('"') + 1,
                        tags[i].indexOf('"', tags[i].indexOf('"') + 1));
            }
        }
        return "Not Given";
    }

    public static void move(int piece, int[] loc) {
        placepiece(piece, loc);
        boolean ignore = false;
        if ((loc[4] != 9) && (loc[5] != 9)) {
            board[loc[4]][loc[5]] = 0;
            ignore = true;
        }
        if ((Math.abs(piece) == 1) && (!ignore)) {
            cleanuppawn(piece, loc);
        } else if ((Math.abs(piece) == 2) && (!ignore)) {
            cleanupbishop(piece, loc);
        } else if ((Math.abs(piece) == 3) && (!ignore)) {
            cleanupknight(piece, loc);
        } else if ((Math.abs(piece) == 4) && (!ignore)) {
            cleanuprook(piece, loc);
        } else if ((Math.abs(piece) == 8) && (!ignore)) {
            cleanupqueen(piece, loc);
        } else if ((Math.abs(piece) == 9) && (!ignore)) {
            cleanupking(piece, loc);
        }

    }

    public static String finalPosition(String game) {
        String[] orderedmoves = gathermoves(game);
        for (int i = 0; i < orderedmoves.length; i++) {
            move(piecefrommove(orderedmoves[i], (i)),
                    locationfrommove(orderedmoves[i]));
        }

        return getfinalboardlayout();
    }

    public static String fileContent(String path) {
        Path file = Paths.get(path);
        StringBuilder sb = new StringBuilder();
        try (BufferedReader reader = Files.newBufferedReader(file)) {
            String line = null;
            while ((line = reader.readLine()) != null) {
                sb.append(line + "\n");
            }
        } catch (IOException e) {
            System.err.format("IOException: %s%n", e);
            System.exit(1);
        }
        return sb.toString();
    }

    public static void main(String[] args) {
        String game = fileContent(args[0]);
        System.out.format("Event: %s%n", tagValue("Event", game));
        System.out.format("Site: %s%n", tagValue("Site", game));
        System.out.format("Date: %s%n", tagValue("Date", game));
        System.out.format("Round: %s%n", tagValue("Round", game));
        System.out.format("White: %s%n", tagValue("White", game));
        System.out.format("Black: %s%n", tagValue("Black", game));
        System.out.format("Result: %s%n", tagValue("Result", game));
        generateboard();
        System.out.println(finalPosition(game));
    }
}
