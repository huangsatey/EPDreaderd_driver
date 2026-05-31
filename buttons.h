// buttons.h
#include <OneButton.h>

// 声明全局按钮对象
extern OneButton btnLeft;
extern OneButton btnRight;
extern OneButton btnMid;

// 按钮初始化函数
void initButtons();

// 按钮更新函数
void updateButtons();

// 左按钮
void onLeftClick();
void onLeftDoublePress();

// 右按钮
void onRightClick();
void onRightDoubleClick();

// 中按钮
void onMidClick();
void onMidLongPress();