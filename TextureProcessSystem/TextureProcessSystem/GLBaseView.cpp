// GLBaseView.CPropertyPage : 实现文件
//
#pragma  once
#include "stdafx.h"
#include "TextureProcessSystem.h"
#include "GLBaseView.h"
#include "plyloader.h"
#include "FindTextureElementPosition.h"
#include "CBMPLoader.h"
#include <fstream>
#include "V_calculate.h"
#include <math.h>
#include "config.h"
#include "textfile.h"
#include "SC.h"
#include <iostream>
#include <sstream>
using namespace std;
// CGLBaseView

IMPLEMENT_DYNCREATE(CGLBaseView, CView)

CGLBaseView::CGLBaseView()
{
	rotate_X = rotate_Y = rotate_Z = 0;
	rotate_X = 0;
	rotate_Z = 0;
	trans_X = trans_Y = trans_Z = 0;
	scale_X = scale_Y = scale_Z = 1;
	trans_X = -0.200000033; trans_Y = -1.08999932; trans_Z = 0;
	rotate_X = -0.277777731; rotate_Y = 180.555542; rotate_Z = 0.000000000;
	scale_X = 0.9;
	isInit = false;
	modelShowIn3Dor2D = true;
	ofstream f("warnlog.txt");
	f.close();
	ofstream f1("texlog.txt");
	f1.close();
	isOpenDepthTest = false;
	isBindVertex = false;
	diffuse[0] = 0;
	diffuse[1] = -3.15;
	diffuse[2] = -5.13;
	diffuse[3] = 1;
	runtimes = 0;
}

CGLBaseView::~CGLBaseView()
{
}

BEGIN_MESSAGE_MAP(CGLBaseView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGLBaseView 绘图

void CGLBaseView::OnDraw(CDC* pDC)
{
	CTextureProcessSystemDoc* pDoc = GetDocument();

	m_pDoc = pDoc;
	// TODO:  在此添加绘制代码
	if (runtimes > 0)
	{
		bool res=m_pDoc->get_ftep()->buildByStep();
		if (res)
		{
			bool cutE = false;
			do{
				cutE= m_pDoc->get_ftep()->cutEges2(m_pDoc->get_ftep()->targetCenter); 
			} while (cutE);
			m_pDoc->get_ftep()->detectCollapse();
		
			runtimes--;
			//截图
			UpdateWindow();
			CDC * pCDC = this->GetDC();
			string ss;
			stringstream sout;
			sout << 1000 - runtimes;
			ss = sout.str();
			string a = "../cap/";
			string b = ".bmp";
			string sb = a + ss + b;
			SC::Screen(sb.c_str(), pCDC);
			ReleaseDC(pCDC);
		}
		else
		{
			runtimes = 0;
		}
		if (runtimes == 0)
		{
			DWORD m, s, ms;
			TimeEnd = GetTickCount();
			TimeUsed = TimeEnd - TimeStart;
			m = TimeUsed / 60000;
			s = TimeUsed / 1000 - m * 60;
			ms = TimeUsed % 1000;
			ofstream f("running record.txt");
			f << "element nums:" << m_pDoc->get_ftep()->m_targetTexture->tes.size() << endl;
			f << "cost time:" << m << " minutes " << s << " seconds" << ms << " milliseconds" << endl;
			f.close();
			KillTimer(1);
		}
	}

	wglMakeCurrent(m_pDC->m_hDC, m_hRC);//使 RC 与当前 DC 相关联
	//////////////////////////////////////

	COLORREF color = pDoc->m_BackColor;
	float r = (float)GetRValue(color) / 255;
	float g = (float)GetGValue(color) / 255;
	float b = (float)GetBValue(color) / 255;
	if (pDoc->isdrawbg)
	{
		r = 192.0 / 255.0;
		g = 192.0 / 255.0;
		b = 192.0 / 255.0;
	}
	glClearColor(r, g, b, 1.0f);//设置背景颜色
 	glClearDepth(10.0f);
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	glDepthFunc(GL_LESS);// 使用深度
 	glViewport(0, 0, w, h);  //设置视口
 	glLoadIdentity();
// 	// 	//gluLookAt(0, 0, -10, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
// 	// 	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
// 	// 	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
// 	// // 	GLfloat LightPosition[] = { 0.0, 0.0, 1.0, 0.0 }; //光源位置
// 	// // 	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); //设置光源位置
// 	// 	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
// 	// 	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glMatrixMode(GL_PROJECTION);//进入投影变换状态
	glLoadIdentity();
	glTranslatef(trans_X, trans_Y, trans_Z);
	glScalef(scale_X*1.0f, scale_Y*1.0f, scale_Z*1.0f);
	glRotatef(rotate_X, 1.0f, 0.0f, 0.0f);
	glRotatef(rotate_Y, 0.0f, 1.0f, 0.0f);
	glRotatef(rotate_Z, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_BUFFER_BIT);
 	glGetIntegerv(GL_VIEWPORT, viewport); // 得到的是最后一个设置视口的参数
 	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
 	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	//glLineWidth(1.5f);
	////////////////////////////////////////

	//glEnable(GL_CULL_FACE);
	if (!isInit)
	{
		LoadGLTextures();
		isInit = true;

	}
	glCullFace(GL_BACK);	


	if (pDoc->isdrawbg)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		drawPLYwithGLSL();
		glDisable(GL_BLEND);
	}
	else
	{
		if (isBindVertex)
		{
			glDeleteShader(VertexShaderID);
			glDeleteShader(FragmentShaderID);
			isBindVertex = false;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawScene();  //RC 绘图
	}

	glDisable(GL_CULL_FACE);

	glLoadIdentity();

	SwapBuffers(m_pDC->m_hDC);//把 RC 中所绘传到当前的 DC 上，从而
	//在屏幕上显示
	wglMakeCurrent(m_pDC->m_hDC, NULL);//释放 RC，以便其它 DC 进行绘图



}


// CGLBaseView 诊断
void CGLBaseView::drawPLYwithGLSL()
{
	if (isBindVertex)
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int textureID[55];
		for (int i = 0; i < 55; i++)
		{
			textureID[i] = i;
		} 
		GLfloat ambient[4] = { 0, 0, 0.1, 1.0 };
	
		// Use the program p
		glUseProgram(shaderID);
		// Bind the vertex array object
		glBindVertexArray(vao);

		glUniform1iv(glGetUniformLocation(shaderID, "texs"), 55,textureID);
		//glUniform4f(glGetUniformLocation(shaderID, "ambient"), ambient[0], ambient[1], ambient[2], ambient[3]);
		glUniform4f(glGetUniformLocation(shaderID, "diffuse"), diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	
		// 	// draw the 6 vertices
		glDrawArrays(GL_TRIANGLES, 0, vertSize);
		glUseProgram(NULL);
	}
	else
	{
		if (m_pDoc->plyLoader.faceArry.size() > 0)
		{
			setupShaders();
			bindVertexBuffer();
			isBindVertex = true;
			for (int i = 0; i < 50; i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // Texture unit 0
				glBindTexture(GL_TEXTURE_2D, m_texture[i + 1].ID);
			}

		}
	}

}
#ifdef _DEBUG
void CGLBaseView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CGLBaseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


CTextureProcessSystemDoc * CGLBaseView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextureProcessSystemDoc)));
	return (CTextureProcessSystemDoc*)m_pDocument;
}
// CGLBaseView 消息处理程序

bool CGLBaseView::isExtensionSupported(const char* string)
{
	char *extension;			/**< 指向扩展字符串的指针 */
	char *end;				    /**< 最后一个字符指针 */
	int idx;
	int nTextureUnits = 0;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &nTextureUnits);

	extension = (char*)glGetString(GL_EXTENSIONS);
	if (extension == NULL)
		return false;

	/** 得到最后一个字符 */
	end = extension + strlen(extension);

	/** 循环查找字符串string */
	while (extension < end)
	{
		/** 找到空格前的一个字符串 */
		idx = strcspn(extension, " ");

		/** 检查是否找到字符串string */
		if ((strlen(string) == idx) && (strncmp(string, extension, idx) == 0))
		{
			return true;
		}

		/** 当前串不是string,继续下一个字符串 */
		extension += (idx + 1);
	}
	/** 没有找到,则返回false */
	return false;
}

bool CGLBaseView::initMultiTexture()
{
	/** 检查是否支持扩展 */
	if (isExtensionSupported("GL_ARB_multitexture"))
	{

		/** 获得函数指针地址 */
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
		return true;
	}
	else
		return false;
}
int CGLBaseView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pDC = new CClientDC(this);  //创建 DC
	ASSERT(m_pDC != NULL);

	if (!SetupPixelFormat())    //设定绘图的位图格式，函数下面列出
		return -1;

	m_hRC = wglCreateContext(m_pDC->m_hDC);//创建 RC

	InitialOpenGL(); //初始化OpenGL
	if (!initMultiTexture())
	{
		//不支持多重纹理
		return -1;
	}
	LoadGLTextures();

	return 0;
}
void CGLBaseView::drawPLYwithMultiTexture()
{
	vector<gl_face> * Triangle = &(m_pDoc->plyLoader.faceArry);
	vector<gl_point> * Vertex = &(m_pDoc->plyLoader.pointArry);
	vector<gl_point2d> * Vertex2d = &(m_pDoc->plyLoader.point2DArry);
	for (int i = 0; i < m_pDoc->plyLoader.faceArry.size(); i++)
	{
		float v1x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].x;
		float v1y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].y;
		float v1z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].z;
		float v2x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].x;
		float v2y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].y;
		float v2z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].z;
		float v3x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].x;
		float v3y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].y;
		float v3z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].z;
		float v1u, v1v, v2u, v2v, v3u, v3v;
		int texCoordsSize = Triangle->at(i).texCoords.size();
		if (texCoordsSize > 0)
		{
			//重叠区域大于maxMultiTextureLayer，警告信息
			if (texCoordsSize > maxMultiTextureLayer)
			{
				if (m_pDoc->logTex)
				{
					ofstream f("warnlog.txt", ios::app);
					f << endl;
					f << "Triangle" << i << " texCoords.size: " << Triangle->at(i).texCoords.size() << endl;
					for (int k = 0; k < Triangle->at(i).texCoords.size(); k++)
					{
						f << "textureIndex: " << Triangle->at(i).texCoords[k]->textureIndex << endl;
						f << Triangle->at(i).texCoords[k]->cor[0][0] << "," << Triangle->at(i).texCoords[k]->cor[0][1] << endl;
						f << Triangle->at(i).texCoords[k]->cor[1][0] << "," << Triangle->at(i).texCoords[k]->cor[1][1] << endl;
						f << Triangle->at(i).texCoords[k]->cor[2][0] << "," << Triangle->at(i).texCoords[k]->cor[2][1] << endl;
					}
					f << endl;
					f.close();
				}
				continue;
			}			
			int textureIndex = 2;
			for (int j = 1; j <= texCoordsSize; j++)
			{
				glActiveTextureARB(GL_TEXTURE0_ARB + j);
				glEnable(GL_TEXTURE_2D);
				textureIndex = Triangle->at(i).texCoords[j-1]->textureIndex;
				glBindTexture(GL_TEXTURE_2D, m_texture[textureIndex].ID);
			}
			glColor3f(0.8, 0.8, 0.8);
			glBegin(GL_TRIANGLES);//显示
			glNormal3fv(Triangle->at(i).n);
			for (int j = 1; j <= texCoordsSize; j++)
			{
				glMultiTexCoord2fARB(GL_TEXTURE0_ARB + j, Triangle->at(i).texCoords[j - 1]->cor[0][0], Triangle->at(i).texCoords[j - 1]->cor[0][1]);
			}			
			glVertex3f(v1x, v1y, v1z);
			for (int j = 1; j <= texCoordsSize; j++)
			{
				glMultiTexCoord2fARB(GL_TEXTURE0_ARB + j, Triangle->at(i).texCoords[j - 1]->cor[1][0], Triangle->at(i).texCoords[j - 1]->cor[1][1]);
			}
			glVertex3f(v2x, v2y, v2z);
			for (int j = 1; j <= texCoordsSize; j++)
			{
				glMultiTexCoord2fARB(GL_TEXTURE0_ARB + j, Triangle->at(i).texCoords[j - 1]->cor[2][0], Triangle->at(i).texCoords[j - 1]->cor[2][1]);
			}
			glVertex3f(v3x, v3y, v3z);
			glEnd();
		}
		
		if (Triangle->at(i).texCoords.size() > 1)
		{
			if (m_pDoc->logTex)
			{
				ofstream f("texlog.txt", ios::app);
				f << "Triangle  " << i << endl;
				for (int k = 0; k < Triangle->at(i).texCoords.size(); k++)
				{
					f << "textureIndex: " << Triangle->at(i).texCoords[k]->textureIndex << endl;
					f << Triangle->at(i).texCoords[k]->cor[0][0] << "," << Triangle->at(i).texCoords[k]->cor[0][1] << endl;
					f << Triangle->at(i).texCoords[k]->cor[1][0] << "," << Triangle->at(i).texCoords[k]->cor[1][1] << endl;
					f << Triangle->at(i).texCoords[k]->cor[2][0] << "," << Triangle->at(i).texCoords[k]->cor[2][1] << endl;
				}
				f << endl;
				f.close();
			}
		}
		for (int j = 1; j <= texCoordsSize; j++)
		{
			glActiveTextureARB(GL_TEXTURE0_ARB + j);
			glDisable(GL_TEXTURE_2D);
		}		
	}
	glFinish();
	m_pDoc->logTex = false;
}

void CGLBaseView::drawPLY()
{

	vector<gl_face> * Triangle = &(m_pDoc->plyLoader.faceArry);
	vector<gl_point> * Vertex = &(m_pDoc->plyLoader.pointArry);
	vector<gl_point2d> * Vertex2d = &(m_pDoc->plyLoader.point2DArry);
	if (m_pDoc->plyLoader.pointArry[Triangle->at(0).ptnum[0]].x<-4e+8)
	{
		AfxMessageBox("model data is null");
		exit(-1);
	}
	int i;
	int a = 3;
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, m_texture[0].ID);
	glBegin(GL_TRIANGLES);//显示模型的面
	for (int i = 0; i < m_pDoc->plyLoader.faceArry.size(); i++)
	{
		if (isFill)
		{		
			/*switch (m_pDoc->plyLoader.faceArry[i].UVatlas_part)
			{
			case 0:
				glColor3f(1, 0.5, 0.5);
				break;
			case 1:
				glColor3f(0.5, 1, 0.5);
				break;
			case 2:
				glColor3f(1, 1, 0.5);
				break;
			case 3:
				glColor3f(0.5, 1, 1);
				break;
			case 4:
				glColor3f(1, 0.5, 0.75);
				break;
			case 5:
				glColor3f(0, 0.5, 0.75);
				break;
			case 6:
				glColor3f(0.5, 0.5, 0.75);
				break;
			case 7:
				glColor3f(0.5, 0.5, 1);
				break;
			case 8:
				glColor3f(1, 0.5, 0.5);
				break;
			case 9:
				glColor3f(0.5, 1, 0);
				break;
			default :
				glColor3f(1, 1, 1);
				break;
			}*/
 			/*if (m_pDoc->dr->m_triangleFaceArry[i].isMark&&false)
 			{
 				glColor3f(1, 0, 0);
 			}
 			else
 			{*/
// 				float c = m_pDoc->dr->m_triangleFaceArry[i].disFromCenter / 2.0;
// 				if (c > 0.8)
// 				{
// 					glColor3f(0, 0, 0);
// 				}
// 				else if (c > 0.5)
// 				{
// 					glColor3f(0, 0, c);
// 				}
// 				else if (c > 0.2)
// 				{
// 					glColor3f(0, c, 0);
// 				}
// 				else{
// 					glColor3f(c, 0, 0);
// 				}
 			glColor3f(0.7, 0.7, 0.7);
// 			}
		}
		else
		{
			if (Triangle->at(i).testV)
			{
				glColor3f(1, 0, 0);
			}
			else
			{
				glColor3f(0.3, 0.3, 0.3);
			}
		}
		float v1x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].x;
		float v1y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].y;
		float v1z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].z;

		float v2x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].x;
		float v2y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].y;
		float v2z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].z;

		float v3x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].x;
		float v3y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].y;
		float v3z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].z;


			if (v1x < -4e+8)
				break;
			//glNormal3fv(Triangle->at(i).n);
			glVertex3f(v1x, v1y, v1z);
			//glTexCoord2f(0, 0);
			glVertex3f(v2x, v2y, v2z);
			//glTexCoord2f(0,1);
			glVertex3f(v3x, v3y, v3z);


			//	glTexCoord2f(1,1);
		
	}
	glEnd();
	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_LINE);
	glLineWidth(1.0f);
	glColor3f(1, 0, 0);
	for (int i = 0; i < m_pDoc->plyLoader.faceArry.size(); i++)
	{

		//绘制法线
		

		float v4x = Triangle->at(i).corex;
		float v4y = Triangle->at(i).corey;
		float v4z = Triangle->at(i).corez;

		float v5x = Triangle->at(i).n[0] + v4x;
		float v5y = Triangle->at(i).n[1] + v4y;
		float v5z = Triangle->at(i).n[2] + v4z;
		glVertex3f(v4x, v4y, v4z);
		glVertex3f(v5x, v5y, v5z);
		
	}
	glEnd();
	glPolygonMode(GL_FRONT, GL_FILL);
	//glDisable(GL_TEXTURE_2D);
}

void CGLBaseView::OnDestroy()
{
	m_hRC = ::wglGetCurrentContext();
	::wglMakeCurrent(NULL, NULL);
	if (m_hRC)
	{
		::wglDeleteContext(m_hRC);
	}

	if (m_pDC)
	{
		delete m_pDC;
	}
	CView::OnDestroy();

}


BOOL CGLBaseView::OnEraseBkgnd(CDC* pDC)
{
	return CView::OnEraseBkgnd(pDC);
}


void CGLBaseView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	VERIFY(wglMakeCurrent(m_pDC->m_hDC, m_hRC));//确认RC与当前DC关联
	if (cy == 0) cy = 1;

	w = cx;
	h = cy;

	VERIFY(wglMakeCurrent(NULL, NULL));//确认DC释放RC
}


void CGLBaseView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	mousepoint = point;
	mousepoint2 = mousepoint1;
	mousepoint1 = point;
	
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}


void CGLBaseView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	mousepoint = CPoint(0, 0);
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}


void CGLBaseView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (GetCapture() == this)
	{
		//Increment the object rotation angles
		rotate_X += float((point.y - mousepoint.y) / 3.6);
		rotate_Y += float((point.x - mousepoint.x) / 3.6);
		//Redraw the view
		Invalidate(FALSE);
		//Set the mouse point
		mousepoint = point;
	};
	CView::OnMouseMove(nFlags, point);
}


BOOL CGLBaseView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值		texCoorGenTime	0	int
	if (zDelta > 0)//向前滚动，放大
	{
		/*scale_X = float(((zDelta / 120) + 0.1)*scale_X);
		scale_Y = float(scale_X);
		scale_Z = float(scale_X);*/
		scale_X = scale_X*1.1; 
		scale_Y = scale_Y*1.1;


	}
	else if (zDelta < 0)
	{
		/*zDelta = -zDelta;
		scale_X = float(((zDelta / 120) - 0.1)*scale_X);
		scale_Y = float(scale_X);
		scale_Z = float(scale_X);*/
		scale_X = scale_X*0.9;
		scale_Y = scale_Y*0.9;
	}

	Invalidate(FALSE);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CGLBaseView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	GLfloat  winX, winY;
	CRect re;
	GetWindowRect(&re);
	int screenHeight = re.Height(), screenWidth = re.Width();//屏幕宽和高

	//变换要绘图函数里的顺序一样，否则坐标转换会产生错误

	winX = GLfloat(point.x);
	winY = GLfloat(screenHeight - point.y);

	gluUnProject((GLdouble)winX, (GLdouble)winY, 0.0, modelview, projection, viewport, &nearX, &nearY, &nearZ);
	gluUnProject((GLdouble)winX, (GLdouble)winY, 1.0, modelview, projection, viewport, &farX, &farY, &farZ);

	g_color = 0.0;

	selectedFace.clear();

	if (IntersectTriangle(m_pDoc->plyLoader.faceArry, m_pDoc->plyLoader.pointArry))
	{
		g_color = 1.0;
		m_pDoc->isChooseFace = false;
		Invalidate(FALSE);
	}
	CView::OnRButtonDown(nFlags, point);
}


BOOL CGLBaseView::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类

	if (pMsg->message == WM_KEYDOWN)
	{
		keyBoard = pMsg->wParam;
		CDC * pCDC = GetDC();
		//分组显示
		if (keyBoard > 51 && keyBoard < 58)
		{
			m_pDoc->showGroup = keyBoard - 51;
			m_pDoc->plyLoader.showPart = keyBoard - 51;
		}

		switch (keyBoard)
		{
			//左37右39上38下40 
		case 37:
		{
				   //左
				   //
				   /*if (m_pDoc->isdrawbg)
				   {
					   diffuse[0] += 1;
				   }else
				   {
					   trans_X -= 0.01;
				   }*/
				  
				   for (int w = 0; w < 10; w++)
				   {
					   if (m_pDoc->_ftep[w])
					   {
						   m_pDoc->_ftep[w]->zBufferCut -= 0.05;
					   }
				   }
		}
			break;
		case 38:
		{
				   //上
				  // m_pDoc->show_ftep = true;
				   if (m_pDoc->isdrawbg)
				   {
					   diffuse[1] += 1;
				   }
				   else
				   {
					   trans_Y += 0.01;
				   }
		}
			break;
		case 39:
		{
				   //右
					for (int w = 0; w < 10; w++)
					{
						if (m_pDoc->_ftep[w])
						{
							m_pDoc->_ftep[w]->zBufferCut += 0.05;
						}
					}
				 /* if (m_pDoc->isdrawbg)
				  {
					  diffuse[0] -= 1;
				  }
				  else
				  {
					  trans_X += 0.01;
				  }*/
		}
			break;
		case 40:
		{
				   //下
				 //  
				   if (m_pDoc->isdrawbg)
				   {
					   diffuse[1] -= 1;
				   }
				   else
				   {
					   trans_Y -= 0.01;;
				   }
				  // m_pDoc->show_ftep = false;
				   
		}
			break;
		case 35:
		{
				   //Home 
				   if(m_pDoc->isdrawbg)
				   {
					   diffuse[2] += 1;
				   }
				   else
				   {
					   rotate_Z += 1;
				   }
		}
			break;
		case 36:
		{
				   //End
				   if (m_pDoc->isdrawbg)
				   {
					   diffuse[2] -= 1;
				   }
				   else
				   {
					   rotate_Z -= 1;
				   }
		}
			break;
		case 8:
		{
				//退格
				  CRect re;
				  GetWindowRect(&re);
				  int screenHeight = re.Height(), screenWidth = re.Width();//屏幕宽和高
				  for (int w = 0; w < 10; w++)
				  {
					  FindTextureElementPosition *ftep = m_pDoc->_ftep[w];
					  if (ftep)
					  {
						  for (int w = 0; w < ftep->m_targetTexture->tes.size(); w++)
						  {
							  bool teShow = true;
							  if (!ftep->m_targetTexture->tes[w]->isShow) continue;
							  int faceNum = ftep->m_targetTexture->tes[w]->face->facenum;
							  double x, y, z, WinX, WinY, WinZ;
							  x = ftep->m_targetTexture->tes[w]->face->corex;
							  y = ftep->m_targetTexture->tes[w]->face->corey;
							  z = ftep->m_targetTexture->tes[w]->face->corez;
							  gluProject((GLdouble)x, (GLdouble)y, (GLdouble)z, modelview, projection, viewport, &WinX, &WinY, &WinZ);
							  double maxX = max(mousepoint1.x, mousepoint2.x);
							  double minX = min(mousepoint1.x, mousepoint2.x);
							  double maxY = max(screenHeight - mousepoint1.y, screenHeight - mousepoint2.y);
							  double minY = min(screenHeight - mousepoint1.y, screenHeight - mousepoint2.y);
							  if ((WinX >= minX && WinX <= maxX) && (WinY >= minY && WinY <= maxY))
							  {
								  ftep->m_targetTexture->tes[w]->isShow = false;
							  }
						  }
					  }
				  }
		}
			break;
		case 13:
		{
				   //回车键
				   if (runtimes == 0)
				   {
					   SetTimer(1, 10, 0);
					   runtimes = 1;
					   TimeStart = GetTickCount();
				   }
				   else
				   {
					   runtimes = 0;
					   KillTimer(1);
				   }

				  
		}
			break;
		case 9:
		{
				  //Table键
				  if (runtimes == 0)
				  {
					  SetTimer(1, 10, 0);
					  runtimes = 1000;
					  TimeStart = GetTickCount();
				  }
				  else
				  {
					  runtimes = 0;
					  KillTimer(1);
				  }
		}break;
		case 45:
		{
				   CString s1;
				   s1.Format("diffuse %f,%f,%f", diffuse[0], diffuse[1], diffuse[2]);
				   //AfxMessageBox(s1);
				   //Insert
				   for (int k = 0; k < m_pDoc->selectfaceLsit.size(); k++)
				   {
					   Point3D pt(0, 0, 0);
					   gl_face * theFace = &m_pDoc->selectfaceLsit[k];
					   pt.setValue(theFace->corex, theFace->corey, theFace->corez);
					   int newElementIndex = m_pDoc->get_ftep()->m_targetTexture->tes.size();
					   TextureElement * tex = NULL;
					   tex = new TextureElement(newElementIndex, theFace, 1, pt);
					   m_pDoc->get_ftep()->getCoverRange(tex, 0.08);
					   m_pDoc->get_ftep()->setN(tex);
					   tex->cid = 2;
					   tex->textureIndex = 2;
					   m_pDoc->get_ftep()->m_targetTexture->addTE(tex);
				   }
				   m_pDoc->selectfaceLsit.clear();
				   if (isOpenDepthTest)
				   {
					   isOpenDepthTest = false;
				   }
				   else
				   {
					   isOpenDepthTest = true;
				   }
				  
		}break;
		case  32:
		{
					 //space
 				   m_pDoc->isdrawbg = !m_pDoc->isdrawbg;
				   m_pDoc->show_ftep = !m_pDoc->show_ftep;
		}
			break;
		}


	}

	Invalidate(FALSE);

	return CView::PreTranslateMessage(pMsg);
}

//OpenGL环境配置 2014.8.12
BOOL CGLBaseView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |          // support OpenGL
		PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ((pixelformat = ChoosePixelFormat(m_pDC->m_hDC, &pfd)) == 0)
	{
		MessageBox("ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->m_hDC, pixelformat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}

	return TRUE;
}

void CGLBaseView::makeCheckImage(void)
{
	src = cvLoadImage("单个苹果1.jpg");

	int i, j;
	for (i = 0; i<src->height; i++)
	{
		for (j = 0; j<src->width; j++)
		{
			if ((i != 0) && (j != 0) && (i != src->height + 1) && (j != src->width + 1))
			{
				checkImage[i][j][0] = CV_IMAGE_ELEM(src, uchar, src->height - i + 1, 3 * j + 2);//r
				checkImage[i][j][1] = CV_IMAGE_ELEM(src, uchar, src->height - i + 1, 3 * j + 1);//g
				checkImage[i][j][2] = CV_IMAGE_ELEM(src, uchar, src->height - i + 1, 3 * j);//b

				if (checkImage[i][j][0]>220 && checkImage[i][j][1]>220 && checkImage[i][j][2]>220)
					checkImage[i][j][3] = 0;//透明，设为 0 
				else
					checkImage[i][j][3] = 255;//不透明，设为 255, 也就是以后用的1.0 
			}
			else
			{
				checkImage[i][j][0] = (GLubyte)0;
				checkImage[i][j][1] = (GLubyte)0;
				checkImage[i][j][2] = (GLubyte)0;
				checkImage[i][j][3] = (GLubyte)0;
			}

		}
	}
}
void CGLBaseView::LoadGLTextures()
{
	if (isInit)
	{
		return;
	}
	/// 文件名 
	string fileNameBackground;
	string fileNameBackgroundres = "background.bmp";
	
	string fileNamepro = "texels";
	string fileNamepro2 = "/te (";
	string fileNamepro3 = "/";
	int m = texturesampleIndex;
	std::stringstream ssm;
	std::string sampleIndexNumString;
	ssm << m;
	ssm >> sampleIndexNumString;
	string fileNameres = ").bmp";
	string fileName;

	fileNameBackground = fileNamepro + sampleIndexNumString + fileNamepro3 + fileNameBackgroundres;
	m_texture[1].Load(fileNameBackground.c_str());
	int num = 11;
	if (texturesampleIndex==1)
	{
		num = 11;
	}
	if (texturesampleIndex == 2)
	{
		num = 5;
	}
	if (texturesampleIndex == 3)
	{
		num = 52;
	}
	if (texturesampleIndex == 4)
	{
		num = 8;
	}
	if (texturesampleIndex == 5)
	{
		num = 15;
	}
	if (texturesampleIndex == 6)
	{
		num = 16;
	}
	if (texturesampleIndex == 7)
	{
		num = 6;
	}
	if (texturesampleIndex == 8)
	{
		num = 12;
	}
	if (texturesampleIndex == 9)
	{
		num = 10;
	}
	if (texturesampleIndex == 10)
	{
		num = 10;
	}
	if (texturesampleIndex == 11)
	{
		num = 10;
	}
	for (int k = 1; k<num; k++)
	{
		std::stringstream ss;
		std::string numString;
		ss << k;
		ss >> numString;
		fileName = fileNamepro + sampleIndexNumString + fileNamepro2 + numString + fileNameres;
		if (!m_texture[k + 1].Load(fileName.c_str()))
		{
			return;
		}
	}
	/*/// 载入四幅位图
	for (int i = 0; i<7; i++)
	{
	if (!m_texture[i].Load(fileName[i]))
	{
	return;
	}

	}
	*/
	return;

}

void CGLBaseView::InitialOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;
	int  n;
	m_pDC = new CClientDC(this);
	ASSERT(m_pDC != NULL);
	if (!SetupPixelFormat())
		return;
	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	glewInit();
	isFill = false;
	
}

void CGLBaseView::cutFtep(FindTextureElementPosition * ftep)
{
	return;
	int i = 0;
	double x, y, z = 0;
	double nx, ny, nz = 0;
	double px, py, pz = 0;
	//屏幕上显示坐标
	double winX, winY, winZ;
	double nwinX, nwinY, nwinZ;
	double pwinX, pwinY, pwinZ;
	for (i = 0; i < ftep->m_targetTexture->tes.size(); i++)
	{
		TextureElement * te = ftep->m_targetTexture->tes[i];
		x = te->pos[0];
		y = te->pos[1];
		z = te->pos[2];
	
		nx = te->n[0];
		ny = te->n[1];
		nz = te->n[2];
		/*if (te->isCovered)
		{
			continue;
		}*/
		//gluUnProject((GLdouble)winX, (GLdouble)winY, 0.0, modelview, projection, viewport, &nearX, &nearY, &nearZ);
		gluProject(x, y, z, modelview, projection, viewport, &winX, &winY, &winZ);
		gluProject(nx, ny, nz, modelview, projection, viewport, &nwinX, &nwinY, &nwinZ);
		if (nwinZ <= 0)
		{
			te->isCovered = true;
		}
		else
		{	
			te->isCovered = false;
		}
	/*	for (int j = 0; j < 4; j++)
		{
			if (te->isCovered)
			{
				break;
			}
			gl_face * gf = te->coverFace.at(j);
			for (int k = 0; k < m_pDoc->plyLoader.pointArry.size(); k++)
			{
				if (te->isCovered)
				{
					break;
				}
				px = m_pDoc->plyLoader.pointArry.at(k).x;
				py = m_pDoc->plyLoader.pointArry.at(k).y;
				pz = m_pDoc->plyLoader.pointArry.at(k).z;
				gluProject(px, py, pz, modelview, projection, viewport, &pwinX, &pwinY, &pwinZ);

				if (pwinZ<winZ)
				{
					te->isCovered = true;
				}
			}
		}*/
	}
}
void CGLBaseView::DrawScene()
{
	/** 用户自定义的绘制过程 */

	bool isVectorShown = false;
	//绘制
	if (m_pDoc->plyLoader.TotalFaces > 0)
	{
 		switch (keyBoard)
 		{
 		case 'p':
 			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);//点画模式
 			break;
 		case 'P':
 			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);//点画模式
 			break;
 		case 'g':
 			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//网格模式
 			break;
 		case 'G':
 			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//网格模式
 			break;
 		case 'f':
 			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充模式
 			break;
 		case 'F':
 			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充模式
 			break;
 		case 'v':
 		{
 					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充模式
 					isVectorShown = true;
 		}
 			break;
 		case 'V':
 		{
 					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充模式
 					isVectorShown = true;
 		}
 			break;
 		case '2':
 		{
 					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//网格模式
 					modelShowIn3Dor2D = false;
 		}
 			break;
 		case '3':
 		{
 					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//网格模式
 					modelShowIn3Dor2D = true;
 		}
 			break;
 
 
 		default:
 		{
 				   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//网格模式
 				   isVectorShown = false;
 		}
 			break;
 		}
 			isFill = true;
 			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 			glColor3f(0.75, 0.75, 0.75);
 			drawPLY();
		/*	glEnable(GL_POLYGON_OFFSET_LINE);
			glPolygonOffset(-1.0f, -1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);;
			glColor3f(0.3, 0.3, 0.3);
			isFill = false;
			drawPLY();
			glDisable(GL_POLYGON_OFFSET_LINE);*/



		glDisable(GL_DEPTH_TEST);
		glDisable(GL_DEPTH_BUFFER_BIT);
		if (isOpenDepthTest)
		{
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_DEPTH_BUFFER_BIT);
		}
		if (m_pDoc->show_ftep&&!m_pDoc->isdrawbg)
		{//
			glPolygonMode(GL_FRONT, GL_FILL);//填充模式
			for (int w = 0; w < 10; w++)
			{
				if (m_pDoc->_ftep[w])
				{
					cutFtep(m_pDoc->_ftep[w]);
					m_pDoc->_ftep[w]->draw();
				}
			}
		}
		if (isOpenDepthTest)
		{
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_DEPTH_BUFFER_BIT);
		}
	}
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.5f, -1.5f);
	glPolygonMode(GL_FRONT, GL_FILL);;
	//绘制选择的面片
	vector<gl_face> * Triangle = &(m_pDoc->plyLoader.faceArry);
	vector<gl_point> * Vertex = &(m_pDoc->plyLoader.pointArry);
	vector<gl_point2d> * Vertex2d = &(m_pDoc->plyLoader.point2DArry);

	int counti = 0;
	for (vector<gl_face>::iterator it = m_pDoc->selectfaceLsit.begin(); it != m_pDoc->selectfaceLsit.end(); it++)
	{
		/*if(!Triangle->at(counti).textureclick)
		{*/
		glColor3f(0.25, 0.36, 0.85);

		glBegin(GL_TRIANGLES);
		glVertex3f(m_pDoc->plyLoader.pointArry[it->ptnum[0]].x,
			m_pDoc->plyLoader.pointArry[it->ptnum[0]].y,
			m_pDoc->plyLoader.pointArry[it->ptnum[0]].z);

		glVertex3f(m_pDoc->plyLoader.pointArry[it->ptnum[1]].x,
			m_pDoc->plyLoader.pointArry[it->ptnum[1]].y,
			m_pDoc->plyLoader.pointArry[it->ptnum[1]].z);

		glVertex3f(m_pDoc->plyLoader.pointArry[it->ptnum[2]].x,
			m_pDoc->plyLoader.pointArry[it->ptnum[2]].y,
			m_pDoc->plyLoader.pointArry[it->ptnum[2]].z);

		glEnd();
		/*}*/
		counti++;
	}

 	glDisable(GL_POLYGON_OFFSET_FILL);
	if (isVectorShown)
	{
		glBegin(GL_LINES);//绘制向量场
		int i = 0;
		for (i = 0; i < Triangle->size(); i++)
		{
			glColor3f(1.0, 1.0, 1.0);
			glVertex3f(Triangle->at(i).corex, Triangle->at(i).corey, Triangle->at(i).corez);
			glVertex3f(Triangle->at(i).endx, Triangle->at(i).endy, Triangle->at(i).endz);
		}
		glEnd();
		glPopMatrix();
		glFinish();
	}
	else
	{

	}
	glFlush();
}

void CGLBaseView::draw2DModel()
{
	vector<gl_face> * Triangle = &(m_pDoc->plyLoader.faceArry);
	vector<gl_point2d> * Vertex2d = &(m_pDoc->plyLoader.point2DArry);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	rotate_X = 0;
	rotate_Y = 0;
	for (int i = 0; i<Triangle->size(); i++)
	{

		//if(i!=0&&i!=2677!=2386&&i!=2387)
		//{
		//	
		///*&&i!=2386&&i!=2387&&i!=2690&&i!=350	
		//&&i!=2386
		//	&&i!=2387&&i!=2690&&i!=350*/
		//	continue;
		//}
		//float v1x=Vertex2d->at(Triangle->at(i).ptnum_2d[0]).x;
		//float v1y=Vertex2d->at(Triangle->at(i).ptnum_2d[0]).y;
		//float v1z=0;

		//float v2x=Vertex2d->at(Triangle->at(i).ptnum_2d[1]).x;
		//float v2y=Vertex2d->at(Triangle->at(i).ptnum_2d[1]).y;
		//float v2z=0;

		//float v3x=Vertex2d->at(Triangle->at(i).ptnum_2d[2]).x;
		//float v3y=Vertex2d->at(Triangle->at(i).ptnum_2d[2]).y;
		//float v3z=0;

		float v1x = Triangle->at(i)._show2dx[0];
		float v1y = Triangle->at(i)._show2dy[0];
		float v1z = 0;

		float v2x = Triangle->at(i)._show2dx[1];
		float v2y = Triangle->at(i)._show2dy[1];
		float v2z = 0;

		float v3x = Triangle->at(i)._show2dx[2];
		float v3y = Triangle->at(i)._show2dy[2];
		float v3z = 0;


		if (i == 350)
		{
			cout << "";
		}
		if (Triangle->at(i).textureclick == true && ((Triangle->at(i).renderTime == m_pDoc->showGroup - 1) || (m_pDoc->showGroup == 1)))
		{
			/*	if(v1x<-4e+8)
			break;*/
			//glColor3f(0.5,0.5,0.5);//灰色
			glColor3f(Triangle->at(i).r, Triangle->at(i).g, Triangle->at(i).b);

			glVertex3f(v1x, v1y, v1z);
			glVertex3f(v2x, v2y, v2z);
			glVertex3f(v3x, v3y, v3z);
		}

	}
	glEnd();
	glFlush();
}

bool CGLBaseView::IntersectTriangle(vector<gl_face> facetarry, vector<gl_point> pointarry)
{
	GLfloat edge1[3];
	GLfloat edge2[3];
	for (vector<gl_face>::iterator face_it = facetarry.begin(); face_it != facetarry.end(); face_it++)
	{
		edge1[0] = pointarry[face_it->ptnum[1]].x - pointarry[face_it->ptnum[0]].x;
		edge1[1] = pointarry[face_it->ptnum[1]].y - pointarry[face_it->ptnum[0]].y;
		edge1[2] = pointarry[face_it->ptnum[1]].z - pointarry[face_it->ptnum[0]].z;

		edge2[0] = pointarry[face_it->ptnum[2]].x - pointarry[face_it->ptnum[0]].x;
		edge2[1] = pointarry[face_it->ptnum[2]].y - pointarry[face_it->ptnum[0]].y;
		edge2[2] = pointarry[face_it->ptnum[2]].z - pointarry[face_it->ptnum[0]].z;

		GLfloat dir[3];
		dir[0] = GLfloat(farX - nearX);
		dir[1] = GLfloat(farY - nearY);
		dir[2] = GLfloat(farZ - nearZ);

		GLfloat w = (GLfloat)sqrt((double)(pow((double)dir[0], 2.0) + (double)pow((double)dir[1], 2.0) + (double)pow((double)dir[2], 2.0)));
		dir[0] /= w;
		dir[1] /= w;
		dir[2] /= w;

		GLfloat pvec[3];
		pvec[0] = dir[1] * edge2[2] - dir[2] * edge2[1];
		pvec[1] = dir[2] * edge2[0] - dir[0] * edge2[2];
		pvec[2] = dir[0] * edge2[1] - dir[1] * edge2[0];

		GLfloat det;
		det = edge1[0] * pvec[0] + edge1[1] * pvec[1] + edge1[2] * pvec[2];

		GLfloat tvec[3];
		if (det > 0)
		{
			tvec[0] = GLfloat(nearX - pointarry[face_it->ptnum[0]].x);
			tvec[1] = GLfloat(nearY - pointarry[face_it->ptnum[0]].y);
			tvec[2] = GLfloat(nearZ - pointarry[face_it->ptnum[0]].z);
		}
		else
		{
			tvec[0] = GLfloat(pointarry[face_it->ptnum[0]].x - nearX);
			tvec[1] = GLfloat(pointarry[face_it->ptnum[0]].y - nearY);
			tvec[2] = GLfloat(pointarry[face_it->ptnum[0]].z - nearZ);

			det = -det;
		}

		if (det < 0.0001f)   continue;

		GLfloat u;
		u = tvec[0] * pvec[0] + tvec[1] * pvec[1] + tvec[2] * pvec[2];

		if (u < 0.0f || u > det)   continue;

		GLfloat qvec[3];
		qvec[0] = tvec[1] * edge1[2] - tvec[2] * edge1[1];
		qvec[1] = tvec[2] * edge1[0] - tvec[0] * edge1[2];
		qvec[2] = tvec[0] * edge1[1] - tvec[1] * edge1[0];

		GLfloat v;
		v = dir[0] * qvec[0] + dir[1] * qvec[1] + dir[2] * qvec[2];
		if (v < 0.0f || u + v > det)      continue;

		GLfloat t = edge2[0] * qvec[0] + edge2[1] * qvec[1] + edge2[2] * qvec[2];
		GLfloat fInvDet = 1.0f / det;
		t *= fInvDet;
		u *= fInvDet;
		v *= fInvDet;

		face_it->length = -t;
		selectedFace.push_back(*face_it);
		//m_pDoc->plyLoader.faceArry.at(face_it->facenum).isShowColorIn3D=true;
	}

	if (selectedFace.size() == 0)
	{
		return false;
	}
	else
	{
		float f_max_length = 0; int selectedNum = -1;
		for (size_t i = 0; i < selectedFace.size(); i++)
		{

			if (i == 0)
			{
				selectedNum = i;
				f_max_length = selectedFace[i].length;
			}
			else if (f_max_length < selectedFace[i].length)
			{
				f_max_length = selectedFace[i].length;
				selectedNum = i;
			}
		}

		if (selectedNum > -1)
		{
			m_pDoc->selectfaceLsit.push_back(selectedFace[selectedNum]);
		}

		m_pDoc->UpdateAllViews(NULL);
		m_pDoc->userSelectingTriangleIndex = selectedFace[selectedNum].facenum;
		return true;
	}
}
void CGLBaseView::DrawTest()
{
	glPushMatrix();
	glTranslatef(trans_X, trans_Y, trans_Z);

	glScalef(scale_X*1.0f, scale_Y*1.0f, scale_Z*1.0f);

	glRotatef(rotate_X, 1.0f, 0.0f, 0.0f);
	glRotatef(rotate_Y, 0.0f, 1.0f, 0.0f);



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_FILL);//网格模式
	glEnable(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);// 使用深度
	/*glGetIntegerv(GL_VIEWPORT, viewport); // 得到的是最后一个设置视口的参数
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);*/

	/** 用户自定义的绘制过程 */

	/** 激活纹理0,并绑定纹理 */
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[0].ID);

	/** 激活纹理1,并绑定纹理 */
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[1].ID);

	/** 绘制一个四方形墙面 */
	glPushMatrix();
	glTranslatef(-2.5, 0, 0);
	glScalef(2.0f, 2.0f, 2.0f);
	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);
	/** 左上点 */
	//glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
	glVertex3f(-1, 1, 0);

	/** 左下点 */
	//glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
	glVertex3f(-1, -1, 0);

	/** 右下点 */
	//glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
	glVertex3f(1, -1, 0);

	/** 右上点 */
	//glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
	glVertex3f(1, 1, 0);

	//________________________

	/** 左上点 */
	glColor3f(1, 0, 0);
	//glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
	glVertex3f(-1.2, 0.8, -1);

	/** 右上点 */
	//glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
	glVertex3f(0.8, 0.8, -1);

	/** 右下点 */
	//glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
	glVertex3f(0.8, -1.2, -1);

	/** 左下点 */
	//glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
	//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
	glVertex3f(-1.2, -1.2, -1);




	//glEnd();    /**< 绘制结束 */
	glColor3f(0, 0, 1);
	glVertex3f(-1.1, 0.9, -0.5);
	glVertex3f(-1.1, -1.1, -0.5);
	glVertex3f(0.9, -1.1, -0.5);
	glVertex3f(0.9, 0.9, -0.5);
	//glBegin(GL_QUADS);

	glEnd();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_BUFFER_BIT);

	glPopMatrix();


	///** 激活纹理0,并绑定纹理 */
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D,  m_texture[2].ID);

	///** 激活纹理1,并绑定纹理 */
	//glActiveTextureARB(GL_TEXTURE1_ARB);
	//
	///** 如果多重纹理启用,则启用该纹理 */
	//if (multitexturing) 
	//	glEnable(GL_TEXTURE_2D); 
	//else 
	//	glDisable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D,  m_texture[3].ID);

	//static float wrap = 0;      /**< 用于雾的流动 */     

	//glTranslatef(2.5, 0, 0);
	//glScalef(2.0f,2.0f,2.0f);
	//glBegin(GL_QUADS);

	//	/** 左上点 */
	//	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
	//	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f - wrap, 1.0f);
	//	glVertex3f(-1, 1, 0);

	//	/** 左下点 */
	//	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
	//	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f - wrap, 0.0f);
	//	glVertex3f(-1, -1, 0);

	//	/** 右下点 */
	//	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
	//	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f - wrap, 0.0f);
	//	glVertex3f(1, -1, 0);

	//	/** 右上点 */
	//	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
	//	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f - wrap, 1.0f);
	//	glVertex3f(1, 1, 0);
	//glEnd();											

	//wrap += 0.001f;                   /**< 递增 */


	//glFlush();	                     /**< 强制执行所有的OpenGL命令 */
}
void CGLBaseView::drawPLYwithMultiTextureTEST()
{
	vector<gl_face> * Triangle = &(m_pDoc->plyLoader.faceArry);
	vector<gl_point> * Vertex = &(m_pDoc->plyLoader.pointArry);
	vector<gl_point2d> * Vertex2d = &(m_pDoc->plyLoader.point2DArry);
	int i = 8451;
	float v1x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].x;
	float v1y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].y;
	float v1z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].z;
	float v2x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].x;
	float v2y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].y;
	float v2z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].z;
	float v3x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].x;
	float v3y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].y;
	float v3z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].z;
	float v1u, v1v, v2u, v2v, v3u, v3v;
	int texCoordsSize = 4;
	int textureIndex = 2;
	int textureIndexs[4] = { 43, 47, 48, 6 };
	float texCoords[4][3][2] = {
		0.958469, 0.171411,
		1.03641, 1.05568,
		0.572128, 1.00791,

		0.958469, 0.171411,
		1.03641, 1.05568,
		0.572128, 1.00791,

		0.440943, 0.792896,
		0.216928, -0.10618,
		- 0.272575, -0.0122489,

		0.67861, 1.02167,
		0.673813, 0.137932,
		0.176888, 0.26701
	};
	
	for (int j = 1; j <= texCoordsSize; j++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + j);
		glEnable(GL_TEXTURE_2D);
		textureIndex = textureIndexs[j-1];
		glBindTexture(GL_TEXTURE_2D, m_texture[textureIndex].ID);
	}
	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_TRIANGLES);//显示
	glNormal3fv(Triangle->at(i).n);
	for (int j = 1; j <= texCoordsSize; j++)
	{
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB + j, texCoords[j - 1][0][0], texCoords[j - 1][0][1]);
	}
	glVertex3f(v1x, v1y, v1z);
	for (int j = 1; j <= texCoordsSize; j++)
	{
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB + j, texCoords[j - 1][1][0], texCoords[j - 1][1][1]);
	}
	glVertex3f(v2x, v2y, v2z);
	for (int j = 1; j <= texCoordsSize; j++)
	{
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB + j, texCoords[j - 1][2][0], texCoords[j - 1][2][1]);
	}
	glVertex3f(v3x, v3y, v3z);
	glEnd();
	for (int j = 1; j <= texCoordsSize; j++)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + j);
		glDisable(GL_TEXTURE_2D);
	}
	glFinish();

}
void CGLBaseView::drawPLYwithMultiTextureTEST2()
{

}
void CGLBaseView::setupShaders()
{
	// variables to hold the shader's source code
	char *vs = NULL, *fs = NULL;



	// create the two shaders
	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// read the source code from file
	vs = textFileRead("texture.vert");
	fs = textFileRead("texture.frag");
	// castings for calling the shader source function
	const char * vv = vs;
	const char * ff = fs;

	// setting the source for each shader
	glShaderSource(VertexShaderID, 1, &vv, NULL);
	glShaderSource(FragmentShaderID, 1, &ff, NULL);

	// free the source strings
	free(vs); free(fs);
	

	// compile the sources
	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);


	// create a program and attach the shaders
	shaderID = glCreateProgram();
	glAttachShader(shaderID, VertexShaderID);
	glAttachShader(shaderID, FragmentShaderID);
	glLinkProgram(shaderID);



}
void CGLBaseView::bindVertexBuffer()
{
	vector<gl_face> * Triangle = &(m_pDoc->plyLoader.faceArry);
	vector<gl_point> * Vertex = &(m_pDoc->plyLoader.pointArry);
	vector<gl_point2d> * Vertex2d = &(m_pDoc->plyLoader.point2DArry);
	GLuint vertexLoc;
	GLuint texCoordLocs[16];	// Get the locations of the attributes in the current program
	vertexLoc = glGetAttribLocation(shaderID, "position");	
	for (int k = 0; k < 16; k++)
	{
		string texCoord = "texCoord";
		stringstream ss;
		std::string numString;
		ss << k + 1;
		ss >> numString;
		char *texCor = new char[(texCoord + numString).size() + 1];
		strcpy(texCor, (texCoord + numString).c_str());
		texCoordLocs[k] = glGetAttribLocation(shaderID, texCor);
	}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float * position, **textureCoords, *texID_1, *texID_2, *texID_3, *texID_4;
	float * normals;
	vertSize = 3 * 4 * m_pDoc->plyLoader.faceArry.size();
	textureCoordSize = 3 * 2 * m_pDoc->plyLoader.faceArry.size();
	int textureIndexSize = 3 * 4 * m_pDoc->plyLoader.faceArry.size();
	position = new float[vertSize];
	normals = new float[vertSize];
	textureCoords = new float*[16];
	texID_1 = new float[textureIndexSize];
	texID_2 = new float[textureIndexSize];
	texID_3 = new float[textureIndexSize];
	texID_4 = new float[textureIndexSize];
	for (int k = 0; k < 16;k++)
	{
		textureCoords[k] = new float[textureCoordSize];
	}
	for (int i = 0; i < m_pDoc->plyLoader.faceArry.size(); i++)
	{
		float v1x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].x;
		float v1y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].y;
		float v1z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[0]].z;

		float v2x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].x;
		float v2y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].y;
		float v2z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[1]].z;

		float v3x = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].x;
		float v3y = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].y;
		float v3z = m_pDoc->plyLoader.pointArry[Triangle->at(i).ptnum[2]].z;

		position[4 * i * 3 + 0] = v1x;  normals[4 * i * 3 + 0] = Triangle->at(i).n[0];
		position[4 * i * 3 + 1] = v1y;	normals[4 * i * 3 + 1] = Triangle->at(i).n[1];
		position[4 * i * 3 + 2] = v1z;	normals[4 * i * 3 + 2] = Triangle->at(i).n[2];
		position[4 * i * 3 + 3] = 1;	normals[4 * i * 3 + 3] = 1;

		position[4 * i * 3 + 4] = v3x;	normals[4 * i * 3 + 4] = Triangle->at(i).n[0];
		position[4 * i * 3 + 5] = v3y;	normals[4 * i * 3 + 5] = Triangle->at(i).n[1];
		position[4 * i * 3 + 6] = v3z;	normals[4 * i * 3 + 6] = Triangle->at(i).n[2];
		position[4 * i * 3 + 7] = 1;	normals[4 * i * 3 + 7] = 1;

		position[4 * i * 3 + 8] = v2x;	normals[4 * i * 3 + 8] = Triangle->at(i).n[0];
		position[4 * i * 3 + 9] = v2y;	normals[4 * i * 3 + 9] = Triangle->at(i).n[1];
		position[4 * i * 3 + 10] = v2z;	normals[4 * i * 3 + 10] = Triangle->at(i).n[2];
		position[4 * i * 3 + 11] = 1;	normals[4 * i * 3 + 11] = 1;


		for (int j = 0; j < 16;j++)
 		{
			if (j<Triangle->at(i).texCoords.size())
			{
				textureCoords[j][2 * i * 3 + 0] = Triangle->at(i).texCoords[j]->cor[0][0];
				textureCoords[j][2 * i * 3 + 1] = Triangle->at(i).texCoords[j]->cor[0][1];
				textureCoords[j][2 * i * 3 + 2] = Triangle->at(i).texCoords[j]->cor[2][0];
				textureCoords[j][2 * i * 3 + 3] = Triangle->at(i).texCoords[j]->cor[2][1];
				textureCoords[j][2 * i * 3 + 4] = Triangle->at(i).texCoords[j]->cor[1][0];
				textureCoords[j][2 * i * 3 + 5] = Triangle->at(i).texCoords[j]->cor[1][1];
				//3个顶点的V都一样
				if (j<4)
				{
					texID_1[3 * 4 * i + j + 0] = Triangle->at(i).texCoords[j]->textureIndex - 1;//0对应的是背景
					texID_1[3 * 4 * i + j + 4] = Triangle->at(i).texCoords[j]->textureIndex - 1;
					texID_1[3 * 4 * i + j + 8] = Triangle->at(i).texCoords[j]->textureIndex - 1;
				}
				else if (j < 8)
				{
					texID_2[3 * 4 * i + (j - 4) + 0] = Triangle->at(i).texCoords[j]->textureIndex - 1;//0对应的是背景
					texID_2[3 * 4 * i + (j - 4) + 4] = Triangle->at(i).texCoords[j]->textureIndex - 1;
					texID_2[3 * 4 * i + (j - 4) + 8] = Triangle->at(i).texCoords[j]->textureIndex - 1;
				}
				else if (j < 12)
				{
					texID_3[3 * 4 * i + (j - 8) + 0] = Triangle->at(i).texCoords[j]->textureIndex - 1;//0对应的是背景
					texID_3[3 * 4 * i + (j - 8) + 4] = Triangle->at(i).texCoords[j]->textureIndex - 1;
					texID_3[3 * 4 * i + (j - 8) + 8] = Triangle->at(i).texCoords[j]->textureIndex - 1;
				}
				else if (j < 16)
				{
					texID_4[3 * 4 * i + (j - 12) + 0] = Triangle->at(i).texCoords[j]->textureIndex - 1;//0对应的是背景
					texID_4[3 * 4 * i + (j - 12) + 4] = Triangle->at(i).texCoords[j]->textureIndex - 1;
					texID_4[3 * 4 * i + (j - 12) + 8] = Triangle->at(i).texCoords[j]->textureIndex - 1;
				}

			}
			else
			{
				textureCoords[j][2 * i * 3 + 0] = 0;
				textureCoords[j][2 * i * 3 + 1] = 0;
				textureCoords[j][2 * i * 3 + 2] = 0;
				textureCoords[j][2 * i * 3 + 3] = 0;
				textureCoords[j][2 * i * 3 + 4] = 0;
				textureCoords[j][2 * i * 3 + 5] = 0;
				if (j < 4)
				{
					texID_1[3 * 4 * i + j + 0] = -1;
					texID_1[3 * 4 * i + j + 4] = -1;
					texID_1[3 * 4 * i + j + 8] = -1;
				}
				else if (j < 8)
				{
					texID_2[3 * 4 * i + (j - 4) + 0] = -1;
					texID_2[3 * 4 * i + (j - 4) + 4] = -1;
					texID_2[3 * 4 * i + (j - 4) + 8] = -1;
				}
				else if (j < 12)
				{
					texID_3[3 * 4 * i + (j - 8) + 0] = -1;
					texID_3[3 * 4 * i + (j - 8) + 4] = -1;
					texID_3[3 * 4 * i + (j - 8) + 8] = -1;
				}
				else if (j < 16)
				{
					texID_4[3 * 4 * i + (j - 12) + 0] = -1;
					texID_4[3 * 4 * i + (j - 12) + 4] = -1;
					texID_4[3 * 4 * i + (j - 12) + 8] = -1;
				}
			}
 		}
	}
	GLuint buffers[16];
	glGenBuffers(16, buffers);
	// bind buffer for vertices and copy data into buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, vertSize*sizeof(float), position, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);
	for (int k = 0; k < 12; k++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffers[k + 1]);
		glBufferData(GL_ARRAY_BUFFER, textureCoordSize* sizeof(float), textureCoords[k], GL_STATIC_DRAW);
		glEnableVertexAttribArray(texCoordLocs[k]);
		glVertexAttribPointer(texCoordLocs[k], 2, GL_FLOAT, 0, 0, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffers[13]);
	glBufferData(GL_ARRAY_BUFFER, textureIndexSize*sizeof(float), texID_1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(shaderID, "texID_1"));
	glVertexAttribPointer(glGetAttribLocation(shaderID, "texID_1"), 4, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[14]);
	glBufferData(GL_ARRAY_BUFFER, textureIndexSize*sizeof(float), texID_2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(shaderID, "texID_2"));
	glVertexAttribPointer(glGetAttribLocation(shaderID, "texID_2"), 4, GL_FLOAT, 0, 0, 0);

// 	glBindBuffer(GL_ARRAY_BUFFER, buffers[15]);
// 	glBufferData(GL_ARRAY_BUFFER, textureIndexSize*sizeof(float), texID_3, GL_STATIC_DRAW);
// 	glEnableVertexAttribArray(glGetAttribLocation(shaderID, "texID_3"));
// 	glVertexAttribPointer(glGetAttribLocation(shaderID, "texID_3"), 4, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[15]);
	glBufferData(GL_ARRAY_BUFFER, vertSize*sizeof(float), normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(shaderID, "normals"));
	glVertexAttribPointer(glGetAttribLocation(shaderID, "normals"), 4, GL_FLOAT, 0, 0, 0);
	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);


}

void CGLBaseView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CView::OnTimer(nIDEvent);
}
