
# Raspbian setup  

Para raspberry normal

https://randomnerdtutorials.com/installing-raspbian-lite-enabling-and-connecting-with-ssh/

Sin teclado ni pantalla. Setup del wifi.

https://howchoo.com/g/ndy1zte2yjn/how-to-set-up-wifi-on-your-raspberry-pi-without-ethernet

Para conectarse por ssh:

Escaneo mi red local con Angry Ip Scanner para ver en que dirección está la raspberry.

```
ssh pi@10.231.204.177
contraseña: raspberry
```

Se configura como un dispositivo midi el USB OTG.

https://raspberrypi.stackexchange.com/questions/38576/can-the-pi-zero-act-as-an-usb-peripheral-device/40626#40626

Para comprobar que se detecta como midi desde fuera.
```
ioreg -p IOUSB
```



## Jugando con midi

### portMidi en Mac

Instalación:

```
brew install portMidi
```

Tras instalar portMidi en Mac....

```
gcc -o prg pmidiout.c -I/usr/local/include -L/usr/local/lib -lportmidi
```

### portMidi en Linux


Instalación:

```
apt install libportmidi-dev
```

Ejecución:
```C
#include <stdio.h>
#include <portmidi.h>
#include <porttime.h>


#define MD_NOTEON 0x90
#define MD_NOTEOFF 0x80
#define MD_PRG  0xC0
#define SBYTE(mess,chan) mess | chan

int main() {

int cnt,i,dev;
PmError retval;  
const PmDeviceInfo *info;
PortMidiStream *mstream;

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
  retval = Pm_OpenOutput(&mstream, dev, 
           NULL,512,NULL,NULL,0);
  
  if(retval != pmNoError)    
     printf("error: %s \n", Pm_GetErrorText(retval));
  else {
    char chan = 0;
    int  prg = 0;
    long time = 0;
    for(i=60; i < 72; prg+=4, i++){
      Pm_WriteShort(mstream, 0, 
         Pm_Message(SBYTE(MD_PRG,chan), prg, 0));
      time = Pt_Time(NULL);
      Pm_WriteShort(mstream, 0, 
         Pm_Message(SBYTE(MD_NOTEON,chan), i, 120));
      while(Pt_Time(NULL) - time < 1000);
      Pm_WriteShort(mstream, 0, 
         Pm_Message(SBYTE(MD_NOTEOFF,chan), i, 120));
    }
  }
  Pm_Close(mstream);  
} else printf("No available output devices\n");

Pm_Terminate();
return 0;
}

```

```
gcc -o prg prueba1midi.c -I/usr/local/include -L/usr/local/lib -lportmidi
```

```
0: Midi Through Port-0 
2: f_midi 
choose device: 2
```

## Para probar sensor ultrasonico

Instalar Bcm2385.h
```
sudo apt-get install html-xml-utils
mkdir -p bcm2835 && (wget -qO - `curl -sL http://www.airspayce.com/mikem/bcm2835 | hxnormalize -x -e | hxselect -s '\n' -c "div.textblock>p:nth-child(4)>a:nth-child(1)"` | tar xz --strip-components=1 -C bcm2835 )
cd bcm2835
./configure
make
sudo make install
```
Connect the trigger pin of the HC-SR04 sensor to Pin 11 (GPIO 17) on the raspberry pi and connect the echo pin on the sensor to Pin 15 (GPIO 22) on the pi. Also connect +5V and GND either from the pi or an external power supply.

```
gcc -o prg main.c HC_SR04.c -l bcm2835
```

**lo de arriba no funciona.... PlanB**

### WiringPi
```Console
gpio -v
sudo apt-get install git-core
sudo apt-get update
sudo apt-get upgrade
cd
git clone git://git.drogon.net/wiringPi
cd ~/wiringPi
git pull origin
cd ~/wiringPi
./build
gpio -v
gpio readall

```

NOTE: To compile programs with wiringPi, you need to add:
    -lwiringPi

```C
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
 
#define TRUE 1
 
#define TRIG 11
#define ECHO 15
 
void setup() {
        wiringPiSetup();
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);
 
        //TRIG pin must start LOW
        digitalWrite(TRIG, LOW);
        delay(30);
}
 
int getCM() {
        //Send trig pulse
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG, LOW);
 
        //Wait for echo start
        while(digitalRead(ECHO) == LOW);
 
        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHO) == HIGH);
        long travelTime = micros() - startTime;
 
        //Get distance in cm
        int distance = travelTime / 58;
 
        return distance;
}
 
int main(void) {
        setup();
 
        printf("Distance: %dcm\n", getCM());
 
        return 0;
}
```

## Con dos ultrasonic sensores

##### HC SR04 - 1

Pin 11 - (GPIO 17) - Wiring PI 0 TRIG

Pin 15 - (GPIO 22) - Wiring PI 3 ECHO

ground -> pin 6
vcc    -> pin 2

##### HC SR04 - 1

Pin 32 - (GPIO 12) - Wiring PI 26

Pin 36 - (GPIO 16) - Wiring PI 27

ground -> pin 39
vcc    -> pin 4

##### HC SR04 - 1

## Posibles mejoras

- Meterle vibratto ya sea por un controlador midi o por el paper http://www-classes.usc.edu/engr/ise/599muscog/2004/projects/yang/ 
- Que vaya por un glisando hasta la sguiente nota 

##### Utilities

```Console

scp ./prueba1midi.c pi@10.231.204.177:prueba.c


```