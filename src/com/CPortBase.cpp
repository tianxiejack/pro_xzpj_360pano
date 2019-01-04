#include "CPortBase.hpp"
#include"Status.hpp"

CPortBase::CPortBase()
{
	pM = getpM();
	_globalDate = getDate();
	_StateManager = getStatus();
}

CPortBase::~CPortBase()
{

}

CMessage * CPortBase::getpM()
{
    pM =  CMessage::getInstance();
    return pM;
}

CGlobalDate* CPortBase::getDate()
{
	_globalDate = CGlobalDate::Instance();
	return _globalDate;
}

CStatusManager* CPortBase::getStatus()
{
	_StateManager = CStatusManager::Instance();
	return _StateManager;
}

void CPortBase::EnableSelfTest()
{
    printf("send %s msg\n",__FUNCTION__);
}

void CPortBase::EnableswitchVideoChannel()
{
  	
}

void CPortBase:: selectVideoChannel()
{
   
}

void CPortBase::EnableTrk()
{
   
}
void CPortBase::mouseevent(int event)
{
	if(event==Status::MOUSEBUTTON)
		{
			if(_globalDate->rcvBufQue.at(6)!=Status::getinstance()->mouseleftright)
				Status::getinstance()->mouseleftright=_globalDate->rcvBufQue.at(6);
			if(_globalDate->rcvBufQue.at(7)!=Status::getinstance()->mousebuttonstaus)
				Status::getinstance()->mousebuttonstaus=_globalDate->rcvBufQue.at(7);
			Status::getinstance()->mousex=_globalDate->rcvBufQue.at(8)<<8|_globalDate->rcvBufQue.at(9);
			Status::getinstance()->mousey=_globalDate->rcvBufQue.at(10)<<8|_globalDate->rcvBufQue.at(11);

			
			OSA_printf("%s the x1=%d x2=%d y1=%d y2=%d \n",__func__,_globalDate->rcvBufQue.at(8),_globalDate->rcvBufQue.at(9),
				_globalDate->rcvBufQue.at(10),_globalDate->rcvBufQue.at(11));
			OSA_printf("%s the x=%d y=%d \n",__func__,Status::getinstance()->mousex,Status::getinstance()->mousey);
			
			pM->MSGDRIV_send(MSGID_EXT_INPUT_MouseEvent, (void *)(Status::MOUSEBUTTON));
		}
	else if(event==Status::MOUSEROLLER)
		{
			
			Status::getinstance()->rollerstatus=_globalDate->rcvBufQue.at(6);
			pM->MSGDRIV_send(MSGID_EXT_INPUT_MouseEvent, (void *)(Status::MOUSEROLLER));
		}

	

}

void CPortBase::displaymod()
{
	if(_globalDate->rcvBufQue.at(5)!=Status::getinstance()->getdisplaymod())
		Status::getinstance()->setdisplaymod(_globalDate->rcvBufQue.at(5));
	else
		return;

	if(Status::getinstance()->getdisplaymod()==Status::LIVEMOD)
		pM->MSGDRIV_send(MSGID_EXT_INPUT_DISMOD, (void *)(Status::LIVEMOD));
	else if(Status::getinstance()->getdisplaymod()==Status::PLAYCALLBACK)
		pM->MSGDRIV_send(MSGID_EXT_INPUT_DISMOD, (void *)(Status::PLAYCALLBACK));

	
	

	//OSA_semSignal(&_globalDate->m_semHndl_socket_s);
  
}
void CPortBase::SetResolution()
{

}
void CPortBase::AIMPOS_X()
{

}

void CPortBase::AIMPOS_Y()
{

}

void CPortBase::EnableParamBackToDefault()
{
}
void CPortBase::updatepano()
{
	pM->MSGDRIV_send(MSGID_EXT_INPUT_Updatapano, 0);

}
void CPortBase::AxisMove()
{

}

void CPortBase::EnableTrkSearch()
{

}

void CPortBase::Enablealgosdrect()
{

}

void CPortBase::ZoomSpeedCtrl()
{

}

int CPortBase::ZoomShortCtrl()
{

    return 0;
}

int CPortBase::ZoomLongCtrl()
{

    return 0;
}

void CPortBase::IrisDown()
{

}

void CPortBase::IrisUp()
{

}

void CPortBase::FocusDown()
{

}

void CPortBase::FocusUp()
{

}

void  CPortBase::AvtAxisCtrl()
{
	
}

void CPortBase::EnableOsdbuffer()
{

}

void CPortBase::EnablewordType()
{

}

void CPortBase::EnablewordSize()
{
 
}

void CPortBase::Config_Write_Save()
{
	
}

void CPortBase::Config_Read()
{

}

void CPortBase::EnableSavePro()
{
  
}

void CPortBase::plantctl()
{
	int mod=0;
	int mod1=0;
	int change=0;
	
	if(_globalDate->rcvBufQue.at(5)!=Status::getinstance()->ptzpanodirection)
		{
			Status::getinstance()->ptzpanodirection=_globalDate->rcvBufQue.at(5);
			if(Status::getinstance()->ptzpanodirection==0)
				mod=0;
			else 
				mod++;
			change=1;
		}

	if(_globalDate->rcvBufQue.at(7)!=Status::getinstance()->ptztitledirection)
		{
			Status::getinstance()->ptztitledirection=_globalDate->rcvBufQue.at(7);
			if(Status::getinstance()->ptztitledirection==0)
				mod1=0;
			else 
				mod1++;
			change=1;
		}

	

	

	if(_globalDate->rcvBufQue.at(6)!=Status::getinstance()->ptzpanspeed)
		{
			Status::getinstance()->ptzpanspeed=_globalDate->rcvBufQue.at(6);
			change=1;
		}
	if(_globalDate->rcvBufQue.at(8)!=Status::getinstance()->ptztitlespeed)
		{
			Status::getinstance()->ptztitlespeed=_globalDate->rcvBufQue.at(8);
			change=1;
		}
	OSA_printf("pandirction=%d  titledirection=%d panspeed=%d titlespeed=%d\n",Status::getinstance()->ptzpanodirection,Status::getinstance()->ptztitledirection,Status::getinstance()->ptzpanspeed,
		Status::getinstance()->ptztitlespeed);
	if(change)
		{
			if(mod==0&&mod1==0)
				pM->MSGDRIV_send(MSGID_EXT_INPUT_PLATCTRL, (void *)(Status::PTZTWOMOV));
			else if(mod==1&&mod1==0)
				pM->MSGDRIV_send(MSGID_EXT_INPUT_PLATCTRL, (void *)(Status::PTZPANOMOV));
			else if(mod==0&&mod1==1)
				pM->MSGDRIV_send(MSGID_EXT_INPUT_PLATCTRL, (void *)(Status::PTZTITLEMOV));
			else if(mod==1&&mod1==1)
				pM->MSGDRIV_send(MSGID_EXT_INPUT_PLATCTRL, (void *)(Status::PTZTWOMOV));
		}

	
}

void CPortBase::AXIS_Y()
{
	
	
}

void CPortBase::Preset_Mtd()
{

}

void CPortBase::StoreMode(int mod)
	{
		if(mod==Status::STOREGO)
			{
				if(Status::getinstance()->getstoremod()!=mod)
					Status::getinstance()->setstoremod(mod);

				if(Status::getinstance()->storegonum!=_globalDate->rcvBufQue.at(5))
					{
						
						Status::getinstance()->storegonum=_globalDate->rcvBufQue.at(5);
					}
				
				OSA_printf("the mod=%d storegonum=%d \n ",Status::getinstance()->getstoremod(),Status::getinstance()->storegonum);
				
				pM->MSGDRIV_send(MSGID_EXT_INPUT_StoreMod, (void *)(Status::STOREGO));
			}
		else if(mod==Status::STORESAVE)
			{
				
				if(Status::getinstance()->getstoremod()!=mod)
					Status::getinstance()->setstoremod(mod);
				
				if(Status::getinstance()->storesavenum!=_globalDate->rcvBufQue.at(5))
					{
						Status::getinstance()->storesavenum=_globalDate->rcvBufQue.at(5);
					}
				
				if(Status::getinstance()->storesavemod!=_globalDate->rcvBufQue.at(6))
					{
						Status::getinstance()->storesavemod=_globalDate->rcvBufQue.at(6);
					}
				
				OSA_printf("the mod=%d savenum=%d mod=%d\n ",Status::getinstance()->getstoremod(),Status::getinstance()->storesavenum,Status::getinstance()->storesavemod);

				pM->MSGDRIV_send(MSGID_EXT_INPUT_StoreMod, (void *)(Status::STORESAVE));
			}
		

	};

void CPortBase::workMode()
{

	if(_globalDate->rcvBufQue.at(5)!=Status::getinstance()->getworkmod())
		Status::getinstance()->setworkmod(_globalDate->rcvBufQue.at(5));
	else
		return;
	if(Status::getinstance()->getworkmod()==0)
		pM->MSGDRIV_send(MSGID_EXT_INPUT_WorkModeCTRL, (void *)(Status::PANOAUTO));
	else if(Status::getinstance()->getworkmod()==1)
		pM->MSGDRIV_send(MSGID_EXT_INPUT_WorkModeCTRL, (void *)(Status::PANOPTZ));
	else if(Status::getinstance()->getworkmod()==2)
		pM->MSGDRIV_send(MSGID_EXT_INPUT_WorkModeCTRL, (void *)(Status::PANOSELECT));
	
	//PANOAUTO

}
void CPortBase::targetCaptureMode()
{
	
}

int CPortBase::prcRcvFrameBufQue(int method)
{
    int ret =  -1;
    int cmdLength= (_globalDate->rcvBufQue.at(2)|_globalDate->rcvBufQue.at(3)<<8)+5;

    if(cmdLength<6){
        printf("Warning::Invaild frame\r\n");
        _globalDate->rcvBufQue.erase(_globalDate->rcvBufQue.begin(),_globalDate->rcvBufQue.begin()+cmdLength);
        return ret;
    }
    unsigned char checkSum = check_sum(cmdLength);

    if(checkSum== _globalDate->rcvBufQue.at(cmdLength-1))
    {	
    	_globalDate->commode = method;
	printf("the id=%d \n",_globalDate->rcvBufQue.at(4));
        switch(_globalDate->rcvBufQue.at(4))
        {
            case 0x01:
                //startSelfCheak();
                break;
            case 0x02:
                EnableswitchVideoChannel();
                break;
            case 0x03:
                mouseevent(Status::MOUSEBUTTON);
                break;
            case 0x04:
                mouseevent(Status::MOUSEROLLER);
                break;
            case 0x05:
                displaymod();
                break;
            case 0x06:
                workMode();
                break;
		case 0x07:
		   StoreMode(Status::STOREGO);
		   break;
            case 0x08:
                StoreMode(Status::STORESAVE);
                break;
            case 0x09:
                updatepano();
                break;
            case 0x0a:
                AxisMove();
                break;
            case 0x0b:
                EnableTrkSearch();
                break;
            case 0x0d:
                Enablealgosdrect();
                break;
            case 0x11:                             
                ZoomSpeedCtrl();
                break;
            case 0x12:
                ZoomShortCtrl();
                ZoomLongCtrl();
                break;
            case 0x13:
                IrisDown();
                IrisUp();
                break;
            case 0x14:
                FocusDown();
                FocusUp();
                break;
            case 0x15:
                plantctl();
                //AXIS_Y();
                break;
            case 0x16:
                //wait to finish
                break;
            case 0x17:
                //wait to finish
                break;
            case 0x18:
                AvtAxisCtrl();
                break;
            case 0x20:
                EnableOsdbuffer();
                break;
            case 0x21:
                EnablewordType();
                EnablewordSize();
                break;
            case 0x30:
                Config_Write_Save();
                break;
            case 0x31:
                Config_Read();
                break;
            case 0x32:
                //provided by a single server
                break;
            case 0x33:
                //provided by a single server
                break;
            case 0x34:
                EnableSavePro();
                break;
            case 0x35:
                //provided by a single server
                break;
            case 0x36:
                plantctl();
               // AXIS_Y();
                break;
            case 0x41:
            	Preset_Mtd();
            	break;
            case 0x42:
                workMode();
            	break;
            case 0x43:
            	targetCaptureMode();
            	break;
            default:
                printf("INFO: Unknow  Control Command, please check!!!\r\n ");
                ret =0;
                break;
        }
    }
    else
        printf("%s,%d, checksum error:,cal is %02x,recv is %02x\n",__FILE__,__LINE__,checkSum,_globalDate->rcvBufQue.at(cmdLength-1));
    _globalDate->rcvBufQue.erase(_globalDate->rcvBufQue.begin(),_globalDate->rcvBufQue.begin()+cmdLength);
    return 1;
}

int  CPortBase::getSendInfo(int  respondId, sendInfo * psendBuf)
{
	printf("respondId = %d\n", respondId);
	switch(respondId){
		case ACK_selfTest:
			//startCheckAnswer(psendBuf);
			break;
		case ACK_wordColor:
			break;
		case   ACK_wordType:
			break;
		case  ACK_wordSize:
			break;
		case  NAK_wordColor:
			break;
		case   NAK_wordType:
			break;
		case  NAK_wordSize:
			break;
		case   ACK_mainVideoStatus:
			mainVedioChannel(psendBuf);
			break;
		case   ACK_Channel_bindingStatus:
			bindVedioChannel(psendBuf);
			break;
		case  ACK_avtTrkStatus:
			trackStatus(psendBuf);
			break;
		case  	ACK_avtTrkType:
			trackTypes(psendBuf);
			break;
		case   ACK_avtErrorOutput:
			trackErrOutput(psendBuf);
			break;
		case  ACK_mmtStatus:
			mutilTargetNoticeStatus(psendBuf);
			break;
		case   ACK_mmtSelectStatus:
			multilTargetNumSelectStatus(psendBuf);
			break;
		case   ACK_EnhStatus:
			imageEnhanceStatus(psendBuf);
			break;
		case   ACK_MtdStatus:
			moveTargetDetectedStat(psendBuf);
			break;
		case   ACK_TrkSearchStatus:
			trackSearchStat(psendBuf);
			break;
		case   ACK_posMoveStatus:
			trackFinetuningStat(psendBuf);
			break;
		case   ACK_moveAxisStatus:
			confirmAxisStat(psendBuf);
			break;
		case   ACK_ElectronicZoomStatus:
			ElectronicZoomStat(psendBuf);
			break;
		case  ACK_picpStatus:
			pictureInPictureStat(psendBuf);
			break;
		case  ACK_VideoChannelStatus:
			vedioTransChannelStat(psendBuf);
			break;
		case  ACK_frameCtrlStatus:
			break;
		case  ACK_compression_quality:
			break;
		case ACK_config_Write:
			settingCmdRespon(psendBuf);
			break;
		case ACK_config_Read:
			readConfigSetting(psendBuf);
			break;
		case ACK_jos_Kboard:
			extExtraInputResponse(psendBuf);
			break;
		case ACK_upgradefw:
			printf("%s,%d, upgradefw response!!!\n",__FILE__,__LINE__);
			upgradefwStat(psendBuf);
			break;
		case ACK_param_todef:
			paramtodef(psendBuf);
			break;
	}
	return 0;
}

void  CPortBase:: mainVedioChannel(sendInfo * spBuf)
{
	int msg_length = 2;
	u_int8_t  mainVedioChannel[msg_length+5];
	mainVedioChannel[4] = 0x04;
	//mainVedioChannel[5]=_globalDate->avt_status.SensorStat;
	package_frame(msg_length, mainVedioChannel);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,mainVedioChannel,msg_length+5);
}

void  CPortBase:: bindVedioChannel(sendInfo * spBuf)
{
	int msg_length = 2;
	u_int8_t  restartCheckAnswer[msg_length+5];
	restartCheckAnswer[4] = 0x05;
	restartCheckAnswer[5]=0x07;
	package_frame(msg_length, restartCheckAnswer);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,restartCheckAnswer,msg_length+5);
}

void  CPortBase:: trackStatus(sendInfo * spBuf)
{
	int msg_length = 2;
	u_int8_t  retrackStatus[msg_length+5];
	unsigned int trkstat = 0;//_globalDate->avt_status.AvtTrkStat;
	if(trkstat == 0)
		trkstat = 1;
	else
		trkstat = 2;
	retrackStatus[4] = 0x06;
	retrackStatus[5]=trkstat;
	package_frame(msg_length, retrackStatus);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,retrackStatus,msg_length+5);
}

void  CPortBase:: trackTypes(sendInfo * spBuf)
{

}

void  CPortBase:: trackErrOutput(sendInfo * spBuf)
{
	int msg_length = 5;
	u_int8_t  retrackErrOutput[msg_length+5];
	retrackErrOutput[4] = 0x08;
	retrackErrOutput[5]= _globalDate->errorOutPut[0];
	retrackErrOutput[5] = retrackErrOutput[5]&0xff;

	retrackErrOutput[6]= _globalDate->errorOutPut[0];
	retrackErrOutput[6] = (retrackErrOutput[6]>>8)&0xff;

	retrackErrOutput[7]= _globalDate->errorOutPut[1];
	retrackErrOutput[7] = retrackErrOutput[7]&0xff;

	retrackErrOutput[8]= _globalDate->errorOutPut[1];
	retrackErrOutput[8] = (retrackErrOutput[8]>>8)&0xff;

	package_frame(msg_length, retrackErrOutput);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,retrackErrOutput,msg_length+5);
}

void  CPortBase:: mutilTargetNoticeStatus(sendInfo * spBuf)
{
	int msg_length = 2;
	channelTable    mutilTargetSta;
	u_int8_t  remutilTargetNoticeStatus[msg_length+5];
	/*
	mutilTargetSta.channel0=_globalDate->avt_status.MtdState[0];
	mutilTargetSta.channel1=_globalDate->avt_status.MtdState[1];
	mutilTargetSta.channel2=_globalDate->avt_status.MtdState[2];
	mutilTargetSta.channel3=_globalDate->avt_status.MtdState[3];
	mutilTargetSta.channel4=_globalDate->avt_status.MtdState[4];
	mutilTargetSta.channel5=_globalDate->avt_status.MtdState[5];
	*/
	remutilTargetNoticeStatus[4] = 0x09;
	remutilTargetNoticeStatus[5]=*(u_int8_t*)(&mutilTargetSta);
	package_frame(msg_length, remutilTargetNoticeStatus);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,remutilTargetNoticeStatus,msg_length+5);
}

void CPortBase::multilTargetNumSelectStatus(sendInfo * spBuf)
{
	int msg_length = 2;
	u_int8_t  tmpbuf[msg_length+5];
	tmpbuf[4] = 0x0a;
	tmpbuf[5]=(u_int8_t) (_globalDate->mainProStat[ACK_mmtSelect_value]&0xff);
	package_frame(msg_length, tmpbuf);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,tmpbuf,msg_length+5);
}

void  CPortBase::imageEnhanceStatus(sendInfo * spBuf)
{
	int msg_length = 2;
	u_int8_t  tmpbuf[msg_length+5];
	channelTable    enhanceChannelSta;
	/*
	enhanceChannelSta.channel0=_globalDate->avt_status.ImgEnhStat[0];
	enhanceChannelSta.channel1=_globalDate->avt_status.ImgEnhStat[1];
	enhanceChannelSta.channel2=_globalDate->avt_status.ImgEnhStat[2];
	enhanceChannelSta.channel3=_globalDate->avt_status.ImgEnhStat[3];
	enhanceChannelSta.channel4=_globalDate->avt_status.ImgEnhStat[4];
	enhanceChannelSta.channel5=_globalDate->avt_status.ImgEnhStat[5];
	*/
	tmpbuf[4] = 0x0b;
	tmpbuf[5]=*(u_int8_t*)(&enhanceChannelSta);;
	package_frame(msg_length, tmpbuf);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,tmpbuf,msg_length+5);
}

void CPortBase::trackFinetuningStat(sendInfo * spBuf)
{
	int msg_length = 3;
	u_int8_t  retrackFinetuningStat[msg_length+5];
	retrackFinetuningStat[4] = 0x0c;
	retrackFinetuningStat[5] = (u_int8_t) (_globalDate->mainProStat[ACK_posMove_value]&0xff);
	retrackFinetuningStat[6] = (u_int8_t) (_globalDate->mainProStat[ACK_posMove_value+1]&0xff);
	package_frame(msg_length, retrackFinetuningStat);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,retrackFinetuningStat,msg_length+5);
}

void CPortBase::confirmAxisStat(sendInfo * spBuf)
{
	int msg_length = 3;
	u_int8_t  tmpbuf[msg_length+5];
	tmpbuf[4] = 0x0d;
	tmpbuf[5]=(u_int8_t) (_globalDate->mainProStat[ACK_moveAxis_value]&0xff);
	tmpbuf[6]=(u_int8_t) (_globalDate->mainProStat[ACK_moveAxis_value+1]&0xff);
	package_frame(msg_length, tmpbuf);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,tmpbuf,msg_length+5);
}

void CPortBase::ElectronicZoomStat(sendInfo * spBuf)
{
	int msg_length = 2;
	u_int8_t  tmpbuf[msg_length+5];
	tmpbuf[4] = 0x0e;
	tmpbuf[5]=(u_int8_t) (_globalDate->mainProStat[ACK_ElectronicZoom_value]&0xff);
	package_frame(msg_length, tmpbuf);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,tmpbuf,msg_length+5);
}

void CPortBase::trackSearchStat(sendInfo * spBuf)
{
	int msg_length = 2;
	u_int8_t  retrackSearchStat[msg_length+5];
	retrackSearchStat[4] = 0x0f;
	unsigned int sectrkstat = 0;//_globalDate->avt_status.AvtTrkStat;
	if(sectrkstat == 4)
		sectrkstat = 1;
	else
		sectrkstat = 2;
	retrackSearchStat[5] = sectrkstat;
	package_frame(msg_length, retrackSearchStat);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,retrackSearchStat,msg_length+5);
}

void CPortBase::moveTargetDetectedStat(sendInfo * spBuf)
{
	int msg_length = 2;
	u_int8_t  tmpbuf[msg_length+5];
	channelTable    currtmoveTargetDet;
	/*
	currtmoveTargetDet.channel0=_globalDate->avt_status.MtdState[0];
	currtmoveTargetDet.channel1=_globalDate->avt_status.MtdState[1];
	currtmoveTargetDet.channel2=_globalDate->avt_status.MtdState[2];
	currtmoveTargetDet.channel3=_globalDate->avt_status.MtdState[3];
	currtmoveTargetDet.channel4=_globalDate->avt_status.MtdState[4];
	currtmoveTargetDet.channel5=_globalDate->avt_status.MtdState[5];
	*/
	tmpbuf[4] = 0x10;
	tmpbuf[5]=*(u_int8_t*)(&currtmoveTargetDet);
	package_frame(msg_length, tmpbuf);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,tmpbuf,msg_length+5);
}

void CPortBase::pictureInPictureStat(sendInfo * spBuf)
{


}

void  CPortBase::vedioTransChannelStat(sendInfo * spBuf)
{

}

void CPortBase::frameFrequenceStat(sendInfo * spBuf)
{
	int msg_length = 3;
	u_int8_t  tmpbuf[msg_length+5];
	int  frameFrequenceStat[2]={0};
	tmpbuf[4] = 0x21;
	tmpbuf[5] = frameFrequenceStat[0];
	tmpbuf[6] = frameFrequenceStat[1];
	package_frame(msg_length, tmpbuf);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,tmpbuf,msg_length+5);
}

void CPortBase::vedioCompressStat(sendInfo * spBuf)
{
	int msg_length = 3;
	u_int8_t  tmpbuf[msg_length+5];
	int  vedioCompressStat[2]={0};
	tmpbuf[4] = 0x22;
	tmpbuf[5] = vedioCompressStat[0];
	tmpbuf[6] = vedioCompressStat[1];
	package_frame(msg_length, tmpbuf);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,tmpbuf,msg_length+5);
}

void CPortBase::settingCmdRespon(sendInfo * spBuf)
{
	int msg_length = 7;
	u_int8_t  tmpbuf[msg_length+5];
	tmpbuf[4] = 0x30;
	tmpbuf[5] = (u_int8_t) (_globalDate->mainProStat[ACK_config_Wblock]&0xff);
	tmpbuf[6] = (u_int8_t) (_globalDate->mainProStat[ACK_config_Wfield]&0xff);
	memcpy(tmpbuf+7,&(_globalDate->Host_Ctrl[config_Wvalue]),4);
	package_frame(msg_length, tmpbuf);
	spBuf->byteSizeSend=msg_length+5;
	memcpy(spBuf->sendBuff,tmpbuf,msg_length+5);
}

void CPortBase::readConfigSetting(sendInfo * spBuf)
{
	u_int8_t sumCheck;
	u_int8_t  readCfgSetting[8];
	float tmp ;
	int msglen = 0x07;
	spBuf->byteSizeSend = 0x0c;

	int len ;
	if(_globalDate->choose){
		len = strlen(_globalDate->ACK_read_content);
		msglen = msglen -4 + len;
		spBuf->byteSizeSend = msglen + 5;
	}

	spBuf->sendBuff[0] = 0xEB;
	spBuf->sendBuff[1] = 0x53;
	spBuf->sendBuff[2]= msglen&0xff;
	spBuf->sendBuff[3]= (msglen>>8)&0xff;
	spBuf->sendBuff[4]= 0x31;
	spBuf->sendBuff[5]= (u_int8_t) (_globalDate->mainProStat[ACK_config_Rblock]&0xff);
	spBuf->sendBuff[6]= (u_int8_t) (_globalDate->mainProStat[ACK_config_Rfield]&0xff);

	if(_globalDate->choose){
		memcpy((void*)&spBuf->sendBuff[7],(void*)_globalDate->ACK_read_content,len);
		sumCheck=sendCheck_sum(msglen+3, spBuf->sendBuff+1);
		spBuf->sendBuff[msglen+4]=(sumCheck&0xff);
		_globalDate->choose = 0;
	}else{
		tmp = _globalDate->ACK_read[0];
		printf("%s: %d      block = %d , field = %d ,value = %f \n",__func__,__LINE__,spBuf->sendBuff[5],spBuf->sendBuff[6],tmp);
		memcpy(spBuf->sendBuff+7,&tmp,4);
		sumCheck=sendCheck_sum(msglen+3, spBuf->sendBuff+1);
		spBuf->sendBuff[msglen+4]=(sumCheck&0xff);
		_globalDate->ACK_read.erase(_globalDate->ACK_read.begin());
	}

	printf("read <=====> is  back\n");
}

void CPortBase::extExtraInputResponse(sendInfo * spBuf)
{

}


void  CPortBase:: upgradefwStat(sendInfo * spBuf)
{
	u_int8_t sumCheck;
	u_int8_t trackStatus[3];
	spBuf->sendBuff[0]=0xEB;
	spBuf->sendBuff[1]=0x53;
	spBuf->sendBuff[2]=0x03;
	spBuf->sendBuff[3]=0x00;
	spBuf->sendBuff[4]=0x35;
	spBuf->sendBuff[5]=_globalDate->respupgradefw_stat;
	spBuf->sendBuff[6]=_globalDate->respupgradefw_perc;
	sumCheck=sendCheck_sum(6,spBuf->sendBuff+1);
	spBuf->sendBuff[7]=(sumCheck&0xff);
	spBuf->byteSizeSend=0x08;
}
void  CPortBase:: paramtodef(sendInfo * spBuf)
{
	u_int8_t sumCheck;
	spBuf->sendBuff[0]=0xEB;
	spBuf->sendBuff[1]=0x53;
	spBuf->sendBuff[2]=0x02;
	spBuf->sendBuff[3]=0x00;
	spBuf->sendBuff[4]=0x0d;
	spBuf->sendBuff[5]= (u_int8_t) (_globalDate->mainProStat[ACK_config_Rblock]&0xff);
	sumCheck=sendCheck_sum(5,spBuf->sendBuff+1);
	spBuf->sendBuff[6]=(sumCheck&0xff);
	spBuf->byteSizeSend=0x07;
}

u_int8_t CPortBase:: sendCheck_sum(uint len, u_int8_t *tmpbuf)
{
	u_int8_t  ckeSum=0;
	for(int n=0;n<len;n++)
		ckeSum ^= tmpbuf[n];
	return ckeSum;
}

u_int8_t CPortBase:: package_frame(uint len, u_int8_t *tmpbuf)
{
	tmpbuf[0] = 0xEB;
	tmpbuf[1] = 0x53;
	tmpbuf[2] = len&0xff;
	tmpbuf[3] = (len>>8)&0xff;
	tmpbuf[len+4]= sendCheck_sum(len+3,tmpbuf+1);
	return 0;
}

unsigned char CPortBase::check_sum(int len_t)
{
    unsigned char cksum = 0;
    for(int n=1; n<len_t-1; n++)
    {
        cksum ^= _globalDate->rcvBufQue.at(n);
    }
    return  cksum;
}

FILE *fp = NULL;
FILE *fp2 = NULL;
int current_len = 0;
int current_len2 = 0;
int CPortBase::upgradefw(unsigned char *swap_data_buf, unsigned int swap_data_len)
{
	int status;
	int write_len;
	int file_len;

	int recv_len = swap_data_len-13;
	unsigned char buf[8] = {0xEB,0x53,0x03,0x00,0x35,0x00,0x00,0x00};

	unsigned char cksum = 0;
	for(int n=1;n<swap_data_len-1;n++)
	{
		cksum ^= swap_data_buf[n];
	}
	if(cksum !=swap_data_buf[swap_data_len-1])
	{
		printf("checksum error,cal is %02x, recv is %02x\n",cksum,swap_data_buf[swap_data_len-1]);
		return -1;
	}
	memcpy(&file_len,swap_data_buf+5,4);
	if(NULL==fp)
	{
		if(NULL ==(fp = fopen("dss_pkt.tar.gz","w")))
		{
			perror("fopen\r\n");
			return -1;
		}
		else
		{
			printf("openfile dss_pkt.tar.gz success\n");
		}
	}

	write_len = fwrite(swap_data_buf+12,1,recv_len,fp);
	fflush(fp);
	if(write_len < recv_len)
	{
		printf("Write upgrade tgz file error!\r\n");
		return -1;
	}
	current_len += recv_len;
	if(current_len == file_len)
	{
		current_len = 0;
		fclose(fp);
		fp = NULL;

		if(fw_update_runtar() == 0)
			_globalDate->respupgradefw_stat= 0x01;
		else
			_globalDate->respupgradefw_stat = 0x02;

		_globalDate->respupgradefw_perc = 100;

		status = update_startsh();
		printf("update start.sh return %d\n",status);

		status = update_fpga();
		printf("update load_fpga.ko return %d\n",status);

		system("sync");
		
		_globalDate->feedback=ACK_upgradefw;
		OSA_semSignal(&_globalDate->m_semHndl);
	}
	else
	{
		_globalDate->respupgradefw_stat= 0x00;
		_globalDate->respupgradefw_perc = current_len*100/file_len;
		_globalDate->feedback=ACK_upgradefw;
		OSA_semSignal(&_globalDate->m_semHndl);
	}

}
int CPortBase::fw_update_runtar(void)
{
	printf("tar zxvf dss_pkt.tar.gz...\r\n"); 
	int iRtn=-1;
	char cmdBuf[128];
	sprintf(cmdBuf, "tar -zxvf %s -C ~/", "dss_pkt.tar.gz");
	iRtn = system(cmdBuf);
	return iRtn;
}

int CPortBase::update_startsh(void)
{
	printf("update start.sh...\r\n"); 
	int iRtn=-1;
	char cmdBuf[128];	
	sprintf(cmdBuf, "mv ~/dss_pkt/start.sh ~/");
	iRtn = system(cmdBuf);
	return iRtn;
}

int CPortBase::update_fpga(void)
{
	printf("update load_fpga.ko...\r\n"); 
	int iRtn=-1;
	char cmdBuf[128];	
	sprintf(cmdBuf, "mv ~/dss_pkt/load_fpga.ko ~/dss_bin/");
	iRtn = system(cmdBuf);
	return iRtn;
}

void CPortBase::seconds_sleep(unsigned seconds)
{
    struct timeval tv;

    int err;
    do{
        tv.tv_sec = seconds;
        tv.tv_usec = 0;
        err = select(0, NULL, NULL, NULL, &tv);
    }while(err<0 && errno==EINTR);
}

