#include "mbed.h"

DigitalOut orange(p5);
DigitalOut blue(p6);
InterruptIn but(p7);
PwmOut led(p9);
Timeout time_pressed;

enum states{DOWN, UP, FALL, MIN, RISE, MAX};
int state = UP;
bool pressed;

void but_rise();
void but_fall();
void intensity();

int main() {
    blue = 0;
    orange = 0;
    led = 1;
    but.rise(&but_rise);
    but.fall(&but_fall);
    while(1){
        printf("Estado: %d\n", state);
        wait_ms(50);
    }
}

void but_rise(){
    pressed = true;
    time_pressed.attach(&intensity, 1.0);
}

void but_fall(){
    pressed = false;
    if ((state==DOWN) || (state == MIN) || ((state == FALL) && (led.read() > 0.0))){
        blue = 1;
        orange = 0;
        state = UP;
    }
    else if((state==UP) || (state == MAX) || ((state == RISE) && (led.read() < 1.0))){
        orange = 1;
        blue = 0;
        state = DOWN;
    }
}

void intensity(){
    if (pressed){
        if((state == RISE) && (led.read() == 1.0)) state = MAX;
        else if((state == FALL) && (led.read() == 0.0)) state = MIN;
        else{
        
            if((state == UP) || (state == RISE)){
                state = RISE;
                if(led.read() <= 0.95) led = led + 0.05;
            }
            if((state == DOWN) || (state == FALL)){
                state = FALL;
                if(led.read() >= 0.05) led = led - 0.05;
            }
        }
    }
}
