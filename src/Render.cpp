/*
 * Render.cpp
 *
 *  Created on: 2018年9月14日
 *      Author: fsmdn113
 */
#include"Render.hpp"
#include"config.h"
#include"osa.h"
#include <glut.h>
#include <freeglut_ext.h>
#include"Cylindrical.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include"Homography.hpp"
#include"Stich.hpp"
#include "FileRW.hpp"
#include "config.hpp"
#include "plantformcontrl.hpp"




using namespace cv;
using namespace std;
//#include <GL/glut.h>
/* ASCII code for the various keys used */
#define ESCAPE 27     /* esc */
#define ROTyp  105    /* i   */
#define ROTym  109    /* m   */
#define ROTxp  107    /* k   */
#define ROTxm  106    /* j   */
#define SCAp   43     /* +   */
#define SCAm   45     /* -   */

#define SPECIAL_KEY_UP			101
#define SPECIAL_KEY_DOWN 		103
#define SPECIAL_KEY_LEFT 		100
#define SPECIAL_KEY_RIGHT 		102

#define SPECIAL_KEY_INSERT 		108
#define SPECIAL_KEY_HOME 		106
#define SPECIAL_KEY_END 		107
#define SPECIAL_KEY_PAGEUP 		104
#define SPECIAL_KEY_PAGEDOWN 	105

#define SPECIAL_KEY_LEFT_SHIFT	112
#define SPECIAL_KEY_RIGHT_SHIFT	113
#define SPECIAL_KEY_LEFT_CTRL	114
#define SPECIAL_KEY_RIGHT_CTRL	115
#define SPECIAL_KEY_LEFT_ALT	116
#define SPECIAL_KEY_RIGHT_ALT	117

#define  GLUT_KEY_F1                        0x0001
#define  GLUT_KEY_F2                        0x0002
#define  GLUT_KEY_F3                        0x0003
#define  GLUT_KEY_F4                        0x0004
#define  GLUT_KEY_F5                        0x0005
#define  GLUT_KEY_F6                        0x0006
#define  GLUT_KEY_F7                        0x0007
#define  GLUT_KEY_F8                        0x0008
#define  GLUT_KEY_F9                        0x0009
#define  GLUT_KEY_F10                       0x000A
#define  GLUT_KEY_F11                       0x000B
#define  GLUT_KEY_F12                       0x000C
#define  GLUT_KEY_LEFT                      0x0064
#define  GLUT_KEY_UP                        0x0065
#define  GLUT_KEY_RIGHT                     0x0066
#define  GLUT_KEY_DOWN                      0x0067
#define  GLUT_KEY_PAGE_UP                   0x0068
#define  GLUT_KEY_PAGE_DOWN                 0x0069
#define  GLUT_KEY_HOME                      0x006A
#define  GLUT_KEY_END                       0x006B
#define  GLUT_KEY_INSERT                    0x006C



#define MOUSELEFT 0x0

#define MOUSERIGHT 0x02
#define MOUSEPRESS 0X00
#define MOUSEUP 0X01



//#define BRIDGE
#define PANO360
Render *Render::pthis=NULL;
//#define CARS

//#define NULLPIC

int window; /* The number of our GLUT window */

Render::Render():selectx(0),selecty(0),selectw(0),selecth(0),pano360texturew(0),pano360textureh(0),MOUSEx(0),MOUSEy(0),BUTTON(0),
	MOUSEST(0),mousex(0),mousey(0),mouseflag(0),pano360renderw(0),pano360renderH(0),pano360renderLux(0),pano360renderLuy(0),
	CameraFov(0),maxtexture(0),pano360texturenum(0),pano360texturewidth(0),pano360textureheight(0),selecttexture(0),shotcutnum(0),
	movviewx(0),movviewy(0),movvieww(0),movviewh(0),menumode(0),tailcut(0),radarinner(3.0),radaroutter(10),viewfov(90),viewfocus(10),
	osdmenushow(0),osdmenushowpre(0)
	{
		displayMode=SINGLE_VIDEO_VIEW_MODE;
		
		panosrcwidth=0;
		panosrcheight=0;
		panowidth=0;
		panoheight=0;
		rotsita=0;
		Fullscreen=0;
		renderwidth=1920;
		renderheight=1080;
		Panpicenum=0;
		shotcut=0;
		seamid=0;
		SeamEable=0;
		Pano360frameId=0;
		Seampostion=0;
		pano360subwidth=0;
		pano360subheight=0;
		pano360subx=0;
		pano360suby=0;
		panoAngle=0;
		panoPreposx=0;
		positionchanged=0;
		panoposx=0;
		memset(panopositon,0,BRIDGENUM*sizeof(Rect));
		for(int i=0;i<BRIDGENUM;i++)
			{
				panopositon[i].y=PANOEXTRAH/2;
				
			}
		//CARS
		carsnum=0;
		memset(carspositon,0,CARSNUM*sizeof(Rect));
		for(int i=0;i<CARSNUM;i++)
			{
				carspositon[i].y=PANOEXTRAH/2;
				
			}
		
		for(int i=0;i<8;i++)
		memset(vrectBatch[i],0,sizeof(GLfloat)*3);


		memset(viewcamera,0,sizeof(viewcamera));
		viewcamera[RENDERCAMERA1].active=1;
		pthis=this;
		//viewcamera[RENDERCAMERA1].updownselcectrect=

		
		
	}
Render::~Render()
	{


	}
bool Render::LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	
	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if(pBits == NULL) 
		return false;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
				 eFormat, GL_UNSIGNED_BYTE, pBits);
	
    free(pBits);
    
    if(minFilter == GL_LINEAR_MIPMAP_LINEAR || 
       minFilter == GL_LINEAR_MIPMAP_NEAREST ||
       minFilter == GL_NEAREST_MIPMAP_LINEAR ||
       minFilter == GL_NEAREST_MIPMAP_NEAREST)
        glGenerateMipmap(GL_TEXTURE_2D);
    
	return true;
}

void Render::ptzinit()
{
	initptzpos(0,360-7.3);
	double zeroangle=360-7.3;
	
	while(PTZOK)
		{
			double angle=0;
			angle=getpanotitle();
			double angleoffet=angle-zeroangle;
			if(angleoffet>300)
				angleoffet=angleoffet-360;
			if(angleoffet<-300)
				angleoffet=angleoffet+360;
			if(abs(angleoffet)<0.2)
				break;
			OSA_waitMsecs(1000);		
		}

}



void Render::Menuinti()
{
	//Menu.submenu[SINGLESHOW].renderfun
	if(Menu==NULL)
		return;
	Menu->setcallback(SINGLESHOW, Menucall);
	Menu->setcallback(PANOSHOW, Menucall);
	Menu->setcallback(SELECTSHOW, Menucall);
	Menu->setcallback(ZEROSHOW, Menucall);

}
void Render::SetupRC(int windowWidth, int windowHeight)
{
		// Blue background
	GLint max;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
	maxtexture=max;
	CameraFov=2*atan2(Config::getinstance()->getpanoprocesswidth(),2*Config::getinstance()->getcamfx())*180/3.141592653;

	viewprojectlen=tan(viewfov*3.141592653/2/180)*viewfocus;
	printf("*******************************the biggest texture is %d ***viewprojectlen=%f****viewfov=%f****viewfocus=%f********************\n",maxtexture,viewprojectlen,viewfov,viewfocus);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    
	shaderManager.InitializeStockShaders();

	loadpanopicture();
	Textureinit();
	singleViewInit();
	panotestViewInit();
	Pano360init();
	ptzinit();
	
	//printf("*******************\n");
	cameraFrame.MoveForward(-7.0f);
	Menu=MENU::getinstance();
	Menuinti();

	/********osd**********/
	Glosdhandle.create();
	Glosdhandle.setwindow(windowWidth, windowHeight);
	Glosdhandle.createunicode("/home/ubuntu/default.ttf", 40, 512, 512);

	//#include "FileRW.hpp"

	createfile();
	writefilehead();


}

void Render::ShutdownRC()
   {
	  glDeleteTextures(TEXTUREMAX, textureID);

   }


 void Render::ReSizeGLScene(int Width, int Height)
{
	GLfloat fAspect;
	renderwidth=Width;
	renderheight=Height;
	glViewport(0, 0, Width, Height);
	fAspect = (GLfloat)Width/(GLfloat)Height;


    // Produce the perspective projection
	viewFrustum.SetPerspective(viewfov,fAspect,1.0,120.0);
   	 projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

 
void Render::mouseButtonPress(int button, int state, int x, int y)
{
	printf(" mouse--> %i %i %i %i\n", button, state, x, y);
	setMouseCor(x,y);
	setMouseButton(button);
	setMousestatue(state);

	
	//if(y>renderheight*4/7)
	//	return;
	
	
	if(getmenumode()==SELECTMODE)
		MouseSelectpos();
	if(getmenumode()==SELECTZEROMODE)
		Mousezeropos();
	//else if(getmenumode()==PANOMODE)
		Mouse2Select();

	viewcameraprocess();

	Mousemenu();
}
 void Render::specialkeyPressed (int key, int x, int y)
{
	ProcessSpecialOitKeys(key, x, y);
}
/* The function called whenever a key is pressed. */
 void Render::keyPressed(unsigned char key, int x, int y)
{
	/* Keyboard debounce */
	/* I don't know which lib has this in win32 */
	usleep(100);


	/* Pressing escape kills everything --Have a nice day! */
	if (key == ESCAPE)
	{
		/* shut down our window */
		glutDestroyWindow(window);

		/* exit the program...normal termination. */
		exit(0);
	}
	ProcessOitKeys(key, x, y);
}
#define DISPLAYMODE_SWITCH_TO(mode) 		{\
			DISPLAYMODE nextMode = mode;\
			displayMode = nextMode;\
		}

	void Render::ProcessSpecialOitKeys(unsigned char key, int x, int y)
		{
			
			switch(key)
				{
					
					case SPECIAL_KEY_UP:
						if(panopositon[Panpicenum].y<PANOEXTRAH)
							panopositon[Panpicenum].y+=2;
						break;
					case SPECIAL_KEY_DOWN:
						if(panopositon[Panpicenum].y>5)
							panopositon[Panpicenum].y-=2;
						break;
					case SPECIAL_KEY_LEFT:
						if(panopositon[Panpicenum].x>1)
							{
							panopositon[Panpicenum].x-=1;
							panopositon[Panpicenum+1].x=panopositon[Panpicenum].x;
							}
						
						break;
					case SPECIAL_KEY_RIGHT:
						panopositon[Panpicenum].x+=1;
						panopositon[Panpicenum+1].x=panopositon[Panpicenum].x;
						rotsita=(rotsita+5000);
						setPanoAngle(rotsita);

						//setSeamPos(rotsita*panowidth/360-getPanoPrepos());
						OSA_printf("[fun=%s line=%d] rotsita=%d \n",__func__,__LINE__,rotsita);
						break;
					case GLUT_KEY_F1:
						Panpicenum=(Panpicenum+1)%BRIDGENUM;
						break;
					case GLUT_KEY_F2:
						Panpicenum=(Panpicenum-1+BRIDGENUM)%BRIDGENUM;
						break;
					case GLUT_KEY_F5://singprint
						shotcut=1;
						shotcutnum++;
						break;
					case GLUT_KEY_F7://singprint
						setfilestoreenable(1);
						break;
					case GLUT_KEY_F8://singprint
						setfilestoreenable(0);
						destoryfile();
						break;
					case GLUT_KEY_F9://singprint
						setpanoflagenable(1);
				
						break;
					case GLUT_KEY_F10://singprint
						setpanoflagenable(0);
					
						break;
					
			
					default:
						break;


				}


			
		}

void Render::ProcessOitKeys(unsigned char key, int x, int y)
{
	printf("the x=%d y=%d\n",x,y);
	switch(key)
		{
			case 's':
				//DISPLAYMODE_SWITCH_TO(SINGLE_VIDEO_VIEW_MODE);
				displayMode=SINGLE_VIDEO_VIEW_MODE;
				setmenumode(SINGLEMODE);
				break;
			case 'm':
				//DISPLAYMODE_SWITCH_TO(MY_TEST_PANO_MODE);
				displayMode=MY_TEST_PANO_MODE;
				break;
			case 'p':
				//DISPLAYMODE_SWITCH_TO(MY_TEST_PANO_MODE);
				
				panomod();
				break;
			case 'o':
				//DISPLAYMODE_SWITCH_TO(MY_TEST_PANO_MODE);
				
				selectmod();
				break;
			case 'z':
				zeromod();
				break;
			case 'F':

				if(Fullscreen){
				Fullscreen = false;
				glutReshapeWindow(renderwidth, renderheight);
				glutPostRedisplay();
				}
			else{
				Fullscreen = true;
				glutFullScreen();
				}	
				break;
			case 'f':
				SeamEable=1;
				setfusionenalge(SeamEable);
				break;
			case 'd':
				SeamEable=0;
				setfusionenalge(SeamEable);
				break;
			case 'j':
//				shotcut=(shotcut+1)%2;
				break;
			case 'r':
				setpanoscan();
				break;
			case 'e':
				setpanoantiscan();
				break;
			case 't':
				setpanoscanstop();
				break;
			case '2':
				setpanopanpos(0);
				break;
			case '4':
				setpanopanpos(90);
				break;
			case '6':
				setpanopanpos(180);
				break;
			case '8':
				setpanopanpos(270);
				break;
			
			
	
			default:
				break;


		}
}


void Render::Menucall(void * contex)
{
	int mod=*(int *)contex;
	switch(mod)
		{
			case SINGLESHOW:
				pthis->displayMode=SINGLE_VIDEO_VIEW_MODE;
				pthis->setmenumode(SINGLEMODE);
				break;
			case PANOSHOW:
				pthis->panomod();
			
				break;
			case SELECTSHOW:
				pthis->selectmod();
				break;
			case ZEROSHOW:
				pthis->zeromod();
				break;



		}
	
}
void Render::RenderScene(void)
	{

    
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//printf("11111111111111\n");
		switch(displayMode)
		{
			case SINGLE_VIDEO_VIEW_MODE:
				singleView(0,0,renderwidth,renderheight);
				break;
			case MY_TEST_PANO_MODE:
				panotestView(0,0,renderwidth,renderheight);
				break;
			case PANO_360_MODE:
				pano360View(0,0,renderwidth,renderheight);
				break;
			default:
				break;
		}
	movMultidetectrect();
	Drawfusion();
	Drawosd();


	// Perform the buffer swap to display back buffer
	glutSwapBuffers();
	}
void Render::selectmod()
{
	
	displayMode=PANO_360_MODE;
	setmenumode(SELECTMODE);
	setpanoscanstop();
	setscanpanflag(0);
}

void Render::zeromod()
{
	
	displayMode=PANO_360_MODE;
	setmenumode(SELECTZEROMODE);
	setpanoscanstop();
	setscanpanflag(0);
}

void Render::panomod()
{
	double angle=0;
	if(getmenumode()==SELECTMODE)
		{
			double zeroangle=getptzzeroangle()-1;
			if(zeroangle<0)
				zeroangle+=360;
			setpanopanpos(zeroangle);
			while(1)
				{
					angle=getpanopan();
					double angleoffet=angle-zeroangle;
					if(angleoffet>300)
						angleoffet=angleoffet-360;
					if(angleoffet<-300)
						angleoffet=angleoffet+360;
					if(abs(angleoffet)<0.2)
						break;
					OSA_waitMsecs(1000);
					
				}
			zeroangle=getptzzerotitleangle();
			if(zeroangle<0)
				zeroangle+=360;
			setpanotitlepos(zeroangle);
			while(1)
				{
					angle=getpanotitle();
					double angleoffet=angle-zeroangle;
					if(angleoffet>300)
						angleoffet=angleoffet-360;
					if(angleoffet<-300)
						angleoffet=angleoffet+360;
					if(abs(angleoffet)<0.2)
						break;
					OSA_waitMsecs(1000);
					
				}
			
				setforcezeroprocess(1);

			
			
		}

	if(getmenumode()==SELECTZEROMODE)
		{
			setstichreset(1);

		}

	

	
	setpanoflagenable(1);
	setfusionenalge(1);
	displayMode=PANO_360_MODE;
	setmenumode(PANOMODE);
	setscanpanflag(1);
	setpanoscan();

}
void Render::singleViewInit(void)
{
	// Load up a triangle
	GLfloat vVerts[] = { -1.0f, 1.0f, 0.0f, 
		                  	    1.0f, 1.0f, 0.0f,
					    -1.0f, -1.0f, 0.0f,
					    1.0f, -1.0f, 0.0f,};

	GLfloat vTexCoords [] = { 0.0f, 0.0f,
		                      	      1.0f, 0.0f, 
						      0.0f, 1.0f ,
						      1.0,1.0};

	triangleBatch.Begin(GL_TRIANGLE_STRIP, 4, 1);
	triangleBatch.CopyVertexData3f(vVerts);
	triangleBatch.CopyTexCoordData2f(vTexCoords, 0);
	triangleBatch.End();



	

}


void Render::Panotexture(void)
{
	int width=255;
	int height=255;
	int texturewidth=0;
	int textureheith=0;
	double texnum=0;
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	
	// Read the texture bits
	#ifdef NULLPIC
	pBits = gltReadTGABits("stone.tga", &width, &height, &nComponents, &eFormat);
	if(pBits == NULL) 
		return ;
	#endif



	#ifdef BRIDGE
	eFormat=GL_BGR_EXT;
	width=768;
	height=1024;
	nComponents=3;
	#endif

	#ifdef CARS
	eFormat=GL_BGR_EXT;
	width=768;
	height=1024;
	nComponents=3;
	#endif

	#ifdef PANO360
	eFormat=GL_BGR_EXT;
	width=Config::getinstance()->getpanoprocesswidth();
	height=Config::getinstance()->getpanoprocessheight();
	nComponents=3;
	#endif
	
	
	//texturewidth=(360)*width/(camerafov*PANOSCALE)-PANOWDTEHENTEND;
	
	//texnum=pano360texturenum

	texturewidth=2*3.141592653*Config::getinstance()->getcamfx()/PANOSCALE;

	texnum=1.0*texturewidth/maxtexture;
	pano360texturenum=ceil(texnum);


	pano360texturewidth=texturewidth/pano360texturenum;

	
	
	printf("******the fov=%f****texturewidth=%d  pano360texturenum=%d\n",CameraFov,texturewidth,pano360texturenum);
	//texturewidth=1920*4;
	pano360textureheight=textureheith=height+PANOEXTRAH;

	setpanoparam(texturewidth,textureheith);
	OSA_printf("[fun=%s line=%d] w=%d h=%d pw=%d ph=%d\n",__func__,__LINE__,width,height,texturewidth,textureheith);
	
	GLenum minFilter=GL_LINEAR;
	GLenum magFilter=GL_LINEAR;
	GLenum wrapMode=GL_CLAMP_TO_EDGE;


	for(int i=0;i<pano360texturenum;i++)
		{
			glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE+i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexImage2D(GL_TEXTURE_2D, 0, nComponents, pano360texturewidth, pano360textureheight, 0,
						 eFormat, GL_UNSIGNED_BYTE, NULL);
	
		}

	for(int i=0;i<PANODETECTNUM;i++)
		{
			if(PANOGRAYDETECT)
			cpuPANO[i]=Mat(textureheith,texturewidth,CV_8UC1,cv::Scalar(0));
			else
			cpuPANO[i]=Mat(textureheith,texturewidth,CV_8UC3,cv::Scalar(0));
		
		}
	setpanomap(cpuPANO);

	pano360texturew=texturewidth;
	pano360textureh=textureheith;
	
	panowidth=texturewidth;
	panoheight=textureheith;
	panosrcwidth=width;
	panosrcheight=height;
	panoformat=eFormat;
	
	

}

void Render::Selecttexture(void)
{
	GLenum minFilter=GL_LINEAR;
	GLenum magFilter=GL_LINEAR;
	GLenum wrapMode=GL_CLAMP_TO_EDGE;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	

}
void Render::Capturetexture(void)
{
	int width=255;
	int height=255;
	int texturewidth=0;
	int textureheith=0;
	
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	
	// Read the texture bits

	texturewidth=Config::getinstance()->getcamwidth();
	textureheith=Config::getinstance()->getcamheight();
	eFormat=GL_BGR_EXT;
	if(eFormat==GL_BGR_EXT)
		nComponents=3;
	else
		nComponents=1;
	
	GLenum minFilter=GL_LINEAR;
	GLenum magFilter=GL_LINEAR;
	GLenum wrapMode=GL_CLAMP_TO_EDGE;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, texturewidth, textureheith, 0,
				 eFormat, GL_UNSIGNED_BYTE, NULL);

	
	

}
//need modify
void Render::Angle2pos()
{
	double angle=getPanoAngle();
	Mat dst=getCurrentFame();
	unsigned int xoffset=0;
	unsigned int yoffset=0;
	unsigned int width=0;
	unsigned int height=0;
	angle=(angle/ANGLESCALE);
	bool refineangle=true;
	while(refineangle)
		{
			if(angle>=360)
				angle-=360;
			else
				refineangle=false;
		}

	xoffset=PANOSCALE*angle*panowidth/360;
	if(xoffset>=PANOSHIFT)
	xoffset=xoffset-PANOSHIFT;
	//printf("the Angle2pos angle=%f\n",angle);
	yoffset=PANOEXTRAH/2;
	width=dst.cols;
	//printf("the Angle2pos angle=%f  width=%d\n",angle,width);
	height=panosrcheight;
	setPanoSubPos(xoffset,yoffset,width,height);
	
	
	if(panopostionchanged())
		setPanoPrepos(getPanopos());

	setPanopos(xoffset);

}

void Render::getnumofpano360texture(int startx,int endx,int *texturestart,int *textureend)
{
	*texturestart=(startx/pano360texturewidth)%pano360texturenum;
	*textureend=(endx/pano360texturewidth)%pano360texturenum;

}
void Render::Pano360fun()
{
	int textureFnum=0;
	int textureSnum=0;
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	Mat currentdis;
	Mat combineframe;
	unsigned int xoffset=0;
	unsigned int yoffset=0;
	unsigned int width=0;
	unsigned int height=0;
	currentdis=getcurrentDis();
	Mat combin1;
	Mat combin2;
	
	//Angle2pos();
	int tailcut=gettailcut();

	if(CYLINDER)
	if(tailcut>PANODSTTAILSHIFT)
		tailcut=tailcut-PANODSTTAILSHIFT;
	getPanoSubPos(&xoffset,&yoffset,&width,&height);
	
	panoformat=GL_BGR_EXT;
	pBits=(GLbyte *)currentdis.data;
	
	getnumofpano360texture(xoffset,xoffset+width,&textureFnum,&textureSnum);
	
	if(textureFnum==textureSnum)
		{

			if(tailcut!=0&&textureFnum==0&&xoffset==0)
				{
					currentdis(Rect(0,0,tailcut,currentdis.rows)).copyTo(combin1);
					pBits=(GLbyte *)combin1.data;
					width=tailcut;
					//printf("the cuttail render ok width=%d tailcut=%d\n",width,tailcut);
					
				}
			glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE+textureFnum]);	
			glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset-textureFnum*pano360texturewidth, yoffset, width,height, panoformat, GL_UNSIGNED_BYTE, pBits);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	else
		{
			
			currentdis(Rect(0,0,(textureFnum+1)*pano360texturewidth-xoffset,currentdis.rows)).copyTo(combin1);
			
			currentdis(Rect((textureFnum+1)*pano360texturewidth-xoffset,0,currentdis.cols-(textureFnum+1)*pano360texturewidth+xoffset,currentdis.rows)).copyTo(combin2);

			glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE+textureFnum]);	
			glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset-textureFnum*pano360texturewidth, yoffset, combin1.cols,height, panoformat, GL_UNSIGNED_BYTE, combin1.data);
			glBindTexture(GL_TEXTURE_2D, 0);
			
			glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE+textureSnum]);	
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, yoffset, combin2.cols,height, panoformat, GL_UNSIGNED_BYTE, combin2.data);
			glBindTexture(GL_TEXTURE_2D, 0);

			


		}


	//OSA_printf("textureFnum=%d textureSnum=%d \n",textureFnum,textureSnum);

	for(int i=textureFnum;i<textureSnum+1;i++)
		{
			if(textureSnum==pano360texturenum)
			glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE]);
			else
			glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE+i]);
			
			glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset-i*pano360texturewidth, yoffset, width,height, panoformat, GL_UNSIGNED_BYTE, pBits);
			glBindTexture(GL_TEXTURE_2D, 0);
			
		}
	//OSA_printf("[F=%s L=%d] x=%d y=%d w=%d h=%d p=%p\n",__func__,__LINE__,xoffset,yoffset,width,height,pBits);
	
	
	
//	OSA_printf("[F=%s L=%d]\n",__func__,__LINE__);


}

void Render::pano(int num)
{

	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	int offsetstatus=0;
	int imgxoffset =0;
	int imgyoffset=0;
#ifdef NULLPIC
	pBits = gltReadTGABits("stone.tga", &nWidth, &nHeight, &nComponents, &eFormat);
	if(pBits == NULL) 
		return ;
#endif

#ifdef BRIDGE
	
	panoformat=GL_BGR_EXT;
	Mat dst(bridge[num].rows,bridge[num].cols,bridge[num].type());
	if(bridge[num].rows==0||bridge[num].cols==0||bridge[num].data==NULL)
		return ;
	cylinderproject(bridge[num],dst,bridge[num].cols+250);
	pBits=(GLbyte *)dst.data;
#endif

#ifdef CARS
	pBits=(GLbyte *)cars[num].data;
	panoformat=GL_BGR_EXT;
	if(cars[num].rows==0||cars[num].cols==0||cars[num].data==NULL)
		return ;
	Mat dst(cars[num].rows,cars[num].cols,cars[num].type());
#endif


#ifdef PANO360
	panoformat=GL_BGR_EXT;
	if(pano360[num].rows==0||pano360[num].cols==0||pano360[num].data==NULL)
		return ;
	Mat dst(pano360[num].rows,pano360[num].cols,pano360[num].type());
	

	if(CYLINDERPROJECTION)
		{
			if(AUTOMOV)
			cylinderproject(Capture,pano360temp,1.0*Config::getinstance()->getcamfx()*Config::getinstance()->getpanoprocesswidth()/Config::getinstance()->getcamwidth());
			else
			cylinderproject(pano360[num],pano360temp,1.0*Config::getinstance()->getcamfx()*Config::getinstance()->getpanoprocesswidth()/Config::getinstance()->getcamwidth());
			//cylinderproject(pano360[num],pano360temp,1.0*CAMERAFOCUSLENGTH*PANO360SRCWIDTH/PANO360WIDTH);
			//cylinderproject(pano360[num],pano360temp,1.0*CAMERAFOCUSLENGTH);
			cylindercut(pano360temp,dst,100);
			//resize(dst,dst,Size(PANO360WIDTH,PANO360HEIGHT),0,0,INTER_LINEAR);
			pBits=(GLbyte *)dst.data;
		}
	else
		{

		if(SeamEable)
				{
					if(num>0)
						{
							BestSeam(pano360[num-1], pano360temp, panopositon[num].x-panopositon[num-1].x);
							pBits=(GLbyte *)pano360temp.data;

						}
					else
						pBits=(GLbyte *)pano360[num].data;
					
				
				}
		else
		pBits=(GLbyte *)pano360[num].data;

		if(AUTOMOV)
			pBits=(GLbyte *)Capture.data;

		}
	//imshow("catpure",Capture);


#endif
	if(num>0)
		{
			offsetstatus=getPano360Offset(pano360[num-1],pano360[num],&imgxoffset,&imgyoffset);
			printf("the xoffset=%d yoffset=%d\n",imgxoffset,imgyoffset);
			
			if(offsetstatus==0)
				{
					panopositon[num].x=panopositon[num].x+imgxoffset;
					panopositon[num+1].x=panopositon[num].x;
					panopositon[num].y=panopositon[num].y+imgyoffset;
					panopositon[num+1].y=panopositon[num].y;
				}

		}

	
	
	/*
	//pBits=bridge[]
	imshow("test",bridge[Panpicenum]);
	waitKey(1);
	*/
	int xoffset=0;
	int yoffset=0;
	int width=0;
	int height=0;
	width=panosrcwidth;
	height=panosrcheight;
	
	yoffset=PANOEXTRAH/2;

	if(AUTOMOV)
	xoffset=rotsita*panowidth/360;
	else
	{
		xoffset=panopositon[num].x;
		//xoffset=0;
		yoffset=panopositon[num].y;
	}
	//OSA_printf("[fun=%s line=%d] \n",__func__,__LINE__);
	if(pBits==NULL)
		return;
	glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, panosrcwidth,panosrcheight, panoformat, GL_UNSIGNED_BYTE, pBits);

}
void Render::Textureinit(void)
{
	//test texture
	glGenTextures(TEXTUREMAX, textureID);
	
	glBindTexture(GL_TEXTURE_2D, textureID[TESTTEXTURE]);
	LoadTGATexture("stone.tga", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	
	
	Panotexture();

	glBindTexture(GL_TEXTURE_2D, textureID[CAPTEXTURE]);
	Capturetexture();
	
	glBindTexture(GL_TEXTURE_2D, textureID[SELECTTEXTURE]);
	Selecttexture();
	
	

	

}

void Render::Pano360init()
{
	for(int i=0;i<MAXSEAM;i++)
	{
		if(CYLINDER)
		Seamframe[i]=Mat(Config::getinstance()->getpanoprocessheight(),Config::getinstance()->getpanoprocesswidth()-PANOSRCSHIFT-PANOCYLINDCUT,CV_8UC3,cv::Scalar(0,0,0));
		else
		Seamframe[i]=Mat(Config::getinstance()->getpanoprocessheight(),Config::getinstance()->getpanoprocesswidth()-PANOSRCSHIFT,CV_8UC3,cv::Scalar(0,0,0));
		
	}
	fusionframe=Mat(Config::getinstance()->getpanoprocessheight(),Config::getinstance()->getpanoprocesswidth(),CV_8UC3,cv::Scalar(0,0,0));
	Pano360tempframe=Mat(Config::getinstance()->getpanoprocessheight(),Config::getinstance()->getpanoprocesswidth(),CV_8UC3,cv::Scalar(0,0,0));

	viewWarningdata[0]=(unsigned char *)malloc(Config::getinstance()->getdisheight()*Config::getinstance()->getdiswidth()*4);
	for(int i=0;i<Config::getinstance()->getdisheight();i++)
		for(int j=0;j<Config::getinstance()->getdiswidth()*4;j=j+4)
			{
				viewWarningdata[0][0]=0;
				viewWarningdata[0][1]=255;
				viewWarningdata[0][2]=0;
				viewWarningdata[0][3]=30;


			}
	viewWarningarea[0]=Mat(Config::getinstance()->getdisheight(),Config::getinstance()->getdiswidth(),CV_8UC4,viewWarningdata[0]);

	

}
void Render::singleView(int x,int y,int width,int height)
{
	char numflame[100];
	 glViewport(0,0,width,height);
	 modelViewMatrix.PushMatrix();
        modelViewMatrix.Translate(0.0f, 0.0f, -10);

        glBindTexture(GL_TEXTURE_2D, textureID[CAPTEXTURE]);
	#if 0
        shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
                                     transformPipeline.GetModelViewMatrix(),
                                     transformPipeline.GetProjectionMatrix(), 
                                     vLightPos, vWhite, 0);
	#else

	M3DMatrix44f identy;
	m3dLoadIdentity44(identy);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, identy, 0);
	#endif
    	triangleBatch.Draw();	
	modelViewMatrix.PopMatrix();

	glUseProgram(0);
	glColor3f(0.0, 0.0, 1.0);
    	glRasterPos2f(0.5, -0.9);
	sprintf(numflame,"image cutnum :%d\n",shotcutnum);
      glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)numflame);
	//
	testcylinder();

}

void Render::Drawmov()
{
	glViewport(0,0,renderwidth,renderheight);
	Glosdhandle.setwindow(renderwidth,renderheight);
	Glosdhandle.drawbegin();


	Glosdhandle.drawcross(renderwidth/2,renderheight/2,40,40);
	//Glosdhandle.drawrect(detect_vect360[i].x, detect_vect360[i].y, detect_vect360[i].width, detect_vect360[i].height);
	
	Glosdhandle.drawend();

}
wchar_t str[50]=L"wnagjie";

void Render::Drawmenuupdate()
{
	Osdcontext *contxt=osdcontex.getOSDcontex();
	for(int i=SINGLEMODE;i<MAXMODE;i++)
		{
			if(getmenumode()==i)
				contxt[i].display=DISMODE;
			else
				contxt[i].display=HIDEMODE;

		}
     if(getplantformcalibration())
	 	contxt[CALIBRATION].display=HIDEMODE;
	 else
	 	contxt[CALIBRATION].display=DISMODE;


	Rect upleft;
	for(int i=RENDERCAMERA1;i<=RENDERCAMERMAX;i++)
			{
				if(viewcamera[i].active==1)
					{
						//printf("the viewcamera=%d\n",i);
						leftdown2leftup(viewcamera[i].leftdownrect,upleft);
						contxt[SELECTING].x=upleft.x+upleft.width-150;
						contxt[SELECTING].y=upleft.y;

					}
				
			
			}



 if(getplantformcalibration()&&getmenumode()==PANOMODE)
 	{
    if(getmodeling())
		contxt[MODELING].display=DISMODE;
	else
		contxt[MODELING].display=HIDEMODE;
	contxt[SELECTING].display=DISMODE;
 	}
 else
 	{
		contxt[SELECTING].display=HIDEMODE;

 	}
 
	

}


void Render::Drawosdmenu()
{

	glViewport(0,0,renderwidth,renderheight);
	Glosdhandle.setwindow(renderwidth,renderheight);
	Glosdhandle.drawunicodebegin();
	/***********OSD DRAW*******************/
	Rgba colour=Rgba(255,255,255,255);


	for(int i=0;i<MENUMAX;i++)
		{
			if(Menu->submenu[i].displayvalid==DISMODE)
				{
					if(Menu->submenu[i].active==ACTIVE)
						Glosdhandle.setcolorunicode(REDColour);
					else
					Glosdhandle.setcolorunicode(Menu->submenu[i].colour);
					Glosdhandle.drawunicode(Menu->submenu[i].x,Menu->submenu[i].y,colour,Menu->submenu[i].context);
				}
		}
	Glosdhandle.drawunicodeend();







	/***********PANEL DRAW*******************/
	glEnable(GL_BLEND);
   	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Glosdhandle.setwindow(renderwidth,renderheight);
	Glosdhandle.setcolorline(GREY);
	Glosdhandle.setcolorlinealpha(0.2);
	Glosdhandle.drawbegin();

	for(int i=FIRSTLEVEL;i<LEVELMAX;i++)
		{
			if(Menu->panelmenu[i].displayvalid==DISMODE)
				{
					Glosdhandle.drawrectfill(Menu->panelmenu[i].x,Menu->panelmenu[i].y,Menu->panelmenu[i].w,Menu->panelmenu[i].h);
				}
		}

	Glosdhandle.drawend();
	
	glDisable(GL_BLEND);


}
void Render::Drawmenu()
{
	glViewport(0,0,renderwidth,renderheight);
	Glosdhandle.setwindow(renderwidth,renderheight);
	Glosdhandle.drawunicodebegin();
	Rgba colour=Rgba(255,255,255,255);
	Osdcontext *contxt=osdcontex.getOSDcontex();
	Drawmenuupdate();
	for(int i=0;i<MAXCONTXT;i++)
		{
			if(contxt[i].display==DISMODE)
				{
					
					Glosdhandle.setcolorunicode(contxt[i].colour);
					Glosdhandle.drawunicode(contxt[i].x,contxt[i].y,colour,contxt[i].context);
				}
		}
	Glosdhandle.drawunicodeend();
	//glUseProgram(0);

}

void Render::Drawlines()
{
	glViewport(0,0,renderwidth,renderheight);
	Glosdhandle.setwindow(renderwidth,renderheight);
	Glosdhandle.setcolorline(GLBLUE);
	Glosdhandle.drawbegin();
	/*
	if(getmenumode()==PANOMODE)
	for(int i=RENDERCAMERA1;i<RENDERCAMERMAX;i++)
		{
			Rect rect=viewcamera[i].updownselcectrect;
			Glosdhandle.drawrect(rect.x,rect.y,rect.width,rect.height);
		}
	//Glosdhandle.drawrect(detect_vect360[i].x, detect_vect360[i].y, detect_vect360[i].width, detect_vect360[i].height);
	*/
	//Glosdhandle.drawloops(detect_vectradarpoints);
	Glosdhandle.drawend();
	//glUseProgram(0);

}
void Render::movMultidetectrect()
{
	std::vector<cv::Rect>	detect_temp;	
	std::vector<cv::Rect>	detect_vectcombination;
	detect_vect180.clear();
	detect_vect360.clear();
	for(int i=0;i<MULTICPUPANONUM;i++)
		{
			getmvdetect(detect_temp,i);
			mvdetectup(detect_temp);
			Multipotionto360(detect_temp,i);
			mvclassification(detect_temp,detect_vect180,detect_vect360);
			//detect_vect360.insert(detect_vect360.end(),detect_temp.begin(),detect_temp.end());
		}

}

void Render::DrawmovMultidetect()
{
	unsigned int pan360w=pano360texturew;
	unsigned int pan360whalf=pano360texturew/2;
	
	
	Glosdhandle.setcolorline(GLRED);
	
	//cout<<"***************DrawmovMultidetect***********************"<<detect_vect180.size()<<endl;
	int size=detect_vect180.size();
	if(size!=0)
	glViewport(mov180viewx,mov180viewy,mov180vieww,mov180viewh);
	Glosdhandle.setwindow(pan360whalf,pano360textureh);
		Glosdhandle.drawbegin();
	for(int i=0;i<size;i++)
		{
			Glosdhandle.drawrect(detect_vect180[i].x, detect_vect180[i].y, detect_vect180[i].width, detect_vect180[i].height);
		}
	Glosdhandle.drawend();

	size=detect_vect360.size();
	if(size!=0)
	glViewport(mov360viewx,mov360viewy,mov360vieww,mov360viewh);
	else
		return;
	Glosdhandle.setwindow(pan360whalf,pano360textureh);
		Glosdhandle.drawbegin();
	for(int i=0;i<size;i++)
		{
			Glosdhandle.drawrect(detect_vect360[i].x, detect_vect360[i].y, detect_vect360[i].width, detect_vect360[i].height);
		}
	Glosdhandle.drawend();
	

	
	
	
	

}
void Render::Drawmovdetect()
{
	glUseProgram(0);
	//#define DETECTTEST (1)
	if(DETECTTEST)
		{
			glViewport(0,0,PANO360WIDTH,PANO360HEIGHT);
			Glosdhandle.setwindow(PANO360WIDTH,PANO360HEIGHT);
		}
	else
		{
			glViewport(movviewx,movviewy,movvieww,movviewh);
			Glosdhandle.setwindow(PANO360WIDTH*2,PANO360HEIGHT);
		}


	//detect_vect.clear();
	//detect_vect.push_back(Rect(0,0,PANO360WIDTH,PANO360HEIGHT));
	getmvdetect(detect_vect,0);
	int size=detect_vect.size();
	if(size==0)
		return;

	if(MOVDETECTDOWENABLE)
		{
	for(int i=0;i<size;i++)
		{
			detect_vect[i].x=detect_vect[i].x*Config::getinstance()->getmvdownup();
			detect_vect[i].y=detect_vect[i].y*Config::getinstance()->getmvdownup();
			detect_vect[i].width=detect_vect[i].width*Config::getinstance()->getmvdownup();
			detect_vect[i].height=detect_vect[i].height*Config::getinstance()->getmvdownup();

		
		}
		}
		
	//1920*PANO360WIDTH*2/15000

	
	Glosdhandle.setcolorline(GLRED);
	Glosdhandle.drawbegin();
	for(int i=0;i<size;i++)
		{
			Glosdhandle.drawrect(detect_vect[i].x, detect_vect[i].y, detect_vect[i].width, detect_vect[i].height);


		}
	
	
	
	Glosdhandle.drawend();

}

void Render::Drawfusion()
{
	unsigned int lx,ly,w,h;
	double inradar=0;
	double outradar=0;
	double baseangle=0;
	double angle180=3.141592653;
	double angle360=3.141592653*2;
	double leftangle=0;
	double rightangle=0;
	Rect mvradar;
	Rect recttartget;
	Rect rectleftdownbase;

	vector<OSDPoint> osdpoints;
	double aspect=0;
	
	
	
	glViewport(0,0,renderwidth,renderheight);
	modelViewMatrix.PushMatrix();
       modelViewMatrix.Translate(0.0f, 0.0f, -viewfocus);
	glEnable(GL_BLEND);
   	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Glosdhandle.setwindow(renderwidth,renderheight);
	Glosdhandle.setcolorline(GLRED);
	Glosdhandle.setcolorlinealpha(0.2);
	Glosdhandle.drawbegin();
	
	if(getmenumode()==PANOMODE)
	for(int i=RENDERCAMERA1;i<=RENDERCAMERA3;i++)
		{
			Rect rect=viewcamera[i].updownselcectrect;
			Glosdhandle.drawrectfill(rect.x,rect.y,rect.width,rect.height);
		}
	//Glosdhandle.drawrect(detect_vect360[i].x, detect_vect360[i].y, detect_vect360[i].width, detect_vect360[i].height);
	
	Glosdhandle.drawend();
	

	if(getmenumode()==PANOMODE)
	{
		lx=viewcamera[RENDERRADER].leftdownrect.x;
		ly=viewcamera[RENDERRADER].leftdownrect.y;
		w=viewcamera[RENDERRADER].leftdownrect.width;
		h=viewcamera[RENDERRADER].leftdownrect.height;
		glViewport(lx,ly,w,h);



		static GLfloat red[] = { 1.0f, 0.0f, 0.0f, 0.2f };
		shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), red);

		for(int i=RENDERCAMERA1;i<=RENDERCAMERA3;i++)
		{
			recttartget=viewcamera[i].updownselcectrect;

			if(viewcamera[i].panotextureindex==0)
				{
					rectleftdownbase=viewcamera[RENDER180].leftdownrect;
					baseangle=angle180;
				}
			else
				{
					rectleftdownbase=viewcamera[RENDER360].leftdownrect;
					baseangle=angle360;
				}
			
			h=rectleftdownbase.height;
			if(h==0)
				h=1;
			w=rectleftdownbase.width;
			if(w==0)
				w=1;
			inradar=radarinner+(renderheight-(recttartget.y+recttartget.height)-rectleftdownbase.y)*(radaroutter-radarinner)/h;
			outradar=radarinner+(renderheight-(recttartget.y)-rectleftdownbase.y)*(radaroutter-radarinner)/h;


			

			leftangle=baseangle-recttartget.x*angle180/w;
			rightangle=baseangle-(recttartget.x+recttartget.width)*angle180/w;
			
			gltMakeradar(radarcamera[i], inradar, outradar, 30, 3,leftangle,rightangle);
			
			
			
		}
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			for(int i=RENDERCAMERA1;i<=RENDERCAMERA3;i++)
			{
				radarcamera[i].Draw();
			}
		int pre180num=0;

		for(int i=0;i<detect_vect180.size();i++)
			{
				recttartget=detect_vect180[i];
				recttartget.x=recttartget.x*viewcamera[RENDER180].leftdownrect.width*2/panowidth;
				recttartget.y=recttartget.y*viewcamera[RENDER180].leftdownrect.height/panoheight;
				recttartget.width=recttartget.width*viewcamera[RENDER180].leftdownrect.width*2/panowidth;
				recttartget.height=recttartget.height*viewcamera[RENDER180].leftdownrect.height/panoheight;

				//cout<<recttartget<<endl;
				rectleftdownbase=viewcamera[RENDER180].leftdownrect;
				baseangle=angle180;
			
				h=rectleftdownbase.height;
				if(h==0)
					h=1;
				w=rectleftdownbase.width;
				if(w==0)
					w=1;
				inradar=radarinner+1.0*(h-(recttartget.y+recttartget.height))*(radaroutter-radarinner)/h;
				

				//printf("(h-(recttartget.y+recttartget.height))=%frecttartget.y+recttartget.height=%d  h=%d",1.0*(h-(recttartget.y+recttartget.height)),recttartget.y+recttartget.height,h);
				
				outradar=radarinner+(h-(recttartget.y))*(radaroutter-radarinner)/h;

				leftangle=baseangle-recttartget.x*angle180/w;
				rightangle=baseangle-(recttartget.x+recttartget.width)*angle180/w;
				gltMakeradarpoints(detect_vectradarpoints[i], inradar, outradar, 2,leftangle,rightangle);
				pre180num=i;
				//mvradar.x=
				//printf("iner =%f outer=%f langle=%f rightalgle=%f radaroutter=%f   radarinner=%f  %d   %d\n",inradar,outradar,leftangle,rightangle,radaroutter,radarinner,w,h);

			}
		for(int i=0;i<detect_vect360.size();i++)
			{
				recttartget=detect_vect360[i];
				recttartget.x=recttartget.x*viewcamera[RENDER360].leftdownrect.width*2/panowidth;
				recttartget.y=recttartget.y*viewcamera[RENDER360].leftdownrect.height/panoheight;
				recttartget.width=recttartget.width*viewcamera[RENDER360].leftdownrect.width*2/panowidth;
				recttartget.height=recttartget.height*viewcamera[RENDER360].leftdownrect.height/panoheight;

				//cout<<recttartget<<endl;
				rectleftdownbase=viewcamera[RENDER360].leftdownrect;
				baseangle=angle360;
			
				h=rectleftdownbase.height;
				if(h==0)
					h=1;
				w=rectleftdownbase.width;
				if(w==0)
					w=1;
				inradar=radarinner+1.0*(h-(recttartget.y+recttartget.height))*(radaroutter-radarinner)/h;

				//printf("(h-(recttartget.y+recttartget.height))=%frecttartget.y+recttartget.height=%d  h=%d",1.0*(h-(recttartget.y+recttartget.height)),recttartget.y+recttartget.height,h);
				
				outradar=radarinner+(h-(recttartget.y))*(radaroutter-radarinner)/h;

				leftangle=baseangle-recttartget.x*angle180/w;
				rightangle=baseangle-(recttartget.x+recttartget.width)*angle180/w;
				gltMakeradarpoints(detect_vectradarpoints[pre180num+i], inradar, outradar, 2,leftangle,rightangle);
				//pre180num=i;
				//mvradar.x=
				//printf("iner =%f outer=%f langle=%f rightalgle=%f radaroutter=%f   radarinner=%f  %d   %d\n",inradar,outradar,leftangle,rightangle,radaroutter,radarinner,w,h);

			}


		


		

		
	}
	glDisable(GL_BLEND);
	/*
	OSDPoint testpoint;
	detect_vectradarpoints.clear();
	testpoint.x=5;
	testpoint.y=0;
	detect_vectradarpoints.push_back(testpoint);
	testpoint.x=6;
	testpoint.y=0;
	detect_vectradarpoints.push_back(testpoint);
	testpoint.x=6;
	testpoint.y=1;
	detect_vectradarpoints.push_back(testpoint);
	testpoint.x=5;
	testpoint.y=1;
	detect_vectradarpoints.push_back(testpoint);
	*/
	if(getmenumode()==PANOMODE)
	{
		lx=viewcamera[RENDERRADER].leftdownrect.x;
		ly=viewcamera[RENDERRADER].leftdownrect.y;
		w=viewcamera[RENDERRADER].leftdownrect.width;
		h=viewcamera[RENDERRADER].leftdownrect.height;
		glViewport(lx,ly,w,h);
		Glosdhandle.setwindow(w,h);
		Glosdhandle.setcolorline(GLRED);
		Glosdhandle.drawbegin();
		Glosdhandle.setlinewidth(1);
		for(int j=0;j<detect_vect180.size()+detect_vect360.size();j++)
			{
				for(int i=0;i<detect_vectradarpoints[j].size();i++)
					{
						detect_vectradarpoints[j][i].x=((detect_vectradarpoints[j][i].x)/viewprojectlen)*h*1.0/w;
						
						detect_vectradarpoints[j][i].y=(detect_vectradarpoints[j][i].y)*1.0/viewprojectlen;

					}
					Glosdhandle.drawloops(detect_vectradarpoints[j]);
			}
	//	printf("viewprojectlen=%f\n",viewprojectlen);
		
			
		
		Glosdhandle.drawend();
	}
	modelViewMatrix.PopMatrix();



}
void Render::Drawosd()
{


	if(getmenumode()==PANOMODE)
		{
			if(MULTICPUPANO)
				DrawmovMultidetect();
			else
				Drawmovdetect();
		}

	if(DETECTTEST)
		Drawmovdetect();
	if(getmenumode()==SINGLEMODE)
		Drawmov();
	Drawmenu();

	Drawlines();
	Drawosdmenu();

	
	

/*
	wchar_t intext3[] = {
	L"X3APPPPPP\u8fdb\\n"
	};
	glUseProgram(0);
	char buffers[20];

	Glosdhandle.drawbegin();
	sprintf(buffers, "%d\n", 2);
	Glosdhandle.drawstrings(100,100,buffers);
	Glosdhandle.drawend();


	glUseProgram(0);
	Glosdhandle.drawunicodebegin();
	Glosdhandle.drawunicode(-100, 100, intext3);
	Glosdhandle.drawunicode(100, 100, intext3);
	Glosdhandle.drawunicodeend();
*/

}
void Render::pano360View(int x,int y,int width,int height)
{
	int startnum=0;
	unsigned int extrablackw=PANOEXTRAH/4;
	char numflame[30];
	M3DMatrix44f identy;
	bool seamfunsion=false;//getSeamEnable();
	unsigned int lx,ly,w,h;
	
	//setPano360RenderPos(lx,ly,w,h);

	
	modelViewMatrix.PushMatrix();
       modelViewMatrix.Translate(0.0f, 0.0f, -viewfocus);
	Angle2pos();
	OptiSeamfun();
	Pano360fun();
	/*
	if(seamfunsion)
		{
		rotsita=(rotsita+5)%360;
		setPanoAngle(rotsita);
		setSeamEnable(0);
		}

	*/
	

	
	/*************************************************************************/
	lx=0;
	ly=height*5/6;
	w=width;
	h=height*1/6;
	mov180viewx=lx;
	mov180viewy=ly;
	mov180vieww=w;
	mov180viewh=h;

	viewcamera[RENDER180].leftdownrect.x=lx;
	viewcamera[RENDER180].leftdownrect.y=ly;
	viewcamera[RENDER180].leftdownrect.width=w;
	viewcamera[RENDER180].leftdownrect.height=h;


	glViewport(lx,ly,w,h);
       glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE]);
	#if 0
        shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
                                     transformPipeline.GetModelViewMatrix(),
                                     transformPipeline.GetProjectionMatrix(), 
                                     vLightPos, vWhite, 0);
	#else

	
	m3dLoadIdentity44(identy);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, identy, 0);
	#endif
    	pan360triangleBatch.Draw();

	glBindTexture(GL_TEXTURE_2D, 0);



	lx=0;
	ly=height*4/6;
	w=width;
	h=height*1/6;
	mov360viewx=lx;
	mov360viewy=ly;
	mov360vieww=w;
	mov360viewh=h;

	
	movviewx=lx;
	movviewy=ly;
	movvieww=w;
	movviewh=h;


	viewcamera[RENDER360].leftdownrect.x=lx;
	viewcamera[RENDER360].leftdownrect.y=ly;
	viewcamera[RENDER360].leftdownrect.width=w;
	viewcamera[RENDER360].leftdownrect.height=h;
	glViewport(lx,ly,w,h);
       glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE+1]);
	#if 0
        shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
                                     transformPipeline.GetModelViewMatrix(),
                                     transformPipeline.GetProjectionMatrix(), 
                                     vLightPos, vWhite, 0);
	#else

	
	m3dLoadIdentity44(identy);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, identy, 0);
	#endif
    	pan360triangleBatch.Draw();

	glBindTexture(GL_TEXTURE_2D, 0);










	/*************************************************************************/



	setPano360RenderPos(lx,ly,w,h*2);


	/*************************************************************************/
		
	
	/**************************radar display***********************************************/
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	lx=width-width/3;
	ly=0;
	w=width/3;
	h=height*2/6;
	
	viewcamera[RENDERRADER].leftdownrect.x=lx;
	viewcamera[RENDERRADER].leftdownrect.y=ly;
	viewcamera[RENDERRADER].leftdownrect.width=w;
	viewcamera[RENDERRADER].leftdownrect.height=h;
	glViewport(lx,ly,w,h);
	
	//glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE+viewcamera[RENDERCAMERA1].panotextureindex]);
	glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE]);
	m3dLoadIdentity44(identy);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, identy, 0);
	static GLfloat vGreen[] = { 1.0f, 0.0f, 0.0f, 0.3f };
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), vGreen);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	radar180.Draw();


	glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE1]);
	m3dLoadIdentity44(identy);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, identy, 0);

	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), vGreen);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	radar360.Draw();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
/*

	glBindTexture(GL_TEXTURE_2D,0);
	
	//static GLfloat vGreen[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vGreen);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	radar180.Draw();
*/
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/**************************select display***********************************************/
	
	movviewx=width/2+extrablackw;
	movviewy=0;
	movvieww=width/2-extrablackw;
	movviewh=ly-extrablackw;

	lx=0;
	ly=0;
	w=width-width/3;
	h=height*2/6-extrablackw;

	viewcamera[RENDERCAMERA1].leftdownrect.x=lx;
	viewcamera[RENDERCAMERA1].leftdownrect.y=ly;
	viewcamera[RENDERCAMERA1].leftdownrect.width=w;
	viewcamera[RENDERCAMERA1].leftdownrect.height=h;
	glViewport(lx,ly,w,h);
	
	glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE+viewcamera[RENDERCAMERA1].panotextureindex]);
	
	m3dLoadIdentity44(identy);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, identy, 0);
	panselecttriangleBatch[RENDERCAMERA1].Draw();




	
	lx=width/2+extrablackw/2;
	ly=height*2/6;
	w=width/2-extrablackw/2;
	h=height*2/6-extrablackw;

	viewcamera[RENDERCAMERA3].leftdownrect.x=lx;
	viewcamera[RENDERCAMERA3].leftdownrect.y=ly;
	viewcamera[RENDERCAMERA3].leftdownrect.width=w;
	viewcamera[RENDERCAMERA3].leftdownrect.height=h;
	glViewport(lx,ly,w,h);
	//glBindTexture(GL_TEXTURE_2D, textureID[CAPTEXTURE]);
	glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE+viewcamera[RENDERCAMERA3].panotextureindex]);
	m3dLoadIdentity44(identy);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, identy, 0);
	panselecttriangleBatch[RENDERCAMERA3].Draw();
	//pansrctriangleBatch.Draw();
	selectupdate();
	glBindTexture(GL_TEXTURE_2D, 0);

	



	lx=0;
	ly=height*2/6;
	w=width/2-extrablackw/2;
	h=height*2/6-extrablackw;

	viewcamera[RENDERCAMERA2].leftdownrect.x=lx;
	viewcamera[RENDERCAMERA2].leftdownrect.y=ly;
	viewcamera[RENDERCAMERA2].leftdownrect.width=w;
	viewcamera[RENDERCAMERA2].leftdownrect.height=h;
	glViewport(lx,ly,w,h);
	
	glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE+viewcamera[RENDERCAMERA2].panotextureindex]);
	
	m3dLoadIdentity44(identy);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, identy, 0);
	panselecttriangleBatch[RENDERCAMERA2].Draw();








	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	/*************************************************************************/
	modelViewMatrix.PopMatrix();

	glUseProgram(0);
	glColor3f(0.0, 0.0, 1.0);
    	glRasterPos2f(0.5, -0.9);
	sprintf(numflame,"image cutnum :%d\n",shotcutnum);
	if(shotcutnum>0)
      glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)numflame);

}

void Render::panotestView(int x,int y,int width,int height)
{
	int startnum=0;
	char numflame[30];
	 M3DMatrix44f identy;
	 glViewport(0,height/2,width,height/2);
	 modelViewMatrix.PushMatrix();
       modelViewMatrix.Translate(0.0f, 0.0f, -10);
	//OptiSeamfun();
	if(AUTOMOV)
		{
			pano(0);

		}
	else
		{
	if(Panpicenum-1<0)
		startnum=0;
	else
		startnum=Panpicenum;
	
	 for(int i=startnum;i<Panpicenum+1;i++)
		 pano(i);
		}
        glBindTexture(GL_TEXTURE_2D, textureID[PANOTEXTURE]);
	#if 0
        shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
                                     transformPipeline.GetModelViewMatrix(),
                                     transformPipeline.GetProjectionMatrix(), 
                                     vLightPos, vWhite, 0);
	#else

	Panpicenum=(Panpicenum+1)%BRIDGENUM;

	
	m3dLoadIdentity44(identy);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, identy, 0);
	#endif
    	pan360triangleBatch.Draw();

	glBindTexture(GL_TEXTURE_2D, 0);

	glViewport(0,0,width/2,height/2);

	glBindTexture(GL_TEXTURE_2D, textureID[CAPTEXTURE]);
	
	m3dLoadIdentity44(identy);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, identy, 0);
	pansrctriangleBatch.Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	
	glViewport(width/2,0,width/2,height/2);
	glBindTexture(GL_TEXTURE_2D, textureID[CAPTEXTURE]);
	
	m3dLoadIdentity44(identy);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, identy, 0);
	pansrctriangleBatch.Draw();
	glBindTexture(GL_TEXTURE_2D, 0);
	modelViewMatrix.PopMatrix();

	glUseProgram(0);
	glColor3f(0.0, 0.0, 1.0);
    	glRasterPos2f(0.9, -0.5);
	sprintf(numflame,"%d\n",Panpicenum);
      glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)numflame);
	

}

void Render::panotestViewInit(void)
{

	GLfloat vVerts[] = { -1.0f, 1.0f, 0.0f, 
		                  	    1.0f, 1.0f, 0.0f,
					    -1.0f, -1.0f, 0.0f,
					    1.0f, -1.0f, 0.0f,};

	GLfloat vTexCoords [] = { 0.0f, 0.0f,
		                      	      1.0f, 0.0f, 
						      0.0f, 1.0f ,
						      1.0,1.0};

      GLfloat vTexselectCoords [] = { 0.0f, 0.0f,
		                      	      0.1f, 0.0f, 
						      0.0f, 1.0f ,
						      0.1,1.0};

	pan360triangleBatch.Begin(GL_TRIANGLE_STRIP, 4, 1);
	pan360triangleBatch.CopyVertexData3f(vVerts);
	pan360triangleBatch.CopyTexCoordData2f(vTexCoords, 0);
	pan360triangleBatch.End();

	pansrctriangleBatch.Begin(GL_TRIANGLE_STRIP, 4, 1);
	pansrctriangleBatch.CopyVertexData3f(vVerts);
	pansrctriangleBatch.CopyTexCoordData2f(vTexCoords, 0);
	pansrctriangleBatch.End();


	for(int i=0;i<SELECTMAX;i++)
		{
			panselecttriangleBatch[i].Begin(GL_TRIANGLE_STRIP, 4, 1);
			panselecttriangleBatch[i].CopyVertexData3f(vVerts);
			panselecttriangleBatch[i].CopyTexCoordData2f(vTexselectCoords, 0);
			panselecttriangleBatch[i].End();
		}

	double angle180=3.141592653;
	double angle360=3.141592653*2;
	gltMakeradar(radar180, radarinner, radaroutter, 360, 10,angle180,0);
	gltMakeradar(radar360, radarinner,radaroutter, 360, 10,angle360,angle180);
	
	//radaroutter
	


      panselectrectBatch.Begin(GL_LINE_LOOP, 4);
      panselectrectBatch.CopyVertexData3f(vrectBatch);
      panselectrectBatch.End();
	
}
void Render::loadpanopicture()
{
//	loadpanobridgepicture();
//	loadpanocarspicture();
	loadpano360picture();
	testcylinderinit();

}
void Render::loadpanobridgepicture()
{
	char name[200];
	for(int i=0;i<BRIDGENUM;i++)
	{
		sprintf(name,"Bridge/%02d.jpg",i+1);
		printf("%s\n",name);
		bridge[i] = imread(name, CV_LOAD_IMAGE_UNCHANGED);
		printf("the w=%d h=%d =%d\n",bridge[i].cols,bridge[i].rows,bridge[i].channels());
		//imshow("test",bridge[i]);
		//waitKey(10);




	}

}

void Render::loadpanocarspicture()
{
	char name[200];
	for(int i=0;i<CARSNUM;i++)
	{
		sprintf(name,"Cars/%02d.jpg",i+1);
		printf("%s\n",name);
		cars[i] = imread(name, CV_LOAD_IMAGE_UNCHANGED);
		printf("the w=%d h=%d =%d\n",cars[i].cols,cars[i].rows,cars[i].channels());
		//imshow("test",bridge[i]);
		//waitKey(10);




	}

}
void Render::loadpano360picture()
{
	char name[200];
	for(int i=0;i<PANO360NUM;i++)
	{
		sprintf(name,"/home/ubuntu/pano/%d.bmp",i+1);
		printf("%s\n",name);
		pano360[i] = imread(name, CV_LOAD_IMAGE_UNCHANGED);
		resize(pano360[i],pano360[i],Size(PANO360WIDTH,PANO360HEIGHT),0,0,INTER_LINEAR);
		//resize();
		printf("the w=%d h=%d =%d\n",pano360[i].cols,pano360[i].rows,pano360[i].channels());
		//imshow("test",bridge[i]);
		//waitKey(10);




	}
	pano360temp=Mat(PANO360HEIGHT,PANO360WIDTH,CV_8UC3);

}
void Render::testcylinderinit()
{
	testcylind=Mat(1024,768,CV_8UC1);
	testcylinddis=Mat(1024,768,CV_8UC1);
	
	for(int i=0;i<3;i++)
	matbuf[i]=(unsigned char *)malloc(1920*1080*3);
	

}

void Render::cylinderproject(Mat& src,Mat & dst,double R)
{
	int width = src.cols, height = src.rows;
	double x, y;
	//double R = width / 2;
	int drcpoint;
	double fovAngle=2*atan(width/(2.0*R));
	for (int hnum = 0; hnum < height;hnum++)
	{
		for (int wnum = 0; wnum < width;wnum++)
		{
			double k = R / sqrt(R*R + (wnum - width / 2)*(wnum - width / 2));
			//x = (wnum - width / 2) / k + width / 2;
			#if 0
			x= width/2.0 + R * tan((wnum-R * fovAngle/2)/R) ;
			y=hnum;
			#else
			//x==  width/2.0 - R  * tan((R  * fovAngle/2 - wnum)/R ) + 0.5;
			x=width/2+R*tan((wnum-width/2)/R);
			y = (hnum - height / 2) / k + height / 2;
			#endif
			
			if (0 < x && x < width && 0 < y &&y < height)
			{
				dst.at<Vec3b>(hnum, wnum) = src.at<Vec3b>(int(y), int(x));
			}
		}
	}


}

void Render::cylindercut(Mat& src,Mat & dst,int cutnum)
{
	int width = src.cols, height = src.rows;
	double x, y;
	//double R = width / 2;
	int drcpoint;

	for (int hnum = 0; hnum < height;hnum++)
	{
		for (int wnum = 0; wnum < width;wnum++)
		{	
			if(wnum+cutnum<width)
			dst.at<Vec3b>(hnum, wnum) = src.at<Vec3b>(int(hnum), int(wnum+cutnum));
			
		}
	}


}
void Render::CapturePreprocess(Mat& src)
{
	
	/*
	if(getPanoPrepos()!=getPanopos())
		panopostionset();

	
	if(panopostionchanged())
		{
		//OSA_printf("[F=%s L=%d]\n",__func__,__LINE__);
		setNextFrameId();
		}
	*/
	Mat dst=getCurrentFame();
	//imshow("the pano ",src);
	//waitKey(1);
	//Mat temp=Pano360tempframe;

	#if 1
	for(int i=0;i<src.rows;i++)
		{
			memcpy(dst.data+i*dst.cols*dst.channels(),src.data+i*src.cols*src.channels(),dst.cols*src.channels());
		}
	#else

		memcpy(dst.data,src.data,src.rows*dst.cols*src.channels());
	#endif
	//printf("CapturePreprocessthe w=%d h=%d\n",dst.cols,dst.rows);
	//dst=src.clone();
	//resize(src,dst,Size(PANO360WIDTH,PANO360HEIGHT),0,0,INTER_LINEAR);
	//cylinderproject(Pano360tempframe,dst,1.0*CAMERAFOCUSLENGTH*PANO360WIDTH/PANO360SRCWIDTH);
	
	
	

}
void Render::PostCaptureFrame()
{
	panopostionreset();

}
void Render::CaptureProcessFrame(int chid,int widht,int height,int channel,unsigned char *data)
{
	Mat cap;
	//printf("widht=%d *height=%d***************\n",widht,height);
	if(data==NULL||widht==0||height==0||channel==0)
		{
			return ;
		}
	if(channel==3)
		ProcessCapture = Mat(height,widht,CV_8UC3,data);

	CapturePreprocess(ProcessCapture);
	
	//imshow("test",cap);
	//waitKey(1);

}
void Render::CaptureFrame(int chid,int widht,int height,int channel,unsigned char *data)
{
	//Mat cap;
	//printf("widht=%d *height=%d***************\n",widht,height);
	if(data==NULL||widht==0||height==0||channel==0)
		{
			return ;
		}
	if(channel==3)
		Capture = Mat(height,widht,CV_8UC3,data);

	if(shotcut==1)
		{
			shotcut=0;
			CaptureSavepicture(Capture);
		}
	if(chid==0)
		{
			//printf("*************************\n");
			//
			glBindTexture(GL_TEXTURE_2D, textureID[CAPTEXTURE]);
	  		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, widht, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	//imshow("test",cap);
	//waitKey(1);

}

void Render::testcylinder()
{
		//cvtColor(cars[0],testcylind,CV_BGR2GRAY);
		
		//CylindricalWithImgData(cars[0].data,matbuf[0],768,1024);
		//testcylinddis.data=matbuf[0];
		
		/*

		Mat src = cars[0];
		Mat dst(src.rows,src.cols,src.type());

		int width = src.cols, height = src.rows;
		double x, y;
		double R = width / 2;
		int drcpoint;
		double fovAngle=2*atan(width/(2.0*R));
		for (int hnum = 0; hnum < height;hnum++)
		{
			for (int wnum = 0; wnum < width;wnum++)
			{
				double k = R / sqrt(R*R + (wnum - width / 2)*(wnum - width / 2));
				//x = (wnum - width / 2) / k + width / 2;
				x= width/2.0 + R * tan((wnum-R * fovAngle/2)/R) ;
				//x==  width/2.0 - R  * tan((R  * fovAngle/2 - wnum)/R ) + 0.5;
				//x=width/2+R*tan((wnum-width/2)/R);
				//y = (hnum - height / 2) / k + height / 2;
				y=hnum;
				if (0 < x && x < width && 0 < y &&y < height)
				{
					dst.at<Vec3b>(hnum, wnum) = src.at<Vec3b>(int(y), int(x));
				}
			}
		}


		OSA_printf("********");
		imshow("test",dst);
		
		waitKey(0);
		*/
	//	FindHomographyfromPicture(cars[0],cars[0],cars[0]);


}


void Render::CaptureSavebmp(Mat& src)
{

	if(src.cols==0||src.rows==0||src.data==NULL)
		return ;
	static int bmpcount=0;
	char file_name[40];
	
	sprintf(file_name,"/home/ubuntu/pano/%d.bmp",bmpcount);
	//sprintf(file_name,"calibration/%d.bmp",bmpcount);
	//Mat frame_copy(SDI_HEIGHT,SDI_WIDTH,CV_8UC4,pic);
	imwrite(file_name,src);
	bmpcount++;

}

void Render::CaptureSavepicture(Mat& src)
{

	if(src.cols==0||src.rows==0||src.data==NULL)
		return ;
	static double tstart = 0;
	#if 1
	if((getTickCount()-tstart)*1000/getTickFrequency()<100)
		return;
	#else
	
	#endif
	tstart = (double)getTickCount();
	CaptureSavebmp(src);

}
void Render::OptiSeamfun()
{
	Mat src=getPreFame();
	Mat dst=getCurrentFame();

	/*
	if(getSeamEnable())
		{	
			//printf("the seam=%d\n",getSeamPos());
			//dst=fusionframe;
			BestSeam(src,dst,getSeamPos());
			
		}
	//printf("the dst=%p\n",dst.data);
	*/
	SetCurrentDis(dst);



	//imshow("seam",fusionframe);
	//printf("the currentdis=%p\n",currentdis.data);
	


}
void Render::BestSeam(Mat& src,Mat & dst,int seampostion)
{
	double processWidth = src.cols - seampostion;
	processWidth=seampostion;
	int rows = src.rows;
	int cols = src.cols; 
	double alpha = 1;


	for (int i = 0; i < rows; i++)
	{
		uchar* p  = src.ptr<uchar>(i);  
		uchar* t  = dst.ptr<uchar>(i);
		uchar* d = dst.ptr<uchar>(i);
		for (int j = 0; j < processWidth; j++)
		{
			if (p[(j+seampostion)* 3] == 0 && p[(j+seampostion) * 3 + 1] == 0 && p[(j+seampostion) * 3 + 2] == 0)
			{
				alpha = 0;
			}
			else
			alpha =(processWidth- j)*1.0/ processWidth;
			
			d[j * 3] = p[(j+seampostion) * 3] * alpha + t[j * 3] * (1 - alpha);
			d[j * 3 + 1] = p[(j+seampostion) * 3 + 1] * alpha + t[j * 3 + 1] * (1 - alpha);
			d[j * 3 + 2] = p[(j+seampostion)* 3 + 2] * alpha + t[j * 3 + 2] * (1 - alpha);
		}
	}

	

}

void Render::selectupdate()
{	
	 GLfloat vTexselectCoords [] = { 0.0f, 0.0f,
		                      	      0.1f, 0.0f, 
						      0.0f, 1.0f ,
						      0.1,1.0};
	 unsigned int x=0;unsigned int y=0;unsigned int w=0;unsigned int h=0;
	 getPano360RenderPos(&x,&y,&w,&h);
	 int yshift=0;
	 //selecty=h/2+1;
	 if(selecty>h/2)
	 	{
	 	y=y+h/2;
		//printf("**selecty***=%d\n",selecty);
		setselecttexture(1);
		yshift=h/2;
	 	}
	 else
	 	setselecttexture(0);
	 	;
	// printf("**selecty*************************=%d\n",selecty);
	 	//
	 h=h/2;

	/************************************/
	/*
	 selecty=selecty-yshift;

	selectx=0;
	
	selectw=1920*PANO360WIDTH/15000;
	selecth=h;
	*/
	/************************************/
	;
	
	#if 0
	vTexselectCoords[0]=1.0*selectx/w;
	vTexselectCoords[1]=1.0*(selecty-yshift)/h;

	vTexselectCoords[2]=1.0*(selectx+selectw)/w;
	vTexselectCoords[3]=1.0*(selecty-yshift)/h;


	vTexselectCoords[4]=1.0*selectx/w;
	vTexselectCoords[5]=1.0*(selecty+selecth-yshift)/h;

	vTexselectCoords[6]=1.0*(selectx+selectw)/w;
	vTexselectCoords[7]=1.0*(selecty+selecth-yshift)/h;

	
	cout<<"contect"<<vTexselectCoords[0]<<" "<<vTexselectCoords[1]<<" "<<vTexselectCoords[2]<<" "\
		<<vTexselectCoords[3]<<" "<<vTexselectCoords[4]<<" "<<vTexselectCoords[5]<<" "<<vTexselectCoords[6]<<" "\
		<<vTexselectCoords[7]<<endl;
	#else

	Rect rect;
	int index=RENDER180;
	for(int i=RENDER180;i<=RENDER360;i++)
		{
			if(viewcamera[i].active==1)
				index=i;
			
		}
	//setselecttexture(1);

	for(int i=RENDERCAMERA1;i<=RENDERCAMERA3;i++)
		{
			index=viewcamera[i].panotextureindex;
			int tempw=viewcamera[index].leftdownrect.width;
			if(tempw==0)
				tempw=1;
			int tempH=viewcamera[index].leftdownrect.height;
			if(tempH==0)
				tempH=1;
			//leftup2leftdown(viewcamera[RENDERCAMERA1].updownselcectrect,rect);
			rect=viewcamera[i].updownselcectrect;
			//if(index==RENDER360)
			rect.y=rect.y-(renderheight-viewcamera[index].leftdownrect.y-viewcamera[index].leftdownrect.height);
			//rect.y=rect.y-viewcamera[RENDER180].leftdownrect.y;
			
			vTexselectCoords[0]=1.0*rect.x/tempw;
			vTexselectCoords[1]=1.0*(rect.y)/tempH;

			vTexselectCoords[2]=1.0*(rect.x+rect.width)/tempw;
			vTexselectCoords[3]=1.0*(rect.y)/tempH;


			vTexselectCoords[4]=1.0*rect.x/tempw;
			vTexselectCoords[5]=1.0*(rect.y+rect.height)/tempH;

			vTexselectCoords[6]=1.0*(rect.x+rect.width)/tempw;
			vTexselectCoords[7]=1.0*(rect.y+rect.height)/tempH;


			
			/*
			cout<<viewcamera[i].leftdownrect<<rect<<"contect"<<vTexselectCoords[0]<<" "<<vTexselectCoords[1]<<" "<<vTexselectCoords[2]<<" "\
				<<vTexselectCoords[3]<<" "<<vTexselectCoords[4]<<" "<<vTexselectCoords[5]<<" "<<vTexselectCoords[6]<<" "\
				<<vTexselectCoords[7]<<" tempw"<<tempw<<"tempH"<<tempH<<"y"<<viewcamera[i].updownselcectrect.y<<endl;
			*/

			#endif
			

			//panselectrectBatch.Begin(GLenum primitive, GLuint nVerts, GLuint nTextureUnits)

			panselecttriangleBatch[i].CopyTexCoordData2f(vTexselectCoords, 0);
		}


	vrectBatch[0][0]=vTexselectCoords[0]*2-1;
	vrectBatch[0][1]=(1-vTexselectCoords[1]*2);

	vrectBatch[1][0]=vTexselectCoords[2]*2-1;
	vrectBatch[1][1]=1-vTexselectCoords[3]*2;

	vrectBatch[2][0]=vTexselectCoords[6]*2-1;
	vrectBatch[2][1]=1-vTexselectCoords[7]*2;

	vrectBatch[3][0]=vTexselectCoords[4]*2-1;
	vrectBatch[3][1]=1-vTexselectCoords[5]*2;
/*
	vrectBatch[4][0]=vTexselectCoords[0];
	vrectBatch[4][1]=vTexselectCoords[1];

	vrectBatch[5][0]=vTexselectCoords[2];
	vrectBatch[5][1]=vTexselectCoords[3];

	vrectBatch[6][0]=vTexselectCoords[6];
	vrectBatch[6][1]=vTexselectCoords[7];

	vrectBatch[7][0]=vTexselectCoords[4];
	vrectBatch[7][1]=vTexselectCoords[5];
*/
	
	

	panselectrectBatch.CopyVertexData3f(vrectBatch);
	 
	

}



void Render::leftdown2leftup(Rect& down,Rect& up)
{
	up.x=down.x;
	up.y=renderheight-(down.y+down.height);
	up.width=down.width;
	up.height=down.height;

}

void Render::leftup2leftdown(Rect& up,Rect& down)
{
	down.x=up.x;
	down.y=renderheight-(up.y+up.height);
	down.width=up.width;
	down.height=up.height;

}
void Render::Mousemenu()
{
	int mousex=0;
	int mousey=0;
	if(MOUSEST==MOUSEUP&&BUTTON==MOUSERIGHT)
		osdmenushow=osdmenushow^1;

	if(osdmenushow==0)
		{
			Menu->menureset();
		}
	else if(osdmenushowpre==0)
		{
			Menu->menushow();
		}
	osdmenushowpre=osdmenushow;
	

	if(MOUSEST==MOUSEPRESS&&BUTTON==MOUSELEFT)
	{
		mousex=MOUSEx;
		mousey=MOUSEy;

		for(int i=WORKMOD;i<MENUMAX;i++)
			{
				if(Menu->submenu[i].displayvalid==HIDEMODE)
					continue;
				//printf("mousex=%d mousey=%d x=%d y=%d w=%d h=%d\n",mousex,mousey,Menu->submenu[i].x,Menu->submenu[i].y,Menu->submenu[i].w,Menu->submenu[i].h);
				if((mousex>=Menu->submenu[i].x)&&(mousex<=Menu->submenu[i].x+Menu->submenu[i].w)&&\
					(mousey>=Menu->submenu[i].y)&&(mousey<=Menu->submenu[i].y+Menu->submenu[i].h))
					{
						Menu->menuselect(i,Menu->submenu[i].active^1);

					}


			}
		
	}
	
		


}
void Render::viewcameraprocess()
{
	Rect leftuprect;
	Rect leftdownrect;
	leftuprect.x=mousex;
	leftuprect.y=mousey;
	int cameraselcect=0;
	if(MOUSEST==MOUSEUP&&BUTTON==MOUSELEFT)
		{
			leftuprect.width=abs(MOUSEx-mousex);
			leftuprect.height=abs(MOUSEy-mousey);

			
			leftup2leftdown(leftuprect,leftdownrect);




			for(int i=RENDER180;i<=RENDER360;i++)
				{
					
					if(leftdownrect.x>viewcamera[i].leftdownrect.x&&leftdownrect.x<viewcamera[i].leftdownrect.x+viewcamera[i].leftdownrect.width&&\
						leftdownrect.y>viewcamera[i].leftdownrect.y&&leftdownrect.y<viewcamera[i].leftdownrect.y+viewcamera[i].leftdownrect.height)
						{
							cameraselcect=1;
							break;
						}
					else
						cameraselcect=0;

				}

			if(cameraselcect)
			for(int i=RENDER180;i<=RENDER360;i++)
				{
					
					if(leftdownrect.x>viewcamera[i].leftdownrect.x&&leftdownrect.x<viewcamera[i].leftdownrect.x+viewcamera[i].leftdownrect.width&&\
						leftdownrect.y>viewcamera[i].leftdownrect.y&&leftdownrect.y<viewcamera[i].leftdownrect.y+viewcamera[i].leftdownrect.height)
						{
							viewcamera[i].active=1;
							for(int j=RENDERCAMERA1;j<=RENDERCAMERA3;j++)
							{
								if(viewcamera[j].active)
									{
										if(leftuprect.x+leftuprect.width>=viewcamera[i].leftdownrect.width)
											leftuprect.width=viewcamera[i].leftdownrect.width-leftuprect.x;
										if(leftuprect.y+leftuprect.height>=renderheight-viewcamera[i].leftdownrect.y)
											leftuprect.height=renderheight-viewcamera[i].leftdownrect.y-leftuprect.y;
										
										viewcamera[j].panotextureindex=i;
										viewcamera[j].updownselcectrect=leftuprect;
										
									}
							}
							

						}
					else
						{
							viewcamera[i].active=0;

						}
				}




			//if(cameraselcect)
			for(int i=RENDERCAMERA1;i<=RENDERCAMERA3;i++)
				{
					//cout<<leftdownrect<<viewcamera[i].leftdownrect<<endl;
					if(leftdownrect.x>viewcamera[i].leftdownrect.x&&leftdownrect.x<viewcamera[i].leftdownrect.x+viewcamera[i].leftdownrect.width&&\
						leftdownrect.y>viewcamera[i].leftdownrect.y&&leftdownrect.y<viewcamera[i].leftdownrect.y+viewcamera[i].leftdownrect.height)
						{
							//printf("the RENDERCAMERA=%d\n",i);
							cameraselcect=1;
							break;
						}
					else
						cameraselcect=0;
				}
			if(cameraselcect)
				{
					for(int i=RENDERCAMERA1;i<=RENDERCAMERA3;i++)
						{
							if(leftdownrect.x>viewcamera[i].leftdownrect.x&&leftdownrect.x<viewcamera[i].leftdownrect.x+viewcamera[i].leftdownrect.width&&\
								leftdownrect.y>viewcamera[i].leftdownrect.y&&leftdownrect.y<viewcamera[i].leftdownrect.y+viewcamera[i].leftdownrect.height)
								{
								//printf("the viewcamera=%d\n",i);
								viewcamera[i].active=1;
								}
							else
								viewcamera[i].active=0;
						}
				}
		}


}
void Render::Mouse2Select()
{

	 unsigned int x=0;unsigned int y=0;unsigned int w=0;unsigned int h=0;
	 getPano360RenderPos(&x,&y,&w,&h);
	// int lux=x+
	if(MOUSEST==MOUSEPRESS&&BUTTON==MOUSELEFT)
		{
			mousex=MOUSEx;
			mousey=MOUSEy;
		}
	if(MOUSEST==MOUSEUP&&BUTTON==MOUSELEFT)
		{
			selectw=abs(MOUSEx-mousex);
			selecth=abs(MOUSEy-mousey);
			selectx=min(MOUSEx,mousex);
			selecty=min(MOUSEy,mousey);
		}
	

}
void Render::MouseSelectpos()
{
	/**/
	int x=0;
	int y=0;
	double mouseangle=0;
	double mousetitleangle=0;
	int panposx=0;
	int cent180y=0;
	int cent360y=0;
	int cent180h=0;
	int cent360h=0;
	cent180y=(renderheight-mov180viewy)/2;
	cent180h=renderheight-mov180viewy;
	cent360y=(renderheight-mov180viewy)*3/2;
	cent360h=renderheight-mov180viewy;
	static int mousexpre=0;
	static int mouseypre=0;
	static int mousewpre=0;
	static int mousehpre=0;
	if(MOUSEST==MOUSEPRESS&&BUTTON==MOUSELEFT)
		{
			mousexpre=MOUSEx;
			mouseypre=MOUSEy;
		}
	if(MOUSEST==MOUSEUP&&BUTTON==MOUSELEFT)
		{
			mousewpre=abs(MOUSEx-mousexpre);
			mousehpre=abs(MOUSEy-mouseypre);
			selectx=min(MOUSEx,mousexpre);
			selecty=min(MOUSEy,mouseypre);
		}
	
	if(MOUSEST==MOUSEUP&&BUTTON==MOUSELEFT)
		{
			x=selectx+mousewpre/2;
			y=selecty+mousehpre/2;

			if(renderheight-mov360viewy<MOUSEy)
				return ;
			if(renderheight-mov180viewy>MOUSEy)
				{
					panposx=x*pano360texturew/(2*renderwidth)-Config::getinstance()->getpanoprocesswidth()/2;
					mouseangle=offet2anglepano(panposx);

					mousetitleangle=1.0*(cent180y-y)*TVFOV/(cent180h)+getptzzerotitleangle();
					
					
					//pano360texturew;
					//mouseangle
				}
			else
				{
					panposx=(x+renderwidth)*pano360texturew/(renderwidth*2)-Config::getinstance()->getpanoprocesswidth()/2;
					mouseangle=offet2anglepano(panposx);

					mousetitleangle=1.0*(cent360y-y)*TVFOV/(cent360h)+getptzzerotitleangle();

				}
			mouseangle+=getptzzeroangle();
			if(mouseangle>360)
				mouseangle=mouseangle-360;
			else if(mouseangle<0)
				mouseangle=mouseangle+360;

				if(mousetitleangle>360)
				mousetitleangle=mousetitleangle-360;
			else if(mousetitleangle<0)
				mousetitleangle=mousetitleangle+360;
			//printf("the angle =%f \n",mouseangle);
			setscanpanflag(0);
			setpanopanpos(mouseangle);
			setpanotitlepos(mousetitleangle);
				;
			

		}
	else
		return ;


}

void Render::Mousezeropos()
{

	int x=0;
	int y=0;
	double mouseangle=0;
	double mousetitleangle=0;
	int panposx=0;
	int cent180y=0;
	int cent360y=0;
	int cent180h=0;
	int cent360h=0;
	cent180y=(renderheight-mov180viewy)/2;
	cent180h=renderheight-mov180viewy;
	cent360y=(renderheight-mov180viewy)*3/2;
	cent360h=renderheight-mov180viewy;
	static int mousexpre=0;
	static int mouseypre=0;
	static int mousewpre=0;
	static int mousehpre=0;
	if(MOUSEST==MOUSEPRESS&&BUTTON==MOUSELEFT)
		{
			mousexpre=MOUSEx;
			mouseypre=MOUSEy;
		}
	if(MOUSEST==MOUSEUP&&BUTTON==MOUSELEFT)
		{
			mousewpre=abs(MOUSEx-mousexpre);
			mousehpre=abs(MOUSEy-mouseypre);
			selectx=min(MOUSEx,mousexpre);
			selecty=min(MOUSEy,mouseypre);
		}
	
	if(MOUSEST==MOUSEUP&&BUTTON==MOUSELEFT)
		{
			x=mousexpre;
			y=mouseypre;

			if(renderheight-mov360viewy<MOUSEy)
				return ;
			if(renderheight-mov180viewy>MOUSEy)
				{
					panposx=x*pano360texturew/(2*renderwidth);
					mouseangle=offet2anglepano(panposx);

					mousetitleangle=1.0*(cent180y-y)*TVFOV/(cent180h)+getptzzerotitleangle();
					
					
					//pano360texturew;
					//mouseangle
				}
			else
				{
					panposx=(x+renderwidth)*pano360texturew/(renderwidth*2);
					mouseangle=offet2anglepano(panposx);

					mousetitleangle=1.0*(cent360y-y)*TVFOV/(cent360h)+getptzzerotitleangle();

				}
			mouseangle+=getptzzeroangle();
			if(mouseangle>360)
				mouseangle=mouseangle-360;
			else if(mouseangle<0)
				mouseangle=mouseangle+360;

				if(mousetitleangle>360)
				mousetitleangle=mousetitleangle-360;
			else if(mousetitleangle<0)
				mousetitleangle=mousetitleangle+360;
			//printf("the angle =%f \n",mouseangle);
			setscanpanflag(0);
			setpanopanpos(mouseangle);
			
				;
			

		}
	else
		return ;


}

void Render::gltMakeradar(GLTriangleBatch& diskBatch, GLfloat innerRadius, GLfloat outerRadius, GLint nSlices, GLint nStacks,double anglestart,double angleend)
	{
	// How much to step out each stack
	GLfloat fStepSizeRadial = outerRadius - innerRadius;
	GLfloat ystep=1;
	GLfloat xstep=1;
	if(fStepSizeRadial < 0.0f)			// Dum dum...
		fStepSizeRadial *= -1.0f;

	fStepSizeRadial /= float(nStacks);
	ystep/=float(nStacks);
	xstep/=float(nSlices);
	GLfloat fStepSizeSlice = (angleend-anglestart) / float(nSlices);
	
	diskBatch.BeginMesh(nSlices * nStacks * 6);
	
	M3DVector3f vVertex[4];
	M3DVector3f vNormal[4];
	M3DVector2f vTexture[4];
	
	float fRadialScale = 1.0f / outerRadius;
	
	for(GLint i = 0; i < nStacks; i++)			// Stacks
		{
		float theyta;
		float theytaNext;
		for(GLint j = 0; j < nSlices; j++)     // Slices
			{
			float inner = outerRadius - (float(i)) * fStepSizeRadial;
			float outer = outerRadius - (float(i+1)) * fStepSizeRadial;
			
			float iny=(float(i))*ystep;
			float ouy= (float(i+1)) *ystep;

			float inx=(float(j))*xstep;
			float oux= (float(j+1)) *xstep;
			
			theyta = fStepSizeSlice * float(j)+anglestart;
			//if(j == (nSlices - 1))
			//	theytaNext = 0.0f+anglestart;
			//else
				theytaNext = fStepSizeSlice * (float(j+1))+anglestart;
				
			// Inner First
			vVertex[0][0] = cos(theyta) * inner;	// X	
			vVertex[0][1] = sin(theyta) * inner;	// Y
			vVertex[0][2] = 0.0f;					// Z
			
			vNormal[0][0] = 0.0f;					// Surface Normal, same for everybody
			vNormal[0][1] = 0.0f;
			vNormal[0][2] = 1.0f;
			
			vTexture[0][0] =inx;	
			vTexture[0][1] = iny;
			
			// Outer First
			vVertex[1][0] = cos(theyta) * outer;	// X	
			vVertex[1][1] = sin(theyta) * outer;	// Y
			vVertex[1][2] = 0.0f;					// Z
			
			vNormal[1][0] = 0.0f;					// Surface Normal, same for everybody
			vNormal[1][1] = 0.0f;
			vNormal[1][2] = 1.0f;
			
			vTexture[1][0] = inx;
			vTexture[1][1] =ouy;
			
			// Inner Second
			vVertex[2][0] = cos(theytaNext) * inner;	// X	
			vVertex[2][1] = sin(theytaNext) * inner;	// Y
			vVertex[2][2] = 0.0f;					// Z
			
			vNormal[2][0] = 0.0f;					// Surface Normal, same for everybody
			vNormal[2][1] = 0.0f;
			vNormal[2][2] = 1.0f;
			
			vTexture[2][0] =oux;
			vTexture[2][1] = iny;
			
			
			// Outer Second
			vVertex[3][0] = cos(theytaNext) * outer;	// X	
			vVertex[3][1] = sin(theytaNext) * outer;	// Y
			vVertex[3][2] = 0.0f;					// Z
			
			vNormal[3][0] = 0.0f;					// Surface Normal, same for everybody
			vNormal[3][1] = 0.0f;
			vNormal[3][2] = 1.0f;
			
			vTexture[3][0] = oux;
			vTexture[3][1] = ouy;
			
			diskBatch.AddTriangle(vVertex, vNormal, vTexture);			
			
			// Rearrange for next triangle
			memcpy(vVertex[0], vVertex[1], sizeof(M3DVector3f));
			memcpy(vNormal[0], vNormal[1], sizeof(M3DVector3f));
			memcpy(vTexture[0], vTexture[1], sizeof(M3DVector2f));
			
			memcpy(vVertex[1], vVertex[3], sizeof(M3DVector3f));
			memcpy(vNormal[1], vNormal[3], sizeof(M3DVector3f));
			memcpy(vTexture[1], vTexture[3], sizeof(M3DVector2f));
					
			diskBatch.AddTriangle(vVertex, vNormal, vTexture);			
			}
		}
	
	diskBatch.End();
	}


void Render::gltMakeradarpoints(vector<OSDPoint>& osdpoints, GLfloat innerRadius, GLfloat outerRadius, GLint nSlices,double anglestart,double angleend)
	{
	// How much to step out each stack
	GLfloat fStepSizeRadial = outerRadius - innerRadius;
	GLfloat ystep=1;
	GLfloat xstep=1;
	if(fStepSizeRadial < 0.0f)			// Dum dum...
		fStepSizeRadial *= -1.0f;

	GLint nStacks=1;
	fStepSizeRadial /= float(nStacks);
	ystep/=float(nStacks);
	xstep/=float(nSlices);
	GLfloat fStepSizeSlice = (angleend-anglestart) / float(nSlices);

	osdpoints.clear();
	OSDPoint pointtemp;

	
	float fRadialScale = 1.0f / outerRadius;
	
	for(GLint i = 0; i < nStacks; i++)			// Stacks
		{
		float theyta;
		float theytaNext;
		for(GLint j = 0; j < nSlices; j++)     // Slices
			{
			float inner = innerRadius + (float(i)) * fStepSizeRadial;
			float outer = innerRadius + (float(i+1)) * fStepSizeRadial;
			
		
			theyta = fStepSizeSlice * float(j)+anglestart;
			
				
			// Inner First
			pointtemp.x = cos(theyta) * inner;	// X	
			pointtemp.y = sin(theyta) * inner;	// Y
							// Z
			osdpoints.push_back(pointtemp);

			/*
			// Outer First
			pointtemp.x = cos(theyta) * outer;	// X	
			pointtemp.y = sin(theyta) * outer;	// Y
			osdpoints.push_back(pointtemp);				// Z
	
			*/

			
		
			}



		for(GLint j = 0; j < nSlices; j++)     // Slices
			{
			
				float inner = innerRadius + (float(i)) * fStepSizeRadial;
				float outer = innerRadius + (float(i+1)) * fStepSizeRadial;
				
			
				theyta = fStepSizeSlice * float(nSlices-1-j)+anglestart;
				
					
					pointtemp.x = cos(theyta) * outer;	// X	
					pointtemp.y = sin(theyta) * outer;	// Y
					osdpoints.push_back(pointtemp);		
			

			
		
			}
		}
	

	}




