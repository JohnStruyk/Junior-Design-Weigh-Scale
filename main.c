#include "xc.h"
#include "include/circular_buffer.h"
#include "include/lcd.h"
#include <stdio.h>
#include <p24Fxxxx.h>

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))


#define SAMPLE_DIVIDER 16
int avgVoltage = 0;
int updatelcd = 0;

void setup(void) { // setup the device
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG |= 0x9fff; // setup digital IO
}

void __attribute__((__interrupt__,__auto_psv__)) _ADC1Interrupt(void) {
    _AD1IF = 0;
    putVal(ADC1BUF0);
}

void __attribute__((__interrupt__,__auto_psv__)) _T3Interrupt(void) {
    _T3IF = 0;
}

void __attribute__((__interrupt__,__auto_psv__)) _T1Interrupt(void) {
    _T1IF = 0;
    updatelcd = 1;
}

void adc_init() { // Initialize ADC
    TRISAbits.TRISA0 = 1;  //sets RA0 input
            
    AD1PCFGbits.PCFG0 = 0; //setup I/O analog
    
    AD1CON2bits.VCFG = 0b000; // uses AVDD (3.3V) and AVSS (0V) as max/min
    AD1CON3bits.ADCS = 1; //TAD >= 75ns (Tcy = 62.5ns) = 2*Tcy
    AD1CON1bits.SSRC = 2; //sample on timer 3 events
    AD1CON3bits.SAMC = 1; // 1 auto sample time bit
    AD1CON1bits.FORM = 0b00; // data output form, unsigned 0V = 0b0000000000, 3.3V = 0b1111111111
    
    AD1CON1bits.ASAM = 1; //sampling begins immediately after last sampling completes
    AD1CON2bits.SMPI = 0b0000; //sets ADC interrupt after 1 samples
    AD1CON1bits.ADON = 1; //turn on ADC
    
    _AD1IF = 0; //clear IF
    _AD1IE = 1; //interrupt enable
    
    TMR3 = 0; //setup timer3
    T3CON = 0;
    T3CONbits.TCKPS = 0b11;
    PR3 = (62499/SAMPLE_DIVIDER);
    _T3IF = 0;
    _T3IE = 1;
    T3CONbits.TON = 1;
    
    TMR1 = 0; //setup timer3
    T1CON = 0;
    T1CONbits.TCKPS = 0b11;
    PR1 = 6259; //100ms
    _T1IF = 0;
    _T1IE = 1;
    T1CONbits.TON = 1;
}

int main(void) { // main function
    setup();
    lcd_init();
    adc_init();
    
    lcd_setCursor(0,0);
    char adStr[20];
    while(1) {
        while(updatelcd == 0);
        updatelcd = 0;
        avgVoltage = getAvg();
        lcd_setCursor(0,0);
        sprintf(adStr,"%6.4f V", ((3.3/1024)*avgVoltage));
        lcd_printStr(adStr);
    }
}
