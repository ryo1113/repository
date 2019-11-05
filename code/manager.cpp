//======================================================================================================================
//
// 管理処理 [manager.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

#include "input.h"
#include "pad.h"
#include "keyboard.h"

#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "pause.h"

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
// メンバ変数
//======================================================================================================================
CRenderer *CManager::m_pRenderer = {};

CKeyboard *CManager::m_pInputKeyboard = {};
CPad *CManager::m_pInputPad = {};

CModeBase *CManager::m_pMode = {};
CManager::MODE CManager::m_Mode = {};
CPause *CManager::m_pPause = {};

CSound *CManager::m_pSound = {};

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
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))			// FALSEでフル画面
	{
		return -1;
	}

	// キーボード
	m_pInputKeyboard = new CKeyboard;
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return -1;
	}

	// パッド
	m_pInputPad = new CPad;
	if (FAILED(m_pInputPad->Init(hInstance, hWnd)))
	{
		return -1;
	}

	// サウンド
	m_pSound = new CSound;
	if (FAILED(m_pSound->InitSound(hWnd)))
	{
		return -1;
	}

	// テクスチャ管理(ないとき)
	if (FAILED(this->TexLoad(hWnd)))
	{
		return -1;
	}

	CManager::SetMode(MODE_TITLE);
	m_Mode = MODE_TITLE;

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
	
	if (m_pMode)
	{
		m_pMode->Uninit();
	}

	if (m_pPause)
	{
		m_pPause->Uninit();
	}

	if (m_pSound)
	{
		m_pSound->UninitSound();
	}

	// テクスチャ破棄
	this->TexUnload();

	CScene::ReleaseAll();
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CManager::Update()
{
	m_pInputKeyboard->Update();

	m_pInputPad->Update();

	m_pRenderer->Update();

	if (m_pMode)
	{
		if (m_pPause)
		{
			m_pPause->Update();
		}
		else
		{
			m_pMode->Update();
		}
	}
}

void CManager::Draw()
{
	m_pRenderer->Draw();

	if (m_pMode)
	{
		m_pMode->Draw();
	}

	if (m_pPause)
	{
		m_pPause->Draw();
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

//======================================================================================================================
// モードの設定
//======================================================================================================================
void CManager::SetMode(MODE mode)
{
	// ポーズに遷移しない時	コンテニューを押してない時
	if (m_pMode && mode != MODE_PAUSE && CPause::GetPauseState() != CPause::PAUSE_STATE_CONTINUE)
	{
		m_pSound->StopSound();

		m_pMode->Uninit();

		CScene::ReleaseAll();

		delete m_pMode;
		m_pMode = NULL;
	}

	// 前がポーズの時
	if (m_pPause && m_Mode == MODE_PAUSE)
	{
		if (CPause::GetPauseState() == CPause::PAUSE_STATE_CONTINUE)
		{
			m_pPause->Uninit();
		}

		delete m_pPause;
		m_pPause = NULL;
	}

	m_Mode = mode;

	if (mode == MODE_PAUSE || (!m_pPause && CPause::GetPauseState() != CPause::PAUSE_STATE_CONTINUE))
	{
		switch (mode)
		{
		case MODE_TITLE:
			m_pMode = CTitle::Create();
			CManager::SetSound(CSound::SOUND_LABEL_BGM000);
			break;

		case MODE_TUTORIAL:
			m_pMode = CTutorial::Create();
			CManager::SetSound(CSound::SOUND_LABEL_BGM001);
			break;

		case MODE_GAME:
			m_pMode = CGame::Create();
			CManager::SetSound(CSound::SOUND_LABEL_BGM002);
			break;

		case MODE_RESULT:
			m_pMode = CResult::Create();
			CManager::SetSound(CSound::SOUND_LABEL_BGM003);
			break;

		case MODE_PAUSE:
			m_pPause = CPause::Create();
			break;
		}
	}
	else
	{// コンテニューを押したとき
		CPause::SetPauseState(CPause::PAUSE_STATE_NONE);
	}
}

//======================================================================================================================
// テクスチャの読み込み
//======================================================================================================================
HRESULT CManager::TexLoad(HWND hWnd)
{
	if (FAILED(CBullet::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Bullet", MB_ICONWARNING);
		return -1;
	}
	if (FAILED(CPlayer::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Player", MB_ICONWARNING);
		return -1;
	}
	if (FAILED(CExplosion::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Explosion", MB_ICONWARNING);
		return -1;
	}
	if (FAILED(CEffect::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Effect", MB_ICONWARNING);
		return -1;
	}
	if (FAILED(CBg::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Bg", MB_ICONWARNING);
		return -1;
	}
	if (FAILED(CEnemy::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Enemy", MB_ICONWARNING);
		return -1;
	}
	if (FAILED(CNumber::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Number", MB_ICONWARNING);
		return -1;
	}
	if (FAILED(CWord::Load()))
	{
		MessageBox(hWnd, "テクスチャがありません", "Word", MB_ICONWARNING);
		return -1;
	}
	return S_OK;
}

//======================================================================================================================
// テクスチャの破棄
//======================================================================================================================
void CManager::TexUnload()
{
	CBullet::Unload();
	CEffect::Unload();
	CEnemy::Unload();
	CExplosion::Unload();
	CPlayer::Unload();
	CBg::Unload();
	CWord::Unload();
}

//======================================================================================================================
// モードの取得
//======================================================================================================================
CManager::MODE CManager::GetMode()
{
	return m_Mode;
}

//======================================================================================================================
// 乱数値の取得(整数)
//======================================================================================================================
int CManager::Random(int min, int max)
{
	// 乱数生成器
	static std::mt19937_64 create(0);

	// 一様分布整数の分布生成器
	std::uniform_int_distribution<int> nGet(min, max);

	// 乱数を生成
	return nGet(create);
}

//======================================================================================================================
// サウンドの再生
//======================================================================================================================
void CManager::SetSound(CSound::SOUND_LABEL label)
{
	m_pSound->PlaySound(label);
}
