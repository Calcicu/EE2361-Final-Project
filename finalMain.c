/* 
 * File:   thom6223_lab5main.c
 * Author: Jake Thompson, Carol Svare, Mai Hashad, Rachel Chadwick
 *
 * Created on November 29, 2017, 4:00 PM
 */

#include "xc.h"
#include <p24Fxxxx.h>
#include "lcdLib.h"
#include <stdio.h>

// CW1: FLASH CONFIGURATION WORD 1 (see PI24 Family Reference Manual 24.1)
#pragma config ICS = PGx1           // Comm Channel Select (Emulator EMUC1/EMUD1 pins areshared with PGC1/PGD1)
#pragma config FWDTEN = OFF         // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF           // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF            // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF         // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2
#pragma config I2C1SEL = PRI        // I2C1 Pin Location Select (Use default SCL1/SDA1pins)
#pragma config IOL1WAY = OFF        // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON        // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME       // Clock Switching and Monitor (Clock switching is enabled,
                                        // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL       // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

/*Global Flags*/
int modeFlag = 0; // Mode starts in the drawing mode
int rightFlag = 0;
int leftFlag = 0;
int upFlag = 0;
int downFLag = 0;
int changeFlag = 0; //checks if there was a change in the joystick position
int cursorRightFlag = 0;
int saveFlag = 0; //save button was pressed
int loadFlag = 0; //Load button pressed
int LEDFlag = 0; //Turn LED on/off

/*Array*/
unsigned char presetColor [7] [3];     // presetColors [colorCount] [GRB]
unsigned char workInProgress [8] [8] [3];   //Format: [X] [Y] [G,R,B]
unsigned char array1 [8] [8] [3];
unsigned char array2 [8] [8] [3];
unsigned char array3 [8] [8] [3];

//color button presses
int colorCount = 0;

void setup(void);
void lcdDisplayCursor(void);
void updateArray(void);
void fillPresetColor(void);
void saveArray(int saveMode, int arrayNum);

void __attribute__((__interrupt__,__auto_psv__)) _IC1Interrupt(void)
{
  
  if (modeFlag == 0)
  {
    ++colorCount;
    if (colorCount == 7)
        colorCount = 0;
  }
  else
  {
    saveFlag = 1;
  }
  
  _IC1IF = 0;
  
}
void __attribute__((__interrupt__,__auto_psv__)) _IC2Interrupt(void)
{
  
  if (modeFlag == 0)
  {
    modeFlag = 1; // upload/save mode
  }
  else 
  {
    modeFlag = 0; //draw mode
  }
  
  _IC2IF = 0;
  
}
void __attribute__((__interrupt__,__auto_psv__)) _IC3Interrupt(void)
{
  // interrupt for the 3rd button
  // in draw mode togles on and off
  // in upload mode save button to certain array
 
  
   if (modeFlag == 0)
  {
    LEDFlag = 1;
  }
  else
  {
    loadFlag = 1;
  }
  
  _IC3IF = 0;
  
}

void __attribute__((__interrupt__,__auto_psv__)) _ADC1Interrupt(void)
{

 if (changeFlag == 0)
 {  
  changeFlag = 1;
   
  if ( ADC1BUF0 > 1.7)
  {
    //this is up
    upFlag = 1;
   
  }
  
  else if (ADC1BUF0 < 1.3)
  {
    //this is up down
    downFLag = 1;
  
  }
     
  if ( ADC1BUF1 > 1.7)
  {
    //this is right
    rightFlag = 1;
      
    if (modeFlag = 1)
    {
      cursorRightFlag++;
      if (cursorRightFlag == 3)
          cursorRightFlag = 0;
    }
    

  }
  
  else if (ADC1BUF1 < 1.3)
  {
    //this is up left
    leftFlag = 1;
    
  }
 }
  
  _AD1IF = 0;
}


/*
void __attribute__((__interrupt__,__auto_psv__)) _T3Interrupt(void) 
{
  _T3IF = 0;
}
// Added this Interrupt just in case we need it later
*/

int main()
{
  setup();
  fillPresetColor();
  
  while(1)
  {  
   
    updateArray();  //This function is timing sensitive, disable interrupts immediately before
    
    // changes flags if a button was pressed during updateArray()
    if (_RB9 = 1){    //if save/cycle color was pressed during refresh
      if (!modeFlag)
        //set save flag
        saveFlag = 1;
      else
        colorCount++;
        
    }//end if _RB9 = 1
    
    if (_RB7 = 1){    //if load/draw was pressed during refresh
      if (!modeFlag)
      {
        loadFlag = 1;
      }
      else
        LEDFlag = 1;
    }//end if _RB7 = 1
    
    if (_RB8 = 1){    //if toggle mode was pressed during refresh
      modeFlag = !modeFlag;
    }
    
    
    //Process flags (update cursor position, update color values, etc.)
    
    if (modeFlag == 1)  //draw mode
    {
      setCursor(0,1);   //set the cd display so that is shows when the game is in drawing mode
      lcdPrintStr("Drawing");
      
      if (rightFlag)
      {
        if(_______ < 7) //if cursor not all the way to rigth
        {
         //move LED cursor one to the right
        }
      
        // else stay at current position
      }
      
      else if (leftFlag)
      {
        if(_____ > 0) //if cursor not all the way to left
        {
         //move LED cursor one to the left
        }
        
        // else stay at current position
      }
      
      else if (upFlag)
      {
        if (_______ > 0)//if cursor not all the way to up
        {
         //move LED cursor one to the up
        }
        
        // else stay at current position
      }
      
     else if (downFlag)
      {
        if(____ < 7)//if cursor not all the way to down
        {
         //move LED cursor one to the right
        }
       
        // else stay at current position
      } 
      
      
    }
    
    else  //upload mode
    {
      setCursor(1,0);
      lcdPrintStr(" 1 2 3 ");
      setCursor(0,1);
      lcdPrintStr("Upload");
      lcdDisplayCursor();
      
      if (saveFlag) // saving c=work into an array
      {
        if (cursorRightFlag == 0)
        {
          // save drawing to array 1
        }
        else if (cursorRightFlag == 1)
        {
          // save drawing to array 2
        }
        else
          //save drawing to array 3
      }
      
      if (loadFlag) //uploading saved work to the LED
      {
        if (cursorRightFlag == 0)
        {
          // place saved drawing form array 1 onto the lcd display
        }
        else if (cursorRightFlag == 1)
        {
          // place saved drawing form array 2 onto the lcd display
        }
        else
          //place saved drawing form array 3 onto the lcd display
      }
      
      
    }
   
      
    //every 2ms:
    // 1. disable interrupts 
    // 2. refresh LED  
    // 3. put the conditions that set the flags and the interrupts
              //jat_wait_1ms(); 
              //jat_wait_1ms();*/
  }
  return 0;
}

void setup(void)
{
  CLKDIVbits.RCDIV = 0; //set Fcy to 16MHz
  AD1PCFG = 0xfffd;   //AN0 and AN1 are analog
  TRISA = 0b1111111111111011; //TRISA still needs to be set
  TRISB = 0b0000001110000011; // TRISA still needs to be set
  
    // pullup resistors
    _CN21PUE = 1; //enable pullup resostor for RB9
    _CN22PUE = 1; //enable pullup resister for RB8
    _CN23PUE = 1; //enable pullup resister for RB7
  
  //initialize input capture for 1st push button
  
  INTCON1bits.NSTDIS = 1; //disables interrupt nesting
  IPC0bits.IC1IP = 4;   // interrupt priority
 
  __builtin_write_OSCCONL (OSCCON & 0xbf);
  RPINR7bits.IC1R = 9;
  __builtin_write_OSCCONL (OSCCON | 0x40);
  
  IFS0bits.IC1IF = 0;
  IC1CON = 0x0002;  //capture and interrupt every falling edge
  
  IEC0bits.IC1IE = 1; //enable interrupt
  
  //initialize input capture for 2nd push button
  
  INTCON1bits.NSTDIS = 1; //disables interrupt nesting
  _IC3IP = 4;   // interrupt priority
 
  __builtin_write_OSCCONL (OSCCON & 0xbf);
  RPINR8bits.IC3R = 7;
  __builtin_write_OSCCONL (OSCCON | 0x40);
  
  _IC3IF = 0;
  IC3CON = 0x0003;  //capture and interrupt every rising edge
  
  _IC3IE = 1; //enable interrupt  
 
  //initialize i2c for LCD and
  
    I2C2CON = 0;
    I2C2CONbits.DISSLW = 1;
    I2C2BRG = 157;
    _MI2C2IF = 0;
    I2C2CONbits.I2CEN = 1; //enable I2C
  
  // initialize timer 3
 
    TMR3 = 0;
    T3CON = 0;
    PR3 = 15999; //show values every 100ms
    T3CONbits.TCKPS = 0; // prescaer set 1:1
    
    _T3IE = 1;
    _T3IP = 3; // set to low priority , we can change that if we need to
    
    _VCFG = 0;
    _ADCS = 0; // sat auto-sampling time  = 1*Tcy 
    _ASAM = 1; // set auto-sampling
    _SSRC = 0b010; // Timer3 compare ends sampling and starts conversionconversion
    _SAMC = 0b010; // Auto-Sample Time bits = 10 *TAD
    _SMPI = 0b001; // Interrupts at the completion of conversion for every 2nd sample/convert sequence
    
    
   // initialize interrupt for A/D:
    _AD1IF = 0; // set interrupt flag to 0
    _AD1IP = 1;   //set interrupt priority to high
    _AD1IE = 1; //enable interrupt
    
    AD1CON1bits.ADON = 1; 
    T3CONbits.TON = 1;
    
    //initialize input capture for the joystick
  
  INTCON1bits.NSTDIS = 1; //disables interrupt nesting
  _IC2IP = 4;   // interrupt priority
 
  
    __builtin_write_OSCCONL (OSCCON & 0xbf);
    RPINR7bits.IC2R = 8; // ic2 is associated with RB8
    __builtin_write_OSCCONL (OSCCON | 0x40);
 
  
  IFS0bits.IC2IF = 0;
  IC3CON = 0x0002;  //capture and interrupt every falling edge
  
  IEC0bits.IC2IE = 1; //enable interrupt
 
}

void lcdDisplayCursor(void)
{
  //enable cursor
  
  if (cursorRightFlag == 0)
  {
   setCursor(1,0);
  }
  else if (cursorRightFlag == 1)
  {
    setCursor(3,0);
  }
  else 
  {
    setCursor(5,0);
  }
 
 }

void updateArray(void){
    int mask;
    int x;
    int y;
    int color;
    
    for(y = 0; y < 8; y++){
        for(x = 0; x < 8; x++){
            for(color = 0; color < 3; color++){
                for(mask = 0x80; mask > 0; mask >>=1){
                    if (mask & (workInProgress [x] [y] [color]))
                        write_1();
                    else
                        write_0();
                }//end for mask
            }//end for color
        }//end for x
    }//end for y
}

void fillPresetColor(void){
    presetColor[0] [0] = 0;
    presetColor[0] [1] = 15;
    presetColor[0] [2] = 0;
    
    presetColor[1] [0] = 7;
    presetColor[1] [1] = 24;
    presetColor[1] [2] = 0;
    
    presetColor[2] [0] = 15;
    presetColor[2] [1] = 15;
    presetColor[2] [2] = 0;
    
    presetColor[3] [0] = 15;
    presetColor[3] [1] = 0;
    presetColor[3] [2] = 0;
    
    presetColor[4] [0] = 0;
    presetColor[4] [1] = 0;
    presetColor[4] [2] = 15;
    
    presetColor[5] [0] = 0;
    presetColor[5] [1] = 12;
    presetColor[5] [2] = 24;
    
    presetColor[6] [0] = 0;
    presetColor[6] [1] = 31;
    presetColor[6] [2] = 15;
}

void saveArray(int saveMode, int arrayNum){
    int x;
    int y;
    int color;
    
    if (saveMode == 0) {         //If loading arrayNum into working array
        if (arrayNum == 1){
            for(y = 0; y < 8; y++){
                for(x = 0; x < 8; x++){
                    for(color = 0; color < 3; color++){
                        workInProgress [x] [y] [color] = array1 [x] [y] [color];
                    }//end for color
                }//end for x
            }//end for y
        }//end arrayNum == 1
    
        if (arrayNum == 2){
            for(y = 0; y < 8; y++){
                for(x = 0; x < 8; x++){
                    for(color = 0; color < 3; color++){
                        workInProgress [x] [y] [color] = array2 [x] [y] [color];
                    }//end for color
                }//end for x
            }//end for y
        }// end arrayNum == 2
        
        if (arrayNum == 3){
            for(y = 0; y < 8; y++){
                for(x = 0; x < 8; x++){
                    for(color = 0; color < 3; color++){
                        workInProgress [x] [y] [color] = array3 [x] [y] [color];
                    }//end for color
                }//end for x
            }//end for y
        }//end arrayNum = 3
    }//end if saveMode = 0 (loading array)
    else{
        if (arrayNum == 1){
            for(y = 0; y < 8; y++){
                for(x = 0; x < 8; x++){
                    for(color = 0; color < 3; color++){
                        array1 [x] [y] [color] = workInProgress [x] [y] [color];
                    }//end for color
                }//end for x
            }//end for y
        }//end arrayNum == 1
    
        if (arrayNum == 2){
            for(y = 0; y < 8; y++){
                for(x = 0; x < 8; x++){
                    for(color = 0; color < 3; color++){
                        array2 [x] [y] [color] = workInProgress [x] [y] [color];
                    }//end for color
                }//end for x
            }//end for y
        }// end arrayNum == 2
        
        if (arrayNum == 3){
            for(y = 0; y < 8; y++){
                for(x = 0; x < 8; x++){
                    for(color = 0; color < 3; color++){
                        array3 [x] [y] [color] = workInProgress [x] [y] [color];
                    }//end for color
                }//end for x
            }//end for y
        }//end arrayNum = 3
    }//end if saveMode == 1 (saving)
}
