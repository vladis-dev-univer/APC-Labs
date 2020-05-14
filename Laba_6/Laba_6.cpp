#include <dos.h>
#include <conio.h>
#include <stdio.h>

#define true 1
#define false 0
#define COUNT 3

int cont = true;
int again = true;
void blink();

void interrupt(*oldKeyboardInterrupt)(...); // Указатель на обработчик прерывания
void interrupt newKeyboardnterrupt(...) {   // Функция обработки прерывания
    //char keyCode = 0;
    if(inp(0x60) == 0x01) cont = false; //Получаем значение из порта 60h и устанавливаем флаг выхода(нажата Esc)

    if(inp(0x60) == 0xFE){  // код ошибки, байт не обработан
        again = true;
    } else again = false;

printf("key:%x\n", inp(0x60));
outp(0x20, 0x20); //сброс контрооллера прерывания
}

void setKeyboardInterrupt() {
    _disable();
     oldKeyboardInterrupt = getvect(0x09); //Сохраняем указатель на старый обработчик(keyboard)
     setvect(0x09, newKeyboardnterrupt);  // Меняем его на новый
     _enable();
    }

void resetKeyboardInterrupt() {
     _disable();
      setvect(0x09, oldKeyboardInterrupt); // Восстанавливаем старый обработчик прерывания
      _enable();
    }

int main() {
    setKeyboardInterrupt();

    blink();
    while (cont);

    resetKeyboardInterrupt();
    return 0;
}

// Функции управления индикаторами
void setIndicator(unsigned char mask);
void blink(void);
int writeToPort(unsigned char mask) {
    for(int i = 0; i <= COUNT; i++){
        while(inp(0x64) & 0x02); // Пока нет подтверждения успешного выполнения команды
        outp(0x60, mask);
        delay(50);
         if(again == 0) return true;
        }
    return false;
}

void setIndicator(unsigned char mask) {
    again = 1;
    if(!writeToPort(0xED)){ // для управления индикатором
        return;
    }
    again = 1;
    if(!writeToPort(mask)){ // маска
        return;
    }
}
//0x05 - индикатор Num Lock,0x06 - индикатор Caps Lock
unsigned char mask[] = {
      0x06, 0x02, 0x05, 0x02, 0x06, 0x02, 0x05, 0x02, 0x06, 0
     // 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x02,
     // 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x06, 0x06,
     // 0x06, 0x06, 0x06, 0x00, 0x00, 0x02, 0x02, 0x02,
     // 0x02, 0x02,  0x00, 0x00,0
};

int delayTime[] = {
    900,500,900,500,900,500,900,500,900,0
    // 900,900,900,900,900,500,500,900,
    // 900,900,900,900,500,500,900,900,
    // 900,900,900,500,500,900,900,900,
    // 900,900,500,500,1500,0
};

 // Функция мигания индикаторами
void blink() {
    for(int i = 0; mask[i] != 0; i++){
        setIndicator(mask[i]);
        delay(delayTime[i]);
        setIndicator(0x00);
        delay(100);
    }
}
