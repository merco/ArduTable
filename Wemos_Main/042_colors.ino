void TABLE_Colors (void) {
int iCOL=0;
bool localStop=false;
TABLE_currentMillis=0;
TABLE_previousMillis=0;
TABLE_INTERVAL = 125;
Serial.println("ENTER color table");
  clsTable(colorLib[iCOL]);
  delay(1000);
   while (!localStop) {
     TABLE_currentMillis = millis();
      common_loop();
       clsTable(colorLib[iCOL]);

      if (buttonClicked==true) {
        //Serial.println("click");
        
         if (buttonLONGClicked) {
             localStop=true;
             Serial.println("EXITIN color table");
             buttonLONGClicked=false;
             buttonClicked=false;
       }

   
         if ((TABLE_currentMillis - TABLE_previousMillis > TABLE_INTERVAL)) {
         
          iCOL=iCOL+1;
          if (iCOL>=colorLib_MAXCOL) iCOL=0;
        }
             Serial.print("Cuur icol"); Serial.println(iCOL);
            clsTable(colorLib[iCOL]);
            TABLE_previousMillis= TABLE_currentMillis;
      
     
    }
   }//while (!localStop) {
}//void TABLE_Colors
