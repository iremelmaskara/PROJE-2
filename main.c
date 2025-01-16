//en son çalýþan kod bu, LCD nin sað alt köþesinde bir kronometre sayýyor. Ama þöyle bir problem var daha önce baþka bir çalýþan kodu çalýþtýrýp ardýndan bunu çalýþtýrýnca çalýþýyor. bunu çöz .Y ani önce apili kodda tek harf yazdýrma kodunu çalýþtýrýnca ardýndan bu çalýþýyor.!!!!!!!
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// LCD Pin Tanýmlarý
#define RS GPIO_PIN_0 // Register Select
#define E GPIO_PIN_1  // Enable
#define DATA (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7) // Veri hattý D4-D7

// LCD Komutlarý
#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02
#define ENTRY_MODE_SET 0x06
#define DISPLAY_ON_CURSOR_OFF 0x0C
#define FUNCTION_SET 0x28
#define SET_CURSOR 0x80 // Ýlk satýr, ilk sütun

// LCD Fonksiyon Prototipleri
void LCD_Command(uint8_t cmd);
void LCD_Init(void);
void LCD_WriteChar(char c);
void LCD_WriteString(const char *str);
void delayMs(int ms);
void FormatTime(char *buffer, uint8_t hours, uint8_t minutes, uint8_t seconds);

// Ana Deðiþkenler
uint8_t hours = 0, minutes = 0, seconds = 0;

// Ana Program
int main(void) {
    // Sistem saatini ayarla
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Port B'yi etkinleþtir
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, RS | E | DATA);

    // LCD baþlat
    LCD_Init();

    while (1) {
        // Kronometreyi saat, dakika, saniye olarak göster
        char timeStr[9]; // Zamaný gösterecek buffer
        FormatTime(timeStr, hours, minutes, seconds); // Zamaný formatla

        LCD_Command(SET_CURSOR | 0x48); //LCD_Command(SET_CURSOR); // Ýmleci ilk satýr ve sütuna götür
        LCD_WriteString(timeStr);

        // 1 saniye bekle ve zamaný güncelle
        delayMs(1000);
        seconds++;
        if (seconds == 60) {
            seconds = 0;
            minutes++;
        }
        if (minutes == 60) {
            minutes = 0;
            hours++;
        }
    }
}

// LCD Komut Gönderme Fonksiyonu
void LCD_Command(uint8_t cmd) {
    GPIOPinWrite(GPIO_PORTB_BASE, RS, 0);           // RS = 0 (Komut modu)
    GPIOPinWrite(GPIO_PORTB_BASE, DATA, cmd & 0xF0); // Yüksek nibble gönder
    GPIOPinWrite(GPIO_PORTB_BASE, E, E);            // Enable pulse
    delayMs(1);
    GPIOPinWrite(GPIO_PORTB_BASE, E, 0);

    GPIOPinWrite(GPIO_PORTB_BASE, DATA, (cmd << 4) & 0xF0); // Düþük nibble gönder
    GPIOPinWrite(GPIO_PORTB_BASE, E, E);
    delayMs(1);
    GPIOPinWrite(GPIO_PORTB_BASE, E, 0);

    delayMs(2); // Komutun iþlenmesi için bekleme süresi
}

// LCD Baþlatma Fonksiyonu
void LCD_Init(void) {
    delayMs(20);                 // Baþlangýç beklemesi
    LCD_Command(FUNCTION_SET);   // 4-bit mod, 2 satýr
    LCD_Command(DISPLAY_ON_CURSOR_OFF); // Ekran açýk, imleç kapalý
    LCD_Command(CLEAR_DISPLAY);  // Ekraný temizle
    LCD_Command(ENTRY_MODE_SET); // Ýmleç saða hareket eder
}

// Karakter Yazma Fonksiyonu
void LCD_WriteChar(char c) {
    GPIOPinWrite(GPIO_PORTB_BASE, RS, RS);         // RS = 1 (Veri modu)
    GPIOPinWrite(GPIO_PORTB_BASE, DATA, c & 0xF0); // Yüksek nibble gönder
    GPIOPinWrite(GPIO_PORTB_BASE, E, E);          // Enable pulse
    delayMs(1);
    GPIOPinWrite(GPIO_PORTB_BASE, E, 0);

    GPIOPinWrite(GPIO_PORTB_BASE, DATA, (c << 4) & 0xF0); // Düþük nibble gönder
    GPIOPinWrite(GPIO_PORTB_BASE, E, E);
    delayMs(1);
    GPIOPinWrite(GPIO_PORTB_BASE, E, 0);

    delayMs(2); // Veri yazma beklemesi
}

// Yazý Yazma Fonksiyonu
void LCD_WriteString(const char *str) {
    while (*str) {
        LCD_WriteChar(*str++);
    }
}

// Zamaný Formatlama Fonksiyonu
void FormatTime(char *buffer, uint8_t hours, uint8_t minutes, uint8_t seconds) {
    buffer[0] = (hours / 10) + '0';  // Onlar basamaðý
    buffer[1] = (hours % 10) + '0';  // Birler basamaðý
    buffer[2] = ':';                 // Ayýrýcý
    buffer[3] = (minutes / 10) + '0';
    buffer[4] = (minutes % 10) + '0';
    buffer[5] = ':';
    buffer[6] = (seconds / 10) + '0';
    buffer[7] = (seconds % 10) + '0';
    buffer[8] = '\0';                // Dize sonlandýrýcý
}

// Milisaniye Bekleme Fonksiyonu
void delayMs(int ms) {
    SysCtlDelay((SysCtlClockGet() / 3 / 1000) * ms);
}
