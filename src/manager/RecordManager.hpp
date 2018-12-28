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
public:
	void create();
	void findrecordnames();
	void getnexvideo();
	void getJustCurrentFile(string path, vector<string> & video,vector<string> & files);
	 bool startsWith(const std::string& str, const std::string& substr);
	 bool endsWith(const std::string& str, const std::string& substr);
private:
	RecordManager();
	~RecordManager();
	static RecordManager*instance;
public:
	static RecordManager*getinstance();
};



#endif /* RECORDMANAGER_HPP_ */
