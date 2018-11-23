#include"StichAlg.hpp"
#include"Queuebuffer.hpp"
#include"Stich.hpp"
#include "plantformcontrl.hpp"
#include"Gyroprocess.hpp"
StichAlg*StichAlg::instance=NULL;


StichAlg::StichAlg():tailcut(0),Seampostion(0),xoffsetfeat(0),yoffsetfeat(0),zeroflameupdate(1),camerazeroossfet(0),zeroflag(0),zeroangle(0),zerocalibflag(1),
	zerolostcout(0),zerolostflag(0),zerocalibrationstatus(1)
{
	
}
StichAlg::~StichAlg()
{

	
}

void StichAlg::zeroreset()
{
	

}

void StichAlg::Zeropreprocess()
{

	Mat src=getcurrentflame();
	if(getzeroflameupdate())
			{
				//if(getcurrentangle()<ANGLEINTREVAL&&getcurrentangle()>0)
				if(getcurrentangle()>0)
					{
						setzeroflameupdate(0);
						setzerolostcout(0);
						setzeroflame(src);
						imwrite("zero.jpg",src);
						//setgyrozero(getcurrentangle());
						//setcamerazeroossfet(-getcurrentangle());

						printf("the zero angle =%f\n",getcurrentangle());
						
						setcamerazeroossfet(-getcurrentangle());
						setzeroangle(0);
						///////////////zero angle
						setgyroangle(0);
						setcurrentangle(0);
						printf("BEGIN %s %d \n",__func__,__LINE__);
						zeroptzangle=Plantformpzt::getinstance()->getpanopan();
						zeroptztiangle=Plantformpzt::getinstance()->getpanotitle();
						printf("END %s %d \n",__func__,__LINE__);
						
						setptzzeroangle(zeroptzangle);
						setptzzerotitleangle(zeroptztiangle);
						
					}
				else
					setcurrentangle(0);
			}



}

int StichAlg::judgezero()
{
	double currentA=getcurrentangle();
	double zeroA=getzeroangle();

	double fovshift=Config::getinstance()->cam_fov*2/3;
	//if(currentA>360-ZEROJUEGE)
	//printf("fovshift=%f currentA=%f  zeroA=%f  \n",fovshift,currentA,zeroA);
	if(currentA>360-fovshift)
		{
			//currentA-=360;
			setzeroflag(1);
		}
	//double angleoffset=abs(currentA-zeroA);
	else{
			setzeroflag(0);
			setzerocalib(0);
			if(getzerocalibrationstatus())
				{
					setzerocalibrationstatus(1);
					setzerolostcoutadd();
				}
				

		}

		
	//if

}
void StichAlg::zerolostreset()
{
	if(getzerolostflag())
		{
			OSA_printf("%s line=%d\n",__func__,__LINE__);
			setzeroflameupdate(true);
		}


}

void StichAlg::zerolostjudge()
{
	if((getzerolostcout()>CALIBLOSTCOUNT)&&(abs(getcurrentangle())<1.0))
		setzerolostflag(1);
	else
		setzerolostflag(0);
		


}
void StichAlg::zeroprocess()
{
	double angle=0;
	double gyroangleoffset=0;
	setzeroprocessflag(0);
	judgezero();
	int zerooffset=0;
	
	int zerobool=getzeroflag();
	int calibool=getzerocalib();
	if(getorcezeroprocess()==0)
		{
			if(zerobool==0||calibool==1)
				{
					//printf("[the zerobool=%d calibool=%d]\n",zerobool,calibool);
					setzerocalibing(0);
					return ;
				}
		}
	
	setforcezeroprocess(0);
	setzerocalibing(1);
	Mat zeroF=getzeroflame();
	Mat currentF=getcurrentflame();

	double currentgyroA=getgyroangle();
	if(currentgyroA>300)
		currentgyroA-=360;
	double zeroA=getzeroangle();
	gyroangleoffset=currentgyroA-zeroA;

	//setyawbase();
	//printf("**************zeroprocess************************\n");
	if(zerocalibration(zeroF,currentF,&angle,&zerooffset))
		{
			
			//setcamerazeroossfet(angle-(currentgyroA-zeroA));
			setzerocalibing(0);
			if(ZEROCALIBRATIONMODE)
				{
					/***drop mod***/
					setzeroprocessflag(1);
					setzerocaliboffset(zerooffset);
					setcurrentangle(0);
						/***drop mod***/
					setzerodroreset(1);
					setcalibrationzeroangle(getgyroangle());
					
				}
			
			
			
			
		
			setyawbase(gyroangleoffset-angle);
			setzerocalib(1);
			
			setzerolostcout(0);
			setzerocalibrationstatus(1);
			
			printf("********basioffset=%f********zeroA=%f  gyroangleoffset=%f currentgyroA=%f \n",gyroangleoffset-angle,zeroA,gyroangleoffset,currentgyroA);
			//setzeroflag(0);
		}
		else
			{
				setzerocalibrationstatus(0);

			}


			

	

	
}

void StichAlg::stichprocess()
{
	//OSA_printf("StichAlg %s:*****************line=%d**********************\n",__func__,__LINE__);
	if(Config::getinstance()->getpanocalibration())
		{
			zerolostjudge();
			//OSA_printf("StichAlg %s:*****************line=%d**********************\n",__func__,__LINE__);
			//zerolostreset();
			//OSA_printf("StichAlg %s:*****************line=%d**********************\n",__func__,__LINE__);
			Zeropreprocess();
			//OSA_printf("StichAlg %s:*****************line=%d**********************\n",__func__,__LINE__);
			zeroprocess();
			//OSA_printf("StichAlg %s:*****************line=%d**********************\n",__func__,__LINE__);
		}
	Panoprocess();
	//OSA_printf("StichAlg %s:*****************line=%d**********************\n",__func__,__LINE__);

}

void StichAlg::Panoprocess()
{
	//setNextFrameId();
	Mat src=getcurrentflame();
	Mat dst=getdisflame();
	
	double angleoffset=0.0;
	int  piexoffset=0;
	int preoffset=getpanooffet(getpreangle());
	int curoffset=getpanooffet(getcurrentangle());
	angleoffset=getcurrentangle()-getpreangle();

	if(angleoffset<0)
		angleoffset=angleoffset+360;

	piexoffset=getpanooffet(angleoffset);


	Mat Imagepre;
	if(IMAGEPROCESSSTICH)
		{
			Imagepre=getpreprocessimage();
			setpreprocessimage(src);
		}
		

	
	setSeamPos(piexoffset);
	Mat temp;
	if(Config::getinstance()->getpanocylinder())
	temp=src;//getCurrentFame();
	else
	temp=dst;

	
	/*******************************/

	



	/******************************/
	
	//Mat dst
	if(src.cols!=Config::getinstance()->getpanoprocesswidth()||src.rows!=Config::getinstance()->getpanoprocessheight())
			resize(src,temp,Size(Config::getinstance()->getpanoprocesswidth(),Config::getinstance()->getpanoprocessheight()),0,0,INTER_LINEAR);
	else
		{
		//memcpy(temp.data,src.data,PANO360WIDTH*PANO360HEIGHT*3);
		if(Config::getinstance()->getpanocylinder()==0)
		Matcpy(src,temp,Config::getinstance()->getpanoprocessshift());
		}
	//return ;
	double exec_time = (double)getTickCount();
	if(Config::getinstance()->getpanocylinder())
		cylinder(temp,dst,1.0*(Config::getinstance()->getcamfx())*Config::getinstance()->getpanoprocesswidth()/Config::getinstance()->getcamwidth(),Config::getinstance()->getpanoprocessshift());

		
	exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
	//OSA_printf("the %s exec_time=%f MS\n",__func__,exec_time);
	if(getpreframeflage()==0)
		return ;
	Mat pre=getpreframe();
	if(FEATURESTICH)
		{
			if(IMAGEPROCESSSTICH)
				getfeaturePanoOffset(Imagepre,dst,&xoffsetfeat,&yoffsetfeat);
			else
				getPanoOffset(dst,pre,&xoffsetfeat,&yoffsetfeat);
				
			//printf();
			setcurrentangle(offet2anglerelative2inter(-xoffsetfeat));
			curoffset=getpanooffet(getcurrentangle());
			setSeamPos(curoffset-preoffset);
			OSA_printf("the xoffsetfeat=%d  yoffsetfeat=%d getcurrentangle=%f\n",xoffsetfeat,yoffsetfeat,getcurrentangle());
		}
	
	if(getfusionenable())
		{
			//Mat pre=getpreframe();
			//if((getcurrentangle()<360-ZEROJUEGE)&&(getcurrentangle()>0))
				FusionSeam(pre,dst,getSeamPos());
			//	histequision(dst);
				//equalizeHist(dst, dst);
				//OSA_printf("the getSeamPos=%d  getpreangle=%f getcurrentangle=%f\n",getSeamPos(),getpreangle(),getcurrentangle());
		}



	
	
	

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
		//OSA_printf("StichAlg %s: Main Proc Tsk Is Entering...while\n",__func__);
		OSA_BufInfo *inputif=(OSA_BufInfo *)queuebuf->getfull(Queue::TOPANOSTICH, 0, OSA_TIMEOUT_FOREVER);
		if(inputif==NULL)
			continue;
		
		//OSA_printf("StichAlg %s:*****************begin**********************\n",__func__);
		Mat src=Mat(inputif->height,inputif->width,CV_8UC3,inputif->virtAddr);
		setcurrentflame(src);
		//imshow("stichalg",src);
		//waitKey(1);
		
		double angle=0;
		if(FEATURESTICH)
		{
			
		}
		else	
		angle=inputif->framegyroyaw*1.0/ANGLESCALE+getcamerazeroossfet();

		//OSA_printf("the algle is %f\n",angle);
		
		if(angle<0)
			angle+=360;
		else if(angle>=360)
			angle-=360;
		setcurrentangle(angle);
		setgyroangle(angle);


		//OSA_printf("StichAlg %s:*****************line=%d**********************\n",__func__,__LINE__);

		OSA_BufInfo *outputif=(OSA_BufInfo *)queuebuf->getempty(Queue::FROMEPANOSTICH,0,OSA_TIMEOUT_NONE);
		
		if(outputif == NULL){
			//OSA_printf("FROMEPANOSTICH NO QUEUE FREE\n");
			queuebuf->putempty(Queue::TOPANOSTICH,0, inputif);
			OSA_printf("StichAlg %s:**********1*******end**********************\n",__func__);
			continue;
			
			}
		Mat dst=Mat(Config::getinstance()->getpanoprocessheight(),Config::getinstance()->getpanoprocesswidth(),CV_8UC3,outputif->virtAddr);
		setdisflame(dst);
		//OSA_printf("StichAlg %s:*****************line=%d**********************\n",__func__,__LINE__);
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
		
			
		//OSA_printf("StichAlg %s:*****************line=%d**********************\n",__func__,__LINE__);

		double exec_time = (double)getTickCount();
		stichprocess();
		//OSA_printf("StichAlg %s:*****************line=%d**********************\n",__func__,__LINE__);
		exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
	 	//OSA_printf("the %s exec_time=%f MS\n",__func__,exec_time);

		//memcpy(dst.data,src.data,Config::getinstance()->getpanoprocessheight()*Config::getinstance()->getpanoprocesswidth()*3);





		outputif->channels = inputif->channels;
		outputif->width =Config::getinstance()->getpanoprocesswidth();
		outputif->height =Config::getinstance()->getpanoprocessheight();
		outputif->timestamp =inputif->timestamp;
		outputif->framegyroroll=inputif->framegyroroll;
		outputif->framegyropitch=inputif->framegyropitch;
		outputif->framegyroyaw=getcurrentangle()*ANGLESCALE;


		//OSA_printf("StichAlg %s:***********2******end**********************\n",__func__);
		queuebuf->putfull(Queue::FROMEPANOSTICH,0, outputif);
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
	ProcessPreimage=Mat(Config::getinstance()->getpanoprocessheight(),Config::getinstance()->getpanoprocesswidth(),CV_8UC3,cv::Scalar(0));
	zeroflame=Mat(Config::getinstance()->getcamheight(),Config::getinstance()->getcamwidth(),CV_8UC3,cv::Scalar(0));
	MAIN_threadCreate();

}

StichAlg *StichAlg::getinstance()
{
	if(instance==NULL)
		instance=new StichAlg();



	return instance;
	


}


