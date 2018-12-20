/*
 * videoload.hpp
 *
 *  Created on: Dec 19, 2018
 *      Author: wj
 */

#ifndef VIDEOLOAD_HPP_
#define VIDEOLOAD_HPP_
#include <osa_thr.h>
#include"osa_sem.h"
#include"config.hpp"
#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
typedef struct {
	bool bTrack;
	bool bMtd;
	bool bBlobDetect;
	int chId;
	int iTrackStat;
	
	//Mat frame;
}MAIN_VideoLoadCaptureThrObj_cxt;

typedef struct {
	MAIN_VideoLoadCaptureThrObj_cxt cxt[2];
	OSA_ThrHndl		thrHandleProc;
	OSA_SemHndl	procNotifySem;
	int pp;
	bool bFirst;
	volatile bool	exitProcThread;
	bool						initFlag;
	void 						*pParent;
}MAIN_VideoLoadCaptureThrObj;

using namespace cv;
typedef void (* VideoCallBackfun)(void *data,void *info);
typedef struct{
	int event;
	double gyrox;
	double gyroy;
	double gyroz;
}VideoLoadData;
class MyTDataLoad
{
public :
  MyTDataLoad() :
    event(0), gyrox(0), gyroy(0),gyroz(0)
  {
  }
 
  explicit MyTDataLoad(int t,double x,double y,double z) :
    event(t), gyrox(x), gyroy(y),gyroz(z)
  {
  }
  int event;
  double gyrox;
  double gyroy;
  double gyroz;
  FileNodeIterator fni;
  FileNodeIterator fniEnd;
  FileNodeIterator current; 

   cv::FileStorage filestore;
  void open(char *name)
  	{
		filestore=FileStorage(name, FileStorage::READ);
		FileNode features = filestore["gyro"];
		fni= features.begin();
    	      fniEnd = features.end();
	      current=fni;
  	}

   void close()
  	{
		
		filestore.release();
  	}

 

  
  VideoLoadData read()  //Read serialization for this class
  {
  	//if(current==)
	VideoLoadData data;
	data.event=(int)(*current)["event"];
	data.gyrox=(double)(*current)["gyrox"];
	data.gyroy=(double)(*current)["gyroy"];
	data.gyroz=(double)(*current)["gyroz"];
	if(current==fniEnd)
		current=fni;
	current++;
	
	return data;
	
  	
   
  }
};





class VideoLoad{

	public:
		
		void create();
		void destory();
		static VideoLoad*getinstance();
		void registerfun(VideoCallBackfun fun);
		void putvideoname(char *name){memcpy(videoname,name,60);};
		char * getvideoname(){return videoname;};
	private:
		VideoLoad();
		~VideoLoad();
		static VideoLoad* instance;
		VideoCallBackfun callfun;
		char videoname[60];
		MyTDataLoad mydata;


	public:
		//MyTDataLoad mydata;
		MAIN_VideoLoadCaptureThrObj	mainRecvThrObj;
		void main_Recv_func();
		int MAIN_threadRecvCreate(void);
		int MAIN_threadRecvDestroy(void);
		static void *mainRecvTsk(void *context)
		{
			MAIN_VideoLoadCaptureThrObj  * pObj= (MAIN_VideoLoadCaptureThrObj*) context;
			if(pObj==NULL)
				{

				printf("++++++++++++++++++++++++++\n");

				}
			VideoLoad *ctxHdl = (VideoLoad *) pObj->pParent;
			ctxHdl->main_Recv_func();
			
			return NULL;
		}



};



#endif /* VIDEOLOAD_HPP_ */
