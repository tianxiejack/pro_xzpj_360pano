/*
 * #
 *
 *  Created on: 2018年11月6日
 *      Author: wj
 */

#ifndef __CONFIG_HPP_
#define __CONFIG_HPP_
class Config{

	public:
	/*camera */
     int cam_width;
     int cam_height;
     int cam_channel;
     double cam_fov;

     double cam_fx; 
     double cam_fy; 
     double cam_ox;
     double cam_oy;
     double cam_k1;
     double cam_k2;
     double cam_k3;
     double cam_k4;

     inline int getcamwidth(){return cam_width;};
     inline int getcamheight(){return cam_height;};
     inline int getcamchannel(){return cam_channel;};
     inline double getcamfx(){return cam_fx;};

    /*display*/
	 int display_width;
	 int display_height;
	 int display_channel;
	 inline int getdiswidth(){return display_width;};
	 inline  int getdisheight(){return display_height;};
	 inline int getdischannel(){return display_channel;};


	 /*pano*/
	 double panozeroptz;
	 int panoprocesswidth;
	 int panoprocessheight;
	 inline double getpanozeroptz(){return panozeroptz;};
	 void setpanozeroptz(double angle){panozeroptz=angle;}

	 inline int getpanoprocesswidth(){return panoprocesswidth;};
	 inline  int getpanoprocessheight(){return panoprocessheight;};

	 /*mvdetect*/
	 int mvprocesswidth;
	 int mvprocessheight;
	 int mvdownup;

	 inline int getmvprocesswidth(){return mvprocesswidth;};
	 inline  int getmvprocessheight(){return mvprocessheight;};
	 inline  int getmvdownup(){return mvdownup;};

	 int ptzwait;
	 int ptzbroad;
	 int ptzaddres;
	 int ptzdp;
	 

	 inline int getptzwait(){return ptzwait;};
	 inline  int getptzbroad(){return ptzbroad;};
	 inline  int getptzaddres(){return ptzaddres;};
	 inline  int getptzdp(){return ptzdp;};





	 int intergralenable;
	 inline int getintergralenable(){return intergralenable;};
	 void setintergralenable(int enable){ intergralenable=enable;};


	 

	static Config *getinstance();
	void saveconfig();
	void loadconfig();
	void SaveConfig();
	private:



	Config();
	~Config();
	static Config *instance;
	//final static Config configinstance=new Config();

};




#endif /* CONFIG_HPP_ */
