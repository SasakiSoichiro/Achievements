//**********************************
//
// メイン処理 [main.cpp]
// Author:Sasaki Soichiro
//
//**********************************

//*******************
// インクルード
//*******************
#include "main.h"
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include<crtdbg.h>
#include "manager.h"
#include "debugproc.h"

//********************
// プロトタイプ宣言
//********************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//********************
// メイン関数
//********************
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hInstanceprev, _In_ LPSTR bCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// メモリリーク検知用のフラグ
	DWORD dwCurrentTime;		// 現在時刻
	DWORD dwExecLastTime;		// 最後に処理した時刻
	DWORD dwFrameCnt;
	DWORD dwFPSLastTime;
	CManager* pManager = new CManager;
	int nCntFPS=0;
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL,IDI_APPLICATION)
	};
	HWND hWnd;
	MSG msg;
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	//ウィンドウを生成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	pManager->Init(hInstance,hWnd, TRUE);
	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();
	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	dwFrameCnt = 0;
	dwFPSLastTime = timeGetTime();
	//メッセージループ
	while(1)// (GetMessage(&msg, NULL, 0, 0) != 0)
	{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
			{//windowsの処理
				if(msg.message==WM_QUIT)
				{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
					break;
				}
			else
			{//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
			else
			{//DirectXの処理
				dwCurrentTime = timeGetTime();//現在時刻を取得
				if ((dwCurrentTime - dwFPSLastTime) >= 500)
				{
					nCntFPS = (dwFrameCnt * 1000) / (dwCurrentTime - dwFPSLastTime);
					dwFPSLastTime = dwCurrentTime;
					dwFrameCnt = 0;
				}
				if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
				{//60分の1秒経過
					dwExecLastTime = dwCurrentTime;//処理開始時刻を保存
					//更新処理
					pManager->Update();
					CDebugProc::Print("FPS:%d",nCntFPS);
					//描画処理
					pManager->Draw();
					dwFrameCnt++;
				}
			
			}
	}
	if (pManager != NULL)
	{
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}
	timeEndPeriod(1);
	//ウィンドウの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return (int)msg.wParam;
}
//********************
// プロシージャ
//********************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			//ウィンドウを破棄する
			DestroyWindow(hWnd);
			break;
		}
		break;
	}
  	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
