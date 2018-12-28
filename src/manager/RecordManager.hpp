/*
 * RecordManager.hpp
 *
 *  Created on: Dec 27, 2018
 *      Author: wj
 */

#ifndef RECORDMANAGER_HPP_
#define RECORDMANAGER_HPP_
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
class RecordManager
{
public:
	vector<string> recordvideonames;
	vector<string> recordfilenames;
	string  recordpath;
	int nextvideoid;
	int createplayertimeid;
	int playertimer;
	int enableplay;
public:
	void create();
	
	void enableplayer(int enable){enableplay=enable;};
	int getableplayer(){return enableplay;};
	
	void createplayertimer();
	void findrecordnames();
	void getnexvideo();
	void getJustCurrentFile(string path, vector<string> & video,vector<string> & files);
	 bool startsWith(const std::string& str, const std::string& substr);
	 bool endsWith(const std::string& str, const std::string& substr);
	static void recordplaycallback(void *arg);
	void setplayertimer(unsigned int timer);

private:
	RecordManager();
	~RecordManager();
	static RecordManager*instance;
public:
	static RecordManager*getinstance();
};



#endif /* RECORDMANAGER_HPP_ */
