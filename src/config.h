/*
 * config.h
 *
 *  Created on: 2018年9月14日
 *      Author: fsmdn113
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define VERSION_STRING		       "v1.00"




#define camerafov	            (18)

#define CAPTURETVWIDTH  (1920)
#define CAPTURETVHEIGHT  (1080)


#define PANO360SRCWIDTH (1920)
#define PANO360SRCHEIGHT (1080)

#define PANO360WIDTH (1920)
#define PANO360HEIGHT (1080)



#define CYLINDERPROJECTION (0)


#define AUTOMOV (0)

//#define CAMERAFOCUSLENGTH (4670)
#define CAMERAFOCUSLENGTH (4759.721)

#define IMAGEQUEUE (1)

#define ANGLESCALE (1000)

#define PANOSCALE (1)

#define PANOSHIFT (0)

#define ANGLEINTREVAL (15)

#define ANGLESHIFT (0)

#define PANOWDTEHENTEND (0)

#define FEATURESTICH (0)

#define CYLINDER (1)

#define PANODETECTNUM (2)

#define PANOEXTRAH 20

#define PANOSRCSHIFT (100)


#define PANOGRAYDETECT (1)

#define MOVEBLOCKNUM (10)

#define MOVEBLOCKENABLE (1)



#define MOVDETECTDOW (2)

#define MOVDETECTDOWENABLE (1)

#define DETECTTEST (0)

#define FILEVIDEO (0)

#define MULTICPUPANO (1)

#define MOVDETECTSRCWIDTH (PANO360WIDTH*2)
#define MOVDETECTSRCHEIGHT (PANO360HEIGHT)


#define MULTICPUPANONUM (8)

#define ZEROJUEGE (10)

#define MULDETECTTRAIN (0)
//#define PANOSRCSHIFT (0)

#endif /* CONFIG_H_ */
