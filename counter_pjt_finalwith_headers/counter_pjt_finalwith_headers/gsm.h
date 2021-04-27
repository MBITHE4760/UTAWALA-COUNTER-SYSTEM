/*
 * gsm.h
 *
 * Created: 4/27/2021 9:04:04 AM
 *  Author: mbithe
 */ 


#ifndef GSM_H_
#define GSM_H_

int ladies_total_count;
int gents_total_count;
int i;
int num_ladies;
int num_gents;
bool gentsbafu_closed;
bool ladiesbafu_closed;
int gents_bath_flag;
int ladies_bath_flag;
volatile int pulsecount_ladies;
volatile int pulsecount_gents;
int gentsbafu_count;
int ladiesbafu_count;
int num_gentsbafu;
int num_ladiesbafu;

char buffer0[256];
char buffer3[256];
int position0;
int position3;


char url[100];
char gents_total[]="&gentstotal=";
char gents_bafu[]="&gentsbafu=";
char ladies_bafu[]="&ladiesbafu=";
char url1[]="AT+HTTPPARA=\"URL\",\"www.fedha.ke/http/insert.php?ladiestotal=";
char url2[]="\"\r\n";

char AT[]="AT\r";
char CONTYPE[]="AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n";
char APN[]="AT+SAPBR=3,1,\"APN\",\"safaricom\"\r\n";
char USER[]="AT+SAPBR=3,1,\"USER\",\"saf\"\r\n";
char PWD[]="AT+SAPBR=3,1,\"PWD\",\"data\"\r\n";
char open_GPRS[]="AT+SAPBR=1,1\r\n";
char query_GPRS[]="AT+SAPBR=2,1\r\n";
char http_init[]="AT+HTTPINIT\r\n";
char http_para[]="AT+HTTPPARA=\"CID\",1\r\n";
char start_GET[]="AT+HTTPACTION=0\r\n";
char http_read[]="AT+HTTPREAD\r\n";
char http_term[]="AT+HTTPTERM\r\n";
char close_GPRS[]="AT+SAPBR=0,1\r\n";

void UART0_TRANSMIT(char* data){
	while(*data){
		while(!(UCSR0A & (1<<UDRE0))){}
		UDR0=*data;
		data++;
	}
}
void UART3_TRANSMIT(char* data){
	while(*data){
		while(!(UCSR3A & (1<<UDRE3))){}
		UDR3=*data;
		data++;
	}
}


void clearbuffer3()
{
	int i;
	for (i=0;i<256;i++)
	{
		buffer3[position3]=0x00;
	}
	position3=0;
}



void send_count()
{
	num_ladies=ladies_total_count;
	ladies_total_count=0;
	num_gents=gents_total_count;
	gents_total_count=0;
	num_gentsbafu=gentsbafu_count;
	gentsbafu_count=0;
	num_ladiesbafu=ladiesbafu_count;
	ladiesbafu_count=0;
	
	if ((ladies_total_count !=0) || (gents_total_count!=0) || (gentsbafu_count !=0) || (ladiesbafu_count !=0))
	{
	char ch[10];
	itoa(num_ladies,ch,10);
	
	char ch1[10];
	itoa(num_gents,ch1,10);
	
	char ch2[10];
	itoa(num_gentsbafu,ch2,10);
	
	char ch3[10];
	itoa(num_ladiesbafu,ch3,10);
	strcpy(url,url1);
	strcat(url,ch);
	strcat(url,gents_total);
	strcat(url,ch1);
	strcat(url,gents_bafu);
	strcat(url,ch2);
	strcat(url,ladies_bafu);
	strcat(url,ch3);
	strcat(url,url2);
	
	UART0_TRANSMIT(AT);
	UART3_TRANSMIT(AT);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(AT);
		UART3_TRANSMIT(AT);
		UART0_TRANSMIT(buffer3);

	}
	clearbuffer3();
	
	UART0_TRANSMIT(CONTYPE);
	UART3_TRANSMIT(CONTYPE);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(CONTYPE);
		UART3_TRANSMIT(CONTYPE);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(APN);
	UART3_TRANSMIT(APN);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(APN);
		UART3_TRANSMIT(APN);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(USER);
	UART3_TRANSMIT(USER);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(USER);
		UART3_TRANSMIT(USER);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(PWD);
	UART3_TRANSMIT(PWD);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(PWD);
		UART3_TRANSMIT(PWD);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(open_GPRS);
	UART3_TRANSMIT(open_GPRS);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(open_GPRS);
		UART3_TRANSMIT(open_GPRS);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(query_GPRS);
	UART3_TRANSMIT(query_GPRS);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(query_GPRS);
		UART3_TRANSMIT(query_GPRS);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(http_init);
	UART3_TRANSMIT(http_init);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(http_init);
		UART3_TRANSMIT(http_init);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(http_para);
	UART3_TRANSMIT(http_para);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(http_para);
		UART3_TRANSMIT(http_para);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(url);
	UART3_TRANSMIT(url);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(url);
		UART3_TRANSMIT(url);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(start_GET);
	UART3_TRANSMIT(start_GET);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(start_GET);
		UART3_TRANSMIT(start_GET);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(http_read);
	UART3_TRANSMIT(http_read);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(http_read);
		UART3_TRANSMIT(http_read);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(http_term);
	UART3_TRANSMIT(http_term);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(http_term);
		UART3_TRANSMIT(http_term);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	
	UART0_TRANSMIT(close_GPRS);
	UART3_TRANSMIT(close_GPRS);
	UART0_TRANSMIT(buffer3);
	if (!strstr(buffer3,"OK"))
	{
		UART0_TRANSMIT(close_GPRS);
		UART3_TRANSMIT(close_GPRS);
		UART0_TRANSMIT(buffer3);
	}
	clearbuffer3();
	}
}
ISR(USART3_RX_vect)//gsm
{
	
	buffer3[position3]=UDR3;
	position3++;
}

ISR(USART0_RX_vect)//serial
{
	
	buffer0[position0]=UDR0;
	position0++;
}

#endif /* GSM_H_ */