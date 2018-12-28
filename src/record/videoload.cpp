#include"videoload.hpp"
#include<stdio.h>
VideoLoad* VideoLoad::instance=NULL;
#define DIRRECTDIR  "/home/ubuntu/calib/"
VideoLoad::VideoLoad():callfun(NULL),readnewfile(0),readname("1.xml"),readavi("1.avi"),readdir(DIRRECTDIR)
{


}
VideoLoad::~VideoLoad()
{
	

}

void VideoLoad::create()
{
	MAIN_threadRecvCreate();
	OSA_semCreate(&loadsem,1,0);

}

void VideoLoad::destory()
{
	MAIN_threadRecvDestroy();
	OSA_semDelete(&loadsem);
}



void VideoLoad::registerfun(VideoCallBackfun fun)
{	
	callfun=fun;
}


void VideoLoad::playvideo()
{
	OSA_semSignal(&loadsem);

}
void VideoLoad::initvideo()
{



}
void VideoLoad::main_Recv_func()
{
	OSA_printf("%s: Main Proc Tsk Is Entering...\n",__func__);
	
	unsigned char *data=NULL;
	int angle=0;
	int status;
	VideoCapture videocapture;
	Mat fileframe;
	videocapture.open(readavi);
	VideoLoadData loaddata;
	mydata.open(readname.c_str());
	string aviname=readdir+readavi;
	string  xmlname=readdir+readname;
	memset(&loaddata,0,sizeof(VideoLoadData));
	static double gyrodata=0;
	
	while(mainRecvThrObj.exitProcThread ==  false)
	{	
		int capangle=0;
		//OSA_waitMsecs(30);
		OSA_semWait(&loadsem,OSA_TIMEOUT_FOREVER);
		if(getreadnewfile())
			{
				setreadnewfile(0);
				aviname=readdir+getreadavi();
				xmlname=readdir+getreadname();
				videocapture.release();
				mydata.close();
				videocapture.open(aviname);
				mydata.open(xmlname.c_str());

			}
		
		//initvideo();
		if(getreadnewfile())
			{

			}
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
				
				//printf("********loaddata=%f******\n",loaddata.gyroz);
			}
		else
			{
				//videocapture.set(CV_CAP_PROP_POS_FRAMES,0);
				videocapture.release();
				videocapture.open(aviname);
				mydata.close();
				mydata.open(xmlname.c_str());
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
