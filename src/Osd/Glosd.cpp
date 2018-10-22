#include "Glosd.hpp"
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>

#include <glut.h>
#include <freeglut_ext.h>



wchar_t intext[] = {


	L"\u6539\n"


};	
wchar_t intext1[] = {

	L"\u7b2c\n"


};	
wchar_t intext2[] = {


	L"\u8fdb\n"

};	


GLOSD::GLOSD()
{
	windowW=720;
	windowH=576;
}
GLOSD::~ GLOSD()
{

}

void GLOSD::create()
{
	drawshaderManager.InitializeStockShaders();
	
}
void GLOSD::drawbegin()
{
    GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 0.5f };
    GLfloat vGreen[] = { 0.0f, 1.0f, 0.0f, 1.0f };
    GLfloat vBlue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat vBlack[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    drawshaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
    glLineWidth(2.0f);
}

void GLOSD::setwindow(int w,int h)
{	
	windowW=w;
	windowH=h;
	ChangeSize(w,h);

}
void GLOSD::drawend()
{
	glLineWidth(1.0f);
}
void GLOSD::windowtoglcenter(float point[],int num)
{
	Vpoints[num][0]=point[0]*2/windowW-1;
	Vpoints[num][1]=-point[1]*2/windowH+1;
	Vpoints[num][2]=0;

}

void GLOSD::drawrect(int x,int y,int w,int h)
{
	GLfloat point[3];
	point[0]=x;
	point[1]=y;
	windowtoglcenter(point,0);

	point[0]=x+w;
	point[1]=y;
	windowtoglcenter(point,1);

	point[0]=x+w;
	point[1]=y+h;
	windowtoglcenter(point,2);

	point[0]=x;
	point[1]=y+h;
	windowtoglcenter(point,3);
	
	 GLBatch			         rectBatch;
	 rectBatch.Begin(GL_LINE_LOOP, 4);
        rectBatch.CopyVertexData3f(Vpoints);
        rectBatch.End();
        rectBatch.Draw();

	
}

void GLOSD::drawline(int xs,int ys,int xe,int ye)
{

	GLfloat point[3];
	point[0]=xs;
	point[1]=ys;
	windowtoglcenter(point,0);

	point[0]=xe;
	point[1]=ye;
	windowtoglcenter(point,1);
	 GLBatch			     lineBatch;
	 lineBatch.Begin(GL_LINES, 2);
        lineBatch.CopyVertexData3f(Vpoints);
        lineBatch.End();
        lineBatch.Draw();

}


void GLOSD::drawstrings(int x,int y,char *string)
{
	GLfloat point[3];
	point[0]=x;
	point[1]=y;
	windowtoglcenter(point,0);

	glRasterPos2f(Vpoints[0][0], Vpoints[0][1]);
	drawstring(string);

}
void GLOSD::drawstring(char *string)
{

    if(string==NULL)
		return ;
 //   sprintf(fps_str, "Capture %6.1f ms\n", fps);
 
    //glColor3f(0.0, 0.0, 1.0);
 //   glRasterPos2f(-0.4, -0.1);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)string);

}

void GLOSD::drawcross(int x,int y,int w,int h)
{
	drawline(x-w,y,x+w,y);
	drawline(x,y-h,x,y+h);

}
void GLOSD::ChangeSize(int w,int h )
{
	  GLfloat fAspect;

    // Prevent a divide by zero
    if(h == 0)
        h = 1;

    // Set Viewport to window dimensions
 

    fAspect = (GLfloat)w/(GLfloat)h;

    // Produce the perspective projection
    viewFrustum.SetPerspective(35.0f,fAspect,1.0,120.0);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);


}
//GLfloat             viewZ = -50.0f;

bool GLOSD::createunicode(const char* faceName,int fontSize,int w ,int h)
{
	Unicodecolor[0]=1.0f;
	Unicodecolor[1]=0.0f;
	Unicodecolor[2]=1.0f;
	Unicodecolor[3]=0.0f;
	freetype = new FreeTypeFont;
	//freetype->create("/usr/share/fonts/truetype/abyssinica/AbyssinicaSIL-R.ttf", 40, 512, 512);
	//freetype->create("/home/fsmdn113/default.ttf", 40, 512, 512);
	freetype->create(faceName, fontSize, w, h);
	return 1;

}

bool GLOSD::desteryunicode()
{
	if(freetype!=NULL)
		delete freetype;
	return 1;
}

bool GLOSD::setcolorunicode(int color)
{
	Unicodecolor[0]=1.0f;
	Unicodecolor[1]=0.0f;
	Unicodecolor[2]=1.0f;
	Unicodecolor[3]=0.0f;
	//GLfloat vFloor[] = { 1.0f, 0.0f, 1.0f, 0.6f };
	return 1;

}

bool GLOSD::drawunicodebegin()
{
	if(freetype==NULL)
		return 0;

	freetype->begin(0, 0);



	return 1;
}
bool GLOSD::drawunicodeend()
{
	if(freetype==NULL)
		return 0;

	freetype->end();
	return 1;
}
void GLOSD::drawunicode(int x,int y,wchar_t* text)
{

	//modelViewMatrix.PushMatrix();
       //modelViewMatrix.Translate(0.0f, 0.0f, viewZ);
       //int a=300;
	M3DMatrix44f mtest;
	m3dLoadIdentity44(mtest);
	drawshaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACEWJ, mtest, 0,Unicodecolor);
	//freetype->drawText(-a, a, 0, Rgba(0,255,0,255), intext3, 0, 0, 0);
	freetype->drawText(x, y, 0, Rgba(0,255,0,255), text, 0, 0, 0);
	

	 modelViewMatrix.PopMatrix();

}
