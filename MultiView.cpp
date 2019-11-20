
// MultiView.cpp: CMultiView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Multi.h"
#endif

#include "MultiDoc.h"
#include "MultiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiView

IMPLEMENT_DYNCREATE(CMultiView, CView)

BEGIN_MESSAGE_MAP(CMultiView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_32771, &CMultiView::OnFileFindmax)
	ON_MESSAGE(WM_MAX, OnMaxFound)
END_MESSAGE_MAP()

// CMultiView 생성/소멸
SData data;
CMultiView::CMultiView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMultiView::~CMultiView()
{
}

BOOL CMultiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMultiView 그리기

void CMultiView::OnDraw(CDC* /*pDC*/)
{
	CMultiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMultiView 인쇄

BOOL CMultiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMultiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMultiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMultiView 진단

#ifdef _DEBUG
void CMultiView::AssertValid() const
{
	CView::AssertValid();
}

void CMultiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMultiDoc* CMultiView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMultiDoc)));
	return (CMultiDoc*)m_pDocument;
}
#endif //_DEBUG


// CMultiView 메시지 처리기

CWinThread* Thread;
UINT ThreadProcedure(LPVOID pParam);

UINT ThreadProcedure(LPVOID pParam) {
	SData* pData = (SData*)pParam;
	int i;
	for (i = 0; i < 5; i++) {
		if (pData->array[i] > pData->max)
			pData->max = pData->array[i];
	} // 최대값 찾기
	::PostMessage(pData->NotifyWindow, WM_MAX, 0, 0);
	return 0;
}
void CMultiView::OnFileFindmax()
{
	
	data.array[0] = 1;
	data.array[1] = 2;
	data.array[2] = 3;
	data.array[3] = 4;
	data.array[4] = 5;
	data.max = data.array[0];
	data.NotifyWindow = m_hWnd;
	Thread = AfxBeginThread(ThreadProcedure, &data);
}





LRESULT CMultiView::OnMaxFound(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 구현 코드 추가.
	CClientDC dc(this);
	CString msg;
	msg.Format(L"%d", data.max);
	dc.TextOut(10, 100, msg);
	TerminateThread(Thread, 0);
	return 0;
	
}
