//======================================================================================================================
//
// ポーズ処理 [pause.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "pause.h"

#include "keyboard.h"
#include "pad.h"

#include "word.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define PAUSE_WORD_POS(y)			(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + y, 0.0f))
#define PAUSE_WORD_SIZE				(D3DXVECTOR3(600.0f, 100.0f, 0.0f))
#define PAUSE_SPACE					(100.0f)

#define SELECT_COLOR				(D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))
#define NO_SELECT_COLOR				(D3DXCOLOR(0.5f,0.5f,0.5f,1.0f))

//======================================================================================================================
// メンバ変数
//======================================================================================================================
CPause::PAUSE_STATE CPause::m_State = PAUSE_STATE_NONE;

//======================================================================================================================
// タイトル生成
//======================================================================================================================
CPause *CPause::Create()
{
	CPause *pPause;

	pPause = new CPause;

	pPause->Init();

	return pPause;
}

//======================================================================================================================
// 初期化
//======================================================================================================================
void CPause::Init()
{
	m_pWord[0] = CWord::Create(CWord::WORD_BLACK_BG, PAUSE_WORD_POS(0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	m_pWord[1] = CWord::Create(CWord::WORD_CONTINUE, PAUSE_WORD_POS(-PAUSE_SPACE), PAUSE_WORD_SIZE);
	m_pWord[2] = CWord::Create(CWord::WORD_RETRY, PAUSE_WORD_POS(0), PAUSE_WORD_SIZE);
	m_pWord[3] = CWord::Create(CWord::WORD_QUIT, PAUSE_WORD_POS(PAUSE_SPACE), PAUSE_WORD_SIZE);

	for (int nCnt = 1; nCnt < PAUSE_WORD; nCnt++)
	{
		m_pWord[nCnt]->SetCollar(NO_SELECT_COLOR);
	}

	m_State = PAUSE_STATE_CONTINUE;
	m_pWord[1]->SetCollar(SELECT_COLOR);
}

//======================================================================================================================
// 開放処理
//======================================================================================================================
void CPause::Uninit()
{
	for (int nCnt = 0; nCnt < PAUSE_WORD; nCnt++)
	{
		m_pWord[nCnt]->Uninit();
	}
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CPause::Update()
{
	CKeyboard *pKey = CManager::GetInputKeyboard();
	CPad *pPad = CManager::GetInputPad();

	// 選択してない色にする
	m_pWord[m_State]->SetCollar(NO_SELECT_COLOR);

	switch (m_State)
	{
	case CPause::PAUSE_STATE_CONTINUE:
		if (pKey->GetKeyboardRepeat(DIK_S) || pPad->GetJoypadRepeat(0, CPad::JOYPADKEY_DOWN))
		{// 下
			m_State = PAUSE_STATE_RETRY;
		}
		else if (pKey->GetKeyboardRepeat(DIK_W) || pPad->GetJoypadRepeat(0, CPad::JOYPADKEY_UP))
		{// 上
			m_State = PAUSE_STATE_QUIT;
		}
		break;

	case CPause::PAUSE_STATE_RETRY:
		if (pKey->GetKeyboardRepeat(DIK_S) || pPad->GetJoypadRepeat(0, CPad::JOYPADKEY_DOWN))
		{// 下
			m_State = PAUSE_STATE_QUIT;
		}
		else if (pKey->GetKeyboardRepeat(DIK_W) || pPad->GetJoypadRepeat(0, CPad::JOYPADKEY_UP))
		{// 上
			m_State = PAUSE_STATE_CONTINUE;
		}
		break;

	case CPause::PAUSE_STATE_QUIT:
		if (pKey->GetKeyboardRepeat(DIK_S) || pPad->GetJoypadRepeat(0, CPad::JOYPADKEY_DOWN))
		{// 下
			m_State = PAUSE_STATE_CONTINUE;
		}
		else if (pKey->GetKeyboardRepeat(DIK_W) || pPad->GetJoypadRepeat(0, CPad::JOYPADKEY_UP))
		{// 上
			m_State = PAUSE_STATE_RETRY;
		}
		break;
	}

	// 選択している色にする
	m_pWord[m_State]->SetCollar(SELECT_COLOR);

	if (pKey->GetKeyboardRepeat(DIK_S) || pPad->GetJoypadRepeat(0, CPad::JOYPADKEY_DOWN) || (pKey->GetKeyboardRepeat(DIK_W) || pPad->GetJoypadRepeat(0, CPad::JOYPADKEY_UP)))
	{
		CManager::SetSound(CSound::SOUND_LABEL_SE_SELECT);
	}


	if (pKey->GetKeyboardTrigger(DIK_RETURN) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_B))
	{
		if (!CRenderer::GetFade())
		{
			CManager::SetSound();

			if (m_State == PAUSE_STATE_CONTINUE)
			{
				CManager::SetMode(CManager::MODE_GAME);
			}
			else if (m_State == PAUSE_STATE_RETRY)
			{
				CRenderer::SetFade(CManager::MODE_GAME);
			}
			else if (m_State == PAUSE_STATE_QUIT)
			{
				CRenderer::SetFade(CManager::MODE_TITLE);
			}
		}
	}
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CPause::Draw()
{

}
