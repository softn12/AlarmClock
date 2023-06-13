// main.cpp
#include "mbed.h"
#include "ESP-call.h"
#include "clock.h"
#include "thread.h"
#include "ISR.h"

Serial pc(USBTX, USBRX, 115200); // baud rate 115200
Thread upTimeThread, makeSoundThread, makeLightThread, printScreenThread;


void wifiConnect(void)
{
    NodeMCU_PGM http_client[] = {
        "", 1,
        "wifi.setmode(wifi.STATION)", 1,
        "", 1,
        "station_cfg={}", 1,
        "station_cfg.ssid=\"SSID\"", 1, // enter WIFI Network ID
        "station_cfg.pwd=\"PASSWORD\"", 1, // enter WiFi Network password
        "station_cfg.save=false", 1,
        "wifi.sta.config(station_cfg)", 1,
        "", 1,
        "wifi.sta.connect()", 80, // long delay here!
        "", 1,
        "print(wifi.sta.status())", 1,
        "print(wifi.sta.getip())", 1,
        NULL, 0,
    };
    ESP_call_multi(http_client);
}

NodeMCU_PGM http_server[] = {
    "", 1,
    "socket:on(\"receive\", function(sck, c) sck:close() end)", 1,
    "http_resp=\"HTTP/1.0 200 OK\\r\\nContent-Type: text/html\\r\\n\\r\\n\"", 1,
    // HTML 코드
    "html_main1 = \"<h1>ALARM CLOCK</h1>\"", 1,
    // ALARM_off
    "html_main2 = \"<form id=\\\"alarm_off\\\" method=\\\"POST\\\">\"", 1,
    "html_main3 = \"<h2>ALARM</h2>\"", 1,
    "html_main4 = \"<button type=\\\"submit\\\" name=\\\"alarm_off\\\" value=\\\"off\\\"> ALARM_OFF </button> </form> \"", 1,
    // ALARM1_on_off
    "html_main5 = \"<form id=\\\"alarm1\\\" method=\\\"POST\\\">\"", 1,
    "html_main6 = \"<h2>ALARM1</h2>\"", 1,
    "html_main7 = \"<button type=\\\"submit\\\" name=\\\"alarm1_set\\\" value=\\\"off\\\"> ALARM1_SET </button>\"", 1,
    "html_main8 = \"<button type=\\\"submit\\\" name=\\\"alarm1_unset\\\" value=\\\"off\\\"> ALARM1_UNSET </button> </form> \"", 1,
     // ALARM2_on_off
    "html_main9 = \"<form id=\\\"alarm2\\\" method=\\\"POST\\\">\"", 1,
    "html_main10 = \"<h2>ALARM2</h2>\"", 1,
    "html_main11 = \"<button type=\\\"submit\\\" name=\\\"alarm2_set\\\" value=\\\"off\\\"> ALARM2_SET </button>\"", 1,
    "html_main12 = \"<button type=\\\"submit\\\" name=\\\"alarm2_unset\\\" value=\\\"off\\\"> ALARM2_UNSET </button> </form> \"", 1,
     // ALARM3_on_off
    "html_main13 = \"<form id=\\\"alarm3\\\" method=\\\"POST\\\">\"", 1,
    "html_main14= \"<h2>ALARM3</h2>\"", 1,
    "html_main15 = \"<button type=\\\"submit\\\" name=\\\"alarm3_set\\\" value=\\\"off\\\"> ALARM3_SET </button>\"", 1,
    "html_main16= \"<button type=\\\"submit\\\" name=\\\"alarm3_unset\\\" value=\\\"off\\\"> ALARM3_UNSET </button> </form> \"", 1,


    "srv = net.createServer(net.TCP)", 1,
    "srv:listen(80, function(conn)", 1,
    "conn:on(\"receive\", function(sck, payload)", 1,

    // 알람 끄기
    "if string.match(payload, \"alarm_off=off\") then ", 1, 
        "print(\"0\")",1, 
    "end", 1,
     // 알람1 set
    "if string.match(payload, \"alarm1_set=off\") then ", 1, 
        "print(\"1\")",1, 
    "end", 1,
     // 알람1 unset
    "if string.match(payload, \"alarm1_unset=off\") then ", 1, 
        "print(\"2\")",1, 
    "end", 1,
     // 알람2 set
    "if string.match(payload, \"alarm2_set=off\") then ", 1, 
        "print(\"3\")",1, 
    "end", 1,
     // 알람2 unset
    "if string.match(payload, \"alarm2_unset=off\") then ", 1, 
        "print(\"4\")",1, 
    "end", 1,
     // 알람3 set
    "if string.match(payload, \"alarm3_set=off\") then ", 1, 
        "print(\"5\")",1, 
    "end", 1,
     // 알람3 unset
    "if string.match(payload, \"alarm3_unset=off\") then ", 1, 
        "print(\"6\")",1, 
    "end", 1,

    "sck:send(http_resp)", 1,
    "sck:send(html_main1)", 1,
    // ALARM
    "sck:send(html_main2)", 1,
    "sck:send(html_main3)", 1,
    "sck:send(html_main4)", 1,
    // ALARM1
    "sck:send(html_main5)", 1,
    "sck:send(html_main6)", 1,
    "sck:send(html_main7)", 1,
    "sck:send(html_main8)", 1,
    // ALARM2
    "sck:send(html_main9)", 1,
    "sck:send(html_main10)", 1,
    "sck:send(html_main11)", 1,
    "sck:send(html_main12)", 1,
    // ALARM3
    "sck:send(html_main13)", 1,
    "sck:send(html_main14)", 1,
    "sck:send(html_main15)", 1,
    "sck:send(html_main16)", 1,
    

    "end)", 1,
    "conn:on(\"sent\", function(sck) sck:close() end)", 1,
    "end)", 1,
    NULL, 0,
};



// Setup ESP echo mode
int main()
{
    // ISR 버튼 동작 기능 정의
    sw1.rise(&ISR_sw1);
    sw2.rise(&ISR_sw2);
    sw3.rise(&ISR_sw3);
    up.rise(&ISR_up);
    down.rise(&ISR_down);
    left.rise(&ISR_left);
    rite.rise(&ISR_rite); 
    center.rise(&ISR_center);

    upTimeThread.start(up_time);
    makeSoundThread.start(make_sound);
    makeLightThread.start(make_light);
    printScreenThread.start(print_screen);

    // Config baudrate of PC and ESP
    PC.baud(115200);
    ESP.baud(115200);
    // Reset ESP
    PC.printf("\r\nReset ESP…\r\n");
    ESP_reset();
    // Setup ESP noecho mode
    PC.printf("Setup ESP noecho…\r\n");
    ESP_noecho();
    // Execute a NodeMCU program
    PC.printf("Execute a NodeMCU program…\r\n");
    wifiConnect();

    ESP_call_multi(http_server); 

    // Config ESP passthrough mode
    PC.printf("ESP recive mode…\r\n");
    ESP.attach(&ISR_ESP_to_PC, Serial::RxIrq);
}