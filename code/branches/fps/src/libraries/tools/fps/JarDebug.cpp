// JarDebug.cpp: implementation of the CJarDebug class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "JarDebug.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#pragma warning( disable : 4996 ) // stop bitching about unsafe string functions
#pragma warning( disable : 4267 ) // stop bitching about conversion of size_t to int


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJarDebug::CJarDebug()
{
	m_chCR[0]=0x0D ;
	m_chCR[1]=0x0A ;
	m_chCR[2]='\0' ;
	//AfxMessageBox("JarDebug active") ;
}

CJarDebug::~CJarDebug()
{

}

//void CJarDebug::StartTiming(void) { m_Time=timeGetTime() ; } 
//int CJarDebug::GetTiming(void) { return timeGetTime()-m_Time ; }


void CJarDebug::LogInit(void)
{
	m_chLog[0]='\0' ;
}

void CJarDebug::LogAdd(char LETTERS[])
{
	strcat(m_chLog, LETTERS) ;
}

void CJarDebug::LogAdd(char LETTERS[], bool bCR)
{
	strcat(m_chLog, LETTERS) ;
	if(bCR) strcat(m_chLog, m_chCR) ; 
}

void CJarDebug::LogAddCR(char LETTERS[])
{
	strcat(m_chLog, LETTERS) ;
	strcat(m_chLog, m_chCR) ; 
}

void CJarDebug::LogCR(void)
{
	strcat(m_chLog, m_chCR) ;
}

void CJarDebug::LogSave(char FILENAME[])
{
	int fh=0 ;
	LogCR() ;
	LogAddCR("END LOG.") ;
	if( (fh = _open( FILENAME, _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, 
                               _S_IREAD | _S_IWRITE )) != -1 )
	{
		_write( fh, m_chLog, sizeof(m_chLog )) ;
		_close( fh );
	}
}


void CJarDebug::PrintInfo(char LETTERS[], int nXPos, int nYPos)
{
	HDC hdc=GetDC(GetActiveWindow()) ;
	TextOut(hdc, nXPos, nYPos, (LPCWSTR)LETTERS, strlen(LETTERS)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;

}

void CJarDebug::Pixel(int nPosX, int nPosY, COLORREF clrCol)
{
	HDC hdc=GetDC(GetActiveWindow()) ;
	SetPixelV(hdc, nPosX, nPosY, clrCol) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}
void CJarDebug::Pixel(int nPosX, int nPosY, int nCol)
{
	HDC hdc=GetDC(GetActiveWindow()) ;
	int nRed=(nCol&63488)/256 ;
	int nGreen=(nCol&2016)/8 ;
	int nBlue=(nCol&31)*8 ;
	SetPixelV(hdc, nPosX, nPosY, RGB(nRed, nGreen, nBlue)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::BlankArea(int nStartX, int nStartY, int nEndX, int nEndY)
{
	HDC hdc=GetDC(GetActiveWindow()) ;
	RECT rect ;

	rect.left=nStartX ;
	rect.right=nEndX ;
	rect.top=nStartY ;
	rect.bottom=nEndY ;

	FillRect(hdc, &rect, (HBRUSH) ( GetStockObject(BLACK_BRUSH) ));
}


/////////////////////////////////////////////
//
// no number
//
/////////////////////////////////////////////

void CJarDebug::Info(char LETTERS[])
{
	HDC hdc=GetDC(GetActiveWindow()) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)LETTERS, strlen(LETTERS)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[])
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	
	TextOut(hdc, Xpos, Ypos,  (LPCWSTR)LETTERS, strlen(LETTERS)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

/////////////////////////////////////////////
//
// 1 number
//
/////////////////////////////////////////////

void CJarDebug::Info(char LETTERS[], int Int0)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

/////////////////////////////////////////////
//
// 2 numbers
//
/////////////////////////////////////////////

void CJarDebug::Info(char LETTERS[], int Int0, int Int1)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

/////////////////////////////////////////////
//
// 3 numbers
//
/////////////////////////////////////////////

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, int Int2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, int Int2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, int Int2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, int Int2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, float Float2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, float Float2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, float Float2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, float Float2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

/////////////////////////////////////////////
//
// 4 numbers
//
/////////////////////////////////////////////

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, int Int2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Int3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, int Int2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Int3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, int Int2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Int3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, int Int2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Int3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, float Float2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Int3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, float Float2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Int3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, float Float2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Int3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, float Float2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Int3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, int Int2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Float3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, int Int2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Float3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, int Int2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Float3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, int Int2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Float3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, float Float2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Float3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, float Float2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Float3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, float Float2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Float3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, float Float2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Float3) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

/////////////////////////////////////////////
//
// 5 numbers
//
/////////////////////////////////////////////

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, int Int2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Int3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, int Int2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Int3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, int Int2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Int3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, int Int2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Int3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, float Float2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Int3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, float Float2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Int3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, float Float2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Int3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, float Float2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Int3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, int Int2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Float3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, int Int2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Float3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, int Int2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Float3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, int Int2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Float3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, float Float2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Float3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, float Float2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Float3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, float Float2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Float3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, float Float2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Float3, Int4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, int Int2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Int3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, int Int2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Int3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, int Int2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Int3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, int Int2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Int3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, float Float2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Int3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, float Float2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Int3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, float Float2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Int3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, float Float2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Int3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, int Int2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Float3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, int Int2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Float3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, int Int2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Float3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, int Int2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Float3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, int Int1, float Float2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Float3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, int Int1, float Float2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Float3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], int Int0, float Float1, float Float2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Float3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(char LETTERS[], float Float0, float Float1, float Float2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Float3, Float4) ;
	
	TextOut(hdc, 0, 0, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}








































/////////////////////////////////////////////
//
// 1 number
//
/////////////////////////////////////////////

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

/////////////////////////////////////////////
//
// 2 numbers
//
/////////////////////////////////////////////

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

/////////////////////////////////////////////
//
// 3 numbers
//
/////////////////////////////////////////////

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, int Int2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, int Int2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, int Int2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, int Int2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, float Float2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, float Float2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, float Float2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, float Float2)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

/////////////////////////////////////////////
//
// 4 numbers
//
/////////////////////////////////////////////

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, int Int2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Int3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, int Int2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Int3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, int Int2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Int3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, int Int2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Int3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, float Float2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Int3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, float Float2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Int3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, float Float2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Int3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, float Float2, int Int3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Int3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, int Int2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Float3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, int Int2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Float3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, int Int2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Float3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, int Int2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Float3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, float Float2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Float3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, float Float2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Float3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, float Float2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Float3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, float Float2, float Float3)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Float3) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

/////////////////////////////////////////////
//
// 5 numbers
//
/////////////////////////////////////////////

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, int Int2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Int3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, int Int2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Int3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, int Int2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Int3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, int Int2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Int3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, float Float2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Int3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, float Float2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Int3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, float Float2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Int3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, float Float2, int Int3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Int3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, int Int2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Float3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, int Int2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Float3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, int Int2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Float3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, int Int2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Float3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, float Float2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Float3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, float Float2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Float3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, float Float2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Float3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, float Float2, float Float3, int Int4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Float3, Int4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, int Int2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Int3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, int Int2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Int3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, int Int2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Int3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, int Int2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Int3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, float Float2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Int3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, float Float2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Int3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, float Float2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Int3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, float Float2, int Int3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Int3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, int Int2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Int2, Float3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, int Int2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Int2, Float3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, int Int2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Int2, Float3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, int Int2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Int2, Float3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, int Int1, float Float2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Int1, Float2, Float3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1, float Float2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Int1, Float2, Float3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], int Int0, float Float1, float Float2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Int0, Float1, Float2, Float3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

void CJarDebug::Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1, float Float2, float Float3, float Float4)
{
	HDC hdc=GetDC(GetActiveWindow()) ;

	sprintf(m_chMessage, LETTERS, Float0, Float1, Float2, Float3, Float4) ;
	
	TextOut(hdc, Xpos, Ypos, (LPCWSTR)m_chMessage, strlen(m_chMessage)) ;
	ReleaseDC(GetActiveWindow(), hdc) ;
}

























// 1 int
void CJarDebug::MessageInt(int NUM0)
{
	char chMessage[256] ;
	sprintf(chMessage, "%d", NUM0) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 2 int
void CJarDebug::MessageInt(int NUM0, int NUM1)
{
	char chMessage[256] ;
	sprintf(chMessage, "%d  %d", NUM0, NUM1) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 3 int
void CJarDebug::MessageInt(int NUM0, int NUM1, int NUM2)
{
	char chMessage[256] ;
	sprintf(chMessage, "%d  %d  %d", NUM0, NUM1, NUM2) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 4 int
void CJarDebug::MessageInt(int NUM0, int NUM1, int NUM2, int NUM3)
{
	char chMessage[256] ;
	sprintf(chMessage, "%d  %d  %d  %d", NUM0, NUM1, NUM2, NUM3) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 5 int
void CJarDebug::MessageInt(int NUM0, int NUM1, int NUM2, int NUM3, int NUM4)
{
	char chMessage[256] ;
	sprintf(chMessage, "%d  %d  %d  %d  %d", NUM0, NUM1, NUM2, NUM3, NUM4) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 6 int
void CJarDebug::MessageInt(int NUM0, int NUM1, int NUM2, int NUM3, int NUM4, int NUM5)
{
	char chMessage[256] ;
	sprintf(chMessage, "%d  %d  %d  %d  %d  %d", NUM0, NUM1, NUM2, NUM3, NUM4, NUM5) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 7 int
void CJarDebug::MessageInt(int NUM0, int NUM1, int NUM2, int NUM3, int NUM4, int NUM5, int NUM6)
{
	char chMessage[256] ;
	sprintf(chMessage, "%d  %d  %d  %d  %d  %d  %d", NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 1 float
void CJarDebug::MessageFloat(float NUM0)
{
	char chMessage[256] ;
	sprintf(chMessage, "%f", NUM0) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 2 float
void CJarDebug::MessageFloat(float NUM0, float NUM1)
{
	char chMessage[256] ;
	sprintf(chMessage, "%f  %f", NUM0, NUM1) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 3 float
void CJarDebug::MessageFloat(float NUM0, float NUM1, float NUM2)
{
	char chMessage[256] ;
	sprintf(chMessage, "%f  %f  %f", NUM0, NUM1, NUM2) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 4 float
void CJarDebug::MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3)
{
	char chMessage[256] ;
	sprintf(chMessage, "%f  %f  %f  %f", NUM0, NUM1, NUM2, NUM3) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 5 float
void CJarDebug::MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4)
{
	char chMessage[256] ;
	sprintf(chMessage, "%f  %f  %f  %f  %f", NUM0, NUM1, NUM2, NUM3, NUM4) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 6 float
void CJarDebug::MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5)
{
	char chMessage[256] ;
	sprintf(chMessage, "%f  %f  %f  %f  %f  %f", NUM0, NUM1, NUM2, NUM3, NUM4, NUM5) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}
// 7 float
void CJarDebug::MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5, float NUM6)
{
	char chMessage[256] ;
	sprintf(chMessage, "%f  %f  %f  %f  %f  %f  %f", NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}

// 8 float
void CJarDebug::MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5, float NUM6, float NUM7)
{
	char chMessage[256] ;
	sprintf(chMessage, "%f  %f  %f  %f  %f  %f  %f  %f", NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}

// 9 float
void CJarDebug::MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5, float NUM6, float NUM7, float NUM8)
{
	char chMessage[256] ;
	sprintf(chMessage, "%f  %f  %f  %f  %f  %f  %f  %f  %f", NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}

// 10 float
void CJarDebug::MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5, float NUM6, float NUM7, float NUM8, float NUM9)
{
	char chMessage[256] ;
	sprintf(chMessage, "%f  %f  %f  %f  %f  %f  %f  %f  %f  %f", NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}

// 11 float
void CJarDebug::MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5, float NUM6, float NUM7, float NUM8, float NUM9, float NUM10)
{
	char chMessage[256] ;
	sprintf(chMessage, "%f  %f  %f  %f  %f  %f  %f  %f  %f  %f  %f", NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM10) ;
	MessageBoxA(GetActiveWindow(),chMessage,"FATAL ERROR",MB_ICONSTOP|MB_OK);
}

#pragma warning( default : 4996 )
#pragma warning( default : 4267 )