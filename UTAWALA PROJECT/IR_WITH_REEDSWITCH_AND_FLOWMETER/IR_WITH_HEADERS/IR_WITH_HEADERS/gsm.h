/*
 * gsm.h
 *
 * Created: 4/19/2021 5:50:26 PM
 *  Author: mbithe
 */ 


#ifndef GSM_H_
#define GSM_H_

char url[100];

char url1[]="AT+HTTPPARA=\"URL\",\"www.fedha.ke/http/insert.php?ladiestotal=";
char url2[]="\"\r\n";

char AT[]="AT\r";
char CONTYPE[]="AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n";
char APN[]="AT+SAPBR=3,1,\"APN\",\"internet\"\r\n";
char USER[]="AT+SAPBR=3,1,\"USER\",\"\"\r\n";
char PWD[]="AT+SAPBR=3,1,\"PWD\",\"\"\r\n";
char open_GPRS[]="AT+SAPBR=1,1\r\n";
char query_GPRS[]="AT+SAPBR=2,1\r\n";
char http_init[]="AT+HTTPINIT\r\n";
char http_para[]="AT+HTTPPARA=\"CID\",1\r\n";
char start_GET[]="AT+HTTPACTION=0\r\n";
char http_read[]="AT+HTTPREAD\r\n";
char http_term[]="AT+HTTPTERM\r\n";
char close_GPRS[]="AT+SAPBR=0,1\r\n";



#endif /* GSM_H_ */