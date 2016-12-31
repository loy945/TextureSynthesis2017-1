
// TextureProcessSystemView.h : CTextureProcessSystemView ��Ľӿ�
//

#pragma once


class CTextureProcessSystemView : public CView
{
protected: // �������л�����
	CTextureProcessSystemView();
	DECLARE_DYNCREATE(CTextureProcessSystemView)

// ����
public:
	CTextureProcessSystemDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CTextureProcessSystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // TextureProcessSystemView.cpp �еĵ��԰汾
inline CTextureProcessSystemDoc* CTextureProcessSystemView::GetDocument() const
   { return reinterpret_cast<CTextureProcessSystemDoc*>(m_pDocument); }
#endif

