#include "pebble.h"
#define MAX 5

//From http://forums.getpebble.com/discussions/8280/displaying-the-value-of-a-floating-point
static char* floatToString(char* buffer, int bufferSize, double number){
  int temp;
  char decimalBuffer[MAX];
  number *= 100;


  temp = (int)((double)(number - (int)number) * (double)1000);
  if((temp%10) >= 5){
    temp += 10;
  }
  temp /= 10;
  if(temp == 100){
    number++;
    temp = 0;
  }
  APP_LOG(0, "%02d", temp);
  snprintf(decimalBuffer, MAX, "%02d", temp);

  // APP_LOG(0, "%s", decimalBuffer);

  snprintf(buffer, bufferSize, "%d", (int)number);
  strcat(buffer, ".");

  strcat(buffer, decimalBuffer);

  return buffer;
}
