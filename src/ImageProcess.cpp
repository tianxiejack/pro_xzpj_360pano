#include "ImageProcess.hpp"
#include"Homography.hpp"
#include "osa_image_queue.h"
#include"osa.h"
#include"Stich.hpp"
#include"Gyroprocess.hpp"

#define IMAGEQUEUESIZE 4
#define IMAGCAPEQUEUESIZE 2
#define IMAGEQUEUEWIDTH 1920
#define IMAGEQUEUEHEIGHT 1080
#define IMAGEQUEUECHANNEL 3

#define SIGNALEQUEUEID 0

ImageProcess *ImageProcess::Pthis;

ImageProcess::ImageProcess():pinpang(0),currentcount(0),AngleStich(-ANGLEINTREVAL),seamid(0),SeamEable(0),Seampostion(0),
			preflag(0),preangle(0),currentangle(0),xoffsetfeat(0),yoffsetfeat(0),camerazeroossfet(0),zeroflag(0),zeroangle(0),zerocalibflag(1),
			zeroflameupdate(1),gyroangle(0),pp(0),blocknum(0),zeroprocessflag(0),zerocalibing(0)
{
	memset(m_bufQue, 0, sizeof(m_bufQue));
	memset(mcap_bufQue, 0, sizeof(mcap_bufQue));
	Pthis=this;

}
ImageProcess::~ImageProcess()
{


}

void ImageProcess::Init()
{
	//processgray[0]=
	stichinit();
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

	for(int i=0;i<MOVEBLOCKNUM;i++)
		{
			if(PANOGRAYDETECT)
			panoblock[i]=Mat(MOVDETECTSRCHEIGHT,MOVDETECTSRCWIDTH,CV_8UC1,cv::Scalar(0));
			else
			panoblock[i]=Mat(MOVDETECTSRCHEIGHT,MOVDETECTSRCWIDTH,CV_8UC3,cv::Scalar(0));	
		}
	for(int i=0;i<2;i++)
	MvtestFRrame[i]=Mat(PANO360HEIGHT,PANO360WIDTH,CV_8UC1,cv::Scalar(0));
	panograysrc=Mat(PANO360HEIGHT,PANO360WIDTH,CV_8UC1,cv::Scalar(0));

	panoblockdown=Mat(PANO360HEIGHT/MOVDETECTDOW,MOVDETECTSRCWIDTH/(MOVDETECTDOW),CV_8UC1,cv::Scalar(0));
	
	MAIN_threadCreate();
	MAIN_detectthreadCreate();
}
void ImageProcess::Create()
{

	char bufname[50];
	m_pMovDetector=MvDetector_Create();
	m_pMovDetector->init(NotifyFunc,( void *) this);
	
	double rate = 2.0;
	Size videoSize(MOVDETECTSRCWIDTH/MOVDETECTDOW,MOVDETECTSRCHEIGHT/MOVDETECTDOW);
	//videowriter=VideoWriter("mov.avi", CV_FOURCC('M', 'J', 'P', 'G'), rate, videoSize);
	//bool status=videowriter.open("mov.avi", CV_FOURCC('X', 'V', 'I', 'D'),rate, videoSize, false);
	for(int i=0;i<MULTICPUPANONUM;i++)
		{
			sprintf(bufname,"/home/ubuntu/calib/mov%d.avi",i);
			videowriter[i].open(bufname, CV_FOURCC('M', 'J', 'P', 'G'),rate, videoSize, false);
		}
	
	OSA_printf("***********Create* end=**************\n");
	
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
	if(getzeroflameupdate()==0)
	if(abs(frameinfo->framegyroyaw*1.0/ANGLESCALE-AngleStich)<ANGLEINTREVAL&&(getzerocalibing()==0))
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

	AngleStich=frameinfo->framegyroyaw*1.0/ANGLESCALE;


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
int ImageProcess::MAIN_detectthreadCreate(void)
{
	int iRet = OSA_SOK;
	iRet = OSA_semCreate(&mainProcThrdetectObj.procNotifySem ,1,0) ;
	OSA_assert(iRet == OSA_SOK);


	mainProcThrdetectObj.exitProcThread = false;

	mainProcThrdetectObj.initFlag = true;

	mainProcThrdetectObj.pParent = (void*)this;

	iRet = OSA_thrCreate(&mainProcThrdetectObj.thrHandleProc, maindetectTsk, 0, 0, &mainProcThrdetectObj);

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
int ImageProcess::MAIN_detectthreadDestroy(void)
{
	int iRet = OSA_SOK;

	mainProcThrdetectObj.exitProcThread = true;
	OSA_semSignal(&mainProcThrdetectObj.procNotifySem);

	iRet = OSA_thrDelete(&mainProcThrdetectObj.thrHandleProc);

	mainProcThrdetectObj.initFlag = false;
	OSA_semDelete(&mainProcThrdetectObj.procNotifySem);

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
	setzeroprocessflag(0);
	judgezero();
	int zerooffset=0;
	
	int zerobool=getzeroflag();
	int calibool=getzerocalib();
	if(zerobool==0||calibool==1)
		{
		setzerocalibing(0);
		return ;
		}
	setzerocalibing(1);
	Mat zeroF=getzeroflame();
	Mat currentF=getcurrentflame();

	double currentgyroA=getgyroangle();
	if(currentgyroA>300)
		currentgyroA-=360;
	double zeroA=getzeroangle();
	gyroangleoffset=currentgyroA-zeroA;

	//setyawbase();
	if(zerocalibration(zeroF,currentF,&angle,&zerooffset))
		{
			
			//setcamerazeroossfet(angle-(currentgyroA-zeroA));
			setcurrentangle(0);
			setzerocaliboffset(zerooffset);
			setzeroprocessflag(1);

			
			setyawbase(gyroangleoffset-angle);
			setzerocalib(1);
			zeroprocessflag=1;
			printf("********basioffset=%f********zeroA=%f  gyroangleoffset=%f currentgyroA=%d \n",gyroangleoffset-angle,zeroA,gyroangleoffset,currentgyroA);
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

	/*******************************/
	int flag=getzeroprocessflag();
	int getoffst=getzerocaliboffset();
	if(flag==1)
		{
			Matcpy(src,src,abs(getoffst));
			Matblack(src,src.cols-abs(getoffst));
			
		}


	/******************************/
	
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
		cylinder(temp,dst,1.0*(CAMERAFOCUSLENGTH)*PANO360WIDTH/PANO360SRCWIDTH,PANOSRCSHIFT);

		

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
			//if((getcurrentangle()<360-ZEROJUEGE)&&(getcurrentangle()>0))
				FusionSeam(pre,dst,getSeamPos());
			//	histequision(dst);
				//equalizeHist(dst, dst);
			//OSA_printf("the preoffset=%d curoffset=%d\n",preoffset,curoffset);
		}

	/***********detect process **********************/
	//if(getcurrentangle()>360-ZEROJUEGE)
	//	return ;
      if(MULTICPUPANO)
	Multicpupanoprocess(dst);
	 else
	cpupanoprocess(dst);


	
	
	

}


void ImageProcess::NotifyFunc(void *context, int chId)
{
	std::vector<cv::Rect>	detect;
	ImageProcess *pParent = (ImageProcess*)context;
	pParent->m_pMovDetector->getMoveTarget(pParent->detect_vect,chId);
	for(int i=0;i<pParent->detect_vect.size();i++)
		{
			detect.push_back(pParent->detect_vect[i].targetRect);
		}
	setmvdetect(detect,chId);
	//if()
}
#define MINAREDETECT (1500)
void ImageProcess::panomoveprocess()
{
	Mat process;
	char recoardnum[50];
	int blocknum=movblocknum;
	if(DETECTTEST)
		{
			if(MOVDETECTDOWENABLE)
			resize(MvtestFRrame[pp^1],MvtestFRrame[pp^1],Size(PANO360WIDTH/MOVDETECTDOW,PANO360HEIGHT/MOVDETECTDOW),0,0,INTER_LINEAR);
			
			m_pMovDetector->setFrame(MvtestFRrame[pp^1],MvtestFRrame[pp^1].cols,MvtestFRrame[pp^1].rows,0,10,MINAREDETECT,200000,30);
			return ;
		}
	if(MULTICPUPANO)
		{

			for(int i=0;i<=MULDETECTTRAIN;i++)
				{
					blocknum=(blocknum+MULTICPUPANONUM-i)%MULTICPUPANONUM;
					if(MOVDETECTDOWENABLE)
						{
							resize(panoblock[blocknum],panoblockdown,Size(MOVDETECTSRCWIDTH/MOVDETECTDOW,MOVDETECTSRCHEIGHT/MOVDETECTDOW),0,0,INTER_LINEAR);
							process=panoblockdown;
						}
					else
						process=panoblock[blocknum];
					//sprintf(recoardnum,"%d.bmp",blocknum);
					//imwrite(recoardnum,process);
					//imshow(recoardnum,process);
					//waitKey(1);
					//if(blocknum==0||blocknum==1)
						{
						//cout<<"************panomoveprocess*************"<<endl;
						videowriter[blocknum]<<process;
						m_pMovDetector->setFrame(process,process.cols,process.rows,blocknum,10,MINAREDETECT,200000,30);
				}
				}
			return;
		}
	//videocapture
	Mat pano360=getpanolastmap();
	
#if PANOGRAYDETECT
	if(MOVEBLOCKENABLE)
		{
			pano360(Rect(1920*5+15000,0,PANO360WIDTH*2,PANO360HEIGHT)).copyTo(panoblock[0]);
			if(MOVDETECTDOWENABLE)
				{
					resize(panoblock[0],panoblockdown,Size(PANO360WIDTH*2/MOVDETECTDOW,PANO360HEIGHT/MOVDETECTDOW),0,0,INTER_LINEAR);
					process=panoblockdown;
				}
			else
				process=panoblock[0];
			//videowriter<<process;
			//videowriter.write(process);
			//videowriter<<process;
			//waitKey(1);
			//OSA_printf("***********video write***************\n");
			//VideoWriter
			//imwrite("blockpano.bmp",panoblock[0]);
			m_pMovDetector->setFrame(process,process.cols,process.rows,0,10,MINAREDETECT,200000,30);


		}

#else
	imwrite("pano.bmp",pano360);

	
		
		//m_pMovDetector->setFrame(frame_gray,frame_gray.cols,frame_gray.rows,0,2,8,200,30);

#endif
		

}
void ImageProcess::getnumofpano360image(int startx,int endx,int *texturestart,int *textureend)
{
	
	unsigned int pan360w=0;
	unsigned int pan360h=0;

	getpanoparam(&pan360w,&pan360h);

	int num=ceil(1.0*pan360w/MOVDETECTSRCWIDTH);
	*texturestart=(startx/MOVDETECTSRCWIDTH)%num;
	if(endx%MOVDETECTSRCWIDTH==0)
	  *textureend=*texturestart;
	else
	*textureend=(endx/MOVDETECTSRCWIDTH)%num;
	//if(*texturestart>=num)

}

void ImageProcess::Multicpupanoprocess(Mat& src)
{
	int startnum=0;
	int endnum=0;
	int multipanx=0;
	int multipany=0;
	int multipanw=0;
	int multipanh=0;
	//static int singalstartnum=0;
	unsigned int pan360w=0;
	unsigned int pan360h=0;

	getpanoparam(&pan360w,&pan360h);
	double exec_time = (double)getTickCount();
		Mat processsrc;
	if(PANOGRAYDETECT)
		{
		cvtColor(src,panograysrc,CV_BGR2GRAY);
		processsrc=panograysrc;
		}
	else
		processsrc=src;

	double angle=getcurrentangle();
	int panx=getpanooffet(angle);
	int pany=0;
	int panw=processsrc.cols-PANOSRCSHIFT;
	int panh=processsrc.rows;
	Rect panorio(panx,pany,panw,panh);
	Rect srcrio(0,0,processsrc.cols-PANOSRCSHIFT,processsrc.rows);

	
	
	getnumofpano360image(panx,panx+panw,&startnum,&endnum);

	//cout<<"startnum"<<startnum<<"**endnum"<<endnum<<endl;

	if(startnum==MULTICPUPANONUM-1&&panx+panw>pan360w)
		{
			;
		}
	else if(startnum==endnum)
	{
		multipanx=panx-startnum*MOVDETECTSRCWIDTH;
		multipany=pany;
		multipanw=panw;
		multipanh=panh;
		panorio=Rect(multipanx,multipany,multipanw,multipanh);
		processsrc(srcrio).copyTo(panoblock[startnum](panorio));

	}
	else
	{
		multipanx=panx-startnum*MOVDETECTSRCWIDTH;
		multipany=pany;
		multipanw=MOVDETECTSRCWIDTH-multipanx;
		multipanh=panh;
		//Rect srcrio(0,0,multipanw,processsrc.rows);
		srcrio=Rect(0,0,multipanw,processsrc.rows);
		panorio=Rect(multipanx,multipany,multipanw,multipanh);
		//cout<<"panorio"<<panorio<<endl;
		//cout<<"srcrio"<<srcrio<<endl;
		processsrc(srcrio).copyTo(panoblock[startnum](panorio));


		multipanx=0;
		multipanw=panw-multipanw;
		srcrio=Rect(panw-multipanw,0,multipanw,processsrc.rows);
		panorio=Rect(0,multipany,multipanw,multipanh);
		//cout<<"2panorio"<<panorio<<endl;
		//cout<<"2srcrio"<<srcrio<<endl;
		processsrc(srcrio).copyTo(panoblock[endnum](panorio));


	}
	if(blocknum!=startnum)
		{
			movblocknum=blocknum;
			blocknum=startnum;
			OSA_semSignal(&mainProcThrdetectObj.procNotifySem);
			//OSA_printf("*********detect*OSA_semSignal*movblocknum=%d*******************\n",movblocknum);


		}



	
	
	


	exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
     //  OSA_printf("the %s exec_time=%f\n",__func__,exec_time);
}
void ImageProcess::cpupanoprocess(Mat& src)
{
	double exec_time = (double)getTickCount();
	Mat processsrc;
	if(PANOGRAYDETECT)
		{
		cvtColor(src,panograysrc,CV_BGR2GRAY);
		processsrc=panograysrc;
		}
	else
		processsrc=src;
	static unsigned int cpupanoonce=0;

	Mat cpupano=getpanomap();
	double angle=getcurrentangle();
	int panx=getpanooffet(angle);
	int pany=PANOEXTRAH/2;
	int panw=processsrc.cols-PANOSRCSHIFT;
	int panh=processsrc.rows;
	Rect panorio(panx,pany,panw,panh);
	

	
		
		
		
	Rect srcrio(0,0,processsrc.cols-PANOSRCSHIFT,processsrc.rows);
	unsigned int pan360w=0;
	unsigned int pan360h=0;

	getpanoparam(&pan360w,&pan360h);

	//OSA_printf("**********%s begin********************\n",__func__);

	
	//OSA_printf("");
	//cout<<"panorio"<<panorio<<endl;
	//cout<<"srcrio"<<srcrio<<endl;
	if(panx+panw>pan360w)
		{
			if(cpupanoonce==0)
				{
					setnextnum();
					cpupanoonce=1;
					if(DETECTTEST==0)
					OSA_semSignal(&mainProcThrdetectObj.procNotifySem);
					OSA_printf("*********detect*OSA_semSignal********************\n");
				}

		}
	else
		{
			cpupanoonce=0;
		      processsrc(srcrio).copyTo(cpupano(panorio));
		}

	//OSA_printf("**********%s end********************\n",__func__);
	//
	 exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
     //  OSA_printf("the %s exec_time=%f\n",__func__,exec_time);
	


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


		if(DETECTTEST)
		{
			//src.copyTo(MvtestFRrame[pp]);
			cvtColor(src,MvtestFRrame[pp],CV_BGR2GRAY);
			pp^=1;
			OSA_semSignal(&mainProcThrdetectObj.procNotifySem);
		}
		//OSA_semSignal(&mainProcThrdetectObj.procNotifySem);
		
		
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
		angle=infocap->framegyroyaw*1.0/ANGLESCALE+getcamerazeroossfet();

		//OSA_printf("the algle is %f\n",angle);
		imageangle=angle;
		if(imageangle<0)
			imageangle+=360;
		else if(imageangle>=360)
			imageangle-=360;
		
		setgyroangle(imageangle);
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
				if(getcurrentangle()<ANGLEINTREVAL&&getcurrentangle()>1)
					{
						setzeroflameupdate(0);
						setzeroflame(src);
						imwrite("zero.jpg",src);
						//setgyrozero(getcurrentangle());
						//setcamerazeroossfet(-getcurrentangle());
						
						setcamerazeroossfet(-getcurrentangle());
						setzeroangle(0);
						///////////////zero angle
						setgyroangle(0);
						setcurrentangle(0);
						
					}
				else
					setcurrentangle(0);
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



void ImageProcess::main_detect_func()
{
	OSA_printf("%s: Main Proc Tsk Is Entering...\n",__func__);
	unsigned int framecount=0;
	OSA_BufInfo* info=NULL;
	OSA_BufInfo* infocap=NULL;
	int queueid=0;
	Mat pre;
	int xoffset,yoffset;
	double imageangle=0;
	while(mainProcThrdetectObj.exitProcThread ==  false)
	{
		OSA_semWait(&mainProcThrdetectObj.procNotifySem, OSA_TIMEOUT_FOREVER);
		//OSA_printf("**********%s***********\n",__func__);
		double exec_time = (double)getTickCount();
		panomoveprocess();
		exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
	 	//OSA_printf("the panomoveprocess=%f MS\n",exec_time);
		
		

	}



}
