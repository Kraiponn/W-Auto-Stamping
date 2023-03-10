 
//-----------------------  DEFINE VARIABLE
 byte blink;

//******************************************************************************
// -------------------------    FUCTION IN USE FOR SETTING SPEC

void Config_Type(void);
void Check_InterFace(void);

boolean Config_BaudRate(void);
void JudgeMent_BaudRate(void);
void Save_BaudRate(byte BR);
void Read_BaudRate(void);

boolean Setting_SpecCoils(void);
void Setting_Coils(void);
void Save_SpecCoils(void);
void Read_SpecCoils(void);

void Setting_Screw(void);
boolean Setting_SpecScrew(void);
void Save_TypeScrew(byte TypeChk);
void Read_TypeScrew(void);

//------------------------------------------------------------------------------
//-------------------   JUDGEMENT BAUD RATE VALUE FOR INTERFACE BOXC WITH PC
//------------------------------------------------------------------------------



//-------------------   CHOOSE TYPE CONFIG < BAUDE RATE,SCREW AND COILS RESISTANCE >
void Config_Type(void)
{
   static byte mode = 1;
   
   sw = 0xFF;
   lcd_gotoxy(1,1); printf(lcd_putc," Please select type ");
   Buzzer_LongBeep();
   Start_Buzzer();
   for(blink = 0; blink < 3; blink++)
      {
         lcd_gotoxy(1,1); printf(lcd_putc,"                    "); delay_ms(300);
         lcd_gotoxy(1,1); printf(lcd_putc," Please select type "); delay_ms(300);
      }
      
   lcd_gotoxy(1,2); printf(lcd_putc,">> 1.Check Interface");
   lcd_gotoxy(1,3); printf(lcd_putc,"   2.Setting Screw  ");
   lcd_gotoxy(1,4); printf(lcd_putc,"   3.Setting Coils  ");
   delay_ms(1100);
   
   while(sw != 0x7F)          //-- Wait until press button enter (Blue switch)
   {
      sw = GetStatus_SS(SW_ID);
      if(sw == 0xF7)       //--------------- Choose Mode Interface
         {
            delay_ms(100);
            while(sw == 0xF7) { sw = GetStatus_SS(SW_ID); }
            mode = 1;
            lcd_gotoxy(1,2); printf(lcd_putc,">> 1.Check Interface");
            lcd_gotoxy(1,3); printf(lcd_putc,"   2.Setting Screw  ");
            lcd_gotoxy(1,4); printf(lcd_putc,"   3.Setting Coils  "); delay_ms(10);
            Buzzer_Beep();
         }
         
      else if(sw == 0xEF)       //--------------- Choose Mode Screw check
         {
            delay_ms(100);
            while(sw == 0xEF) { sw = GetStatus_SS(SW_ID); }
            mode = 2;
            lcd_gotoxy(1,2); printf(lcd_putc,"   1.Check Interface");
            lcd_gotoxy(1,3); printf(lcd_putc,">> 2.Setting Screw  ");
            lcd_gotoxy(1,4); printf(lcd_putc,"   3.Setting Coils  "); delay_ms(10);
            Buzzer_Beep();
         }   
         
      else if(sw == 0xDF)       //--------------- Choose Mode Coils Spec
         {
            delay_ms(100);
            while(sw == 0xDF) { sw = GetStatus_SS(SW_ID); }
            mode = 3;
            lcd_gotoxy(1,2); printf(lcd_putc,"   1.Check Interface");
            lcd_gotoxy(1,3); printf(lcd_putc,"   2.Setting Screw  ");
            lcd_gotoxy(1,4); printf(lcd_putc,">> 3.Setting Coils  "); delay_ms(10);
            Buzzer_Beep();
         }  
  
   }
   
   sw = 0xFF;
   delay_ms(350); 
   Start_Buzzer();
   switch(mode)
   {           
      case 1:         
               Check_InterFace(); delay_ms(150);
               sw = GetStatus_SS(SW_ID);
               while( sw != 0x7F ) { sw = GetStatus_SS(SW_ID); }
      break;
      
      case 2:  Setting_Screw();    
      break;
      
      case 3:  Setting_Coils();    
      break;
      
      default: 
               Check_InterFace(); delay_ms(150);
               sw = GetStatus_SS(SW_ID);
               while( sw != 0x7F ) { sw = GetStatus_SS(SW_ID); }
      break;
   }
   
}



//------------------------------------------------------------------------------
//-------------------   
void Check_InterFace(void)
{
    Clear_LCD(); delay_ms(200);
    lcd_gotoxy(1,1); printf(lcd_putc,"  Screw & BaudRate  ");
    Read_TypeScrew();
    lcd_gotoxy(1,3); printf(lcd_putc,"BaudRate: 19200 B/s ");
    
    if(Rob.Display_Spec == FALSE)
    {
       sw = GetStatus_SS(SW_ID);
       while( sw != 0x7F ) { sw = GetStatus_SS(SW_ID); }
    }
    
    lcd_gotoxy(1,1); printf(lcd_putc,"Spec Coils 1-3...   ");
    lcd_gotoxy(1,2); printf(lcd_putc,"                    ");
    lcd_gotoxy(1,3); printf(lcd_putc,"                    ");
    Read_SpecCoils();    
}



//------------------------------------------------------------------------------
//-------------------   SETTING SPEC OF SCREW
void Setting_Screw(void)
{
      Rob.Reset_Flag   = FALSE;
      Rob.CompleteSpec = FALSE;

      lcd_gotoxy(1,1); printf(lcd_putc,"Choose mode check   ");
      for(blink = 0; blink < 3; blink++)
         {
            lcd_gotoxy(1,1); printf(lcd_putc,"                    "); delay_ms(350);
            lcd_gotoxy(1,1); printf(lcd_putc,"Choose mode check   "); delay_ms(350);
         }
         
            lcd_gotoxy(1,2); printf(lcd_putc,">> Check all screw  ");
            delay_ms(50);
            
            lcd_gotoxy(1,3); printf(lcd_putc,"   Don't care all   ");
            lcd_gotoxy(1,4); printf(lcd_putc,"                    ");
            delay_ms(1300);
            while(Rob.CompleteSpec == FALSE)  //-- Wait For JudgeMent Spec To Complete
               {
                  Rob.CompleteSpec = Setting_SpecScrew(); 
               }    
               
      delay_ms(1200);
      Rob.Reset_Flag   = TRUE;
      Rob.CompleteSpec = FALSE; 
}

//------------------------------------------------------------------------------
//-------------------   SETTING TYPE FOR CHECKING SCREW
boolean Setting_SpecScrew(void)
{
  boolean ret = FALSE;
  
      sw = GetStatus_SS(SW_ID);
      if(sw == 0xF7)                      //--------------- Choose Mode Check
         {
            delay_ms(100);
            while(sw == 0xF7) { sw = GetStatus_SS(SW_ID); } 
            SPEC.TypeScr = 1;
            ret = FALSE;
            lcd_gotoxy(1,2); printf(lcd_putc,">> Check all screw  ");
            lcd_gotoxy(1,3); printf(lcd_putc,"   Don't care all   ");
            lcd_gotoxy(1,4); printf(lcd_putc,"                    "); 
            Buzzer_Beep();
         }
         
      else if(sw == 0xEF)                //---------------  Choose Mode Interface
         {
            delay_ms(100);
            while(sw == 0xEF) { sw = GetStatus_SS(SW_ID); } 
            SPEC.TypeScr = 2;
            ret = FALSE;
            lcd_gotoxy(1,2); printf(lcd_putc,"   Check all screw  ");
            lcd_gotoxy(1,3); printf(lcd_putc,">> Don't care all   ");
            lcd_gotoxy(1,4); printf(lcd_putc,"                    "); 
            Buzzer_Beep();
         } 
         
         
     else if(sw == 0x7F) //---------------  Choose Mode Interface
         {
            delay_ms(100);
            while(sw == 0x7F) { sw = GetStatus_SS(SW_ID); }
            ret = TRUE;
            Buzzer_Config();
            Save_TypeScrew(SPEC.TypeScr);
         }
         
  return(ret);
}

//------------------------------------------------------------------------------
//-------------------   SAVE TYPE FOR CHECK SCREW TO EEPROM
void Save_TypeScrew(byte TypeChk)
{
   lcd_gotoxy(1,1); printf(lcd_putc,"Save type check...  ");
   lcd_gotoxy(1,3); printf(lcd_putc,"                    ");
   lcd_gotoxy(1,4); printf(lcd_putc,"                    ");
   delay_ms(100);
   
   switch(TypeChk)
   {
      case 1:  
               write_eeprom(70,1);   delay_ms(1);
               lcd_gotoxy(1,2); printf(lcd_putc,"Use mode check all  ");
      break;
      
      case 2:  
               write_eeprom(70,2);   delay_ms(1);
               lcd_gotoxy(1,2); printf(lcd_putc,"Use mode Don't Care ");
      break;
      
      default: 
               write_eeprom(70,1);   delay_ms(1);
               lcd_gotoxy(1,2); printf(lcd_putc,"Use mode check all  ");
      break;
   }
   
   delay_ms(2000);
      
}

//------------------------------------------------------------------------------
//-------------------   READ TYPE OF MODE CHECK SCREW FROM EEPROM
void Read_TypeScrew(void)
{   
   SPEC.TypeScr = read_eeprom(70);
   delay_ms(250);
   
     switch(SPEC.TypeScr)
            {
               case 1: //----------- READ MODE CHECK ALL SCREW
                       lcd_gotoxy(1,2); printf(lcd_putc,"Check all screw     ");
               break;
               
               case 2: //----------- READ MODE DON'T CARE FOR CHECK ALL SCREW
                       lcd_gotoxy(1,2); printf(lcd_putc,"Don't care all screw");
               break;
               
               default:
                       lcd_gotoxy(1,2); printf(lcd_putc,"Check all screw     ");
               break;
            }
       
}




//##############################################################################------------(4)
//                     PART JUDGEMENT COILS RESISTANCE 
//##############################################################################

//------------------------------------------------------------------------------
//-------------------   JUDGEMENT SPEC OF COILS RESISTANCE
void Setting_Coils(void) 
{
      Rob.Reset_Flag   = FALSE;
      Rob.CompleteSpec = FALSE;

            lcd_gotoxy(1,1); printf(lcd_putc,"Config Spec Coils.. ");
            delay_ms(100);
            lcd_gotoxy(1,2); printf(lcd_putc,"C1H: %03ld  C1L: %03ld  ",SPEC.C1h,SPEC.C1l);
            lcd_gotoxy(1,3); printf(lcd_putc,"C2H: %03ld  C2L: %03ld  ",SPEC.C2h,SPEC.C2l);
            lcd_gotoxy(1,4); printf(lcd_putc,"C3H: %03ld  C3L: %03ld  ",SPEC.C3h,SPEC.C3l);
            delay_ms(1300);
            while(Rob.CompleteSpec == FALSE)        //-- Wait For JudgeMent Spec To Complete
               {
                  Rob.CompleteSpec = Setting_SpecCoils(); 
               }  
               
      delay_ms(1500);
      Rob.Reset_Flag   = TRUE;
      Rob.CompleteSpec = FALSE;              
}

//------------------------------------------------------------------------------
//-------------------   SETTING SPEC OF COIL RESISTANCE
boolean Setting_SpecCoils(void)
{
  boolean ret = FALSE;
   
      sw = GetStatus_SS(SW_ID);
//------------------   Judgement Value of The First Coil   ---------------------
      if(sw == 0xF7)       //--------------- Count Up High Spec of The First Coil
         {
            delay_ms(100);
            while(sw == 0xF7) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
                      
            if(SPEC.C1h >= 255) SPEC.C1h = 0;
            SPEC.C1h++;
            lcd_gotoxy(6,2); printf(lcd_putc,"%03ld",SPEC.C1h);
            ret = FALSE;
         }
         
      else if(sw == 0xEF)   //--------------- Count Down High Spec of The First Coil
         {
            delay_ms(100);
            while(sw == 0xEF) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
           
            if(SPEC.C1h < 1) SPEC.C1h = 256;
            SPEC.C1h--;
            lcd_gotoxy(6,2); printf(lcd_putc,"%03ld",SPEC.C1h);
            ret = FALSE;
         }
         
      else if(sw == 0xF3)   //--------------- Count Up Low Spec of The First Coil
         {
            delay_ms(100);
            while(sw == 0xF3) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
        
            if(SPEC.C1l >= 255) SPEC.C1l = 0;
            SPEC.C1l++;
            lcd_gotoxy(16,2); printf(lcd_putc,"%03ld",SPEC.C1l);
            ret = FALSE;
         }
         
      else if(sw == 0xEB)   //--------------- Count Down Low Spec of The First Coil
         {
            delay_ms(100);
            while(sw == 0xEB) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
          
            if(SPEC.C1l < 1) SPEC.C1l = 256;
            SPEC.C1l--;
            lcd_gotoxy(16,2); printf(lcd_putc,"%03ld",SPEC.C1l);
            ret = FALSE;
         }
         
//------------------   Judgement Value of The Second Coil   --------------------
      if(sw == 0xF5)       //--------------- Count Up High Spec of The Second Coil
         {
            delay_ms(100);
            while(sw == 0xF5) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
         
            if(SPEC.C2h >= 255) SPEC.C2h = 0;
            SPEC.C2h++;
            lcd_gotoxy(6,3); printf(lcd_putc,"%03ld",SPEC.C2h);
            ret = FALSE;
         }
         
      else if(sw == 0xED)   //--------------- Count Down High Spec of The Second Coil
         {
            delay_ms(100);
            while(sw == 0xED) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
          
            if(SPEC.C2h < 1) SPEC.C2h = 256;
            SPEC.C2h--;
            lcd_gotoxy(6,3); printf(lcd_putc,"%03ld",SPEC.C2h);
            ret = FALSE;
         }
         
      else if(sw == 0xF1)   //--------------- Count Up Low Spec of The Second Coil
         {
            delay_ms(100);
            while(sw == 0xF1) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
           
            if(SPEC.C2l >= 255) SPEC.C2l = 0;
            SPEC.C2l++;
            lcd_gotoxy(16,3); printf(lcd_putc,"%03ld",SPEC.C2l);
            ret = FALSE;
         }
         
      else if(sw == 0xE9)   //--------------- Count Down Low Spec of The Second Coil
         {
            delay_ms(100);
            while(sw == 0xE9) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
            
            if(SPEC.C2l < 1) SPEC.C2l = 256;
            SPEC.C2l--;
            lcd_gotoxy(16,3); printf(lcd_putc,"%03ld",SPEC.C2l);
            ret = FALSE;
         }
         
         
//------------------   Judgement Value of The Third Coil   --------------------
      if(sw == 0xF6)       //--------------- Count Up High Spec of The Second Coil
         {
            delay_ms(100);
            while(sw == 0xF6) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
         
            if(SPEC.C3h >= 255) SPEC.C3h = 0;
            SPEC.C3h++;
            lcd_gotoxy(6,4); printf(lcd_putc,"%03ld",SPEC.C3h);
            ret = FALSE;
         }
         
      else if(sw == 0xEE)   //--------------- Count Down High Spec of The Second Coil
         {
            delay_ms(100);
            while(sw == 0xEE) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
          
            if(SPEC.C3h < 1) SPEC.C3h = 256;
            SPEC.C3h--;
            lcd_gotoxy(6,4); printf(lcd_putc,"%03ld",SPEC.C3h);
            ret = FALSE;
         }
         
      else if(sw == 0xF2)   //--------------- Count Up Low Spec of The Second Coil
         {
            delay_ms(100);
            while(sw == 0xF2) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
           
            if(SPEC.C3l >= 255) SPEC.C3l = 0;
            SPEC.C3l++;
            lcd_gotoxy(16,4); printf(lcd_putc,"%03ld",SPEC.C3l);
            ret = FALSE;
         }
         
      else if(sw == 0xEA)   //--------------- Count Down Low Spec of The Second Coil
         {
            delay_ms(100);
            while(sw == 0xEA) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Beep();
            
            if(SPEC.C3l < 1) SPEC.C3l = 256;
            SPEC.C3l--;
            lcd_gotoxy(16,4); printf(lcd_putc,"%03ld",SPEC.C3l);
            ret = FALSE;
         }         
         
      else if(sw == 0x7F) //--------------- SAVE SPEC TO PIC'S EEPROM
         {
            delay_ms(100);
            while(sw == 0x7F) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Config();
            
            Save_SpecCoils();
            ret = TRUE;
         }

   return(ret);
}

//------------------------------------------------------------------------------
//-------------------   SAVE COILS RESISTANCE TO EEPROM
void Save_SpecCoils(void)
{
   byte x,y;
   lcd_gotoxy(1,1); printf(lcd_putc,"  << Save Spec >>   ");
   lcd_gotoxy(1,2); printf(lcd_putc,"Wait For Save       ");
   lcd_gotoxy(1,3); printf(lcd_putc,"                  ");
   lcd_gotoxy(1,4); printf(lcd_putc,"                  ");
   
   write_eeprom(10,SPEC.C1h); delay_ms(1);
   write_eeprom(20,SPEC.C1l); delay_ms(1);
   write_eeprom(30,SPEC.C2h); delay_ms(1);
   write_eeprom(40,SPEC.C2l); delay_ms(1);
   write_eeprom(50,SPEC.C3h); delay_ms(1);
   write_eeprom(60,SPEC.C3l); delay_ms(1);
   
   for(x = 0; x < 3; x++)
      {
         for(y = 14; y < 17; y++)
            {
               lcd_gotoxy(y,2); printf(lcd_putc,".");
               delay_ms(350);
            }
         lcd_gotoxy(14,2); printf(lcd_putc,"       ");
         delay_ms(350);
      }
   
   //Buzzer_Config();
   lcd_gotoxy(1,3); printf(lcd_putc,"Save is complete... ");
   lcd_gotoxy(1,4); printf(lcd_putc,"                    ");
   delay_ms(1300);
}


//------------------------------------------------------------------------------
//-------------------   READ SPEC OF COILS FROM EEPROM
void Read_SpecCoils(void)
{  
   SPEC.C1h = read_eeprom(10);
   SPEC.C1l = read_eeprom(20);
   SPEC.C2h = read_eeprom(30);
   SPEC.C2l = read_eeprom(40);
   SPEC.C3h = read_eeprom(50);
   SPEC.C3l = read_eeprom(60);
   
   lcd_gotoxy(1,2); printf(lcd_putc,"C1H: %03ld  C1L: %03ld  ",SPEC.C1h,SPEC.C1l);
   lcd_gotoxy(1,3); printf(lcd_putc,"C2H: %03ld  C2L: %03ld  ",SPEC.C2h,SPEC.C2l);
   lcd_gotoxy(1,4); printf(lcd_putc,"C3H: %03ld  C3L: %03ld  ",SPEC.C3h,SPEC.C3l);   
       
}





//------------------------------------------------------------------------------
//-------------------   JUDGEMENT BAUD RATE VALUE FOR INTERFACE BOXC WITH PC
void JudgeMent_BaudRate(void)
{
      Rob.Reset_Flag   = FALSE;
      Rob.CompleteSpec = FALSE;

      lcd_gotoxy(1,1);       printf(lcd_putc,"Pls choose baud rate");
      for(blink = 0; blink < 3; blink++)
         {
            lcd_gotoxy(1,1); printf(lcd_putc,"                    "); delay_ms(350);
            lcd_gotoxy(1,1); printf(lcd_putc,"Pls choose baud rate"); delay_ms(350);
         }
            
            lcd_gotoxy(1,2); printf(lcd_putc,">> 9600             ");
            lcd_gotoxy(1,3); printf(lcd_putc,"   19200            ");
            lcd_gotoxy(1,4); printf(lcd_putc,"   115200           "); delay_ms(10);
            delay_ms(1300);
            while(Rob.CompleteSpec == FALSE)     //-- Wait For JudgeMent Spec To Complete
               {
                  Rob.CompleteSpec = Config_BaudRate(); 
               }    
               
      delay_ms(1200);
      Rob.Reset_Flag   = TRUE;
      Rob.CompleteSpec = FALSE; 
}


//------------------------------------------------------------------------------
//-------------------   SETTING BAUD RATE FOR USE INTERFACE BOXC WITH PC
boolean Config_BaudRate(void)
{
  boolean ret    = FALSE;
      sw = GetStatus_SS(SW_ID);
      if(sw == 0xF7)       //--------------- Choose Mode Interface In 9600
         {
            delay_ms(100);
            while(sw == 0xF7) { sw = GetStatus_SS(SW_ID); }
            SPEC.BrSize = 1;
            lcd_gotoxy(1,2); printf(lcd_putc,">> 9600             ");
            lcd_gotoxy(1,3); printf(lcd_putc,"   19200            ");
            lcd_gotoxy(1,4); printf(lcd_putc,"   115200           "); delay_ms(10);
            Buzzer_Beep();
            ret = FALSE;
         }
         
     else if(sw == 0xFB)       //--------------- Choose Mode Interface In 19200
         {
            delay_ms(100);
            while(sw == 0xFB) { sw = GetStatus_SS(SW_ID); }
            SPEC.BrSize = 2;
            lcd_gotoxy(1,2); printf(lcd_putc,"   9600             ");
            lcd_gotoxy(1,3); printf(lcd_putc,">> 19200            ");
            lcd_gotoxy(1,4); printf(lcd_putc,"   115200           "); delay_ms(10);
            Buzzer_Beep();
            ret = FALSE;
         }  
         
     else if(sw == 0xFD)       //--------------- Choose Mode Interface In 115200
         {
            delay_ms(100);
            while(sw == 0xFD) { sw = GetStatus_SS(SW_ID); }
            SPEC.BrSize = 3;
            lcd_gotoxy(1,2); printf(lcd_putc,"   9600             ");
            lcd_gotoxy(1,3); printf(lcd_putc,"   19200            ");
            lcd_gotoxy(1,4); printf(lcd_putc,">> 115200           "); delay_ms(10);
            Buzzer_Beep();
            ret = FALSE;
         }
         
    else if(sw == 0xFE)
        {
            delay_ms(150);
            while(sw == 0xFE) { sw = GetStatus_SS(SW_ID); }
            Buzzer_Config();
            Save_BaudRate(SPEC.BrSize);
            ret = TRUE;
        }
        
   return(ret);     
}

//------------------------------------------------------------------------------
//-------------------   SAVE BAUD RATE TO EEPROM
void Save_BaudRate(byte BR)
{
   lcd_gotoxy(1,1); printf(lcd_putc,"Save baud rate...   ");
   switch(BR)
   {
      case 1:  
               write_eeprom(50,BR);   delay_ms(1);
               #use rs232(baud = 9600, xmit = TX1, rcv = RX1, BITS = 8, PARITY = N)
      break;
      
      case 2:  
               write_eeprom(50,BR);   delay_ms(1);
               #use rs232(baud = 19200, xmit = TX1, rcv = RX1, BITS = 8, PARITY = N)
      break;
      
      case 3:  
               write_eeprom(50,BR);   delay_ms(1);
               #use rs232(baud = 115200, xmit = TX1, rcv = RX1, BITS = 8, PARITY = N)
      break;
      
      default:
               #use rs232(baud = 19200, xmit = TX1, rcv = RX1, BITS = 8, PARITY = N)
   }
   
   //Start_Buzzer();
   delay_ms(2000);   
}

//------------------------------------------------------------------------------
//-------------------   READ BAUD RATE VALUE FROM EEPROM
void Read_BaudRate(void)
{   
   SPEC.BrSize = read_eeprom(50);
   delay_ms(250);
   
   switch(SPEC.BrSize)
   {
      case 1: //----------- READ BAUD RATE 9600
              #use rs232(baud = 9600, xmit = TX1, rcv = RX1, BITS = 8, PARITY = N)
              lcd_gotoxy(1,2); printf(lcd_putc,"BaudRate: 9600 B/s  ");
      break;
      
      case 2: //----------- READ BAUD RATE 19200
              #use rs232(baud = 19200, xmit = TX1, rcv = RX1, BITS = 8, PARITY = N)
              lcd_gotoxy(1,2); printf(lcd_putc,"BaudRate: 19200 B/s ");
      break;
      
      case 3: //----------- READ BAUD RATE 115200
              #use rs232(baud = 115200, xmit = TX1, rcv = RX1, BITS  = 8, PARITY = N) 
              lcd_gotoxy(1,2); printf(lcd_putc,"BaudRate: 115200 B/s");
      break; 
      
      default:
              #use rs232(baud = 19200, xmit = TX1, rcv = RX1, BITS  = 8, PARITY = N) 
              lcd_gotoxy(1,2); printf(lcd_putc,"BaudRate: 19200 B/s "); 
   }
       
}
