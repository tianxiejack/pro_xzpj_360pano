#include "plantformcontrl.hpp"
#include "pelcoFactory.h"
#include "pelcoBase.h"
#include "CUartBase.hpp"
#include <osa_thr.h>
static IPelcoBaseFormat *PlantformContrl;

CUartBase Uart;

#define UART422NAME "/dev/ttyTHS1"

int fd=0;
OSA_ThrHndl thrHandleProc;
unsigned char recvbuff[2000];

PELCO_D_REQPKT PELCO_D;
int mainloop =1;

#define SENDLEN (7)
void * mainProcTsk(void *param)
{
	int buflen=0;
	while(mainloop)
		{
			buflen=Uart.UartRecv( fd,  recvbuff, 100);
			printf("%s  buflen=%d\n",recvbuff,buflen);
			//puts(recvbuff);
		}
	
}


void plantformcontrlinit()
{
	
	PlantformContrl=IPelcoFactory::createIpelco(pelco_D);
	fd=Uart.UartOpen(UART422NAME);
	Uart.UartSet(fd, 9600, 8, 'n', 1);
	//Uart.UartSet(fd, 1, 8, 'n', 1);
	OSA_thrCreate(&thrHandleProc, mainProcTsk, 0, 0, NULL);

}


void setpanoscan()
{
	int len=0;
	PlantformContrl->MakeMove(&PELCO_D, PTZ_MOVE_Right,20,true, 1);
	char buf[200];
	//sprintf(buf,"i\r");
	sprintf(buf,"SERVICEMODE\r");
	unsigned char *pelcodbuf=( unsigned char *) &PELCO_D;
	for(int i=0;i<SENDLEN;i++)
		printf(" %0x \t" ,pelcodbuf[i]);
	printf("\n");
	//len=Uart.UartSend(fd,( unsigned char *) buf, strlen(buf));
	len=Uart.UartSend(fd,pelcodbuf,SENDLEN);
	printf("********************ok*****************send len=%d\n",len);
}
void setpanoscanstop()
{

	PlantformContrl->MakeMove(&PELCO_D, PTZ_MOVE_Stop,0x10,true, 1);
	
	Uart.UartSend(fd,( unsigned char *) &PELCO_D, SENDLEN);
}

void setpanopanpos(short value)
{
	PlantformContrl->MakeSetPanPos(&PELCO_D, value,1);
	
	Uart.UartSend(fd,( unsigned char *)& PELCO_D, SENDLEN);

}

void setpanotitlepos(short value)
{
	PlantformContrl->MakeSetTilPos(&PELCO_D, value,1);
	
	Uart.UartSend(fd,( unsigned char *) &PELCO_D, SENDLEN);

}

void plantformcontrluninit()
{
	Uart.UartClose(fd);

}
