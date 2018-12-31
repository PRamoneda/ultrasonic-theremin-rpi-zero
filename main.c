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


#define ALTURA 1
#define INTENSIDAD 0
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

int getIntensidad(){
        int ans = 0;
        float intensidad = getCM(INTENSIDAD);
        if ( 6.0 < intensidad && intensidad < 38.0 ){
                ans = intensidad + 80;
        }
        return ans;
}

int getAltura(){
        int ans = 0;
        float altura = getCM(ALTURA);
        if ( 6.0 < altura && altura < 38.0 ){
                ans = altura + 56;
        }
        return ans;
}
// refactorizar a uno para read y otro para play
void readNotes(PortMidiStream *mstream, int *lastNote, int *lastVelocity, int prg){
        int newNote = getIntensidad();
        int newVelocity = getAltura();

        if ((newNote != 0 && newNote != *lastNote) || (newVelocity != 0 && newVelocity != *lastVelocity) ){
                printf("SONANDO?? V: %d, N %d\n", newVelocity, newNote);
                char chan = 0;
                Pm_WriteShort(mstream, 0, Pm_Message(SBYTE(MD_PRG,chan), prg, 0));
                Pm_WriteShort(mstream, 0, Pm_Message(SBYTE(MD_NOTEOFF,chan), *lastNote, *lastVelocity));
                Pm_WriteShort(mstream, 0, Pm_Message(SBYTE(MD_NOTEON,chan), newNote, newVelocity));

                *lastNote = newNote;
                *lastVelocity = newVelocity;         
        }
}
 
int main(void) {
        // setup
        setupWiring();
        PortMidiStream* mstream;
        mstream = setupMIDI();
        delay(5000);
        // Declare Variables
        int velocity = 0, note = 0, prg = 0;
        // Program 
        while (prg < 800)
        {
                readNotes(mstream, &note, &velocity, prg);
                prg = prg + 4; //avanza 4 bytes el MIDI
                delay(SAMPLE_RATE);
        }
 
        return 0;
}