#include"Status.hpp"
#include "stdio.h"
Status* Status::instance=NULL;

Status::Status():displaymod(0),ptzpanspeed(0),ptztitlespeed(0),ptzpanodirection(0),ptztitledirection(0),workmode(-1),mouseevent(-1),mousex(-1),mousey(-1),mousemod(-1)
{


}
Status::~Status()
{


}
Status* Status::getinstance()
{
	if(instance==NULL)
		instance=new Status();
	return instance;

}
