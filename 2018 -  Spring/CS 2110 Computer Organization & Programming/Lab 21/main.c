// Make a simple GBA state machine
// Name: YOUR NAME HERE

#include <stdbool.h>

#include "myLib.h"
// These pictures are available as the variables snoop1, snoop2, etc.
// Peep the img/ directory if you're nosy.
#include "img/snoop1.h"
#include "img/snoop2.h"
#include "img/snoop3.h"
#include "img/snoop4.h"
#include "img/snoop5.h"
#include "img/snoop6.h"

// We gave this to you because we're really nice. Do NOT implement them
// or you'll get linker errors galore on compilation.
void waitForVBlank();
void drawFullscreenImage3(const unsigned short *image);

volatile unsigned short *videoBuffer = (unsigned short *)0x6000000;

// State enum definition
enum GBAState {
	STATE_START,
    STATE_SNOOP1,
    STATE_SNOOP2,
    STATE_SNOOP3,
    STATE_SNOOP4,
    STATE_SNOOP5,
    STATE_SNOOP6,
};

int main() {
	REG_DISPCNT = MODE3 | BG2_ENABLE;

	enum GBAState state = STATE_START;

    // TODO: Use this somehow in the states below. When you press and
    //       hold A (the z key in the emulator we recommend), it
    //       should NOT cycle through all of the states!
    bool a_down = false;

	while(1) {
		waitForVBlank();

		switch(state) {
		case STATE_START:
            // TODO: Draw snoop1 and go to STATE_SNOOP1
			break;

		case STATE_SNOOP1:
            // TODO: If A is pressed, go to STATE_SNOOP2.
            //
            //       (Hint: If A is down and we've marked it as down, we
            //              should do nothing. If A is down and it
            //              wasn't before, we should mark it as down,
            //              draw the next snoop, and go to the next
            //              state. If A is not down but we'd marked it
            //              as down, we whould mark it as not down.)
            //
            //       (Hint #2: You can check if A is currently down with
            //                 KEY_DOWN_NOW(BUTTON_A)

            break;

		case STATE_SNOOP2:
            // TODO: If A is pressed, go to STATE_SNOOP3
            //
            //       (Hint: This is a copypaste of STATE_SNOOP1 tbh)
            break;

		case STATE_SNOOP3:
            // TODO: If A is pressed, go to STATE_SNOOP4
            break;

		case STATE_SNOOP4:
            // TODO: If A is pressed, go to STATE_SNOOP5
            break;

		case STATE_SNOOP5:
            // TODO: If A is pressed, go to STATE_SNOOP6
            break;

		case STATE_SNOOP6:
            // TODO: If A is pressed, go back to STATE_SNOOP1
            break;
		}
	}

	return 0;
}

