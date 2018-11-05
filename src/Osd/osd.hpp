/*
 * osd.hpp
 *
 *  Created on: 2018年10月31日
 *      Author: wj
 */

#ifndef OSD_HPP_
#define OSD_HPP_
#define odstextlen 100
#define OSDCONTEXLEN 100





typedef enum 
{
	
	SINGLEMODE,
	PANOMODE,
	SELECTMODE,
	SELECTZEROMODE,
	MAXMODE,
	CALIBRATION,
	MODELING,
	

	MAXCONTXT
}contexid;



	





typedef enum 
{
	REDColour,
	BLUEColour,


}contexidColour;
typedef enum 
{
	HIDEMODE=1,
	DISMODE,


}contexidDismode;

typedef struct
{
	contexid index;
	int x;
	int y;
	contexidColour colour;
	contexidDismode display;
	wchar_t context[odstextlen];
	

}Osdcontext;


class OSD
{
public:
	OSD();
	~OSD();
	
	Osdcontext * getOSDcontex();


};


#endif /* OSD_HPP_ */
