#include <stdio.h>
	#include <stdlib.h>
	#include <wiringPi.h>
	
	#define TRUE 1
	
	#define TRIG 0
	#define ECHO 3
	
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

	        while(digitalRead(ECHO) == LOW);
	

	        long startTime = micros();
	        while(digitalRead(ECHO) == HIGH);
	        long travelTime = micros() - startTime;
	

	        float distance = (float) travelTime / 58.0f;
			long endTimegetCM = micros();
			printf("| %ld us ", endTimegetCM - startTimegetCM);
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

// | tiempo     |  distancia |
// |------------|------------|
// 	| 761 us | 2.913793cm|
// | 650 us | 3.000000cm|
// | 620 us | 2.586207cm|
// | 625 us | 2.620690cm|
// | 628 us | 2.689655cm|
// | 650 us | 3.034483cm|
// | 654 us | 3.086207cm|
// | 619 us | 1.948276cm|
// | 657 us | 3.137931cm|
// | 668 us | 3.362069cm|
// | 689 us | 3.706897cm|
// | 715 us | 4.172414cm|
// | 771 us | 5.172414cm|
// | 816 us | 5.931035cm|
// | 867 us | 3.810345cm|
// | 915 us | 7.620690cm|
// | 976 us | 8.706897cm|
// | 1053 us | 9.965517cm|
// | 1109 us | 11.000000cm|
// | 1158 us | 11.827586cm|
// | 1210 us | 12.689655cm|
// | 1295 us | 14.172414cm|
// | 1319 us | 14.603448cm|
// | 1346 us | 15.051724cm|
// | 1375 us | 15.482759cm|
// | 1409 us | 16.137932cm|
// | 1431 us | 16.517241cm|
// | 1482 us | 17.396551cm|
// | 1559 us | 18.655172cm|
// | 1609 us | 19.534483cm|
// | 1705 us | 21.241379cm|
// | 1774 us | 22.413794cm|
// | 1829 us | 23.431034cm|
// | 1897 us | 24.551723cm|
// | 1945 us | 25.362068cm|
// | 2024 us | 26.741379cm|
// | 2076 us | 27.655172cm|
// | 2145 us | 28.810345cm|
// | 2204 us | 29.879311cm|
// | 2271 us | 31.017241cm|
// | 2401 us | 33.206898cm|
// | 2504 us | 35.034481cm|
// | 2590 us | 36.482758cm|
// | 2771 us | 39.620689cm|
// | 2868 us | 41.327587cm|
// | 2953 us | 42.689655cm|
// | 3052 us | 44.431034cm|
// | 3148 us | 46.155174cm|
// | 3243 us | 47.758621cm|
// | 3361 us | 49.793102cm|
// | 3516 us | 52.482758cm|
// | 3590 us | 53.724136cm|
// | 3708 us | 55.775864cm|
// | 3809 us | 57.517242cm|
// | 3923 us | 59.448277cm|
// | 4005 us | 60.844826cm|
// | 4118 us | 62.844826cm|
// | 4283 us | 65.689651cm|
// | 4373 us | 67.224136cm|
// | 4540 us | 70.086205cm|
// | 4587 us | 70.948273cm|
// | 4697 us | 72.896553cm|
// | 4714 us | 73.224136cm|
// | 4637 us | 71.844826cm|
// | 4457 us | 68.655174cm|
// | 4354 us | 66.913795cm|
// | 4101 us | 62.500000cm|
// | 3956 us | 60.034481cm|
// | 3756 us | 56.586208cm|
// | 3620 us | 54.224136cm|
// | 3444 us | 51.224136cm|
// | 3267 us | 48.206898cm|
// | 3043 us | 44.275864cm|
// | 2811 us | 40.275864cm|
// | 2669 us | 37.793102cm|
// | 2576 us | 36.275864cm|
// | 2426 us | 33.637932cm|
// | 2214 us | 30.000000cm|
// | 2002 us | 26.310345cm|
// | 1780 us | 22.500000cm|
// | 1574 us | 18.948277cm|
// | 1397 us | 15.879311cm|
// | 1225 us | 12.948276cm|
// | 1086 us | 10.568966cm|
// | 942 us | 8.051724cm|
// | 831 us | 6.086207cm|
// | 714 us | 4.172414cm|
// | 674 us | 3.465517cm|
// | 633 us | 2.775862cm|
// | 657 us | 3.137931cm|
// | 641 us | 2.931035cm|
// | 634 us | 2.775862cm|
