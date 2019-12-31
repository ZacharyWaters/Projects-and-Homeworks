package com.example.testsimon1;

public class Player implements Comparable<Player>{
    String name;
    int score;
    public Player(String newname, int newscore){
        name = newname;
        score = newscore;
    }

    @Override
    public int compareTo(Player other) {
        if(this.getScore() < other.getScore())
            return 1;
        else if (this.getScore() == other.getScore())
            return 0 ;
        return -1 ;
    }

    public int getScore(){
        return score;
    }

    public String getName(){
        return name;
    }
}
