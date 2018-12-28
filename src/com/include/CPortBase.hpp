#ifndef __CPORTBASE_H__
#define __CPORTBASE_H__

#include "CPortInterface.hpp"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include <stdint.h>

using namespace std;

class CPortBase : public CPortInterface
{
public:
    CPortBase();
    ~CPortBase();

    CMessage *pM;
    CMessage *getpM();
    CGlobalDate* _globalDate;
    CGlobalDate* getDate();
    CStatusManager* _StateManager;
    CStatusManager* getStatus();

    void seconds_sleep(unsigned seconds);
    void EnableSelfTest();
    void EnableswitchVideoChannel();
    void selectVideoChannel();
    void EnableTrk();
    void displaymod();
    void SetResolution();
    void AIMPOS_X();
    void AIMPOS_Y();
    void EnableParamBackToDefault();
    void AxisMove();
    void EnableTrkSearch();
    void Enablealgosdrect();
    void ZoomSpeedCtrl();
    int  ZoomShortCtrl();
    int  ZoomLongCtrl();
    void IrisDown();
    void IrisUp();
    void FocusDown();
    void FocusUp();
    void AvtAxisCtrl();
    void EnableOsdbuffer();
    void EnablewordType();
    void EnablewordSize();
    void Config_Write_Save();
    void Config_Read();
    void EnableSavePro();
    void AXIS_X();
    void AXIS_Y();
    void Preset_Mtd();
    void workMode();
    void targetCaptureMode();
//
    int  getSendInfo(int  respondId, sendInfo * psendBuf);
    void mainVedioChannel(sendInfo * spBuf);
    void bindVedioChannel(sendInfo * spBuf);
    void trackStatus(sendInfo * spBuf);
    void trackTypes(sendInfo * spBuf);
    void trackErrOutput(sendInfo * spBuf);
    void mutilTargetNoticeStatus(sendInfo * spBuf);
    void multilTargetNumSelectStatus(sendInfo * spBuf);
    void  imageEnhanceStatus(sendInfo * spBuf);
    void trackFinetuningStat(sendInfo * spBuf);
    void confirmAxisStat(sendInfo * spBuf);
    void ElectronicZoomStat(sendInfo * spBuf);
    void trackSearchStat(sendInfo * spBuf);
    void moveTargetDetectedStat(sendInfo * spBuf);
    void pictureInPictureStat(sendInfo * spBuf);
    void  vedioTransChannelStat(sendInfo * spBuf);
    void frameFrequenceStat(sendInfo * spBuf);
    void vedioCompressStat(sendInfo * spBuf);
    void settingCmdRespon(sendInfo * spBuf);
    void readConfigSetting(sendInfo * spBuf);
    void extExtraInputResponse(sendInfo * spBuf);
    void upgradefwStat(sendInfo * spBuf);
    void paramtodef(sendInfo * spBuf);
	int upgradefw(unsigned char *swap_data_buf, unsigned int swap_data_len);
	int fw_update_runtar(void);
	int update_startsh(void);
	int update_fpga(void);
    u_int8_t  sendCheck_sum(uint len, u_int8_t *tmpbuf);
	u_int8_t package_frame(uint len, u_int8_t *tmpbuf);



protected:
    int prcRcvFrameBufQue(int method);
    unsigned char check_sum(int len_t);
};

#endif