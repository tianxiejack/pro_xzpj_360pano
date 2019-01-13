#include"configfile.hpp"
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include"osa.h"
ConfigFile* ConfigFile::instance=NULL;
#define MODE (S_IRWXU | S_IRWXG | S_IRWXO)
#define configdir "/home/ubuntu/config/"

#define detectconfigfile "/home/ubuntu/config/detect.txt"

ConfigFile::ConfigFile():mvconfigfile(NULL)
{
	memset(movarearect,0,sizeof(MovDetectAreaRect)*16);
}

ConfigFile::~ConfigFile()
{


}

int ConfigFile::mk_dir(char *dir)
{

    DIR *mydir = NULL;
    if((mydir= opendir(dir))==NULL)
    {
      int ret = mkdir(dir, MODE);
      if (ret != 0)
      {
          return -1;
      }
      printf("%s created sucess!/n", dir);
    }
    else
    {
        printf("%s exist!/n", dir);
    }
 
    return 0;
}

void ConfigFile::setdetectdate(MovDetectAreaRect *data)
{
	if(data==NULL)
		return ;
	for(int i=0;i<16;i++)
		{
			movarearect[i]=data[i];
		}
}
void ConfigFile::getdetectdate(MovDetectAreaRect *data)
{
	if(data==NULL)
		return ;
	for(int i=0;i<16;i++)
		{
			data[i]=movarearect[i];
		}
}
void ConfigFile::detectload()
{


	//OSA_mutexLock(&filelock);
	mvconfigfile=fopen(detectconfigfile,"r");
	if (mvconfigfile==NULL)			
		{
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Open Failed.\n");
			return;
		}
	int id=0;
	if(mvconfigfile!=NULL)
		{
		
			while(!feof(mvconfigfile))
				{
					fgets(buffer,MAXSTORE_LINE,mvconfigfile);
					sscanf(buffer,"%d:%d,%d:%d,%d:%d,%d:%d,%d:",&movarearect[id].detectflag,&movarearect[id].area[0].point.x,&movarearect[id].area[0].point.y,\
						&movarearect[id].area[1].point.x,&movarearect[id].area[1].point.y,&movarearect[id].area[2].point.x,&movarearect[id].area[2].point.y,\
						&movarearect[id].area[3].point.x,&movarearect[id].area[3].point.y);
				
					id++;
				}
		
		}


	if(mvconfigfile!=NULL)
		fclose(mvconfigfile);
}

void ConfigFile::detectsave()
{

	
	if ((mvconfigfile=fopen(detectconfigfile,"w"))==NULL)			
		{
			printf("Open Failed.\n");
			return;
		} 

	for(int id=0;id<16;id++)
		{
			sprintf(buffer,"%d:%d,%d:%d,%d:%d,%d:%d,%d:",movarearect[id].detectflag,movarearect[id].area[0].point.x,movarearect[id].area[0].point.y,\
						movarearect[id].area[1].point.x,movarearect[id].area[1].point.y,movarearect[id].area[2].point.x,movarearect[id].area[2].point.y,\
						movarearect[id].area[3].point.x,movarearect[id].area[3].point.y);
			if(mvconfigfile!=NULL)
	  			fprintf(mvconfigfile,"%s\n",buffer);
			else 
				break;
			fflush(mvconfigfile);


		}
	if(mvconfigfile!=NULL)
		fclose(mvconfigfile);
	

}
void ConfigFile::filecreate()
{
	
	

}
void ConfigFile::create()
{
	mk_dir(configdir);
	filecreate();
	

}
ConfigFile *ConfigFile::getinstance()
{
	if(instance==NULL)
		instance= new ConfigFile;

	return instance;

}
