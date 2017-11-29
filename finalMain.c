/* 
 * File:   thom6223_lab5main.c
 * Author: Jake Thompson, Carol Svare, Mai Hashad, Rachel Chadwick
 *
 * Created on November 21, 2017, 11:45 AM
 */

#include "xc.h"
#include <p24Fxxxx.h>
#include "lcdLib.h"
#include <stdio.h>
#include <math.H>

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
