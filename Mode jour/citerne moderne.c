#include <stdbool.h>
#define Set_Low(pin){pin=0;}
#define Set_High(pin){pin=1;}
#define valider Portb.RB1
#define Niveau_A Portb.RB4
#define Niveau_B Portb.RB5
#define Niveau_C Portb.RB6
#define Led_R Portc.RC0
#define Led_B Portc.RC1
#define Led_O Portc.RC2
#define BUZZER Portc.RC3
#define Filtration Portc.RC4
#define Electrovanne Portc.RC5
#define Mode_jour portb.RB0
#define Mode_nuit portd.RD7
#define Qualite Porta.RA0
#define RAZ portb.RB7
// LCD module connections
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
// End LCD module connections
int timer_counter;
bool timer_flag=false;
unsigned int adc_value;
float voltage;
unsigned int percentage;
unsigned int remplissage;
unsigned int remp=0;
unsigned int indice = 0;
unsigned int clean=0;
void init_timer() {
    OPTION_REG.T0CS = 0;
    OPTION_REG.PSA = 0;
    OPTION_REG.PS0 = 1;
    OPTION_REG.PS1 = 1;
    OPTION_REG.PS2 = 1;
    TMR0 = 65216;
    INTCON.TMR0IE = 1;
}
void interrupt(){
      int i=0;
      if(Mode_jour==1){
        if(Niveau_A==1){
          Set_Low(Electrovanne);
          Set_Low(Led_B);
          remplissage++;
          indice = 1;
        }
        if(Niveau_B==1){
          Set_High(BUZZER);
          Delay_ms(100);
          Set_Low(BUZZER);
          for(i=0;i<3;i++){
              Set_High(Led_R);
              Delay_ms(1000);
              Set_Low(Led_R);
              Delay_ms(1000);
          }
          Delay_ms(2000);
        }
        if(INTCON.EEIF){
          indice = 1;
          INTCON.EEIF = 0;
        }
        //timrt
        if (INTCON.TMR0IF) {
          timer_counter++;
          if (timer_counter >= 4) {
            timer_flag = true;
            timer_counter = 0;
          }
          TMR0 = 65216;
          INTCON.TMR0IF = 0;
        }
        //
        INTCON.RBIF = 0;
      }
}
int main(){
    int i=0;
    int repos=1;
    /////seuil :
    INTCON.GIE = 0;
    EEPROM_Write(0x20, 75);
    indice = 0;
    INTCON.GIE = 1;
    //////
    //timer
    INTCON.T0IE=1;
    OPTION_REG.T0CS=0;//set the timer (0:timer/1:compteur)
    //
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
    Set_Low(Led_R);
    Set_Low(Led_B);
    Set_Low(Electrovanne);
    LCD_Init();
    init_timer();
    Set_Low(Led_O);
    remp=0;
    while(1){
      adc_value = ADC_Read(0);
      voltage = adc_value * 5.0 / 1023.0;
      percentage = voltage / 5.0 * 100;
      if(valider==1 || Niveau_A==1 || Niveau_B==1 || Niveau_C==1 || Mode_jour==1 || Mode_nuit==1){
        repos=0;
      }
      else{
           repos=1;
      }
      if (Mode_jour==1){
            Lcd_Out(1,1,"Mode jour");
            Lcd_Cmd(_LCD_CURSOR_OFF);
            if(valider==1){
              Set_High(Electrovanne);
              Set_High(Led_B);
            }
            if (indice == 1){
              INTCON.GIE = 0;
              EEPROM_Write(0x50, remplissage);
              indice = 0;
              INTCON.GIE = 1;
            }
            if (Niveau_A == 1){
              Lcd_Chr(2, 1, ' ');
              Lcd_Chr(2, 2, (percentage / 100) % 10 + 48);
              Lcd_Chr(2, 3, (percentage / 10) % 10 + 48);
              Lcd_Chr(2, 4, percentage % 10 + 48);
              Lcd_Out(2, 5, "%");
              Delay_ms(500);
              if(percentage>EEPROM_Read(0x20)){
                int j;
                Lcd_Out(2,6,"Eau Sale!! ");
                Set_High(Filtration);
                for(j=0;j<5;j++){
                Set_High(Led_O);
                Delay_ms(1000);
                Set_Low(Led_O);
                Delay_ms(1000);
                }
                Set_Low(Filtration);
              }
              if(percentage<EEPROM_Read(0x20)){
                Lcd_Out(2,6,"Eau Propre ");
                Lcd_Cmd(_LCD_CURSOR_OFF);
              }
            }
            if (Niveau_B == 1){
              Lcd_Out(2,1," Remplir citerne");
              Lcd_Cmd(_LCD_CURSOR_OFF);
            }
            if(EEPROM_Read(0x50)>5)
            {
              Lcd_Out(2,1,"   Gaspillage");
              Lcd_Cmd(_LCD_CURSOR_OFF);
              Set_High(BUZZER);
              if(RAZ=1)
             {
                Set_Low(BUZZER);
                Set_Low(Led_R);
                Set_Low(Led_B);
                Set_Low(remplissage);
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