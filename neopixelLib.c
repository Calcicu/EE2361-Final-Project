#include "lcdLib.h"
#include "thom6223_lab4b_asmLib.h"
#include "neopixelLib.h"

void lcdDisplayCursor(void)         //
{
  //enable cursor
  
  if (cursorRightFlag == 0)
  {
   setCursor(2,0);
  }
  else if (cursorRightFlag == 1)
  {
    setCursor(4,0);
  }
  else 
  {
    setCursor(6,0);
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
    presetColor[0] [0] = 0;     //Red
    presetColor[0] [1] = 15;
    presetColor[0] [2] = 0;
    
    presetColor[1] [0] = 7;     //Orange
    presetColor[1] [1] = 24;
    presetColor[1] [2] = 0;
    
    presetColor[2] [0] = 15;    //Yellow
    presetColor[2] [1] = 15;
    presetColor[2] [2] = 0;
    
    presetColor[3] [0] = 15;    //Green
    presetColor[3] [1] = 0;
    presetColor[3] [2] = 0;
    
    presetColor[4] [0] = 0;     //Blue
    presetColor[4] [1] = 0;
    presetColor[4] [2] = 15;
    
    presetColor[5] [0] = 0;     //Purple
    presetColor[5] [1] = 12;
    presetColor[5] [2] = 24;
    
    presetColor[6] [0] = 0;     //Pink
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

void clearArray(void){
    int x;
    int y;
    int color;
    
    for(y = 0; y < 8; y++){
        for(x = 0; x < 8; x++){
            for(color = 0; color < 3; color++){
                workInProgress [x] [y] [color] = 0;
            }//end for color
        }//end for x
    }//end for y
}

void drawPixel(int xPos, int yPos){
    if ((workInProgress [xPos] [yPos] [0] == presetColor[colorCount] [0]) &&
            (workInProgress [xPos] [yPos] [1] == presetColor[colorCount] [1]) &&
            (workInProgress [xPos] [yPos] [2]) == presetColor[colorCount] [2]){
        workInProgress [xPos] [yPos] [0] = 0;
        workInProgress [xPos] [yPos] [1] = 0;
        workInProgress [xPos] [yPos] [2] = 0;
    }
    else{
        workInProgress [xPos] [yPos] [0] = presetColor [colorCount] [0];
        workInProgress [xPos] [yPos] [1] = presetColor [colorCount] [1];
        workInProgress [xPos] [yPos] [2] = presetColor [colorCount] [2];
    }
}

void blink (int x, int y)
{
   int backupColorG;
   int backupColorR;
   int backupColorB;
  
   backupColorG = workInProgress [x] [y] [0];
   backupColorR = workInProgress [x] [y] [1];
   backupColorB = workInProgress [x] [y] [2];
   workInProgress [x] [y] [0] = 15;
   workInProgress [x] [y] [1] = 15;
   workInProgress [x] [y] [2] = 15;

    updateArray();
    waitms(250);

     workInProgress [x] [y] [0] = backupColorG;
     workInProgress [x] [y] [1] = backupColorR;
     workInProgress [x] [y] [2] = backupColorB;
}

void stripeDemo(void){
    int x;
    int y;
    
    for(y = 0, x = 4; y < 5; y++){      //Red stripe
        array3 [x] [y] [1] = 15;
        x--;
    }//end for y
    
    for(y = 0, x = 5; y < 6; y++){      //Orange stripe
        array3 [x] [y] [0] = 7;
        array3 [x] [y] [1] = 24;
        x--;
    }//end for y
    
    for(y = 0, x = 6; y < 7; y++){      //Yellow stripe
        array3 [x] [y] [0] = 15;
        array3 [x] [y] [1] = 15;
        x--;
    }//end for y
    
    for(y = 0, x = 7; y < 8; y++){      //Green stripe
        array3 [x] [y] [0] = 15;
        x--;
    }//end for y
    
    for(y = 1, x = 7; y < 8; y++){      //Blue stripe
        array3 [x] [y] [2] = 15;
        x--;
    }//end for y
    
    for(y = 2, x = 7; y < 8; y++){      //Purple stripe
        array3 [x] [y] [1] = 12;
        array3 [x] [y] [2] = 24;
        x--;
    }//end for y
    
    for(y = 3, x = 7; y < 8; y++){      //Pink stripe
        array3 [x] [y] [1] = 31;
        array3 [x] [y] [2] = 15;
        x--;
    }//end for y
}
