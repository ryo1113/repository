//======================================================================================================================
//
// 入力処理 [keyboard.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "keyboard.h"
#include "manager.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// グローバル変数
//======================================================================================================================

//======================================================================================================================
// キーボードの初期化
//======================================================================================================================
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス（キーボード）の作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
		
	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//======================================================================================================================
// キーボードの終了処理
//======================================================================================================================
void CKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//======================================================================================================================
// キーボードの更新処理
//======================================================================================================================
void CKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];			// キーボードの入力情報
	
	// デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// キートリガー情報を生成
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// キーリリース情報を生成
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];

			// キーリピート情報を生成
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if (aKeyState[nCntKey])
			{
				m_aKeyStateRepeatCnt[nCntKey]++;

				if (m_aKeyStateRepeatCnt[nCntKey] >= 20 && (m_aKeyStateRepeatCnt[nCntKey] % 10 == 0 || m_aKeyStateRepeatCnt[nCntKey] > 50))
				{
					m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCnt[nCntKey] = 0;

				m_aKeyStateRepeat[nCntKey] = 0;
			}

			m_aKeyState[nCntKey] = aKeyState[nCntKey];		//キープレス情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();				//キーボードへのアクセス権を取得
	}
}

//======================================================================================================================
// キーボードのプレス状態を取得
//======================================================================================================================
bool CKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//======================================================================================================================
// トリガーのプレス状態を取得
//======================================================================================================================
bool CKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//======================================================================================================================
// キーボードのリリ－ス状態を取得
//======================================================================================================================
bool CKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//======================================================================================================================
// キーボードのリピート状態を取得
//======================================================================================================================
bool CKeyboard::GetKeyboardRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

CKeyboard::CKeyboard()
{

}

CKeyboard::~CKeyboard()
{

}
