#ifndef INIT_H
#define INIT_H
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <OneButton.h>
#include <FS.h>
#include <SD.h>  // 添加SD卡库
#include <SPI.h>

struct MenuItem;

#define EPD_CS    5
#define EPD_DC    4
#define EPD_RST   3
#define EPD_BUSY  1
#define EPD_SCK   6
#define EPD_MOSI  7
#define EPD_PWR   21 //EPD的LDO，没有可以注释掉

#define SD_CS     10    // SD卡片选
#define SD_SCK    EPD_SCK     // 与EPD共用SCK
#define SD_MOSI   EPD_MOSI     // 与EPD共用MOSI
#define SD_DAT0   2    // 数据线0
#define SD_MISO   SD_DAT0

#define SPI_FREQ 4000000 //SPI频率，不行的可以降一点

#define IF_BUSY true//是否启用BUSY

#define ROLL 3// 设置旋转方向 (0=竖屏, 1=横屏, 2=竖屏翻转, 3=横屏翻转)

#define WAIT_SERIAL 100//初始化等待串口的延时ms
#define FILL_DELAY  700//屏幕填充时的延时ms

#define BAT_X 230//默认电池横坐标

#define SW_HIGH_LEVEL LOW//按钮高电平有效
#define SW_LOW_LEVEL HIGH

#define BT_LEFT  8//按钮定义
#define BT_RIGHT 20
#define BT_MID   9
// 按钮时间定义（单位：毫秒）
#define IF_UPPILL true //是否上拉
#define BT_DEBOUNCE_TIME   50   // 去抖时间
#define BT_SHORT_PRESS_TIME 400  // 短按最小时间
#define BT_LONG_PRESS_TIME  700 // 长按阈值时间

#define EPD_WIDTH   250
#define EPD_HEIGHT  122
#define SAFE_MENU_H 18
#define NO_MENU_TEXT_H 30 //提示加载菜单的文字Y坐标


#define PARA1 1
#define MENU_START_Y (SAFE_MENU_H - PARA1)
#define MENU_HEIGHT (EPD_HEIGHT - SAFE_MENU_H - 10)
#define MENU_WIDTH  EPD_WIDTH
#define MENU_ITEM_HEIGHT 16
#define MENU_VISIBLE_ITEMS (MENU_HEIGHT / MENU_ITEM_HEIGHT)

#ifndef FILE_ITEM_DEFINED
#define FILE_ITEM_DEFINED
struct FileItem {
    String name;
    bool isDirectory;
    String path;
};

// 文件浏览相关常量
#define MAX_FILE_ITEMS 50
#define MENU_LEVEL_FILE_BROWSER 5
#endif

// 全局变量声明
extern String currentPath;
extern FileItem currentFiles[MAX_FILE_ITEMS];
extern int currentFileCount;
extern bool inFileBrowser;
extern MenuItem fileBrowserMenu[MAX_FILE_ITEMS + 1];

#endif
extern GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display;//选择屏幕样式 看selection.h

extern OneButton btnLeft;
extern OneButton btnRight;
extern OneButton btnMid;

void initButtons(); 
void updateButtons();

void initEPD();

void wakeUp();
void justSleep();
void powerOff();

void fillWhite();
void fillBlack();

void drawBar(int y,int bat,bool is_WIFI,bool is_SLEEP,bool is_CARD,bool is_MENU,bool is_FULLSCREEN);
void drawBattery(int x, int y, int level);
void drawWifiIcon(int x, int y);
void drawSleepIcon(int x, int y);

// SD卡函数声明
bool initSDCard();
void reinitEPDSPI();
void listDirectory(String path, FileItem* items, int* count);
void browseDirectory(String path);
void updateFileBrowserMenu();
void enterFileBrowser();
void handleFileBrowserSelect();
