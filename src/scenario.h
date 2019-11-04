#include <FastLED.h>
#define NUM_LEDS 144
#define LED_PER_STAIR_COUNT 18
#define STAIRS_COUNT 8


struct Scenario{
    String name;
    int delayPerChair;
    int delayChairs;
    int delayAfter;
    int red;
    int green;
    int blue;
    int brightness;
};

struct btw_thread{
    int chair;
    Scenario scenario;
    bool isFinishing;
    bool isForward;
};

pthread_mutex_t mutex;

Scenario runningScenario;
Scenario thread_scenario;
CRGB leds[NUM_LEDS];

bool needRunScenario = false;
bool startfrom = false;
bool NEED_CLEAR_DELAY = false;
bool NEED_STOP = false;
pthread_t scenarioThread;
pthread_t threads[STAIRS_COUNT];

void printBTW(btw_thread btw);
int smartDelay(long mydelay);
void * scenarioThreadFunc(void *scenarioLoop);
void runScenario(Scenario scenario,bool _startFrom );
void runScenarioNoEffect(bool forward,Scenario scenario,bool isFinish);


void runScenarioSnake(bool forward,Scenario scenario,bool isFinish);
void *strangeSnakeOnChair(void *threadid);

void setBlackAll();

void * scenarioThreadFunc(void *scenarioLoop){
     while (1 > 0){
    if (needRunScenario){ 
        runningScenario.brightness = 10; //TODO 
        needRunScenario = false;
        if (runningScenario.name == "noeffect"){
            runScenarioNoEffect(startfrom,runningScenario,false);
            if (smartDelay(runningScenario.delayAfter)==0){
                runScenarioNoEffect(!startfrom,runningScenario,true);
            }
        } else
        
        if (runningScenario.name == "snake"){
            runScenarioSnake(startfrom,runningScenario,false);
            if (smartDelay(runningScenario.delayAfter)==0){
                runScenarioSnake(!startfrom,runningScenario,true);
            }
        } else
        
        if (runningScenario.name == "ltor"){

        } else
        
        if (runningScenario.name == "rtol"){
        
        } else
        if (runningScenario.name == "toceneter"){
        }else

         if (runningScenario.name == "strange_snake"){
           /* runScenarioStrangeSnake(startfrom,runningScenario,false);
            if (smartDelay(runningScenario.delayAfter)==0){
                runScenarioStrangeSnake(startfrom,runningScenario,true);
            }*/
        } 
        else {
            Serial.print("No available scenarios with name ");
            Serial.println(runningScenario.name);
        } 
    }
    delay(200);
    }
}



void runScenario(Scenario scenario,bool _startFrom ){    
    runningScenario = scenario;
    needRunScenario = true;
    startfrom =_startFrom;
}



int smartDelay(long mydelay){
    Serial.print("Smart delay starts with ");
    Serial.println(mydelay);

    int time = millis();
    Serial.print("started time = ");
    Serial.println(time);

    Serial.print("first  (millis - time) ");
    Serial.println(millis() - time);

    while ( (millis() - time) < mydelay){
        delay(20);
        if (NEED_CLEAR_DELAY)
            time = millis();
        if (NEED_STOP)
            return -1;
    Serial.print(" (millis - time) ");
    Serial.println((millis() - time));
       

    }
    return 0;
}


void runScenarioNoEffect(bool forward,Scenario scenario,bool isFinish){
   for (int i = 0; i < NUM_LEDS; i++){
       if (isFinish){
            leds[i].setRGB(0,0,0);
       }
       else{
            leds[i].setRGB(scenario.red,scenario.green,scenario.blue);
       }
  }
  FastLED.setBrightness(10);
   FastLED.show();
}
void * scenarioThreadFunc(void *scenarioLoop);


void runScenarioSnake(bool forward,Scenario scenario,bool isFinish){

 for (int i = 0; i < STAIRS_COUNT ; i++){
     btw_thread btw;
     btw.chair = i;
     btw.isFinishing = isFinish;
     btw.scenario = scenario;
     btw.isForward = forward;
    pthread_create(&threads[i], NULL, strangeSnakeOnChair, (void *)&btw);
    pthread_join(threads[i],NULL);

    delay(scenario.delayChairs);
  }
}
/*
void runScenarioStrangeSnake(bool forward,Scenario scenario,bool isFinish){
    thread_scenario = scenario;
 for (int i = 0; i < STAIRS_COUNT ; i++){
     btw_thread btw;
     btw.chair = i;
     btw.isFinishing = isFinish;
     btw.scenario = scenario;
     btw.isForward = forward;
    pthread_create(&threads[i], NULL, strangeSnakeOnChair, (void *)&btw);
    delay(scenario.delayChairs);
  }

   for (int i = 0; i < STAIRS_COUNT ; i++){
      pthread_join(threads[i],NULL);
  }

}*/

void printBTW(btw_thread btw){

    Serial.print("btw thread out - chair = ");
    Serial.print(btw.chair);
    Serial.print("scenario name = ");
    Serial.print(btw.scenario.name);
    Serial.print(" is finishing = ");
    Serial.print(btw.isFinishing);
    Serial.print(" is forward = ");
    Serial.print(btw.isForward);
    Serial.print("rgb (");
    Serial.print(btw.scenario.red);
    Serial.print(",");
    Serial.print(btw.scenario.green);
    Serial.print(",");
    Serial.print(btw.scenario.blue);
    Serial.print(") bright = ");
    Serial.print(btw.scenario.brightness);
    Serial.print("delays , delayPerChair = ");
    Serial.print(btw.scenario.delayPerChair);
    Serial.print("delays , delayBetw = ");
    Serial.print(btw.scenario.delayChairs);
    Serial.print("delays , delayAfter = ");
    Serial.print(btw.scenario.delayAfter);

}

void *strangeSnakeOnChair(void *threadid) {    
    btw_thread btw;
    memcpy(&btw,threadid,sizeof(btw_thread));
    printBTW(btw);
    int nstair = btw.chair;
    int startLed = nstair*LED_PER_STAIR_COUNT;
        for (int led = startLed; led <= (startLed + LED_PER_STAIR_COUNT) ;led ++){
           // pthread_mutex_lock(&mutex);

           if (!btw.isFinishing){
                 Serial.println("before set");
                 leds[led] .setRGB( btw.scenario.red, btw.scenario.green, btw.scenario.blue);
                 FastLED.setBrightness(btw.scenario.brightness);
                 FastLED.show();

           } else {
                 leds[led] .setRGB( 0, 0, 0);
                 //FastLED.setBrightness(0);
                 FastLED.show();
           }
          
            Serial.print("Thread = ");
            Serial.print((int) threadid);
            Serial.print(" led num = ");
            Serial.println((int)led);                 
            //pthread_mutex_unlock(&mutex);
            delay(btw.scenario.delayPerChair);
        }
     pthread_exit (NULL);
    //Serial.println((int)threadid);
}



void setBlackAll(){
  for (int i = 0; i < NUM_LEDS; i++){
    //leds[i].setRGB(0,0,0);
  }
   //FastLED.show();
}
