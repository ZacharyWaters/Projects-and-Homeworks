/**********************************************************
*ENDLESS RUNNER
*Author: Zachary Waters 
*
***********************************************************/
#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define OFFSET(r, c, rowlen) ((c) + (rowlen)*(r)) 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myLib.h"
#include "GBA_SCREEN.h"
#include "Methods.h"
#include "font.h"
#include "text.h"
#include "stdio.h"
#include "bottomb.h"
#include "Meteor.h"
#include "explosion1.h"
#include "losescreen.h"
#include "right.h"
#include "left.h"

//unsigned short *videoBuffer = (unsigned short *)0x6000000;

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

typedef struct{
int x;
int y;
int z;
int q;
}meteor;



int main() { //A
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	int x = 115;	
	int y = 68;
	int jumpanimation=0;
	int jumpsize=20;
	int baseHeight=81;
	int screenstate=0;
	int score=0;
	char scores[20];
	int explosiontime=10;
	int once = 0;	
	int facing = 0;
	meteor meteors[3];

	meteors[0].x=rand() % 241;
	meteors[0].y=0;
	meteors[0].z=1;
	meteors[0].q=explosiontime;

	meteors[1].x=rand() % 241;
	meteors[1].y=0;
	meteors[1].z=1;
	meteors[1].q=explosiontime;

	meteors[2].x=rand() % 241;
	meteors[2].y=0;
	meteors[2].z=1;
	meteors[2].q=explosiontime;

	meteors[3].x=rand() % 241;
	meteors[3].y=0;
	meteors[3].z=1;
	meteors[3].q=explosiontime;

	int meteorclock=100;
	int meteorcount=0;
	int meteormax = 4;
	int hadfallen = 0;
	//GAME LOOP
while(1){
	while(screenstate==0){
		int randseed=0;
		randseed++;
		drawImage3(0,0,GBA_SCREEN_WIDTH,GBA_SCREEN_HEIGHT,GBA_SCREEN);	
		if(KEY_DOWN_NOW(BUTTON_START)){
		srand(randseed);
		meteors[0].x=rand() % 241;
		meteors[1].x=rand() % 241;
		meteors[2].x=rand() % 241;
		meteors[3].x=rand() % 241;
		screenstate=1;
		drawRect(0,0,160,240,BLACK);
		drawImage3(baseHeight,0,BOTTOMB_WIDTH,BOTTOMB_HEIGHT,bottomb);
		x = 115;	
		y = 66;
		}
	}
	while(screenstate==1) {//B
		int landed = isGround(y, x);
		if(jumpanimation>0){//0.5	
		jumpanimation--;
				   }//0.5
		if(meteorclock>0){		//SPAWNS THE FIRST WAVE OF METEORS
		   meteorclock--;		//AFTER METEORCLOCK TIME
			if(meteorclock==0 && meteorcount!=meteormax){
				meteorclock=100;
				meteorcount++;
					  		     }


				}
		if( landed == 0){//1		CHECKS IF YOU ARE ON A CLOUD
			if(!(jumpanimation>0)){//1.1	
				y=y+1;		//CHECKS IF YOU ARE JUMPING
				landed = isGround(y, x);
					      }//1.1
		}//1
		if( landed == 0){//1.2		CHECKS IF YOU ARE ON A CLOUD
			if(!(jumpanimation>0)){//1.21	
				y=y+1;		//CHECKS IF YOU ARE JUMPING
				landed = isGround(y, x);
					      }//1.21
		}//1.2

		if(KEY_DOWN_NOW(BUTTON_UP)) {//2
		if((jumpanimation==0)&&(landed==1)){//2.1		
		jumpanimation=jumpsize; }//2.1	YOU ARE NOW JUMPING(HEIGHT=JUMPSIZE)
		if(jumpanimation>0){//2.2
			int upwall = canMoveUp(y, x);
			if(upwall==1){//2.21
				y=y-1;
				     }//2.21
		        upwall = canMoveUp(y, x);
			if(upwall==1){//2.22
				y=y-1;
				     }//2.22
				   }//2.2
					    }//2
		if(KEY_DOWN_NOW(BUTTON_DOWN)) {//3
		if(jumpanimation>0){//3.1
			jumpanimation=0;//	CANCELS THE JUMP ANIMATION
				   }//3.1	
			if(landed==0){//3.2			
				y++;
				landed = isGround(y, x);
				     }//3.2
					      }//3
		if(KEY_DOWN_NOW(BUTTON_LEFT)) {//4
		facing = 0;
		int leftwall = canMoveLeft(y, x);
			if(leftwall==1){//4.1
				x--;
					}//4.1
					    }//4
		if(KEY_DOWN_NOW(BUTTON_RIGHT)) {//5
		facing = 1;
		int rightwall = canMoveRight(y, x);
			if(rightwall==1){//5.1
				x++;
					}//5.1
					    }//5
		for(int i=0;i<meteorcount;i++){//6
			if(meteors[i].z==1){//6.1
		drawImage3(meteors[i].y,meteors[i].x,METEOR_WIDTH,METEOR_HEIGHT,Meteor);
		meteors[i].y=meteors[i].y+1;
				           }//6.1
			if(meteors[i].z==2){//6.2
		drawImage3(meteors[i].y+8,meteors[i].x,EXPLOSION1_WIDTH,EXPLOSION1_HEIGHT,explosion1);
				           }//6.2
		                            }//6	


		if(facing==0){		
		drawImage3(y,x,LEFT_WIDTH,LEFT_HEIGHT,left);}
		if(facing==1){
		drawImage3(y,x,RIGHT_WIDTH,RIGHT_HEIGHT,right);}
		
		//BELOW CHECKS TO SEE IF YOU ARE IN A STATE THAT WOULD TRIGGER A LOSS		
		int lost = lose(y,x);
		if((lost==1)||(lost==2)){
		delay(60);
		if((lost==2)){
		hadfallen=1;
		             }
		drawImage3(0,0,LOSESCREEN_WIDTH,LOSESCREEN_HEIGHT,losescreen);
		screenstate=2;
		break;
		}
		//drawRect(0,0,160,260, BLACK);		
		//drawRect(y, x, 10, 10, GREEN);

		//PRINT TEXT STUFF
		sprintf(scores, "Score:  %-10d" , score);
		drawString(0, 140, scores, CYAN);
		
		//VBLANK
		waitForVblank();
		
		//BELOW HANDLES CLEANUP OF CHARACTER
		drawRect(y, x, 13, 10, BLACK);

		//BELOW HANDLES THE CLEANING UP OF THE METEORS		
		for(int i=0;i<meteorcount;i++){//7
			if(meteors[i].z==1){//7.1
				drawRect(meteors[i].y-1,meteors[i].x,19,12,BLACK);
					if(meteortouch(meteors[i].y,meteors[i].x)){//7.11
						meteors[i].z=2;
						score++;
					   }//7.11
				           }//7.1
			if(meteors[i].z==2){//7.2
				if(meteors[i].q==0){//7.21
					drawRect(meteors[i].y+8,meteors[i].x,20,20,BLACK);
				meteors[i].x=rand() % 241;
				meteors[i].y=0;
				meteors[i].z=1;
				meteors[i].q=explosiontime;
						  }//7.21
				else{//7.22
				   meteors[i].q--;
				    }//7.22
				           }//7.2
		                            }//7		

		if(KEY_DOWN_NOW(BUTTON_SELECT)){//8
		drawRect(0,0,160,240,BLACK);		
		screenstate=0;
		score=0;
		meteorclock=100;
		meteorcount=0;
		hadfallen = 0;
		meteors[0].x=rand() % 241;
		meteors[0].y=0;
		meteors[0].z=1;
		meteors[1].x=rand() % 241;
		meteors[1].y=0;
		meteors[1].z=1;
		meteors[2].x=rand() % 241;
		meteors[2].y=0;
		meteors[2].z=1;
		meteors[3].x=rand() % 241;
		meteors[3].y=0;
		meteors[3].z=1;
						}//8
		
		              }//B
	//GAME OVER SCREEN
	while(screenstate==2){//C
		//PRINT FINAL SCORE
		if((score>0)&&(once==0)){
			score=score - 1;
			score=score+hadfallen;
			once = 1;
					}
		sprintf(scores, "FINAL SCORE:  %-10d" , score);
		drawString(60, 70, scores, WHITE);
		meteorclock=100;
		meteorcount=0;
		hadfallen = 0;
		meteors[0].x=rand() % 241;
		meteors[0].y=0;
		meteors[0].z=1;
		meteors[1].x=rand() % 241;
		meteors[1].y=0;
		meteors[1].z=1;
		meteors[2].x=rand() % 241;
		meteors[2].y=0;
		meteors[2].z=1;
		meteors[3].x=rand() % 241;
		meteors[3].y=0;
		meteors[3].z=1;
		if( (KEY_DOWN_NOW(BUTTON_SELECT)) || (KEY_DOWN_NOW(BUTTON_START))
		  || (KEY_DOWN_NOW(BUTTON_LEFT)) || (KEY_DOWN_NOW(BUTTON_RIGHT)) 
                  || (KEY_DOWN_NOW(BUTTON_UP))  || (KEY_DOWN_NOW(BUTTON_DOWN)) ){//CA
		score=0;
		drawRect(0,0,160,240,BLACK);		
		screenstate=0;
						}//CA
			     }//C
}

}//A
