# PROJE-2
Projenin temel amacı, LCD ekran üzerinden gerçek zamanlı bir kronometre görüntülemektir. Kod, LCD ekranın sağ alt köşesinde formatlanmış bir zaman göstergesi (hh:mm:ss) ile bir sayıcı uygular. Proje çalıştırılmadan önce belirli bir başlatma sırasına dikkat edilmesi gerektiği keşfedilmiştir.
Bu proje, bir Tiva C Series (TM4C123) mikrodenetleyicisi kullanarak bir 4-bit modunda çalışan LCD ekran üzerinde basit bir kronometre uygulamasını gerçekleştirmektedir. Proje, saat, dakika ve saniyeyi sürekli olarak ekranda gösteren bir kronometre uygulaması içerir.
********************
LCD Fonksiyonları:
LCD ekranın başlatılması, temizlenmesi, komut ve veri gönderilmesi için kullanılan fonksiyonları içerir:
LCD_Init() - LCD ekranını başlatır.
LCD_Command(uint8_t cmd) - LCD'ye komut gönderir.
LCD_WriteChar(char c) - Tek bir karakter yazar.
LCD_WriteString(const char *str) - Ekrana bir yazı dizisi yazar.
LCD_Clear() - Ekranı temizler.
**************
Kronometre:
Kronometreyi saat, dakika ve saniye olarak formatlar ve her saniye ekranı günceller:

FormatTime(char *buffer, uint8_t hours, uint8_t minutes, uint8_t seconds) - Zamanı "hh:mm:ss" formatında hazırlar.
Zaman, her saniye güncellenir ve LCD ekranın sağ alt köşesinde gösterilir.
**************
Gecikme Fonksiyonu:
delayMs(int ms) - Milisaniye bazlı bekleme için kullanılır.
Kodun Çalıştırılması
***********
Sistem Saat Ayarı:
SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN)
Tiva C'nin 40 MHz sistem saati için yapılandırılması yapılır.

GPIO Konfigürasyonu:
Port B'nin LCD veri ve kontrol pinleri için kullanımı etkinleştirilir:
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
************************
LCD Başlatma:
LCD, 4-bit modunda çalıştırılır.
Başlatma sırasında sırasıyla FUNCTION_SET, DISPLAY_ON_CURSOR_OFF, CLEAR_DISPLAY ve ENTRY_MODE_SET komutları gönderilir.
************************
Kronometre:
Her saniye güncellenen bir sayıcı, LCD ekranın belirli bir konumuna yazdırılır.
Zaman değişkenleri (hours, minutes, seconds) formatlanarak LCD ekranına aktarılır.
