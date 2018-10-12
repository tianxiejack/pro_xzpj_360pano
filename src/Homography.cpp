#include"Homography.hpp"
#include <iostream>
#include "stdio.h"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "osa.h"

#include "extra.h" 
using namespace cv;
using namespace std;
void FindHomographyfromPicture(Mat& src,Mat & dst,Mat &Ha)
{
/*
	  //Mat img_object = imread( "1.png", CV_LOAD_IMAGE_GRAYSCALE );  
	 // Mat img_scene = imread( "2.png", CV_LOAD_IMAGE_GRAYSCALE );  
	 Mat img_object = imread( "Bridge/01.jpg", CV_LOAD_IMAGE_GRAYSCALE );  
	 Mat img_scene = imread( "Bridge/02.jpg", CV_LOAD_IMAGE_GRAYSCALE );  
	  
	 //-- Step 1: Detect the keypoints using SURF Detector
	  int minHessian = 400;

	  SurfFeatureDetector detector( minHessian );

	  std::vector<KeyPoint> keypoints_object, keypoints_scene;

	  detector.detect( img_object, keypoints_object );
	  detector.detect( img_scene, keypoints_scene );

	  //-- Step 2: Calculate descriptors (feature vectors)
	  SurfDescriptorExtractor extractor;

	  Mat descriptors_object, descriptors_scene;

	  extractor.compute( img_object, keypoints_object, descriptors_object );
	  extractor.compute( img_scene, keypoints_scene, descriptors_scene );

	  //-- Step 3: Matching descriptor vectors using FLANN matcher
	  FlannBasedMatcher matcher;
	  std::vector< DMatch > matches;
	  matcher.match( descriptors_object, descriptors_scene, matches );

	  double max_dist = 0; double min_dist = 100;

	  //-- Quick calculation of max and min distances between keypoints
	  for( int i = 0; i < descriptors_object.rows; i++ )
	  { double dist = matches[i].distance;
	    if( dist < min_dist ) min_dist = dist;
	    if( dist > max_dist ) max_dist = dist;
	  }

	  printf("-- Max dist : %f \n", max_dist );
	  printf("-- Min dist : %f \n", min_dist );

	  //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	  std::vector< DMatch > good_matches;

	  for( int i = 0; i < descriptors_object.rows; i++ )
	  { if( matches[i].distance < 3*min_dist )
	     { good_matches.push_back( matches[i]); }
	  }

	  Mat img_matches;
	  drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
	               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
	               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	  //-- Localize the object
	  std::vector<Point2f> obj;
	  std::vector<Point2f> scene;

	  for( int i = 0; i < good_matches.size(); i++ )
	  {
	    //-- Get the keypoints from the good matches
	    obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
	    scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	  }

	  Mat H = findHomography( obj, scene, CV_RANSAC );

	  //-- Get the corners from the image_1 ( the object to be "detected" )
	  std::vector<Point2f> obj_corners(4);
	  obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
	  obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
	  std::vector<Point2f> scene_corners(4);

	  perspectiveTransform( obj_corners, scene_corners, H);

#if 0
	 

	  Mat dst1;

	  Mat shftMat=(Mat_<double>(3,3)<<1.0,0,img_object.cols, 0,1.0,0, 0,0,1.0);
	  warpPerspective(img_scene,dst1,shftMat*H,Size(img_scene.cols*2, img_scene.rows));

	  img_object.copyTo(Mat(dst1,Rect(0,0,img_object.cols,img_object.rows)));
	    imshow( "dst", dst1 );
#endif
	  //-- Draw lines between the corners (the mapped object in the scene - image_2 )
	  line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
	  line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	  line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	  line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

	  //line(img_matches,Point2f(0,0),Point2f(img_object.cols,img_object.rows),Scalar(0, 255, 0), 4);
	  //-- Show detected matches

	  imshow( "Good Matches & Object detection", img_matches );

	  waitKey(0);
*/
}

Vec3f rotationMatrixToEulerAngles(Mat &R)
{



    float sy = sqrt(R.at<double>(0,0) * R.at<double>(0,0) +  R.at<double>(1,0) * R.at<double>(1,0) );

    bool singular = sy < 1e-6; // If

    float x, y, z;
    if (!singular)
    {
    	 //printf("***************usr right*****************\n");
        x = atan2(R.at<double>(2,1) , R.at<double>(2,2));
        y = atan2(-R.at<double>(2,0), sy);
        z = atan2(R.at<double>(1,0), R.at<double>(0,0));
    }
    else
    {
        x = atan2(-R.at<double>(1,2), R.at<double>(1,1));
        y = atan2(-R.at<double>(2,0), sy);
        z = 0;
    }
    return Vec3f(x, y, z);
}

void find_feature_matches ( const Mat& img_1, const Mat& img_2,
                            std::vector<KeyPoint>& keypoints_1,
                            std::vector<KeyPoint>& keypoints_2,
                            std::vector< DMatch >& matches )
{
    
    Mat descriptors_1, descriptors_2;
   double exec_time=0;
    // used in OpenCV3 
  #if 0
   Ptr<FeatureDetector> detector = FeatureDetector::create ( "ORB" );
   Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create ( "ORB" );
   Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create ( "BruteForce-Hamming" );

   #elif 1
  Ptr<FeatureDetector> detector = FeatureDetector::create ( "ORB" );
  // OrbFeatureDetector detector(500);
   Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create ( "ORB" );
   Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create ( "BruteForce-Hamming" );
  #else
   Ptr<FeatureDetector> detector = FeatureDetector::create ( "SURF" );
   Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create ( "SURF" );
   Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create ( "BruteForce" );
  #endif
     exec_time = (double)getTickCount();
   // detector.removeInvalidPoints(const Mat & mask, vector < KeyPoint > & keypoints)
    detector->detect ( img_1,keypoints_1 );
    detector->detect ( img_2,keypoints_2 );
     exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();

    OSA_printf("the %s detect exec_time=%f\n",__func__,exec_time);
    exec_time = (double)getTickCount();
    descriptor->compute ( img_1, keypoints_1, descriptors_1 );
    descriptor->compute ( img_2, keypoints_2, descriptors_2 );
     exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
     OSA_printf("the %s compute exec_time=%f\n",__func__,exec_time);
   
    vector<DMatch> match;
    //BFMatcher matcher ( NORM_HAMMING );
    exec_time = (double)getTickCount();
    matcher->match ( descriptors_1, descriptors_2, match );
	 exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
     OSA_printf("the %s match exec_time=%f\n",__func__,exec_time);
	
    double min_dist=10000, max_dist=0;
	
       
    for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = match[i].distance;
        if ( dist < min_dist ) min_dist = dist;
        if ( dist > max_dist ) max_dist = dist;
    }




#if 1
	sort(matches.begin(), matches.end()); 
    printf ( "-- Max dist : %f \n", max_dist );
    printf ( "-- Min dist : %f \n", min_dist );

  
  //  for ( int i = 0; i < descriptors_1.rows; i++ )
   for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        if ( match[i].distance <= max ( 2*min_dist, 30.0 ) )
        {
            matches.push_back ( match[i] );
        }
    }
  #else
  	for(int i=0; i<match.size(); i++)  
	{  
		if(match[i].distance < 0.2 * max_dist)  
		{  
			matches.push_back(match[i]);  
		}  
	} 
 #endif
    
}

void pose_estimation_2d2d ( std:: vector<Point2f> points1,
                            vector<Point2f> points2,
                         
                            Mat& R, Mat& t )
{
 
   // Mat K = ( Mat_<double> ( 3,3 ) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1 );
#if 0
    double kx=2.666666;
    double ky=2.666666;
#else
	double kx=1;
    double ky=1;
#endif
    double fx=4671.959882/kx;
    double fy=4669.274908/ky;
    double ox=971.9267629/kx;
    double oy=545.9823086/ky;
    Mat K = ( Mat_<double> ( 3,3 ) << fx, 0, ox, 0, fy, oy, 0, 0, 1 );

    /*
    vector<Point2f> points1;
    vector<Point2f> points2;

    for ( int i = 0; i < ( int ) matches.size(); i++ )
    {
        points1.push_back ( keypoints_1[matches[i].queryIdx].pt );
        points2.push_back ( keypoints_2[matches[i].trainIdx].pt );
    }

  */
  /*
    Mat fundamental_matrix;
    fundamental_matrix = findFundamentalMat ( points1, points2, CV_FM_8POINT );
    cout<<"fundamental_matrix is "<<endl<< fundamental_matrix<<endl;
*/
    
    Point2d principal_point (ox, oy);	
    double focal_length = fy;			
    Mat essential_matrix;
    essential_matrix = findEssentialMat ( points1, points2, focal_length, principal_point );
    cout<<"essential_matrix is "<<endl<< essential_matrix<<endl;

 /*
    Mat homography_matrix;
    homography_matrix = findHomography ( points1, points2, RANSAC, 3 );
    cout<<"homography_matrix is "<<endl<<homography_matrix<<endl;
  
*/
   
    recoverPose ( essential_matrix, points1, points2, R, t, focal_length, principal_point );

 
   /*
    Vec3f sita;
    printf("___________________________________________________________________________\n");
    sita=rotationMatrixToEulerAngles(R);
   
    cout<<"sitais "<<endl<<sita<<endl;
    cout<<"X="<<sita[0]*180/3.141592653<<"**************"<<"Y="<<sita[1]*180/3.141592653<<"**************"<<"Z="<<sita[2]*180/3.141592653<<endl;
 
    
    cout<<"R is "<<endl<<R<<endl;
    cout<<"t is "<<endl<<t<<endl;
    printf("___________________________________________________________________________\n");
    */
    
}

int  getPano360Rotation(cv::Mat & src,cv::Mat & dst,double *rotation )
{
    int status=0;
   
    vector<KeyPoint> keypoints_1, keypoints_2;
    vector<DMatch> matches;
    find_feature_matches ( src, dst, keypoints_1, keypoints_2, matches );
     
    Mat R,t;
    vector<Point2f> points1;
    vector<Point2f> points2;
    for ( int i = 0; i < ( int ) matches.size(); i++ )
    {
        points1.push_back ( keypoints_1[matches[i].queryIdx].pt );
        points2.push_back ( keypoints_2[matches[i].trainIdx].pt );
    }

/*
     Mat first_match;
	drawMatches(src, keypoints_1, dst, keypoints_2, matches, first_match);
	imshow("first_match ", first_match);
  
	waitKey(1);
*/
    if(matches.size()<10)
    	{
		cout<<"the matches num is low="<<matches.size()<<endl;
		status= -1;
		 return status;

    	}
    pose_estimation_2d2d ( points1, points2, R, t );
    Vec3f sita;
    sita=rotationMatrixToEulerAngles(R);
    cout<<"X="<<sita[0]*180/3.141592653<<"**************"<<"Y="<<sita[1]*180/3.141592653<<"**************"<<"Z="<<sita[2]*180/3.141592653<<endl;
     *rotation=sita[1]*180/3.141592653;
    return status;
}
void getoffsetfromfeaturepoint(std::vector<Point2f> points1,std::vector<Point2f> points2,int* xoffset,int* yoffset,int width)
{
	int pointnum=20;
	Point2f src,dst;
	int sumx=0;
	int sumy=0;
	if(points1.size()<pointnum)
		pointnum=points1.size();
	for(int i=0;i<pointnum;i++)
		{
			sumx+=points1[i].x;
			sumy+=points1[i].y;

		}

	src.x=floor(sumx/pointnum);
	src.y=floor(sumy/pointnum);
	sumx=0;
	sumy=0;
	for(int i=0;i<pointnum;i++)
		{
			sumx+=points2[i].x;
			sumy+=points2[i].y;

		}

	dst.x=floor(sumx/pointnum);
	dst.y=floor(sumy/pointnum);

	*yoffset=dst.y-src.y;
	*xoffset=src.x-dst.x;
	//*xoffset=width-src.x+dst.x;

}

#define RESIZE 1
int  getPano360Offset(cv::Mat & src,cv::Mat & dst,int *xoffset ,int* yoffset)
{
	  int status=0;
   double exec_time = (double)getTickCount();
    vector<KeyPoint> keypoints_1, keypoints_2;
    vector<DMatch> matches;
	//(processgray[frameid],processgraytemp[frameid],Size(720,405),0,0,INTER_LINEAR)
    Mat tempsrc;
    if(RESIZE)
    resize(src,tempsrc,Size(960,540),0,0,INTER_LINEAR);
  
   // resize(src,tempsrc,Size(1920,1080),0,0,INTER_LINEAR);
    if(RESIZE)
    cvtColor(tempsrc,tempsrc,CV_BGR2GRAY);
    else
     cvtColor(src,tempsrc,CV_BGR2GRAY);
    
    Mat tempdst;
    if(RESIZE)
    resize(dst,tempdst,Size(960,540),0,0,INTER_LINEAR);
  
  //resize(dst,tempdst,Size(1920,1080),0,0,INTER_LINEAR);
    if(RESIZE)
    cvtColor(tempdst,tempdst,CV_BGR2GRAY);
    else
     cvtColor(dst,tempdst,CV_BGR2GRAY);
    find_feature_matches ( tempsrc, tempdst, keypoints_1, keypoints_2, matches );
     
    Mat R,t;
    vector<Point2f> points1;
    vector<Point2f> points2;
    for ( int i = 0; i < ( int ) matches.size(); i++ )
    {
        points1.push_back ( keypoints_1[matches[i].queryIdx].pt );
        points2.push_back ( keypoints_2[matches[i].trainIdx].pt );
    }

/*
     Mat first_match;
	drawMatches(src, keypoints_1, dst, keypoints_2, matches, first_match);
	imshow("first_match ", first_match);
  
	waitKey(1);
*/
    if(matches.size()<10)
    	{
		cout<<"the matches num is low="<<matches.size()<<endl;
		status= -1;
		 return status;

    	}
    getoffsetfromfeaturepoint(points1,points2,xoffset,yoffset,src.cols);
	   if(RESIZE)
	   	{
	*xoffset=*xoffset*2;
	*yoffset=*yoffset*2;
	   	}


    exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();

    OSA_printf("the %s exec_time=%f\n",__func__,exec_time);
    return status;

}

