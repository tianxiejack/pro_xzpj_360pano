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

public:
	int getdisplaymod(){return displaymod;};
	void setdisplaymod(int dis){displaymod=dis;};
private:
	int displaymod;
private:
	Status();
	~Status();
	static Status*instance;

public:
	static Status* getinstance();

};




#endif /* STATUS_HPP_ */
