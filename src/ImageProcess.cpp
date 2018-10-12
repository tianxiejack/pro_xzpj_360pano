#include "ImageProcess.hpp"
#include"Homography.hpp"
#include "osa_image_queue.h"
#include"osa.h"
#include"Stich.hpp"
#include"Gyroprocess.hpp"
#define IMAGEQUEUESIZE 4
#define IMAGCAPEQUEUESIZE 4
#define IMAGEQUEUEWIDTH 1920
#define IMAGEQUEUEHEIGHT 1080
#define IMAGEQUEUECHANNEL 3

#define SIGNALEQUEUEID 0



ImageProcess::ImageProcess():pinpang(0),currentcount(0),AngleStich(-ANGLEINTREVAL),seamid(0),SeamEable(0),Seampostion(0),
			preflag(0),preangle(0),currentangle(0),xoffsetfeat(0),yoffsetfeat(0),camerazeroossfet(0),zeroflag(0),zeroangle(0),zerocalibflag(1),
			zeroflameupdate(1),gyroangle(0)
{
	memset(m_bufQue, 0, sizeof(m_bufQue));
	memset(mcap_bufQue, 0, sizeof(mcap_bufQue));
	

}
ImageProcess::~ImageProcess()
{


}

void ImageProcess::Init()
{
	//processgray[0]=
	for(int i=0;i<FRAMEFIFO;i++)
		{
			processgray[i]=Mat(CAPTURETVHEIGHT,CAPTURETVWIDTH,CV_8UC1,cv::Scalar(0));
			processtest=Mat(CAPTURETVHEIGHT,CAPTURETVWIDTH,CV_8UC1,cv::Scalar(0));
			processgraytemp[i]=Mat(405,720,CV_8UC1);
			//OSA_printf("the  DATA=%p\n",processgray[i].data);		
		}
	for(int chId=0; chId<IMAGEQUEUE; chId++)
		image_queue_create(&m_bufQue[chId], IMAGEQUEUESIZE,
				IMAGEQUEUEWIDTH*IMAGEQUEUEHEIGHT*IMAGEQUEUECHANNEL,
				memtype_normal);
	for(int chId=0; chId<IMAGEQUEUE; chId++)
		image_queue_create(&mcap_bufQue[chId], IMAGCAPEQUEUESIZE,
				IMAGEQUEUEWIDTH*IMAGEQUEUEHEIGHT*IMAGEQUEUECHANNEL,
				memtype_normal);
	processtemp=Mat(PANO360HEIGHT,PANO360WIDTH,CV_8UC3,cv::Scalar(0));
	processtempgray=Mat(PANO360HEIGHT,PANO360WIDTH,CV_8UC1,cv::Scalar(0));
	
	for(int i=0;i<MAXSEAM;i++)
		{
			Seamframe[i]=Mat(PANO360HEIGHT,PANO360WIDTH,CV_8UC3,cv::Scalar(0));
		}
	zeroflame=Mat(PANO360SRCHEIGHT,PANO360SRCWIDTH,CV_8UC3,cv::Scalar(0));
	
	MAIN_threadCreate();
}

void ImageProcess::unInit()
{
	
	MAIN_threadDestroy();
	for(int chId=0; chId<IMAGEQUEUE; chId++)
		image_queue_delete(&m_bufQue[chId]);
	for(int chId=0; chId<IMAGEQUEUE; chId++)
		image_queue_delete(&mcap_bufQue[chId]);
}

void ImageProcess::CaptureThreadProcess(Mat src,OSA_BufInfo* frameinfo)
{
	int queueid=0;
#if 0
	int frameid=getImagePinpang();
	int preframeid=getImagePrePinpang();
	double exec_time = (double)getTickCount();
	cvtColor(src,processgray[frameid],CV_BGR2GRAY);
	double rotsita=0;

	
	//processgray[frameid] = imread("/home/ubuntu/pano/4.bmp", 1);    //\u53f3\u56fe
	//processgray[preframeid] = imread("/home/ubuntu/pano/3.bmp", 1);
	#if 0
	resize(processgray[frameid],processgraytemp[frameid],Size(720,405),0,0,INTER_LINEAR);
	 
	  // do something ...
	   if(currentcount>100)
	  frameinfo->validintervalsita=getPano360Rotation(processgraytemp[preframeid],processgraytemp[frameid],&rotsita);
	 #else
	 
//	OSA_printf("the preid=%d currentid=%d DATA=%p\n",preframeid,frameid,processgray[preframeid].data);
//	 if(currentcount>100)
//	frameinfo->validintervalsita=getPano360Rotation(processgray[frameid],processgray[preframeid],&rotsita);
	
	 #endif
	  exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();

	  OSA_printf("the exec_time=%f\n",exec_time);
	
	frameinfo->intervalsita=rotsita;
	currentcount++;
	setnextImagePinpang();
#endif
	//OSA_semSignal(&mainProcThrObj.procNotifySem);
	if(abs(frameinfo->framegyroyaw/ANGLESCALE-AngleStich)<ANGLEINTREVAL)
		{
			
			return ;

		}
	OSA_BufInfo* info=NULL;
	info = image_queue_getEmpty(&mcap_bufQue[queueid]);
	if(info == NULL){
			return;
			}
	memcpy(info->virtAddr,src.data,src.rows*src.cols*src.channels());
	//OSA_printf("the w=%d h=%d c=%d\n",src.cols,src.rows,src.channels());
	info->channels = frameinfo->channels;
	info->width =frameinfo->width;
	info->height =frameinfo->height;
	info->timestamp =frameinfo->timestamp;
	info->framegyroroll=frameinfo->framegyroroll;
	info->framegyropitch=frameinfo->framegyropitch;
	info->framegyroyaw=frameinfo->framegyroyaw;
	image_queue_putFull(&mcap_bufQue[queueid], info);

	AngleStich=frameinfo->framegyroyaw/ANGLESCALE;


}

int ImageProcess::MAIN_threadCreate(void)
{
	int iRet = OSA_SOK;
	iRet = OSA_semCreate(&mainProcThrObj.procNotifySem ,1,0) ;
	OSA_assert(iRet == OSA_SOK);


	mainProcThrObj.exitProcThread = false;

	mainProcThrObj.initFlag = true;

	mainProcThrObj.pParent = (void*)this;

	iRet = OSA_thrCreate(&mainProcThrObj.thrHandleProc, mainProcTsk, 0, 0, &mainProcThrObj);
	




	//OSA_waitMsecs(2);

	return iRet;
}


int ImageProcess::MAIN_threadDestroy(void)
{
	int iRet = OSA_SOK;

	mainProcThrObj.exitProcThread = true;
	OSA_semSignal(&mainProcThrObj.procNotifySem);

	iRet = OSA_thrDelete(&mainProcThrObj.thrHandleProc);

	mainProcThrObj.initFlag = false;
	OSA_semDelete(&mainProcThrObj.procNotifySem);

	return iRet;
}


void ImageProcess::OptiSeam()
{


	Mat src=getPreFame();
	Mat dst=getCurrentFame();
	if(getSeamEnable())
		{	
			//printf("the seam=%d\n",getSeamPos());
			//dst=fusionframe;
			FusionSeam(src,dst,getSeamPos());
			
		}

}

int ImageProcess::judgezero()
{
	double currentA=getcurrentangle();
	double zeroA=getzeroangle();
	if(currentA>300)
		currentA-=360;
	double angleoffset=abs(currentA-zeroA);
	if(angleoffset>ZEROJUEGE)
		{
		setzeroflag(0);
		setzerocalib(0);
		}
	else
		setzeroflag(1);
		
	//if

}
void ImageProcess::zeroprocess()
{
	double angle=0;
	double gyroangleoffset=0;

	judgezero();
	
	int zerobool=getzeroflag();
	int calibool=getzerocalib();
	if(zerobool==0||calibool==1)
		return ;
	
	Mat zeroF=getzeroflame();
	Mat currentF=getcurrentflame();

	double currentgyroA=getgyroangle();
	if(currentgyroA>300)
		currentgyroA-=360;
	double zeroA=getzeroangle();
	gyroangleoffset=currentgyroA-zeroA;

	//setyawbase();
	if(zerocalibration(zeroF,currentF,&angle))
		{
			
			//setcamerazeroossfet(angle-(currentgyroA-zeroA));
			setyawbase(gyroangleoffset-angle);
			setzerocalib(1);
			printf("*********************zerocalibration****camerazeroossfet=%f****img=%f  gy=%f* \n",getcamerazeroossfet(),angle,gyroangleoffset);
			//setzeroflag(0);
		}
	

	
}

void ImageProcess::Panoprocess(Mat src,Mat dst)
{
	//setNextFrameId();
	int preoffset=getpanooffet(getpreangle());
	int curoffset=getpanooffet(getcurrentangle());
	setSeamPos(curoffset-preoffset);
	Mat temp;
	if(CYLINDER)
	temp=src;//getCurrentFame();
	else
	temp=dst;
	
	//Mat dst
	if(src.cols!=PANO360WIDTH||src.rows!=PANO360HEIGHT)
			resize(src,temp,Size(PANO360WIDTH,PANO360HEIGHT),0,0,INTER_LINEAR);
	else
		{
		//memcpy(temp.data,src.data,PANO360WIDTH*PANO360HEIGHT*3);
		if(CYLINDER==0)
		Matcpy(src,temp,PANOSRCSHIFT);
		}
	//return ;
	if(CYLINDER)
	cylinder(temp,dst,1.0*CAMERAFOCUSLENGTH*PANO360WIDTH/PANO360SRCWIDTH,500);

		

	if(getpreframeflage()==0)
		return ;
	Mat pre=getpreframe();
	if(FEATURESTICH)
		{
			getPanoOffset(pre,dst,&xoffsetfeat,&yoffsetfeat);
			setcurrentangle(offet2angle(xoffsetfeat*ANGLESCALE));
			curoffset=getpanooffet(getcurrentangle()/ANGLESCALE);
			setSeamPos(curoffset-preoffset);
		}
	
	if(getfusionenable())
		{
			//Mat pre=getpreframe();
			FusionSeam(pre,dst,getSeamPos());
			OSA_printf("the preoffset=%d curoffset=%d\n",preoffset,curoffset);
		}
	

}
void ImageProcess::main_proc_func()
{
	OSA_printf("%s: Main Proc Tsk Is Entering...\n",__func__);
	unsigned int framecount=0;
	OSA_BufInfo* info=NULL;
	OSA_BufInfo* infocap=NULL;
	int queueid=0;
	Mat pre;
	int xoffset,yoffset;
	double imageangle=0;
	while(mainProcThrObj.exitProcThread ==  false)
	{
	/******************************************/
		infocap=image_queue_getFullforever(&mcap_bufQue[queueid]);
		if(infocap==NULL)
			continue;
		Mat src=Mat(infocap->height,infocap->width,CV_8UC3,infocap->virtAddr);
		//imshow("t",cap);
		//waitKey(2);
		//OSA_printf("1****************************\n");

		
		
		//OSA_semWait(&mainProcThrObj.procNotifySem, OSA_TIMEOUT_FOREVER);
		info = image_queue_getEmpty(&m_bufQue[SIGNALEQUEUEID]);
		if(info == NULL){
			//info = image_queue_getFull(imgQ[queueid]);
			//OSA_assert(info != NULL);
			OSA_printf("NO QUEUE FREE\n");
			image_queue_putEmpty(&mcap_bufQue[queueid],infocap);
			continue;
			
			}
		Mat dst=Mat(PANO360HEIGHT,PANO360WIDTH,CV_8UC3,info->virtAddr);

		/********************pre flame  pre angle*********************/
		int count=image_queue_fullCount(&m_bufQue[SIGNALEQUEUEID]);
		if(count>0)
			{
			setpreframeflag(1);
			char *data=(char *)m_bufQue[queueid].bufInfo[(info->currentbufid-1+IMAGEQUEUESIZE)%IMAGEQUEUESIZE].virtAddr;
			unsigned w=m_bufQue[queueid].bufInfo[(info->currentbufid-1+IMAGEQUEUESIZE)%IMAGEQUEUESIZE].width;
			unsigned h=m_bufQue[queueid].bufInfo[(info->currentbufid-1+IMAGEQUEUESIZE)%IMAGEQUEUESIZE].height;
			double angle=m_bufQue[queueid].bufInfo[(info->currentbufid-1+IMAGEQUEUESIZE)%IMAGEQUEUESIZE].framegyroyaw*1.0/ANGLESCALE;
			setpreangle(angle);
			pre=Mat(h,w,CV_8UC3,data);
			//imshow("t",pre);
			//waitKey(2);
			setpreframe(pre);
			}
		else
			setpreframeflag(0);
		/*****************************************/
		double exec_time = (double)getTickCount();

		/*****************current angle  no feature*********************/
		double angle=0;
		if(FEATURESTICH)
		{
			
		}
		else	
		angle=infocap->framegyroyaw*1.0/ANGLESCALE;

		setgyroangle(angle);
		
		//OSA_printf("the algle is %f\n",angle);
		imageangle=angle+getcamerazeroossfet();
		if(imageangle<0)
			imageangle+=360;
		else if(imageangle>=360)
			imageangle-=360;
		
		setcurrentangle(imageangle);

		/*****************************************/

		#if 0
		if(infocap->width!=PANO360WIDTH||infocap->height!=PANO360HEIGHT)
			resize(src,dst,Size(PANO360WIDTH,PANO360HEIGHT),0,0,INTER_LINEAR);
		else
		memcpy(info->virtAddr,infocap->virtAddr,infocap->width*infocap->height*infocap->channels);
		#endif
		//setSeamPos(rotsita*panowidth/360-getPanoPrepos());
		/********************zero*********************/
		setcurrentflame(src);
		if(getzeroflameupdate())
			{
				setzeroflameupdate(0);
				setzeroflame(src);
				imwrite("zero.jpg",src);
				setzeroangle(getcurrentangle());
			}
		
		zeroprocess();
		/*****************************************/
		Panoprocess(src,dst);
		
		exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
	 	//OSA_printf("the exec_time=%f MS\n",exec_time);
		
		info->channels = infocap->channels;
		info->width =PANO360WIDTH;
		info->height =PANO360HEIGHT;
		info->timestamp =infocap->timestamp;
		info->framegyroroll=infocap->framegyroroll;
		info->framegyropitch=infocap->framegyropitch;
		if(FEATURESTICH)
				info->framegyroyaw=getcurrentangle();
		else
				info->framegyroyaw=getcurrentangle()*ANGLESCALE;//(+ANGLESHIFT*ANGLESCALE);
	

		//OSA_printf("the framegyroyaw=%d MS\n",info->framegyroyaw);
		

		
		 image_queue_putFull(&m_bufQue[SIGNALEQUEUEID], info);
		 
		 image_queue_putEmpty(&mcap_bufQue[queueid],infocap);
		//OSA_printf("2****************************\n");

	}
}

