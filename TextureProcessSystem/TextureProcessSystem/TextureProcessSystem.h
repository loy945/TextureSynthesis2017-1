
// TextureProcessSystem.h : TextureProcessSystem Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "TextureProcessSystemDoc.h"
// CTextureProcessSystemApp:
// �йش����ʵ�֣������ TextureProcessSystem.cpp
//

class CTextureProcessSystemApp : public CWinApp
{
public:
	CTextureProcessSystemApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	afx_msg void OnSetValue(char rbffuntion);
	afx_msg void OnSetValueL();
	afx_msg void OnSetValueC();
	afx_msg void OnSetValueG();
	afx_msg void OnSetValueM();
	afx_msg void OnSetValueT();
	afx_msg void Check();
	afx_msg void MappingToFace();
	afx_msg void SelectFace();
	DECLARE_MESSAGE_MAP()
};

extern CTextureProcessSystemApp theApp;
