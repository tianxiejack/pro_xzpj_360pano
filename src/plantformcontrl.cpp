#include "plantformcontrl.hpp"
#include "pelcoFactory.h"
#include "pelcoBase.h"
#include "CUartBase.hpp"
#include <osa_thr.h>
#include"osa_sem.h"
static IPelcoBaseFormat *PlantformContrl;

CUartBase Uart;

#define UART422NAME "/dev/ttyTHS1"

int fd=0;
OSA_ThrHndl thrHandleProc;
unsigned char recvbuff[2000];

PELCO_D_REQPKT PELCO_D;
int mainloop =1;

#define SENDLEN (7)

double panangle=0.0;
double titleangle=0.0;
OSA_SemHndl	procNotifySem;

int calibration =0;

void setplantformcalibration(int flag)
{
	calibration=flag;
}
int  getplantformcalibration()
{
	return calibration;
}
unsigned char chechsum(unsigned char *pelcodbuf)
{
	unsigned char sum=0;
	sum+=pelcodbuf[1];
	sum+=pelcodbuf[2];
	sum+=pelcodbuf[3];
	sum+=pelcodbuf[4];
	sum+=pelcodbuf[5];
	return sum;
	

}

#define TIMEOUTCOUNT (5)
static int timeout=0;
void * mainProcTsk(void *param)
{
	int buflen=0;
	unsigned short pana=0;
	unsigned short titlea=0;
	while(mainloop)
		{
			buflen=Uart.UartRecv( fd,  recvbuff, 100);
			if(buflen==0)
				continue;
			if(buflen!=SENDLEN)
				{
					timeout++;
					if(timeout<TIMEOUTCOUNT)
						{
							getpanopanpos();
							getpanotitlepos();
						}
					else
						{

							OSA_semSignal(&procNotifySem);
						}
					continue;
				}
			if(recvbuff[6]!=chechsum(recvbuff))
				{
					timeout++;
					if(timeout<TIMEOUTCOUNT)
						{
							getpanopanpos();
							getpanotitlepos();
						}
					else
						{
							OSA_semSignal(&procNotifySem);

						}
					continue ;
				}
			if(recvbuff[0]==0xff&&recvbuff[1]==0x01&&recvbuff[2]==0x00)
				{
					if(recvbuff[3]==0x59)
						{
							pana=recvbuff[4]<<8|recvbuff[5];
							panangle=pana*1.0/100;
						}
					else if(recvbuff[3]==0x5b)
						{
							titlea=recvbuff[4]<<8|recvbuff[5];
							titleangle=titlea*1.0/100;

						}

				}
			OSA_semSignal(&procNotifySem);
			
			
			//puts(recvbuff);
		}
	
}


void plantformcontrlinit()
{
	int iRet = OSA_SOK;
	PlantformContrl=IPelcoFactory::createIpelco(pelco_D);
	fd=Uart.UartOpen(UART422NAME);
	Uart.UartSet(fd, 9600, 8, 'n', 1);
	//Uart.UartSet(fd, 1, 8, 'n', 1);
	OSA_thrCreate(&thrHandleProc, mainProcTsk, 0, 0, NULL);
	iRet = OSA_semCreate(&procNotifySem ,1,0) ;
	OSA_assert(iRet == OSA_SOK);

}


void setpanoscan()
{
	int len=0;
	PlantformContrl->MakeMove(&PELCO_D, PTZ_MOVE_Right,20,true, 1);


	unsigned char *pelcodbuf=( unsigned char *) &PELCO_D;

	//len=Uart.UartSend(fd,( unsigned char *) buf, strlen(buf));
	len=Uart.UartSend(fd,pelcodbuf,SENDLEN);
	//printf("********************ok*****************send len=%d\n",len);
}

void setpanoantiscan()
{
	int len=0;
	PlantformContrl->MakeMove(&PELCO_D, PTZ_MOVE_Left,20,true, 1);


	unsigned char *pelcodbuf=( unsigned char *) &PELCO_D;

	//len=Uart.UartSend(fd,( unsigned char *) buf, strlen(buf));
	len=Uart.UartSend(fd,pelcodbuf,SENDLEN);
	//printf("********************ok*****************send len=%d\n",len);
}
void setpanoscanstop()
{

	PlantformContrl->MakeMove(&PELCO_D, PTZ_MOVE_Stop,0x10,true, 1);
	
	Uart.UartSend(fd,( unsigned char *) &PELCO_D, SENDLEN);
}

void setpanopanpos(double value)
{
	if(getplantformcalibration()==0)
		return ;
	unsigned short panvalue=value*100;
	PlantformContrl->MakeSetPanPos(&PELCO_D, panvalue,1);
	
	Uart.UartSend(fd,( unsigned char *)& PELCO_D, SENDLEN);

	//printf("getpanopan=%f\n",getpanopan());

	//getpanopanpos();

}

void setpanotitlepos(double value)
{
	if(getplantformcalibration()==0)
		return ;
	unsigned short panvalue=value*100;
	PlantformContrl->MakeSetTilPos(&PELCO_D, panvalue,1);
	
	Uart.UartSend(fd,( unsigned char *) &PELCO_D, SENDLEN);

	

}

void initptzpos(double pan,double title)
{
	
	unsigned short panvalue=title*100;
	PlantformContrl->MakeSetTilPos(&PELCO_D, panvalue,1);
	
	Uart.UartSend(fd,( unsigned char *) &PELCO_D, SENDLEN);

	

}

double getpanopan()
{
	unsigned char *pelcodbuf=( unsigned char *) &PELCO_D;
	pelcodbuf[0]=0xff;
	pelcodbuf[1]=0x01;
	pelcodbuf[2]=0x00;
	pelcodbuf[3]=0x51;
	pelcodbuf[4]=0x00;
	pelcodbuf[5]=0x00;
	pelcodbuf[6]=0x52;
	Uart.UartSend(fd,( unsigned char *) &PELCO_D, SENDLEN);
	OSA_semWait(&procNotifySem,OSA_TIMEOUT_FOREVER);

	return panangle;

}

double getpanotitle()
{
	unsigned char *pelcodbuf=( unsigned char *) &PELCO_D;
	pelcodbuf[0]=0xff;
	pelcodbuf[1]=0x01;
	pelcodbuf[2]=0x00;
	pelcodbuf[3]=0x53;
	pelcodbuf[4]=0x00;
	pelcodbuf[5]=0x00;
	pelcodbuf[6]=0x54;
	Uart.UartSend(fd,( unsigned char *) &PELCO_D, SENDLEN);
	OSA_semWait(&procNotifySem,OSA_TIMEOUT_FOREVER);
	return titleangle;
}

void getpanopanpos()
{

	unsigned char *pelcodbuf=( unsigned char *) &PELCO_D;
	pelcodbuf[0]=0xff;
	pelcodbuf[1]=0x01;
	pelcodbuf[2]=0x00;
	pelcodbuf[3]=0x51;
	pelcodbuf[4]=0x00;
	pelcodbuf[5]=0x00;
	pelcodbuf[6]=0x52;
	Uart.UartSend(fd,( unsigned char *) &PELCO_D, SENDLEN);

}

void getpanotitlepos()
{
	
	
	unsigned char *pelcodbuf=( unsigned char *) &PELCO_D;
	pelcodbuf[0]=0xff;
	pelcodbuf[1]=0x01;
	pelcodbuf[2]=0x00;
	pelcodbuf[3]=0x53;
	pelcodbuf[4]=0x00;
	pelcodbuf[5]=0x00;
	pelcodbuf[6]=0x54;
	Uart.UartSend(fd,( unsigned char *) &PELCO_D, SENDLEN);

}

void plantformcontrluninit()
{
	Uart.UartClose(fd);

}
