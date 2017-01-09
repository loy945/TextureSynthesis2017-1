
// TextureProcessSystemDoc.h : CTextureProcessSystemDoc ��Ľӿ�
//


#pragma once
#include "plyloader.h"
#include "stdafx.h"
#include "DistanceRecord.h"
#include "ObjLoader.h"
class FindTextureElementPosition;

class CTextureProcessSystemDoc : public CDocument
{
protected: // �������л�����
	CTextureProcessSystemDoc();
	DECLARE_DYNCREATE(CTextureProcessSystemDoc)

// ����
public:
	COLORREF m_BackColor;
// ����
public:
	Model_PLY plyLoader;
	bool isChooseFace;//��ʾ��ѡ�����Ƭ�ǣ���֮Ϊtrue
	//2015.4.6
	int centerIndex;
public:
	vector<gl_face> selectfaceLsit;
	void calculateValue(char rbffuntion);
	int  findAllPoint();
	int findFaceByTwoPoint(int point1,int point2,int oldFaceIndex);//����������������
	vector<int>  CTextureProcessSystemDoc::findFaceByTwoPointNewWay(int point1,int point2,int oldFaceIndex);//�������ݽṹ����·���
	void rbfcreate(char rbffunt,float *res,int *num,float *coe);
	double rbfliner(float r,float constant);//���Ժ���
	double rbfclbic(float r,float constant);//���η�����
	double rbfgaussian(float r,float constant);//��˹����
	double rbfmultiquadrics(float r,float constant);//mul����
	double rbfthinplate(float r,float constant);//������������
	void rotate(int i,float *de);//����Ҫ����ĵ�i��������Ƭ��ţ���������ھ���de��
	void face(float *a1,float *a2,float *a3,float *res);//ƽ��
	void R(double a,double b,float *n,float *vec,float *res,int i);//�ֱ�����ȡ��������ߡ�ABC������ �� ���µ���ת������A1B1C1.
	void destination(int i,float *de);//��R1 R2������� ����ԭʼ����
	void error(int i);//��Ϊ�������������Ա�����Ϊ����������������Ҫ��������������Ƭ���i
	void node(float * fac1,float *fac2,float *fac3,float *point);//��������ƽ�棬�õ�����ƽ��Ľ���
	float rbf(float d,float e,int *size,float *c,float *coefficient,char rbffuntion);//RBF��ֵ����
	//void crosspoint1(float * a,float *b,float *c);
	//void crosspoint2(float a,float *b,float *c);
//	void count_h(int i);//�����i�������ε�h

	//12.17�¼����㷨
//	void count_TexcoordByHuang(int a,int b,int c,int d,int e);
//	void findDiffrentPoint(int a,int d,int &b,int &c1,int &c2,int e1,int e2);
//	int count_Texcoord(int d,int c1,int c2);
//	int  count_3FaceTexcoord(int d);
	//12.18�¼��� ��ӳ�䵽2d��2�������εı�ƴ������
//	void alterFace2dCoord(int a,int d,int b,int c1,int c2,int e1,int e2);
	//12.27
//	void resetOuterTriangleTex();//������������0-1֮���������������
	//2015.4.6
	//void calTexCor();
	int findFaceIndex(int d,int e1,int e2);
	vector<int> toProcesseTriangleIndex;	//������Ҫ��������������
	//2015.5.14
	//vector<int> pocessedTriangleIndex; //�����Ѵ�������������
	//bool ispocessed(int index);
	//
	int calTexCorTimes;//���������������
	//bool addIndextoProcesseTriangleIndex(int pareIndex,int index);
	//void deleteIndexfromProcesseTriangleIndex(int index);
	//void calTexCorByIndex(int processedindex,int steps);
	//2015.4.15
	float r,g,b;
	int count;
	vector<int> userSelectTriangleIndex;//�û�ָ��չ����ʼ��
	int times;//չ������
	int userSelectingTriangleIndex;
	vector<gl_face *> markConnectedFace(double centerLimitCos,double limitCos,int steps);//����Ϊ userSelectingTriangleIndex����
	double getNorError(float * n1,float * n2);
	int showGroup;
	double _limitCos;//��Ե���߼н�����
	float _centerLimitCos;//���ķ��߼н�����
	int _steps;//��������
	//2015.5.5
	FindTextureElementPosition *_ftep[10];
	//2016.5.3
	int ftepIndex;
	FindTextureElementPosition * get_ftep();
	void set_ftep(FindTextureElementPosition * ftep);
	//2015.5.20
	int texGenTime;
	void reset2dCoord();
	bool logTex;
	bool show_ftep;//��ʾ����Ԫ��
	//2015.5.21 
	float offset[2];//ƫ����
	Point3D * offsetPT;
	//2016.2.9
	float tempTextureElementPos[3];
	float tempTextureElementNormal[3];
	void calVertex2D(float pos[3],int index);//�����index���������ϵ�pos��ͶӰ�������
	double buildTexCoordByIndex(int index, int maxDeep, int maxNum, float radius,int textureIndex);
	bool buildTexCoord(int index, vector<int>&v, int &deep, int maxDeep, int maxNum, float radius);
	double getSelectArea(vector<int>&v);
	//20150906
	DistanceRecord * dr;//����߾�������
	bool isdrawbg;//���������ɫ
	bool istestV;
	//2016.4.19
	ObjLoader * objloader;
	bool loadDataFromObj();

	//
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CTextureProcessSystemDoc();

	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	BOOL OnSaveDocument(LPCTSTR lpszPathName);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
