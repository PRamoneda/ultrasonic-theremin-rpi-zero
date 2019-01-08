#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
// MIDI
#include <stdio.h>
#include <portmidi.h>
#include <porttime.h>

// #define DEBUGGING 1
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
#define SAMPLE_RATE 200 // va en milisegundos
#define SAMPLE_RATE_2 SAMPLE_RATE/2 // va en milisegundos
#define SAMPLE_RATE_4 SAMPLE_RATE/4 // va en milisegundos
 
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
        float distance = (float) travelTime / 58.0f; // Si se baja los "cm" se hacen mas grandes . 58 es el normal
 
        return distance;
}

int getIntensidad(){
        int ans = 0;
        float intensidad = getCM(INTENSIDAD);
        if ( 6.0 < intensidad && intensidad < 36.0 ){
                ans = (int) (intensidad - 5) * 4;
        }
        return ans;
}

int getAlturaInit( float *distanciaNota){
        int ans = 0;
        *distanciaNota = getCM(ALTURA);
        if ( 6.0 < *distanciaNota && *distanciaNota < 50.0 ){
                ans = (*distanciaNota + 46);
        }
        return ans;
}

int filtroAltura( float distanciaNota){
        int ans = 0;
        if ( 6.0 < distanciaNota && distanciaNota < 50.0 ){
                ans = ((int) distanciaNota + 46);
        }
        return ans;
}

void readNotesInit(int *note, int *velocity, float *distanciaNota){
        *note = getAlturaInit(distanciaNota);
        *velocity = getIntensidad();
}

int readNotes( int *note, int *velocity){
        float newIntervalo =   getCM(ALTURA) - *distanciaNota;
        #ifdef DEBUGGING
                        printf("NOW //// intervalo: %f \n", newIntervalo);
        #endif
        int ans = 0;
        if ( -1.0f > newIntervalo || newIntervalo > 1.0f)
        {
                int nowV = getIntensidad();
                int nowN = filtroAltura(*distanciaNota + newIntervalo);
                #ifdef DEBUGGING
                        printf("NOW //// V: %d, N %d  \n", nowV, nowN);
                #endif
                ans = ((nowN != 0 && nowN != *note) || (nowV != 0 && nowV != *velocity));
                if(ans){
                        *note = nowN;
                        *velocity = nowV;
                        *distanciaNota = newIntervalo + *distanciaNota;
                }
                
        }
        return ans;
}

void play(PortMidiStream *mstream, int* lastNote, int* lastVelocity, int newNote, int newVelocity ){

                #ifdef DEBUGGING
                        printf("SONANDO?? V: %d, N %d  \n", newVelocity, newNote);
                #else
                char chan = 0;

                //Pm_WriteShort(mstream, 0, Pm_Message(SBYTE(MD_PRG,chan), prg, 0));      
                Pm_WriteShort(mstream, 0, Pm_Message(SBYTE(MD_NOTEOFF,chan), *lastNote, *lastVelocity));
                delay(1);
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
        int velocity = 0, note = 0,lastNote = 0, lastVelocity = 0, prg = 0, playIt = 1;
        readNotesInit( &note, &velocity, &distanciaNota);
        // Program 
        while (prg < 1600) {
                if (playIt) 
                        play(mstream, &lastNote, &lastVelocity, note, velocity);
                delay(SAMPLE_RATE_2);
                playIt = readNotes(&note, &velocity, &distanciaNota);
                prg++;
        }
 
        return 0;
}