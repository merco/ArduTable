
void setTablePixel (byte r,byte c,  unsigned int long col) {
  int addr=pgm_read_byte(&ledsAdress[r][c]);
  //Serial.println(addr);
 // byte i= leds[addr];   //ledsAdress[r][c];
  leds[addr]=col;
}
void clsTable ( unsigned int long col) {
 for (int r = 0; r < 12 ; r++) {
    for (int c = 0; c < 12 ; c++) {
         setTablePixel(r,c,col);
    }
 }
  refreshTable();
}
void setTableBrightness ( byte value) {
FastLED.setBrightness(value);
}
void clearTablePixel (byte r,byte c) {
 byte i=    leds[pgm_read_byte(&ledsAdress[r][c])];  //ledsAdress[r][c];
  leds[i]=0;
}
void refreshTable() {

   FastLED.show();
}
