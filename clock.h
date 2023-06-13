// clock.h

// ====================================================================
#include <cstdint>
#include "mbed.h"
#include "pitches.h"
#include "C12832.h"

// 현재 시각에 대한 구조체
struct CurrentTime {
    uint8_t HH = 0;
    uint8_t MM = 0;
    uint8_t SS = 0;
};

// 알람 1에 대한 구조체
struct Alarm1 {
    uint8_t HH = 0;
    uint8_t MM = 0;
    uint8_t SS = 0;
    bool alarmSet = false;
};

// 알람 2에 대한 구조체
struct Alarm2 {
    uint8_t HH = 0;
    uint8_t MM = 0;
    uint8_t SS = 0;
    bool alarmSet = false;
};

// 알람 3에 대한 구조체
struct Alarm3 {
    uint8_t HH = 0;
    uint8_t MM = 0;
    uint8_t SS = 0;
    bool alarmSet = false;
};

extern uint8_t screenMode; // lcd에 출력될 화면을 선택
extern bool setClockMode; // 시간 설정 모드 on(=true), off()
extern bool lcdCleared; // 페이지를 넘길 때 lcd가 1회 clear되도록 설정하는 변수, 초기값 = false

extern CurrentTime c1;
extern Alarm1 a1;
extern Alarm2 a2;
extern Alarm3 a3;

extern float frequency[];
extern float beat[];

extern InterruptIn up; // up = A2
extern InterruptIn down; // down = A3
extern InterruptIn left; // left = A4
extern InterruptIn rite; // rite = A5
extern InterruptIn center; // center = D4
extern InterruptIn sw1; // sw1 = D3
extern InterruptIn sw2; // sw2 = D8
extern InterruptIn sw3; // sw3 = D9
extern DigitalOut led_r; // led_r = D5
extern C12832 lcd; // lcd = D11, D13, D12, D7, D10
extern PwmOut buzzer; // buzzer = PTA1

extern bool setAlarm1; // 알람1 : true == set, false == unset
extern bool setAlarm2; // 알람2 : true == set, false == unset
extern bool setAlarm3; // 알람3 : ture == set, false == unset
extern bool ringAlarm; // 스피커에서 알람이 울리고 있으면 true, 울리지 않고 있으면 false 
extern uint8_t snoozeState; // 어떤 알람이 울렸었는지에 대한 번호 저장(초기값 0)

void up_HH(); // ScreenMode에 따라 HH 1 증가
void down_HH(); // ScreenMode에 따라 HH 1 감소
void up_MM(); // ScreenMode에 따라 MM 1 증가
void down_MM(); // ScreenMode에 따라 MM 1 감소
void reset_SS(); // MainScreen에서만 사용, SS 0으로 초기화
void buzzer_trigger(); // buzzer를 울림
void led_trigger(); // LED를 점멸함
void print_main_screen(); // main화면 lcd에 출력
void print_set_alarm1(); // alarm설정1 lcd에 출력
void print_set_alarm2(); // alarm설정2 lcd에 출력
void print_set_alarm3(); // alarm설정3 lcd에 출력
void snooze(); // 알람이 울리고 있을 때 누르면 해당 알람이 5분 뒤에 다시 울림