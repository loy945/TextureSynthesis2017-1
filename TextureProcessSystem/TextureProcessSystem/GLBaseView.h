#pragma once
#include "stdafx.h"
#include "TextureProcessSystemDoc.h"
#include "plyloader.h"
#include "cv.h"
#include "highgui.h"
#include <opencv2/opencv.hpp> 
// CGLBaseView ��ͼ
#define  BUFFER_LENGTH 512
#include "var.h"
#include "CBMPLoader.h"

class CGLBaseView : public CView
{
	DECLARE_DYNCREATE(CGLBaseView)

protected:
	CGLBaseView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CGLBaseView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	CTextureProcessSystemDoc* GetDocument(); //��ȡϵͳ�ĵ���

	//����OpenGL����
public:
	int w;//��
	int h;//���

	HGLRC m_hRC;  //��ͼ RC
	CClientDC * m_pDC; //��ͼ DC
	CTextureProcessSystemDoc *m_pDoc;

	virtual void DrawScene(); //���Ƴ���

	BOOL SetupPixelFormat(); //��ͼ������
	void InitialOpenGL(); //��ʼ��OpenGL
	bool initMultiTexture();
	bool isExtensionSupported(const char* string);
	void makeCheckImage(void);
	void LoadGLTextures(void);
	void drawPLY(void);
	void drawPLYwithMultiTexture();
	void drawPLYwithMultiTextureTEST();
	void drawPLYwithMultiTextureTEST2();
	void drawPLYwithGLSL();
	//2014.8.11 ģ����ת�任�������
	bool isInit;
	//2015.3.2���ģ��չ��Ϊƽ�����
	bool modelShowIn3Dor2D;//true������ʾ��άģ�� false��ʾչ������ά�ϵ�ģ��
	void draw2DModel();
	CBMPLoader m_texture[53];      /**< ����52������ */
	GLuint texture;
	void DrawTest();
	void cutFtep(FindTextureElementPosition * ftep);
	bool isFill;
	bool isOpenDepthTest;
	//
	bool isBindVertex;
	char *texCor[6];
	DWORD TimeStart, TimeEnd, TimeUsed;
	int runtimes;
public:
	//���ű���
	float scale_X;
	float scale_Y;
	float scale_Z;

	//��ת����
	float rotate_X;
	float rotate_Y;
	float rotate_Z;

	//ƽ�Ʊ���
	float trans_X;
	float trans_Y;
	float trans_Z;

	//��굱ǰ�����
	CPoint mousepoint;
	CPoint mousepoint1;
	CPoint mousepoint2;
	//���̰��� G ������ʾ P �㻭ģʽ F���ģʽ
	char keyBoard;

	//ѡ����Ƭ 2014.8.11
	//2016.4.24
	void setupShaders();
	GLuint shaderID;
	GLuint VertexShaderID;
	GLuint FragmentShaderID;
	void bindVertexBuffer();
	GLuint vao;
	int vertSize;
	int textureCoordSize;
public:
	bool IntersectTriangle(vector<gl_face> faceArry, vector<gl_point> pointarry);//�ж������Ƿ��ཻ

	GLuint selectBuff[BUFFER_LENGTH];//ѡ�񻺴�
	GLint viewport[4];//�ӵ�
	GLdouble modelview[16];//ģ�;���
	GLdouble projection[16];//ͶӰ����

	GLdouble nearX, nearY, nearZ;//�������
	GLdouble farX, farY, farZ;//�����Ͻ�Զ�ĵ�

	GLfloat g_color;//ѡ����Ƭ��ɫ

	vector<gl_face> selectedFace;//��ű�ѡ�е���Ƭ
	GLfloat diffuse[4];//��Դλ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg virtual void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg virtual void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg virtual void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg virtual void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


