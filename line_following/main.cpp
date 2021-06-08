#include"mbed.h"
#include "bbcar.h"


BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx


Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);
volatile int steps;
volatile int last;
BBCar car(pin5, pin6, servo_ticker);

Thread t1;

int cnt = 0;
int ind = 0;
int done = 0;
char str1[20];

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}

void controller()
{
	int x1, y1 ,x2, y2, delta = 0, sign = 1, modify = 0;
	char temp[3];
	encoder_ticker.attach(&encoder_control, 10ms);
	steps = 0;
	last = 0;
	car.stop();

while (1) {

	if (str1[0] == 'A') {
		x1 = 100 * (int(str1[1]) - 48) + 10 * (int(str1[2]) - 48) + (int(str1[3]) - 48);
		y1 = 100 * (int(str1[5]) - 48) + 10 * (int(str1[6]) - 48) + (int(str1[7]) - 48);
		x2 = 100 * (int(str1[9]) - 48) + 10 * (int(str1[10]) - 48) + (int(str1[11]) - 48);
		y2 = 100 * (int(str1[13]) - 48) + 10 * (int(str1[14]) - 48) + (int(str1[15]) - 48);		
		printf("%d %d %d %d\n", x1, y1, x2, y2);
//		ThisThread::sleep_for(50ms);
//		car.goStraight(40);
//   		while(steps*6.5*3.14/32 < 8) {
//			printf("going\n");
//      		ThisThread::sleep_for(100ms);
//   		}

		delta = x2 - x1;
		sign = 1;
		if (delta < 0) { 
			delta = -delta;
			sign = -1;
		}
		if (sign == -1) {
			if (delta <= 10) delta *= 2;			
			car.turn_new(30,35+delta,1, 1);
			ThisThread::sleep_for(100ms);
		}	
		else {	
//			if (delta <= 10) delta *= 3;		
			car.turn_new(30+delta/4,30, 1 ,1);
			ThisThread::sleep_for(100ms);
		}


//		ThisThread::sleep_for(50ms);
/*
		if (delta > 0 && delta <= 5) {
			if (sign == 1) {
				car.turn_new(25,30);
				printf("left\n");
			}
			if (sign == -1) {
				car.turn_new(30,25);
				printf("right");
			}
//			ThisThread::sleep_for(10ms);
		}
		if (delta > 5  && delta <= 10) {
			if (sign == 1) {
				car.turn_new(22,30);
				printf("left\n");
			}
			if (sign == -1) {
				car.turn_new(30,22);
				printf("right");
			}
//			ThisThread::sleep_for(10ms);
		}
		if (delta > 10) {
			if (sign == 1) {
				car.turn_new(20,30);
				printf("left\n");
			}
			if (sign == -1) {
				car.turn_new(30,20);
				printf("right");
			}
//			ThisThread::sleep_for(10ms);
		}
*/
//		done = 0;
//		steps = 0;
//		last = 0;
	
	}
	if (str1[0] == 'B') car.goStraight(0);
	ThisThread::sleep_for(20ms);
}

}





int main(void) {
	uart.set_baud(9600);

	t1.start(controller);
	while(1){
		if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            str1[cnt] = recv[0];
            cnt++;
            if (recv[0] == 'E') {
            	cnt = 0;

//				printf("QQQQ%s\n", str1);

            }
//            pc.write(recv, sizeof(recv));
      }
   }
}




