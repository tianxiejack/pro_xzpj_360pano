#include"Comcontrl.hpp"


COM_Contrl *COM_Contrl::instance=NULL;



COM_Contrl::COM_Contrl():udpfd(0)
{

}
COM_Contrl::~COM_Contrl()
{


}
void COM_Contrl::create()
{
	udpfd=UDP.NetOpen();
	UDP.NetBind(udpfd,5560);
}
void COM_Contrl::main_Recv_func()
{
	OSA_printf("%s: Main Proc Tsk Is Entering...\n",__func__);

	while(mainObj.exitProcThread ==  false)
	{
		
			int len=UDP.NetRecv(udpfd,buf,1000);
			for(int j=0;j<len;j++)
				{
					printf("[%d]=%d\t",j,buf[j]);

				}
			printf("n");
		
		
		
	}



}
int COM_Contrl::MAIN_threadRecvCreate(void)
{
	int iRet = OSA_SOK;
	iRet = OSA_semCreate(&mainObj.procNotifySem ,1,0) ;
	OSA_assert(iRet == OSA_SOK);


	mainObj.exitProcThread = false;

	mainObj.initFlag = true;

	mainObj.pParent = (void*)this;

	iRet = OSA_thrCreate(&mainObj.thrHandleProc, mainRecvTsk, 0, 0, &mainObj);
	

	return iRet;
}


int COM_Contrl::MAIN_threadRecvDestroy(void)
{
	int iRet = OSA_SOK;

	mainObj.exitProcThread = true;
	OSA_semSignal(&mainObj.procNotifySem);

	iRet = OSA_thrDelete(&mainObj.thrHandleProc);

	mainObj.initFlag = false;
	OSA_semDelete(&mainObj.procNotifySem);

	return iRet;
}


COM_Contrl * COM_Contrl::getinstance()
{
	if(instance==NULL)
		instance=new COM_Contrl();
	return instance;
}
