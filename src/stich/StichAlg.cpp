#include"StichAlg.hpp"
#include"Queuebuffer.hpp"
StichAlg*StichAlg::instance=NULL;


StichAlg::StichAlg()
{
	
}
StichAlg::~StichAlg()
{

	
}


void StichAlg::main_proc_func()
{

	OSA_printf("StichAlg %s: Main Proc Tsk Is Entering...\n",__func__);
	unsigned int framecount=0;
	OSA_BufInfo* info=NULL;
	OSA_BufInfo* infocap=NULL;
	Queue *queuebuf;
	Mat pre;
	while(mainProcThrObj.exitProcThread ==  false)
	{

		queuebuf=Queue::getinstance();
		OSA_printf("StichAlg %s: Main Proc Tsk Is Entering...while\n",__func__);
		OSA_BufInfo *inputif=(OSA_BufInfo *)queuebuf->getfull(Queue::TOPANOSTICH, 0, OSA_TIMEOUT_FOREVER);
		if(inputif==NULL)
			continue;

		Mat src=Mat(infocap->height,infocap->width,CV_8UC3,infocap->virtAddr);
		
		double angle=0;
		if(FEATURESTICH)
		{
			
		}
		else	
		angle=infocap->framegyroyaw*1.0/ANGLESCALE+getcamerazeroossfet();

		//OSA_printf("the algle is %f\n",angle);
		
		if(angle<0)
			angle+=360;
		else if(angle>=360)
			angle-=360;
		setcurrentangle(angle);



		OSA_BufInfo *outputif=(OSA_BufInfo *)queuebuf->getempty(Queue::FROMEPANOSTICH,0,OSA_TIMEOUT_NONE);
		
		if(outputif == NULL){
			OSA_printf("FROMEPANOSTICH NO QUEUE FREE\n");
			queuebuf->putempty(Queue::TOPANOSTICH,0, inputif);
			continue;
			
			}
		Mat dst=Mat(Config::getinstance()->getpanoprocessheight(),Config::getinstance()->getpanoprocesswidth(),CV_8UC3,info->virtAddr);

		int count=queuebuf->getfullcount(Queue::FROMEPANOSTICH,0);
		if(count>0)
			{
				
				setpreframeflag(1);
				OSA_BufInfo *preif=(OSA_BufInfo *)queuebuf->getprefull(Queue::FROMEPANOSTICH,0,outputif);				
				char *data=(char *)(preif->virtAddr);
				unsigned w=preif->width;
				unsigned h=preif->height;
				double angle=preif->framegyroyaw*1.0/ANGLESCALE;
				setpreangle(angle);
				pre=Mat(h,w,CV_8UC3,data);
				//imshow("t",pre);
				//waitKey(2);
				setpreframe(pre);
				;
			}
		else
			setpreframeflag(0);
		
			





		



		queuebuf->putempty(Queue::FROMEPANOSTICH,0, outputif);
		queuebuf->putempty(Queue::TOPANOSTICH,0, inputif);

		
		

	}


}
int StichAlg::MAIN_threadCreate(void)
{
	int iRet = OSA_SOK;
	iRet = OSA_semCreate(&mainProcThrObj.procNotifySem ,1,0) ;
	OSA_assert(iRet == OSA_SOK);


	mainProcThrObj.exitProcThread = false;

	mainProcThrObj.initFlag = true;

	mainProcThrObj.pParent = (void*)this;

	iRet = OSA_thrCreate(&mainProcThrObj.thrHandleProc, mainProcTsk, 0, 0, &mainProcThrObj);
	





	return iRet;
}
int StichAlg::MAIN_threadDestroy(void)
{
	int iRet = OSA_SOK;

	mainProcThrObj.exitProcThread = true;
	OSA_semSignal(&mainProcThrObj.procNotifySem);

	iRet = OSA_thrDelete(&mainProcThrObj.thrHandleProc);

	mainProcThrObj.initFlag = false;
	OSA_semDelete(&mainProcThrObj.procNotifySem);

	return iRet;
}
void StichAlg::create()
{
	
	MAIN_threadCreate();

}

StichAlg *StichAlg::getinstance()
{
	if(instance==NULL)
		instance=new StichAlg();



	return instance;
	


}


