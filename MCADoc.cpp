
// MCADoc.cpp : implementation of the CMCADoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MCA.h"
#endif

#include "MCADoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ftd2xx.h"

FT_HANDLE   ftHandle[4];
FT_STATUS   ftStatus;
DWORD       numDevs;
DWORD		devIndex;
int			devcnt, devlast;
char        buf[256];


// CMCADoc

IMPLEMENT_DYNCREATE(CMCADoc, CDocument)

BEGIN_MESSAGE_MAP(CMCADoc, CDocument)
	ON_COMMAND(ID_OPERATION_SUBTRACTAFROMB, &CMCADoc::OnOperationSubtractafromb)
	ON_COMMAND(ID_MCA_START, &CMCADoc::OnMcaStart)
	ON_COMMAND(ID_MCA_STOP, &CMCADoc::OnMcaStop)
	ON_COMMAND(ID_MCA_MEMORYCLEAR, &CMCADoc::OnMcaMemoryclear)
	ON_COMMAND(ID_MCA_POINTERCLEAR, &CMCADoc::OnMcaPointerclear)
	ON_COMMAND(ID_MCA_MEMORYREAD, &CMCADoc::OnMcaMemoryread)
	ON_COMMAND(ID_MCA_THRESHOLD, &CMCADoc::OnMcaThreshold)
	ON_COMMAND(ID_MCA_THRESHOLD32777, &CMCADoc::OnMcaThreshold32777)
	ON_COMMAND(ID_OPERATION_ADDATOB, &CMCADoc::OnOperationAddatob)
	ON_COMMAND(ID_OPERATION_SAMPLEDATA, &CMCADoc::OnOperationSampledata)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMCADoc::OnFileSaveAs)
	ON_COMMAND(ID_OPERATION_COPYATOC, &CMCADoc::OnOperationCopyatoc)
	ON_COMMAND(ID_OPERATION_COPYATOD, &CMCADoc::OnOperationCopyatod)
	ON_COMMAND(ID_MCA_WAVEFORMREC, &CMCADoc::OnMcaWaveformrec)
	ON_COMMAND(ID_VIEW_1D, &CMCADoc::OnView1d)
	ON_COMMAND(ID_OPERATION_BIAS, &CMCADoc::OnOperationBias)
	ON_COMMAND(ID_MCA_WAVEMONITOR, &CMCADoc::OnMcaWavemonitor)
	ON_COMMAND(ID_OPERATION_COPYBTOA, &CMCADoc::OnOperationCopybtoa)
	ON_COMMAND(ID_OPERATION_CLEARA, &CMCADoc::OnOperationCleara)
	ON_COMMAND(ID_OPERATION_COPYCTOA, &CMCADoc::OnOperationCopyctoa)
	ON_COMMAND(ID_OPERATION_COPYDTOA, &CMCADoc::OnOperationCopydtoa)
	ON_COMMAND(ID_OPERATION_CLEARB, &CMCADoc::OnOperationClearb)
	ON_COMMAND(ID_OPERATION_CLEARC, &CMCADoc::OnOperationClearc)
	ON_COMMAND(ID_OPERATION_CLEARD, &CMCADoc::OnOperationCleard)
	ON_COMMAND(ID_MCA_IMAGEREADOUT, &CMCADoc::OnMcaImagereadout)
	ON_COMMAND(ID_OPERATION_COPYATOB, &CMCADoc::OnOperationCopyatob)
	ON_COMMAND(ID_OPERATION_OPTION, &CMCADoc::OnOperationOption)
	ON_COMMAND(ID_OPERATION_MULTIPLY, &CMCADoc::OnOperationMultiply)
	ON_COMMAND(ID_OPERATION_SUBTRACTBFROMA, &CMCADoc::OnOperationSubtractbfroma)
END_MESSAGE_MAP()


// CMCADoc construction/destruction

CMCADoc::CMCADoc()
{
	// TODO: add one-time construction code here
	int i,j;

	icsr=0;
	for(i=0;i<DMAX;i++){
		a[i]=b[i]=c[i]=d[i]=0;
		dp[i].x=i;dp[i].y=0;
	}
	for(i=0;i<IMAGESIZE;i++){
		for(j=0;j<IMAGESIZE;j++){
			image[i][j]=0;
		}
	}
	for(i=0;i<100;i++)param[i]=0;  // Communication between CMCAView and CMCADoc

	param[20]=0; /* 1-dim */
	param[19]=0; // USB DEVICE IDLING
	param[18]=0; // USB DEVICE ACTIVE =1

	param[2]=16; /* Initial Scale x-axis : 16 = 4K Channel,  4 = 16K Channel */
	param[3]=65536*2;/* Initial Scale y-axis*/
	param[9]=0; /* Current Position */
	param[10]=4; /* transfer length */
	param[11]=10; /* 10 times */

	for(i=0;i<16;i++)roi[i]=0.0;

///////////////////////////////////////////////////////////////////////////
	// USB setup
    numDevs = 0;

           ftStatus = FT_ListDevices(&numDevs,NULL,FT_LIST_NUMBER_ONLY);
           devlast = numDevs;
		   d[0]=devlast; d[1]=ftStatus;

			for(devcnt = 0;devcnt != devlast;devcnt++){
               devIndex = (DWORD)devcnt;

               ftStatus = FT_ListDevices((PVOID)devIndex,buf,
                                          FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);
 		   b[0]=ftStatus;
               if(ftStatus == FT_OK){

                   ftStatus = FT_OpenEx(buf,
                                        FT_OPEN_BY_SERIAL_NUMBER,&ftHandle[devcnt]);
				   param[18]=1; // active device found
				   param[19]=99; // Just Conneced
 
               }
		   }


}

CMCADoc::~CMCADoc()
{
	
           for(devcnt = 0;devcnt != devlast;devcnt++){
             ftStatus = FT_Close(ftHandle[devcnt]);  // USB Device CLOSE
		   }

}



// CMCADoc serialization

void CMCADoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMCADoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMCADoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMCADoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMCADoc diagnostics

#ifdef _DEBUG
void CMCADoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMCADoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMCADoc commands


void CMCADoc::OnOperationSubtractafromb()
{
	int i;
	for(i=0;i<DMAX;i++){
	a[i]-=b[i];
	}
	SetModifiedFlag();
	UpdateAllViews(NULL);

}


void CMCADoc::OnMcaStart()
{
  DWORD       WriteNum, TransNum;
  
  // USB input

    devcnt=0;	// device #0		Normal collection mode command 0x7;
                 TransNum = 0; WriteNum=1; 
				buf[0]=0x7;	
			   ftStatus = FT_Write(ftHandle[devcnt],buf,WriteNum,&TransNum);
		param[19]=7;

	SetModifiedFlag();
	UpdateAllViews(NULL);	


}


void CMCADoc::OnMcaStop()
{

  DWORD       WriteNum, TransNum;
	
                TransNum = 0; WriteNum=1; 
				buf[0]=0x4; // set idling mode	
			   ftStatus = FT_Write(ftHandle[devcnt],buf,WriteNum,&TransNum);
		param[19]=4;

	SetModifiedFlag();
	UpdateAllViews(NULL);	

}


void CMCADoc::OnMcaMemoryclear()
{
  DWORD       WriteNum, TransNum;
  

  devcnt=0;	// device #0
               TransNum = 0; WriteNum=1; 
			   buf[0]=0x01; // MEMORY CLEAR COMMAND
               ftStatus = FT_Write(ftHandle[devcnt],buf,WriteNum,&TransNum);
		b[0]=ftStatus;
	param[19]=1;

}


void CMCADoc::OnMcaPointerclear()
{
	
	  DWORD       WriteNum,  TransNum;
  	

    devcnt=0;	// device #0		address counter clear command 0x20;
               TransNum = 0; WriteNum=1; 
			   buf[0]=0x02; //
               ftStatus = FT_Write(ftHandle[devcnt],buf,WriteNum,&TransNum);
               TransNum = 0; WriteNum=1; 
			   buf[0]=0x04; // 
               ftStatus = FT_Write(ftHandle[devcnt],buf,WriteNum,&TransNum);
	param[19]=2;
  
	SetModifiedFlag();
	UpdateAllViews(NULL);	

}


void CMCADoc::OnMcaMemoryread()
{
	int i,j;
  DWORD       WriteNum, TransNum;
  DWORD       ReadNum;
  unsigned char bufc[128];

  devcnt=0;	// device #0
 
  for (i=0;i<64;i++){
			TransNum = 0; WriteNum=1; 
			   buf[0]=0x08; // transfer len set to be 128
               ftStatus = FT_Write(ftHandle[devcnt],buf,WriteNum,&TransNum);

			   for (j=0;j<128;j++) bufc[j]=buf[j];
				
			   buf[0]=0x05; // USB FIFO data load command
               ftStatus = FT_Write(ftHandle[devcnt],buf,WriteNum,&TransNum);

			   for (j=0;j<128;j++) bufc[j]=buf[j];

				  TransNum = 0; WriteNum=0; ReadNum=128; 

               ftStatus = FT_Read(ftHandle[devcnt],bufc,ReadNum,&TransNum);			   
			   for (j=0;j<64;j++) a[j+i*64]=bufc[2*j]+bufc[2*j+1]*256;

				

  }
  
  param[19]=5;

	SetModifiedFlag();
	UpdateAllViews(NULL);	

}


void CMCADoc::OnMcaThreshold()
{
		
  DWORD       WriteNum, TransNum;
  
  // Threshold UP

  devcnt=0;	// device #0
               TransNum = 0; WriteNum=1; 
			   buf[0]=0x10; 
               ftStatus = FT_Write(ftHandle[devcnt],buf,WriteNum,&TransNum);
	param[19]=16;
	SetModifiedFlag();
	UpdateAllViews(NULL);	

}


void CMCADoc::OnMcaThreshold32777()
{
	  DWORD       WriteNum, TransNum;
  
  // Threshold DOWN

  devcnt=0;	// device #0
               TransNum = 0; WriteNum=1; 
			   buf[0]=0x11; 
               ftStatus = FT_Write(ftHandle[devcnt],buf,WriteNum,&TransNum);
	param[19]=17;

	SetModifiedFlag();
	UpdateAllViews(NULL);	


}


void CMCADoc::OnOperationAddatob()
{
	int i;
	for(i=0;i<DMAX;i++){b[i]=a[i];}
	UpdateAllViews(NULL);
}


void CMCADoc::OnOperationSampledata()
{
	int i,j;
	double s;

	for (j = 0; j < 300; j++) {
		s = 0;
		for (i = 0; i < 2000; i++) {
			if (a[i] > b[i + j]) { s = s + a[i] - b[i + j]; }
			else { s = s + b[i + j] - a[i]; }
		}
		c[j] = s / 300;
	}
	SetModifiedFlag();
	UpdateAllViews(NULL);
}



BOOL CMCADoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CStdioFile afile;
	CString Ctmp;
	char cs[2048];
	afile.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
//	Ctmp="HELLO\n";
//	afile.WriteString(Ctmp);
	int i = 0;
	for(i=0;i<DMAX;i++){
		sprintf_s(cs, ("%lf\n"), a[i]); Ctmp=cs;
		afile.WriteString(Ctmp);
	}

	afile.Close();
	UpdateAllViews(NULL);
	return TRUE;
	//return CDocument::OnSaveDocument(lpszPathName);
}

BOOL CMCADoc::OnOpenDocument(LPCTSTR lpszPathName)
{
CStdioFile afile;
CFileException fe;
int i;

CString Ctmp;

if( !afile.Open(lpszPathName, CFile::modeRead | CFile::typeText))
		return false;

//if (!CDocument::OnOpenDocument(lpszPathName))return FALSE;

for(i=0;i<DMAX;i++){afile.ReadString(Ctmp); 
a[i]=atof(CT2A(Ctmp));}

	afile.Close();
	UpdateAllViews(NULL);

	return TRUE;
//return CDocument::OnOpenDocument(lpszPathName );

}



BOOL CMCADoc::OnNewDocument()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	return CDocument::OnNewDocument();
}


void CMCADoc::OnOperationCopyatoc()
{
	int i;
	for(i=0;i<DMAX;i++){c[i]=a[i];}
	UpdateAllViews(NULL);
}


void CMCADoc::OnOperationCopyatod()
{
	int i;
	for(i=0;i<DMAX;i++){d[i]=a[i];}
	UpdateAllViews(NULL);
}


void CMCADoc::OnMcaWaveformrec()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。		
  DWORD       WriteNum, TransNum;
  
  // USB input

  devcnt=0;	// device #0
               TransNum = 0; WriteNum=1; 
			   buf[0]=0x03; 
               ftStatus = FT_Write(ftHandle[devcnt],buf,WriteNum,&TransNum);
	param[19]=3;
	SetModifiedFlag();
	UpdateAllViews(NULL);	

}


void CMCADoc::OnView1d()
{
	 param[20]=1-param[20]; // 1D/ 2D toggle
}


void CMCADoc::OnOperationBias()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	int i;
	for(i=0;i<DMAX;i++){a[i]-=512;}
	UpdateAllViews(NULL);
}


void CMCADoc::OnMcaWavemonitor()
{
	// READ 64k points from FPGA board
	int i, j,k;
	DWORD       WriteNum, TransNum;
	DWORD       ReadNum;
	unsigned char bufc[128];

	devcnt = 0;	// device #0


	for (k = 0; k < 16; k++) {
		for (i = 0; i < 64; i++) {
			TransNum = 0; WriteNum = 1;
			buf[0] = 0x08; // transfer len set to be 128
			ftStatus = FT_Write(ftHandle[devcnt], buf, WriteNum, &TransNum);

			for (j = 0; j < 128; j++) bufc[j] = buf[j];

			buf[0] = 0x05; // USB FIFO data load command
			ftStatus = FT_Write(ftHandle[devcnt], buf, WriteNum, &TransNum);

			for (j = 0; j < 128; j++) bufc[j] = buf[j];

			TransNum = 0; WriteNum = 0; ReadNum = 128;

			ftStatus = FT_Read(ftHandle[devcnt], bufc, ReadNum, &TransNum);
			for (j = 0; j < 64; j++) a[j + i * 64+k*4096] = bufc[2 * j] + bufc[2 * j + 1] * 256;
		}
		Sleep(10);

		SetModifiedFlag();
		UpdateAllViews(NULL);
	}

	param[19] = 5;

	SetModifiedFlag();
	UpdateAllViews(NULL);
}


void CMCADoc::OnOperationCopybtoa()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	int i;
	for(i=0;i<DMAX;i++){a[i]=b[i];}
	UpdateAllViews(NULL);
}


void CMCADoc::OnOperationCleara()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	int i;
	for(i=0;i<DMAX;i++){a[i]=0;}
	UpdateAllViews(NULL);
}


void CMCADoc::OnOperationCopyctoa()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	int i;
	for(i=0;i<DMAX;i++){a[i]=c[i];}
	UpdateAllViews(NULL);
}


void CMCADoc::OnOperationCopydtoa()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	int i;
	for(i=0;i<DMAX;i++){a[i]=d[i];}
	UpdateAllViews(NULL);
}


void CMCADoc::OnOperationClearb()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	int i;
	for(i=0;i<DMAX;i++){b[i]=0;}
	UpdateAllViews(NULL);
}


void CMCADoc::OnOperationClearc()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	int i;
	for(i=0;i<DMAX;i++){c[i]=0;}
	UpdateAllViews(NULL);
}


void CMCADoc::OnOperationCleard()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	int i;
	for(i=0;i<DMAX;i++){d[i]=0;}
	for (i = 0; i < DMAX / 2; i++){ d[i] = 0; }
	UpdateAllViews(NULL);
}


void CMCADoc::OnMcaImagereadout()
{
	// TODO: Add your command handler code here
	int i, j;
	DWORD       WriteNum, TransNum;
	DWORD       ReadNum;
	unsigned char bufc[128];

	devcnt = 0;	// device #0

	for (i = 0; i<64; i++) {
		TransNum = 0; WriteNum = 1;
		buf[0] = 0x08; // transfer len set to be 128
		ftStatus = FT_Write(ftHandle[devcnt], buf, WriteNum, &TransNum);

		for (j = 0; j<128; j++) bufc[j] = buf[j];

		buf[0] = 0x05; // USB FIFO data load command
		ftStatus = FT_Write(ftHandle[devcnt], buf, WriteNum, &TransNum);

		for (j = 0; j<128; j++) bufc[j] = buf[j];

		TransNum = 0; WriteNum = 0; ReadNum = 128;

		ftStatus = FT_Read(ftHandle[devcnt], bufc, ReadNum, &TransNum);
		for (j = 0; j<64; j++) u[j + i * 64] = bufc[2 * j] + bufc[2 * j + 1] * 256;
	}
	for (i = 0; i < 2047; i++) {
		a[i] = u[2 * i]; b[i] = u[2 * i + 1];
	}

	param[19] = 5;

	SetModifiedFlag();
	UpdateAllViews(NULL);

}


void CMCADoc::OnOperationCopyatob()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	int i;
	for (i = 0; i<DMAX; i++) { b[i] = a[i]; }
	UpdateAllViews(NULL);
}


void CMCADoc::OnOperationOption()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMCADoc::OnOperationMultiply()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMCADoc::OnOperationSubtractbfroma()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}
