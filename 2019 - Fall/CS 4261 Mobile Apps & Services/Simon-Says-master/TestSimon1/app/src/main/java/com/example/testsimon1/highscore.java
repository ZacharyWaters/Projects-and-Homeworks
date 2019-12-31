package com.example.testsimon1;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.ArrayList;

public class highscore extends AppCompatActivity {

    TextView name1;
    TextView name2;
    TextView name3;
    TextView name4;
    TextView name5;
    TextView name6;
    TextView name7;
    TextView name8;
    TextView name9;
    TextView score1;
    TextView score2;
    TextView score3;
    TextView score4;
    TextView score5;
    TextView score6;
    TextView score7;
    TextView score8;
    TextView score9;

    public void setmatching(String inputName, int inputScore, int index){
        String inputScoreString = String.valueOf(inputScore);
        if(index == 0){
            name1.setText(inputName);
            score1.setText(inputScoreString);
        } else if(index == 1){
            name2.setText(inputName);
            score2.setText(inputScoreString);
        } else if(index == 2){
            name3.setText(inputName);
            score3.setText(inputScoreString);
        } else if(index == 3){
            name4.setText(inputName);
            score4.setText(inputScoreString);
        } else if(index == 4){
            name5.setText(inputName);
            score5.setText(inputScoreString);
        } else if(index == 5){
            name6.setText(inputName);
            score6.setText(inputScoreString);
        } else if(index == 6){
            name7.setText(inputName);
            score7.setText(inputScoreString);
        } else if(index == 7){
            name8.setText(inputName);
            score8.setText(inputScoreString);
        } else if(index == 8){
            name9.setText(inputName);
            score9.setText(inputScoreString);
        }
    }



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_highscore);
        Button restartButton = (Button) findViewById(R.id.button3);
        name1 = (TextView) findViewById(R.id.namecolHeader);
        name2 = (TextView) findViewById(R.id.namecolHeader3);
        name3 = (TextView) findViewById(R.id.namecolHeader4);
        name4 = (TextView) findViewById(R.id.namecolHeader5);
        name5 = (TextView) findViewById(R.id.namecolHeader6);
        name6 = (TextView) findViewById(R.id.namecolHeader7);
        name7 = (TextView) findViewById(R.id.namecolHeader8);
        name8 = (TextView) findViewById(R.id.namecolHeader9);
        name9 = (TextView) findViewById(R.id.namecolHeader10);
        score1 = (TextView) findViewById(R.id.textView4);
        score2 = (TextView) findViewById(R.id.textView5);
        score3 = (TextView) findViewById(R.id.textView6);
        score4 = (TextView) findViewById(R.id.textView7);
        score5 = (TextView) findViewById(R.id.textView3);
        score6 = (TextView) findViewById(R.id.textView8);
        score7 = (TextView) findViewById(R.id.textView9);
        score8 = (TextView) findViewById(R.id.textView13);
        score9 = (TextView) findViewById(R.id.textView10);

        ArrayList<Player> newPlayerArray = GameOver.playerArrayList;
        for(int i=0; i < newPlayerArray.size(); i++){
            Player x = newPlayerArray.get(i);
            int playerScore = x.getScore();
            String playerName = x.getName();
            setmatching(playerName,playerScore,i);
        }



        restartButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
            Intent myIntent = new Intent(view.getContext(), startScreen.class);
            startActivityForResult(myIntent, 0);
            }

        });
    }
}
