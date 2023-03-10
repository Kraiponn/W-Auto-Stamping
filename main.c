/******************************************************************************\
|  Programming   : Auto Check And Stamping in AFE-3032 V1.2.2                  |
|  Describetion  : Chech Coils & Screw, At last Stamping Shutter               |
|  Complier      : CCS C V4.049.                                               |
|  MCU           : PIC18F8720 X-TAL 10 MHz.                                    |
|  CirCuit Board : COPAL-PLC                                                   |
|  Date          : 28/08/2012.                                                 |
/******************************************************************************\ 
|>>>>>>>>>>>>>   Command for Request data from function tester    <<<<<<<<<<<<<<
|  Format Coils Resistance Data -> [R1xx.xx,OK,R2xx.xx,OK]
|  COIL SPECIFICATION :-----------
   | Coil1-2       : 102(+-10) Ohm
   | Coil Rotor(3) : 13(+-1.3) Ohm
   |------------------------------
|  
|  
|   
|   
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/  

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stamp_lib.c>
#include <Gb_Spec.c>

//############################################################################\\
//####################      Define Parameter       ###########################\\      
   char Rcv;
   int8 Cnt_TimeOut;  
   
/*
*************************************************
**         Function in use
*************************************************
*/

void Rx1_Isr(void);
void Rx2_Isr(void);
void Reset_Isr(void);
void Tim0_Isr(void);

void Initial_Port(void);
void ConfigTimer0(void);
void Initial_Robot(void);
void ProcessMode(void);


//------------------------------------------------------------------------------
//-------------------   CONFIG TIMER0
void ConfigTimer0(void)
{   
   setup_timer_0(RTCC_INTERNAL|RTCC_8_BIT|RTCC_DIV_256);
   set_timer0(0);
}

//------------------------------------------------------------------------------
//-------------------   INITIAL CYLINDER
void Initial_Robot(void)
{
   Rob.Err_Coil   = 3;
   Rob.ResCoils   = FALSE;
   Rob.ResScrew   = FALSE;
   Rob.ModeI      = 0;
   Rob.Reset_Flag = FALSE;
   Rob.Get_Qr     = FALSE;
   
   Rob.CompleteSpec = FALSE;
   Rcv            = '\0';
   
   Initial_StageRobot();      
   Clear_Led();         
      
   if(Rob.Display_Spec == TRUE)
      {            
         lcd_gotoxy(1,1); printf(lcd_putc,"  <<  AFE-3196  >>  ");
         lcd_gotoxy(1,2); printf(lcd_putc,"   FS120829-2.1.2   ");
         delay_ms(2000);
         
         Check_InterFace();
         printf("MACHINE START\r");
         delay_ms(2000);
         Rob.Display_Spec = FALSE;
      }
      
   Diplay_Ready();
   Start_Buzzer();
   
}


//******************************************************************************
//****************  CLEAR STAGE OF ROBOT TO INITIAL
void Initial_Port(void)
{
   Cnt_TimeOut = 0;
   output_high(SV_COILS);  output_high(SV_STAMP); 
   output_high(SV_LOCK);   output_high(SV_QR);   output_high(SV_SCREW);
   output_high(SV_SLIDER); delay_ms(200);
   output_high(BUZZER);    output_high(LEDG);    output_high(LEDR);
   
   ConfigTimer0();
   setup_adc_ports(AN0_TO_AN2|VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   output_low(RELAY);
   
   lcd_init();
   delay_ms(500);
   Initial_SenSor();   
}


//------------------------------------------------------------------------------
//-------------------  
void ProcessMode(void)
{   
   if(!input(SW_START))       //-- AUTO FOR ALL PROCESS (CHECK COILS,SCREW AND QR CODE).
    {
      delay_ms(200);
      while(!input(SW_START));
         Clear_LCD();
         Clear_Led();
         if( (!input(SS_COIL_UP)) && (input(SS_SCREW_UP)) && (!input(SS_QR_UP)) && 
           (!input(SS_UNLOCK)) && (!input(SS_STAMP_UP)) && (!input(SS_L)) )
          {
               sw = GetStatus_SS(SW_ID);
               delay_ms(200);
               if(sw == 0xFF)          //  --ALL
                  {
                     delay_ms(200);
                     Rob.ModeI = 2;
                  }
               
               else if(sw == 0xFB)    //  --Chk&Stm
                  {
                     delay_ms(200);
                     Rob.ModeI = 3;
                  }
                  
               else if(sw == 0xF9)    //   --Stamp Only
                   {
                     delay_ms(200);
                     Rob.ModeI = 4;
                  }
                  
                  
               switch(Rob.ModeI)
               {
                  case 1:
                          Rob.ModeI = 0;
                          Config_Type();
                          //printf(" Config Type \n");
                  break;
                  
                  case 2:
                          Rob.ModeI = 0;
                          lcd_gotoxy(1,2); printf(lcd_putc,"Check...           ");
                          OperateAll();
                          //printf(" Check All Process \n");
                  break;
                           
                  case 3:
                          Rob.ModeI = 0;
                          lcd_gotoxy(1,2); printf(lcd_putc,"Check...           ");
                          Check_R_Stamp();   
                          //printf(" Check & Stamping But No QR \n");
                  break;
                           
                  case 4:
                          Rob.ModeI = 0;
                          lcd_gotoxy(1,2); printf(lcd_putc,"Check...           ");
                          StampOnly();  
                         // printf(" Stamping Only \n");
                  break; 
               }                  
          }
       
        else
         {
             Display_CylinderError();
             Buzzer_Alarm(10);
         }  
        //Clear_LCD(); 
       // Resistance3(); 
         
         
    }
    
  else if(Rob.ModeI == 1)
   {
         Rob.ModeI = 0;
         Config_Type();
   }
   
   
 /* else if(Rob.Ack_Qr)
    {
         Rob.Ack_Qr = FALSE;
         printf("%s",QR_Packet);
      
         lcd_gotoxy(1,2); printf(lcd_putc,"QR Code :           ");
         lcd_gotoxy(1,3); printf(lcd_putc,"%s               ",Qr_Show);
    }
         
  else if(Rob.Get_Qr)
    {
         Rob.Get_Qr = FALSE;
         LED_GREEN(); delay_ms(10);
         //Clear_Led();
         lcd_gotoxy(1,4); printf(lcd_putc,"Data is OK...       ");
    } */ 
    
}


//**  SETUP INTERRUPT PRIORITY LEVEL
#PRIORITY RDA,RDA2,TIMER0,EXT

//############################################################################\\
//-----------  INTERRUPT ON RECIEVER1 CHANNEL (RX1)
#INT_RDA
void Rx1_Isr(void)
{        
   static enum _Sequence 
      {
         first,
         second
      } Sequence = first;
      
   Rcv = getc1USART();
   
   switch(Sequence)
      {
         case first:
                     //output_toggle(PIN_H0);
                     if((Rcv == 'O') || (Rcv == 'N') || (Rcv == 'R'))
                           Sequence = second;
                        
                     else
                        {
                           Rob.Get_Qr  = FALSE;
                           Sequence    = first;
                           Rcv         = '\0';
                        }      
         break;
         
         case second:
                     //output_toggle(PIN_H0);
                     switch(Rcv)
                        {
                           case 'K': 
                                       output_low(LEDG);
                                       Rob.Get_Qr  = TRUE;
                                       Sequence    = first;
                                       Rcv         = '\0';
                           break;
                           
                           case 'G':
                                       Rob.Get_Qr  = FALSE;
                                       Sequence    = first;
                                       Rcv         = '\0'; 
                           break;
                           
                           default:    Rob.Get_Qr  = FALSE;
                                       Sequence    = first;
                                       Rcv         = '\0'; 
                           break;
                        }
         break;

      }
    //output_toggle(PIN_H0); 
}


//############################################################################\\
//-----------  INTERRUPT ON RECIEVER2 CHANNEL (RX2)
#INT_RDA2
void Rx2_Isr(void)
{        
   static int cnt_add = 0;
   char Rcv2 = '\0';
      
   Rcv2 = getc2USART();
   //output_toggle(LEDG); 

   if(Rcv2 != '\r')
      {
         QR_Packet[cnt_add] = Rcv2;
         Qr_Show[cnt_add]   = QR_Packet[cnt_add];
         cnt_add++;
         Rob.Ack_Qr         = FALSE;
      }
      
   else
      {         
         QR_Packet[cnt_add] = '\r';
         cnt_add    = 0;
         Rob.Ack_Qr = TRUE;         
      }
   
}



//############################################################################\\
//-----------  INTERRUPT ON TIMER0
#INT_TIMER0
void Tim0_Isr(void)
{
   Cnt_TimeOut++;
   if(Cnt_TimeOut >= 83)
      {
         Cnt_TimeOut = 0;
         output_toggle(LED_START);
      }
}

//############################################################################\\
//-----------  INTERRUPT ON EXTERNAL
#INT_EXT
void Reset_Isr(void) 
{
   delay_us(20);
   while(!input(PIN_B0));
   Rob.Reset_Flag = TRUE;
}


//############################################################################\\
//        _________________      MAIN FUNCTION       __________________       \\
//############################################################################\\
void main(void)
{                                       
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_RDA);
   enable_interrupts(INT_RDA2);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_EXT);
   EXT_INT_EDGE(H_TO_L);
   
   set_tris_a(0b11100111);   
   set_tris_b(0b00111111);  
   set_tris_c(0b10000110);
   set_tris_d(0b00000000);   
   set_tris_e(0b10000000);   
   set_tris_f(0b11111111);
   set_tris_g(0b00011101);
   set_tris_h(0b00000000);
   set_tris_j(0b00000000);
   
   Initial_Port(); 

   Init_Rob:
      Initial_Robot();             
 
   while(TRUE)             
   {
     //SV_STAMP
     _StatusIO_();  //output_toggle(RELAY); delay_ms(700);
      ProcessMode();

      if(Rob.Reset_Flag == TRUE)
         {
            delay_ms(500);
            Rob.Reset_Flag = FALSE;
            putc1USART("RESET\r");
            GOTO Init_Rob;
         }
      //output_d(0x00); delay_ms(500);
      //output_d(0xff); delay_ms(500);
      /*else if(!input(SS_SCREW4))
      {
         delay_ms(500);
         output_toggle(LEDG);
      }*/
   } 
}
//                         ^-^     ^-^     ^-^
//####################       < End Program >         #########################//
