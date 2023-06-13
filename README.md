# AlarmClock
Mbed embedded programming

알람 시계 기능 

[SW1]
1) 누를 때마다 MainScreen, Alarm1SetScreen, Alarm2SetScreen, Alarm3SetScreen으로 순서대로 이동(화면 순환)

[SW2]
1) SW2 누르면 ClockSet Mode로 진입
2) Mbed Application Shield(이하 Shield) joystick으로 시간 설정
3) MainScreen ClockSet Mode : 
> joystick up : 시(HH) 1씩 증가
> 
> joystick down : 시(HH) 1씩 감소
> 
> joystick left : 분(MM) 1씩 증가
> 
> joystick right : 분(MM) 1씩 감소
> 
> joystic center : 초(SS) 리셋\n
4) AlarmSetScreen ClockSet Mode : 
> joystick up : 시(HH) 1씩 증가\n
> 
> joystick down : 시(HH) 1씩 감소\n
> 
> joystick left : 분(MM) 1씩 증가\n
> 
> joystick right : 분(MM) 1씩 감소\n

[SW3] 
1) 설정한 알람이 울리고 있으면 알람 중지
2) AlarmSetScreen에서 SW3 누르면 해당 알람 해제
> ex) Alarm1SetScreen에서 SW3 누르면 설정된 알람이 해제됨

[LCD]
1) MainScreen : 현재 시각 Display
2) Alarm1SetScreen : 첫 번째 알람 설정 화면
3) Alarm2SetScreen : 두 번째 알람 설정 화면
4) Alarm3SetScreen : 세 번째 알람 설정 화면

[Buzzer]
1) 알람 설정한 시각이 되면 소리가 출력
