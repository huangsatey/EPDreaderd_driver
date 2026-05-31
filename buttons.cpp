// buttons.cpp
#include "init.h"
#include "buttons.h"
#include "menu.h" 


void onLeftClick() {
    Serial.println("[LEFT] Short press - Menu Up");
    menuUp();  // 调用菜单向上
}


void onLeftDoublePress() {
    Serial.println("[LEFT] db press - Back to previous menu");
    menuBack();  // 调用菜单返回上一级
}


void onRightClick() {
    Serial.println("[RIGHT] Click - Menu Down");
    menuDown();  // 调用菜单向下
}


void onRightDoubleClick() {
    Serial.println("[RIGHT] Double click");
}


void onMidClick() {
    Serial.println("[MID] Short press - Menu Enter");
    menuEnter();  // 调用菜单进入
}


void onMidLongPress() {
    Serial.println("[MID] Long press");
}