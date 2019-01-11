# Trabajo de la asignatura Sistemas Empotrados 2

## Audio, Linux y tiempo real.

La idea del trabajo era hacer una primera aproximación a un sistema de audio en tiempo real y con sistema operativo. Por ello, lo mas sencillo para hacer un sistema completo ha sido hacer un controlador MIDI. En concreto se ha hecho un theremin MIDI. Lo bueno de usar este protocolo es que desacopla la activación del sonido a la sintetizacion del sonido. Dicho de otra manera, que por una parte se pueden hacer una serie de controladores desde los cuales calcular las variables del sonido, mandar el sonido a otro dispositivo por medio de MIDI y este último dispositivo que sintetice el sonido como sea debido.

Desde el punto de vista del intérprete se han de conseguir buenos tiempos de reacción de los instrumentos electrónicos, para tener un buen manejo y percepción del sonido. En concreto los tiempos de latencia han de estar entre 15ms y 30 ms para que se pueda considerar "tiempo real". Por eso, este era el principal requisito. Adjunto un artículo de Ge Wang sobre la creación de nuevos instrumentos electronicos musicales.

## Theremin

El theremin, ollamado en un principio eterófono, thereminófono, termenvox o thereminvox,es considerado uno de los primeros instrumentos musicales electrónicos. Se controla sin que haya contacto físico entre el intérprete y el instrumento.

El instrumento está formado por dos antenas metálicas que detectan la posición relativa de las manos del thereminista y los osciladores para controlar la frecuencia (altura) con una mano y la amplitud (volumen o intensidad) con la otra. Las señales eléctricas se amplifican por medio de un amplificador y son emitidas por altavoces.

Se ha elegido este instrumento por su relativa sensillez. Así como la librería **portMidi y wiringPi** . Aunque a posteriori, habría sido interesante utilizar el framework JUCE https://www.youtube.com/watch?v=jq7zUKEcyzI .


En mi caso solamente he desactivado los servicios que tenía instalados. Ya que utilizaba Raspbian Lite y muchas cosas  no las tenía.

Fuente: https://wiki.linuxaudio.org/wiki/raspberrypi

```Console
## Stop the ntp service
sudo service ntp stop

## Stop the triggerhappy service
sudo service triggerhappy stop

## Stop the dbus service. Warning: this can cause unpredictable behaviour when running a desktop environment on the RPi
sudo service dbus stop

## Stop the console-kit-daemon service. Warning: this can cause unpredictable behaviour when running a desktop environment on the RPi
sudo killall console-kit-daemon

## Stop the polkitd service. Warning: this can cause unpredictable behaviour when running a desktop environment on the RPi
sudo killall polkitd

## Only needed when Jack2 is compiled with D-Bus support (Jack2 in the AutoStatic RPi audio repo is compiled without D-Bus support)
#export DBUS_SESSION_BUS_ADDRESS=unix:path=/run/dbus/system_bus_socket

## Remount /dev/shm to prevent memory allocation errors
sudo mount -o remount,size=128M /dev/shm

## Kill the usespace gnome virtual filesystem daemon. Warning: this can cause unpredictable behaviour when running a desktop environment on the RPi
killall gvfsd

## Kill the userspace D-Bus daemon. Warning: this can cause unpredictable behaviour when running a desktop environment on the RPi
killall dbus-daemon

## Kill the userspace dbus-launch daemon. Warning: this can cause unpredictable behaviour when running a desktop environment on the RPi
killall dbus-launch

## Uncomment if you'd like to disable the network adapter completely
#echo -n “1-1.1:1.0” | sudo tee /sys/bus/usb/drivers/smsc95xx/unbind
## In case the above line doesn't work try the following
#echo -n “1-1.1” | sudo tee /sys/bus/usb/drivers/usb/unbind

## Set the CPU scaling governor to performance
echo -n performance | sudo tee /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
```




## Raspbian setup  

Para raspberry normal

https://randomnerdtutorials.com/installing-raspbian-lite-enabling-and-connecting-with-ssh/

Sin teclado ni pantalla. Setup del wifi.

https://howchoo.com/g/ndy1zte2yjn/how-to-set-up-wifi-on-your-raspberry-pi-without-ethernet

Para conectarse por ssh:

Escaneo mi red local con Angry Ip Scanner para ver en que dirección está la raspberry.

```
  
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
sudo apt install libportmidi-dev
```

Ejecución:

Los fuentes están en el repositorio de github.

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

En un principio se exploró la idea de utilizar esta librería, pero se desechó debido al gran numero de problemas que causaba y su alto tiempo de respuesta.

```
sudo apt-get install html-xml-utils
mkdir -p bcm2835 && (wget -qO - `curl -sL http://www.airspayce.com/mikem/bcm2835 | hxnormalize -x -e | hxselect -s '\n' -c "div.textblock>p:nth-child(4)>a:nth-child(1)"` | tar xz --strip-components=1 -C bcm2835 )
cd bcm2835
./configure
make
sudo make install
```

```
gcc -o prg main.c HC_SR04.c -l bcm2835
```

**La libreria de arriba va como el culo.... PlanB**

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

## Con dos sensores de ultrasonido HC-SR04. Mapa de conexiones.

##### HC SR04 - 1

Pin 11 - (GPIO 17) - Wiring PI 0 TRIG

Pin 15 - (GPIO 22) - Wiring PI 3 ECHO

ground -> pin 6
vcc    -> pin 2

##### HC SR04 - 1

Pin 32 - (GPIO 12) - Wiring PI 26 TRIG

Pin 36 - (GPIO 16) - Wiring PI 27 ECHO

ground -> pin 39
vcc    -> pin 4

##### HC SR04 - 1

## Posibles mejoras

- Meterle vibratto ya sea por un controlador midi o por el paper http://www-classes.usc.edu/engr/ise/599muscog/2004/projects/yang/ 

- Que vaya por un glisando hasta la siguiente nota (hecha desde ableton)

- Hacer un theremin con CV

- Hacer un theremin con un iPhone y un iWatch

### Compilación del programa

```Console

gcc -o prg main.c -I/usr/local/include -L/usr/local/lib -lportmidi -lwiringPi
```

### Ejecución del programa
```Console

./main 
```



##### Utilidades

```Console

scp ./main.c pi@10.231.204.177:main.c


```

Como hacer un sintetizador que parezca un theremin para Ableton Live.

https://music.tutsplus.com/tutorials/quick-tip-how-to-emulate-a-theremin-sound-in-ableton-live--audio-6198

