#include "StdAfx.h"
#include "MyParameterization.h"
#include <Eigen/Dense>  
#include <Eigen/LU>
#include <Eigen/Geometry>
using namespace Eigen;
using namespace Eigen::internal;
using namespace Eigen::Architecture;

MyParameterization::MyParameterization()
{
}


MyParameterization::~MyParameterization()
{
}
void MyParameterization::init(Model_PLY * ply, vector<int> faceIndexs)
{
	//������ƽ��Ϊ��׼
	int centerIndex = faceIndexs[0];
	float center[3];
	center[0] = ply->faceArry[faceIndexs[centerIndex]].corex;
	center[1] = ply->faceArry[faceIndexs[centerIndex]].corey;
	center[2] = ply->faceArry[faceIndexs[centerIndex]].corez;
	int size = faceIndexs.size();
	float res[3];
	
	//������������ͶӰ����ƽ����
	for (int j = 0; j < faceIndexs.size(); j++)
	{
		float pos[3];
		pos[0] = ply->faceArry[faceIndexs[j]].corex;
		pos[1] = ply->faceArry[faceIndexs[j]].corey;
		pos[2] = ply->faceArry[faceIndexs[j]].corez;
		projection(ply->faceArry[centerIndex].n, center, pos,res);

		MatrixXd m(4, 4);
		m(0, 0) = 3;
		m(0, 1) = -1;
		m(0, 2) = 3;
		m(0, 3) = -1;

		m(0, 0) = 3;
		m(0, 1) = -1;
		m(0, 2) = 3;
		m(0, 3) = -1;

		m(0, 0) = 3;
		m(0, 1) = -1;
		m(0, 2) = 3;
		m(0, 3) = -1;

		m(0, 0) = 3;
		m(0, 1) = -1;
		m(0, 2) = 3;
		m(0, 3) = -1;
	}

}

void MyParameterization::projection(float *n, float * position, float * planeCenterPos, float res[3])
{
	
	Vector3f nor, a;//nor��ƽ�淨������a��ҪͶӰ����ƽ���ϵ�����
	for (int i = 0; i < 3; i++)
	{
		nor[i] = n[i];
		a[i] = position[i] - planeCenterPos[i];
	}
	//��������λ��
	nor = nor.normalized();
	//��aͶӰ���������ϻ������b
	Vector3f b;
	b = (a.dot(nor) / nor.norm())*a;
	Vector3f c = a - b;
	for (int i = 0; i < 3; i++)
	{
		res[i] = c[i] + planeCenterPos[i];
	}

}