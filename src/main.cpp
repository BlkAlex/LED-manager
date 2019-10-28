#define NUM_LEDS 144
#include <FastLED.h>
#include <pthread.h>
#include "BluetoothSerial.h"
#include "ArduinoJson.h"
#define PIN 15


#define STAIRS_COUNT 8
pthread_t threads[STAIRS_COUNT];

pthread_t bluetoothThread;
#define LED_PER_STAIR_COUNT 18

CRGB leds[NUM_LEDS];
byte counter;
void firstTryanim(int delayBS);
void secondTryAnim(int delayBS);
void setBlack();
void *showStairOnThread(void *threadid);
void *bluetoothLoop(void *threadid);
void threadTryAnimation(int delayBS);
void setRGB(int r, int g ,int b,int br);
std::vector<String> static split(String str,String sep);
BluetoothSerial ESP_BT;

void setup() {
  FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    Serial.begin(115200);
    FastLED.setBrightness(50);

    
 	 	 Serial.begin(115200);
 	 	 ESP_BT.begin("ESP32_LED_Control");
 	 	 Serial.println("Bluetooth Device is Ready to Pair");
 	 	 
        pthread_create(&bluetoothThread, NULL, bluetoothLoop, 0);

 	 	
}
void loop() {
       //threadTryAnimation(10);    
       //delay(5000);
       delay(100);
       //setBlack();
}

void * bluetoothLoop(void *threadid){
    while(1 > 0){
      if (ESP_BT.available ()) // Проверяем, получаем ли мы что-нибудь от Bluetooth 
        { 
          String input = ESP_BT.readString (); // Читаем, что мы получаем 
          Serial.print ("Received:"); Serial.println (input);
          
          std::vector<String> arr;
          arr = split(input,"\n");

          Serial.print("arr capacity =");
          Serial.println(arr.capacity());

          
          Serial.print("arr size =");
          Serial.println(arr.size());


          Serial.print("arr[0]");
          Serial.println(arr[0]);

          Serial.print("arr[last]");
          Serial.println(arr[arr.size()-1]);
          String lastinput = arr[arr.size()-1];

          StaticJsonDocument<200> docq;
          DeserializationError error = deserializeJson(docq, lastinput);

            // Test if parsing succeeds.
          if (error) {
              Serial.print(F("deserializeJson() failed: "));
              Serial.println(error.c_str());
              continue;
          }

          int r = docq["red"];
          int g = docq["green"];
          int b = docq["blue"];
          int br = docq["brightness"];

          Serial.print("r = ");
          Serial.print(r);
          Serial.print(" g = ");
          Serial.print(g);
          Serial.print(" b = ");
          Serial.print(b);
          Serial.print(" br = ");
          Serial.println(br);
          setRGB(r,g,b,br);
          }
      delay(1);
    }
}

void setRGB(int r, int g ,int b,int br){
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i].setRGB(r,g,b);
  }
  FastLED.setBrightness(br);
   FastLED.show();
}


void setBlack(){
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i].setRGB(0,0,0);
  }
   FastLED.show();
}

void threadTryAnimation(int delayBS){
  for (int i = 0; i < STAIRS_COUNT ; i++){
    pthread_create(&threads[i], NULL, showStairOnThread, (void *)i);
    delay(delayBS);
  }
  delay(5000); // костыль ебаны, сделать condition wait
}

void *showStairOnThread(void *threadid) {
  int nstair = (int)threadid;
  int startLed = nstair*LED_PER_STAIR_COUNT;
        for (int led = startLed; led <= (startLed + LED_PER_STAIR_COUNT) ;led ++){
            leds[led] .setRGB( 30, 0, 255);
            FastLED.show();
            Serial.print("Thread = ");
            Serial.print((int) threadid);
            Serial.print(" led num = ");
            Serial.println((int)led);
            delay(50);
        }

    //Serial.println((int)threadid);
}


void firstTryAnim(int delayBS){
  int nNextLed = 0;

  for (int stair = 0 ; stair < STAIRS_COUNT ; stair ++){
  
    for (int led = 0 ; led <LED_PER_STAIR_COUNT ;led ++,nNextLed++){
       // leds[nNextLed].setRGB(104, 24, 25);
        FastLED.show();
        delay(30);
    }
    delay(delayBS);
  }
}

void secondTryAnim(int delayBS){
  int nNextLed = 0;

  for (int stair = 0 ; stair < STAIRS_COUNT ; stair ++){
  
      for (int i = 0 ; i < 94 ; i +=9){
        for (int led = nNextLed ; led < nNextLed + LED_PER_STAIR_COUNT ;led ++){
            leds[led] = CHSV( 57, 100, i);
        }
        FastLED.show();
        delay(25);
      }
  nNextLed +=LED_PER_STAIR_COUNT;
  
    delay(delayBS);
  }
}


std::vector<String> static split(String str,String sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<String> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}

