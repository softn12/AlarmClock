// clock.cpp
#include <cstdint> // uint8_t 등이 정의되어 있는 라이브러리
#include "mbed.h" // FRDM-K64F 보드 구동에 필요한 라이브러리
#include "C12832.h" // lcd 제어를 위한 라이브러리
#include "pitches.h" // sound frequency가 설정된 라이브러리
#include "clock.h"

uint8_t screenMode = 0; // lcd에 출력될 화면을 선택
uint8_t snoozeState = 0;

// bool 타입 변수는 모두 초기값으로 false 설정
bool setClockMode = false;
bool lcdCleared = false; 
bool setAlarm1 = false;
bool setAlarm2 = false;
bool setAlarm3 = false;
bool ringAlarm = false;

InterruptIn up(A2); 
InterruptIn down(A3); 
InterruptIn left(A4); 
InterruptIn rite(A5);
InterruptIn center(D4); 
InterruptIn sw1(D3); 
InterruptIn sw2(D8);
InterruptIn sw3(D9);
DigitalOut led_r(D5);
C12832 lcd(D11, D13, D12, D7, D10);
PwmOut buzzer(D6);

CurrentTime c1;
Alarm1 a1;
Alarm2 a2;
Alarm3 a3;

float frequency[] = {
    NOTE_E6, NOTE_E6, 0, NOTE_E6, 0, NOTE_C6, NOTE_E6, 0, NOTE_G6, 0, 0,
    0, NOTE_G5, 0, 0, 0, NOTE_C6, 0, 0, NOTE_G5, 0, 0, NOTE_E5, 0, 0,
    NOTE_A5, 0, NOTE_B5, 0, NOTE_AS5, NOTE_A5, 0, NOTE_G5, NOTE_E6, NOTE_G6,
    NOTE_A6, 0, NOTE_F6, NOTE_G6, 0, NOTE_E6, 0, NOTE_C6, NOTE_D6, NOTE_B5,
    0, 0, NOTE_C6, 0, 0, NOTE_G5, 0, 0, NOTE_E5, 0, 0, NOTE_A5, 0, NOTE_B5,
    0, NOTE_AS5, NOTE_A5, 0, NOTE_G5, NOTE_E6, NOTE_G6, NOTE_A6, 0, NOTE_F6,
    NOTE_G6, 0, NOTE_E6, 0, NOTE_C6, NOTE_D6, NOTE_B5, 0, 0
};

float beat[] = {
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 9, 9, 9, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 9, 9, 9, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12,
};

int length = 75; // frequency[]와 beat[]의 길이

void up_HH() {
    if(setClockMode) {
        switch(screenMode) {
            case 0:
            c1.HH++;
            if(c1.HH >= 24)
                c1.HH = 0;
            break;
        
            case 1:
            a1.HH++;
            if(a1.HH >= 24)
                a1.HH = 0;
            break;

            case 2:
            a2.HH++;
            if(a2.HH >= 24)
                a2.HH = 0;
            break;

            case 3:
            a3.HH++;
            if(a3.HH >= 24)
                a3.HH = 0;
            break;
        }
    }
}

// HH의 type이 uint_8이므로 0에서 -1하면 overflow되어 255가 된다. 그래서 0에서 -1하면 23이 되도록 설정
void down_HH() {
    if(setClockMode) {
        switch(screenMode) {
            case 0:
            c1.HH--;
            if(c1.HH >= 61)
                c1.HH = 23;
            break;
        
            case 1:
            a1.HH--;
            if(a1.HH >= 61)
                a1.HH = 23;
            break;

            case 2:
            a2.HH--;
            if(a2.HH >= 61)
                a2.HH = 23;
            break;

            case 3:
            a3.HH--;
            if(a3.HH >= 61)
                a3.HH = 23;
            break;
        }
    }
}

void up_MM() {
    if(setClockMode) {
        switch(screenMode) {
            case 0:
            c1.MM++;
            if(c1.MM >= 60)
                c1.MM = 0;
            break;
        
            case 1:
            a1.MM++;
            if(a1.MM >= 60)
                a1.MM = 0;
            break;

            case 2:
            a2.MM++;
            if(a2.MM >= 60)
                a2.MM = 0;
            break;

            case 3:
            a3.MM++;
            if(a3.MM >= 60)
                a3.MM = 0;
            break;
        }
    }
}

// MM의 type이 uint_8이므로 0에서 -1하면 overflow되어 255가 된다. 그래서 0에서 -1하면 59가 되도록 설정
void down_MM() {
    if(setClockMode) {
        switch(screenMode) {
            case 0:
            c1.MM--;
            if(c1.MM >= 61)
                c1.MM = 59;
            break;
        
            case 1:
            a1.MM--;
            if(a1.MM >= 61)
                a1.MM = 59;
            break;

            case 2:
            a2.MM--;
            if(a2.MM >= 61)
                a2.MM = 59;
            break;

            case 3:
            a3.MM--;
            if(a3.MM >= 61)
                a3.MM = 59;
            break;
        }
    }
}

void reset_SS() {
    if(setClockMode) {
        switch(screenMode) {
            case 0:
            c1.SS = 0;
            break;

            case 1:
            a1.SS = 0;
            break;

            case 2:
            a2.SS = 0;
            break;

            case 3:
            a3.SS = 0;
            break;
        }
    }
}

// buzzer를 울림, Music = SuperMario BGM
void buzzer_trigger() {
    for(int i = 0; i <= 4; i++) { // 5번 반복
        for(int j = 0; j <= length; j++) {
            if(!ringAlarm) {
                buzzer = 0.0;
                return;
            }
            if(frequency[j] == 0) {
                buzzer = 0.0;
            } else {
                buzzer.period(1.0 / frequency[j]);  // period = (1.0 / frequency)
                buzzer = 0.5;                       // duty cycle = 100%
            }
            thread_sleep_for(2500.0 / beat[j]);     // duration = (C / beat) ms
        }
    }
    
    ringAlarm = false;
} 


// LED를 1분간 점멸함
void led_trigger() {
    for(int i = 0; i < 60; i++) {
        if(!ringAlarm) {
            led_r = 1;
            return;
        }
        led_r = 1; // led = ON
        thread_sleep_for(500);
        led_r = 0; // led = OFF
        thread_sleep_for(500);
    }
}

// Main_screen을 lcd에 출력
void print_main_screen() {
    if(!lcdCleared) {
        lcd.cls();
        lcdCleared = true;
    }
    lcd.locate(0,6);
    lcd.printf("Current time");
    lcd.locate(0, 16);
    lcd.printf("%02d : %02d : %02d", c1.HH, c1.MM, c1.SS);

    if(setClockMode) {
        lcd.locate(100, 6);
        lcd.printf("SET");
    }
    else if(!setClockMode) {
        lcd.locate(100, 6);
        lcd.printf("   ");
    }
}

// 첫 번째 alarm을 lcd에 출력
void print_set_alarm1() {
    if(!lcdCleared) {
        lcd.cls();
        lcdCleared = true;
    }
    lcd.locate(0,6);
    lcd.printf("Alarm1");
    lcd.locate(0, 16);
    lcd.printf("%02d : %02d", a1.HH, a1.MM);

    if(setClockMode) {
        lcd.locate(100, 6);
        lcd.printf("SET");
    }

    if(!setClockMode && setAlarm1) {
        lcd.locate(100, 6);
        lcd.printf("ON ");
    }
    else if (!setClockMode && !setAlarm1) {
        lcd.locate(100, 6);
        lcd.printf("OFF");
    }
} 

// 두 번째 alarm을 lcd에 출력
void print_set_alarm2() {
    if(!lcdCleared) {
        lcd.cls();
        lcdCleared = true;
    }
    lcd.locate(0,6);
    lcd.printf("Alarm2");
    lcd.locate(0, 16);
    lcd.printf("%02d : %02d", a2.HH, a2.MM);

    if(setClockMode) {
        lcd.locate(100, 6);
        lcd.printf("SET");
    }

    if(!setClockMode && setAlarm2) {
        lcd.locate(100, 6);
        lcd.printf("ON ");
    }
    else if(!setClockMode && !setAlarm2) {
        lcd.locate(100, 6);
        lcd.printf("OFF");
    }
}

// 세 번째 alarm을 lcd에 출력
void print_set_alarm3() {
    if(!lcdCleared) {
        lcd.cls();
        lcdCleared = true;
    }
    lcd.locate(0,6);
    lcd.printf("Alarm3");
    lcd.locate(0, 16);
    lcd.printf("%02d : %02d", a3.HH, a3.MM);

    if(setClockMode) {
        lcd.locate(100, 6);
        lcd.printf("SET");
    }
    
    if(!setClockMode && setAlarm3) {
        lcd.locate(100, 6);
        lcd.printf("ON ");
    }
    else if(!setClockMode && !setAlarm3) {
        lcd.locate(100, 6);
        lcd.printf("OFF");
    }
}

// 알람이 울릴 때 sw2를 누르면 해당 알람이 5분 뒤에 다시 울리도록 설정
void snooze() {
    ringAlarm = false;
    switch(snoozeState) {
       case 1:
       a1.MM += 5;
       break;

       case 2:
       a2.MM += 5;
       break;

       case 3:
       a3.MM += 5; 
       break;
    }

    snoozeState = 0;
}
