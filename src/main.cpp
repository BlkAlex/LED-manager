#include <FastLED.h>
#include <pthread.h>
#include "BluetoothSerial.h"
#include "ArduinoJson.h"
#include "scenario.h"
#define PIN 15

pthread_t bluetoothThread;
extern CRGB leds[NUM_LEDS];

byte counter;
void firstTryanim(int delayBS);
void secondTryAnim(int delayBS);
void setBlack();
void *showStairOnThread(void *threadid);
void *bluetoothLoop(void *threadid);
void threadTryAnimation(int delayBS);
void setRGB(int r, int g ,int b,int br);
void parseAndRun(DynamicJsonDocument docq);
String cutFromInputAndGetLastMEssage(String input);

extern pthread_t scenarioThread;

 
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
        pthread_create(&scenarioThread, NULL, scenarioThreadFunc, 0);

 	 	
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
          
          
          String lastinput = cutFromInputAndGetLastMEssage(input);

          StaticJsonDocument<500> docq;
          DeserializationError error = deserializeJson(docq, lastinput);

            // Test if parsing succeeds.
          if (error) {
              Serial.print(F("deserializeJson() failed: "));
              Serial.println(error.c_str());
              continue;
          }
          parseAndRun(docq);
          }
      delay(10);
    }
}

void parseAndRun(DynamicJsonDocument docq){
    
    Scenario scenario;

    String scenarioName = docq["scenario"];
    scenario.name = scenarioName;

    int delayChairs = docq["delayChairs"];    
    scenario.delayChairs = delayChairs;

    int delayPerChair = docq["delayPerChair"]; 
    scenario.delayPerChair = delayPerChair;
    int delayAfterAnim = docq["delayAfterAnimation"]; 
    scenario.delayAfter = delayAfterAnim;

    int red = docq["red"]; 
    scenario.red = red;
    int green = docq["green"]; 
    scenario.green = green;
      int blue = docq["blue"]; 
    scenario.blue = blue;
      int brightness = docq["brightness"]; 
    scenario.brightness = brightness;
    
    runScenario(scenario,true);//TODO от какого из датчиков пришел сигнал

}

String cutFromInputAndGetLastMEssage(String input){
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
          return arr[arr.size()-1];
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

