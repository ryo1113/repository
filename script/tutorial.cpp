//======================================================================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "tutorial.h"

#include "keyboard.h"
#include "pad.h"

#include "word.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define TRIANGLE_SIZE				(D3DXVECTOR3(50.0f, 100.0f, 0.0f))

//======================================================================================================================
// メンバ変数
//======================================================================================================================

//======================================================================================================================
// タイトル生成
//======================================================================================================================
CTutorial *CTutorial::Create()
{
	CTutorial *pTitle;

	pTitle = new CTutorial;

	pTitle->Init();

	return pTitle;
}

//======================================================================================================================
// 初期化
//======================================================================================================================
void CTutorial::Init()
{
	for (int nCnt = 0; nCnt < TUTORIAL_BG_POS; nCnt++)
	{
		m_pos[nCnt] = D3DXVECTOR3(SCREEN_WIDTH * nCnt - SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
	}

	m_nOld = 0;
	m_nNext = 0;

	for (int nCnt = 0; nCnt < TUTORIAL_BG_NUM; nCnt++)
	{
		if (nCnt >= 2)
		{
			m_pBg[nCnt] = CWord::Create(CWord::WORD_TYPE(CWord::WORD_TUTORIAL_BG_00 + nCnt), m_pos[TUTORIAL_BG_POS - 1], D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
		}
		else
		{
			m_pBg[nCnt] = CWord::Create(CWord::WORD_TYPE(CWord::WORD_TUTORIAL_BG_00 + nCnt), m_pos[nCnt + 1], D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
		}
	}

	m_Triangle[0] = CWord::Create(CWord::WORD_TRIANGLE, D3DXVECTOR3(1230.0f, 360.0f, 0.0f), TRIANGLE_SIZE);
	m_Triangle[1] = CWord::Create(CWord::WORD_TRIANGLE, D3DXVECTOR3(50.0f, 360.0f, 0.0f), TRIANGLE_SIZE);
	m_Triangle[1]->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI));

	m_pEnter = CWord::Create(CWord::WORD_ENTER, D3DXVECTOR3(1030.0f, 670.0f, 0.0f), D3DXVECTOR3(550.0f, 80.0f, 0.0f));
}

//======================================================================================================================
// 開放処理
//======================================================================================================================
void CTutorial::Uninit()
{

}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CTutorial::Update()
{
	CKeyboard *pKey = CManager::GetInputKeyboard();
	CPad *pPad = CManager::GetInputPad();

	D3DXVECTOR3 pos = m_pos[1] - m_pBg[m_nNext]->GetPos();

	// ページスライド
	if (fabsf(pos.x) <= 0.5f)
	{
		if ((pKey->GetKeyboardTrigger(DIK_D) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_RIGHT)) && m_nNext < TUTORIAL_BG_NUM - 1)
		{
			CManager::SetSound(CSound::SOUND_LABEL_SE_SELECT);
			m_nOld = m_nNext;
			m_nNext++;
		}
		else if ((pKey->GetKeyboardTrigger(DIK_A) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_LEFT)) && 0 < m_nNext)
		{
			CManager::SetSound(CSound::SOUND_LABEL_SE_SELECT);
			m_nOld = m_nNext;
			m_nNext--;
		}
	}
	else
	{
		m_pBg[m_nOld]->SetPos(m_pBg[m_nOld]->GetPos() + pos * 0.2f);
		m_pBg[m_nNext]->SetPos(m_pBg[m_nNext]->GetPos() + pos * 0.2f);

		for (int nCnt = 0; nCnt < TUTORIAL_BG_NUM; nCnt++)
		{
			if (m_nNext == nCnt || m_nOld == nCnt)
				continue;

			else if (m_nNext > nCnt)
			{
				m_pBg[nCnt]->SetPos(m_pos[0]);
			}
			else
			{
				m_pBg[nCnt]->SetPos(m_pos[TUTORIAL_BG_POS - 1]);
			}
		}
	}

	// エンター最終ページ固定
	m_pEnter->SetPos(m_pBg[TUTORIAL_BG_NUM - 1]->GetPos() + D3DXVECTOR3(350.0f, 310.0f, 0.0f));

	this->TriangleFlash();

	if ((pKey->GetKeyboardTrigger(DIK_RETURN) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_B)) && m_nNext == TUTORIAL_BG_NUM - 1)
	{
		CManager::SetSound();

		CRenderer::SetFade(CManager::MODE_GAME);
	}
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CTutorial::Draw()
{

}

//======================================================================================================================
// 三角の点滅処理
//======================================================================================================================
void CTutorial::TriangleFlash()
{
	if (m_nNext < TUTORIAL_BG_NUM - 1)
	{
		m_Triangle[0]->Flash();
	}
	else
	{
		m_Triangle[0]->SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	if (m_nNext > 0)
	{
		m_Triangle[1]->Flash();
	}
	else
	{
		m_Triangle[1]->SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}
}
