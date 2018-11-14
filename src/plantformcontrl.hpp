/*
 * plantformcontrl.hpp
 *
 *  Created on: 2018年10月25日
 *      Author: wj
 */

#ifndef PLANTFORMCONTRL_HPP_
#define PLANTFORMCONTRL_HPP_
#include "pelcoFactory.h"
#include "pelcoBase.h"
#include "CUartBase.hpp"
#include <osa_thr.h>
#include"osa_sem.h"
#include"config.hpp"

typedef struct {
	bool bTrack;
	bool bMtd;
	bool bBlobDetect;
	int chId;
	int iTrackStat;
	
	//Mat frame;
}MAIN_plantfromThrObj_cxt;

typedef struct {
	MAIN_plantfromThrObj_cxt cxt[2];
	OSA_ThrHndl		thrHandleProc;
	OSA_SemHndl	procNotifySem;
	int pp;
	bool bFirst;
	volatile bool	exitProcThread;
	bool						initFlag;
	void 						*pParent;
}MAIN_plantfromThrObj;

class Plantformpzt
{


	enum{
		PLANTFORMGETPAN,
		PLANTFORMGETTITLE,
		PLANTFORMINITPAN,
		PLANTFORMINITTITLE,
		PLANTFORMGETANGEL,
		PLANTFORMMAX,


		};
	#define SENDLEN (7)
	public:
		static Plantformpzt *getinstance();
		void create();
		void destery();
	private:
		Plantformpzt();
		~Plantformpzt();
		static Plantformpzt *instance;


	private:
		CUartBase Uart;



		int fd;
		#define TIMEOUTCOUNT (5)
		

		unsigned char recvbuff[2000];

		PELCO_D_REQPKT PELCO_D;
		int mainloop ;
		int address;
		int ptzpd;

		double plantformpan;
		double plantformtitle;



		double panangle;
		double titleangle;

		double getpanangle(){return plantformpan;};
		double gettitleangle(){return plantformtitle;};
		OSA_SemHndl	procNotifySem;

		int calibration ;


		MAIN_plantfromThrObj	mainRecvThrObj;
		MAIN_plantfromThrObj	mainContrlThrdetectObj;


		void main_Recv_func();
		int MAIN_threadRecvCreate(void);
		int MAIN_threadRecvDestroy(void);
		static void *mainRecvTsk(void *context)
		{
			MAIN_plantfromThrObj  * pObj= (MAIN_plantfromThrObj*) context;
			if(pObj==NULL)
				{

				printf("++++++++++++++++++++++++++\n");

				}
			Plantformpzt *ctxHdl = (Plantformpzt *) pObj->pParent;
			ctxHdl->main_Recv_func();
			
			return NULL;
		}

		void main_contrl_func();
		int MAIN_contrlthreadCreate(void);
		int MAIN_contrlthreadDestroy(void);
		static void *maincontrlTsk(void *context)
		{
			MAIN_plantfromThrObj  * pObj= (MAIN_plantfromThrObj*) context;
			if(pObj==NULL)
				{

				printf("++++++++++++++++++++++++++\n");

				}
			Plantformpzt *ctxHdl = (Plantformpzt *) pObj->pParent;
			ctxHdl->main_contrl_func();
			
			return NULL;
		}
	public:
		#define TIMEOUT (1000)
		int timeout[PLANTFORMMAX];
		int timeoutflag[PLANTFORMMAX];
		int plantinitflag;


	public :
		int getplantinitflag(){return plantinitflag;};
		void milliseconds_sleep(unsigned long mSec);
		void plantformcontrluninit();
		void plantformcontrlinit();
		void setpanoscan();
		void setpanoscanstop();
		void setpanopanpos(double value);
		void setpanotitlepos(double value);
		void getpanopanpos();
		void getpanotitlepos();
		double getpanopan();
		double getpanotitle();
		void initptzpos(double pan,double title);
		void setplantformcalibration(int flag);
		int  getplantformcalibration();
		void setpanoantiscan();
		unsigned char chechsum(unsigned char *pelcodbuf);
};
#endif /* PLANTFORMCONTRL_HPP_ */
