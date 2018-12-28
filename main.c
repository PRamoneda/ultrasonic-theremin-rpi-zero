#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
// MIDI
#include <stdio.h>
#include <portmidi.h>
#include <porttime.h>


#define MD_NOTEON 0x90
#define MD_NOTEOFF 0x80
#define MD_PRG  0xC0
#define SBYTE(mess,chan) mess | chan
//FIN MIDI
 
// #define TRIG1 0
// #define ECHO1 3

// #define TRIG2 26
// #define ECHO2 27

// el dispositivo 0 es la altura y el dispositivo 1 la intensidad
const int numDispositivos = 2;
const int TRIG[2] = {0, 26};
const int ECHO[2] = {3, 27};


#define ALTURA 0
#define INTENSIDAD 1
#define SAMPLE_RATE 500 // va en milisegundos


 
void setupWiring() {
        wiringPiSetup();
        for (int i = 0; i < numDispositivos; ++i)
        {
                pinMode(TRIG[i], OUTPUT);
                pinMode(ECHO[i], INPUT);

                //TRIG1 pin must start LOW
                digitalWrite(TRIG[i], LOW);
                delay(30);
        }
}
 
float getCM(int numHC) {
        //Send trig1 pulse
        digitalWrite(TRIG[numHC], HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG[numHC], LOW);
 
        //Wait for echo1 start
        while(digitalRead(ECHO[numHC]) == LOW);
 
        //Wait for echo1 end
        long startTime = micros();
        while(digitalRead(ECHO[numHC]) == HIGH);
        long travelTime = micros() - startTime;
 
        //Get distance in cm
        float distance = travelTime / 58.0;
 
        return distance;
}

void leerNotas(){
         calcularIntensidad();
         calcularAltura();
}
 
int main(void) {
        setupWiring();
        delay(5000);
        for (int i = 0; i < 20; ++i)
        {
                leerNotas();
                delay(SAMPLE_RATE);
        }
 
        return 0;
}