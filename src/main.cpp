#include "mbed.h"

DigitalOut orange(p5);
DigitalOut blue(p6);
InterruptIn but(p7);
PwmOut led(p9);
Timeout time_pressed;
Timeout time_blink;

enum states{DOWN, UP, FALL, MIN, RISE, MAX};
int state = UP;
bool pressed;
float intensity;
float period;

void but_rise();
void but_fall();
void transition();
void blink();

int main() {
    blue = 1;
    orange = 0;
    intensity = 0.5;
    but.rise(&but_rise);
    but.fall(&but_fall);
    while(1){ 
        led = intensity;
        printf("Estado: %d\n", state);
        printf("Intensidade: %f\n", intensity);
        wait_ms(50);
    }
}

void but_rise(){
    pressed = true;
    time_pressed.attach(&transition, 1.0);
}

void but_fall(){
    pressed = false;
    if ((state==DOWN) || (state == MIN) || ((state == RISE) && (intensity < 1.0))){
        blue = 1;
        orange = 0;
        state = UP;
    }
    else if((state==UP) || (state == MAX) || ((state == FALL) && (intensity > 0.0))){
        orange = 1;
        blue = 0;
        state = DOWN;
    }
}

void transition(){
    time_pressed.attach(&transition, 1.0);
    time_blink.attach(&blink, 0.2);
    if (pressed){
        if((state == RISE) && (intensity == 1.0)) state = MAX;
        else if((state == FALL) && (intensity == 0.0)) state = MIN;
        else{
        
            if((state == UP) || (state == RISE)){
                state = RISE;
                intensity += 0.05;
            }
            if((state == DOWN) || (state == FALL)){
                state = FALL;
                intensity -= 0.05;
            }
        }
        if(intensity >= 1.0){
            intensity = 1.0;
            state = MAX;
        }
        else if(intensity <= 0.0){
            intensity = 0.0;
            state = MIN;
        }
    }
}

void blink(){
    if((state == MIN) || (state == MAX)) period = 0.1;
    else if (state == FALL) period = 0.2*(intensity/0.05 - 1);
    else if (state == RISE) period = 0.2*(1 + intensity/0.05);
    
    if((state == MIN) || (state == FALL)) orange = !orange;
    else if((state == MAX) || (state == RISE)) blue = !blue;

    time_blink.attach(&blink, period);
}
