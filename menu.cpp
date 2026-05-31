// menu.cpp
#include "menu.h"
#include "buttons.h"
#include "sdcard.h"
// menu.cpp - 在文件开头添加

// 动态生成的文件浏览器菜单
MenuItem fileBrowserMenu[MAX_FILE_ITEMS + 1];  // +1 用于"返回上一级"

// 更新文件浏览器菜单
void updateFileBrowserMenu() {
    Serial.print("updateFileBrowserMenu: fileCount=");
    Serial.println(currentFileCount);
    
    // 添加"返回上一级"
    fileBrowserMenu[0] = {"返回", -1, menuBack};
    
    // 添加文件和目录 - 为每个项设置回调
    for (int i = 0; i < currentFileCount && i < MAX_FILE_ITEMS; i++) {
        static char itemNames[MAX_FILE_ITEMS][64];
        String name = currentFiles[i].name;
        if (currentFiles[i].isDirectory) {
            name += "/";
        }
        strncpy(itemNames[i + 1], name.c_str(), 63);
        itemNames[i + 1][63] = '\0';
        fileBrowserMenu[i + 1].name = itemNames[i + 1];
        fileBrowserMenu[i + 1].subMenuIndex = -1;
        fileBrowserMenu[i + 1].action = onFileSelected;  // 设置回调函数
        Serial.print("  Added: ");
        Serial.println(itemNames[i + 1]);
    }
    
    // 如果文件数为0，显示提示
    if (currentFileCount == 0) {
        fileBrowserMenu[1].name = "   (空目录)";
        fileBrowserMenu[1].subMenuIndex = -1;
        fileBrowserMenu[1].action = nullptr;  // 空目录项不可选择
        Serial.println("  Empty directory, showing hint");
    }
}

// 进入文件浏览器
void enterFileBrowser() {
    Serial.println("enterFileBrowser called");
    currentState.currentMenuLevel = MENU_LEVEL_FILE_BROWSER;
    currentState.selectedIndex = 0;
    currentState.scrollOffset = 0;
    
    // 更新菜单项
    updateFileBrowserMenu();
    
    // 重新指向菜单层级
    menuLevels[MENU_LEVEL_FILE_BROWSER].items = fileBrowserMenu;
    int itemCount = currentFileCount + 1;
    if (currentFileCount == 0) {
        itemCount = 2;  // 返回 + 空目录提示
    }
    menuLevels[MENU_LEVEL_FILE_BROWSER].itemCount = itemCount;
    
    Serial.print("Menu item count: ");
    Serial.println(itemCount);
    
    renderFullMenu();
}

// 处理文件浏览器选择
void handleFileBrowserSelect() {
       int selectedIndex = currentState.selectedIndex;
    Serial.print("handleFileBrowserSelect: index=");
    Serial.println(selectedIndex);
    
    if (selectedIndex == 0) {
        // 返回上一级
        Serial.print("Return from: ");
        Serial.println(currentPath);
        
        if (currentPath != "/") {
            // 获取父目录
            int lastSlash = currentPath.lastIndexOf('/', currentPath.length() - 2);
            String parentPath = currentPath.substring(0, lastSlash + 1);
            if (parentPath == "") parentPath = "/";
            Serial.print("Going to parent: ");
            Serial.println(parentPath);
            
            // 直接浏览父目录
            browseDirectory(parentPath);
        } else {
            // 在根目录，退出文件浏览器回到主菜单
            Serial.println("Exiting file browser to main menu");
            inFileBrowser = false;
            
            // 清理历史栈，返回主菜单
            historyDepth = 0;
            currentState.currentMenuLevel = 0;
            currentState.selectedIndex = 0;
            currentState.scrollOffset = 0;
            renderFullMenu();
        }
        return;
    }
    
    // 如果是空目录，忽略选择
    if (currentFileCount == 0) {
        Serial.println("Empty directory, ignoring selection");
        return;
    }
    
    // 获取选中的文件/目录
    int fileIndex = selectedIndex - 1;
    if (fileIndex < currentFileCount) {
        Serial.print("Selected item: ");
        Serial.print(currentFiles[fileIndex].name);
        Serial.print(" isDirectory: ");
        Serial.println(currentFiles[fileIndex].isDirectory);
        
        if (currentFiles[fileIndex].isDirectory) {
            // 进入子目录
            String newPath = currentPath;
            if (!newPath.endsWith("/")) newPath += "/";
            newPath += currentFiles[fileIndex].name;
            if (!newPath.endsWith("/")) newPath += "/";
            Serial.print("Entering directory: ");
            Serial.println(newPath);
            
            // 直接进入子目录
            browseDirectory(newPath);
        } else {
            // 是文件，显示文件内容或执行操作
            Serial.print("Selected file: ");
            Serial.println(currentFiles[fileIndex].name);
            // TODO: 在这里添加打开文件的代码
        }
    } else {
        Serial.print("Invalid file index: ");
        Serial.print(fileIndex);
        Serial.print(" >= ");
        Serial.println(currentFileCount);
    }
}


// 定义回调函数（示例）
void enterSubMenu1() { 
    Serial.println("Entered Sub Menu 1");
    // 在这里实现具体功能
}

void enterSubMenu2() { 
    Serial.println("Entered Sub Menu 2");
}

void openSetting1() { 
    Serial.println("Setting 1");
}

void openSetting2() { 
    Serial.println("Setting 2");
}

void openSetting3() { 
    Serial.println("Setting 3");
}

void showAbout() {
    Serial.println("About - Version 1.0");
}

// 定义菜单树结构
MenuItem mainMenu[] = {
    {"设备信息", 1, nullptr},
    {"WiFi设置", 2, nullptr},
    {"显示设置", 3, nullptr},
    {"系统设置", 4, nullptr},
    {"文件浏览", MENU_LEVEL_FILE_BROWSER, nullptr},
    {"关于设备", -1, showAbout}
};

MenuItem deviceMenu[] = {
    {"电池状态", -1, openSetting1},
    {"存储信息", -1, openSetting2},
    {"固件版本", -1, enterSubMenu1},
    {"返回", -1, menuBack}
};

MenuItem wifiMenu[] = {
    {"扫描网络", -1, openSetting1},
    {"连接WiFi", -1, openSetting2},
    {"IP地址", -1, openSetting3},
    {"返回", -1, menuBack}
};

MenuItem displayMenu[] = {
    {"亮度调节", -1, openSetting1},
    {"休眠时间", -1, openSetting2},
    {"刷新模式", -1, openSetting3},
    {"返回", -1, menuBack}
};

MenuItem systemMenu[] = {
    {"恢复出厂", -1, openSetting1},
    {"重启设备", -1, openSetting2},
    {"语言设置", -1, openSetting3},
    {"返回", -1, menuBack}
};

// 菜单层级结构
MenuLevel menuLevels[] = {
    {mainMenu, sizeof(mainMenu) / sizeof(MenuItem)},      // 层级0：主菜单
    {deviceMenu, sizeof(deviceMenu) / sizeof(MenuItem)},  // 层级1：设备信息
    {wifiMenu, sizeof(wifiMenu) / sizeof(MenuItem)},      // 层级2：WiFi设置
    {displayMenu, sizeof(displayMenu) / sizeof(MenuItem)},// 层级3：显示设置
    {systemMenu, sizeof(systemMenu) / sizeof(MenuItem)},   // 层级4：系统设置
    {fileBrowserMenu, 1}                                   // 层级5：文件浏览器（初始只有返回）
};

const int MENU_LEVEL_COUNT = sizeof(menuLevels) / sizeof(MenuLevel);

// 菜单状态
MenuState currentState = {0, 0, 0};
int menuHistory[10] = {0};  // 菜单历史栈
int historyDepth = 0;

// 菜单显示区域（自动计算）
MenuRect menuArea = {0, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT};

// ========== 辅助函数 ==========

// 获取当前菜单的项数
int getCurrentItemCount() {
    return menuLevels[currentState.currentMenuLevel].itemCount;
}

// 获取当前菜单的项名称
const char* getCurrentItemName(int index) {
    return menuLevels[currentState.currentMenuLevel].items[index].name;
}

// 获取当前菜单项
MenuItem* getCurrentItem(int index) {
    return &menuLevels[currentState.currentMenuLevel].items[index];
}

// 滚动到选中项可见
void ensureVisible() {
    if (currentState.selectedIndex < currentState.scrollOffset) {
        // 向上滚动
        currentState.scrollOffset = currentState.selectedIndex;
    } else if (currentState.selectedIndex >= currentState.scrollOffset + MENU_VISIBLE_ITEMS) {
        // 向下滚动
        currentState.scrollOffset = currentState.selectedIndex - MENU_VISIBLE_ITEMS + 1;
    }
}

// ========== 菜单渲染函数 ==========

// 清除菜单区域
void clearMenuArea() {
    display.fillRect(menuArea.x, menuArea.y, menuArea.width, menuArea.height, GxEPD_WHITE);
}

// 清除单个菜单项行
void clearMenuItemLine(int yOffset) {
    int y = menuArea.y + yOffset;
    display.fillRect(menuArea.x, y, menuArea.width, MENU_ITEM_HEIGHT, GxEPD_WHITE);
}

// 绘制单个菜单项
void drawMenuItem(int index, bool isSelected, int visiblePos) {
    int y = menuArea.y + visiblePos * MENU_ITEM_HEIGHT;
    const char* itemName = getCurrentItemName(index);
    MenuItem* item = getCurrentItem(index);
    
    // 清除这一行
    clearMenuItemLine(visiblePos);
    
    if (isSelected) {
        // 选中项：黑底白字
        display.fillRect(menuArea.x, y, menuArea.width, MENU_ITEM_HEIGHT, GxEPD_BLACK);
        u8g2Fonts.setForegroundColor(GxEPD_WHITE);
        u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
    } else {
        // 未选中：白底黑字
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    }
    
    // 绘制菜单项名称（使用U8g2支持中文）
    u8g2Fonts.setCursor(menuArea.x + 8, y + 15);
    u8g2Fonts.print(itemName);
    
    // 如果有子菜单或回调，显示箭头
    if (item->subMenuIndex != -1 || item->action != nullptr) {
        u8g2Fonts.setCursor(menuArea.x + menuArea.width - 20, y + 12);
        if(item->action == menuBack){
        u8g2Fonts.print("<");
        }else{
        u8g2Fonts.print(">");
        }

    }
}

// 渲染整个菜单
void renderFullMenu() {
    // 只刷新菜单区域
    display.setPartialWindow(menuArea.x, menuArea.y, menuArea.width, menuArea.height);
    display.firstPage();
    do {
        // 清除菜单区域
        display.fillRect(menuArea.x, menuArea.y, menuArea.width, menuArea.height, GxEPD_WHITE);
        
        int itemCount = getCurrentItemCount();
        int startIndex = currentState.scrollOffset;
        int endIndex = startIndex + MENU_VISIBLE_ITEMS;
        if (endIndex > itemCount) endIndex = itemCount;
        
        for (int i = startIndex; i < endIndex; i++) {
            int visiblePos = i - startIndex;
            bool isSelected = (i == currentState.selectedIndex);
            
            int y = menuArea.y + visiblePos * MENU_ITEM_HEIGHT;
            const char* itemName = getCurrentItemName(i);
            MenuItem* item = getCurrentItem(i);
            
            // 清除这一行
            display.fillRect(menuArea.x, y, menuArea.width, MENU_ITEM_HEIGHT, GxEPD_WHITE);
            
            if (isSelected) {
                display.fillRect(menuArea.x, y, menuArea.width, MENU_ITEM_HEIGHT, GxEPD_BLACK);
                u8g2Fonts.setForegroundColor(GxEPD_WHITE);
                u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
            } else {
                u8g2Fonts.setForegroundColor(GxEPD_BLACK);
                u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
            }
            
            // 文字位置
            u8g2Fonts.setCursor(menuArea.x + 8, y + 12);
            u8g2Fonts.print(itemName);
            
            if (item->subMenuIndex != -1 || item->action != nullptr) {
                u8g2Fonts.setCursor(menuArea.x + menuArea.width - 20, y + 12);
                if(item->action == menuBack){
                    u8g2Fonts.print("<");
                }else{
                    u8g2Fonts.print(">");
                    }
            }
        }
        
    } while (display.nextPage());
}
// 只刷新选中的行（效率最高）
void refreshSelectedOnly() {
    int startIndex = currentState.scrollOffset;
    int oldVisiblePos = -1;
    int newVisiblePos = -1;
    
    // 找到需要刷新的行位置
    if (currentState.selectedIndex >= startIndex && 
        currentState.selectedIndex < startIndex + MENU_VISIBLE_ITEMS) {
        newVisiblePos = currentState.selectedIndex - startIndex;
    }
    
    // 注意：需要知道旧选中的位置，这需要额外保存
    // 简化处理：刷新整个可见区域（对于墨水屏来说更稳定）
    display.setPartialWindow(menuArea.x, menuArea.y, menuArea.width, menuArea.height);
    display.firstPage();
    do {
        int itemCount = getCurrentItemCount();
        int startIdx = currentState.scrollOffset;
        int endIdx = startIdx + MENU_VISIBLE_ITEMS;
        if (endIdx > itemCount) endIdx = itemCount;
        
        for (int i = startIdx; i < endIdx; i++) {
            int visiblePos = i - startIdx;
            bool isSelected = (i == currentState.selectedIndex);
            drawMenuItem(i, isSelected, visiblePos);
        }
    } while (display.nextPage());
}

// 更新菜单显示（根据情况选择刷新方式）
void updateMenuDisplay() {
    // 检查是否需要滚动
    int oldScrollOffset = currentState.scrollOffset;
    ensureVisible();
    renderFullMenu();
    /*
    if (oldScrollOffset != currentState.scrollOffset) {
        // 滚动时刷新整个菜单
        renderFullMenu();
    } else {
        // 只刷新选中项
        refreshSelectedOnly();
    }
    */
}

// ========== 菜单操作函数 ==========

// 向上移动
void menuUp() {
    if (currentState.selectedIndex > 0) {
        currentState.selectedIndex--;
        if (inFileBrowser && currentState.currentMenuLevel == MENU_LEVEL_FILE_BROWSER) {
            // 文件浏览器模式，重新渲染
            renderFullMenu();
        } else {
            updateMenuDisplay();
        }
        Serial.print("Menu Up, selected: ");
        Serial.println(currentState.selectedIndex);
    }
}

// 向下移动
void menuDown() {
    int itemCount = getCurrentItemCount();
    if (currentState.selectedIndex < itemCount - 1) {
        currentState.selectedIndex++;
        if (inFileBrowser && currentState.currentMenuLevel == MENU_LEVEL_FILE_BROWSER) {
            renderFullMenu();
        } else {
            updateMenuDisplay();
        }
        Serial.print("Menu Down, selected: ");
        Serial.println(currentState.selectedIndex);
    }
}

// 进入菜单/执行操作
void menuEnter() {
      MenuItem* item = getCurrentItem(currentState.selectedIndex);
    
    // 特殊处理：如果要进入文件浏览器层级（层级5）
    if (item->subMenuIndex == MENU_LEVEL_FILE_BROWSER) {
        // 保存当前菜单到历史栈，以便返回时能回到主菜单
        menuHistory[historyDepth++] = currentState.currentMenuLevel;
        
        // 如果还没有扫描过根目录，先扫描
        if (currentFileCount == 0) {
            Serial.println("No files found, browsing root directory");
            browseDirectory("/");
        } else {
            enterFileBrowser();
        }
        return;
    }
    
    // 处理普通子菜单
    if (item->subMenuIndex != -1 && item->subMenuIndex < MENU_LEVEL_COUNT) {
        // 进入子菜单
        menuHistory[historyDepth++] = currentState.currentMenuLevel;
        currentState.currentMenuLevel = item->subMenuIndex;
        currentState.selectedIndex = 0;
        currentState.scrollOffset = 0;
        renderFullMenu();
        Serial.print("Enter submenu: ");
        Serial.println(currentState.currentMenuLevel);
    } 
    // 处理有回调函数的菜单项
    else if (item->action != nullptr) {
        Serial.print("Executing action for: ");
        Serial.println(item->name);
        item->action();
    } 
    else {
        Serial.print("Selected (no action): ");
        Serial.println(item->name);
    }
}

// 返回上一级菜单
void menuBack() {
     Serial.print("menuBack called, historyDepth=");
    Serial.println(historyDepth);
    
    // 如果当前在文件浏览器中
    if (currentState.currentMenuLevel == MENU_LEVEL_FILE_BROWSER) {
        // 文件浏览器的返回逻辑由 handleFileBrowserSelect 处理
        // 但这里也要处理历史栈
        if (historyDepth > 0) {
            currentState.currentMenuLevel = menuHistory[--historyDepth];
            currentState.selectedIndex = 0;
            currentState.scrollOffset = 0;
            renderFullMenu();
            Serial.println("Back to previous menu");
        } else {
            // 没有历史记录，返回主菜单
            currentState.currentMenuLevel = 0;
            currentState.selectedIndex = 0;
            currentState.scrollOffset = 0;
            renderFullMenu();
            Serial.println("Back to main menu");
        }
    } else if (historyDepth > 0) {
        currentState.currentMenuLevel = menuHistory[--historyDepth];
        currentState.selectedIndex = 0;
        currentState.scrollOffset = 0;
        renderFullMenu();
        Serial.println("Back to previous menu");
    } else {
        Serial.println("Already at top level menu");
    }
}

// 刷新状态栏（与菜单分离）
/*
void refreshStatusBar() {
    // 只刷新状态栏区域
    display.setPartialWindow(0, 0, EPD_WIDTH, SAFE_MENU_H);
    display.firstPage();
    do {
        // 这里可以重新绘制状态栏内容
        // 具体内容可以根据需要调用drawBar函数
    } while (display.nextPage());
}*/

// 初始化菜单
void initMenu() {
    currentState.currentMenuLevel = 0;
    currentState.selectedIndex = 0;
    currentState.scrollOffset = 0;
    historyDepth = 0;
    
    // 输出调试信息
    Serial.println("========== Menu Initialized ==========");
    Serial.print("Screen: ");
    Serial.print(EPD_WIDTH);
    Serial.print(" x ");
    Serial.println(EPD_HEIGHT);
    Serial.print("Menu area: x=");
    Serial.print(menuArea.x);
    Serial.print(", y=");
    Serial.print(menuArea.y);
    Serial.print(", w=");
    Serial.print(menuArea.width);
    Serial.print(", h=");
    Serial.println(menuArea.height);
    Serial.print("Visible items: ");
    Serial.println(MENU_VISIBLE_ITEMS);
    Serial.print("Item height: ");
    Serial.println(MENU_ITEM_HEIGHT);
    Serial.println("======================================");
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    // 渲染菜单
    renderFullMenu();
    Serial.println("Menu initialized successfully");
}

void onFileSelected() {
    Serial.println("onFileSelected called");
    handleFileBrowserSelect();
}