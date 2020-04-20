void TABLE_Timer (void) {
  AsyncDelay delay_1s;
    AsyncDelay delay_0250;
  bool TABLE_Timer_ON=false;
  unsigned long TABLETimer_prevMillisStart=0;
long TABLETimer_INTERVAL = 5500;  //in ms
int timerValue=0;
bool localStop=false;
bool timerDrin=false;
TABLE_currentMillis=0;
TABLE_previousMillis=0;
TABLE_INTERVAL = 125;
int timerContaSec=0;
Serial.println("ENTER timer table");
  clsTable(0);
      delay(1500);
      TABLETimer_prevMillisStart= millis();
      TABLE_previousMillis= millis();
      
   while (!localStop) {
     TABLE_currentMillis = millis();
     
      common_loop();
  

      if (!TABLE_Timer_ON) {
            printDigit (timerValue, 3, 2, RED, '0');

           if ((TABLE_currentMillis - TABLETimer_prevMillisStart > TABLETimer_INTERVAL)) {
              TABLE_Timer_ON=true;
              Serial.println("   TIMER ON!");
               clsTable(0);
               timerValue=timerValue-1;
               timerContaSec=59;
                 printDigit (timerValue, 3, 0, WHITE, '0');
                printDigit (timerContaSec, 3, 7, YELLOW, '0');
                delay_1s.start(1000, AsyncDelay::MILLIS);
             }
      }

        if (TABLE_Timer_ON && delay_1s.isExpired()) {
          timerContaSec=timerContaSec-1;
          if (timerContaSec==0) { timerValue=timerValue-1;}
          if (timerContaSec<0) {timerContaSec=59; }
           clsTable(0);
           printDigit (timerValue, 3, 0, WHITE, '0');
           printDigit (timerContaSec, 3, 7, YELLOW, '0');
          delay_1s.repeat(); // Count from when the delay expired, not now

          if (timerContaSec==0 && timerValue<=0) {
            timerDrin=true;
            localStop=true;
          }
          
        }
      if (buttonClicked==true) {
     
        
         if (buttonLONGClicked) {
             localStop=true;
             Serial.println("EXITIN timer table");
             buttonLONGClicked=false;
             buttonClicked=false;
       }

      if (!TABLE_Timer_ON) {
         if ((TABLE_currentMillis - TABLE_previousMillis > TABLE_INTERVAL)) {
            //click
            timerValue=timerValue+1;
            TABLE_previousMillis= TABLE_currentMillis;
              TABLETimer_prevMillisStart=TABLE_currentMillis;
         }

          if ( keyRepeat>=3)  { timerValue=timerValue+4;     delay(250); }
      } // if (!TABLE_Timer_ON) {
        
     
    }// if (buttonClicked==true) {
   }//while (!localStop) {


  if (localStop && timerDrin) {
    delay_0250.start(250, AsyncDelay::MILLIS);
    localStop=false;
    buttonClicked=false;
    buttonLONGClicked=false;
     Serial.println("  timer table DRIRIRIRIRN");
    while (!localStop) {
          common_loop();
      
            if (delay_0250.isExpired()) {
              timerDrin=!timerDrin;

              if (timerDrin)  {BeepOFF(); clsTable(colorLib[3]);}
              else  {BeepON(); clsTable(colorLib[0]);}
              
             delay_0250.repeat(); 
            }
            if (buttonClicked==true) {
                 if (buttonLONGClicked) {
                  BeepOFF();
                     localStop=true;
                     Serial.println("EXITIN timer table");
                     buttonLONGClicked=false;
                     buttonClicked=false;
                  }
            }
    } //while

    
  }

   BeepOFF();
}//void TABLE_Timer
