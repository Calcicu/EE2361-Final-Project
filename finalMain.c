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

void setup(void);

void __attribute__((__interrupt__,__auto_psv__)) _IC1Interrupt(void)
{
  // interrupt for color button
}
void __attribute__((__interrupt__,__auto_psv__)) _IC2Interrupt(void)
{
  //implement flag that determines draw or upload mode
  // interrupt for the joystick
  // draw mode and select on lcd(for each saved drawing
  
  
}
void __attribute__((__interrupt__,__auto_psv__)) _IC3Interrupt(void)
{
  // interrupt for the 3rd button
  // in draw mode togles on and off
  // in u0pload mode save button to certain array
  
}

void __attribute__((__interrupt__,__auto_psv__)) _ADC1Interrupt(void)
{
  if ( AD1CONBUF0 > 1.7)
    //this is up
    
  else if (AD1CCONBUF0 < 1.3)
    //this is up down
     
    if ( AD1CONBUF1 > 1.7)
    //this is right
    
  else if (ADC1CONBUF1 < 1.3)
    //this is up left
    
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
  
  return 0;
}

void setup(void)
{
  CLKDIVbits.RCDIV = 0; //set Fcy to 16MHz
  AD1PCFG = 0xfffd;   //AN0 and AN1 are analog
  TRISA = 0b0000000000000000
  TRISB = 0b0000000000000000
  
  
  //initialize input capture for 1st push button
  
  INTCON1bits.NSTDIS = 1; //disables interrupt nesting
  IPC0bits.IC1IP = 4;   // interrupt priority
 
  __builtin_write_OSCCONL (OSCCON & 0xbf);
  RPIN7bits.IC1R = 9;
  __builtin_write_OSCCONL (OSCCON | 0x40);
  
  IFS0bits.IC1IF = 0;
  IC1CON = 0x0002;  //capture and interrupt every falling edge
  
  IEC0bits.IC1IE = 1; //enable interrupt
  
  //initialize input capture for 2nd push button
  
  INTCON3bits.NSTDIS = 1; //disables interrupt nesting
  IPC0bits.IC3IP = 4;   // interrupt priority
 
  __builtin_write_OSCCONL (OSCCON & 0xbf);
  RPIN8bits.IC3R = 7;
  __builtin_write_OSCCONL (OSCCON | 0x40);
  
  IFS0bits.IC3IF = 0;
  IC3CON = 0x0002;  //capture and interrupt every falling edge
  
  IEC0bits.IC3IE = 1; //enable interrupt
 
  
 
  //initialize i2c for LCD and
  
    I2C2CON = 0;
    I2C2CONbits.DISSLW = 1;
    I2CBRG = 157;
    _MI2C2IF = 0;
    I2C2CONbits.I2CEN = 1; //enable I2C
  
  // initialize timer 3
 
    TRM3 = 0;
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
    _AD1IP = 1   //set interrupt priority to high
    _AD1IE = 1; //enable interrupt
    
    AD1CON1bits.ADON = 1; 
    T3COnbits.TON = 1;
    
    //initialize input capture for the joystick
  
   INTCON2bits.NSTDIS = 1; //disables interrupt nesting
  IPC0bits.IC2IP = 4;   // interrupt priority
 
  
    __builtin_write_OSCCONL (OSCCON & 0xbf);
    RPIN7bits.IC2R = 8; // ic2 is associated with RB8
    __builtin_write_OSCCONL (OSCCON | 0x40);
 
  
  IFS0bits.IC2IF = 0;
  IC3CON = 0x0002;  //capture and interrupt every falling edge
  
  IEC0bits.IC2IE = 1; //enable interrupt
 
}
