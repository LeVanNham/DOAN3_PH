#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>


#include "lcd_4bit.h"


#define offset 0.00
//adc
#define INT_MODE (1<<REFS1)|(1<<REFS0)|(0<<ADLAR)
#define ADC_VREF_TYPE INT_MODE
//

//timmer


uint16_t ADC_val;
float ph;

int read_adc(unsigned char adc_ch){
   ADMUX=adc_ch|ADC_VREF_TYPE;
   ADCSRA|=(1<<ADSC);
   loop_until_bit_is_set(ADCSRA,ADIF);
   return ADCW;
}

float ph_val(){
   int i=0,j=0;
   int buf[10],temp;
   float avg_val,ph;
   //unt16_t temp=0;
   for(i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
   { 
      buf[i]=read_adc(0);
      _delay_ms(10);
   }
   for(i=0;i<9;i++)        //sort the analog from small to large
   {
      for(j=i+1;j<10;j++)
      {
	 if(buf[i]>buf[j])
	 {
	    temp=buf[i];
	    buf[i]=buf[j];
	    buf[j]=temp;
	 }
      }
   }
   
   avg_val=0; 
   i=2;
   while(i<10){
      avg_val+= buf[i];
      i++;
   }
   ph=3.5*1000*(float)avg_val/1024/6;
   return ph;
}
int main()
 { 
 
    DDRD&=0xF3;
    MCUCR=(1<<ISC00)|(1<<ISC01)|(1<<ISC10)|(1<<ISC11);//ngat canh xuong INT0 va INT1
    GICR=(1<<INT0)|(1<<INT1);		//cho phep ngat INT0 va INT1
   
 
     sei();
    
    ADCSRA=(1<<ADEN)|(1<<ADPS2)|(ADPS0);//chuyen doi adc
    ADMUX=ADC_VREF_TYPE;
    
    DDRB|=0x80;
    int b=1425;
    float a;
    a=1000*(float)b*2.56/1024;

    char z[]="PROJECT 3: Do PH";
    char ph_title[]="PH: ";
    
    lcd_init();
    //lcd_putString(z);
    lcd_print_number(a);
    lcd_move(2,1); lcd_putString(ph_title);
   while (1)
      ;
   return 0;
 }
 
 ISR(INT0_vect){
    ph=ph_val();
    lcd_move(2,5);
    lcd_print_number(ph);
 }
 
 ISR(INT1_vect){
    float a=read_adc(2);
 }
 
