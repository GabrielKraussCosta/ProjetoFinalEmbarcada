#include "adc.h"
#include "lcd.h"
#include "ssd.h"
#include "timer.h"
#include "config.h"
#include "keypad.h"
#include "serial.h"
#include <proc/pic18f4520.h>

#define L_ON  0x0F
#define L_OFF 0x08
#define L_CLR 0x01
#define L_L1  0x80
#define L_L2  0xC0

char Flag = 1;
int ValorAD = 0;
unsigned int Leitura = 0;

void LeTeclado(void){
    if(kpRead() != Leitura){
        Leitura = kpRead();
        
        switch(kpReadKey()){
            case 'U':
                lcdCommand(L_L1);
                lcdString("Bruno");
                break;
            case 'L':
                lcdCommand(L_L2);
                lcdString("2021002510");
                break;
            case 'D':
                Flag = 2;
                
                ssdDigit(14, 0);
                ssdDigit(12, 1);
                ssdDigit( 0, 2);
                ssdDigit(16, 3);
                break;
            case 'R':
                Flag = 1;
                
                ssdDigit(16, 0);
                ssdDigit(16, 1);
                ssdDigit(16, 2);
                ssdDigit(16, 3);
                break;
            case 'S':
                lcdCommand(L_CLR);
                break;
            default:
                break;
        }        
    }
}

void Serial(void){    
    char Tecla;
    
    Tecla = serialRead();
    
    if(Tecla != 0){
        switch(Tecla){
            case 'U':
                lcdCommand(L_L1);
                lcdString("Bruno");
                break;
            case 'L':
                lcdCommand(L_L2);
                lcdString("2021002510");
                break;
            case 'D':
                Flag = 2;
                
                ssdDigit(14, 0);
                ssdDigit(12, 1);
                ssdDigit( 0, 2);
                ssdDigit(16, 3);
                break;
            case 'R':
                Flag = 1;
                
                ssdDigit(16, 0);
                ssdDigit(16, 1);
                ssdDigit(16, 2);
                ssdDigit(16, 3);
                break;
            case 'S':
                lcdCommand(L_CLR);
                break;
            default:
                break;
        }
    }
    
    ValorAD = adcRead(0);
    
    serialSend('T');
    serialSend(':');
    serialSend(' ');
    serialSend((ValorAD/1000) % 10 + '0');
    serialSend((ValorAD/100)  % 10 + '0');
    serialSend((ValorAD/10)   % 10 + '0');
    serialSend((ValorAD/1)    % 10 + '0');
    serialSend('\n');
}

void MostraDisplay(void){
    ssdDigit((ValorAD/1000) % 10, 0);
    ssdDigit((ValorAD/100) % 10, 1);
    ssdDigit((ValorAD/10) % 10, 2);
    ssdDigit((ValorAD/1) % 10, 3);
}

void main(void){
    char slot;
    
    kpInit();
    adcInit();
    lcdInit();
    ssdInit();
    timerInit();
    serialInit();    
    
    ssdDigit(16, 0);
    ssdDigit(16, 1);
    ssdDigit(16, 2);
    ssdDigit(16, 3);
    
    lcdCommand(L_CLR);
    
    for(;;){
        timerReset(5000);
        ssdUpdate();
        
        switch(slot){
            case 0:
                LeTeclado();
                slot = 1;
                break;
            case 1:
                Serial();
                slot = 2;
                break;
            case 2:
                kpDebounce();                
                slot = 3;
                break;
            case 3:
                if(Flag == 1){
                    MostraDisplay();
                }
                
                slot = 0;
                break;
            default:
                slot = 0;
                break;
        }
        
        timerWait();
    }
}