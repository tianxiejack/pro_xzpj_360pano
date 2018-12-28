#include"Status.hpp"
#include "stdio.h"
Status* Status::instance=NULL;

Status::Status():displaymod(0)
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
