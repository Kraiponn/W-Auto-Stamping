//##############################################################################
//                   
//    *********   *******
//    *       *  *      *
//    *       *  * 
//    *********   *******
//    *   *              *  
//    *    *      *      *
//    *      *     ******
//


char getc1USART();
void putc1USART(char c);
void puts1USART(char s);
char getc2USART();
void putc2USART(char c);
void puts2USART(char s);

//------------------------------------------------------------------------------
//-------------------   GET CHARACTER FROM USART1 
char getc1USART()
{
   #use rs232(baud = SPD_19200, xmit = TX1, rcv = RX1, BITS = 8, PARITY = N)
   return(getc());
}

//------------------------------------------------------------------------------
//-------------------   PUT CHARACTER TO USART1 
void putc1USART(char c)
{
   #use rs232(baud = SPD_19200, xmit = TX1, rcv = RX1, BITS = 8, PARITY = N)
   putc(c);
}

//------------------------------------------------------------------------------
//-------------------   PUT STRING TO USART1 
void puts1USART(char s)
{
   #use rs232(baud = SPD_19200, xmit = TX1, rcv = RX1, BITS = 8, PARITY = N)
   puts(s);
}

//------------------------------------------------------------------------------
//-------------------   GET CHARACTER FROM USART2 
char getc2USART()
{
   #use rs232(baud = SPD_38400, xmit = TX2, rcv = RX2, BITS = 8, PARITY = N)
   return(getc());
}

//------------------------------------------------------------------------------
//-------------------   PUT CHARACTER TO USART2 
void putc2USART(char c)
{
  #use rs232(baud = SPD_38400, xmit = TX2, rcv = RX2, BITS = 8, PARITY = N)
   putc(c);
}

//------------------------------------------------------------------------------
//-------------------   PUT STRING TO USART1 
void puts2USART(char s)
{
   #use rs232(baud = SPD_38400, xmit = TX1, rcv = RX1, BITS = 8, PARITY = N)
   puts(s);
}


