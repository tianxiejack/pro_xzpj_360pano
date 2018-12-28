#include"videorecord.hpp"
#include<stdio.h>
#include <queue>

VideoRecord* VideoRecord::instance=NULL;
#define SAVEDIR "/home/ubuntu/calib"
VideoRecord::VideoRecord():timeenable(1),eventenable(0),tm_year(0),tm_mon(0),tm_mday(0),tm_hour(0),tm_min(0),tm_sec(0),videorecordfb(NULL)
{


}
VideoRecord::~VideoRecord()
{


}

void VideoRecord::create()
{
	_syncdata=new queue<SyncDate>;
	
}
void VideoRecord::recordvideo(void *data,void* size)
{
	//printf("data=%p size=%d instance=%p\n",data,size,instance);
	if(data==NULL||size==NULL||instance==NULL)
		return ;
	//return;
	char filename[64];
	char filedataname[64];
	struct tm tm_set;
	struct timeval tv_set;
	struct timezone tz_set;
	time_t tt_set=0;
	gettimeofday(&tv_set, &tz_set);
	tt_set = tv_set.tv_sec;
	memcpy(&tm_set, localtime(&tt_set), sizeof(struct tm));

	SyncDate syncdata;
	memset(&syncdata,0,sizeof(SyncDate));
	int filewriteenable=0;

	char *videodata=(char *)data;
	int videolen=*(int*)size;
	if(videolen>10)
		filewriteenable=1;
	if(filewriteenable)
	instance->getsync(&syncdata);

	//printf("the  size=%d \n",videolen);

	int year=tm_set.tm_year+1900;
	int mon=tm_set.tm_mon+1;
	int day=tm_set.tm_mday;
	int hour=tm_set.tm_hour;
	int mint=tm_set.tm_min;
	int sect=tm_set.tm_sec;
	//printf("instance->getrecordflag()=%d\n",instance->getrecordflag());
	if(instance->getrecordflag()==0)
		{
			if(instance->videorecordfb!=NULL)
				{
					fclose(instance->videorecordfb);
					instance->mydata.close();
					instance->videorecordfb=NULL;

				}
			return ;
		}
		if(instance->videorecordfb == NULL)
		{
			
			sprintf(filename, "%s/local_%04d%02d%02d-%02d%02d%02d.avi", 
				SAVEDIR, 
				year, mon, day,
				hour, mint,sect);
			sprintf(filedataname, "%s/local_%04d%02d%02d-%02d%02d%02d.xml", 
				SAVEDIR, 
				year, mon, day,
				hour, mint,sect);
			instance->tm_year=year;
			instance->tm_mon=mon;
			instance->tm_mday=day;
			instance->tm_hour=hour;
			instance->tm_min=mint;
			instance->tm_sec=sect;
			
			instance->videorecordfb = fopen(filename,"wb");
			instance->mydata.open(filedataname);
			instance->g_gst_wrPkt = 0;
			printf(" open local file %s\n", filename);
		}
		if((year!=instance->tm_year||instance->tm_mon!=mon||instance->tm_mday!=day||instance->tm_hour!=hour)&&(videolen<10))
			{
				fclose(instance->videorecordfb);
				instance->mydata.close();
				instance->videorecordfb=NULL;
				if(instance->videorecordfb == NULL)
					{
						
						sprintf(filename, "%s/local_%04d%02d%02d-%02d%02d%02d.avi", 
							SAVEDIR, 
							year, mon, day,
							hour, mint,sect);
						sprintf(filedataname, "%s/local_%04d%02d%02d-%02d%02d%02d.xml", 
							SAVEDIR, 
							year, mon, day,
							hour, mint,sect);
						instance->tm_year=year;
						instance->tm_mon=mon;
						instance->tm_mday=day;
						instance->tm_hour=hour;
						instance->tm_min=mint;
						instance->tm_sec=sect;
						
						instance->videorecordfb = fopen(filename,"wb");
						instance->mydata.open(filedataname);
						instance->g_gst_wrPkt = 0;
						printf(" open local file %s\n", filename);
					}

			}

		if(instance->videorecordfb != NULL)
		{
			fwrite(videodata, videolen, 1, instance->videorecordfb);
			if(filewriteenable)
			instance->mydata.write(syncdata.event, syncdata.gyroX, syncdata.gyroY, syncdata.gyroZ);
			instance->g_gst_wrPkt++;
			if((instance->g_gst_wrPkt % 50) == 0)
			{
				fflush(instance->videorecordfb);
			}
		}


	
	

}

void VideoRecord::putsync(void *data)
{
	if(data==NULL)
		return;
	SyncDate syncdata=*(SyncDate*)data;
	_syncdata->push(syncdata);
}


void VideoRecord::getsync(void *data)
{
	SyncDate*  syncdata=(SyncDate*)data;
	if(data==NULL)
		return;
	if(_syncdata->empty())
		return;
	*syncdata=_syncdata->front();
	_syncdata->pop();

	//printf("%s queue size=%d\n",__func__,_syncdata->size());

}


VideoRecord* VideoRecord::getinstance()
{
	if(instance==NULL)
		instance=new VideoRecord();
	return instance;

}
