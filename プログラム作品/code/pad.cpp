//======================================================================================================================
//
// ゲームパッド処理 [pad.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "pad.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// メンバ変数初期化
//======================================================================================================================
LPDIRECTINPUTDEVICE8 CPad::m_apDIDevJoypad[NUM_JOYPAD_MAX] = {};
int CPad::m_nCntPad = 0;
int CPad::m_nIntervalRepeat = INTERVAL_REPEAT;

//======================================================================================================================
// 入力処理の初期化
//======================================================================================================================
HRESULT CPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (m_pInput == NULL)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);

		m_nCountWaitRepeat = COUNT_WAIT_REPEAT;
	}

	for (m_nCntPad = 0; m_nCntPad < NUM_JOYPAD_MAX; m_nCntPad++)
	{
		hr = m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallbackJoypad, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || m_apDIDevJoypad[m_nCntPad] == NULL)
		{
			MessageBox(hWnd, "ジョイパッドがありません", "警告", MB_ICONWARNING);// 確認用
			return hr;
		}

		// データフォーマットを設定
		hr = m_apDIDevJoypad[m_nCntPad]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ジョイパッドのデータフォーマットを設定できませんでした。", "警告", MB_ICONWARNING);
			return hr;
		}

		// 協調モードを設定（フォアグラウンド＆非排他モード）
		hr = m_apDIDevJoypad[m_nCntPad]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ジョイパッドの協調モードを設定できませんでした。", "警告", MB_ICONWARNING);
			return hr;
		}

		hr = m_apDIDevJoypad[m_nCntPad]->EnumObjects(EnumAxesCallbackJoypad, NULL, DIDFT_AXIS);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ジョイパッドが見つかりませんでした", "警告", MB_ICONWARNING);
			return hr;
		}

		// デバイスの設定
		DIPROPRANGE diprg;

		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);

		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

		// Ｘ方向の範囲を指定
		m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

		// Ｙ方向の範囲を指定
		m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		diprg.diph.dwObj = DIJOFS_Z;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

		// Ｚ方向の範囲を指定
		m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		diprg.diph.dwObj = DIJOFS_RZ;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

		// ＲＺ方向の範囲を指定
		m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// キーボードへのアクセス権を獲得(入力制御開始)
		m_apDIDevJoypad[m_nCntPad]->Acquire();

		// ワーククリア
		memset(&m_aJoypadState[m_nCntPad], 0, sizeof m_aJoypadState[m_nCntPad]);
		memset(m_aKeyStateJoypad[m_nCntPad], 0, sizeof m_aKeyStateJoypad[m_nCntPad]);
		memset(m_aKeyStateTriggerJoypad[m_nCntPad], 0, sizeof m_aKeyStateTriggerJoypad[m_nCntPad]);
		memset(m_aKeyStateReleaseJoypad[m_nCntPad], 0, sizeof m_aKeyStateReleaseJoypad[m_nCntPad]);
		memset(m_aKeyStateRepeatJoypad[m_nCntPad], 0, sizeof m_aKeyStateRepeatJoypad[m_nCntPad]);
	}

	return hr;
}

//======================================================================================================================
// 入力処理の終了処理
//======================================================================================================================
void CPad::Uninit(void)
{
	CInput::Uninit();

	// ジョイパッドの終了処理
	for (int nCntJoypad = 0; nCntJoypad < NUM_JOYPAD_MAX; nCntJoypad++)
	{
		if (m_apDIDevJoypad[nCntJoypad] != NULL)
		{// デバイスオブジェクトの開放
			m_apDIDevJoypad[nCntJoypad]->Release();
			m_apDIDevJoypad[nCntJoypad] = NULL;
		}
	}
}

//======================================================================================================================
// 入力処理の更新処理
//======================================================================================================================
void CPad::Update(void)
{
	HRESULT hr;
	bool aKeyStateOld[JOYPADKEY_MAX];

	for (int nCntJoypad = 0; nCntJoypad < NUM_JOYPAD_MAX; nCntJoypad++)
	{
		if (m_apDIDevJoypad[nCntJoypad] == NULL)
		{
			return;
		}

		// 前回のデータを保存
		for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
		{
			aKeyStateOld[nCntKey] = m_aKeyStateJoypad[nCntJoypad][nCntKey];
		}

		// デバイスからデータを取得
		hr = m_apDIDevJoypad[nCntJoypad]->GetDeviceState(sizeof(m_aJoypadState[nCntJoypad]), &m_aJoypadState[nCntJoypad]);

		if (SUCCEEDED(hr))
		{
			// キー情報設定
			SetKeyStateJoypad(nCntJoypad);

			//	軸位置を記録
			m_aKeyStateAxis[nCntJoypad].x = (float)m_aJoypadState[nCntJoypad].lX / JOY_MAX_RANGE;
			m_aKeyStateAxis[nCntJoypad].y = (float)m_aJoypadState[nCntJoypad].lY / JOY_MAX_RANGE;
			m_aKeyStateAxis[nCntJoypad].z = (float)m_aJoypadState[nCntJoypad].lZ / JOY_MAX_RANGE;

			for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
			{
				// トリガー情報の作成
				m_aKeyStateTriggerJoypad[nCntJoypad][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateJoypad[nCntJoypad][nCntKey]) & m_aKeyStateJoypad[nCntJoypad][nCntKey];

				// リリース情報の作成
				m_aKeyStateReleaseJoypad[nCntJoypad][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateJoypad[nCntJoypad][nCntKey]) & !m_aKeyStateJoypad[nCntJoypad][nCntKey];

				// リピート情報の作成
				m_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = m_aKeyStateTriggerJoypad[nCntJoypad][nCntKey];

				if (m_aKeyStateJoypad[nCntJoypad][nCntKey])
				{
					m_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey]++;

					if (((m_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] - m_nCountWaitRepeat) % m_nIntervalRepeat) == 0 && (m_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] % 10 == 0 || m_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] > 50))
					{
						m_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = m_aKeyStateJoypad[nCntJoypad][nCntKey];
					}
				}
				else
				{
					m_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] = 0;
					m_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = 0;
				}
			}
		}
		else
		{
			m_apDIDevJoypad[nCntJoypad]->Acquire();
		}
	}
}

//======================================================================================================================
// ジョイスティック問い合わせ用コールバック関数
//======================================================================================================================
BOOL CALLBACK CPad::EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	static GUID pad_discrimination[NUM_JOYPAD_MAX];	// 各デバイスの識別子を格納
	DIDEVCAPS	diDevCaps;							// デバイス情報
	HRESULT		hRes;

	for (int nCnt = 0; nCnt < NUM_JOYPAD_MAX; nCnt++)
	{
		if (pad_discrimination[nCnt] == lpddi->guidInstance)
		{
			return DIENUM_CONTINUE;			// 列挙を続ける
		}
	}

	// ジョイスティック用デバイスオブジェクトを作成
	hRes = m_pInput->CreateDevice(lpddi->guidInstance, &m_apDIDevJoypad[m_nCntPad], NULL);
	if (FAILED(hRes))
	{
		return DIENUM_CONTINUE;			// 列挙を続ける
	}

	// ジョイスティックの能力を調べる
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hRes = m_apDIDevJoypad[m_nCntPad]->GetCapabilities(&diDevCaps);
	if (FAILED(hRes))
	{
		if (m_apDIDevJoypad[m_nCntPad])
		{
			m_apDIDevJoypad[m_nCntPad]->Release();
			m_apDIDevJoypad[m_nCntPad] = NULL;
		}
		return DIENUM_CONTINUE;			// 列挙を続ける
	}

	// デバイスの識別子を保存
	pad_discrimination[m_nCntPad] = lpddi->guidInstance;

	return DIENUM_STOP;		// このデバイスを使うので列挙を終了する
}

BOOL CALLBACK CPad::EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	HRESULT hr;

	DIPROPRANGE diprg;

	// スティック軸の値の範囲を設定（-32768～32767）
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -32768;
	diprg.lMax = 32768;

	if (m_apDIDevJoypad[m_nCntPad] != NULL)
	{
		hr = m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (FAILED(hr))
		{
			return DIENUM_STOP;
		}
	}

	return DIENUM_CONTINUE;
}

//======================================================================================================================
// ジョイパッドのキー情報設定
//======================================================================================================================
void CPad::SetKeyStateJoypad(int nIDPad)
{
	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 225 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 315 * 100)
	{// 十字キー[左]が押されている
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = false;
	}

	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 45 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 135 * 100)
	{// 十字キー[右]が押されている
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = false;
	}

	if ((m_aJoypadState[nIDPad].rgdwPOV[0] >= 315 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 360 * 100) || (m_aJoypadState[nIDPad].rgdwPOV[0] >= 0 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 45 * 100))
	{// 十字キー[上]が押されている
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = false;
	}

	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 135 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 225 * 100)
	{// 十字キー[下]が押されている
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = false;
	}

	for (int nKey = JOYPADKEY_X; nKey <= JOYPADKEY_START; nKey++)
	{
		if (m_aJoypadState[nIDPad].rgbButtons[nKey])
		{// ボタンが押されている
			m_aKeyStateJoypad[nIDPad][nKey] = true;
		}
		else
		{
			m_aKeyStateJoypad[nIDPad][nKey] = false;
		}
	}
}

//======================================================================================================================
// ジョイパッド情報取得＿長押し
//======================================================================================================================
bool CPad::GetJoypadPress(int nIDPad, JOYPADKEY key)
{
	return m_aKeyStateJoypad[nIDPad][key];
}

//======================================================================================================================
// ジョイパッド情報取得＿押す
//======================================================================================================================
bool CPad::GetJoypadTrigger(int nIDPad, JOYPADKEY key)
{
	return m_aKeyStateTriggerJoypad[nIDPad][key];
}

//======================================================================================================================
// ジョイパッド情報取得＿離す
//======================================================================================================================
bool CPad::GetJoypadRelease(int nIDPad, JOYPADKEY key)
{
	return m_aKeyStateReleaseJoypad[nIDPad][key];
}

//======================================================================================================================
// ジョイパッド情報取得＿繰り返す
//======================================================================================================================
bool CPad::GetJoypadRepeat(int nIDPad, JOYPADKEY key)
{
	return m_aKeyStateRepeatJoypad[nIDPad][key];
}

//======================================================================================================================
// ジョイパッドのトリガー(左)の値取得
//======================================================================================================================
int CPad::GetJoypadTriggerLeft(int nIDPad)
{
	return m_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_LEFT_TRIGGER];
}

//======================================================================================================================
// ジョイパッドのトリガー(右)の値取得
//======================================================================================================================
int CPad::GetJoypadTriggerRight(int nIDPad)
{
	return m_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_RIGHT_TRIGGER];
}

//======================================================================================================================
// ジョイパッドのアナログスティック(左)の値取得
//======================================================================================================================
void CPad::GetJoypadStickLeft(int nIDPad, float *pValueH, float *pValueV)
{
	*pValueH = (float)m_aJoypadState[nIDPad].lX;
	*pValueV = (float)-m_aJoypadState[nIDPad].lY;
}

//======================================================================================================================
// ジョイパッドのアナログスティック(右)の値取得
//======================================================================================================================
void CPad::GetJoypadStickRight(int nIDPad, float *pValueH, float *pValueV)
{
	*pValueH = (float)m_aJoypadState[nIDPad].lZ;
	*pValueV = (float)-m_aJoypadState[nIDPad].lRz;
}

// コンストラクタ
CPad::CPad()
{

}

// デストラクタ
CPad::~CPad()
{

}
