#include <Arduino.h>

#define VICTORY_LENGTH 1000                        
#define EXPLOSION_LENGTH 5000                 
#define GAMELENGTH 40000              // in milliseconds
#define BEEP_LENGTH 100
#define BEEP_DELAY 2100 
#define BEEP_DECREASE (int)(GAMELENGTH/(BEEP_DELAY-100)*100)      

#define MIN_PIN_NUMBER 24
#define MAX_PIN_NUMBER 43


#define WRONG_PIN 19


#define LED LED_BUILTIN
#define SIREN 50
#define RESTART_PIN 53


int startTime;
int beepTime = 0;
int beepDecTimer = 0;
int beepDelay = BEEP_DELAY;
const int CORRECT_WIRES[5] = {29,27,25,23,21};
bool gameOver = false;


void setup() {
  for (int i = MIN_PIN_NUMBER; i < MAX_PIN_NUMBER; i += 2) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(LED, OUTPUT);
  pinMode(SIREN, OUTPUT);
  pinMode(RESTART_PIN, INPUT_PULLUP);
  restart();
}

void restart(){
  startTime = millis();
  gameOver = false;
  beepTime = 0;
  beepDecTimer = 0;
}

void loop() {
  if (digitalRead(RESTART_PIN) == LOW) {
    restart();
  }
  if(gameOver) return;

  if (millis() - startTime > GAMELENGTH || digitalRead(WRONG_PIN) == LOW) {
    gameOver = true;
    digitalWrite(LED, HIGH);
    digitalWrite(SIREN, HIGH);
    delay(EXPLOSION_LENGTH);
    digitalWrite(LED, LOW);
    digitalWrite(SIREN, LOW);
  }

  for(int i = 0; i < 5; i++){
    if(digitalRead(CORRECT_WIRES[i]) == LOW){
      gameOver = true;
      digitalWrite(LED, HIGH);
      digitalWrite(SIREN, HIGH);
      delay(VICTORY_LENGTH);
      digitalWrite(SIREN, LOW);
    }
  }

  beepTime += millis() - beepTime;
  beepDecTimer += millis() - beepDecTimer;
  if(beepDecTimer > BEEP_DECREASE){
    beepDelay -= 100;
    beepDecTimer = 0;
  }
  if(beepTime > beepDelay){
    digitalWrite(SIREN, HIGH);
    digitalWrite(LED, HIGH);
    delay(BEEP_LENGTH);
    digitalWrite(SIREN, LOW);
    digitalWrite(LED, LOW);
    beepTime = 0;
  }
}