// sdcard.cpp
#include "init.h"
#include "menu.h"

// 全局变量
String currentPath = "/";
FileItem currentFiles[MAX_FILE_ITEMS];
int currentFileCount = 0;
bool inFileBrowser = false;

// 初始化SD卡
bool initSDCard() {
        Serial.println("Initializing SD card...");
    
    // 重新配置SPI引脚（先释放EPD的SPI）
    SPI.end();
    delay(10);
    
    // 使用与测试程序相同的方式初始化SPI
    // 注意参数顺序: SPI.begin(sck, miso, mosi, cs)
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    delay(10);
    
    // 使用与测试程序相同的方式初始化SD卡
    if (!SD.begin(SD_CS)) {
        Serial.println("SD Card initialization failed!");
        Serial.println("Trying with alternative SPI settings...");
        
        // 尝试不同的SPI频率
        if (!SD.begin(SD_CS, SPI, 4000000)) {
            Serial.println("Still failed!");
            return false;
        }
    }
    
    // 获取SD卡信息
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return false;
    }
    
    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) Serial.println("MMC");
    else if (cardType == CARD_SD) Serial.println("SDSC");
    else if (cardType == CARD_SDHC) Serial.println("SDHC");
    else Serial.println("UNKNOWN");
    
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    
    // 测试列出根目录（调试用）
    Serial.println("Testing root directory listing:");
    File root = SD.open("/");
    if (root) {
        Serial.println("Root opened successfully!");
        File file = root.openNextFile();
        int count = 0;
        while (file) {
            count++;
            Serial.printf("  %s %s\n", file.name(), file.isDirectory() ? "[DIR]" : "");
            file = root.openNextFile();
        }
        root.close();
        Serial.printf("Total items found: %d\n", count);
    } else {
        Serial.println("Cannot open root directory!");
    }
    
    return true;
}

// 重新初始化EPD的SPI（在退出文件浏览后调用）
void reinitEPDSPI() {
    SPI.end();
    SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);
    display.epd2.selectSPI(SPI, SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));
}

// 列出目录内容
void listDirectory(String path, FileItem* items, int* count) {
    *count = 0;
    
    File root = SD.open(path);
    if (!root) {
        Serial.println("Failed to open directory: " + path);
        return;
    }
    
    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return;
    }
    
    File file = root.openNextFile();
    while (file && *count < MAX_FILE_ITEMS) {
        items[*count].name = file.name();
        items[*count].isDirectory = file.isDirectory();
        items[*count].path = path;
        
        // 如果是目录，路径后加/
        if (file.isDirectory()) {
            items[*count].path = path + file.name() + "/";
        } else {
            items[*count].path = path;
        }
        
        Serial.printf("Found: %s %s\n", file.name(), file.isDirectory() ? "[DIR]" : "[FILE]");
        
        (*count)++;
        file = root.openNextFile();
    }
    root.close();
    
    // 排序：目录在前，文件在后
    for (int i = 0; i < *count - 1; i++) {
        for (int j = i + 1; j < *count; j++) {
            if (!items[i].isDirectory && items[j].isDirectory) {
                FileItem temp = items[i];
                items[i] = items[j];
                items[j] = temp;
            }
        }
    }
}

// 浏览目录
void browseDirectory(String path) {
    Serial.print("browseDirectory: ");
    Serial.println(path);
    
    // 保存当前路径到历史（如果需要返回）
    // 注意：这里不保存历史，由调用者决定
    
    currentPath = path;
    listDirectory(currentPath, currentFiles, &currentFileCount);
    
    inFileBrowser = true;
    
    // 直接进入文件浏览器
    enterFileBrowser();
}