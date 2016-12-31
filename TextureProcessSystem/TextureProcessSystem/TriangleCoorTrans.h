#pragma once
#include "Point3D.h"
#include <Eigen/Dense>  
#include <Eigen/LU>
using namespace Eigen;
using namespace Eigen::internal;
using namespace Eigen::Architecture;
class TriangleCoorTrans
{
public:
	TriangleCoorTrans();
	~TriangleCoorTrans();
	Point3D * m_centerPosXYCoord;
	Point3D * m_trianglePos[3];
	Point3D * m_centerPosUVCoord;
	Point3D * m_centerPosXYZCoord;
	Point3D * m_centerPosABCCoord;

public:
	void init(Point3D ** trianglePos);
	//����������������ǰ�����ȳ�ʼ�������ζ���λ��
	Point3D * convertCoordXY2UV(Point3D * centerPosXYCoord);
	Point3D * convertCoordUV2XY(Point3D * centerPosUVCoord);
	//����������XYZת������������ABC
	Point3D * convertCoordXYZ2ABC(Point3D * centerPosXYZCoord);
	Point3D * convertCoordABC2XYZ(Point3D * centerPosABCCoord);

};

