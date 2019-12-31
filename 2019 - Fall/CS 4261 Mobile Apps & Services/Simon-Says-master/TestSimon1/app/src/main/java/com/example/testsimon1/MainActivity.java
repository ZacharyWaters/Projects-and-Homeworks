package com.example.testsimon1;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import android.view.View;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import java.util.Random;
import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MainActivity extends AppCompatActivity {

    ImageButton redButton;
    ImageButton blueButton;
    ImageButton greenButton;
    ImageButton yellowButton;
    TextView timerText;
    TextView testText;
    TextView roundText;
    TextView scoreText;
    static final int numberOfButtons = 4;
    static final int redCorospondnumber = 1;
    static final int blueCorospondnumber = 2;
    static final int greenCorospondnumber = 3;
    static final int yellowCorospondnumber = 4;
    static boolean redButtonPressed = false;
    static boolean blueButtonPressed = false;
    static boolean greenButtonPressed = false;
    static boolean yellowButtonPressed = false;
    static int shortTimer = 3;
    static int longTimer = 30;
    static int simonIndex = 0;
    static int simonRounds = 0;
    public static int score = 0;
    static int[] currentSimonSequence;
    static int expectedUserButton;
    static int userIndex = 0;
    static boolean userMode = false;
    Handler testHandler = new Handler();
    Intent gameLoseIntent;

    private void enableAllButtons(){
        redButton.setEnabled(true);
        blueButton.setEnabled(true);
        greenButton.setEnabled(true);
        yellowButton.setEnabled(true);
    }

    private void disableAllButtons(){
        redButton.setEnabled(false);
        blueButton.setEnabled(false);
        greenButton.setEnabled(false);
        yellowButton.setEnabled(false);
    }

    public void pressCorrectButton(int buttonNumber){
        if(buttonNumber == redCorospondnumber){
            redButton.performClick();
        } else if(buttonNumber == blueCorospondnumber){
            blueButton.performClick();
        } else if(buttonNumber == greenCorospondnumber){
            greenButton.performClick();
        } else if(buttonNumber == yellowCorospondnumber){
            yellowButton.performClick();
        }
    }

    private Runnable shortTimerRunnable = new Runnable() {
        @Override
        public void run() {
            testHandler.removeCallbacks(testRedButtonRunnable);
            testHandler.removeCallbacks(testBlueButtonRunnable);
            testHandler.removeCallbacks(testGreenButtonRunnable);
            testHandler.removeCallbacks(testYellowButtonRunnable);
            testHandler.removeCallbacks(longTimerRunnable);
            testHandler.removeCallbacks(roundWon);
            shortTimer = shortTimer - 1;
            timerText.setText(String.valueOf(shortTimer));
            testText.setText("GET READY!");
            if(shortTimer > 0) {
                testHandler.postDelayed(shortTimerRunnable, 1000);
            }
            else{
                testText.setText("WATCH CLOSELY!");
                timerText.setVisibility(View.GONE);
                testHandler.postDelayed(simonStarter, 450);
                testHandler.removeCallbacks(shortTimerRunnable);
            }
        }
    };

    private Runnable longTimerRunnable = new Runnable() {
        @Override
        public void run() {
            testHandler.removeCallbacks(simonLooper);
            if((userMode == true)){
                longTimer = longTimer - 1;
                timerText.setText(String.valueOf(longTimer));
                if((longTimer > 0) && (userMode == true)) {
                    testHandler.postDelayed(longTimerRunnable, 1000);
                }
                else{
                    testText.setText("YOU RAN OUT OF TIME!");
                    disableAllButtons();
                    timerText.setVisibility(View.GONE);
                    gameLoseIntent =  new Intent(getBaseContext(), GameOver.class);
                    testHandler.postDelayed(gameLose, 1300);
                }
        }}
    };

    private int[] getSimonSequence(int length){
        int[] simonArray = new int[length];
        Random random = new Random();
        for(int i = 0; i < simonArray.length; i++){
            int randomInteger = random.nextInt(numberOfButtons);
            randomInteger = randomInteger + 1;
            simonArray[i] = randomInteger;
        }
        return simonArray;
    }

    private Runnable simonStarter = new Runnable() {
        @Override
        public void run() {
            testHandler.removeCallbacks(shortTimerRunnable);
            currentSimonSequence = getSimonSequence(2 + simonRounds);
            testHandler.postDelayed(simonLooper, 100);
            testHandler.removeCallbacks(simonStarter);
        }
    };



    private Runnable simonLooper = new Runnable() {
        @Override
        public void run() {
            testHandler.removeCallbacks(simonStarter);
            if(simonIndex < currentSimonSequence.length){
                int buttonToPress = currentSimonSequence[simonIndex];
                pressCorrectButton(buttonToPress);
                simonIndex = simonIndex + 1;
                testHandler.postDelayed(simonLooper, 1000);
            }else {
                testText.setText("YOUR TURN!");
                longTimer = 30;
                timerText.setVisibility(View.VISIBLE);
                timerText.setText(String.valueOf(longTimer));
                userMode = true;
                simonIndex = 0;
                userIndex = 0;
                expectedUserButton = currentSimonSequence[userIndex];
                enableAllButtons();
                testHandler.postDelayed(longTimerRunnable, 1000);
                testHandler.removeCallbacks(simonLooper);
            }
        }
    };

    public void checkUser(int corospondInt){
        //Log.d("CHECKING_USER", String.valueOf(userIndex));
        if(expectedUserButton == corospondInt){
            if(userIndex < currentSimonSequence.length) {
                score = score + 1;
                scoreText.setText("Score: " + String.valueOf(score));
                userIndex = userIndex + 1;
            }
            if(userIndex == currentSimonSequence.length){
                disableAllButtons();
                testText.setText("GOOD JOB!");
                testHandler.postDelayed(roundWon, 1100);
                testHandler.removeCallbacks(longTimerRunnable);
            } else if(userIndex < currentSimonSequence.length){
                    expectedUserButton = currentSimonSequence[userIndex];
            }
        } else {
            testText.setText("YOU MADE A MISTAKE!");
            disableAllButtons();
            timerText.setVisibility(View.GONE);
            gameLoseIntent =  new Intent(this, GameOver.class);
            testHandler.removeCallbacks(longTimerRunnable);
            testHandler.postDelayed(gameLose, 1300);
        }
    }

    private Runnable gameLose = new Runnable() {
        @Override
        public void run() {
            startActivity(gameLoseIntent);
        }
    };

    private Runnable roundWon = new Runnable() {
        @Override
        public void run() {
            shortTimer = 3;
            testHandler.removeCallbacks(longTimerRunnable);
            timerText.setText(String.valueOf(shortTimer));
            testText.setText("GET READY!");
            userMode = false;
            simonRounds = simonRounds + 1;
            roundText.setText("Round " +String.valueOf(simonRounds + 1));
            testHandler.postDelayed(shortTimerRunnable, 1000);
            testHandler.removeCallbacks(roundWon);
            }
    };



    View.OnClickListener redButtonListener = new View.OnClickListener(){
        public void  onClick  (View  v){
            redButtonPressed = true;
            redButton.setImageResource(R.drawable.testlightredbutton1);
            testHandler.postDelayed(testRedButtonRunnable, 250);
        }
    };

    private Runnable testRedButtonRunnable = new Runnable() {
        @Override
        public void run() {
            redButton.setImageResource(R.drawable.testredbutton1);
            redButtonPressed = false;
            if(userMode == true){
                checkUser(redCorospondnumber);
            }
        }
    };


    View.OnClickListener blueButtonListener = new View.OnClickListener(){
        public void  onClick  (View  v){
            blueButtonPressed = true;
            blueButton.setImageResource(R.drawable.testlightbluebutton1);
            testHandler.postDelayed(testBlueButtonRunnable, 250);
        }
    };


    private Runnable testBlueButtonRunnable = new Runnable() {
        @Override
        public void run() {
            blueButton.setImageResource(R.drawable.testbluebutton1);
            blueButtonPressed = false;
            if(userMode == true){
                checkUser(blueCorospondnumber);
            }
        }
    };

    View.OnClickListener greenButtonListener = new View.OnClickListener(){
        public void  onClick  (View  v){
            greenButtonPressed = true;
            greenButton.setImageResource(R.drawable.testlightgreenbutton1);
            testHandler.postDelayed(testGreenButtonRunnable, 250);
        }
    };


    private Runnable testGreenButtonRunnable = new Runnable() {
        @Override
        public void run() {
            greenButton.setImageResource(R.drawable.testgreenbutton1);
            greenButtonPressed = false;
            if(userMode == true){
                checkUser(greenCorospondnumber);
            }
        }
    };

    View.OnClickListener yellowButtonListener = new View.OnClickListener(){
        public void  onClick  (View  v){
            yellowButtonPressed = true;
            yellowButton.setImageResource(R.drawable.testlightyellowbutton1);
            testHandler.postDelayed(testYellowButtonRunnable, 250);
        }
    };


    private Runnable testYellowButtonRunnable = new Runnable() {
        @Override
        public void run() {
            yellowButton.setImageResource(R.drawable.testyellowbutton1);
            yellowButtonPressed = false;
            if(userMode == true){
                checkUser(yellowCorospondnumber);
            }
        }
    };



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        testText = (TextView)findViewById(R.id.textboxUnderButton);
        timerText = (TextView)findViewById(R.id.timeText);
        scoreText = (TextView)findViewById(R.id.scoreTracker);
        roundText = (TextView)findViewById(R.id.roundNumber);
        score = 0;

        redButton = (ImageButton) findViewById(R.id.redImageButton);
        redButton.setOnClickListener(redButtonListener);
        blueButton = (ImageButton) findViewById(R.id.blueImageButton);
        blueButton.setOnClickListener(blueButtonListener);
        greenButton = (ImageButton) findViewById(R.id.greenImageButton);
        greenButton.setOnClickListener(greenButtonListener);
        yellowButton = (ImageButton) findViewById(R.id.yellowImageButton);
        yellowButton.setOnClickListener(yellowButtonListener);



        disableAllButtons();
        scoreText.setText("Score: 0");
        simonRounds = 0;
        shortTimer = 3;
        userIndex = 0;
        timerText.setText(String.valueOf(shortTimer));
        testText.setText("GET READY!");
        userMode = false;
        testHandler.postDelayed(shortTimerRunnable, 1000);

    }
}
