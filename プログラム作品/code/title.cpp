//======================================================================================================================
//
// タイトル処理 [title.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "title.h"

#include "keyboard.h"
#include "pad.h"

#include "bg.h"
#include "word.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// メンバ変数
//======================================================================================================================

//======================================================================================================================
// 初期化
//======================================================================================================================
void CTitle::Init()
{
	m_nFadeCount = 360;

	CBg::Create(0.0005f);

	CWord::Create(CWord::WORD_TITLE, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR3(800.0f, 100.0f, 0.0f));
	CWord::Create(CWord::WORD_ENTER, D3DXVECTOR3(1000.0f, 670.0f, 0.0f), D3DXVECTOR3(550.0f, 80.0f, 0.0f));
}

//======================================================================================================================
// 終了処理
//======================================================================================================================
void CTitle::Uninit()
{

}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CTitle::Update()
{
	CKeyboard *pKey = CManager::GetInputKeyboard();
	CPad *pPad = CManager::GetInputPad();

	if (!CRenderer::GetFade())
	{
		if (pKey->GetKeyboardTrigger(DIK_RETURN) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_B))
		{
			CManager::SetSound();

			CRenderer::SetFade(MODE_TUTORIAL);
		}
		else if (--m_nFadeCount <= 0)
		{
			CRenderer::SetFade(MODE_RESULT);
		}
	}
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CTitle::Draw()
{

}
