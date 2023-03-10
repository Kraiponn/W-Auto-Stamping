
//#include <stamp_lib.c>

void Buzzer_Alarm(byte tot);
void Buzzer_LongBeep(void);
void Start_Buzzer(void);
void Clear_Status(void);
void Buzzer_Beep(void);
void Show_alarm(void);
void Lcd_Status(void);
void _StatusIO_(void);

void LED_RED(void);
void LED_GREEN(void);



//------------------------------------------------------------------------------
//-------------------   BUZZER ALARM FOR RESET OR INITIAL MACHINE STATUS
void Start_Buzzer(void)
{
   byte cnt;
      for(cnt = 0; cnt < 2; cnt++)
         {
            output_high(BUZZER); delay_ms(120);
            output_low(BUZZER);  delay_ms(30);
         }
}

//------------------------------------------------------------------------------
//-------------------   SHOW STATUS FROM CLICK THE BUTTON
void Buzzer_Beep(void)
{
   output_high(BUZZER); delay_ms(100);
   output_low(BUZZER);  delay_ms(25);
}

//------------------------------------------------------------------------------
//-------------------   ALARM FOR LONG VOLUME
void Buzzer_LongBeep(void)
{
   output_high(BUZZER); delay_ms(1000);
   output_low(BUZZER);  delay_ms(125);
}

//------------------------------------------------------------------------------
//-------------------   BUZZER ALARM 
void Buzzer_Alarm(byte tot) 
{    
    output_high(BUZZER); delay_ms(1500);
    output_low(BUZZER);  delay_ms(70);
      for(; tot > 0 ; tot--)
         {
            output_high(BUZZER); delay_ms(90);
            output_low(BUZZER);  delay_ms(20);
         }
}

//------------------------------------------------------------------------------
//-------------------   Display Error
void Show_alarm(void)
{    
    lcd_gotoxy(1,1);
    printf(lcd_putc," Mistake...     ");
    
    lcd_gotoxy(1,2);
    printf(lcd_putc," CYLINDER ERROR ");
    
}

//------------------------------------------------------------------------------
//-------------------   Initial Status of LED
void Clear_Status(void)
{
   output_low(LEDR); 
   output_low(LEDG); 
   delay_ms(10);
}

//------------------------------------------------------------------------------
//-------------------   Initial Status of LCD
void Lcd_Status(void)
{
   lcd_gotoxy(1,1); printf(lcd_putc,"   _MACHINE_    ");
   lcd_gotoxy(1,2); printf(lcd_putc,"Ready...        ");
}

//------------------------------------------------------------------------------
//-------------------   STATUS OK
void LED_GREEN(void)
{
   output_low(LEDR); delay_ms(100);
   output_high(LEDG); delay_ms(100);
}

//------------------------------------------------------------------------------
//-------------------   STATUS NG
void LED_RED(void)
{
   output_high(LEDR); delay_ms(100);
   output_low(LEDG);  delay_ms(100);
}


//------------------------------------------------------------------------------
//-------------------   I_O STATUS
void _StatusIO_(void)
{
   sw = PCF8574A_Read();
   //lcd_gotoxy(1,2); printf(lcd_putc,"SW %02X",sw);
   if(sw == 0xF9)
      {
         delay_ms(150);
         while( sw == 0xF9) { sw = PCF8574A_Read(); }
         
         lcd_gotoxy(1,1); printf(lcd_putc,"Check Spec...   ");
         lcd_gotoxy(1,2); printf(lcd_putc,"                ");
         delay_ms(200);
         Buzzer_LongBeep();
         Rob.Spec_Flag = TRUE;
         Rob.Spec_Ver  = TRUE;
         Rob.Res_Flag  = TRUE;
      }
   
   else if( sw == 0xD9 )        //#######################    TEST PIN MOVE
      {
          delay_ms(150);
          while( sw == 0xD9) { sw = PCF8574A_Read(); }
          if( (!input(STAMP_UP)) && (!input(PIN_UP)) && (!input(SLIDER_ORIGIN)) )
            {
               //MOVE_PIN(); delay_ms(150);
            }
            
          else if( (!input(STAMP_UP)) && (!input(PIN_DOWN)) && (!input(SLIDER_ORIGIN)) )
            {
               if(ORIGIN_PIN() == TRUE) {}
               //ORIGIN_PIN(); delay_ms(150);
            }
          
      }
      
      
   else if( sw == 0xCD )      //#######################    TEST SLIDER MOVE
      {
          delay_ms(150);
          while( sw == 0xCD) { sw = PCF8574A_Read(); }
          
         
      }
      
  else if( sw == 0xD5 )      //#######################    TEST STAMP MOVE
      {
          delay_ms(150);
          while( sw == 0xD5) { sw = PCF8574A_Read(); }
                         
               
      } 
      
   else if( sw == 0xDC )      //#######################   STAMP ONLY
      {
          delay_ms(150);
          while( sw == 0xDC) { sw = PCF8574A_Read(); }

            
      }
      
   else if( sw == 0xDE )      //#######################    AUTO PROCESS BUT NOT QR CODE
      {
          delay_ms(150);
          while( sw == 0xDE) { sw = PCF8574A_Read(); }

      } 
      
   else if( sw == 0x9E )   //---   AUTO PROCESS FOR CHECK ALL
      {
          delay_ms(150);
          while( sw == 0x9E) { sw = PCF8574A_Read(); }
          

      }
      
}



