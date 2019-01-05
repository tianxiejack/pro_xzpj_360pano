/*
 * Status.hpp
 *
 *  Created on: Dec 27, 2018
 *      Author: wj
 */

#ifndef STATUS_HPP_
#define STATUS_HPP_


#define HELDNUM  2
#define HELDWEEK  7
#define HELDHOUR  24
class Status{
public:
	typedef enum {
		LIVEMOD=0,
		PLAYCALLBACK,
	}Dismod;

	typedef enum {
		PTZPANOMOV,
		PTZTITLEMOV,
		PTZTWOMOV,
	}Plantctl;

	typedef enum {
		PTZFOCUSSTOP,
		PTZFOCUSSNEAR,
		PTZFOCUSSFAR,
	}Focusctl;
	typedef enum {
		PTZFOCUSONSTOP,
		PTZFOCUSSONDOWN,
		PTZFOCUSSONUP,
	}Focusonctl;

	typedef enum {
		PTZIRISSTOP,
		PTZIRISDOWN,
		PTZIRISUP,
	}Irisctl;

	typedef enum {
		PANOAUTO,
		PANOPTZ,
		PANOSELECT,
	}Workmod;

	typedef enum {
		MOUSEBUTTON,
		MOUSEROLLER,
		
	}MouseEvent;


	typedef enum {
		STOREGO,
		STORESAVE,
		
	}STOREMOD;
	typedef enum {
		
		STOREERASENUM,
		STORESAVENUM,
		
	}STOREMODNUM;


	typedef enum {
		ZEROSTOP,
		ZEROSTART,
		ZEROSAVE,
		
	}ZeroMod;

	

	int ptzpanodirection;
	int ptztitledirection;
	int ptzpanspeed;
	int ptztitlespeed;

	int mouseevent;
	int mousemod;
	int mousebuttonstaus;
	int mouseleftright;
	int mousex;
	int mousey;
	int rollerstatus;


	int storemod;
	
	int storegonum;

	int storesavenum;

	int storesavemod;

	/******plantform config*******/

	int ptzaddress;
	int ptzprotocal;
	int ptzbaudrate;
	int ptzspeed;

	/******sensor config*******/
	unsigned char brightness;
	unsigned char contract;
	unsigned char autobright;
	unsigned char backandwrite;
	unsigned char correct;
	unsigned char digitfilter;
	unsigned char digitdenoise;
	unsigned char mirror;
	unsigned char crossdisplay;
	unsigned int crossx;
	unsigned int crossy;

	unsigned int save;


	/******zero config*******/

	unsigned char zeromod;


	/******record config*******/
	int recordpositionheld[HELDNUM][HELDWEEK][HELDHOUR];

	/******movedetect config*******/

	int movedetectalgenable;

	int movedetectmaxnum;

	int speedpriority;

	int sensitivity;

	int movedetectrecord;

	int moverecordtime;

	/******movearea config*******/
	int detectareanum;
	int detectareaenable;


	/******display config*******/

	int displayresolution;

	/******time config*******/
	int correctyear;
	int correctmonth;
	int correctday;
	int correcthour;
	int correctmin;
	int correctsec;

	/******pano config*******/
	int panoptzspeed;
	int panopiexfocus;
	int panopicturerate;

	

	

public:
	void getmouseparam(int &button, int &state, int &x, int &y){button=mouseleftright;state=mousebuttonstaus;x=mousex;y=mousey;};
	int getdisplaymod(){return displaymod;};
	void setdisplaymod(int dis){displaymod=dis;};

	int getworkmod(){return workmode;};
	void setworkmod(int dis){workmode=dis;};


	int getstoremod(){return storemod;};
	void setstoremod(int mod){storemod=mod;};
	
private:
	int displaymod;
	int workmode;
private:
	Status();
	~Status();
	static Status*instance;

public:
	static Status* getinstance();

};




#endif /* STATUS_HPP_ */
