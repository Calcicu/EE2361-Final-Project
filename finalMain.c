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
#include "thom6223_lab4b_asmLib.h"
#include "neopixelLib.h"

// CW1: FLASH CONFIGURATION WORD 1  (see PI24 Family Reference Manual 24.1)
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

void setup(void);

/*Global Flags*/
int modeFlag = 0; // Mode starts in the upload mode
int modeChangeFlag = 1;
int rightFlag = 0;
int leftFlag = 0;
int upFlag = 0;
int downFlag = 0;
int changeFlag = 0; //checks if there was a change in the joystick position
int cursorRightFlag = 0;        //Tracks cursor position on LCD
int saveFlag = 0; //save button was pressed
int loadFlag = 0; //Load button pressed
int LEDFlag = 0; //Turn LED on/off
int colorFlag = 0;      //Shows when colors need to be cycled

/*Array*/
unsigned char cursorPosition [2];   //cursorPosition[0] = x, cursorPosition [1] = y
unsigned char presetColor [7] [3];     // presetColors [colorCount] [GRB]
unsigned char workInProgress [8] [8] [3];   //Format: [X] [Y] [G,R,B]
unsigned char array1 [8] [8] [3];
unsigned char array2 [8] [8] [3];
unsigned char array3 [8] [8] [3];

//color button presses
int colorCount = 0;
  
void __attribute__((__interrupt__,__auto_psv__)) _IC1Interrupt(void)
{
  
    if (modeFlag == 1){
        colorFlag = 1;
    }
    else{
        saveFlag = 1;
    }
  
  _IC1IF = 0;
  
}
void __attribute__((__interrupt__,__auto_psv__)) _IC2Interrupt(void)
{
    modeChangeFlag = 1;
  if (modeFlag == 0)
  {
    modeFlag = 1; // draw mode
  }
  else 
  {
    modeFlag = 0; //upload/save
  }
  
  _IC2IF = 0;
  
}
void __attribute__((__interrupt__,__auto_psv__)) _IC3Interrupt(void)
{
  // interrupt for the 3rd button
  // in draw mode togles on and off
  // in upload mode save button to certain array
 
  
   if (modeFlag == 1)
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
   
  if ( ADC1BUF0*(3.3/1024) > 2)
  {
    //this is up
    upFlag = 1;
    changeFlag = 1;
   
  }
  
  else if (ADC1BUF0*(3.3/1024) < 1)
  {
    //this is down
    downFlag = 1;
    changeFlag = 1;
  
  }
     
  if ( ADC1BUF1*(3.3/1024) > 2){
    leftFlag = 1;
    changeFlag = 1;
  }
  
  else if (ADC1BUF1*(3.3/1024) < 1)
  {
    rightFlag = 1;
    changeFlag = 1;
    
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
    lcd_init();
    fillPresetColor();
    cursorPosition [0] = 0;
    cursorPosition [1] = 0;
    clearArray();
    stripeDemo();           //Loads rainbow stripes into array3
    
    while(1)
    {  
    while(!_T3IF);  //Wait for 1/4 second
    _T3IF = 0;
    
    //Process flags (update cursor position, update color values, etc.)
    
    if (colorFlag){
        colorCount++;
        colorFlag = 0;
        if (colorCount >= 7)
            colorCount = 0;
    }
    
    if (modeFlag == 1)  //draw mode
    {
      if (modeChangeFlag){  
        setCursor(0,1);   //set the cd display so that is shows when the game is in drawing mode
        lcdPrintStr("Drawing");
        modeChangeFlag = 0;
      }
    
      lcdDisplayCursor();
      
      if (rightFlag)
      {
        if(cursorPosition [0] < 7) //if cursor not all the way to rigth
        {
            cursorPosition [0] = cursorPosition [0] + 1;
        }
        // else stay at current position
        rightFlag = 0;
        changeFlag = 0;
      }
      
      if (leftFlag)
      {
        if(cursorPosition [0] > 0) //if cursor not all the way to left
        {
         cursorPosition [0] = cursorPosition [0] - 1;
        }
        // else stay at current position
        leftFlag = 0;
        changeFlag = 0;
      }
      
      if (upFlag)
      {
        if (cursorPosition [1] > 0)//if cursor not all the way to up
        {
         cursorPosition [1] = cursorPosition [1] - 1;
        }
        // else stay at current position
        upFlag = 0;
        changeFlag = 0;
      }
      
     if (downFlag)
      {
        if(cursorPosition [1] < 7)//if cursor not all the way to down
        {
         cursorPosition [1] = cursorPosition [1] + 1;
        }
        // else stay at current position
        downFlag = 0;
       changeFlag = 0;
      } 
      
      if (LEDFlag)
      {
        LEDFlag = 0;
        drawPixel(cursorPosition[0], cursorPosition[1]);
      }
        
      
      
    }
    
    else{  //upload mode
        if (modeChangeFlag){    //The display only refreshes on mode changes, not every loop
            setCursor(1,0);
            lcdPrintStr(" 1 2 3 ");
            setCursor(0,1);
            lcdPrintStr("Upload ");
            modeChangeFlag = 0;
        }
        
        lcdDisplayCursor();
        
        if (saveFlag) // saving c=work into an array
        {
            saveFlag = 0;
            if (cursorRightFlag == 0){
                saveArray(1,1);         // save drawing to array 1
            }
            else if (cursorRightFlag == 1){
                saveArray(1,2);         // save drawing to array 2
            }
            else                        
                saveArray(1,3);         //save drawing to array 3
        }
      
        if (loadFlag){ //uploading saved work to the LED
            loadFlag = 0;
            if (cursorRightFlag == 0){
                saveArray(0,1);           // place saved drawing from array 1 onto the lcd display
            }
            else if (cursorRightFlag == 1){
                saveArray(0,2);           // place saved drawing from array 2 onto the lcd display
            }
            else
                saveArray(0,3);           // place saved drawing from array 3 onto the lcd display
        }
      
      
        if (rightFlag){
            cursorRightFlag++;
            if(cursorRightFlag > 2)
                cursorRightFlag = 0;
            
            lcdDisplayCursor();
            rightFlag = 0;
            changeFlag = 0;
        }
      
        if (leftFlag){
            cursorRightFlag--;
            if(cursorRightFlag < 0)
                cursorRightFlag = 2;
            
            lcdDisplayCursor();
            leftFlag = 0;
            changeFlag = 0;
        }
      
        if (upFlag){        //Up and down are not used in upload mode
            upFlag = 0;
            changeFlag = 0;
        }
      
        if (downFlag){
            downFlag = 0;
            changeFlag = 0;
        }
       
    }//end upload mode
   
      
    _AD1IE = 0;
    _IC1IE = 0;
    _IC2IE = 0;
    _IC3IE = 0;
    blink(cursorPosition[0], cursorPosition[1]);
    updateArray();  //This function is timing sensitive, disable interrupts immediately before
      
    _AD1IE = 1;
    _IC1IE = 1;
    _IC2IE = 1;
    _IC3IE = 1;
    
    jat_wait_50us();
    
    //lcdDisplayCursor();
    
    // changes flags if a button was pressed during updateArray()
    if (_RB9 == 0){    //if save/cycle color was pressed during refresh
      if (!modeFlag)
        //set save flag
        saveFlag = 1;
      else
        colorCount++;
        
    }//end if _RB9 = 1
    
    if (_RB7 == 0){    //if load/draw was pressed during refresh
      if (!modeFlag)
      {
        loadFlag = 1;
      }
      else
        LEDFlag = 1;
    }//end if _RB7 = 1
    
    if (_RB8 == 0){    //if toggle mode was pressed during refresh
     // modeFlag = !modeFlag;
    }
    
    
    //every 2ms:
    // 1. disable interrupts 
    // 2. refresh LED  
    // 3. put the conditions that set the flags and the interrupts
              //jat_wait_1ms(); 
              //jat_wait_1ms();
  }
  return 0;
}

void setup(void)
{
    CLKDIVbits.RCDIV = 0; //set Fcy to 16MHz
    AD1PCFG = 0xfffc;   //AN0 and AN1 are analog
    TRISA = 0b1111111111111011; //TRISA still needs to be set
    TRISB = 0b0000001110000011; // TRISB still needs to be set
  
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
  
    //initialize timer 3
    TMR3 = 0;
    T3CON = 0;
    PR3 = 25999; //update values every ~1/4 second
    T3CONbits.TCKPS = 0b11; // prescaer set 1:256
    
    //_T3IE = 1;
    //_T3IP = 3; // set to low priority , we can change that if we need to
    
    AD1CON1 = 0x0000;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x0000;
    AD1CSSL = 0x0003;   //Scan AN0, AN1 for positive input
    AD1CHS = 0x0000;    //VR- for negative input
    _CSCNA = 1;     // enable scanning
    _VCFG = 0;      //VR+, VR- from AVdd, AVss
    _ADCS = 0;      // sat auto-sampling time  = 1*Tcy 
    _ASAM = 1;      // set auto-sampling
    _SSRC = 0b111;  // Auto-convert
    _SAMC = 0b11111;  // Auto-Sample Time bits = 8*TAD
    _SMPI = 0b0001; // Interrupts at the completion of conversion for every 2nd sample/convert sequence
    
    
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
 
  
    _IC2IF = 0;
    IC2CON = 0x0002;  //capture and interrupt every falling edge
  
    _IC2IE = 1; //enable interrupt
 
}
