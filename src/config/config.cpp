#include "config.hpp"
#include "stdio.h"
#include <iostream>
#include <string>
#include "config.h"
#include "opencv2/core/core.hpp"
using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::ostream;
using namespace cv;
FileStorage configfilestore;
#define CONFIGFILENAME "./config.xml"


Config *Config::instance=new Config();

Config::Config():panozeroptz(0)
{
	#if CONFIGINIT
	configfilestore=FileStorage(CONFIGFILENAME, FileStorage::WRITE);
	#else
	configfilestore=FileStorage(CONFIGFILENAME, FileStorage::READ);
	#endif


}
Config::~Config()
{

	configfilestore.release();

}



void Config::SaveConfig()
{
	configfilestore.release();
	configfilestore=FileStorage(CONFIGFILENAME, FileStorage::WRITE);
	saveconfig();
	configfilestore.release();

}
void Config::saveconfig()
{
	
	configfilestore <<"camera" << "{" << "cam_width" << CAPTURETVWIDTH << "cam_height" << CAPTURETVHEIGHT << "cam_channel" << 3 << "cam_fx" << CAMERAFOCUSLENGTH << "cam_fy" << CAMERAFOCUSLENGTH<< "cam_ox" \
		<< CAPTURETVWIDTH/2<< "cam_oy" << CAPTURETVHEIGHT/2<< "cam_k1" <<0 << "cam_k2" << 0<< "cam_k3" << 0<< "cam_k4" << 0<< "}";

	configfilestore <<"display" << "{" << "display_width" << CAPTURETVWIDTH << "display_height" << CAPTURETVHEIGHT << "display_channel" << 3 << "}";

	configfilestore <<"panoalg" << "{" << "panoprocesswidth" << PANO360WIDTH << "panoprocessheight" << PANO360HEIGHT  <<"panozeroptz"<< panozeroptz << "}";

	configfilestore <<"mvdetect" << "{" << "mvprocesswidth" << MOVDETECTSRCWIDTH << "mvprocessheight" << MOVDETECTSRCHEIGHT  <<"mvdownup"<<MOVDETECTDOW<< "}";

	configfilestore <<"ptz" << "{" << "ptzwait" << PTZWAIT << "ptzbroad" << PTZBOARD  <<"ptzaddres"<<PTZADDR<<"ptzdp"<<PTZDP<< "}";

	
	
	configfilestore.release();
	
}
void Config::loadconfig()
{
	FileNode struct_node = configfilestore["camera"];
	cam_width=(int )struct_node["cam_width"];
	cam_height=(int )struct_node["cam_height"];
	cam_channel=(int )struct_node["cam_channel"];
	cam_fx=(double )struct_node["cam_fx"];
	cam_fy=(double )struct_node["cam_fy"];
	cam_ox=(double )struct_node["cam_ox"];
	cam_oy=(double )struct_node["cam_oy"];
	cam_k1=(double )struct_node["cam_k1"];
	cam_k2=(double )struct_node["cam_k2"];
	cam_k3=(double )struct_node["cam_k3"];
	cam_k4=(double )struct_node["cam_k4"];
	
	struct_node = configfilestore["display"];
	display_width=(int )struct_node["display_width"];
	display_height=(int )struct_node["display_height"];
	display_channel=(int )struct_node["display_channel"];
	
	struct_node = configfilestore["panoalg"];
	panoprocesswidth=(int )struct_node["panoprocesswidth"];
	panoprocessheight=(int )struct_node["panoprocessheight"];
	panozeroptz=(double)struct_node["panozeroptz"];

	struct_node = configfilestore["mvdetect"];
	mvprocesswidth=(int )struct_node["mvprocesswidth"];
	mvprocessheight=(int )struct_node["mvprocessheight"];
	mvdownup=(int )struct_node["mvdownup"];

	struct_node = configfilestore["ptz"];
	ptzwait=(int )struct_node["ptzwait"];
	ptzbroad=(int )struct_node["ptzbroad"];
	ptzaddres=(int )struct_node["ptzaddres"];
	ptzdp=(int )struct_node["ptzdp"];
	

	printf("%s c w=%d c h=%d  d w=%d d h=%d",__func__,cam_width,cam_height,display_width,display_height);
	

}
Config *Config::getinstance()
{
	if(instance==NULL)
		instance=new Config();

	return instance;


}








 
  