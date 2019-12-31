                 =================================
                  _        _    ____    _ ____  _ 
                 | |      / \  | __ )  / | ___|| |
                 | |     / _ \ |  _ \  | |___ \| |
                 | |___ / ___ \| |_) | | |___) |_|
                 |_____/_/   \_\____/  |_|____/(_)
                                                  
                 =================================
                           Spring 2018
                    A State Machine on the GBA

In this lab, you'll be implementing a state machine similar to what
you'll need to implement for your Homework 9.

When you press A (the z key in VBAM, the emulator `make vba' runs), the
screen should slowly zoom into Calvin Cordozar Broadus, Jr.'s face.
There are 6 frames and they should loop, i.e., 1->2->3->4->5->6->1->2->3->...
We've provided the images and you can draw them with
`drawFullscreenImage3(snoopX);', where X is the frame number you want to
draw. We've provided drawFullscreenImage3() and waitForVblank(), so
don't implement them.

Two rules:

 1. Holding down A MUST NOT quickly cycle through all the states.
 2. You MUST call waitForVblank() exactly once, at the beginning of the
    loop.

Have fun!
