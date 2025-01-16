//en son �al��an kod bu, LCD nin sa� alt k��esinde bir kronometre say�yor. Ama ��yle bir problem var daha �nce ba�ka bir �al��an kodu �al��t�r�p ard�ndan bunu �al��t�r�nca �al���yor. bunu ��z .Y ani �nce apili kodda tek harf yazd�rma kodunu �al��t�r�nca ard�ndan bu �al���yor.!!!!!!!
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// LCD Pin Tan�mlar�
#define RS GPIO_PIN_0 // Register Select
#define E GPIO_PIN_1  // Enable
#define DATA (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7) // Veri hatt� D4-D7

// LCD Komutlar�
#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02
#define ENTRY_MODE_SET 0x06
#define DISPLAY_ON_CURSOR_OFF 0x0C
#define FUNCTION_SET 0x28
#define SET_CURSOR 0x80 // �lk sat�r, ilk s�tun

// LCD Fonksiyon Prototipleri
void LCD_Command(uint8_t cmd);
void LCD_Init(void);
void LCD_WriteChar(char c);
void LCD_WriteString(const char *str);
void delayMs(int ms);
void FormatTime(char *buffer, uint8_t hours, uint8_t minutes, uint8_t seconds);

// Ana De�i�kenler
uint8_t hours = 0, minutes = 0, seconds = 0;

// Ana Program
int main(void) {
    // Sistem saatini ayarla
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Port B'yi etkinle�tir
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, RS | E | DATA);

    // LCD ba�lat
    LCD_Init();

    while (1) {
        // Kronometreyi saat, dakika, saniye olarak g�ster
        char timeStr[9]; // Zaman� g�sterecek buffer
        FormatTime(timeStr, hours, minutes, seconds); // Zaman� formatla

        LCD_Command(SET_CURSOR | 0x48); //LCD_Command(SET_CURSOR); // �mleci ilk sat�r ve s�tuna g�t�r
        LCD_WriteString(timeStr);

        // 1 saniye bekle ve zaman� g�ncelle
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

// LCD Komut G�nderme Fonksiyonu
void LCD_Command(uint8_t cmd) {
    GPIOPinWrite(GPIO_PORTB_BASE, RS, 0);           // RS = 0 (Komut modu)
    GPIOPinWrite(GPIO_PORTB_BASE, DATA, cmd & 0xF0); // Y�ksek nibble g�nder
    GPIOPinWrite(GPIO_PORTB_BASE, E, E);            // Enable pulse
    delayMs(1);
    GPIOPinWrite(GPIO_PORTB_BASE, E, 0);

    GPIOPinWrite(GPIO_PORTB_BASE, DATA, (cmd << 4) & 0xF0); // D���k nibble g�nder
    GPIOPinWrite(GPIO_PORTB_BASE, E, E);
    delayMs(1);
    GPIOPinWrite(GPIO_PORTB_BASE, E, 0);

    delayMs(2); // Komutun i�lenmesi i�in bekleme s�resi
}

// LCD Ba�latma Fonksiyonu
void LCD_Init(void) {
    delayMs(20);                 // Ba�lang�� beklemesi
    LCD_Command(FUNCTION_SET);   // 4-bit mod, 2 sat�r
    LCD_Command(DISPLAY_ON_CURSOR_OFF); // Ekran a��k, imle� kapal�
    LCD_Command(CLEAR_DISPLAY);  // Ekran� temizle
    LCD_Command(ENTRY_MODE_SET); // �mle� sa�a hareket eder
}

// Karakter Yazma Fonksiyonu
void LCD_WriteChar(char c) {
    GPIOPinWrite(GPIO_PORTB_BASE, RS, RS);         // RS = 1 (Veri modu)
    GPIOPinWrite(GPIO_PORTB_BASE, DATA, c & 0xF0); // Y�ksek nibble g�nder
    GPIOPinWrite(GPIO_PORTB_BASE, E, E);          // Enable pulse
    delayMs(1);
    GPIOPinWrite(GPIO_PORTB_BASE, E, 0);

    GPIOPinWrite(GPIO_PORTB_BASE, DATA, (c << 4) & 0xF0); // D���k nibble g�nder
    GPIOPinWrite(GPIO_PORTB_BASE, E, E);
    delayMs(1);
    GPIOPinWrite(GPIO_PORTB_BASE, E, 0);

    delayMs(2); // Veri yazma beklemesi
}

// Yaz� Yazma Fonksiyonu
void LCD_WriteString(const char *str) {
    while (*str) {
        LCD_WriteChar(*str++);
    }
}

// Zaman� Formatlama Fonksiyonu
void FormatTime(char *buffer, uint8_t hours, uint8_t minutes, uint8_t seconds) {
    buffer[0] = (hours / 10) + '0';  // Onlar basama��
    buffer[1] = (hours % 10) + '0';  // Birler basama��
    buffer[2] = ':';                 // Ay�r�c�
    buffer[3] = (minutes / 10) + '0';
    buffer[4] = (minutes % 10) + '0';
    buffer[5] = ':';
    buffer[6] = (seconds / 10) + '0';
    buffer[7] = (seconds % 10) + '0';
    buffer[8] = '\0';                // Dize sonland�r�c�
}

// Milisaniye Bekleme Fonksiyonu
void delayMs(int ms) {
    SysCtlDelay((SysCtlClockGet() / 3 / 1000) * ms);
}
