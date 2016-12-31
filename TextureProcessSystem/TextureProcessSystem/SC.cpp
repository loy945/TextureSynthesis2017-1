#include "stdafx.h"
#include "SC.h"


SC::SC()
{
}


SC::~SC()
{
}


void SC::Screen(const char * filename, CDC *pDC)
{
	/*CDC *pDC;//��ĻDC
	pDC = CDC::FromHandle(GetDC(NULL));//��ȡ��ǰ������ĻDC*/
	int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//�����ɫģʽ
	//int Width = pDC->GetDeviceCaps(HORZRES);
	//int Height = pDC->GetDeviceCaps(VERTRES);
	LPRECT rect = new RECT();
	pDC->GetWindow()->GetClientRect(rect);
	int Width = abs(rect->right - rect->left);
	int Height = abs(rect->top - rect->bottom);

	CDC memDC;//�ڴ�DC
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
	memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

	//���´��뱣��memDC�е�λͼ���ļ�
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//���λͼ��Ϣ

	FILE *fp = fopen(filename, "w+b");

	BITMAPINFOHEADER bih = { 0 };//λͼ��Ϣͷ
	bih.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//�߶�
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
	bih.biWidth = bmp.bmWidth;//����

	BITMAPFILEHEADER bfh = { 0 };//λͼ�ļ�ͷ
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ����
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//�ļ��ܵĴ�С
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//д��λͼ�ļ�ͷ

	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//д��λͼ��Ϣͷ

	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//�����ڴ汣��λͼ����

	GetDIBits(memDC.m_hDC, (HBITMAP)memBitmap.m_hObject, 0, Height, p,
		(LPBITMAPINFO)&bih, DIB_RGB_COLORS);//��ȡλͼ����

	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//д��λͼ����

	delete[] p;

	fclose(fp);

	memDC.SelectObject(oldmemBitmap);
}