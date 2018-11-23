/*
 * Render.hpp
 *
 *  Created on: 2018年9月14日
 *      Author: fsmdn113
 */

#ifndef RENDER_HPP_
#define RENDER_HPP_
#include <GL/glew.h> // glUniformxxx()
#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.
#include <stdio.h>
#include <time.h>       /* For our FPS */
#include <string.h>    /* for strcpy and relatives */
#include <unistd.h>     /* needed to sleep*/
#include <math.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <GLBatch.h>
#include <GLShaderManager.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrustum.h>
#include"Compress.hpp"
#include "config.h"
#include <iostream>
#include "Glosd.hpp"
#include "osd.hpp"
#include"menu.hpp"

//#include "mvdectInterface.hpp"
//static const int ALPHA_MASK_HEIGHT= DEFAULT_IMAGE_HEIGHT;
//static const int ALPHA_MASK_WIDTH = (DEFAULT_IMAGE_WIDTH/16);
/* A general OpenGL initialization function. */
/* Called once from main() */
/*set up render scene*/
//static const M3DVector3f DEFAULT_ORIGIN = {0.0f, 0.0f, 50.0f};
using namespace cv;


#define MAXTURESELECT (8)
typedef struct{

	Rect leftdownrect;

	Rect updownselcectrect;
	int active;
	
	int panotextureindex;

	int blindtextnum;
	int blindtextid[MAXTURESELECT];

	//#define SELECTMAX (10)
	GLBatch	*panselecttriangleBatch[MAXTURESELECT];
	
	


}ViewCamera;







class Render{
#define BRIDGENUM 40
#define CARSNUM 24
#define PANO360NUM 20

#define MAXPICMUN 60

#define MAXSEAM 2

#define MAXPANOFRAME 2
public:
	Render();
	~Render();


	enum {
	RENDER180,
	RENDER360,
	RENDERCAMERA1,
	RENDERCAMERA2,
	RENDERCAMERA3,
	RENDERCAMERA4,
	RENDERCAMERMAX,
	RENDERRADER,
	MAXCAMER,



	};
	 enum TEXTUREID {
		TESTTEXTURE,
		PANOTEXTURE,
		PANOTEXTURE1,
		PANOTEXTURE2,
		PANOTEXTURE3,
		PANOTEXTURE4,
		CAPTEXTURE,
		SELECTTEXTURE,
		FUSIONTAIL,
		TEXTUREMAX,
		} Textureid; 
	//bool getPointsValue(int direction, int x, Point2f *Point);



	Compress compress;
	unsigned char *screenpiex;
	Mat screenpiexframe;
	unsigned char screenenable;

	void destroyPixList();
	void SetupRC(int windowWidth, int windowHeight);
	void ShutdownRC();
	void initCorners(void);
	void ReSizeGLScene(int Width, int Height);
	void DrawGLScene();
	void initClass(void);
	void initPixle(void);
	void readPixleFile(const char* file, int index);
	void InitScanAngle(void);
	void screenshot();
	void screenshotinit();
	void readScanAngle(const char * filename);
	void writeScanAngle(const char *filename,float angle,float angleofruler);
	void ProcessOitKeys(unsigned char key, int x, int y);
	void ProcessSpecialOitKeys(unsigned char key, int x, int y);
	void mouseButtonPress(int button, int state, int x, int y);
	void mouseMotionPress(int x, int y);
	inline void setMouseCor(int x, int y){MOUSEx = x;  MOUSEy = y;};
	inline void setMouseButton(int button){BUTTON=button;};
	inline void setMousestatue(int statue){MOUSEST=statue;};

	void Mouse2Select();
	void MouseSelectpos();
	void Mousezeropos();
	

	int MOUSEx , MOUSEy , BUTTON ,MOUSEST;
	int mousex,mousey,mouseflag;
	void PostCaptureFrame();

	void keyPressed(unsigned char key, int x, int y);
	void specialkeyPressed (int key, int x, int y);
	bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void RenderScene(void);
	void Textureinit(void);
	void Panotexture(void);
	void Selecttexture(void);

	void singleView(int x,int y,int width,int height);
	void singleViewInit(void);

	void panotestView(int x,int y,int width,int height);
	void panotestViewInit(void);
	void loadpanopicture();
	void loadpanobridgepicture();
	void loadpanocarspicture();
	void loadpano360picture();
	void testcylinder();
	void testcylinderinit();
	void cylinderproject(Mat& src,Mat & dst,double R);
	void CaptureFrame(int chid,int widht,int height,int channel,unsigned char *data);
	void CaptureProcessFrame(int chid,int widht,int height,int channel,unsigned char *data);
	void CapturePreprocess(Mat& src);
	void Capturetexture(void);
	void CaptureSavebmp(Mat& src);
	void CaptureSavepicture(Mat& src);
	void cylindercut(Mat& src,Mat & dst,int cutnum);
	
	void Pano360init();

	void Fusiontailtexture(void);
	int recordscreen;
	static Render *pthis;

	/***ViewCamera**/
	ViewCamera viewcamera[MAXCAMER];
	Mat viewWarningarea[MAXCAMER];
	unsigned char *viewWarningdata[MAXCAMER];
	void viewcameraprocess();
	void leftdown2leftup(Rect& down,Rect& up);
	void leftup2leftdown(Rect& down,Rect& up);


	VideoWriter videowriter;
	/******************modeselect ***********************/
	static void callbackpanomod(void *contex);
	void selectmod();
	void panomod();
	void zeromod();

	void pano(int num);

	/******************osd menu ***********************/
	void Menuinti();
	void Mousemenu();
	static void  Menucall(void *context);
	
	int osdmenushow;
	int osdmenushowpre;
	/******************osd ***********************/
	
	void Drawosd();
	void Drawmovdetect();
	void DrawmovMultidetect();
	void Drawmov();
	void Drawlines();
	void Drawfusion();
	void Drawmenu();
	void Drawosdmenu();
	

	MENU *Menu;
	int movviewx;
	int movviewy;
	int movvieww;
	int movviewh;

	int mov180viewx;
	int mov180viewy;
	int mov180vieww;
	int mov180viewh;

	int mov360viewx;
	int mov360viewy;
	int mov360vieww;
	int mov360viewh;


	int menumode;
	std::vector<cv::Rect>	detect_vect180;
	std::vector<cv::Rect>	detect_vect360;

	//std::vector<OSDPoint>	detect_vectradarpoints;
	std::vector<OSDPoint>  detect_vectradarpoints[500];
	void movMultidetectrect();
	void setmenumode(int mod){menumode=mod;};
	int getmenumode(){return menumode;};

	//std::vector<TRK_RECT_INFO>	detect_vect;


	void ptzinit();
	GLOSD Glosdhandle;

	OSD osdcontex;
	/******************select function mod ***********************/

	void selectupdate();
	void Drawmenuupdate();
	int selecttexture;
	void setselecttexture(int mod){selecttexture=mod;};
	int  getselecttexture(){return selecttexture;};
	
	unsigned int selectx;
	unsigned int selecty;
	unsigned int selectw;
	unsigned int selecth;

	std::vector<cv::Rect>	detect_vect;
	


	/******************display mod ***********************/

	void pano360View(int x,int y,int width,int height);
	void pano360triangleBatchhalfhead(int mod);

	/******************360pano***********************/
	
	void Pano360fun();
	Mat Pano360frame[MAXPANOFRAME];
	Mat Pano360tempframe;
	unsigned int Pano360frameId;
	unsigned int pano360subwidth;
	unsigned int pano360subheight;
	unsigned int pano360subx;
	unsigned int pano360suby;
	int panoAngle;

	
	unsigned int panoPreposx;
	unsigned int panoposx;
	unsigned int positionchanged;


	unsigned int pano360texturew;
	unsigned int pano360textureh;

	unsigned int pano360renderw;
	unsigned int pano360renderH;
	unsigned int pano360renderLux;
	unsigned int pano360renderLuy;

	void setPano360RenderPos(unsigned int x,unsigned int y,unsigned int w,unsigned int h){pano360renderLux=x;pano360renderLuy=y;pano360renderw=w;pano360renderH=h;};
	void getPano360RenderPos(unsigned int * x,unsigned int * y,unsigned int * w,unsigned int * h){*x=pano360renderLux;*y=pano360renderLuy;*w=pano360renderw;*h=pano360renderH;};
	

	
	
	unsigned int panopostionchanged(){return positionchanged;};
	void panopostionreset(){positionchanged=0;};
	void panopostionset(){positionchanged=1;};
	
	void setPanoPrepos(unsigned int pos){panoPreposx=pos;};
	unsigned int getPanoPrepos(){return panoPreposx;};

	void setPanopos(unsigned int pos){panoposx=pos;};
	unsigned int getPanopos(){return panoposx;};
	

	
	
	void setPanoSubPos(unsigned int x,unsigned int y,unsigned int w,unsigned int h){pano360subx=x;pano360suby=y;pano360subwidth=w;pano360subheight=h;};
	void getPanoSubPos(unsigned int * x,unsigned int * y,unsigned int * w,unsigned int * h){*x=pano360subx;*y=pano360suby;*w=pano360subwidth;*h=pano360subheight;};
	
	Mat getcurrentDis(){return Pano360frame[Pano360frameId];};
	void SetCurrentDis(Mat src){Pano360frame[Pano360frameId]=src;};

	int getcurrnetId(){return Pano360frameId;};
	void setcurrentId(int id){Pano360frameId=id;};

	void Angle2pos();
	int getPanoAngle(){return panoAngle; };
	void setPanoAngle( int angle){panoAngle=angle;};

	int tailcut=0;
	void settailcut(int cut){tailcut=cut;};
	int gettailcut(){return tailcut;};
	

	void Drawosdcamera();
	
	/******************seamfunction***********************/
	void BestSeam(Mat& src,Mat & dst,int seampostion);
	void OptiSeamfun();
	/******************seaminit***********************/
	Mat Seamframe[MAXSEAM];
	Mat fusionframe;
	unsigned int seamid;
	unsigned int SeamEable;
	unsigned int Seampostion;

	Mat getCurrentFame(){return Seamframe[seamid%MAXSEAM];};
	Mat getPreFame(){return Seamframe[(seamid+1)%MAXSEAM];};

	unsigned int getcurrentFrameId(){return seamid;};
	void setcurrentFrameId(unsigned int id){ seamid=id;};
	void setNextFrameId(){seamid=(seamid+1)%MAXSEAM;};
	
	bool getSeamEnable(){return SeamEable;};
	void setSeamEnable(bool enable){SeamEable=enable;};
	
	unsigned int getSeamPos(){return Seampostion;};
	void setSeamPos(unsigned int sem){ Seampostion=sem;};
	/******************cameraparam***********************/
	double CameraFov;

	/******************TEXTURE USE***********************/
	unsigned int maxtexture;
	unsigned int pano360texturenum;
	unsigned int pano360texturewidth;
	unsigned int pano360textureheight;

	void pano360triangleBatchhalf();
	void pano360triangleBatchhalfhead(GLBatch &Batch,int mod);
	void pano360triangleBatchhalfhead(GLTriangleBatch &Batch,int mod);
	void getnumofpano360texture(int startx,int endx,int* texturestart,int* textureend);


	/******************mov detect***********************/
	Mat cpuPANO[PANODETECTNUM];
	
public:
	unsigned int Fullscreen;
	
	
public:
	unsigned int panosrcwidth;
	unsigned int panosrcheight;
	unsigned int panowidth;
	unsigned int panoheight;
	int panoformat;
	int rotsita;
	Mat bridge[MAXPICMUN];
	int Panpicenum;
	Rect panopositon[MAXPICMUN];

	Mat cars[MAXPICMUN];
	Mat pano360[MAXPICMUN];
	int carsnum;
	Rect carspositon[MAXPICMUN];

	Mat pano360temp;

	Mat testcylind;
	Mat testcylinddis;
	unsigned char * matbuf[8];

	unsigned int shotcut;
	unsigned int shotcutnum;

	Mat Capture;
	Mat ProcessCapture;
	


	
public:
	GLShaderManager		shaderManager;
	GLMatrixStack		modelViewMatrix;
	GLMatrixStack		projectionMatrix;
	GLFrame				cameraFrame;
	GLFrame             objectFrame;
	GLFrustum	     viewFrustum;
	double   viewfov;
	double   viewfocus;
	double viewprojectlen;

	

	GLBatch             pyramidBatch;

	GLuint              textureID[TEXTUREMAX];

	GLBatch	triangleBatch;

	GLBatch	pan360triangleBatch;
	GLBatch	pansrctriangleBatch;
	#define SELECTMAX (10)
	GLBatch	panselecttriangleBatch[SELECTMAX];
	void gltMakeradar(GLTriangleBatch& diskBatch, GLfloat innerRadius, GLfloat outerRadius, GLint nSlices, GLint nStack,double anglestart,double angleend);
	void gltMakeradar(GLTriangleBatch& diskBatch, GLfloat innerRadius, GLfloat outerRadius, GLint nSlices, GLint nStacks,double anglestart,double angleend,int mod);
	void gltMakeradarpoints(vector<OSDPoint>& osdpoints, GLfloat innerRadius, GLfloat outerRadius, GLint nSlices,double anglestart,double angleend);
	GLTriangleBatch     radar180;
	GLTriangleBatch     radar360;

	GLTriangleBatch     radar180half;
	GLTriangleBatch     radar360half;


	GLTriangleBatch     radar90quarter;
	GLTriangleBatch     radar180quarter;
	GLTriangleBatch     radar270quarter;
	GLTriangleBatch     radar360quarter;
	
	GLTriangleBatch     radarcamera[MAXCAMER];
	GLTriangleBatch     radarMultidetect;
	double radarinner;
	double radaroutter;
	
	GLBatch	panselectrectBatch;
	GLfloat vrectBatch[8][3];

	GLGeometryTransform	transformPipeline;
	M3DMatrix44f		shadowMatrix;
	int renderwidth;
	int renderheight;
	 enum DISPLAYMODE {
		SINGLE_VIDEO_VIEW_MODE,
		MY_TEST_PANO_MODE,
		PANO_360_MODE,
		PREVIEW_MODE,
		HALF_PANO_MODE,
		FRONT_AND_BACK_MODE,
		TOTAL_MODE_COUNT
		} displayMode; 


	
	
};




#endif /* RENDER_HPP_ */
