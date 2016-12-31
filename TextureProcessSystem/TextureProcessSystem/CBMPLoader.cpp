//======================================================================
/**
*  @file      CBMPLoader.cpp
*
*  ��Ŀ������ ����ӳ��
*  �ļ�����:  ����λͼ�� 
*  ����ƽ̨�� Windows98/2000/NT/XP
*  
*  ���ߣ�     WWBOSS
*  �����ʼ�:  wwboss123@gmail.com
*  �������ڣ� 2006-11-23	
*  �޸����ڣ� 2007-07-10
*	
*/																			
//======================================================================
#include "stdafx.h"
#include"CBMPLoader.h"              /**< ����ͷ�ļ� */

/** ���캯�� */
CBMPLoader::CBMPLoader()
{
   /** ��ʼ����ԱֵΪ0 */
	image = 0;
	imageWidth = 0;
	imageHeight = 0;
}

/** �������� */
CBMPLoader::~CBMPLoader()
{
   FreeImage(); /**< �ͷ�ͼ������ռ�ݵ��ڴ� */
}

/** װ��һ��λͼ�ļ� */
bool CBMPLoader::LoadBitmapTexture(const char *file)
{
	FILE *pFile = 0; /**< �ļ�ָ�� */
	
	/** ����λͼ�ļ���Ϣ��λͼ�ļ�ͷ�ṹ */
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
  
	unsigned char textureColors = 0;/**< ���ڽ�ͼ����ɫ��BGR�任��RGB */

   /** ���ļ�,�������� */
	pFile = fopen(file, "rb");
		if(pFile == 0) return false;

	/** ����λͼ�ļ�ͷ��Ϣ */ 
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);
	
	/** �����ļ��Ƿ�Ϊλͼ�ļ� */
	if(header.bfType != BITMAP_ID)
	   {
		   fclose(pFile);             /**< ������λͼ�ļ�,��ر��ļ������� */
		   return false;
	   }

	/** ����λͼ�ļ���Ϣ */
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

	/** ����ͼ��Ŀ�Ⱥ͸߶� */
	imageWidth = bitmapInfoHeader.biWidth;
    imageHeight = bitmapInfoHeader.biHeight;

    /** ȷ����ȡ���ݵĴ�С */
   if(bitmapInfoHeader.biSizeImage == 0)
      bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth *
      bitmapInfoHeader.biHeight * 3;

	/** ��ָ���Ƶ����ݿ�ʼλ�� */
	fseek(pFile, header.bfOffBits, SEEK_SET);

	/** �����ڴ� */
	image = new unsigned char[bitmapInfoHeader.biSizeImage];
	imageA = new unsigned char[bitmapInfoHeader.biWidth *bitmapInfoHeader.biHeight * 4];
	/** ����ڴ�����Ƿ�ɹ� */
	if(!image)                        /**< �������ڴ�ʧ���򷵻� */
	   {
		   delete[] image;
		   fclose(pFile);
		   return false;
	   }

	/** ��ȡͼ������ */
	fread(image, 1, bitmapInfoHeader.biSizeImage, pFile);

	/** ��ͼ����ɫ���ݸ�ʽ���н���,��BGRת��ΪRGB */
	int indexA = 0;
	for (int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index += 3 )
	   {
			indexA += 4;
		   textureColors = image[index];
		   image[index] = image[index + 2];
		   image[index + 2] = textureColors;

		   imageA[indexA] = image[index];
		   imageA[indexA + 1] = image[index + 1];
		   imageA[indexA + 2] = image[index + 2];
		   if ((image[index] == 0) && (image[index+1] == 255) && (image[index+2] == 0))
		   {
			   imageA[indexA + 3] = 0;
		   }
		   else
		   {
			   imageA[indexA + 3] = 1;
		   }
		   

	   }
  
	fclose(pFile);       /**< �ر��ļ� */
	return true;         /**< �ɹ����� */
}

/** ����λͼ�ļ������������� */
bool CBMPLoader::Load(const char* fileName)
{
	if(!this->LoadBitmapTexture(fileName))
	{
		MessageBox(NULL,"����λͼ�ļ�ʧ��!","����",MB_OK);
		exit(0);
	}
	/** ��������������� */
	glGenTextures(1, &ID);
   
    /** ����������� */
    glBindTexture(GL_TEXTURE_2D, ID);
	

	/** �����˲� */
	float color[4] = { 1.0f, 1.0f,1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glGenerateMipmap(GL_TEXTURE_2D);
	/** �������� */
 /*  	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, imageWidth,
	                  imageHeight, GL_RGBA, GL_UNSIGNED_BYTE,
	                  imageA);*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
   return true;
}

/** �ͷ��ڴ� */
void CBMPLoader::FreeImage()
{
   /** �ͷŷ�����ڴ� */
   if(image)
      {
         delete[] image;
         image = 0;
      }
   if (imageA)
   {
	   delete[] imageA;
	   imageA = 0;
   }
}
