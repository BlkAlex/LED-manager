/*
#include "scenario.h"
#include <pthread.h>
extern Scenario runningScenario;
extern Scenario thread_scenario;
extern pthread_t threads[STAIRS_COUNT];
extern CRGB leds[NUM_LEDS];



void runScenario(Scenario scenario,bool _startFrom ){    
    runningScenario = scenario;
    //memcpy(&outleds, &_leds, sizeof(outleds));
    needRunScenario = true;
    startfrom =_startFrom;
}





int smartDelay(int mydelay){
    int time = millis();
    while ( (int)(millis - time) < mydelay){
        delay(20);
        if (NEED_CLEAR_DELAY)
            time = millis();
        if (NEED_STOP)
            return -1;
        if (needRunScenario)
            return 0;
    }
    return 0;
}


void runScenarioNoEffect(bool forward,Scenario scenario){
   for (int i = 0; i < NUM_LEDS; i++){
    leds[i].setRGB(scenario.red,scenario.green,scenario.blue);
  }
  FastLED.setBrightness(scenario.brightness);
   FastLED.show();
}

void runScenarioSnake(bool forward,Scenario scenario){
    thread_scenario = scenario;
 for (int i = 0; i < STAIRS_COUNT ; i++){
    pthread_create(&threads[i], NULL, snakeOnChair, (void *)i);
    delay(scenario.delayChairs);
  }

   for (int i = 0; i < STAIRS_COUNT ; i++){
      pthread_join(threads[i],NULL);
  }

}


void *snakeOnChair(void *threadid) {
  int nstair = (int)threadid;
  int startLed = nstair*LED_PER_STAIR_COUNT;
        for (int led = startLed; led <= (startLed + LED_PER_STAIR_COUNT) ;led ++){
            leds[led] .setRGB( thread_scenario.red, thread_scenario.green, thread_scenario.blue);
            FastLED.setBrightness(thread_scenario.brightness);
            FastLED.show();
            Serial.print("Thread = ");
            Serial.print((int) threadid);
            Serial.print(" led num = ");
            Serial.println((int)led);
            delay(thread_scenario.delayPerChair);
        }

    //Serial.println((int)threadid);
}*/