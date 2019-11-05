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
#include "enemy.h"
#include "explosion.h"
#include "player.h"

#include "word.h"
#include "score.h"
#include "number.h"

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

//======================================================================================================================
// ゲーム生成
//======================================================================================================================
CGame *CGame::Create()
{
	CGame *pGame;

	pGame = new CGame;

	pGame->Init();

	return pGame;
}

//======================================================================================================================
// 初期化処理
//======================================================================================================================
void CGame::Init()
{
	CBg::Create(0.0005f);

	CPlayer::Create();

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

		CKeyboard *pKey = CManager::GetInputKeyboard();
		CPad *pPad = CManager::GetInputPad();

		// ポーズに切替
		if (pKey->GetKeyboardTrigger(DIK_P) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_START))
		{
			if (!CRenderer::GetFade())
			{
				CManager::SetMode(CManager::MODE_PAUSE);
			}
		}
#ifdef _DEBUG
		if (pKey->GetKeyboardTrigger(DIK_RETURN) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_B))
		{
			CRenderer::SetFade(CManager::MODE_RESULT);
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
// 弾の種類描画処理
//======================================================================================================================
void CGame::BulletTypeDisplay()
{
	CKeyboard *pKey = CManager::GetInputKeyboard();
	CPad *pPad = CManager::GetInputPad();

	for (int nCnt = 0; nCnt < BULLET_TYPE; nCnt++)
	{
		if (pKey->GetKeyboardTrigger(DIK_LSHIFT) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_Y))
		{
			if (m_pWord[nCnt]->GetCollar().a == 1.0f)
			{
				m_pWord[nCnt]->SetCollar(TYPEDISPLAY_BULLET);
			}
			else if (m_pWord[nCnt]->GetCollar().a == TYPEDISPLAY_BULLET.a)
			{
				m_pWord[nCnt]->SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
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
		CEnemy::Create(D3DXVECTOR3(1700.0f - CCamera::GetCamera()->x, (float)CManager::Random(100, 550), 0.0f), CEnemy::ENEMYTYPE_PART2, nCount / 500.0f);
	}
	if (nCount % 200 == 0)
	{
		CEnemy::Create(D3DXVECTOR3(1700.0f - CCamera::GetCamera()->x, (float)CManager::Random(100, 550), 0.0f), CEnemy::ENEMYTYPE_PART3, 2.0f);
	}
	if (nCount % 100 == 0 || nCount % 125 == 0)
	{
		CEnemy::Create(D3DXVECTOR3(1700.0f - CCamera::GetCamera()->x, (float)CManager::Random(100, 650), 0.0f), CEnemy::ENEMYTYPE_PART1, (float)CManager::Random(100, 200) / 100.0f);
	}
}
