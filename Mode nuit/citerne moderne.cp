#line 1 "D:/cours/2A/semestre 2/micro/projet/integration/integration(nuit)/citerne moderne.c"
#line 1 "c:/users/public/documents/mikroelektronika/mikroc pro for pic/include/stdbool.h"



 typedef char _Bool;
#line 19 "D:/cours/2A/semestre 2/micro/projet/integration/integration(nuit)/citerne moderne.c"
sbit LCD_RS at RD4_bit;
sbit LCD_EN at RD5_bit;
sbit LCD_D4 at RD0_bit;
sbit LCD_D5 at RD1_bit;
sbit LCD_D6 at RD2_bit;
sbit LCD_D7 at RD3_bit;
sbit LCD_RS_Direction at TRISD4_bit;
sbit LCD_EN_Direction at TRISD5_bit;
sbit LCD_D4_Direction at TRISD0_bit;
sbit LCD_D5_Direction at TRISD1_bit;
sbit LCD_D6_Direction at TRISD2_bit;
sbit LCD_D7_Direction at TRISD3_bit;

unsigned int adc_value;
float voltage;
unsigned int percentage;
unsigned int remplissage;
unsigned int remp=0;
unsigned int indice = 0;
unsigned int clean=0;
void interrupt(){
 int i=0;
 if( portd.RD7 ==1){
 if ( Portb.RB6  == 1) {
  Portc.RC5  = 1;
  Portc.RC1  = 1;
  Portc.RC0  = 0;
 remplissage++;
 indice = 1;
 }
 if(INTCON.EEIF){
 indice = 1;
 INTCON.EEIF = 0;
 }
 if ( Portb.RB4  == 1) {
  Portc.RC5  = 0;
  Portc.RC1  = 0;
  Portc.RC0  = 1;
 }
 INTCON.RBIF = 0;
 }
 if( portb.RB0 ==1){
 if( Portb.RB4 ==1){
  { Portc.RC5 =0;} ;
  { Portc.RC1 =0;} ;
 }
 if( Portb.RB5 ==1){
  { Portc.RC3 =1;} ;
 Delay_ms(100);
  { Portc.RC3 =0;} ;
 for(i=0;i<3;i++){
  { Portc.RC0 =1;} ;
 Delay_ms(1000);
  { Portc.RC0 =0;} ;
 Delay_ms(1000);
 }
 Delay_ms(2000);
 }
 INTCON.RBIF = 0;
 }
}
int main(){
 int i=0;
 int repos=1;

 INTCON.GIE = 0;
 EEPROM_Write(0x20, 75);
 indice = 0;
 INTCON.GIE = 1;

 ADCON1 = 0x80;
 EECON1 = 0x00;
 INTCON.GIE=1;
 INTCON.RBIE=1;
 INTCON.EEIE = 1;
 INTCON.PEIE = 1;
 EECON1 = 0x00;
 TRISA.RA0=1;
 TRISB.RB0=1;
 TRISB.RB1=1;
 TRISB.RB4=1;
 TRISB.RB5=1;
 TRISB.RB6=1;
 TRISB.RB7=1;
 TRISC.RC0=0;
 TRISC.RC1=0;
 TRISC.RC2=0;
 TRISC.RC3=0;
 TRISC.RC4=0;
 TRISC.RC5=0;
 TRISD.RD7=1;
  { Portc.RC0 =0;} ;
  { Portc.RC1 =0;} ;
  { Portc.RC5 =0;} ;
 LCD_Init();
  { Portc.RC2 =0;} ;
 remp=0;
 while(1){
 adc_value = ADC_Read(0);
 voltage = adc_value * 5.0 / 1023.0;
 percentage = voltage / 5.0 * 100;
 if( Portb.RB1 ==1 ||  Portb.RB4 ==1 ||  Portb.RB5 ==1 ||  Portb.RB6 ==1 ||  portb.RB0 ==1 ||  portd.RD7 ==1){
 repos=0;
 }
 else{
 repos=1;
 }
 if ( portd.RD7 ==1){
 Lcd_Out(1,1,"Mode nuit");
 Lcd_Cmd(_LCD_CURSOR_OFF);
 if (indice == 1) {
 INTCON.GIE = 0;
 EEPROM_Write(0x50, remplissage);
 indice = 0;
 INTCON.GIE = 1;
 }
 if ( Portb.RB4  == 1) {
 Lcd_Chr(2, 1, ' ');
 Lcd_Chr(2, 2, (percentage / 100) % 10 + 48);
 Lcd_Chr(2, 3, (percentage / 10) % 10 + 48);
 Lcd_Chr(2, 4, percentage % 10 + 48);
 Lcd_Out(2, 5, "%");
 Delay_ms(500);
 if(percentage>EEPROM_Read(0x20)){
 int j;
 Lcd_Out(2,6,"Eau Sale!!");
  Portc.RC4 =1;
 for(j=0;j<5;j++){
  { Portc.RC2 =1;} ;
 Delay_ms(1000);
  { Portc.RC2 =0;} ;
 Delay_ms(1000);
 }
  Portc.RC4 =0;
 }
 if(percentage<EEPROM_Read(0x20)){
 Lcd_Out(2,6,"Eau Propre");
 Lcd_Cmd(_LCD_CURSOR_OFF);
 }
 }
 if(EEPROM_Read(0x50)>5)
 {
 Lcd_Out(2,6,"Gaspillage");
 Lcd_Cmd(_LCD_CURSOR_OFF);
  Portc.RC3 =1;
 if( portb.RB7 =1)
 {
  Portc.RC3 =0;
  Portc.RC0 =0;
  Portc.RC1 =0;
 remplissage=0;
 EEPROM_Write(0x50,remplissage);
 Lcd_Out(2,1,"                                  ");
 Lcd_Cmd(_LCD_CURSOR_OFF);
 }
 }
 }
 if(repos==1){
 LCD_Out(1,1,"Bienvenue");
 LCD_Out(2, 1, "Eau: ");
 Lcd_Chr(2, 6, (percentage / 100) % 10 + 48);
 Lcd_Chr(2, 7, (percentage / 10) % 10 + 48);
 Lcd_Chr(2, 8, percentage % 10 + 48);
 Lcd_Out(2, 9, "%");
 LCD_Cmd(_LCD_CURSOR_OFF);
 }
 }
 return 0;
}
