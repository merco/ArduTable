int responseHH[7];
int responseWW[7];
String responseTT[7];
String responseNN[7];
void getWeather() {
  /***********************************************************/
    HTTPClient http;
  String payload = "";
  http.begin(urlStringMeteo + urlCittaString); //HTTP
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
      Serial.println(payload);

      int lengthString = payload.length();
      Serial.printf("lunghezza: %d\n", lengthString);
      char  buf[lengthString + 1];
      payload.toCharArray(buf, lengthString + 1);
      StaticJsonDocument<1400> doc;
     // JsonObject& root = doc.parseObject(buf);
     DeserializationError errordes=deserializeJson(doc,buf);
      if (errordes) {
        Serial.println("parseObject() failed");
      } else {
        //const char* sensor    = root["oggi"];
      
       for(int i=0;i<7;i++){ 
        responseNN[i]="";
           responseHH[i]     = doc["previsione"][i][0];
           const char* sensw = doc["previsione"][i][1];
           const char* senst = doc["previsione"][i][3];
           const char* sensN = doc["previsione"][i][2];
          responseTT[i]="";
          responseNN[i]=sensN;
          String appot=senst;
           responseTT[i]=appot;
         // char stringBuffer[10];
         // strcpy_P(stringBuffer, (char*)senst);
          //responseTT[i]= String(stringBuffer);
          
           String appow=sensw;
           appow=appow.substring(1);
           responseWW[i]=appow.toInt();

            if (responseWW[i]>100) {
              responseWW[i]=responseWW[i]-100;
            }
           
            if (responseHH[i]<=0) {
              responseHH[i]=-1;
            }
           else {
               if (responseHH[i]>12) {
                  responseHH[i]=responseHH[i]-0;
                } 
           }
       } //for
    
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

}
}//getWeather()

void TABLE_Weahter (void) {
    bool localStop=false;
TABLE_currentMillis=0;
TABLE_previousMillis=0;
TABLE_INTERVAL = 125;
 clsTable(0);
 getWeather();
 int curIndex=0;
 char valArr[25];
 String appoWeMsg="";
  delay(1000);
Serial.println("ENTER TABLE_weather");
  while (!localStop) {
     TABLE_currentMillis = millis();
     common_loop();
     if (responseNN[curIndex]!="") {
        printDigit(responseHH[curIndex],3,0,BLUE,'0');
        appoWeMsg=responseNN[curIndex]+ " "+responseTT[curIndex];
        byte sl=appoWeMsg.length();
        Serial.println(appoWeMsg);
          appoWeMsg.toCharArray(valArr,25);
        scrollText(valArr,sl,colorLib[0],5);
          curIndex=curIndex+1;
            if (curIndex>=7) curIndex=0;
      
     } else curIndex=0;
     if (keyRepeat>=1) {
           Serial.println("buttonClicked TABLE_weather");
            Serial.print("curIndex ");  Serial.println(curIndex);
           clsTable(0);
           if (buttonLONGClicked) {
               localStop=true;
               Serial.println("EXITIN TABLE_weather");
               buttonLONGClicked=false;
               buttonClicked=false;
         }


          if ((TABLE_currentMillis - TABLE_previousMillis > TABLE_INTERVAL)) {

            curIndex=curIndex+1;
            if (curIndex>=7) curIndex=0;
             Serial.print("New curIndex ");  Serial.println(curIndex);
          }
          TABLE_previousMillis= TABLE_currentMillis;


      
     }// if (buttonClicked==true) 
  
    
  }// while (!localStop)
}//void TABLE_Weahter
