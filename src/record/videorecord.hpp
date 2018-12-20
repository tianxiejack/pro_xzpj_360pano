/*
 * #
 *
 *  Created on: Dec 19, 2018
 *      Author: wj
 */

#ifndef VIDEORECORD_HPP_
#define VIDEORECORD_HPP_
#include "osa.h"
#include <queue>
#include "opencv2/core/core.hpp"
typedef struct {
	double	gyroX;
	double  	gyroY;
	double	gyroZ;
	int event;


}SyncDate;
using namespace std;

using namespace cv;
class MyTData
{
public :
  MyTData() :
    event(0), gyrox(0), gyroy(0),gyroz(0)
  {
  }
 
  explicit MyTData(int t,double x,double y,double z) :
    event(t), gyrox(x), gyroy(y),gyroz(z)
  {
  }
  int event;
  double gyrox;
  double gyroy;
  double gyroz;

   cv::FileStorage filestore;
  void open(char *name)
  	{
		filestore=FileStorage(name, FileStorage::WRITE);
		filestore << "gyro" << "[";
  	}

   void close()
  	{
		filestore << "]";
		filestore.release();
  	}

 
  void write(int event,double gyrox,double gyroy,double gyroz)  //Write serialization for this class
  {
    filestore << "{" << "event" << event << "gyrox" << gyrox << "gyroy" << gyroy << "gyroz" << gyroz << "}";
	// fs << "{"<< "time" << Xspeed  <<  "}";
  }
  
  void read(const FileNode& node)  //Read serialization for this class
  {

    event = (int)node["event"];
    gyrox = (double)node["gyrox"];
    gyroy = (double)node["gyroy"];
    gyroz = (double)node["gyroz"];
   
  }
};
class VideoRecord{

	public:
		void create();
		static VideoRecord*getinstance();
		static void recordvideo(void *data,void* size);
		void putsync(void *data);
		void getsync(void *data);

		void settimerecordenable(int enable){timeenable=enable;};
		void seteventrecordenable(int enable){eventenable=enable;};

		int getrecordflag(){return timeenable||eventenable;};
	private:

		int timeenable;
		int eventenable;
		VideoRecord();
		~VideoRecord();
		static VideoRecord* instance;
		

		 FILE *videorecordfb;
		 MyTData mydata;
		 int tm_year;
		 int tm_mon;
		 int tm_mday;
		 int tm_hour;
		 int tm_min;
		 int tm_sec;
		 int g_gst_wrPkt;
		 queue<SyncDate> *_syncdata;



};



#endif /* VIDEORECORD_HPP_ */
