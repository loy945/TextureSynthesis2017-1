#pragma once
#include "GLBaseView.h"

// C_Left_Bottom_View ��ͼ

class C_Left_Bottom_View : public CGLBaseView
{
	DECLARE_DYNCREATE(C_Left_Bottom_View)

protected:
	C_Left_Bottom_View();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~C_Left_Bottom_View();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void DrawScene();

	float Src_A[3];
	float Src_B[3];
	float Src_C[3];

	float Des_A[3];
	float Des_B[3];
	float Des_C[3];
	bool showInfo;
	float core[3];//���� 
	float Des_vector[3];//ƽ������,Des_vector[2]=-1;
	float value;//��
	int index;//�����ζ����
	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg virtual void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){return false;}

};


