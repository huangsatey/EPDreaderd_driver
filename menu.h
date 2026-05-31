// menu.h
#ifndef MENU_H
#define MENU_H

#include "init.h"
#include <U8g2_for_Adafruit_GFX.h>

// 菜单项结构体
struct MenuItem {
    const char* name;      // 菜单项名称（支持中文）
    int subMenuIndex;      // 子菜单索引，-1表示无子菜单
    void (*action)();      // 回调函数（用于叶子节点）
};

// 菜单层级结构
struct MenuLevel {
    MenuItem* items;
    int itemCount;
};

// 菜单状态结构体
struct MenuState {
    int currentMenuLevel;  // 当前显示的菜单层级
    int selectedIndex;     // 当前选中的项
    int scrollOffset;      // 滚动偏移量
};

// 菜单矩形区域
struct MenuRect {
    int x, y;              // 起始坐标
    int width, height;     // 宽度和高度
};

// 外部声明U8g2对象
extern U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

extern MenuState currentState;
extern int menuHistory[10];
extern int historyDepth;
extern MenuLevel menuLevels[];
extern const int MENU_LEVEL_COUNT;
extern MenuRect menuArea;

// 函数声明
void initMenu();
void drawMenu();
void menuUp();
void menuDown();
void menuEnter();
void menuBack();
void updateMenuDisplay();
void refreshStatusBar();
void renderFullMenu();
void onFileSelected();

// 菜单数据（在menu.cpp中定义）
extern MenuItem mainMenu[];
extern MenuItem deviceMenu[];
extern MenuItem wifiMenu[];
extern MenuItem displayMenu[];
extern MenuItem systemMenu[];
extern MenuItem fileBrowserMenu[];

#endif