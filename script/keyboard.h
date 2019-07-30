//===========================================================================================================
//
// 入力処理 [keyboard.h]
// Author:RYO KANDA
//
//===========================================================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//===========================================================================================================
// インクルードファイル
//===========================================================================================================
#include "main.h"
#include "input.h"

//===========================================================================================================
// マクロ定義
//===========================================================================================================
#define	NUM_KEY_MAX			(256)	// キーの最大数

//===========================================================================================================
// クラス定義
//===========================================================================================================
class CKeyboard : public CInput
{
public:
	CKeyboard();
	~CKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);
	bool GetKeyboardRepeat(int nKey);
	bool GetKeyboardRelease(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];		// キーボードのリリース情報ワーク
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート情報ワーク

	int	m_aKeyStateRepeatCnt[NUM_KEY_MAX];		// キーボードのリピートカウンタ

};

#endif
