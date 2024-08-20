#include "oled.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void oled_init() {
    u8g2.begin();
    u8g2.enableUTF8Print();
}

void oled_show(float temp, float humidity, int count) {
    u8g2.clearBuffer();
    // 汉字字体的大小为 16*16像素
    // 屏幕像素宽128，设置x=40，可以起到居中效果，计算公式：(128-16*3) / 2
    //  u8g2.setCursor(/*x*/ 40, /*y*/ 15);
    // 利用U8g2自带的字体类型画天气符号
    //  u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
    //  u8g2.drawGlyph(3, 16, 67);  //代表雨
    // 画一条横线
    //  u8g2.drawLine(/*x-start*/ 0, /*y-start*/ 17, /*x-end*/ 128, /*y-end*/ 17);

    u8g2.setFont(u8g2_font_chinese);
    u8g2.setCursor(2, 20);
    u8g2.print("加湿: " + String(count) + "次");
    u8g2.setCursor(2, 37);
    u8g2.print("温度: " + String(temp, 2));
    u8g2.setCursor(2, 55);
    u8g2.print("湿度: " + String(humidity, 2));
    // Serial.println(temp.temperature);
    // 画边框线示例。先执行此代码，后面在画字体时，可能会将“线条”冲掉
    u8g2.drawFrame(/*x*/ 0, /*y*/ 0, /*width*/ 128, /*height*/ 64);
    u8g2.sendBuffer();
}