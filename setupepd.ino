#include "init.h"
#include "buttons.h"
#include "menu.h"
#include "sdcard.h"

void setup() {
    // 1. 初始化按钮
    initButtons();
    
    // 2. 初始化SD卡（先于屏幕，因为SD卡初始化会重置SPI）
    bool sdReady = initSDCard();
    
    // 3. 初始化屏幕（这会重新配置SPI给EPD使用）
    initEPD();
    
    // 4. 恢复EPD的SPI配置（如果需要）
    // reinitEPDSPI();  // 注意：initEPD已经配置了SPI，这行可能不需要
    
    // 5. 如果SD卡就绪，浏览根目录获取文件列表
    if (sdReady) {
        // 重要：扫描根目录，填充 currentFiles 数组
        browseDirectory("/");
        Serial.print("Found ");
        Serial.print(currentFileCount);
        Serial.println(" files/directories in root");
    }
    
    // 6. 清屏并显示状态栏
    fillBlack();
    
    // 7. 显示状态栏（根据SD卡状态）
    if (sdReady) {
        drawBar(10, 80, false, false, true, false, true);
        Serial.println("SD Card Ready");
    } else {
        drawBar(10, 80, false, false, false, false, true);
        Serial.println("SD Card Not Found");
    }
    
    // 8. 初始化菜单
    initMenu();
}

void loop() {
    updateButtons();
    delay(10);
}