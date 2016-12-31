/* Demonstrates how to load PLY files
* Needs some refactoring.
*
* Model needs to be triangulated
* Use blender
*
* Just the class for loading PLY files.
*
*/

#pragma once

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <string>
#include <vector>
#include "Point3D.h"
#include "Point3fv3.h"
using namespace std;

class gl_point
{
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;

	GLfloat u;
	GLfloat v;
	int pointNum;//������к�
	vector<int> beLongToFaceIndex;
	gl_point(){ x = y = z = 0; pointNum = -1; u = -1; v = -1;}
};
class gl_point2d
{
public:
	GLfloat x;
	GLfloat y;

	int pointNum;
	gl_point2d(){ x = y = 0; pointNum = -1; }
};

class gl_face
{
public:
	int ptnum[3];
	int facenum;//������к�
	float length;//������㵽��Ƭ�ľ��룬�����Ͻ����棻

	float value;//��
	float h;//�¼���Ĳ���h
	bool isSetValue;
	float vectorPosX;//��������
	float vectorPosY;

	float R1[4][4],R2[4][4];//��ת���󱣴�
	float corex,corey,corez;//������Ƭ����λ�ã�Ҳ�ǲ�ֵ�������  
	float pcorex,pcorey;//��ת��������Ƭ����λ��  
	float endx,endy,endz;//��ֵ�����յ�

	//12.11�¼�����Ƭӳ���2άʱ,3�������2ά��������
	int ptnum_2d[3];

	bool textureclick;
	float r,g,b;
	gl_face(){ facenum = -1;vectorPosX=0;vectorPosY=0;value=0;isSetValue=false;
	textureclick=false;r=1;g=1;b=1;
	is2DCordFixed = false; isProcessedTexCor = false; renderTime = 0; isShowColorIn3D = false; disFromCenter = 9999; isCoveredByElement = false; testV = false;
	belongTo = -1; UVatlas_part = 1;
	}
	//3.22 2D����ƽ��ƴ�Ӻ�̶�
	bool is2DCordFixed;
	//12.4.6
	bool isProcessedTexCor;
	//չ�����2D����
	float _2dx[3];
	float _2dy[3];
	//2015.4.14
	//������Ƭ3�����������꣨u,v��
	/*float _2du[3];
	float _2dv[3];*/
	//2015.4.15
	float n[3];//����
	//2015.4.15
	float _show2dx[3];
	float _show2dy[3];
	//2015.4.16
	int renderTime;
	//3dģʽ�Ƿ���ʾ��ɫ
	bool isShowColorIn3D;
	//2015.5.10
	vector<Point3fv3 *> texCoords;//�����������飬���Ա��������������
	Point3fv3 texCoord;//��������
	double disFromCenter; 
	bool isCoveredByElement;
	bool testV;
	//2016.5.17
	int belongTo;
	int UVatlas_part;
public:
	void updateTexCoord();
};

 typedef struct Templet//���������ṹ��
{
    float x;//x����
	float y;//y����
	float z;//z����
	float c;//����������Ӧ��c
	//float s;//����
}Templet;

class Model_PLY
{
public:
	bool Model_PLY::Load(string filename);
	void Model_PLY::writeMesh(char *filename);
	void Model_PLY::writeMeshPLYT(char * filename);
	void Model_PLY::Draw();
	float* Model_PLY::calculateNormal(float *coord1, float *coord2, float *coord3);
	Model_PLY();
	~Model_PLY();

	vector<gl_face> faceArry;
	vector<gl_point> pointArry;
	vector<gl_point2d> point2DArry;

	float* Faces_Triangles;
	float* Faces_Quads;
	float* Vertex_Buffer;
	float* Normals;

	int TotalConnectedTriangles;
	int TotalConnectedQuads;
	int TotalConnectedPoints;
	int TotalFaces;

	int showPart;
};

