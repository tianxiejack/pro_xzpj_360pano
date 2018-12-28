#include"RecordManager.hpp"
#include <stdio.h>
#include <iostream>
#include "fstream"  
#include <dirent.h>
#include <stdio.h>
#include <string>
#include"videoload.hpp"
#include "DxTimer.hpp"

RecordManager*RecordManager::instance=NULL;
#define AVHEAD "local"
#define AVTAIL ".avi"
#define FILETAIL ".xml"
#define AVCONTENC "/home/ubuntu/calib/"


RecordManager::RecordManager():recordpath(AVCONTENC),nextvideoid(0),createplayertimeid(0),playertimer(60),enableplay(0)
{

}
RecordManager::~RecordManager()
{


}

void RecordManager::create()
{
	findrecordnames();
	createplayertimer();
	

}
void RecordManager::recordplaycallback(void *arg)
{
	
	//printf("%s\n",__func__);
	if(instance==NULL)
		return ;
	if(instance->getableplayer())
	VideoLoad::getinstance()->playvideo();
	
}

void RecordManager::setplayertimer(unsigned int timer)
{
	DxTimer::getinstance()->resetTimer(createplayertimeid,timer);

}
void RecordManager::createplayertimer()
{
	createplayertimeid=DxTimer::getinstance()->createTimer();
	DxTimer::getinstance()->registerTimer(createplayertimeid,recordplaycallback,0);
	DxTimer::getinstance()->startTimer(createplayertimeid,playertimer);

}
 bool RecordManager::startsWith(const std::string& str, const std::string& substr)
    {
        return str.find(substr) == 0;
    }

bool RecordManager::endsWith(const std::string& str, const std::string& substr)
    {
        return str.rfind(substr) == (str.length() - substr.length());
    }
void RecordManager::getJustCurrentFile(string  path, vector<string> & video,vector<string> & files)
{

    DIR * dir;
    struct dirent * ptr;
    int i=0;   
    string x,names;
    video.clear();
    files.clear();
    dir = opendir((char *)path.c_str());
    while((ptr = readdir(dir)) != NULL)
    	{

		x=ptr->d_name;
	      names = x;
	      if(startsWith(names,AVHEAD))
	      	{
	      		//cout<<names<<endl;
	      		if(endsWith(names,AVTAIL))
				video.push_back(names);
			else if(endsWith(names,FILETAIL))
				files.push_back(names);

	      	}
		 
	    
    	}
	closedir(dir);
   

   
 }
void RecordManager::findrecordnames()
{
	//string temp;
	getJustCurrentFile(recordpath, recordvideonames,recordfilenames);

}
void RecordManager::getnexvideo()
{	
	int num=recordvideonames.size();
	if(num==0)
		return ;
	nextvideoid++;
	nextvideoid=nextvideoid%num;

	string videname=recordvideonames[nextvideoid];
	VideoLoad::getinstance()->setreadavi(videname);
	cout<<videname<<endl;
	string::iterator begin=videname.end()-4;
	string::iterator end=videname.end();
	videname.erase(begin,end);//+FILETAIL;
	videname=videname+FILETAIL;
	//string filename=videname.erase( 4,4);//+FILETAIL;
	VideoLoad::getinstance()->setreadname(videname);
	VideoLoad::getinstance()->setreadnewfile(1);

	cout<<videname<<endl;

}
RecordManager*RecordManager::getinstance()
{
	if(instance==NULL)
	{
		instance=new RecordManager();
	}
	return instance;
}
