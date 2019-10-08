//======================================================================================================================
//
// マウス入力処理 [mouse.cpp]
// Author:RYO KANDA
//
//======================================================================================================================

//======================================================================================================================
// インクルードファイル
//======================================================================================================================
#include "mouse.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

// コンストラクタ
CMouse::CMouse()
{

}

// デストラクタ
CMouse::~CMouse()
{

}

//======================================================================================================================
// マウスの初期化処理
//======================================================================================================================
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL);
	m_pDevice->SetDataFormat(&c_dfDIMouse2); // マウス用のデータ・フォーマットを設定

	m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// デバイスの設定    
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）    
	m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// 入力制御開始    
	m_pDevice->Acquire();

	// 代入
	m_hWnd = hWnd;

	return S_OK;
}

//======================================================================================================================
// マウスの終了処理
//======================================================================================================================
void CMouse::Uninit()
{
	CInput::Uninit();
}

//======================================================================================================================
// マウスの更新処理
//======================================================================================================================
void CMouse::Update()
{
	// 変数宣言
	POINT point;

	// 値の初期化    
	m_mouse.mouseState.rgbButtons[0] = 0;

	GetCursorPos(&point);
	ScreenToClient(m_hWnd, &point);

	m_mouse.pos.x = (float)point.x;
	m_mouse.pos.y = (float)point.y;

	// 値の更新 
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_mouse.mouseState)))
	{
		for (int nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
		{
			// 排他的論理和かどうか判断
			m_aButtonStateTrigger[nCntButton] = (m_aButtonState[nCntButton] ^ m_mouse.mouseState.rgbButtons[nCntButton]) & m_mouse.mouseState.rgbButtons[nCntButton];

			// キープレス情報保存
			m_aButtonState[nCntButton] = m_mouse.mouseState.rgbButtons[nCntButton];	
		}
	}
	else
	{
		m_pDevice->Acquire(); // １発目や２発目にエラーが出るが無視してよい。 
	}
}

//======================================================================================================================
// マウスのプレス状態を取得
//======================================================================================================================
bool CMouse::GetMousePress(MOUSEBUTTON nButton)
{
	return (m_mouse.mouseState.rgbButtons[nButton] & 0x80) ? true : false;
}

//======================================================================================================================
// マウスのトリガー処理
//======================================================================================================================
bool CMouse::GetMousePressTrigger(MOUSEBUTTON nButton)
{
	return (m_aButtonStateTrigger[nButton] & 0x80) ? true : false;
}

//======================================================================================================================
// マウスのX座標の取得
//======================================================================================================================
LONG CMouse::GetMouseX()
{
	return (LONG)m_mouse.pos.x;
}

//======================================================================================================================
// マウスのY座標の取得
//======================================================================================================================
LONG CMouse::GetMouseY()
{
	return (LONG)m_mouse.pos.y;
}