#include <stdio.h>
	#include <stdlib.h>
	#include <wiringPi.h>
	
	#define TRUE 1
	
	#define TRIG 0
	#define ECHO 3
	#define TIEMPO_MAXIMO 3738
	
	void setup() {
	        wiringPiSetup();
	        pinMode(TRIG, OUTPUT);
	        pinMode(ECHO, INPUT);
	

	        digitalWrite(TRIG, LOW);
	        delay(30);
	}
	
	float getCM() {
			long startTimegetCM = micros();
			//para generar un pulso limpio ponemos a HIGH 4us
	        digitalWrite(TRIG, HIGH); 
	        delayMicroseconds(4);
	        digitalWrite(TRIG, LOW);
			delayMicroseconds(10);

	        while (digitalRead(ECHO) == LOW);
	

	        long startTime = micros();
	        while ((digitalRead(ECHO) == HIGH) && ((micros() - startTime)< TIEMPO_MAXIMO));

	        long travelTime = micros() - startTime;

	        float distance = 0.0f;
			if (travelTime < TIEMPO_MAXIMO){
		        distance = (float) travelTime / 58.0f;
				long endTimegetCM = micros();
				printf("| %ld us ", endTimegetCM - startTimegetCM);
			}

	        return distance;
	}
	
	int main(void) {
	        setup();
			for (int i = 0; i < 1000; ++i)
			{
	        	printf("| %fcm|\n", getCM());
	        	delay(200);
			}
	
	        return 0;
	}
// como maximo se va a medir 64 cm que son 64 * 58.4 = 3738us
// | tiempo     |  distancia |
// |------------|------------|
// 	| 0.000000cm|
// | 0.000000cm|
// | 1151 us | 11.655172cm|
// | 735 us | 4.482759cm|
// | 728 us | 4.344828cm|
// | 702 us | 3.982759cm|
// | 698 us | 3.862069cm|
// | 673 us | 3.413793cm|
// | 671 us | 3.396552cm|
// | 679 us | 3.500000cm|
// | 646 us | 2.982759cm|
// | 677 us | 3.517241cm|
// | 687 us | 3.672414cm|
// | 673 us | 3.413793cm|
// | 823 us | 5.913793cm|
// | 764 us | 5.000000cm|
// | 839 us | 6.310345cm|
// | 885 us | 7.120690cm|
// | 928 us | 7.810345cm|
// | 961 us | 8.396552cm|
// | 1049 us | 9.913794cm|
// | 1104 us | 10.844828cm|
// | 1127 us | 11.258620cm|
// | 1167 us | 11.982759cm|
// | 1187 us | 12.327586cm|
// | 1203 us | 12.551724cm|
// | 1256 us | 13.465517cm|
// | 1315 us | 14.500000cm|
// | 1404 us | 16.051723cm|
// | 1537 us | 18.327587cm|
// | 1654 us | 20.293104cm|
// | 1724 us | 21.534483cm|
// | 1829 us | 23.327587cm|
// | 1946 us | 25.362068cm|
// | 2002 us | 26.379311cm|
// | 2074 us | 27.568966cm|
// | 2109 us | 28.172413cm|
// | 2182 us | 29.465517cm|
// | 2236 us | 30.396551cm|
// | 2319 us | 31.534483cm|
// | 2386 us | 32.965519cm|
// | 2491 us | 34.793102cm|
// | 2589 us | 36.500000cm|
// | 2618 us | 36.931034cm|
// | 2664 us | 37.741379cm|
// | 2732 us | 38.896553cm|
// | 2873 us | 41.344826cm|
// | 2885 us | 41.551723cm|
// | 2942 us | 42.534481cm|
// | 3081 us | 44.948277cm|
// | 3105 us | 45.362068cm|
// | 3184 us | 46.724136cm|
// | 3327 us | 49.189655cm|
// | 3286 us | 48.500000cm|
// | 3456 us | 51.396553cm|
// | 3352 us | 49.637932cm|
// | 3344 us | 49.413792cm|
// | 3379 us | 50.086208cm|
// | 0.000000cm|
// | 3594 us | 53.775864cm|
// | 3599 us | 53.913792cm|
// | 3703 us | 55.637932cm|
// | 3594 us | 53.775864cm|
// | 0.000000cm|
// | 0.000000cm|
// | 0.000000cm|
// | 3901 us | 59.068966cm|
// | 3992 us | 60.620689cm|
// | 0.000000cm|
// | 0.000000cm|
// | 0.000000cm|
// | 0.000000cm|
// | 0.000000cm|
// | 0.000000cm|
// | 0.000000cm|
// | 3134 us | 45.862068cm|
// | 0.000000cm|
// | 2581 us | 36.327587cm|
// | 1999 us | 26.275862cm|
// | 1445 us | 16.706896cm|
// | 1205 us | 12.603448cm|
// | 0.000000cm|
// | 0.000000cm|
