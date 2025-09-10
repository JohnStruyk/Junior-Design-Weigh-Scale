#ifndef STRUY013_LCD6_H
#define	STRUY013_LCD6_H

#ifdef	__cplusplus
extern "C" {
#endif
    void delay_ms(unsigned int ms);
    void delay_us(unsigned int us);
    void lcd_init(void);
    void lcd_cmd6(char command);
    void lcd_setCursor(char x, char y);
    void lcd_printChar(char myChar);
    void lcd_printStr(const char s[]);
#ifdef	__cplusplus
}
#endif

#endif	/* STRUY013_LCD_H */

