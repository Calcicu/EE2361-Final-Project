#include "lcdLib.h"
#include "xc.h"
#include <p24Fxxxx.h>
#include "thom6223_lab4b_asmLib.h"

int CONTRAST = 25;   //CONTRAST valid range: 0 to 63

void lcd_cmd(char package){
    I2C2CONbits.SEN = 1;        //Initiate start sequence
    while(I2C2CONbits.SEN);     //Wait until start is finished
    _MI2C2IF = 0;
    I2C2TRN = 0b01111100;       //7 bit slave address and R/nW
    while(!_MI2C2IF);           //Wait for send to complete
    _MI2C2IF = 0;               //Reset flag
    I2C2TRN = 0b00000000;       //Control byte
    while(!_MI2C2IF);
    _MI2C2IF = 0;
    I2C2TRN = package;          //Data byte
    while(!_MI2C2IF);
    _MI2C2IF = 0;
    I2C2CONbits.PEN = 1;        //Initiate stop sequence
    while(I2C2CONbits.PEN);
    _MI2C2IF = 0;
}

void lcd_init(void){
    waitms(50);
    lcd_cmd(0b00111000);    //function set, normal instruction mode
    lcd_cmd(0b00111001);    //function set, extended instruction mode
    lcd_cmd(0b00010100);    //interval osc
    lcd_cmd(0b01110000 | (CONTRAST & 0b1111));    //contrast C3-C0
    lcd_cmd(0b01010100 | ((CONTRAST >> 4) & 0b11));    //C5-C4, Ion, Bon
    lcd_cmd(0b01101100);    //follower control
    waitms(200);
    lcd_cmd(0b00111000);    //function set, normal instruction mode
    lcd_cmd(0b00001100);    //display on
    lcd_cmd(0b00000001);    //clear display
    waitms(5);
}

void setCursor(char x, char y){
    lcd_cmd(x + (0x40)*y + 128);
}

void lcdPrintChar(char package){
    I2C2CONbits.SEN = 1;        //Initiate start sequence
    while(I2C2CONbits.SEN);     //Wait until start is finished
    _MI2C2IF = 0;
    I2C2TRN = 0b01111100;       //7 bit slave address and R/nW
    while(!_MI2C2IF);           //Wait for send to complete
    _MI2C2IF = 0;               //Reset flag
    I2C2TRN = 0b01000000;       //Control byte, rs = 1
    while(!_MI2C2IF);
    _MI2C2IF = 0;
    I2C2TRN = package;          //Data byte
    while(!_MI2C2IF);
    _MI2C2IF = 0;
    I2C2CONbits.PEN = 1;        //Initiate stop sequence
    while(I2C2CONbits.PEN);
    _MI2C2IF = 0;
}

void lcdPrintStr(const char *s){
    int i = 0;
    
    I2C2CONbits.SEN = 1;        //Initiate start sequence
    while(I2C2CONbits.SEN);     //Wait until start is finished
    _MI2C2IF = 0;
    I2C2TRN = 0b01111100;       //7 bit slave address and R/nW
    while(!_MI2C2IF);           //Wait for send to complete
    _MI2C2IF = 0;               //Reset flag
    
    while((i < 8) && ((char) *(s+i) != '\0')){   //iterate until 9th char or null
        if ((*(s+i+1) == '\0') || (i == 7)) //if last char or 8th char
            I2C2TRN = 0b01000000;           //co = 0, rs = 1
        else
            I2C2TRN = 0b11000000;           //co = 1, rs = 1
        
        while(!_MI2C2IF);
        _MI2C2IF = 0;
        
        I2C2TRN = *(s + i);
        while(!_MI2C2IF);
        _MI2C2IF = 0;
        i++;
    }
    
    I2C2CONbits.PEN = 1;        //Initiate stop sequence
    while(I2C2CONbits.PEN);
    _MI2C2IF = 0;
    
    if(*(s+i) != '\0'){         //If there are more chars to be written (>8)
     /*   This is an optional addition to make scrolling easier
        setCursor(32,1);            //Rewrite first row to last 8 bits of row 1
        
        I2C2CONbits.SEN = 1;        //Initiate start sequence
        while(I2C2CONbits.SEN);     //Wait until start is finished
        _MI2C2IF = 0;
        I2C2TRN = 0b01111100;       //7 bit slave address and R/nW
        while(!_MI2C2IF);           //Wait for send to complete
        _MI2C2IF = 0;               //Reset flag
        
        for(i = 0; i < 7; i++){
            I2C2TRN = 0b11000000;
            while(!_MI2C2IF);
            _MI2C2IF = 0;
            
            I2C2TRN = *(s+i);
            while(!_MI2C2IF);
            _MI2C2IF = 0;
        }//end for i < 7
        
        I2C2TRN = 0b01000000;
        while(!_MI2C2IF);
        _MI2C2IF = 0;
        
        I2C2TRN = *(s+i);
        while(!_MI2C2IF);
        _MI2C2IF = 0;
        i++;
        
        I2C2CONbits.PEN = 1;        //Initiate stop sequence
        while(I2C2CONbits.PEN);
        _MI2C2IF = 0;
        
        End of the optional addition to make scrolling easier*/       
        setCursor(0,1);         //Wrap to next line
        I2C2CONbits.SEN = 1;        //Initiate start sequence
        while(I2C2CONbits.SEN);     //Wait until start is finished
        _MI2C2IF = 0;
        I2C2TRN = 0b01111100;       //7 bit slave address and R/nW
        while(!_MI2C2IF);           //Wait for send to complete
        _MI2C2IF = 0;               //Reset flag
        
        while((char) *(s+i) != '\0'){           //iterate until null char
            if (*(s+i+1) == '\0')               //if this is the last char,
                I2C2TRN = 0b01000000;           //co = 0, rs = 1
            else
                I2C2TRN = 0b11000000;           //co = 1, rs = 1
        
            while(!_MI2C2IF);
            _MI2C2IF = 0;
        
            I2C2TRN = *(s + i);
            while(!_MI2C2IF);
            _MI2C2IF = 0;
            i++;
        }
    }//end if (second line logic))
    
    I2C2CONbits.PEN = 1;        //Initiate stop sequence
    while(I2C2CONbits.PEN);
    _MI2C2IF = 0;
}

void waitms(int ms){
    int i = 0;
    for(i = 0; i < ms; i++)
        jat_wait_1ms();
}