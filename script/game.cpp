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

#include "keyboard.h"
#include "pad.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define TRANSPARENT_BULLET		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f))

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

	CEnemy::Create(D3DXVECTOR3(1000.0f, 500.0f, 0.0f), CEnemy::ENEMYTYPE_PART1);
	CEnemy::Create(D3DXVECTOR3(1240.0f, 300.0f, 0.0f), CEnemy::ENEMYTYPE_PART1);

	CEnemy::Create(D3DXVECTOR3(1700.0f, 300.0f, 0.0f), CEnemy::ENEMYTYPE_PART2, 1.5f);

	CScore::Create(D3DXVECTOR3(1250.0f, 60.0f, 0.0f), D3DXVECTOR3(60.0f, 100.0f, 0.0f));

	m_pWord[0] = CWord::Create(CWord::WORD_BULLET_00, D3DXVECTOR3(100.0f, 75.0f, 0.0f), D3DXVECTOR3(150.0f, 150.0f, 0.0f));
	m_pWord[1] = CWord::Create(CWord::WORD_BULLET_01, D3DXVECTOR3(250.0f, 75.0f, 0.0f), D3DXVECTOR3(150.0f, 150.0f, 0.0f));

	CCamera::Create();

	srand((unsigned int)time(NULL));
	nCount = 0;

	m_pWord[1]->SetCollar(TRANSPARENT_BULLET);
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
	CCamera::MoveCamera(D3DXVECTOR3(-0.2f, 0.0f, 0.0f));

	nCount++;

	if (nCount % 400 == 0)
	{
		CEnemy::Create(D3DXVECTOR3(1700.0f - CCamera::GetCamera()->x, (float)CManager::random(100, 550), 0.0f), CEnemy::ENEMYTYPE_PART2, nCount / 500.0f);
	}
	if (nCount % 200 == 0)
	{
		CEnemy::Create(D3DXVECTOR3(1700.0f - CCamera::GetCamera()->x, (float)CManager::random(100, 550), 0.0f), CEnemy::ENEMYTYPE_PART3, 2.0f);
	}
	if (nCount % 100 == 0 || nCount % 125 == 0)
	{
		CEnemy::Create(D3DXVECTOR3(1700.0f - CCamera::GetCamera()->x, (float)CManager::random(100, 650), 0.0f), CEnemy::ENEMYTYPE_PART1, (float)CManager::random(100, 200) / 100.0f);
	}

	CKeyboard *pKey = CManager::GetInputKeyboard();
	CPad *pPad = CManager::GetInputPad();

	for (int nCnt = 0; nCnt < BULLET_TYPE; nCnt++)
	{
		if (pKey->GetKeyboardTrigger(DIK_LSHIFT) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_Y))
		{
			if (m_pWord[nCnt]->GetCollar().a == 1.0f)
			{
				m_pWord[nCnt]->SetCollar(TRANSPARENT_BULLET);
			}
			else if (m_pWord[nCnt]->GetCollar().a == TRANSPARENT_BULLET.a)
			{
				m_pWord[nCnt]->SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

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
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CGame::Draw()
{
	
}

//======================================================================================================================
// ゲーム状態の設定
//======================================================================================================================
void CGame::SetGameState(GAMESTATE state)
{
	g_GameState = state;
	g_nCounterGameState = 0;
}

//======================================================================================================================
// ゲーム状態の取得
//======================================================================================================================
CGame::GAMESTATE CGame::GetGameState()
{
	return g_GameState;
}

