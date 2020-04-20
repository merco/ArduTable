
uint8_t loadCharInBuffer35(char letter) {
  uint8_t* tmpCharPix;
  uint8_t tmpCharWidth;


   int letterIdx = (letter - 32) * 4;
  int x = 0; int y = 0;
  for (int idx = letterIdx; idx < letterIdx + 4; idx++) {
    unsigned char xc=pgm_read_byte(&font3x5[idx]);
    
    for (int x = 0; x < 8; x++) {
       
     // charBuffer[x][y] = ((xc) & (1 << (7 - x))) > 0;
    charBuffer[x][y] = ((xc) & (1 << (x))) > 0;
   
 
    }

   
    y++;
  }


    
   
  tmpCharWidth = 4;
  return tmpCharWidth;
}
void printDigit (int DDD ,int xoffset, int yoffset, unsigned long color, char leadZero) {

String payload="";
  char valArr[3];
payload=String(DDD);
 if (DDD<=9) payload=leadZero+payload;

payload.toCharArray(valArr,3);

  printText35(valArr,2,xoffset,yoffset,color);
  
}
void printText35(char* text, unsigned int textLength, int xoffset, int yoffset, unsigned long color) {
  uint8_t curLetterWidth = 0;
  int curX = xoffset;
  //clearImg();

  //Loop over all the letters in the string
  for (int i = 0; i < textLength; i++) {
    //Determine width of current letter and load its pixels in a buffer
    curLetterWidth = loadCharInBuffer35(text[i]);
    //Loop until width of letter is reached
    for (int lx = 0; lx < curLetterWidth; lx++) {
      //Now copy column per column to field (as long as within the field
      if (curX >= 12) { //If we are to far to the right, stop loop entirely
        break;
      } else if (curX >= 0) { //Draw pixels as soon as we are "inside" the drawing area
        for (int ly = 0; ly < 5; ly++) { //Finally copy column
          leds[pgm_read_byte(&ledsAdress[yoffset + ly][curX])] = charBuffer[ly][lx] * color;

        }
      }
      curX++;
    }
  }
  FastLED.show();
}
//Load char in buffer and return width in pixels
uint8_t loadCharInBuffer(char letter) {
  uint8_t* tmpCharPix;
  uint8_t tmpCharWidth;

  int letterIdx = (letter - 32) * 8;
  int x = 0; int y = 0;
  for (int idx = letterIdx; idx < letterIdx + 8; idx++) {
    for (int x = 0; x < 8; x++) {
       unsigned char xc=pgm_read_byte(&font[idx]);
      charBuffer[x][y] = ((xc) & (1 << (7 - x))) > 0;
    }
    y++;
  }

  tmpCharWidth = 8;
  return tmpCharWidth;
}
void printText(char* text, unsigned int textLength, int xoffset, int yoffset, unsigned long color) {
  uint8_t curLetterWidth = 0;
  int curX = xoffset;
  //clearImg();

  //Loop over all the letters in the string
  for (int i = 0; i < textLength; i++) {
    //Determine width of current letter and load its pixels in a buffer
    curLetterWidth = loadCharInBuffer(text[i]);
    //Loop until width of letter is reached
    for (int lx = 0; lx < curLetterWidth; lx++) {
      //Now copy column per column to field (as long as within the field
      if (curX >= 12) { //If we are to far to the right, stop loop entirely
        break;
      } else if (curX >= 0) { //Draw pixels as soon as we are "inside" the drawing area
        for (int ly = 0; ly < 8; ly++) { //Finally copy column
          leds[pgm_read_byte(&ledsAdress[yoffset + ly][curX])] = charBuffer[lx][ly] * color;

        }
      }
      curX++;
    }
  }
  FastLED.show();
}
//Scroll current selection text from right to left;
void scrollText(char* curSelectionText, int curSelectionTextLength, unsigned long color ,int yoffset) {
  for (int x = 12; x > -(curSelectionTextLength * 8); x--) {
    printText(curSelectionText, curSelectionTextLength, x, yoffset, color);
     delay(100);
    common_IR();
    if (keyRepeat>=6) {buttonLONGClicked=true; break;}
   
  }
}
