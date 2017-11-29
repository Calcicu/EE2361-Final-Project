#ifndef LCDLIB_H
#define	LCDLIB_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

extern int CONTRAST;

void lcd_cmd(char package);
void lcd_init(void);
void setCursor(char x, char y);
void lcdPrintChar(char package);
void lcdPrintStr(const char *s);
void waitms(int ms);

#endif	/* LCDLIB_H */

