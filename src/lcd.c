#include "xc.h"
#include "../include/lcd.h"

void delay_ms(unsigned int ms) { // 1ms delay at Fcy = 16MHz
    while (ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}

void delay_us(unsigned int us) { // 1us delay at Fcy = 16MHz
    while (us-- > 0) {
        asm("repeat #1598");
        asm("nop");
    }
}

void lcd_init(void) { // Initialize LCD in 4-bit mode
    I2C2BRG = 156; //100kHz
    IFS3bits.MI2C2IF = 0; //clears interrupt flag
    I2C2CONbits.I2CEN = 1; //Enable I2C mode
            
    delay_ms(40); //power on delay
    
    lcd_cmd6(0x38); //function set
    delay_us(27);
    
    lcd_cmd6(0x39); //function set, advanced instruction Mode
    delay_us(27);
    
    lcd_cmd6(0x14); //interval osc
    delay_us(27);
    
    lcd_cmd6(0x70); //contrast low
    delay_us(27);
    
    lcd_cmd6(0x56); //Power/ICON/Contrast Control
    delay_us(27);
    
    lcd_cmd6(0x6c); //follower contrast
    
    delay_ms(201); //power stabalizes
    
    lcd_cmd6(0x38); //function set
    delay_us(27);
    
    lcd_cmd6(0x0c); //Display On
    delay_us(27);
    
    lcd_cmd6(0x01); //Clear Display
    
    delay_ms(1);
}

void lcd_cmd6(char command) { //Sends command to LCD
    
  I2C2CONbits.SEN = 1;	//Initiate Start condition
  while (I2C2CONbits.SEN == 1);  // SEN will clear when Start Bit is complete
  IFS3bits.MI2C2IF = 0;
  
  I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
  while (IFS3bits.MI2C2IF == 0); // *Refer to NOTE below*
  IFS3bits.MI2C2IF = 0;
  
  I2C2TRN = 0b00000000; // 8-bits consisting of control byte
  while (IFS3bits.MI2C2IF == 0);
  IFS3bits.MI2C2IF = 0;
  
  I2C2TRN = command; // 8-bits consisting of the data byte
  while (IFS3bits.MI2C2IF == 0);
  IFS3bits.MI2C2IF = 0;
  
  I2C2CONbits.PEN = 1;
  while (I2C2CONbits.PEN == 1); // PEN will clear when Stop bit is complete
  IFS3bits.MI2C2IF = 0;

}

void lcd_setCursor(char x, char y) { // x = col, y = row
    char cursor = ((0x40*y) + x) | 0x80;
    lcd_cmd6(cursor);
}

void lcd_printChar(char myChar) { //Sends character to LCD
  
  I2C2CONbits.SEN = 1;	//Initiate Start condition
  while (I2C2CONbits.SEN == 1);  // SEN will clear when Start Bit is complete
  IFS3bits.MI2C2IF = 0;
  
  I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
  while (IFS3bits.MI2C2IF == 0); // *Refer to NOTE below*
  IFS3bits.MI2C2IF = 0;
  
  I2C2TRN = 0b01000000; // 8-bits consisting of control byte
  while (IFS3bits.MI2C2IF == 0);
  IFS3bits.MI2C2IF = 0;
  
  I2C2TRN = myChar; // 8-bits consisting of the data byte
  while (IFS3bits.MI2C2IF == 0);
  IFS3bits.MI2C2IF = 0;
  
  I2C2CONbits.PEN = 1;
  while (I2C2CONbits.PEN == 1); // PEN will clear when Stop bit is complete
  IFS3bits.MI2C2IF = 0;
  
}

void lcd_printStr(const char s[]) { //Sends string to LCD
    int i = 0;
    
    I2C2CONbits.SEN = 1;	//Initiate Start condition
    while (I2C2CONbits.SEN == 1);  // SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF = 0;
  
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while (IFS3bits.MI2C2IF == 0); // *Refer to NOTE below*
    IFS3bits.MI2C2IF = 0;
    
    while (s[i] != '\0') {
        
        if (s[i+1] == '\0') {
            I2C2TRN = 0b01000000; // 8-bits consisting of control byte 0 for non-repetition
        } else {
            I2C2TRN = 0b11000000; // 8-bits consisting of control byte 1 for repetition
        }
        while (IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
  
        I2C2TRN = s[i]; // 8-bits consisting of the data byte
        while (IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
        
        i++;
    }
    
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN == 1); // PEN will clear when Stop bit is complete
    IFS3bits.MI2C2IF = 0;
}
