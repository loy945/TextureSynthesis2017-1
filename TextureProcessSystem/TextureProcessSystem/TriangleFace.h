#pragma once
#include "plyloader.h"
class TriangleFace :public gl_face
{
public:
	TriangleFace();
	~TriangleFace();
public:
	int faceNearByIndex[3];
	float faceNearByDistance[3];
	int faceNearByNums;
	TriangleFace * tempLinkPara;//ָ����Դ��Ѱ��·����ʱ�����ο�
	float cost;//����Դ������һ�ڵ��ܳ���
	bool isMark;
	pair<float, int> nearestTE;//�����Ԫ�صĲ���߾��뼰elementIndex
};

