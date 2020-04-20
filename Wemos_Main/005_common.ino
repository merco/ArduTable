String splitString(String str, char sep, int index){

  int found = 0;
  int strIdx[] = { 0, -1 };
  int maxIdx = str.length() - 1;

  for (int i = 0; i <= maxIdx && found <= index; i++)
  {
    if (str.charAt(i) == sep || i == maxIdx)
    {
      found++;
      strIdx[0] = strIdx[1] + 1;
      strIdx[1] = (i == maxIdx) ? i + 1 : i;
    }
  }
  return found > index ? str.substring(strIdx[0], strIdx[1]) : "";
}
char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}

void microBIP() {

  digitalWrite(D0, HIGH);
  delay(10);
 digitalWrite(D0, LOW);
}
void FiveBEEPS() {
   for (int i = 1; i <= 5 ; i++) {
      microBIP();
       delay(10);
   }
}
void BeepON() {
   digitalWrite(D0, HIGH);
}
void BeepOFF() {
   digitalWrite(D0, LOW);
}
