  int TIME_HH = 0;
  int TIME_MM = 0;
  int TIME_SS = 0;
  int TIME_GG = 0;
  int TIME_ME = 0;
  int TIME_AA = 0;

void getTimeWeb() {
  HTTPClient http;
  http.begin("http://davidemercanti.altervista.org/nownow.php");
  int httpCode = http.GET();
  String payload = "";

  if (httpCode == HTTP_CODE_OK) {

    payload = http.getString();
    Serial.println(payload);
    int ind1 = payload.indexOf(' ');
    if (ind1 > 0) {

      String payload1 = splitString(payload, ' ', 0);
      TIME_GG = splitString(payload1, '-', 0).toInt();
      TIME_ME = splitString(payload1, '-', 1).toInt();
      TIME_AA = splitString(payload1, '-', 2).toInt();
      Serial.println(TIME_GG);
      Serial.println(TIME_ME);
      Serial.println(TIME_AA);

      payload = splitString(payload, ' ', 1);
      Serial.println(payload);
      TIME_HH = splitString(payload, ':', 0).toInt();
      TIME_MM = splitString(payload, ':', 1).toInt();
      TIME_SS = splitString(payload, ':', 2).toInt();
      Serial.println(TIME_HH);
      Serial.println(TIME_MM);
      Serial.println(TIME_SS);


    }
  }
  http.end();
}
void TABLE_Time (void) {
bool localStop=false;
int timeMode=1;
int time_ic1=0;
int k_ic1=1;
int k_ic2=-1;
int time_ic2=4;
byte time_icol=0;

clsTable(0);
getTimeWeb();
delay(1500);
 char* spaces="  "; 
AsyncDelay delay_1m;
AsyncDelay delay_1s;
delay_1m.start(60000, AsyncDelay::MILLIS);
delay_1s.start(1000, AsyncDelay::MILLIS);
TABLE_currentMillis=0;
TABLE_previousMillis=0;
TABLE_INTERVAL = 125;
Serial.println("ENTER time table");
  while (!localStop) {
     TABLE_currentMillis = millis();
      common_loop();

           
      if (timeMode==1) {
        
         printDigit (TIME_HH, time_ic1, 0, BLUE, '0');
         printDigit(TIME_MM, time_ic2, 7, RED, '0');
      } else {
        printDigit (TIME_GG, time_ic1, 0, GREEN, '0');
        printDigit(TIME_ME, time_ic2, 7, PURPLE, '0');
      }

       if (delay_1m.isExpired()) {
          getTimeWeb();
          delay_1m.repeat(); // Count from when the delay expired, not now
        }
        if (delay_1s.isExpired()) {
           
          printText35(spaces,2,time_ic1,0,0);
          printText35(spaces,2,time_ic2,7,0);
          time_ic1=time_ic1+k_ic1;
          time_ic2=time_ic2+k_ic2;
          if ((time_ic1>4) || (time_ic1<=0) ) k_ic1=k_ic1*-1; //time_ic1=0;
           if ((time_ic2>4) || (time_ic2<=0) ) k_ic2=k_ic2*-1; //time_ic1=0;
          time_icol=time_icol+1;
          if (time_icol>9) time_icol=0;
         
          
          delay_1s.repeat(); // Count from when the delay expired, not now
        }
    if (buttonClicked==true) {
       if (buttonLONGClicked) {
        localStop=true;
         Serial.println("EXITIN time table");
           buttonLONGClicked=false;
           buttonClicked=false;
       }
    if ((TABLE_currentMillis - TABLE_previousMillis > TABLE_INTERVAL)) {

      timeMode=timeMode+1;
      if (timeMode==3) timeMode=1;
    }
      TABLE_previousMillis= TABLE_currentMillis;
    }
   
     
  
  }
  

}//void TABLE_Time
