// thread.cpp
#include "mbed.h"
#include "clock.h"

void up_time() {
    while (true) {
        c1.SS++;
        c1.MM += (c1.SS / 60);
        c1.HH += (c1.MM / 60);
        c1.HH = c1.HH % 24;
        c1.MM = c1.MM % 60;
        c1.SS = c1.SS % 60;
        ThisThread::sleep_for(1000); // 일정한 시간 간격으로 루프를 반복하기 위해 sleep 추가
    }
}


void print_screen() {
    while(true) {
        switch(screenMode) {
            case 0:
            print_main_screen();
            break;

            case 1:
            print_set_alarm1();
            break;

            case 2:
            print_set_alarm2();
            break;

            case 3:
            print_set_alarm3();
            break;
        }
    }
}

void make_sound() {
    while(true) {
        if(setAlarm1) {
            if(c1.HH == a1.HH && c1.MM == a1.MM && c1.SS == a1.SS) {
                ringAlarm = true;
                snoozeState = 1;
                buzzer_trigger();
            }
        }
        
        if(setAlarm2) {
            if(c1.HH == a2.HH && c1.MM == a2.MM && c1.SS == a2.SS) {
                ringAlarm = true;
                snoozeState = 2;
                buzzer_trigger();
            }
        }
            
        if(setAlarm3) {   
            if(c1.HH == a3.HH && c1.MM == a3.MM && c1.SS == a3.SS) {
                ringAlarm = true;
                snoozeState = 3;
                buzzer_trigger();
            }
        }
    }
}

void make_light() {
    while(true) {
        if(setAlarm1) {
            if(c1.HH == a1.HH && c1.MM == a1.MM && c1.SS == a1.SS) {
                ringAlarm = true;
                led_trigger();
            }
        }
        
        if(setAlarm2) {
            if(c1.HH == a2.HH && c1.MM == a2.MM && c1.SS == a2.SS) {
                ringAlarm = true;
                led_trigger();
            }
        }
            
        if(setAlarm3) {   
            if(c1.HH == a3.HH && c1.MM == a3.MM && c1.SS == a3.SS) {
                ringAlarm = true;
                led_trigger();
            }
        }
    }
}
