//======================================================================================================================
//
// ゲーム処理 [game.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "game.h"

#include "camera.h"
#include "fade.h"

#include "bg.h"
#include "bullet.h"
#include "effect.h"
#include "explosion.h"
#include "player.h"

#include "enemy/bird.h"
#include "enemy/heri.h"
#include "enemy/void.h"

#include "word.h"
#include "number.h"
#include "score.h"
#include "scoreplus.h"

#include "keyboard.h"
#include "pad.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define TYPEDISPLAY_BULLET		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f))
#define PREPARATION_TIME		(3)

//======================================================================================================================
// メンバ変数
//======================================================================================================================
list<CScorePlus*> CGame::m_Scoreplus = {};

//======================================================================================================================
// 初期化処理
//======================================================================================================================
void CGame::Init()
{
	CBg::Create(0.0005f);

	m_Player = CPlayer::Create();

	CScore::Create(D3DXVECTOR3(1250.0f, 60.0f, 0.0f), D3DXVECTOR3(60.0f, 100.0f, 0.0f));

	m_pWord[0] = CWord::Create(CWord::WORD_BULLET_00, D3DXVECTOR3(100.0f, 75.0f, 0.0f), D3DXVECTOR3(150.0f, 150.0f, 0.0f));
	m_pWord[1] = CWord::Create(CWord::WORD_BULLET_01, D3DXVECTOR3(250.0f, 75.0f, 0.0f), D3DXVECTOR3(150.0f, 150.0f, 0.0f));

	CCamera::Create();

	nCount = PREPARATION_TIME * 60;
	m_State = GAMESTATE_PREPARATION;

	m_pWord[1]->SetCollar(TYPEDISPLAY_BULLET);

	m_pBg = CWord::Create(CWord::WORD_BLACK_BG, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	m_pNumber = CNumber::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 8.0f, SCREEN_HEIGHT / 3.0f, 0.0f));
	m_pNumber->SetNum(PREPARATION_TIME);
}

//======================================================================================================================
// 終了処理
//======================================================================================================================
void CGame::Uninit()
{
	for (int nCnt = 0; nCnt < (int)m_Scoreplus.size(); nCnt++)
	{
		m_Scoreplus.front()->Uninit();
	}
	if (!m_Scoreplus.empty())
	{
		m_Scoreplus.clear();
	}
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CGame::Update()
{
	// 弾の種類表示
	this->BulletTypeDisplay();

	switch (m_State)
	{
	case CGame::GAMESTATE_PREPARATION:

		if (--nCount <= 0)
		{
			if (m_pNumber)
			{
				m_pNumber->Uninit();
				m_pNumber = NULL;
			}
			if (m_pBg)
			{
				m_pBg->Uninit();
				m_pBg = NULL;
			}

			nCount = 0;
			m_State = GAMESTATE_NORMAL;
		}
		else if (nCount % 60 == 0)
		{
			if (m_pNumber)
			{
				m_pNumber->SetNum(nCount / 60);
			}
		}
		break;

	case CGame::GAMESTATE_NORMAL:

		CCamera::MoveCamera(D3DXVECTOR3(SCREEN_SPEED, 0.0f, 0.0f));

		// 敵の出現タイミング
		this->EnemySpring();

		// 獲得スコア表示
		auto& aBegin = m_Scoreplus.begin();
		for (int nCnt = 0; nCnt < (int)m_Scoreplus.size(); nCnt++)
		{
			(*aBegin)->Update();

			// カウントゼロで消滅
			if ((*aBegin)->GetCount() == 0)
			{
				this->PlusUIDelete();
				aBegin = m_Scoreplus.begin();
				nCnt = 0;
			}
			else
			{
				aBegin++;
			}
		}

		CKeyboard *pKey = CManager::GetInputKeyboard();
		CPad *pPad = CManager::GetInputPad();

		// ポーズに切替
		if (pKey->GetKeyboardTrigger(DIK_P) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_START))
		{
			if (!CRenderer::GetFade())
			{
				CManager::SetMode(MODE_PAUSE);
			}
		}
#ifdef _DEBUG
		if (pKey->GetKeyboardTrigger(DIK_RETURN) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_B))
		{
			CRenderer::SetFade(MODE_RESULT);
		}
#endif
		break;
	}
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CGame::Draw()
{

}

//======================================================================================================================
// 取得したスコア表示
//======================================================================================================================
void CGame::ScorePlusUI(D3DXVECTOR3 pos, D3DXCOLOR col, int plus)
{
 	if ((int)m_Scoreplus.size() >= 5)
	{
		PlusUIDelete();
	}

	m_Scoreplus.push_back(CScorePlus::Create(pos, col, plus));
}

//======================================================================================================================
// 取得したスコア表示削除
//======================================================================================================================
void CGame::PlusUIDelete()
{
	if (!m_Scoreplus.empty())
	{
		m_Scoreplus.front()->Uninit();
		m_Scoreplus.pop_front();
	}
}

//======================================================================================================================
// 弾の種類描画処理
//======================================================================================================================
void CGame::BulletTypeDisplay()
{
	CKeyboard *pKey = CManager::GetInputKeyboard();
	CPad *pPad = CManager::GetInputPad();

	if (pKey->GetKeyboardTrigger(DIK_LSHIFT) || pKey->GetKeyboardTrigger(DIK_RSHIFT) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_Y))
	{
		for (int nCnt = 0; nCnt < BULLET_TYPE; nCnt++)
		{
			if (m_Player->GetBulletType() == nCnt)
			{
				m_pWord[nCnt]->SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				m_pWord[nCnt]->SetCollar(TYPEDISPLAY_BULLET);
			}
		}
	}
}

//======================================================================================================================
// 敵の出現処理
//======================================================================================================================
void CGame::EnemySpring()
{
	nCount++;

	if (nCount % 400 == 0)
	{
		CEnemyHeli::Create(D3DXVECTOR3(1700.0f - CCamera::GetCamera()->x, (float)CManager::Random(100, 550), 0.0f), nCount / 500.0f);
	}
	if (nCount % 230 == 0)
	{
		CEnemyVoid::Create(D3DXVECTOR3(1500.0f - CCamera::GetCamera()->x, (float)CManager::Random(100, 550), 0.0f), 2.0f);
	}
	if (nCount % 100 == 0 || nCount % 125 == 0)
	{
		CEnemyBird::Create(D3DXVECTOR3(1700.0f - CCamera::GetCamera()->x, (float)CManager::Random(100, 650), 0.0f), (float)CManager::Random(100, 200) / 100.0f);
	}
}
