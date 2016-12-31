
// TextureProcessSystemView.cpp : CTextureProcessSystemView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TextureProcessSystem.h"
#endif

#include "TextureProcessSystemDoc.h"
#include "TextureProcessSystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextureProcessSystemView

IMPLEMENT_DYNCREATE(CTextureProcessSystemView, CView)

BEGIN_MESSAGE_MAP(CTextureProcessSystemView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTextureProcessSystemView ����/����

CTextureProcessSystemView::CTextureProcessSystemView()
{
	// TODO:  �ڴ˴���ӹ������

}

CTextureProcessSystemView::~CTextureProcessSystemView()
{
}

BOOL CTextureProcessSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CTextureProcessSystemView ����

void CTextureProcessSystemView::OnDraw(CDC* /*pDC*/)
{
	CTextureProcessSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CTextureProcessSystemView ��ӡ

BOOL CTextureProcessSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CTextureProcessSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CTextureProcessSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CTextureProcessSystemView ���

#ifdef _DEBUG
void CTextureProcessSystemView::AssertValid() const
{
	CView::AssertValid();
}

void CTextureProcessSystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTextureProcessSystemDoc* CTextureProcessSystemView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextureProcessSystemDoc)));
	return (CTextureProcessSystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CTextureProcessSystemView ��Ϣ�������
