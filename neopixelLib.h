/* 
 * File:   neopixelLib.h
 * Author: calcicu
 *
 * Created on December 14, 2017, 7:57 PM
 */

#ifndef NEOPIXELLIB_H
#define	NEOPIXELLIB_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

/*Global Flags*/
extern int modeFlag; // Mode starts in the upload mode
extern int modeChangeFlag;
extern int rightFlag;
extern int leftFlag;
extern int upFlag;
extern int downFlag;
extern int changeFlag; //checks if there was a change in the joystick position
extern int cursorRightFlag;        //Tracks cursor position on LCD
extern int saveFlag; //save button was pressed
extern int loadFlag; //Load button pressed
extern int LEDFlag; //Turn LED on/off
extern int colorFlag;      //Shows when colors need to be cycled

/*Array*/
extern unsigned char cursorPosition [2];   //cursorPosition[0] = x, cursorPosition [1] = y
extern unsigned char presetColor[7] [3];     // presetColors [colorCount] [GRB]
extern unsigned char workInProgress [8] [8] [3];   //Format: [X] [Y] [G,R,B]
extern unsigned char array1 [8] [8] [3];
extern unsigned char array2 [8] [8] [3];
extern unsigned char array3 [8] [8] [3];

//color button presses
extern int colorCount;

void lcdDisplayCursor(void);
void updateArray(void);
void fillPresetColor(void);
void saveArray(int saveMode, int arrayNum);
void clearArray(void);
void drawPixel(int xPos, int yPos);
void blink(int x, int y);       //To be called with (cursorPos[0], cursorPos[1])
void stripeDemo(void);

#endif	/* NEOPIXELLIB_H */

