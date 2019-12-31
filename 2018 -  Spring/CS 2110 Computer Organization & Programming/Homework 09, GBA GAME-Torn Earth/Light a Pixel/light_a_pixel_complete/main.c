#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define OFFSET(r, c, rowlen) ((r)*(rowlen) + (c))

#define COLOR(r, g, b) ((r) | (g) << 5 | (b) << 10)

#define BLACK 0
#define WHITE COLOR(31,31,31)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31,0,31)
#define CYAN COLOR(0,31,31)
#define YELLOW COLOR(31,31,0)

unsigned short *videoBuffer = (unsigned short *)0x6000000;

#define SCANLINECOUNTER *(volatile short *)0x4000006

// Buttons
#define BUTTON_A      (1 << 0)
#define BUTTON_B      (1 << 1)
#define BUTTON_SELECT (1 << 2)
#define BUTTON_START  (1 << 3)
#define BUTTON_RIGHT  (1 << 4)
#define BUTTON_LEFT   (1 << 5)
#define BUTTON_UP     (1 << 6)
#define BUTTON_DOWN   (1 << 7)
#define BUTTON_R      (1 << 8)
#define BUTTON_L      (1 << 9)

#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *) 0x4000130

void delay(int num);
void setPixel(int row, int col, unsigned short color);
void drawRect(int row, int col, int height, int width, unsigned short color);
void waitForVblank();

struct ball {
  int row;
  int col;
  int rd;
  int cd;
  unsigned short color;
} ;

int main() {
  // *(unsigned short *)0x4000000 = MODE3 | BG2_ENABLE;
  REG_DISPCTL = MODE3 | BG2_ENABLE;

  //  *(videoBuffer + 0) = 0x7fff;
  //   videoBuffer[0] = 0x7fff;
  struct ball balls[] = {
    {80, 120, 1, 1, RED},
    {80, 120, 1, -1, BLUE},
    {80, 120, -1, 1, GREEN},
    {80, 120, -1, -1, WHITE},
    {30, 40, -1, -1, CYAN},
    {20, 40, -1, 1, YELLOW},
    {30, 30, 1, 1, MAGENTA}
  };

  int nballs = sizeof(balls) / sizeof(balls[0]);
  int size = 5;

  while(1) { 
    if (KEY_DOWN_NOW(BUTTON_UP)) {
      size++;
      if (size > 150) {
        size = 150;
      }
    }
    if (KEY_DOWN_NOW(BUTTON_DOWN)) {
      size--;
      if (size < 3) {
        size = 3;
      }
    }

    for (int i = 0; i < nballs; i++) {
      struct ball *bp = &balls[i];

      bp->row += bp->rd;

      balls[i].col += balls[i].cd;

      if (balls[i].row + size - 1 > 159) {
         balls[i].rd *= -1;
         balls[i].row = 159 - size + 1;
      }
      if (balls[i].row < 0) {
         balls[i].row = 0;
         balls[i].rd *= -1;
      }
      if (balls[i].col < 0) {
         balls[i].col = 0;
         balls[i].cd *= -1;
      }
      if (balls[i].col + size - 1 > 239) {
         balls[i].col = 239 - size + 1;
         balls[i].cd *= -1;
      }

      drawRect(balls[i].row, balls[i].col, size, size, balls[i].color);
    }
  
    waitForVblank();
    // delay(1);

    for (int i = 0; i < nballs; i++) {
      drawRect(balls[i].row, balls[i].col, size, size, 0);
    }

  }

  return 0;
}

void setPixel(int row, int col, unsigned short color) {
  videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color) {
  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      setPixel(row + r, col + c, color);
    }
  }
}

void delay(int num) {
   volatile int x = 0;
   for (int i = 0; i < num*8000; i++) {
      x++;
   }
}

void waitForVblank() {
  while(SCANLINECOUNTER > 160);
  while(SCANLINECOUNTER < 160);
}
