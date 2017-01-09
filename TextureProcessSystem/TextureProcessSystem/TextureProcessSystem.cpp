
// TextureProcessSystem.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "TextureProcessSystem.h"
#include "MainFrm.h"

#include "TextureProcessSystemDoc.h"
#include "TextureProcessSystemView.h"
#include "GLBaseView.h"

#include "FindTextureElementPosition.h"
#include "Parameterization.h"
#include "LocalParameterization.h"
#include "Polyhedron.h"
#include <fstream>
#include "TriangleCoorTrans.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextureProcessSystemApp

BEGIN_MESSAGE_MAP(CTextureProcessSystemApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CTextureProcessSystemApp::OnAppAbout)
	ON_COMMAND(ID_BUTTON_SETVALUE_L, &CTextureProcessSystemApp::OnSetValueL)
	ON_COMMAND(ID_BUTTON_SETVALUE_C, &CTextureProcessSystemApp::OnSetValueC)
	ON_COMMAND(ID_BUTTON_SETVALUE_G, &CTextureProcessSystemApp::OnSetValueG)
	ON_COMMAND(ID_BUTTON_SETVALUE_M, &CTextureProcessSystemApp::OnSetValueM)
	ON_COMMAND(ID_BUTTON_SETVALUE_T, &CTextureProcessSystemApp::OnSetValueT)
	ON_COMMAND(ID_BUTTON_CHECK, &CTextureProcessSystemApp::Check)
	ON_COMMAND(ID_BUTTON_MappingToFace, &CTextureProcessSystemApp::MappingToFace)
	ON_COMMAND(ID_BUTTON_SELECTFACE, &CTextureProcessSystemApp::SelectFace)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CTextureProcessSystemApp ����

CTextureProcessSystemApp::CTextureProcessSystemApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO:  ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("TextureProcessSystem.AppID.NoVersion"));

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CTextureProcessSystemApp ����

CTextureProcessSystemApp theApp;


// CTextureProcessSystemApp ��ʼ��

BOOL CTextureProcessSystemApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTextureProcessSystemDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CTextureProcessSystemView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->SetWindowText("������ϵͳ");
	m_pMainWnd->UpdateWindow();


	
	return TRUE;
}

int CTextureProcessSystemApp::ExitInstance()
{
	//TODO:  �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CTextureProcessSystemApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CTextureProcessSystemApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
void CTextureProcessSystemApp::OnSetValue(char rbffuntion)
{
	

	CFrameWnd* pMain=(CFrameWnd*)CWinThread::m_pMainWnd;
	CTextureProcessSystemDoc * pDoc = (CTextureProcessSystemDoc*)pMain->CFrameWnd::GetActiveDocument();

	
	pDoc->calculateValue(rbffuntion);
	CString s1;
	s1.Format("%s ��ֵ",&rbffuntion);
	AfxMessageBox(s1);

}
void CTextureProcessSystemApp::OnSetValueL()
{
	OnSetValue('l');

}void CTextureProcessSystemApp::OnSetValueC()
{
	OnSetValue('c');
}

void CTextureProcessSystemApp::OnSetValueG()
{
	OnSetValue('g');
}
void CTextureProcessSystemApp::OnSetValueM()
{
	OnSetValue('m');
}
void CTextureProcessSystemApp::OnSetValueT()
{
	OnSetValue('t');

}

// CTextureProcessSystemApp ��Ϣ�������
void CTextureProcessSystemApp::SelectFace()
{
	CFrameWnd* pMain=(CFrameWnd*)CWinThread::m_pMainWnd;
	CTextureProcessSystemDoc * pDoc = (CTextureProcessSystemDoc*)pMain->CFrameWnd::GetActiveDocument();
	if (pDoc->selectfaceLsit.size() == 0)
	{
		pDoc->userSelectingTriangleIndex = 3410;
	}
	else
	{
		pDoc->userSelectingTriangleIndex = pDoc->selectfaceLsit[0].facenum;
	}

	//pDoc->userSelectingTriangleIndex = 2641;
	//pDoc->markConnectedFace(0,0 ,1000);
	/*��ѡ
	pDoc->markConnectedFace(0.5,0.9659258262890683 ,10000);
	//cos5=0.9961946980917455
	//cos10=0.984807753012208 
	//cos15=0.9659258262890683
	//cos20=0.9396926207859084
	//cos25=0.9063077870366499
	//cos30=0.8660254037844387
	//cos45=0.7071067811865476
	//cos60=0.5000000000000000
	
	int count=0;
	for(int i=0;i<pDoc->plyLoader.faceArry.size();i++)
	{
		if(pDoc->plyLoader.faceArry.at(i).isShowColorIn3D)
		{
			count++;
		}	
	}
	CString s1;
	s1.Format("Select %d face",count);
	AfxMessageBox(s1);
	*/
	pDoc->selectfaceLsit.clear();
	//��������
	FindTextureElementPosition *_ftep=new FindTextureElementPosition();
	pDoc->set_ftep(_ftep);
	pDoc->ftepIndex++;	
	_ftep->getDR(pDoc->dr);
	_ftep->init(pDoc);
 	pDoc->dr->initDistanceFromCenter(pDoc->userSelectingTriangleIndex);
	CString s1;
	s1.Format("%d ",_ftep->m_targetTexture->tes.size());
	AfxMessageBox(s1);
}
void CTextureProcessSystemApp::Check()
{
	CFrameWnd* pMain=(CFrameWnd*)CWinThread::m_pMainWnd;
	CTextureProcessSystemDoc * pDoc = (CTextureProcessSystemDoc*)pMain->CFrameWnd::GetActiveDocument();
	pDoc->istestV = true;
	POSITION Position = pDoc->GetFirstViewPosition();
	CGLBaseView * view = (CGLBaseView *)pDoc->GetNextView(Position);
	
	view->runtimes = 0;
	view->KillTimer(1);

	for (int i = 0; i < pDoc->plyLoader.faceArry.size(); i++)
	{
		pDoc->plyLoader.faceArry[i].updateTexCoord();
	}


	//pDoc->selectfaceLsit.clear();

	int size=pDoc->plyLoader.faceArry.size();
	pDoc->count=0;
	int times=0;//չ������
	int lastCount=0;
	int tempCenter=0;
	int newCenter=0;
	double minCos=1;	
	//Ϊÿ����Ԫ��ͼ
	ofstream f("triangleIndex.txt");
	ofstream f2("mapping record.txt");
	DWORD TimeStart, TimeEnd, TimeUsed;
	DWORD m, s, ms;
	TimeStart = GetTickCount();
	FindTextureElementPosition * ftep;
	double minStrech = 999, maxStrech = 0;
	for (int w = 0; w < 10; w++)
	{
		ftep = pDoc->_ftep[w];
		if (ftep)
		{
			for (int i = 0; i < ftep->m_targetTexture->tes.size(); i++)
			{
				if (!ftep->m_targetTexture->tes[i]->isShow) continue;
				int textureIndex = ftep->m_targetTexture->tes[i]->textureIndex;
				//�����Ԫ��������Ƭ�����λ��
				Point3D pt1;//��i����Ԫ��λ��
				pt1.x = ftep->m_targetTexture->tes[i]->pos[0];
				pt1.y = ftep->m_targetTexture->tes[i]->pos[1];
				pt1.z = ftep->m_targetTexture->tes[i]->pos[2];
				//������Ƭ���
				int faceNum = ftep->m_targetTexture->tes[i]->face->facenum;
				TriangleCoorTrans tct;
				Point3D * tri[3];
				for (int j = 0; j < 3; j++)
				{
					tri[j] = new Point3D(pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry[faceNum].ptnum[j]].x,
						pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry[faceNum].ptnum[j]].y,
						pDoc->plyLoader.pointArry[pDoc->plyLoader.faceArry[faceNum].ptnum[j]].z);
				}
				tct.init(tri);
				Point3D * temp = tct.convertCoordXYZ2ABC(&pt1);
				pDoc->offsetPT->x = temp->x;
				pDoc->offsetPT->y = temp->y;
				pDoc->offsetPT->z = temp->z;
				for (int j = 0; j < 3; j++)
				{
					pDoc->tempTextureElementPos[j] = ftep->m_targetTexture->tes[i]->pos[j];
					pDoc->tempTextureElementNormal[j] = ftep->m_targetTexture->tes[i]->n[j];
				}
				double strech=pDoc->buildTexCoordByIndex(faceNum, 0, 50, 0.008, textureIndex);
				minStrech = minStrech < strech ? minStrech : strech;
				maxStrech = maxStrech > strech ? maxStrech : strech;
				for (int j = 0; j < 3; j++)
				{
					delete tri[j];
					tri[j] = NULL;
				}
			}
		}
		else
		{
			break;
		}
	}
	CString s1;
	s1.Format("minStrech:  %f, maxStrech: %f", minStrech, maxStrech);
	lastCount = pDoc->count;
	AfxMessageBox(s1);
	f.close();
	TimeEnd = GetTickCount();
	TimeUsed = TimeEnd - TimeStart;
	m = TimeUsed / 60000;
	s = TimeUsed / 1000 - m * 60;
	ms = TimeUsed % 1000;
	f2 << "element nums:" << pDoc->get_ftep()->m_targetTexture->tes.size() << endl;
	f2 << "cost time:" << m << " minutes " << s << " seconds" << ms << " milliseconds" << endl;
	f2.close();
	pDoc->logTex = true;
	pDoc->show_ftep = false; 
	pDoc->isdrawbg = true;
}

void CTextureProcessSystemApp::MappingToFace()
{
	CFrameWnd* pMain = (CFrameWnd*)CWinThread::m_pMainWnd;
	CTextureProcessSystemDoc * pDoc = (CTextureProcessSystemDoc*)pMain->CFrameWnd::GetActiveDocument();
	pDoc->istestV = false;
	//��������
	for (int i = 0; i < pDoc->plyLoader.faceArry.size();i++)
	{
		pDoc->plyLoader.faceArry[i].updateTexCoord();
	}


	int faceNum = pDoc->userSelectingTriangleIndex;
	gl_face * faceSelect = &pDoc->plyLoader.faceArry.at(faceNum);
	Point3D pt[10];
	vector<Point3D> res;
	//��������Ƭ��������
	for (int k = 0; k < 3; k++)
	{
		pt[k].x = pDoc->plyLoader.pointArry.at(faceSelect->ptnum[k]).x;
		pt[k].y = pDoc->plyLoader.pointArry.at(faceSelect->ptnum[k]).y;
		pt[k].z = pDoc->plyLoader.pointArry.at(faceSelect->ptnum[k]).z;
	}
	//��Ƭ������
	pt[3] = (pt[0] + pt[1] + pt[2]) / 3.0;
	//���е�
	pt[4] = (pt[0] + pt[1]) / 2.0;
	pt[5] = (pt[1] + pt[2]) / 2.0;
	pt[6] = (pt[2] + pt[0]) / 2.0;
	//���������ĵ��������ĵ�
	pt[7] = (pt[3] + pt[0]) / 2.0;
	pt[8] = (pt[3] + pt[1]) / 2.0;
	pt[9] = (pt[3] + pt[2]) / 2.0;
	TriangleCoorTrans tct;
	Point3D * tri[3];
	for (int i = 0; i < 3; i++)
	{
		tri[i] = &pt[i];
	}
	tct.init(tri);
	Point3D * temp = tct.convertCoordXYZ2ABC(&pt[3]);
	pDoc->offsetPT->x = temp->x;
	pDoc->offsetPT->y = temp->y;
	pDoc->offsetPT->z = temp->z;
	pDoc->buildTexCoordByIndex(faceNum, 0, 100, 0.008, rand() % 48 + 2);
	

	pDoc->logTex = true;
	pDoc->show_ftep = false;
	pDoc->isdrawbg = true;
}
//CString s1;
//s1.Format("�����:  %d,times: %d", pDoc->count - lastCount, times);
//lastCount = pDoc->count;
//AfxMessageBox(s1);
