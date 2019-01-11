#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
// MIDI
#include <stdio.h>
#include <portmidi.h>
#include <porttime.h>

// #define DEBUGGING 1
// #define DEBUGGING_SAMPLE_RATE 1
#define MD_NOTEON 0x90
#define MD_NOTEOFF 0x80
#define MD_PRG  0xC0
#define SBYTE(mess,chan) mess | chan
//FIN MIDI
 

// el dispositivo 0 es la altura y el dispositivo 1 la intensidad
const int numDispositivos = 2;
const int TRIG[2] = {0, 26};
const int ECHO[2] = {3, 27};


#define ALTURA 0
#define INTENSIDAD 1
#define SAMPLE_RATE 10000 // va en us
#define SAMPLE_RATE_2 SAMPLE_RATE/2 // va en us
#define SAMPLE_RATE_4 SAMPLE_RATE/4 // va en us
#define TIEMPO_MAXIMO 3738
#define TIEMPO_200_MAXIMO 3738 + 200
 
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

PortMidiStream * setupMIDI(){
        int cnt,i,dev;
        PmError retval;  
        const PmDeviceInfo *info;
        PortMidiStream* mstream;

        Pm_Initialize();
  
        if((cnt = Pm_CountDevices())){  
                for(i=0; i < cnt; i++){
                        info = Pm_GetDeviceInfo(i);
                        if(info->output)
                                printf("%d: %s \n", i, info->name);
                        }
                printf("choose device: ");
                scanf("%d", &dev); 
                Pt_Start(1, NULL, NULL);
                retval = Pm_OpenOutput(&mstream, dev, NULL,512,NULL,NULL,0);
          
                if(retval != pmNoError){
                        printf("error: %s \n", Pm_GetErrorText(retval));
                        exit(1);        
                }   
        } else {
                printf("No available output devices\n");
                exit(1);
        }
        return mstream;
}

void closeMIDI(PortMidiStream *mstream){
        Pm_Close(mstream);
        Pm_Terminate();
}


int getCM(int numHC) {

        //para generar un pulso limpio ponemos a HIGH 4us
        digitalWrite(TRIG[numHC], HIGH); 
        delayMicroseconds(4);
        digitalWrite(TRIG[numHC], LOW);
        // delayMicroseconds(10);
        while (digitalRead(ECHO[numHC]) == LOW);

        long startTime = micros();
        while ((digitalRead(ECHO[numHC]) == HIGH) && ((micros() - startTime) < TIEMPO_MAXIMO));

        long travelTime = micros() - startTime;
        while ((micros() - startTime) < TIEMPO_200_MAXIMO);

        int distance = 0;
        if (travelTime < TIEMPO_MAXIMO){
                distance = travelTime / 58;
        }

        return distance;
}

int filtroIntensidad(int unidades){
        int ans = unidades * 2 + 40;
        return (ans >125)? 125: ans;
}


int filtroAltura( int unidades){
        return (unidades != 0)? (((unidades * 3) / 2/* / 1.5*/) + 30): 0;
}


int readNotes( int *note, int *velocity){
        
        int nowV = filtroIntensidad(getCM(INTENSIDAD));
        int nowN = filtroAltura(getCM(ALTURA));
        #ifdef DEBUGGING
                printf("NOW //// V: %d, N %d  \n", nowV, nowN);
        #endif
        int ans = ((nowN != 0 && nowN != *note)); //(nowV != 0 && nowV != *velocity));
        if(ans){
                *note = nowN;
                *velocity = nowV;
        }
                
        return ans;
}

void play(PortMidiStream *mstream, int* lastNote, int* lastVelocity, int newNote, int newVelocity, int prg){

                #ifdef DEBUGGING
                        printf("SONANDO?? V: %d, N %d  \n", newVelocity, newNote);
                #else
                char chan = 0;

                Pm_WriteShort(mstream, 0, Pm_Message(SBYTE(MD_PRG,chan), prg, 0));      
                //Pm_WriteShort(mstream, 0, Pm_Message(SBYTE(MD_NOTEOFF,chan), *lastNote, *lastVelocity));
                Pm_WriteShort(mstream, 0, Pm_Message(SBYTE(MD_NOTEON,chan), newNote, newVelocity));  
                *lastNote = newNote;
                *lastVelocity = newVelocity;
                #endif

}

void cuentaAtras(){
        for (int i = 5; i >= 0; i--){
                printf("%d....\n", i);
                delay(1000);
        }
}
 
int main(void) {
        // setup
        setupWiring();
        PortMidiStream* mstream;
        mstream = setupMIDI();
        cuentaAtras();
        // Declare Variables
        int velocity = 0, note = 0,lastNote = 0, lastVelocity = 0, prg = 0, playIt = 0;
        // Program 
        while (prg < 160000) {
                long startTimeSampleRate = micros();
                playIt = readNotes(&note, &velocity);
                if (playIt) 
                        play(mstream, &lastNote, &lastVelocity, note, velocity, prg);

                #ifdef DEBUGGING_SAMPLE_RATE
                        long endTimeSampleRate = micros();
                        printf("| %ld us \n", endTimeSampleRate - startTimeSampleRate);
                #endif
                while ( ((micros() - startTimeSampleRate) < SAMPLE_RATE));
                prg+=4;
        }
 
        return 0;
}