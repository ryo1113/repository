//======================================================================================================================
//
// レンダリング処理 [renderer.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "renderer.h"
#include "scene.h"

#include "fade.h"
#include "number.h"

#include "bullet.h"
#include "effect.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// メンバ変数
//======================================================================================================================
CFade *CRenderer::m_Fade = NULL;

// コンストラクタ
CRenderer::CRenderer()
{

}

// デストラクタ
CRenderer::~CRenderer()
{

}

//======================================================================================================================
// 初期化処理
//======================================================================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ(Ｚバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート(現在の速度に合わせる)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// クライアント領域を直ちに更新する

	// Direct3Dデバイスの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,					// ディスプレイアダプタ
		D3DDEVTYPE_HAL,						// デバイスタイプ
		hWnd,								// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,// デバイス作成制御の組み合わせ
		&d3dpp,								// デバイスのプレゼンテーションパラメータ
		&m_pD3DDevice)))					// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	//レンダラステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリング設定

	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定						//テクスチャ背景なし３行
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αディスティネイションカラーの指定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数


#ifdef _DEBUG
	// デバッグ表示用フォントを設定
	D3DXCreateFont(m_pD3DDevice, 20, 10, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "ＭＳ ゴシック", &m_pFont);
#endif

	return S_OK;
}


//======================================================================================================================
// 終了処理
//======================================================================================================================
void CRenderer::Uninit()
{
#ifdef _DEBUG

	if (m_pFont != NULL)
	{// デバッグ表示用フォントの開放
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// Direct3Dデバイスの開放
	if (m_pD3DDevice != NULL)
	{	
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの開放
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	if (m_Fade)
	{
		delete m_Fade;
		m_Fade = NULL;
	}
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CRenderer::Update()
{
	CScene::UpdateAll();

	if (m_Fade)
	{
		m_Fade->UpdateFade();

		if (m_Fade->GetFade() == CFade::FADE_NONE)
		{
			m_Fade->UninitFade();

			delete m_Fade;
			m_Fade = NULL;
		}
	}
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CRenderer::Draw()
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CScene::DrawAll();

		CEffect::DrawAll();
		CBullet::DrawAll();

		if (m_Fade)
		{
			m_Fade->DrawFade();
		}

#ifdef _DEBUG
		// FPS表示
		DrawFPS();
#endif

		// 描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#ifdef _DEBUG
//======================================================================================================================
// FPS表示処理
//======================================================================================================================
void CRenderer::DrawFPS()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	char aStr[256];

	wsprintf(&aStr[0], "FPS:%d\n", m_nCountFPS);

	// テキスト描画
	m_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
#endif

//======================================================================================================================
// モードの設定(フェード)
//======================================================================================================================
void CRenderer::SetFade(CManager::MODE modeNext)
{
	if (!m_Fade)
	{
		m_Fade = CFade::Create(modeNext);
	}
}

//======================================================================================================================
// フェードの取得
//======================================================================================================================
CFade *CRenderer::GetFade()
{
	return m_Fade;
}

//======================================================================================================================
// デバイスの取得
//======================================================================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice()
{
	return m_pD3DDevice;
}

