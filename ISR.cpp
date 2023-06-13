// ISR.cpp
#include "mbed.h"
#include "clock.h"

void ISR_sw1() {
    lcdCleared = false;
    screenMode++;
    if(screenMode >= 4) 
        screenMode = 0; // mainScreen == 0, alarm1 == 1, alaram2 == 2, alarm3 == 3
}

void ISR_sw2() {

    if(ringAlarm) { // 알람이 울릴 때는 sw2가 snooze button으로 동작
        snooze();
    }

    else { // 알람이 울리지 않을 때는 시간 설정 모드
        if(!setClockMode) { // 시간 설정이 false일 때 sw2를 누르면 시간 설정 모드로 진입
            setClockMode = true;
        }
        else // 시간 설정 모드 on일 때 sw2를 누르면 시간 설정 모드 빠져 나옴
            setClockMode = false;
        
        if(setClockMode) {
            switch(screenMode) {
                case 1:
                setAlarm1 = true;
                break;

                case 2:
                setAlarm2 = true;
                break;

                case 3:
                setAlarm3 = true;
                break;
            }    
        }
    }
}

void ISR_sw3() {
    ringAlarm = false;

    switch(screenMode) {
        case 1:
        setAlarm1 = false;
        break;

        case 2:
        setAlarm2 = false;
        break;

        case 3:
        setAlarm3 = false;
        break;
    }
}

 void ISR_up() {
    up_HH();
 }
    
void ISR_down() {
    down_HH();
}

void ISR_left() {
    up_MM();
}

void ISR_rite() {
    down_MM();
} 

void ISR_center() {
    reset_SS();
}