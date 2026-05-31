#include "init.h"
#include "buttons.h"
#include <U8g2_for_Adafruit_GFX.h>


U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

OneButton btnLeft(BT_LEFT, SW_LOW_LEVEL, IF_UPPILL);
OneButton btnRight(BT_RIGHT, SW_LOW_LEVEL, IF_UPPILL);
OneButton btnMid(BT_MID, SW_LOW_LEVEL, IF_UPPILL);

void initButtons() {
    // 设置时间参数
    btnLeft.setPressMs(BT_SHORT_PRESS_TIME);      // 长按判定时间 500ms
    btnRight.setPressMs(BT_SHORT_PRESS_TIME);
    btnMid.setPressMs(BT_SHORT_PRESS_TIME);
    
    btnLeft.setDebounceMs(BT_DEBOUNCE_TIME);    // 消抖时间
    btnRight.setDebounceMs(BT_DEBOUNCE_TIME);
    btnMid.setDebounceMs(BT_DEBOUNCE_TIME);
    

    btnLeft.attachClick(onLeftClick);
    btnLeft.attachDoubleClick(onLeftDoublePress);
    

    btnRight.attachClick(onRightClick);
    btnRight.attachDoubleClick(onRightDoubleClick);
    // btnRight.attachLongPressStart(onRightLongPress);  // 不需要，注释掉
    

    btnMid.attachClick(onMidClick);
    btnMid.attachLongPressStart(onMidLongPress);
    
    Serial.println("INIT: Buttons initialized");

}
void updateButtons() {
    btnLeft.tick();
    btnRight.tick();
    btnMid.tick();
}

void initEPD(){
    Serial.begin(115200);

    delay(WAIT_SERIAL);

    #ifdef EPD_PWR
    pinMode(EPD_PWR, OUTPUT);
    digitalWrite(EPD_PWR, HIGH);
    delay(500);
    Serial.println("INIT:LDO ON");
    #endif

    SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);
    display.epd2.selectSPI(SPI, SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    display.init(115200, true, 2, IF_BUSY);

    display.setRotation(ROLL);
    //display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_BLACK);
    u8g2Fonts.begin(display);
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    Serial.println("INIT: USE fillWhite");
    fillWhite();
    Serial.println("INIT: Setup Finish");
}

void wakeUp(){
    display.init(115200, false, 2, IF_BUSY);
    Serial.println("SLEEP:Screen Power ON");
}
void justSleep(){
    display.hibernate();
    Serial.println("SLEEP:Screen Power Off,But LDO On");

}
void powerOff(){//再次开启时需要init
    display.powerOff();
    Serial.println("POWER:Screen Power Off,LDO Off");
    delay(100);
    #ifdef EPD_PWR
    digitalWrite(EPD_PWR, HIGH);
    #endif
    
}

void fillWhite(){
    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);
    } while (display.nextPage());
    delay(FILL_DELAY);

    Serial.println("FILL:White The Screen");
}

void fillBlack(){
    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_BLACK);
    } while (display.nextPage());
    delay(FILL_DELAY);

    Serial.println("FILL:Black The Screen");
}

void drawBar(int y,int bat,bool is_WIFI,bool is_SLEEP,bool is_CARD,bool is_MENU,bool is_FULLSCREEN){
    //display.setFullWindow();
    
    if(is_FULLSCREEN){
        display.setFullWindow();
    }else{
        if(is_MENU){
            display.setPartialWindow(0, 0, 250, y + 4);
        }else{
            display.setPartialWindow(0, 0, 250, y + NO_MENU_TEXT_H);
        }
    }
    
    display.firstPage();
    do {
        display.drawLine(0, y, 250, y, GxEPD_BLACK);
        drawBattery(BAT_X,y-9,bat);
        if(is_WIFI){
          drawWifiIcon(BAT_X - 14 - 5, y - 9);
          }
         if (is_SLEEP) {
            drawSleepIcon(BAT_X - 20 -10- 6,y-10);
        }
        if (!is_CARD){
            display.setFont(NULL);
            display.setCursor(BAT_X - (36 + 50), y-9);
            display.print("NO CARD");
        }
        if(!is_MENU){
            u8g2Fonts.begin(display);
            u8g2Fonts.setForegroundColor(GxEPD_BLACK);
            u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
            u8g2Fonts.setFont(u8g2_font_wqy14_t_chinese3);
            u8g2Fonts.setCursor((EPD_WIDTH/2)-27, y+20);
            u8g2Fonts.print("加载中..");
            u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
            }
        
    } while (display.nextPage());
    delay(FILL_DELAY);
}

void drawBattery(int x, int y, int level) {
    if (level < 0) level = 0;
    if (level > 100) level = 100;
         display.fillRect(x + 1, y + 1, 10, 6, GxEPD_WHITE);
        display.drawRect(x, y, 12, 8, GxEPD_BLACK);
        display.fillRect(x + 12, y + 2, 2, 4, GxEPD_BLACK);
        int fillWidth = map(level, 0, 100, 0, 8);
        if (fillWidth > 0) {
            display.fillRect(x + 1, y + 1, fillWidth, 6, GxEPD_BLACK);
}
}

void drawWifiIcon(int x, int y) {
    display.fillRect(x, y + 5, 2, 3, GxEPD_BLACK);
    display.fillRect(x + 3, y + 3, 2, 5, GxEPD_BLACK);
    display.fillRect(x + 6, y + 1, 2, 7, GxEPD_BLACK);
    display.fillRect(x + 9, y, 2, 8, GxEPD_BLACK);
}

void drawSleepIcon(int x, int y) {
    display.drawLine(x, y + 1, x + 6, y + 1, GxEPD_BLACK);
    display.drawLine(x + 6, y + 1, x, y + 4, GxEPD_BLACK);
    display.drawLine(x, y + 4, x + 6, y + 4, GxEPD_BLACK);
    
    // 第二个Z (稍小，偏右)
    display.drawLine(x + 4, y + 5, x + 10, y + 5, GxEPD_BLACK);
    display.drawLine(x + 10, y + 5, x + 4, y + 8, GxEPD_BLACK);
    display.drawLine(x + 4, y + 8, x + 10, y + 8, GxEPD_BLACK);
}