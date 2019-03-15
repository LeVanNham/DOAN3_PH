
#include <util/delay.h>

#define PORT_LCD PORTB
#define DDR_LCD DDRB

#define PORT_CTRL_LCD PORTB
#define DDR_CTRL_LCD DDRB

#define sbi(port,bit) port|=_BV(bit)
#define cbi(port,bit) port&=~_BV(bit)

#define RS PB4
#define RW PB5
#define EN PC1


#define D4 PB0
#define D5 PB1
#define D6 PB2
#define D7 PB3

#define time_ms 15
#define time_us 400

#define RS_HIGH sbi(PORT_CTRL_LCD,RS)
#define RW_HIGH sbi(PORT_CTRL_LCD,RW)
#define EN_HIGH sbi(PORTC,EN)

#define RS_LOW cbi(PORT_CTRL_LCD,RS)
#define RW_LOW cbi(PORT_CTRL_LCD,RW)
#define EN_LOW cbi(PORTC,EN)

#define lcd_wait1 _delay_ms(15)
#define lcd_wait2 _delay_us(400)

#define lcd_out_4bit(data) {PORT_LCD &=0xf0;PORT_LCD|=(data&0x0f);EN_HIGH;lcd_wait1;EN_LOW;lcd_wait2;}

#define lcd_send(data) {lcd_out_4bit(data>>4);lcd_out_4bit(data);}
//#define lcd_send(data) { PORT_LCD=data; EN_HIGH; _delay_ms(time_ms); EN_LOW;}
#define lcd_putChar(chr) { RS_HIGH,RW_LOW; lcd_send(chr); _delay_us(time_us);}
#define lcd_cmd(cmd) { RS_LOW; RW_LOW; lcd_send(cmd); _delay_us(time_us);}
#define lcd_move(y,x) lcd_cmd(0x80 + 0x40*(y-1) + x-1)
#define lcd_clear lcd_cmd(0x01);




void lcd_putString(char *s){
	while(*s){
		lcd_putChar(*s);
		s++;
	}
}

void lcd_print_number(double n){
   uint16_t a,b,i,x;
   char num[10];
   a=n;
   b=(n-a)*1000+1000;
   i=9;
   
   while(b){
      x=b%10;
      b=b/10;
      num[i]=x+48;
      i--;
   }
   i++;
   num[i]='.';
   i--;
   if(a==0) {num[i]='0'; i--;}
   else while (a){
      x=a%10;
      a=a/10;
      num[i]=x+48;
      i--;
   }
   
   i++;
   while(i<10){
      //++i;
      lcd_putChar(num[i]);
      i++;
      }
   
}

void lcd_init(){
       DDR_LCD|=0x3F;
       DDRC|=0x02;
   
	RS_LOW;
	RW_LOW;
	_delay_us(time_us);

	lcd_send(0x03);
	_delay_us(time_us);
	
	lcd_send(0x02);
	_delay_us(time_us);

	lcd_send(0x28);
	_delay_us(time_us);

	lcd_send(0x01);
	_delay_us(time_us);

	lcd_send(0x0f);
	_delay_us(time_us);

	lcd_send(0x06);
	_delay_us(time_us);

	lcd_send(0x0c);
	_delay_us(time_us);
}