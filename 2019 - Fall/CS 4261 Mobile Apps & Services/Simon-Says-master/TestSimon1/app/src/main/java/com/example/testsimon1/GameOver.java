package com.example.testsimon1;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;
import java.util.Collections;

public class GameOver extends AppCompatActivity {

    TextView scoreText;
    public static String userName;
    TextView userInput;
    FirebaseDatabase database = FirebaseDatabase.getInstance();
    DatabaseReference myRef = database.getReference("Users&Scores");
    public static ArrayList<Player> playerArrayList = new ArrayList<Player>();

    public void parseData(DataSnapshot dataSnapshot){
        playerArrayList.clear();
        for(DataSnapshot ds: dataSnapshot.getChildren()){
            String unparsedData = ds.child("User|Score").getValue().toString();
            int cutIndex = unparsedData.indexOf('|');
            String name = unparsedData.substring(0, cutIndex);
            String score = unparsedData.substring(cutIndex + 1, unparsedData.length());
            playerArrayList.add(new Player(name,Integer.parseInt(score)));
        }
        Collections.sort(playerArrayList);
    }



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game_over);

        userInput = (TextView)findViewById(R.id.editText);
        scoreText = (TextView)findViewById(R.id.yourScore);
        scoreText.setText("YOUR SCORE: " +String.valueOf(MainActivity.score));

        Button continueNormal = (Button) findViewById(R.id.button);
        continueNormal.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                userName = userInput.getText().toString();
                String databaseEntry = userName+"|"+String.valueOf(MainActivity.score);
                myRef.push().child("User|Score").setValue(databaseEntry);
                Intent myIntent = new Intent(view.getContext(), highscore.class);
                startActivityForResult(myIntent, 0);
            }
        });

        myRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                parseData(dataSnapshot);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });


        Button continueWithout = (Button) findViewById(R.id.button2);
        continueWithout.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                Intent myIntent = new Intent(view.getContext(), highscore.class);
                startActivityForResult(myIntent, 0);
            }

        });
    }

}
