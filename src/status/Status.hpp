/*
 * Status.hpp
 *
 *  Created on: Dec 27, 2018
 *      Author: wj
 */

#ifndef STATUS_HPP_
#define STATUS_HPP_
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
