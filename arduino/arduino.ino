#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define PIN 6
#define NUMPIXELS 150

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
//#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
//  clock_prescale_set(clock_div_1);
//#endif
  Serial.begin(9600);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

int acc = 0;
int count = 0;
int incomingByte = 0;

int colorR = 255;
int colorG = 255;
int colorB = 255;
bool isFlashing = true;


/////////////////////////////////////////////////////////////
const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
 // if (Serial.available() > 0) {
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
      if (data.charAt(i) == separator || i == maxIndex) {
          found++;
          strIndex[0] = strIndex[1] + 1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);

        if(receivedChars[0] == 'c') { //Change color
          String recievedString;
          recievedString = receivedChars;
  
          int R = getValue(recievedString, ',',1).toInt();
          int G = getValue(recievedString, ',',2).toInt();
          int B = getValue(recievedString, ',',3).toInt();
  
          Serial.println(R);
          Serial.println(G);
          Serial.println(B);
  
          colorR = R;
          colorG = G;
          colorB = B;
        } 

        if(receivedChars[0] == 'f') {
          isFlashing = true;
        }

        newData = false;
    }
}

//////////////////////////////////////////////////////////////


void loop() {
  recvWithStartEndMarkers();
  showNewData();
//  count += 1;
  
//  if(count%10 == 0) {
//     acc += 10;  
//     runningLine(acc, 10);  
//  }

  Serial.println("Running line...");
  for (int j = 0; j < 100; j+=1) {
    runningLine(j, 10);
    delay(30);  
  }
  Serial.println("/Running line...");

  Serial.println("Wait listen...");
  waitListen();
  Serial.println("/Wait listen...");
  
  if (isFlashing) { flash(); }
}


void waitListen() {
//  int startColor = 255;
//  int endColor = 100;
//  easeLights(startColor, endColor);

  for (int i = 0; i < NUMPIXELS; i++) {    
      pixels.setPixelColor(i, pixels.Color(13, 181, 224));
   }
   pixels.show();
   
   delay(2000);
}

void flash() {
  int DELAY = 300;
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
  }
  delay(DELAY);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
  }
  delay(DELAY);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
  }
  isFlashing = false;
}

void runningLine(int offset, int lngth) {
  int limitedOffset = offset % NUMPIXELS;
  int limitedOffset2 = (offset + NUMPIXELS / 3) % NUMPIXELS;
  int limitedOffset3 = (offset + NUMPIXELS * 2 / 3) % NUMPIXELS;
  for (int i = 0; i < NUMPIXELS; i++) {
    if ((i > limitedOffset && i < limitedOffset + lngth) | (i > limitedOffset2 && i < limitedOffset2 + lngth) | (i > limitedOffset3 && i < limitedOffset3 + lngth)) {
      pixels.setPixelColor(i, pixels.Color(colorR, colorG, colorB));
    } else {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  }
 pixels.show();
}
