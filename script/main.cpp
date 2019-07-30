//======================================================================================================================
//
// メイン処理 [main.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"

#ifdef _DEBUG
// デバック処理の時だけ

#endif

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define CLASS_NAME		"WindowClass"			// ウインドウのクラス名
#define WINDOW_NAME		"Windows"			// ウインドウのキャプション名

//======================================================================================================================
// 構造体定義
//======================================================================================================================

//======================================================================================================================
// プロトタイプ宣言
//======================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Init(void);
void Uninit(void);
void Update(void);
void Draw(void);

//MODE g_Mode = MODE_TITLE;		//現在のモード
//MODE g_ModeOld = MODE_TITLE;		//前のモード

//======================================================================================================================
// グローバル変数
//======================================================================================================================
LPDIRECT3D9			g_pD3D = NULL;			// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ(描画に必要)

//======================================================================================================================
// メンバ変数
//======================================================================================================================
#ifdef _DEBUG
int CRenderer::m_nCountFPS = 0;
#endif

//======================================================================================================================
// 初期化処理
//======================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL, IDI_APPLICATION)
	};
	HWND hWnd;
	MSG msg;
	RECT rect =	{0,	0, SCREEN_WIDTH, SCREEN_HEIGHT};

	DWORD dwCurrentTime;		// 現在時刻
	DWORD dwExecLastTime;		// 最後に処理した時刻	

	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウインドウを作成
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	CManager *pManager;
	pManager = new CManager;

	// 初期化処理(ウィンドウを作成してから行う)
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))		// FALSEでフル画面
	{
		return -1;
	}

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// システム時刻をミリ秒単位で取得

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// システム時刻をミリ秒単位で取得

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// メッセージを取得しなかった場合 0 を返す
		{// Windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージが来たらループを抜ける
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理

			dwCurrentTime = timeGetTime();		// システム時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				CRenderer::m_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);		// FPSを測定
#endif

				dwFPSLastTime = dwCurrentTime;		// FPSを測定した時刻を保存
				dwFrameCount = 0;					// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))		// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;		// 処理した時刻を保存

				// 更新処理
				pManager->Update();

				// 描画処理
				pManager->Draw();

				dwFrameCount++;			// カウントを加算
			}
		}
	}
	
	if (pManager != NULL)
	{
		// 終了処理
		pManager->Uninit();

		delete pManager;
		pManager = NULL;
	}

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//======================================================================================================================
// ウインドウプロシージャ
//======================================================================================================================
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
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//======================================================================================================================
// 初期化処理
//======================================================================================================================
void Init(void)
{

}


//======================================================================================================================
// 終了処理
//======================================================================================================================
void Uninit(void)
{	

}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void Update(void)
{

}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void Draw(void)
{

}

//======================================================================================================================
// モードの設定
//======================================================================================================================

