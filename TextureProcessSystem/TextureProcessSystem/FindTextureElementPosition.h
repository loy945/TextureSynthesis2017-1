#pragma once
#include "TextureProcessSystemDoc.h"
#include "TargetTexture.h"
#include "SampleTexture.h"
#include "Point3D.h"
#include "plyloader.h"
#include "DistanceRecord.h"
class FindTextureElementPosition
{
public:
	FindTextureElementPosition(void);
	~FindTextureElementPosition(void);
	void init(CTextureProcessSystemDoc * pDoc);
	void getDR(DistanceRecord * pDR);
	void addTargetTE(gl_face * face,int textureIndex);
	void getPoint(double dis, double angel, TextureElement * centerTE, vector<Point3D> P, Point3D &res);
	double getPointError(double dis, double angel, TextureElement * centerTE, Point3D P);
	vector<Point3D> addPointToSelectList(vector<gl_face *> F);
	bool buildTargetTextureElement(TextureElement * centerTE, vector<int> &matchF,vector<vector<int> >  &linkPairs);
	void addlinks(TextureElement * centerTE, vector<int> &matchF,vector<vector<int> >  &linkPairs);//������������λ�ö��ѹ̶��Ļ�Ԫ���������ж��α�
	TextureElement * theNearsetTE();
	bool buildByStep();
	void amend();
	bool amendTargetTE(TextureElement * te);//������ǰ��Ԫ��link����
	void detectCross1(TextureElement * te);//���1�ͱ߳�ͻ,��teΪ���ĵ�һȦ��Ԫ����һ�ͱ��н������Ӧ��Ԫֹͣ����
	void detectLapped();
	bool detectRegion(TextureElement * te);
	bool getCoverRange(TextureElement * te, double radius);
	void setN(TextureElement * te);
	void markCoveredFace(TextureElement * te);
	void detectCollapse();
	void selectNearByFace(TextureElement * te, double radius, vector<gl_face *> &faceIndex);
	void collapse(TextureElement * te1, TextureElement * te2);
	void cutEges(TextureElement * te1);
	bool cutEges2(TextureElement * te1);
public:
	SampleTexture * m_sampleTexture;
	TargetTexture * m_targetTexture;
	TextureElement * smapleCenter;
	TextureElement * targetCenter;
public:
	CTextureProcessSystemDoc * pDoc;
	DistanceRecord * m_pDR;
	vector<Point3D> P;
	Point3D firstPoint;
	TextureElement * initElements;
	vector<int> latestAddIn;
	double zBufferCut;
public:
	void draw();
};

