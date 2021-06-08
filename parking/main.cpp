#include "mbed.h"
//#include "bbcar_rpc.h"
#include "mbed_rpc.h"
#include "bbcar.h"

BufferedSerial pc(USBTX, USBRX);
BufferedSerial xbee(D10, D9);

Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);
volatile int steps;
volatile int last;
double x, y, z;

void park(Arguments *in, Reply *out);
RPCFunction rpcpark(&park, "park");

BBCar car(pin5, pin6, servo_ticker);
void encoder_control() {
    int value = encoder;
    if (!last && value) steps++;
    last = value;
}

int main(void) {
    pc.set_baud(9600);
    encoder_ticker.attach(&encoder_control, 10ms);

   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
      RPC::call(buf, outbuf);
      printf("%s\r\n", outbuf);
   }


 
}


void park(Arguments *in, Reply *out)   {

    // In this scenario, when using RPC delimit the two arguments with a space.
    x = in->getArg<double>();
    y = in->getArg<double>();
    z = in->getArg<double>();

    int x1 = x, y1 = y, z1 = z, sign = -1;
    if (z1 == 1) sign = 1;
    else
        sign = -1;
    printf("%d %d\n", x1, y1);
    // Have code here to call another RPC function to wake up specific led or close it.
    steps = 0;
    last = 0;
    car.turn_new(-30, -30, 1, 0.84);
    while(steps*6.5*3.14/32 < x1 - 0.5) {
        // printf("encoder = %d\r\n", steps);
        ThisThread::sleep_for(100ms);
    }
    car.stop();
    if (z1 == 1) {
        car.turn(-100,0.1);
        ThisThread::sleep_for(970ms);
    }
    else {
        car.turn(-100,-0.1);
        ThisThread::sleep_for(1300ms);
    }

    car.stop();

    steps = 0;
    last = 0;
    car.turn_new(-30, -30, 1, 0.84);
    if (z1 == 2) sign = 3;
    while(steps*6.5*3.14/32 < y1 + 8) {
        // printf("encoder = %d\r\n", steps);
        ThisThread::sleep_for(100ms);
    }
    car.stop();




}