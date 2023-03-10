#include <18F8720.h>
#device adc=10

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES H4                       //High speed osc with HW enabled 4X PLL
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NOOSCSEN                 //Oscillator switching is disabled, main oscillator is source
#FUSES NOBROWNOUT               //No brownout reset
#FUSES BORV45                   //Brownout reset at 4.5V
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOCPD                    //No EE protection
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOWRTD                   //Data EEPROM not write protected
#FUSES NOWAIT                   //Wait selections unavailable for Table Reads or Table Writes
#FUSES MCU                      //Microcontroller Mode
#FUSES NOWRTC                   //configuration not registers write protected
#FUSES NOWRTB                   //Boot block not write protected
#FUSES NOEBTR                   //Memory not protected from table reads
#FUSES NOEBTRB                  //Boot block not protected from table reads
#FUSES NOCPB                    //No Boot Block code protection

#use delay(clock = 40000000)

#use fast_io(A)
#use fast_io(B)
#use fast_io(C)
//#use fast_io(D)
#use fast_io(E)
#use fast_io(F)
#use fast_io(G)
#use fast_io(H)
#use fast_io(J)


#define Tx1         PIN_C6  //-- Pin interface Computer
#define Rx1         PIN_C7  
#define Tx2         PIN_G1  
#define Rx2         PIN_G2
#define SPD_115200  115200
#define SPD_38400   38400
#define SPD_19200   19200
#define SPD_9600    9600
#define SW_ID       0x70

//******************************************************************************
//    OUTPUT DEFINE
#define RELAY      PIN_A3
#define TRIG       PIN_C0

#define SV_SLIDER  PIN_D0
#define SV_LOCK    PIN_D1
#define SV_STAMP   PIN_D2
#define SV_QR      PIN_D3 
#define SV_COILS   PIN_D4
#define SV_SCREW   PIN_D5   

#define LED_START  PIN_D7
#define BUZZER     PIN_E1
#define LEDG       PIN_E2
#define LEDR       PIN_E3

//******************************************************************************
//    INPUT DEFINE
#define SW_RES         PIN_B0  
#define SW_START       PIN_B1     
#define SS_LOCK        PIN_B2

#define SS_UNLOCK      PIN_B3   
#define SS_STAMP_DOWN  PIN_B4
#define SS_STAMP_UP    PIN_B5     
#define SS_QR_DOWN     PIN_C1   
#define SS_QR_UP       PIN_C2
#define SS_COIL_UP     PIN_G0     
#define SS_COIL_DOWN   PIN_E7
#define SS_SCREW_UP    PIN_G3     
#define SS_SCREW_DOWN  PIN_G4
#define SS_L           PIN_F0  
#define SS_R           PIN_F1 
#define SS_SCREW1      PIN_F2        
#define SS_SCREW2      PIN_F3     
#define SS_SCREW3      PIN_F4        
#define SS_SCREW4      PIN_F5  


#include <lcd_16char_2or4_line.c> 
                                                        
#use rs232(baud = SPD_19200, xmit = TX1, rcv = RX1, BITS = 8, PARITY = N)
#use i2c(Master,Fast,sda = PIN_C4,scl = PIN_C3)

#include <RS232_Lib.c> 


//############################################################################\\
//####################      Define Parameter       ###########################\\
      
   typedef struct
      {
         boolean Reset_Flag;
         boolean Display_Spec; 
         boolean CompleteSpec;
         boolean Get_Qr;
         boolean Ack_Qr;
         boolean ResCoils;
         boolean ResScrew;
         int Err_Coil;
         byte ModeI;
      }_Rob_;
   _Rob_ Rob;
   
   typedef struct
      {         
         int16 C1h;
         int16 C1l;
         int16 C2h;
         int16 C2l;
         int16 C3h;
         int16 C3l;
         byte  BrSize;
         byte  TypeScr;
      } _Spec_;
   _Spec_ SPEC;
      
   byte sw;
   char QR_Packet[20];
   char Qr_Show[20];
   //const char Vers[] = "V1.2.1";
   const int16 TimeOut = 2000;
   
   
byte GetStatus_SS(byte id)  ;  
   
//-------------------------------------------------------:PCF8574 READ
byte GetStatus_SS(byte id) 
{
  byte idat;                      
      i2c_start();                      
      i2c_write(id+1);
      idat = i2c_read(0);             
      i2c_stop();                     
  return(idat);
}



