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
