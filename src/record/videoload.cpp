#include"videoload.hpp"
#include<stdio.h>
VideoLoad* VideoLoad::instance=NULL;
VideoLoad::VideoLoad():callfun(NULL)
{


}
VideoLoad::~VideoLoad()
{


}

void VideoLoad::create()
{
	MAIN_threadRecvCreate();

}

void VideoLoad::destory()
{
	MAIN_threadRecvDestroy();

}


void VideoLoad::registerfun(VideoCallBackfun fun)
{

	callfun=fun;
}

#define AVINAME "/home/ubuntu/calib/1.avi"
#define DATAINAME "/home/ubuntu/calib/1.xml"
void VideoLoad::main_Recv_func()
{
	OSA_printf("%s: Main Proc Tsk Is Entering...\n",__func__);
	
	unsigned char *data=NULL;
	int angle=0;
	int status;
	VideoCapture videocapture;
	Mat fileframe;
	videocapture.open(AVINAME);
	VideoLoadData loaddata;
	mydata.open(DATAINAME);
	memset(&loaddata,0,sizeof(VideoLoadData));
	while(mainRecvThrObj.exitProcThread ==  false)
	{	
		int capangle=0;
		OSA_waitMsecs(30);
		status=videocapture.read(fileframe);
		if(!fileframe.empty())
			{
				
				//if()
				//resize(fileframe,fileframe,Size(config->getpanoprocesswidth(),config->getpanoprocessheight()),0,0,INTER_LINEAR);
				//printf("********2*file cap ok******\n");
				//fileframe.copyTo(img);

				
				//cvtColor(fileframe,img,CV_GRAY2BGR);
				//printf("********3*file cap ok******\n");

				loaddata=mydata.read();
				printf("********loaddata=%f******\n",loaddata.gyroz);
			}
		else
			{
				//videocapture.set(CV_CAP_PROP_POS_FRAMES,0);
				videocapture.release();
				videocapture.open(AVINAME);
				mydata.close();
				mydata.open(DATAINAME);
				videocapture.read(fileframe);
				loaddata=mydata.read();

			}
		
		if(callfun!=NULL)
			{

				callfun(fileframe.data,&loaddata);
			}
		//OSA_printf("%s: Main Proc Tsk Is Entering. capture ..\n",__func__);
		/*
		data=(unsigned char *)readfilepicture(&angle);
		//capangle=angle*1000;
		if(capturecallback!=NULL)
		capturecallback(capture.data,&angle);
		*/
	}

}


int VideoLoad::MAIN_threadRecvCreate(void)
{
	int iRet = OSA_SOK;
	iRet = OSA_semCreate(&mainRecvThrObj.procNotifySem ,1,0) ;
	OSA_assert(iRet == OSA_SOK);


	mainRecvThrObj.exitProcThread = false;

	mainRecvThrObj.initFlag = true;

	mainRecvThrObj.pParent = (void*)this;

	iRet = OSA_thrCreate(&mainRecvThrObj.thrHandleProc, mainRecvTsk, 0, 0, &mainRecvThrObj);
	

	return iRet;
}


int VideoLoad::MAIN_threadRecvDestroy(void)
{
	int iRet = OSA_SOK;

	mainRecvThrObj.exitProcThread = true;
	OSA_semSignal(&mainRecvThrObj.procNotifySem);

	iRet = OSA_thrDelete(&mainRecvThrObj.thrHandleProc);

	mainRecvThrObj.initFlag = false;
	OSA_semDelete(&mainRecvThrObj.procNotifySem);

	return iRet;
}

VideoLoad *VideoLoad::getinstance()
{
	if(instance==NULL)
		instance=new VideoLoad();
	return instance;

}
