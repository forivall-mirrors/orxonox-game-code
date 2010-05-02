// JarDebug.h: interface for the CJarDebug class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JARDEBUG_H__69B857B3_878C_470A_B99E_4AA56D6B9508__INCLUDED_)
#define AFX_JARDEBUG_H__69B857B3_878C_470A_B99E_4AA56D6B9508__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h> 

#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

//#include <mmsystem.h> // for the timer functions

#define JLOGSIZE 5120000 // log is 500k 

class CJarDebug  
{
public:
	CJarDebug();
	virtual ~CJarDebug();

	//DWORD m_Time ;
	//void StartTiming(void) ;
	//int GetTiming(void) ;

	void LogInit() ;
	void LogAdd(char LETTERS[]) ;
	void LogAdd(char LETTERS[], bool bCR) ;
	void LogAddCR(char LETTERS[]) ;
	void LogCR() ;
	void LogSave(char FILENAME[]) ;

	void Pixel(int nPosX, int nPosY, COLORREF clrCol) ;
	void Pixel(int nPosX, int nPosY, int nCol) ;
	void BlankArea(int nStartX, int nStartY, int nEndX, int nEndY) ;

	void MessageInt(int NUM0) ;
	void MessageInt(int NUM0, int NUM1) ;
	void MessageInt(int NUM0, int NUM1, int NUM2) ;
	void MessageInt(int NUM0, int NUM1, int NUM2, int NUM3) ;
	void MessageInt(int NUM0, int NUM1, int NUM2, int NUM3, int NUM4) ;
	void MessageInt(int NUM0, int NUM1, int NUM2, int NUM3, int NUM4, int NUM5) ;
	void MessageInt(int NUM0, int NUM1, int NUM2, int NUM3, int NUM4, int NUM5, int NUM6) ;
	void MessageFloat(float NUM0) ;
	void MessageFloat(float NUM0, float NUM1) ;
	void MessageFloat(float NUM0, float NUM1, float NUM2) ;
	void MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3) ;
	void MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4) ;
	void MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5) ;
	void MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5, float NUM6) ;
	void MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5, float NUM6, float NUM7) ;
	void MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5, float NUM6, float NUM7, float NUM8) ;
	void MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5, float NUM6, float NUM7, float NUM8, float NUM9) ;
	void MessageFloat(float NUM0, float NUM1, float NUM2, float NUM3, float NUM4, float NUM5, float NUM6, float NUM7, float NUM8, float NUM9, float NUM10) ;

	void PrintInfo(char LETTERS[], int nXPos, int nYPos) ;

	char m_chMessage[1024] ;
	
	void Info(char LETTERS[]) ;
	void Info(int Xpos, int Ypos, char LETTERS[]) ;

	void Info(char LETTERS[], int Int0) ;
	void Info(char LETTERS[], float Float0) ;

	void Info(char LETTERS[], int Int0,			int Int1) ;
	void Info(char LETTERS[], float Float0, int Int1) ;
	void Info(char LETTERS[], int Int0,			float Float1) ;
	void Info(char LETTERS[], float Float0, float Float1) ;

	void Info(char LETTERS[], int Int0,			int Int1,			int Int2) ;
	void Info(char LETTERS[], float Float0, int Int1,			int Int2) ;
	void Info(char LETTERS[], int Int0,			float Float1,	int Int2) ;
	void Info(char LETTERS[], float Float0, float Float1,	int Int2) ;
	void Info(char LETTERS[], int Int0,			int Int1,			float Float2) ;
	void Info(char LETTERS[], float Float0, int Int1,			float Float2) ;
	void Info(char LETTERS[], int Int0,			float Float1,	float Float2) ;
	void Info(char LETTERS[], float Float0, float Float1,	float Float2) ;

	void Info(char LETTERS[], int Int0,			int Int1,			int Int2,			int Int3) ;
	void Info(char LETTERS[], float Float0, int Int1,			int Int2,			int Int3) ;
	void Info(char LETTERS[], int Int0,			float Float1,	int Int2,			int Int3) ;
	void Info(char LETTERS[], float Float0, float Float1,	int Int2,			int Int3) ;
	void Info(char LETTERS[], int Int0,			int Int1,			float Float2,	int Int3) ;
	void Info(char LETTERS[], float Float0, int Int1,			float Float2,	int Int3) ;
	void Info(char LETTERS[], int Int0,			float Float1,	float Float2,	int Int3) ;
	void Info(char LETTERS[], float Float0, float Float1,	float Float2,	int Int3) ;
	void Info(char LETTERS[], int Int0,			int Int1,			int Int2,			float Float3) ;
	void Info(char LETTERS[], float Float0, int Int1,			int Int2,			float Float3) ;
	void Info(char LETTERS[], int Int0,			float Float1,	int Int2,			float Float3) ;
	void Info(char LETTERS[], float Float0, float Float1,	int Int2,			float Float3) ;
	void Info(char LETTERS[], int Int0,			int Int1,			float Float2,	float Float3) ;
	void Info(char LETTERS[], float Float0, int Int1,			float Float2,	float Float3) ;
	void Info(char LETTERS[], int Int0,			float Float1,	float Float2,	float Float3) ;
	void Info(char LETTERS[], float Float0, float Float1,	float Float2,	float Float3) ;

	void Info(char LETTERS[], int Int0,			int Int1,			int Int2,			int Int3,			int Int4) ;
	void Info(char LETTERS[], float Float0, int Int1,			int Int2,			int Int3,			int Int4) ;
	void Info(char LETTERS[], int Int0,			float Float1,	int Int2,			int Int3,			int Int4) ;
	void Info(char LETTERS[], float Float0, float Float1,	int Int2,			int Int3,			int Int4) ;
	void Info(char LETTERS[], int Int0,			int Int1,			float Float2,	int Int3,			int Int4) ;
	void Info(char LETTERS[], float Float0, int Int1,			float Float2,	int Int3,			int Int4) ;
	void Info(char LETTERS[], int Int0,			float Float1,	float Float2,	int Int3,			int Int4) ;
	void Info(char LETTERS[], float Float0, float Float1,	float Float2,	int Int3,			int Int4) ;
	void Info(char LETTERS[], int Int0,			int Int1,			int Int2,			float Float3,	int Int4) ;
	void Info(char LETTERS[], float Float0, int Int1,			int Int2,			float Float3,	int Int4) ;
	void Info(char LETTERS[], int Int0,			float Float1,	int Int2,			float Float3,	int Int4) ;
	void Info(char LETTERS[], float Float0, float Float1,	int Int2,			float Float3,	int Int4) ;
	void Info(char LETTERS[], int Int0,			int Int1,			float Float2,	float Float3,	int Int4) ;
	void Info(char LETTERS[], float Float0, int Int1,			float Float2,	float Float3,	int Int4) ;
	void Info(char LETTERS[], int Int0,			float Float1,	float Float2,	float Float3,	int Int4) ;
	void Info(char LETTERS[], float Float0, float Float1,	float Float2,	float Float3,	int Int4) ;
	void Info(char LETTERS[], int Int0,			int Int1,			int Int2,			int Int3,			float Float4) ;
	void Info(char LETTERS[], float Float0, int Int1,			int Int2,			int Int3,			float Float4) ;
	void Info(char LETTERS[], int Int0,			float Float1,	int Int2,			int Int3,			float Float4) ;
	void Info(char LETTERS[], float Float0, float Float1,	int Int2,			int Int3,			float Float4) ;
	void Info(char LETTERS[], int Int0,			int Int1,			float Float2,	int Int3,			float Float4) ;
	void Info(char LETTERS[], float Float0, int Int1,			float Float2,	int Int3,			float Float4) ;
	void Info(char LETTERS[], int Int0,			float Float1,	float Float2,	int Int3,			float Float4) ;
	void Info(char LETTERS[], float Float0, float Float1,	float Float2,	int Int3,			float Float4) ;
	void Info(char LETTERS[], int Int0,			int Int1,			int Int2,			float Float3,	float Float4) ;
	void Info(char LETTERS[], float Float0, int Int1,			int Int2,			float Float3,	float Float4) ;
	void Info(char LETTERS[], int Int0,			float Float1,	int Int2,			float Float3,	float Float4) ;
	void Info(char LETTERS[], float Float0, float Float1,	int Int2,			float Float3,	float Float4) ;
	void Info(char LETTERS[], int Int0,			int Int1,			float Float2,	float Float3,	float Float4) ;
	void Info(char LETTERS[], float Float0, int Int1,			float Float2,	float Float3,	float Float4) ;
	void Info(char LETTERS[], int Int0,			float Float1,	float Float2,	float Float3,	float Float4) ;
	void Info(char LETTERS[], float Float0, float Float1,	float Float2,	float Float3,	float Float4) ;




	void Info(int Xpos, int Ypos, char LETTERS[], int Int0) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0) ;

	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1) ;

	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			int Int2) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			int Int2) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	int Int2) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	int Int2) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			float Float2) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			float Float2) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	float Float2) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	float Float2) ;

	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			int Int2,			int Int3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			int Int2,			int Int3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	int Int2,			int Int3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	int Int2,			int Int3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			float Float2,	int Int3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			float Float2,	int Int3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	float Float2,	int Int3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	float Float2,	int Int3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			int Int2,			float Float3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			int Int2,			float Float3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	int Int2,			float Float3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	int Int2,			float Float3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			float Float2,	float Float3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			float Float2,	float Float3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	float Float2,	float Float3) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	float Float2,	float Float3) ;

	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			int Int2,			int Int3,			int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			int Int2,			int Int3,			int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	int Int2,			int Int3,			int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	int Int2,			int Int3,			int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			float Float2,	int Int3,			int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			float Float2,	int Int3,			int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	float Float2,	int Int3,			int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	float Float2,	int Int3,			int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			int Int2,			float Float3,	int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			int Int2,			float Float3,	int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	int Int2,			float Float3,	int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	int Int2,			float Float3,	int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			float Float2,	float Float3,	int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			float Float2,	float Float3,	int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	float Float2,	float Float3,	int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	float Float2,	float Float3,	int Int4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			int Int2,			int Int3,			float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			int Int2,			int Int3,			float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	int Int2,			int Int3,			float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	int Int2,			int Int3,			float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			float Float2,	int Int3,			float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			float Float2,	int Int3,			float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	float Float2,	int Int3,			float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	float Float2,	int Int3,			float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			int Int2,			float Float3,	float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			int Int2,			float Float3,	float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	int Int2,			float Float3,	float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	int Int2,			float Float3,	float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			int Int1,			float Float2,	float Float3,	float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, int Int1,			float Float2,	float Float3,	float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], int Int0,			float Float1,	float Float2,	float Float3,	float Float4) ;
	void Info(int Xpos, int Ypos, char LETTERS[], float Float0, float Float1,	float Float2,	float Float3,	float Float4) ;






private:
	char m_chLog[JLOGSIZE] ;
	char m_chCR[3] ;
};

#endif // !defined(AFX_JARDEBUG_H__69B857B3_878C_470A_B99E_4AA56D6B9508__INCLUDED_)
