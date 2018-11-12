/*
 * StichAlg.hpp
 *
 *  Created on: 2018年11月12日
 *      Author: wj
 */

#ifndef STICHALG_HPP_
#define STICHALG_HPP_
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

typedef struct {
	bool bTrack;
	bool bMtd;
	bool bBlobDetect;
	int chId;
	int iTrackStat;
	
	//Mat frame;
}MAIN_ProcThrStichObj_cxt;

typedef struct {
	MAIN_ProcThrStichObj_cxt cxt[2];
	OSA_ThrHndl		thrHandleProc;
	OSA_SemHndl	procNotifySem;
	int pp;
	bool bFirst;
	volatile bool	exitProcThread;
	bool						initFlag;
	void 						*pParent;
}MAIN_ProcThrStichObj;

class StichAlg 
{


	public :
		void create();

		
		static StichAlg *getinstance();
	private:
		
		double currentangle;
		double getcurrentangle(){return currentangle;};
		void setcurrentangle(double angle){currentangle=angle;};

			/********camrea********/
		double camerazeroossfet;
		void setcamerazeroossfet(double flag){camerazeroossfet=flag;};
		double getcamerazeroossfet(){return camerazeroossfet;};


		Mat currentflame;
		Mat getcurrentflame(){return currentflame;};
		void setcurrentflame(Mat flame){currentflame=flame;};


		Mat preframe;
		Mat getpreframe(){return preframe;};
		void setpreframe(Mat frame){preframe=frame;};

		double preangle;
		double getpreangle(){return preangle;};
		void setpreangle(double angle){preangle=angle;};


		unsigned int preflag;
		unsigned int  getpreframeflage(){return preflag;};
		void setpreframeflag(int flag){preflag=flag;};


	private:

		
		static StichAlg*instance;
		StichAlg();
		~StichAlg();

		MAIN_ProcThrStichObj	mainProcThrObj;
		void main_proc_func();
		int MAIN_threadCreate(void);
		int MAIN_threadDestroy(void);
		static void *mainProcTsk(void *context)
		{
			MAIN_ProcThrStichObj  * pObj= (MAIN_ProcThrStichObj*) context;
			if(pObj==NULL)
				{

				printf("error \n");

				}
			StichAlg *ctxHdl = (StichAlg *) pObj->pParent;
			ctxHdl->main_proc_func();
		
			return NULL;
		}



};


#endif /* STICHALG_HPP_ */
