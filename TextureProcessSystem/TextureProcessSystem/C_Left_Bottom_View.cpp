// C_Left_Bottom_View.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TextureProcessSystem.h"
#include "MainFrm.h"
#include "TextureProcessSystemDoc.h"
#include "GLBaseView.h"
#include "C_Left_Bottom_View.h"
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <fstream>
#include "plyloader.h"

#define PI 3.1415926
// C_Left_Bottom_View

IMPLEMENT_DYNCREATE(C_Left_Bottom_View, CGLBaseView)

C_Left_Bottom_View::C_Left_Bottom_View()
{
	showInfo=false;
	Des_vector[0]=0;
	Des_vector[1]=0;
	Des_vector[2]=-1;
		
}

C_Left_Bottom_View::~C_Left_Bottom_View()
{
}

BEGIN_MESSAGE_MAP(C_Left_Bottom_View, CGLBaseView)
END_MESSAGE_MAP()

// C_Left_Bottom_View ���

#ifdef _DEBUG
void C_Left_Bottom_View::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void C_Left_Bottom_View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

#define �� 3.1415926

void face(float *a1, float *a2, float *a3, float *res)//����������ά�㣬��ƽ��Ax+By+Cz+D=0
{
	float vec1[3];
	float vec2[3];
	float vec3[3];
	int i;
	for (i = 0; i < 3; i++)
	{
		vec1[i] = a2[i] - a1[i];
		vec2[i] = a3[i] - a2[i];
	}

	vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	vec3[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	vec3[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];

	res[0] = vec3[0];
	res[1] = vec3[1];
	res[2] = vec3[2];
	res[3] = 0 - a1[0] * res[0] - a1[1] * res[1] - a1[2] * res[2];
}

void R(double a, double b, float *n, float *vec1, float *vec2, float *vec3, float *res1, float *res2, float *res3)//�ֱ�����ȡ��������ߡ�ABC������ �� ���µ���ת������A1B1C1
{
	CvMat* R1 = cvCreateMat(4, 4, CV_32FC1);
	CvMat* R2 = cvCreateMat(4, 4, CV_32FC1);
	CvMat* A = cvCreateMat(4, 1, CV_32FC1);

	/***************����R1*******************/
	CV_MAT_ELEM(*R1, float, 0, 0) = cos(b)*cos(b)*cos(a) + sin(b)*sin(b);
	CV_MAT_ELEM(*R1, float, 0, 1) = cos(b)*sin(a);
	CV_MAT_ELEM(*R1, float, 0, 2) = cos(b)*cos(a)*sin(b) - sin(b)*cos(b);
	CV_MAT_ELEM(*R1, float, 0, 3) = 0;

	CV_MAT_ELEM(*R1, float, 1, 0) = -sin(a)*cos(b);
	CV_MAT_ELEM(*R1, float, 1, 1) = cos(a);
	CV_MAT_ELEM(*R1, float, 1, 2) = -sin(a)*sin(b);
	CV_MAT_ELEM(*R1, float, 1, 3) = 0;

	CV_MAT_ELEM(*R1, float, 2, 0) = sin(b)*cos(b)*(cos(a) - 1);
	CV_MAT_ELEM(*R1, float, 2, 1) = sin(b)*sin(a);
	CV_MAT_ELEM(*R1, float, 2, 2) = sin(b)*sin(b)*cos(a) + cos(b)*cos(b);
	CV_MAT_ELEM(*R1, float, 2, 3) = 0;

	CV_MAT_ELEM(*R1, float, 3, 0) = (vec1[0] * (1 - cos(b)*cos(b)*cos(a) - sin(b)*sin(b))) + (vec1[1] * sin(a)*cos(b)) + (vec1[2] * sin(b)*cos(b)*(1 - cos(a)));
	CV_MAT_ELEM(*R1, float, 3, 1) = (-vec1[0] * cos(b)*sin(a)) + (vec1[1] * (1 - cos(a))) - (vec1[2] * sin(b)*sin(a));
	CV_MAT_ELEM(*R1, float, 3, 2) = (vec1[0] * sin(b)*cos(b)*(1 - cos(a))) + (vec1[1] * sin(b)*sin(a)) + (vec1[2] * sin(b)*sin(b)*(1 - cos(a)));
	CV_MAT_ELEM(*R1, float, 3, 3) = 1;

	/***************��нǦ�*****************/
	CV_MAT_ELEM(*A, float, 0, 0) = n[0];
	CV_MAT_ELEM(*A, float, 1, 0) = n[1];
	CV_MAT_ELEM(*A, float, 2, 0) = n[2];
	CV_MAT_ELEM(*A, float, 3, 0) = 1;

	/*for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	{
		printf("%f ", CV_MAT_ELEM(*R1, float, i, j));
		if (j == 3)
			printf("\n");
	}
	printf("\n");
	for (int i = 0; i < 4; i++)
	for (int j = 0; j < 1; j++)
	{
		printf("%f ", CV_MAT_ELEM(*A, float, i, j));
		if (j == 0)
			printf("\n");
	}*/


	cvMatMul(R1, A, A);//��ת����*������=�·���������ֵ��A����

	/*printf("\n");
	for (int i = 0; i < 4; i++)
	for (int j = 0; j<1; j++)
	{
		printf("%f ", CV_MAT_ELEM(*A, float, i, j));
		if (j == 0)
			printf("\n");
	}*/

	float m = (CV_MAT_ELEM(*A, float, 2, 0)) / (sqrt(CV_MAT_ELEM(*A, float, 0, 0)*CV_MAT_ELEM(*A, float, 0, 0) + CV_MAT_ELEM(*A, float, 1, 0)*CV_MAT_ELEM(*A, float, 1, 0) + CV_MAT_ELEM(*A, float, 2, 0)*CV_MAT_ELEM(*A, float, 2, 0)));
	double �� = acos(m);
	if (��>�� / 2)
		�� = �� - ��;
	//��=-��;//ȡ��
	/***************����R2*******************/
	CV_MAT_ELEM(*R2, float, 0, 0) = cos(��);
	CV_MAT_ELEM(*R2, float, 0, 1) = 0;
	CV_MAT_ELEM(*R2, float, 0, 2) = sin(��);
	CV_MAT_ELEM(*R2, float, 0, 3) = 0;

	CV_MAT_ELEM(*R2, float, 1, 0) = 0;
	CV_MAT_ELEM(*R2, float, 1, 1) = 1;
	CV_MAT_ELEM(*R2, float, 1, 2) = 0;
	CV_MAT_ELEM(*R2, float, 1, 3) = 0;

	CV_MAT_ELEM(*R2, float, 2, 0) = -sin(��);
	CV_MAT_ELEM(*R2, float, 2, 1) = 0;
	CV_MAT_ELEM(*R2, float, 2, 2) = cos(��);
	CV_MAT_ELEM(*R2, float, 2, 3) = 0;

	CV_MAT_ELEM(*R2, float, 3, 0) = vec1[0] * (1 - cos(��)) + vec1[2] * sin(��);
	CV_MAT_ELEM(*R2, float, 3, 1) = 0;
	CV_MAT_ELEM(*R2, float, 3, 2) = -vec1[0] * sin(��) + vec1[2] * (1 - cos(��));
	CV_MAT_ELEM(*R2, float, 3, 3) = 1;

	//cvMatMul(R2,R1,R1);

	/***************����A��*******************/
	CV_MAT_ELEM(*A, float, 0, 0) = vec1[0];
	CV_MAT_ELEM(*A, float, 1, 0) = vec1[1];
	CV_MAT_ELEM(*A, float, 2, 0) = vec1[2];
	CV_MAT_ELEM(*A, float, 3, 0) = 1;

	cvMatMul(R1, A, A);
	cvMatMul(R2, A, A);

	res1[0] = CV_MAT_ELEM(*A, float, 0, 0);
	res1[1] = CV_MAT_ELEM(*A, float, 1, 0);
	res1[2] = CV_MAT_ELEM(*A, float, 2, 0);

	for (int i = 0; i < 3; i++)
	{
		if (abs(res1[i]) < 1e-005)
			res1[i] = 0;
	}

	/***************����B��*******************/
	CV_MAT_ELEM(*A, float, 0, 0) = vec2[0];
	CV_MAT_ELEM(*A, float, 1, 0) = vec2[1];
	CV_MAT_ELEM(*A, float, 2, 0) = vec2[2];
	CV_MAT_ELEM(*A, float, 3, 0) = 1;

	cvMatMul(R1, A, A);
	cvMatMul(R2, A, A);

	res2[0] = CV_MAT_ELEM(*A, float, 0, 0);
	res2[1] = CV_MAT_ELEM(*A, float, 1, 0);
	res2[2] = CV_MAT_ELEM(*A, float, 2, 0);

	for (int i = 0; i < 3; i++)
	{
		if (abs(res2[i]) < 1e-005)
			res2[i] = 0;
	}

	/***************����C��*******************/
	CV_MAT_ELEM(*A, float, 0, 0) = vec3[0];
	CV_MAT_ELEM(*A, float, 1, 0) = vec3[1];
	CV_MAT_ELEM(*A, float, 2, 0) = vec3[2];
	CV_MAT_ELEM(*A, float, 3, 0) = 1;

	cvMatMul(R1, A, A);
	cvMatMul(R2, A, A);

	res3[0] = CV_MAT_ELEM(*A, float, 0, 0);
	res3[1] = CV_MAT_ELEM(*A, float, 1, 0);
	res3[2] = CV_MAT_ELEM(*A, float, 2, 0);

	for (int i = 0; i < 3; i++)
	{
		if (abs(res3[i]) < 1e-005)
			res3[i] = 0;
	}


}


void trans3to2(float *Src_A, float *Src_B, float *Src_C, float *Des_A, float *Des_B, float *Des_C )
{
	/***************������n****************/
	float a[3] = { Src_A[0] - Src_B[0], Src_A[1] - Src_B[1], Src_A[2] - Src_B[2] };
	float b[3] = { Src_A[0] - Src_C[0], Src_A[1] - Src_C[1], Src_A[2] - Src_C[2] };
	float n[3];//����
	n[0] = a[1] * b[2] - a[2] * b[1];
	n[1] = a[2] * b[0] - a[0] * b[2];
	n[2] = a[0] * b[1] - a[1] * b[0];

	/***************��нǦ�*****************/
	float k[3] = { n[0], 0, n[2] };
	double m = (n[0] * k[0] + n[1] * k[1] + n[2] * k[2]) / (sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2])*sqrt(k[0] * k[0] + k[1] * k[1] + k[2] * k[2]));
	if (abs(m > 0.999))
		m = 1;
	double �� = acos(m);//����ȣ���λΪ����
	if (�� > �� / 2)
		�� = �� - ��;

	/***************��нǦ�*****************/
	float O[3] = { 0, Src_A[1], 0 };
	float O1[3] = { 0, Src_A[1], Src_A[2] + 8 };
	float res[4];
	float fac[4];
	float D[3];
	float A1[3], B1[3], C1[3];
	face(Src_A, O, O1, res);
	face(Src_A, Src_B, Src_C, fac);
	D[0] = Src_A[0] + 5;
	D[1] = (-res[3] / res[1]);
	D[2] = (-fac[3] - D[0] * fac[0] - D[1] * fac[1]);

	float vec[3] = { D[0] - Src_A[0], D[1] - Src_A[1], D[2] - Src_A[2] };
	m = (vec[2]) / (sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]));
	double �� = acos(m);//���������λΪ����
	if (�� > �� / 2)
		�� = �� - ��;

	//��=��-��;

	/****************R�������******************************/
	R(��, ��, n, Src_A, Src_B, Src_C, A1, B1, C1);

	Des_A[0] = A1[0];
	Des_A[1] = A1[1];
	Des_A[2] = A1[2];
	Des_B[0] = B1[0];
	Des_B[1] = B1[1];
	Des_B[2] = B1[2];
	Des_C[0] = C1[0];
	Des_C[1] = C1[1];
	Des_C[2] = C1[2];




}

void C_Left_Bottom_View::DrawScene()
{
	if (m_pDoc->selectfaceLsit.size() > 0)
	{
		gl_face selectface = m_pDoc->selectfaceLsit[m_pDoc->selectfaceLsit.size() - 1];//gl_face�Ǳ仯����ʾ��ƽ���ϵ�������
		index=selectface.facenum;
		gl_face * dataface=&(m_pDoc->plyLoader.faceArry.at(index));//ģ�������ж�Ӧ��������

		
		Src_A[0] = m_pDoc->plyLoader.pointArry[selectface.ptnum[0]].x;
		Src_B[0] = m_pDoc->plyLoader.pointArry[selectface.ptnum[1]].x;
		Src_C[0] = m_pDoc->plyLoader.pointArry[selectface.ptnum[2]].x;

		Src_A[1] = m_pDoc->plyLoader.pointArry[selectface.ptnum[0]].y;
		Src_B[1] = m_pDoc->plyLoader.pointArry[selectface.ptnum[1]].y;
		Src_C[1] = m_pDoc->plyLoader.pointArry[selectface.ptnum[2]].y;

		Src_A[2] = m_pDoc->plyLoader.pointArry[selectface.ptnum[0]].z;
		Src_B[2] = m_pDoc->plyLoader.pointArry[selectface.ptnum[1]].z;
		Src_C[2] = m_pDoc->plyLoader.pointArry[selectface.ptnum[2]].z;

		trans3to2(Src_A, Src_B, Src_C, Des_A, Des_B, Des_C);
		//CString s1;
		//s1.Format("%f %f %f\n%f %f %f\n%f %f %f\n\n\n%f %f %f\n%f %f %f\n%f %f %f",Src_A[0], Src_A[1], Src_A[2],Src_B[0], Src_B[1], Src_B[2],Src_C[0], Src_C[1], Src_C[2],Des_A[0], Des_A[1], Des_A[2],Des_B[0], Des_B[1], Des_B[2],Des_C[0], Des_C[1], Des_C[2]);
		float src_triangle=((Src_A[0]*Src_B[1]-Src_B[0]*Src_A[1])+(Src_B[0]*Des_C[1]-Src_C[0]*Src_B[1])+(Src_C[0]*Src_A[1]-Src_A[0]*Src_C[1]))/2;
		float des_triangle=((Des_A[0]*Des_B[1]-Des_B[0]*Des_A[1])+(Des_B[0]*Des_C[1]-Des_C[0]*Des_B[1])+(Des_C[0]*Des_A[1]-Des_A[0]*Des_C[1]))/2;
	/*if(!showInfo)
	{
				AfxMessageBox(s1);

	showInfo=true;
	}*/
		////��������ƽ�Ƶ�Z=-1ƽ����
		Des_A[2]=-1;
		Des_B[2]=-1;
		Des_C[2]=-1;

		float scaleFactor;
		////�����������
		float s_triangle=((Des_A[0]*Des_B[1]-Des_B[0]*Des_A[1])+(Des_B[0]*Des_C[1]-Des_C[0]*Des_B[1])+(Des_C[0]*Des_A[1]-Des_A[0]*Des_C[1]))/2;
		if(s_triangle<0)s_triangle=-s_triangle;
		if(s_triangle==0) s_triangle=1;
		//����Ŵ�ʽ
		scaleFactor=0.1/(sqrtf(s_triangle))*5;
		float tempCore[3];//����
		for(int i=0;i<3;i++)
		{
			tempCore[i]=(Des_A[i]+Des_B[i]+Des_C[i])/3;
		}
		//�����㵽���ľ����ƽ����
		float distance_triangleA=sqrtf(pow((Des_A[0]-tempCore[0]),2)+pow((Des_A[1]-tempCore[1]),2));
		float distance_triangleB=sqrtf(pow((Des_B[0]-tempCore[0]),2)+pow((Des_B[1]-tempCore[1]),2));
		float distance_triangleC=sqrtf(pow((Des_C[0]-tempCore[0]),2)+pow((Des_C[1]-tempCore[1]),2));

		scaleFactor=1/(distance_triangleA+distance_triangleB+distance_triangleC);

		scaleFactor=1/(distance_triangleA+distance_triangleB+distance_triangleC)+0.1/(sqrtf(s_triangle));
		Des_A[0]*=scaleFactor;
		Des_A[1]*=scaleFactor;
		Des_B[0]*=scaleFactor;
		Des_B[1]*=scaleFactor;
		Des_C[0]*=scaleFactor;
		Des_C[1]*=scaleFactor;
		
		for(int i=0;i<3;i++)
		{
			core[i]=(Des_A[i]+Des_B[i]+Des_C[i])/3;
		}

		for(int i=0;i<2;i++)
		{
			Des_A[i]-=core[i];
			Des_B[i]-=core[i];
			Des_C[i]-=core[i];
		}		

		//��������
		if(!m_pDoc->isChooseFace)
		{
			if(dataface->isSetValue)
			{
				//Des_vector[0]=selectface.vectorPosX;
				//Des_vector[1]=selectface.vectorPosY;
				value=dataface->value;
	
				/*Des_vector[0]=cos(value/180*PI);
				Des_vector[1]=sin(value/180*PI);*/
				Des_vector[0]=cos(value);
				Des_vector[1]=sin(value);
				/*if(value<0) Des_vector[1]=-Des_vector[1];*/
				
			}else
			{
				Des_vector[0]=1;
				Des_vector[1]=0;
				value=0;
			}
			
			Des_vector[2]=-1;

			m_pDoc->isChooseFace=true;
		}
		glColor3f(1.0, 0, 0);
		glBegin(GL_LINES);
		glVertex3f(0,0,-0.5);//0,0,-1������ƽ�Ƶ����ĵ�������
		glVertex3f(Des_vector[0],Des_vector[1],-0.5);
		glEnd();

		//����������
		glPushMatrix();

		glScalef(scale_X*1.0f, scale_Y*1.0f, scale_Z*1.0f);
		glRotatef(rotate_X, 1.0f, 0.0f, 0.0f);
		glRotatef(rotate_Y, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0, 0.0, 0.0);
		glColor3f(0, 1.0, 0);

		glPolygonMode(GL_BACK, GL_FILL);//���ģʽ
		glBegin(GL_TRIANGLES);
		glVertex3f(Des_A[0], Des_A[1], Des_A[2]);
		glVertex3f(Des_B[0], Des_B[1], Des_B[2]);
		glVertex3f(Des_C[0], Des_C[1], Des_C[2]);
		glEnd();
		glPopMatrix();
		glFinish();

	}
}

// C_Left_Bottom_View ��Ϣ�������
void C_Left_Bottom_View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	mousepoint = point;
	SetCapture();
	
	CView::OnLButtonDown(nFlags, point);

}


void C_Left_Bottom_View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	mousepoint = CPoint(0, 0);
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}
void C_Left_Bottom_View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (GetCapture() == this)
	{
		//Increment the object rotation angles
		//rotate_X += float((point.y - mousepoint.y) / 3.6);
		//rotate_Y += float((point.x - mousepoint.x) / 3.6);
		//Redraw the view

		//���ȵ�λ��
		Des_vector[0]+=(point.x - mousepoint.x)*0.01;
		Des_vector[1]-=(point.y - mousepoint.y)*0.01;
		float le=pow(Des_vector[0],2)+pow(Des_vector[1],2);

		if(le>0)
		{
			Des_vector[0]/=sqrtf(le)*2;
			Des_vector[1]/=sqrtf(le)*2;
		}
		//��������X������н�
		float cosValue=Des_vector[0]/(sqrtf(pow(Des_vector[0],2)+pow(Des_vector[1],2)));
		value=acos(cosValue);
		if(Des_vector[1]<0) value=-value;
	





		Invalidate(FALSE);
		//Set the mouse point
		mousepoint = point;
	};
	CView::OnMouseMove(nFlags, point);
}

void C_Left_Bottom_View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	gl_face * selectface = &(m_pDoc->plyLoader.faceArry[index]);//ѡ���������

	GLfloat  winX, winY;
	CRect re;
	GetWindowRect(&re);
	int screenHeight = re.Height(), screenWidth = re.Width();//��Ļ��͸�
	CString value_Info;
	value_Info.Format("index:%d value:%f",index,value);
	AfxMessageBox(value_Info);
	//2015.4.16
	m_pDoc->userSelectTriangleIndex.push_back(index);
	//���浽�ļ�
	

	ofstream fout;
	fout.open("c:\\cow.txt",ios::app);
	fout<<index<<" "<<value<<"\n";
	fout<<flush;
	fout.close();
	selectface->vectorPosX=Des_vector[0];
	selectface->vectorPosY=Des_vector[1];
	selectface->value=value;
	selectface->isSetValue=true;
	//�任Ҫ��ͼ�������˳��һ������������ת�����������

	//winX = GLfloat(point.x);
	//winY = GLfloat(screenHeight - point.y);

	//gluUnProject((GLdouble)winX, (GLdouble)winY, 0.0, modelview, projection, viewport, &nearX, &nearY, &nearZ);
	//gluUnProject((GLdouble)winX, (GLdouble)winY, 1.0, modelview, projection, viewport, &farX, &farY, &farZ);

	//g_color = 0.0;

	//selectedFace.clear();

	//if (IntersectTriangle(m_pDoc->plyLoader.faceArry, m_pDoc->plyLoader.pointArry))
	//{
	//	g_color = 1.0;

	//	Invalidate(FALSE);
	//}
	
	//m_pDoc->findFaceByTwoPoint(selectface->pt1num,selectface->pt2num,selectface->facenum)
	//���ڽ�������
	/*
	vector<int>  res;
	res=m_pDoc->findFaceByTwoPointNewWay(selectface->pt1num,selectface->pt2num,selectface->facenum);
	if(res.size()==0)
	{
		CString s1;
		s1.Format("�Ҳ��������������ΰ�����������%d �ĵ�1���͵�2����",selectface->facenum);
		AfxMessageBox(s1);

	}
	while(res.size()>0)
	{
	CString s1;
	s1.Format("������%d �ĵ�1���͵�2���㣬Ҳ��������%d ����",selectface->facenum,res.back());
	res.pop_back();
	AfxMessageBox(s1);
	}*/
	CView::OnRButtonDown(nFlags, point);

}
