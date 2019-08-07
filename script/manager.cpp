//======================================================================================================================
//
// 処理 [manager.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

#include "input.h"
#include "pad.h"
#include "keyboard.h"
#include "mouse.h"

#include "title.h"
#include "game.h"
#include "result.h"

#include "number.h"
#include "word.h"

#include "bg.h"
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "explosion.h"
#include "player.h"
#include "score.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// 構造体定義
//======================================================================================================================

//======================================================================================================================
// プロトタイプ宣言
//======================================================================================================================

//======================================================================================================================
// メンバ変数
//======================================================================================================================
CRenderer *CManager::m_pRenderer = NULL;
CKeyboard *CManager::m_pInputKeyboard = NULL;
CPad *CManager::m_pInputPad = NULL;
CMouse *CManager::m_pInputMouse = NULL;

CModeBase *CManager::m_pMode = new CTitle;
CManager::MODE CManager::m_Mode = MODE_TITLE;

// コンストラクタ
CManager::CManager()
{
}

// デストラクタ
CManager::~CManager()
{
}

//======================================================================================================================
// 初期化
//======================================================================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	m_pRenderer = new CRenderer;
	// 初期化処理(ウィンドウを作成してから行う)
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return -1;
	}

	m_pInputKeyboard = new CKeyboard;
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return -1;
	}

	m_pInputPad = new CPad;
	if (FAILED(m_pInputPad->Init(hInstance, hWnd)))
	{
		return -1;
	}

	m_pInputMouse = new CMouse;
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return -1;
	}

	// テクスチャ管理(ないとき)
	if (FAILED(CBullet::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Bullet", MB_ICONWARNING);// 確認用
		return -1;
	}
	if (FAILED(CPlayer::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Player", MB_ICONWARNING);// 確認用
		return -1;
	}
	if (FAILED(CExplosion::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Explosion", MB_ICONWARNING);// 確認用
		return -1;
	}
	if (FAILED(CEffect::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Effect", MB_ICONWARNING);// 確認用
		return -1;
	}
	if (FAILED(CBg::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Bg", MB_ICONWARNING);// 確認用
		return -1;
	}
	if (FAILED(CEnemy::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Enemy", MB_ICONWARNING);// 確認用
		return -1;
	}
	if (FAILED(CNumber::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Number", MB_ICONWARNING);// 確認用
		return -1;
	}
	if (FAILED(CWord::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Word", MB_ICONWARNING);// 確認用
		return -1;
	}

	if (m_pMode)
	{
		m_pMode->Init();
	}

	return S_OK;
}

//======================================================================================================================
// 終了処理
//======================================================================================================================
void CManager::Uninit()
{
	if (m_pRenderer != NULL)
	{
		// 終了処理
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (m_pInputKeyboard != NULL)
	{
		// 終了処理
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (m_pInputPad != NULL)
	{
		// 終了処理
		m_pInputPad->Uninit();

		delete m_pInputPad;
		m_pInputPad = NULL;
	}
	
	if (m_pInputMouse != NULL)
	{
		// 終了処理
		m_pInputMouse->Uninit();

		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	if (m_pMode)
	{
		m_pMode->Uninit();
	}

	// テクスチャ破棄
	CBullet::Unload();
	CEffect::Unload();
	CEnemy::Unload();
	CExplosion::Unload();
	CPlayer::Unload();
	CBg::Unload();
	CWord::Unload();

	CScene::ReleaseAll();
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CManager::Update()
{
	m_pInputKeyboard->Update();

	m_pInputMouse->Update();

	m_pInputPad->Update();

	m_pRenderer->Update();

	if (m_pMode)
	{
		m_pMode->Update();
	}
}

void CManager::Draw()
{
	m_pRenderer->Draw();

	if (m_pMode)
	{
		m_pMode->Draw();
	}
}

CRenderer *CManager::GetRenderer()
{
	return m_pRenderer;
}

CKeyboard *CManager::GetInputKeyboard()
{
	return m_pInputKeyboard;
}

CPad *CManager::GetInputPad()
{
	return m_pInputPad;
}

CMouse *CManager::GetInputMouse()
{
	return m_pInputMouse;
}

//======================================================================================================================
// モードの設定
//======================================================================================================================
void CManager::SetMode(MODE mode)
{
	if (m_pMode)
	{
		CScene::ReleaseAll();

		delete m_pMode;
		m_pMode = NULL;
	}

	m_Mode = mode;

	switch (mode)
	{
	case MODE_TITLE:
		m_pMode = CTitle::Create();
		break;

	case MODE_TUTORIAL:
		break;

	case MODE_GAME:
		m_pMode = CGame::Create();
		break;

	case MODE_RESULT:
		m_pMode = CResult::Create();
		break;

	case MODE_RANKING:
		break;
	}
}

//======================================================================================================================
// モードの取得
//======================================================================================================================
CManager::MODE CManager::GetMode()
{
	return m_Mode;
}
