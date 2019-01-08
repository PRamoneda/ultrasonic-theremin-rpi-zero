#include <stdio.h>
	#include <stdlib.h>
	#include <wiringPi.h>
	
	#define TRUE 1
	
	#define TRIG 0
	#define ECHO 3
	#define TIEMPO_MAXIMO 3738
	#define TIEMPO_200_MAXIMO 3738 + 200
	
	void setup() {
	        wiringPiSetup();
	        pinMode(TRIG, OUTPUT);
	        pinMode(ECHO, INPUT);
	

	        digitalWrite(TRIG, LOW);
	        delay(30);
	}
	
	int getCM() {
			long startTimegetCM = micros();
			//para generar un pulso limpio ponemos a HIGH 4us
	        digitalWrite(TRIG, HIGH); 
	        delayMicroseconds(4);
	        digitalWrite(TRIG, LOW);
			delayMicroseconds(10);

	        while (digitalRead(ECHO) == LOW);
	

	        long startTime = micros();

	        while ((digitalRead(ECHO) == HIGH) && ((micros() - startTime) < TIEMPO_MAXIMO));

	        long travelTime = micros() - startTime;

	        while ( ((micros() - startTime) < TIEMPO_200_MAXIMO));

	        int distance = 0;
			if (travelTime < TIEMPO_MAXIMO){
		        distance = travelTime / 145;
				long endTimegetCM = micros();
				printf("| %ld us ", endTimegetCM - startTimegetCM);
			}
	        return distance;
	}
	
	int main(void) {
	        setup();
			for (int i = 0; i < 1000; ++i)
			{
	        	printf("| %du ramonicas|\n", getCM());
	        	delay(200);
			}
	
	        return 0;
	}
// como maximo se va a medir 64 cm que son 64 * 58.4 = 3738us
// | tiempo     |  distancia |
// |------------|------------|
// | 4415 us | 2u ramonicas|
// | 4411 us | 1u ramonicas|
// | 4410 us | 1u ramonicas|
// | 4411 us | 1u ramonicas|
// | 4411 us | 1u ramonicas|
// | 4412 us | 1u ramonicas|
// | 4411 us | 1u ramonicas|
// | 4410 us | 1u ramonicas|
// | 4444 us | 1u ramonicas|
// | 4412 us | 1u ramonicas|
// | 4412 us | 1u ramonicas|
// | 4411 us | 0u ramonicas|
// | 4414 us | 1u ramonicas|
// | 4414 us | 2u ramonicas|
// | 4416 us | 2u ramonicas|
// | 4415 us | 3u ramonicas|
// | 4412 us | 4u ramonicas|
// | 4414 us | 5u ramonicas|
// | 4413 us | 7u ramonicas|
// | 4414 us | 8u ramonicas|
// | 4414 us | 10u ramonicas|
// | 4413 us | 11u ramonicas|
// | 4411 us | 12u ramonicas|
// | 4409 us | 14u ramonicas|
// | 4411 us | 15u ramonicas|
// | 4415 us | 16u ramonicas|
// | 4411 us | 17u ramonicas|
// | 4411 us | 18u ramonicas|
// | 4412 us | 19u ramonicas|
// | 4411 us | 20u ramonicas|
// | 4410 us | 20u ramonicas|
// | 4414 us | 21u ramonicas|
// | 4414 us | 21u ramonicas|
// | 4411 us | 21u ramonicas|
// | 4413 us | 22u ramonicas|
// | 4411 us | 23u ramonicas|
// | 4415 us | 23u ramonicas|
// | 4411 us | 24u ramonicas|
// | 4413 us | 24u ramonicas|
// | 0u ramonicas|
// | 4412 us | 25u ramonicas|
// | 0u ramonicas|
// | 0u ramonicas|
// | 0u ramonicas|
// | 0u ramonicas|
// | 4411 us | 25u ramonicas|
// | 0u ramonicas|
// | 4416 us | 24u ramonicas|
// | 4407 us | 24u ramonicas|
// | 0u ramonicas|
// | 4408 us | 24u ramonicas|
// | 0u ramonicas|
// | 4413 us | 23u ramonicas|
// | 0u ramonicas|
// | 4410 us | 21u ramonicas|
// | 4414 us | 18u ramonicas|
// | 4411 us | 12u ramonicas|
// | 4415 us | 8u ramonicas|
// | 4414 us | 4u ramonicas|
// | 4465 us | 1u ramonicas|
// | 4412 us | 1u ramonicas|
// | 0u ramonicas|
// | 0u ramonicas|
// | 0u ramonicas|

