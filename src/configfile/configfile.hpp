/*
 * configfile.hpp
 *
 *  Created on: Jan 12, 2019
 *      Author: wj
 */

#ifndef CONFIGFILE_HPP_
#define CONFIGFILE_HPP_
#include <osa_file.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
typedef struct
{
	Point point;


}MovDetectAreaPoint;

typedef struct
{
	//Rect rectare;
	MovDetectAreaPoint area[4];
	int movupdown;
	int  detectflag;


}MovDetectAreaRect;
#define MAXSTORE_LINE (100)
class ConfigFile
{

private:
	ConfigFile();
	~ConfigFile();
	int mk_dir(char *dir);
	;
	void filecreate();
	char buffer[100];
	
	static ConfigFile* instance;
private:
	FILE * mvconfigfile;
	MovDetectAreaRect movarearect[MAXSTORE_LINE];
public:
	void setdetectdate(MovDetectAreaRect *data);
	void getdetectdate(MovDetectAreaRect *data);
	void detectload();
	void detectsave();
	void create();
	static ConfigFile *getinstance();
};



#endif /* CONFIGFILE_HPP_ */
