#include"videoload.hpp"
#include<stdio.h>


VideoLoad* VideoLoad::instance=NULL;
#define DIRRECTDIR  "/home/ubuntu/calib/video/"

#define OPENCVAVI (0)
VideoLoad::VideoLoad():callfun(NULL),readnewfile(0),readname("1.xml"),readavi("1.avi"),readdir(DIRRECTDIR)
{


}
VideoLoad::~VideoLoad()
{
	

}
void VideoLoad::initgstreamer()
{
	 descr =
             //g_strdup_printf ("filesrc location=%s ! qtdemux name=demux ! queue ! h264parse ! omxh264dec ! videoconvert ! appsink name=sink sync=false "
    		  g_strdup_printf ("filesrc  location=/home/ubuntu/calib/video/1.avi ! avidemux name=demux ! queue ! h264parse ! omxh264dec ! videoconvert  ! video/x-raw, format=(string)BGR !  appsink name=sink sync=false "
    		//  g_strdup_printf ("appsrc name=source ! avidemux name=demux ! queue ! h264parse ! omxh264dec ! videoconvert ! 'video/x-raw, format=RGB' ! appsink name=sink sync=false "
    	  );
//
      pipeline = gst_parse_launch (descr, &error);
       
      if (error != NULL) {

        g_print ("could not construct pipeline: %s\n", error->message);

        g_clear_error (&error);

        exit (-1);

      }

         /*get appsrc*/
    	//g_object_set (G_OBJECT (app_src),"stream-type",0,"format",GST_FORMAT_TIME,NULL);
        //    app_src = gst_bin_get_by_name (GST_BIN (pipeline), "source");
    	//g_signal_connect(app_src,"need-data",G_CALLBACK(cb_need_data),app_src);
    	//g_object_set (G_OBJECT (app_src),"stream-type",0,"format",GST_FORMAT_TIME,NULL);
        /* get sink */
            sink = gst_bin_get_by_name (GST_BIN (pipeline), "sink");

       /* set to PAUSED to make the first frame arrive in the sink */
           ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
           switch (ret) {

            case GST_STATE_CHANGE_FAILURE:{
               g_print ("failed to play the file---------\n");
               exit (-1);
       	}
             case GST_STATE_CHANGE_NO_PREROLL:{
               g_print ("live sources not supported yet\n");
               exit (-1);
    	}

            default:

             break;

      }


      /* This can block for up to 5 seconds. If your machine is really overloaded,

       * it might time out before the pipeline prerolled and we generate an error. A

       * better way is to run a mainloop and catch errors there. */

      ret = gst_element_get_state (pipeline, NULL, NULL, 1 * GST_SECOND);

      if (ret == GST_STATE_CHANGE_FAILURE) {

        g_print ("failed to play the file==========\n");

        exit (-1);

      }

     gst_element_send_event (pipeline,gst_event_new_step (GST_FORMAT_BUFFERS, 1, 1, TRUE, FALSE));

      clock_t start,finish;
      int count = 0;
      bool isEnd = false;
    double sum;

}

Mat VideoLoad::getgstframebegin()
{
	g_signal_emit_by_name (sink, "pull-preroll", &sample, NULL);
	Mat show;
	  if(gst_app_sink_is_eos((GstAppSink*)sink)){

             g_print("is end of stream\n");
		return show;
      }
     
      if (sample)
	  	{
		        caps = gst_sample_get_caps (sample);
		        if (!caps) {

		          g_print ("could not get snapshot format\n");

				return show;
		          //exit (-1);

		        }

		        s = gst_caps_get_structure (caps, 0);



		        /* we need to get the final caps on the buffer to get the size */
			/*
		        res = gst_structure_get_int (s, "width", &width);
		        res |= gst_structure_get_int (s, "height", &height);
		        if (!res) {
		             g_print ("could not get snapshot dimension\n");
		             return show;
		        }
				*/
		        //printf("the widht =%d height=%d\n",width,height);
		        /* create pixmap from buffer and save, gstreamer video buffers have a stride

		         * that is rounded up to the nearest multiple of 4 */

		        buffer = gst_sample_get_buffer (sample);

		        /* Mapping a buffer can fail (non-readable) */

		        if (gst_buffer_map (buffer, &map, GST_MAP_READ)) {

		    	//render using map_info.data
		    		printf("********************************8\n");
		           // cv::Mat frame = cv::Mat(1080*3/2, 1920, CV_8UC1, (char *)map.data, cv::Mat::AUTO_STEP);
		    	 //g_print ("sizeof(*map.data) =  %d\n",sizeof(*map.data));
		    	    //show = cv::Mat(1080, 1920, CV_8UC3, (char *)map.data, cv::Mat::AUTO_STEP);	
		    	       memcpy(record.data,(char *)map.data,1920*1080*3);
			       gst_buffer_unmap (buffer, &map);
			       gst_sample_unref (sample);
			       gst_element_send_event (pipeline,gst_event_new_step (GST_FORMAT_BUFFERS, 1, 1, TRUE, FALSE));
		          return record;

        		}
			gst_sample_unref (sample);
	

     }
   
      gst_element_send_event (pipeline,gst_event_new_step (GST_FORMAT_BUFFERS, 1, 1, TRUE, FALSE));
}
void  VideoLoad::getgstframeend()
{
	return ;
	  if (sample)
	  	{
			    gst_buffer_unmap (buffer, &map);
			   gst_sample_unref (sample);
	  	}
	   
      gst_element_send_event (pipeline,gst_event_new_step (GST_FORMAT_BUFFERS, 1, 1, TRUE, FALSE));

}
void VideoLoad::uninitgstreamer()
{
	gst_element_set_state (pipeline, GST_STATE_NULL);

      gst_object_unref (pipeline);

}
void VideoLoad::create()
{
	record=Mat(1080,1920,CV_8UC3,cv::Scalar(0));
	MAIN_threadRecvCreate();
	OSA_semCreate(&loadsem,1,0);
	initgstreamer();

}

void VideoLoad::destory()
{
	MAIN_threadRecvDestroy();
	OSA_semDelete(&loadsem);
}



void VideoLoad::registerfun(VideoCallBackfun fun)
{	
	callfun=fun;
}


void VideoLoad::playvideo()
{
	OSA_semSignal(&loadsem);

}
void VideoLoad::initvideo()
{



}
void VideoLoad::main_Recv_func()
{
	OSA_printf("%s: Main Proc Tsk Is Entering...\n",__func__);
	
	unsigned char *data=NULL;
	int angle=0;
	int status;
	VideoCapture videocapture;
	Mat fileframe;
	if(OPENCVAVI)
	videocapture.open(readavi);
	VideoLoadData loaddata;
	mydata.open(readname.c_str());
	string aviname=readdir+readavi;
	string  xmlname=readdir+readname;
	memset(&loaddata,0,sizeof(VideoLoadData));
	static double gyrodata=0;
	
	while(mainRecvThrObj.exitProcThread ==  false)
	{	
		int capangle=0;
		//OSA_waitMsecs(30);
		OSA_semWait(&loadsem,OSA_TIMEOUT_FOREVER);
		//printf("********%s bengin******\n",__func__);
		if(getreadnewfile())
			{
				setreadnewfile(0);
				aviname=readdir+getreadavi();
				xmlname=readdir+getreadname();
				if(OPENCVAVI)
				videocapture.release();
				else
				uninitgstreamer();
				mydata.close();
				if(OPENCVAVI)
				videocapture.open(aviname);
				else
				initgstreamer();
				mydata.open(xmlname.c_str());

			}
		
		//initvideo();
		//if(getreadnewfile())
			{
				;

			}
		if(OPENCVAVI)
		status=videocapture.read(fileframe);
		else
			fileframe=getgstframebegin();
		if(!fileframe.empty())
			{
				
				//if()
				//resize(fileframe,fileframe,Size(config->getpanoprocesswidth(),config->getpanoprocessheight()),0,0,INTER_LINEAR);
				//printf("********2*file cap ok******\n");
				//fileframe.copyTo(img);

				
				//cvtColor(fileframe,img,CV_GRAY2BGR);
				//printf("********3*file cap ok******\n");

				loaddata=mydata.read();
				
				//printf("********loaddata=%f******\n",loaddata.gyroz);
			}
		else
			{
				//videocapture.set(CV_CAP_PROP_POS_FRAMES,0);
				if(OPENCVAVI)
					{
				videocapture.release();
				videocapture.open(aviname);
					}
				else
					{
						getgstframeend();
						uninitgstreamer();
						initgstreamer();

					}
				mydata.close();
				mydata.open(xmlname.c_str());
				if(OPENCVAVI)
				videocapture.read(fileframe);
				else
					{
						fileframe=getgstframebegin();
					}
				loaddata=mydata.read();

			}
		//printf("********%s end******\n",__func__);
		if(callfun!=NULL)
			{

				callfun(fileframe.data,&loaddata);
			}
		if(OPENCVAVI)
			;
		else
		getgstframeend();
		//OSA_printf("%s: Main Proc Tsk Is Entering. capture ..\n",__func__);
		/*
		data=(unsigned char *)readfilepicture(&angle);
		//capangle=angle*1000;
		if(capturecallback!=NULL)
		capturecallback(capture.data,&angle);
		*/
	}

}


int VideoLoad::MAIN_threadRecvCreate(void)
{
	int iRet = OSA_SOK;
	iRet = OSA_semCreate(&mainRecvThrObj.procNotifySem ,1,0) ;
	OSA_assert(iRet == OSA_SOK);


	mainRecvThrObj.exitProcThread = false;

	mainRecvThrObj.initFlag = true;

	mainRecvThrObj.pParent = (void*)this;

	iRet = OSA_thrCreate(&mainRecvThrObj.thrHandleProc, mainRecvTsk, 0, 0, &mainRecvThrObj);
	

	return iRet;
}


int VideoLoad::MAIN_threadRecvDestroy(void)
{
	int iRet = OSA_SOK;

	mainRecvThrObj.exitProcThread = true;
	OSA_semSignal(&mainRecvThrObj.procNotifySem);

	iRet = OSA_thrDelete(&mainRecvThrObj.thrHandleProc);

	mainRecvThrObj.initFlag = false;
	OSA_semDelete(&mainRecvThrObj.procNotifySem);

	return iRet;
}

VideoLoad *VideoLoad::getinstance()
{
	if(instance==NULL)
		instance=new VideoLoad();
	return instance;

}
