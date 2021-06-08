#include"mbed.h"
#include "bbcar.h"


BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BufferedSerial xbee(D10, D9);

Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalInOut ping(D12);
DigitalIn encoder(D11);
volatile int steps;
volatile int last;
BBCar car(pin5, pin6, servo_ticker);

Thread t1, t2;
Timer t;

int cnt = 0;
int ind = 0;
int done = 0;
float send_angle = 0.0;
char str1[45];

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}

void PPPping(void)
{
   	float val;
//   	pc.set_baud(9600);
	xbee.set_baud(9600);
	char buff[25] = {0};
   	
	while(1) {

    	ping.output();
      	ping = 0; wait_us(200);
      	ping = 1; wait_us(5);
      	ping = 0; wait_us(5);

      	ping.input();
      	while(ping.read() == 0);
      	t.start();
      	while(ping.read() == 1);
      	val = t.read();
//     	printf("Ping = %lf\r\n", val*17700.4f);
		sprintf(buff, "dist %.2lf angle %.2f\r\n", val*17700.4f, send_angle);
//		printf(buff);
		xbee.write(buff, sizeof(buff));
      	t.stop();
      	t.reset();

      	ThisThread::sleep_for(1s);
   	}

}

void controller()
{
	float dist, angle, Tx, Tz, tmp_angle, tTx, tTz;
	int delay = 0, go = 0, d = 0, st = 0, last = 0, modify = 0, der = 1, done = 0;
	car.goStraight(0);	
	ThisThread::sleep_for(500ms);	

		Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
		Tz = -1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
//		angle = 100 * (int(str1[24]) - 48) + 10 * (int(str1[25]) - 48) + (int(str1[26]) - 48) + 0.1 * (int(str1[28]) - 48) + 0.01 * (int(str1[29]) - 48);
//		dist = 100 * (int(str1[36]) - 48) + 10 * (int(str1[37]) - 48) + (int(str1[38]) - 48) + 0.1 * (int(str1[40]) - 48) + 0.01 * (int(str1[41]) - 48);
		tTx = Tx;
		tTz = Tz;
		car.goStraight(50);
		ThisThread::sleep_for(1000ms);
		Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
		Tz = -1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
		if (((Tx - tTx) <= 1 || (Tx - tTx) >= -1) && ((Tz - tTz) <= 0.5 || (Tz - tTz) >= -0.5)) st = 1;
		else 
			go = 1;
//			st = 0;
		

		car.goStraight(-50);
		ThisThread::sleep_for(1000ms);
		car.stop();
		ThisThread::sleep_for(1000ms);

	while (1) {
		der = 1;
		Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
		Tz = -1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
		angle = 100 * (int(str1[24]) - 48) + 10 * (int(str1[25]) - 48) + (int(str1[26]) - 48) + 0.1 * (int(str1[28]) - 48) + 0.01 * (int(str1[29]) - 48);
		dist = 100 * (int(str1[36]) - 48) + 10 * (int(str1[37]) - 48) + (int(str1[38]) - 48) + 0.1 * (int(str1[40]) - 48) + 0.01 * (int(str1[41]) - 48);
//		printf("%.2f %.2f %.2f %.2f\n", Tx, Tz, dist, angle);
//		printf("%c %d\n", str1[0], go);

	if (st && str1[0] == 'A') {
		if ((Tx <= 1 || Tx >= -1) && (angle <= 5 || angle >= 355)) {
			car.goStraight(100);
	    	while(steps*6.5*3.14/32 < (-1*Tz*6.2-15)) {
        	// printf("encoder = %d\r\n", steps);
        	ThisThread::sleep_for(100ms);
    		}	
			car.stop();
			ThisThread::sleep_for(100ms);
			st = 0;	
		}
		else
			go = 1;
	}

	if (go && str1[0] == 'A' && dist >= 20) {
		d = 0;
		
		if (angle > 300) {
			last = 1;
			delay = int((90 - (360 - angle))/90*1.0*1000);
			car.turn_new(100, 100, 1, 0.1);

			while (d < delay - 100) {
				d += 100;
				ThisThread::sleep_for(100ms);
			}
			if (delay <= 600) der = 2;

			car.stop();
			ThisThread::sleep_for(1000ms);
			car.turn_new(100, 100, 0.1, 1);

			ThisThread::sleep_for(1000ms / der + 50ms);

			car.stop();
			ThisThread::sleep_for(100ms);
		}
		else {
			last = 2;
			delay = int((90 - angle)/90*1.0*1000);
			car.turn_new(100, 100, 0.1, 1);
			
			while (d < delay ) {
				d += 100;
				ThisThread::sleep_for(100ms);
			}
			if (delay <= 550) der = 2;
			car.stop();
			ThisThread::sleep_for(1000ms);
			car.turn_new(100, 100, 1, 0.1);

			ThisThread::sleep_for(900ms / der - 50ms);

			car.stop();
			ThisThread::sleep_for(100ms);		
		}
		
	}
		ThisThread::sleep_for(1000ms);
		Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
		Tz = -1 * (10 * (int(str1[15]) - 48) + (int(str1[16]) - 48) + 0.1 * (int(str1[18]) - 48) + 0.01 * (int(str1[19]) - 48));
		angle = 100 * (int(str1[24]) - 48) + 10 * (int(str1[25]) - 48) + (int(str1[26]) - 48) + 0.1 * (int(str1[28]) - 48) + 0.01 * (int(str1[29]) - 48);
		if ((Tx <= 0.5 || Tx >= -0.5) && (angle <= 5 || angle >= 355) && !done) {
			go = 0;
			modify = 1;
			done = 1;
			send_angle = angle;
			printf("done\n");
			if (dist >= 30) {
				car.goStraight(50);
			    while(steps*6.5*3.14/32 < 10) {
        		// printf("encoder = %d\r\n", steps);
        		ThisThread::sleep_for(100ms);
    			}			
			}
//			printf("%.2f %.2f %.2f %.2f\n", Tx, Tz, dist, angle);
			car.stop();
			ThisThread::sleep_for(1000ms);

		}

		if (modify && str1[0] == 'A' && !done) {
			printf("modify\n");
			Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
//			printf("%.2f\n", Tx);
			if ((last == 1 && Tx > 0) || (last == 2 && Tx > 0)) {
//				Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));		
				while (!((Tx <= 0.6 || Tx >= -0.6) /*&& str1[0] == 'A'*/)) {
					Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
					car.turn_new(20, 20, 0.1, 1);
					ThisThread::sleep_for(100ms);
				}
				car.stop();
				ThisThread::sleep_for(200ms);
				modify = 0;
				go = 0;
			}
			if ((last == 1 && Tx < 0) || (last == 2 && Tx < 0)) {
//				Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));		
				while (!((Tx <= 0.6 || Tx >= -0.6) /*&& str1[0] == 'A'*/)) {
					Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));
					car.turn_new(20, 20, 1, 0.1);
					ThisThread::sleep_for(100ms);
				}
				car.stop();
				ThisThread::sleep_for(200ms);
				modify = 0;
				go = 0;
			}
		}
		ThisThread::sleep_for(1000ms);
		if ((!done && go && str1[0] == 'B' && dist >= 20) || (!modify  && str1[0] == 'B')){
			printf("lost\n");
			Tx = -1 * (10 * (int(str1[5]) - 48) + (int(str1[6]) - 48) + 0.1 * (int(str1[8]) - 48) + 0.01 * (int(str1[9]) - 48));	
			printf("%.2f\n",Tx);	
			if ((last == 1 && Tx >= 0) ||(last == 2 && Tx >= 0)) {
				car.goStraight(-100);
				ThisThread::sleep_for(500ms);
				while (!(str1[0] == 'A')) {
					car.turn_new(30, 30, 0.1, 1);
					ThisThread::sleep_for(200ms);
				}
				car.stop();
				ThisThread::sleep_for(1000ms);
			}
			if ((last == 1 && Tx < 0) || (last == 2 && Tx < 0)) {
				car.goStraight(-100);
				ThisThread::sleep_for(700ms);
				while (!(str1[0] == 'A')) {
					car.turn_new(30, 30, 1, 0.1);
					ThisThread::sleep_for(200ms);
				}
				car.stop();
				ThisThread::sleep_for(1000ms);
			}
		}

		

		ThisThread::sleep_for(300ms);
	}

}





int main(void) {
	uart.set_baud(9600);
    encoder_ticker.attach(&encoder_control, 10ms);

	t1.start(controller);
	t2.start(PPPping);

	while(1){
		if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            str1[cnt] = recv[0];
            cnt++;
            if (recv[0] == 'E') {
            	cnt = 0;

//				printf("%s\n", str1);

            }
//            pc.write(recv, sizeof(recv));
      }
   }



}




