/*
 * ImageProcess.hpp
 *
 *  Created on: 2018年9月29日
 *      Author: wj
 */

#ifndef IMAGEPROCESS_HPP_
#define IMAGEPROCESS_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>
#include"osa.h"
#include"osa_sem.h"
#include"osa_thr.h"
#include"osa_mutex.h"
#include"osa_buf.h"
#include"config.h"
using namespace cv;
//#define DS_CHAN_MAX         (4)
typedef struct
{
	double intervalsita;
	int validintervalsita;

}FrameINFO;

#define FRAMEFIFO 2
typedef struct _main_thr_obj_cxt{
	bool bTrack;
	bool bMtd;
	bool bBlobDetect;
	int chId;
	int iTrackStat;
	
	//Mat frame;
}MAIN_ProcThrObj_cxt;

typedef struct _main_thr_obj{
	MAIN_ProcThrObj_cxt cxt[2];
	OSA_ThrHndl		thrHandleProc;
	OSA_SemHndl	procNotifySem;
	int pp;
	bool bFirst;
	volatile bool	exitProcThread;
	bool						initFlag;
	void 						*pParent;
}MAIN_ProcThrObj;

#define ZEROJUEGE 4
class ImageProcess
{
public:
	ImageProcess();
	~ImageProcess();

	void Init();
	void unInit();
	void CaptureThreadProcess(Mat src,OSA_BufInfo* frameinfo);


public:
		MAIN_ProcThrObj	mainProcThrObj;
		Mat processgray[FRAMEFIFO];
		Mat processtest;
		Mat processgraytemp[FRAMEFIFO];
		unsigned int currentcount;
		unsigned int pinpang;
		void setImagePinpang(unsigned int id){pinpang=id;};
		void setnextImagePinpang(){pinpang=(pinpang+1)%FRAMEFIFO;};
		unsigned int getImagePinpang(){return pinpang;};
		unsigned int getImagePrePinpang(){return (FRAMEFIFO+pinpang-1)%FRAMEFIFO;};

		void Panoprocess(Mat src,Mat dst);


		//////////////////ring buffer/////////////////////
		OSA_BufHndl m_bufQue[IMAGEQUEUE];
		OSA_BufHndl mcap_bufQue[IMAGEQUEUE];

		///////////////////FRAME/////////////////////////
		double AngleStich;
		Mat processtemp;
		Mat processtempgray;

		/////////////////////panoprocess////////////////////////
		int xoffsetfeat;
		int yoffsetfeat;
		double preangle;
		double currentangle;
		double getcurrentangle(){return currentangle;};
		void setcurrentangle(double angle){currentangle=angle;};
		double getpreangle(){return preangle;};
		void setpreangle(double angle){preangle=angle;};
		void OptiSeam();
			/******************seaminit***********************/
		#define MAXSEAM 2
		Mat Seamframe[MAXSEAM];
		Mat fusionframe;
		unsigned int seamid;
		unsigned int SeamEable;
		unsigned int Seampostion;

		unsigned int preflag;
		Mat preframe;
		Mat getpreframe(){return preframe;};
		void setpreframe(Mat frame){preframe=frame;};
		unsigned int  getpreframeflage(){return preflag;};
		void setpreframeflag(int flag){preflag=flag;};

		Mat getCurrentFame(){return Seamframe[seamid%MAXSEAM];};
		Mat getPreFame(){return Seamframe[(seamid+1)%MAXSEAM];};

		unsigned int getcurrentFrameId(){return seamid;};
		void setcurrentFrameId(unsigned int id){ seamid=id;};
		void setNextFrameId(){seamid=(seamid+1)%MAXSEAM;};
		
		bool getSeamEnable(){return SeamEable;};
		void setSeamEnable(bool enable){SeamEable=enable;};
		
		unsigned int getSeamPos(){return Seampostion;};
		void setSeamPos(unsigned int sem){ Seampostion=sem;};


		
		
		/********angle********/
		double gyroangle;
		void setgyroangle(double flag){gyroangle=flag;};
		double getgyroangle(){return gyroangle;};
		

		/********camrea********/
		double camerazeroossfet;
		void setcamerazeroossfet(double flag){camerazeroossfet=flag;};
		double getcamerazeroossfet(){return camerazeroossfet;};


		/********zero********/
		int zeroflameupdate;
		int zeroflag;
		int zerocalibflag;
		double zeroangle;

		void setzeroflameupdate(int flag){zeroflameupdate=flag;};
		int getzeroflameupdate(){return zeroflameupdate;};

		void setzerocalib(int flag){zerocalibflag=flag;};
		int getzerocalib(){return zerocalibflag;};
		void setzeroangle(double flag){zeroangle=flag;};
		double getzeroangle(){return zeroangle;};
		void setzeroflag(int flag){zeroflag=flag;};
		int getzeroflag(){return zeroflag;};
		void zeroprocess();
		int judgezero();


		/********frame********/
		Mat zeroflame;\
		Mat getzeroflame(){return zeroflame;};
		void setzeroflame(Mat flame){memcpy(zeroflame.data,flame.data,flame.cols*flame.rows*flame.channels());};

		Mat currentflame;
		Mat getcurrentflame(){return currentflame;};
		void setcurrentflame(Mat flame){currentflame=flame;};
	
		
private:
	OSA_MutexHndl m_mutex;
//	unsigned char *m_grayMem[2];

	void main_proc_func();
	int MAIN_threadCreate(void);
	int MAIN_threadDestroy(void);
	static void *mainProcTsk(void *context)
	{
		MAIN_ProcThrObj  * pObj= (MAIN_ProcThrObj*) context;
		if(pObj==NULL)
			{

			printf("++++++++++++++++++++++++++\n");

			}
		ImageProcess *ctxHdl = (ImageProcess *) pObj->pParent;
		ctxHdl->main_proc_func();
		printf("****************************************************\n");
		return NULL;
	}
		

};


#endif /* IMAGEPROCESS_HPP_ */
