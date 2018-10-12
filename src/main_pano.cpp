/*
 * main_pano.cpp
 *
 *  Created on: 2018年9月14日
 *      Author: fsmdn113
 */

#include"Gldisplay.hpp"
#include <glew.h>
#include <glut.h>
#include <freeglut_ext.h>
#include "ChosenCaptureGroup.h"
#include "Displayer.hpp"
#include "main.h"
#include "osa_image_queue.h"
#include"Gyroprocess.hpp"
#include "ImageProcess.hpp"
static GLMain render;

ImageProcess *Imageprocesspt;
using namespace cv;
static OSA_BufHndl *imgQ[QUE_CHID_COUNT];
//static OSA_BufHndl *procossQ[QUE_CHID_COUNT];
void processFrame_pano(int cap_chid,unsigned char *src, struct v4l2_buffer capInfo, int format)
{
	char WindowName[64]={0};
	Mat img;
	int queueid=0;
	OSA_BufInfo* info=NULL;
	Mat cap = Mat(TV_HEIGHT,TV_WIDTH,CV_8UC2,src);
	GYRO_DATA_T gyro;
	
	//OSA_printf("%d %s. 1 chid=%d", OSA_getCurTimeInMsec(), __func__,cap_chid);
	if(cap_chid==TV_DEV_ID)
		{
		getGyroprocess(cap,&gyro);
		
		queueid=0;
		info = image_queue_getEmpty(imgQ[queueid]);
		if(info == NULL){
			//info = image_queue_getFull(imgQ[queueid]);
			//OSA_assert(info != NULL);
			return;
			}
		}
	img = Mat(TV_HEIGHT,TV_WIDTH,CV_8UC3, info->virtAddr);
	cvtColor(cap,img,CV_YUV2BGR_YUYV);
	FrameINFO fameinfo;
	
	getEuler(&info->framegyroroll,&info->framegyropitch,&info->framegyroyaw);

	//OSA_printf("the roll =%d pitch=%d yaw=%d\n",info->framegyroroll,info->framegyropitch,info->framegyroyaw);
	//info->rotaionstamp=fameinfo.intervalsita;
	//info->rotationvalid=fameinfo.validintervalsita;
	//OSA_printf("%d %s. 1   valid=%d rot=%d\n", OSA_getCurTimeInMsec(), __func__,info->rotationvalid,info->rotaionstamp);
	
	info->channels = img.channels();
	info->width = img.cols;
	info->height = img.rows;
	info->timestamp = (uint64)capInfo.timestamp.tv_sec*1000*1000*1000
			+ (uint64)capInfo.timestamp.tv_usec*1000;
    	 Imageprocesspt->CaptureThreadProcess(img,info);
	//cv::imshow(WindowName, img);
	//waitKey(1);
	  image_queue_putFull(imgQ[queueid], info);




	
	//OSA_printf("%d %s. 1w=%d h=%d\n", OSA_getCurTimeInMsec(), __func__,info->width,info->height);

}

int main_pano(int argc, char **argv)
{

	GLMain_InitPrm dsInit;
	Imageprocesspt=new ImageProcess();
	Imageprocesspt->Init();
	//procossQ[0]=&Imageprocesspt->mcap_bufQue;
	render.IPocess_bufQue[0]=&Imageprocesspt->m_bufQue[0];
	//memcpy(render.IPocess_bufQue,Imageprocesspt->m_bufQue,sizeof(Imageprocesspt->m_bufQue));
	dsInit.bFullScreen = false;
	//dsInit.keyboardfunc = keyboard_event;
	dsInit.nChannels = 1;
	dsInit.nQueueSize = 3;
	dsInit.memType = memtype_malloc;
	dsInit.channelsSize[0].w = TV_WIDTH;
	dsInit.channelsSize[0].h = TV_HEIGHT;
	dsInit.channelsSize[0].c = 3;
	render.start(argc,  argv,(void *)&dsInit);
	imgQ[0] = &render.m_bufQue[0];
	
	ChosenCaptureGroup *grop[2];
	grop[0] = ChosenCaptureGroup :: GetTVInstance();
	OSA_printf("run app success!\n");
	render.mainloop();
	return 0;
}



