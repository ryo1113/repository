//===========================================================================================================
//
// ゲームパッド処理 [pad.h]
// Author:RYO KANDA
//
//===========================================================================================================
#ifndef _PAD_H_
#define _PAD_H_

#define _CRT_SECURE_NO_WARNINGS

//===========================================================================================================
// インクルードファイル
//===========================================================================================================
#include "main.h"
#include "input.h"

//===========================================================================================================
// マクロ定義
//===========================================================================================================
#define	COUNT_WAIT_REPEAT	(20)	// リピート開始までの待ち時間
#define	INTERVAL_REPEAT		(1)		// リピート間隔

#define NUM_JOYPAD_MAX		(1)		// パッド最大数
#define JOY_MAX_RANGE		(1024)	// パッド有効範囲
#define	NUM_KEY_MAX			(256)	// キーの最大数

//===========================================================================================================
// クラス定義
//===========================================================================================================
class CPad : public CInput
{
public:
	CPad();
	~CPad();

	//===========================================================================================================
	// キーの種類
	//===========================================================================================================
	enum JOYPADKEY
	{
		JOYPADKEY_X = 0,			// [0] Ｘボタン
		JOYPADKEY_Y,				// [1] Ｙボタン
		JOYPADKEY_A,				// [2] Ａボタン
		JOYPADKEY_B,				// [3] Ｂボタン
		JOYPADKEY_LEFT_SHOULDER,	// [4] 左手前ボタン
		JOYPADKEY_RIGHT_SHOULDER,	// [5] 右手前ボタン
		JOYPADKEY_LEFT_TRIGGER,		// [6] 左トリガー
		JOYPADKEY_RIGHT_TRIGGER,	// [7] 右トリガー
		JOYPADKEY_LEFT_THUMB,		// [8] 左スティック
		JOYPADKEY_RIGHT_THUMB,		// [9] 右スティック
		JOYPADKEY_BACK,				// [10] バックボタン
		JOYPADKEY_START,			// [11] スタートボタン
		JOYPADKEY_LEFT,				// [12] 方向キー[左]
		JOYPADKEY_RIGHT,			// [13] 方向キー[右]
		JOYPADKEY_UP,				// [14] 方向キー[上]
		JOYPADKEY_DOWN,				// [15] 方向キー[下]
		JOYPADKEY_MAX				// [16] キーの種類数
	};

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetJoypadPress(int nIDPad, JOYPADKEY key);
	bool GetJoypadTrigger(int nIDPad, JOYPADKEY key);
	bool GetJoypadRelease(int nIDPad, JOYPADKEY key);
	bool GetJoypadRepeat(int nIDPad, JOYPADKEY key);

	int GetJoypadTriggerLeft(int nIDPad);
	int GetJoypadTriggerRight(int nIDPad);

	void GetJoypadStickLeft(int nIDPad, float *pValueH, float *pValueV);
	void GetJoypadStickRight(int nIDPad, float *pValueH, float *pValueV);

private:
	static BOOL CALLBACK EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);
	static BOOL CALLBACK EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
	void SetKeyStateJoypad(int nIDPad);

	BYTE							m_aJoyKeyState[NUM_KEY_MAX];			// ジョイパッドの入力情報ワーク
	int								m_nCountWaitRepeat = 0;					// リピート待ち時間
	int								m_nIntervalRepeat = 0;						// リピート間隔

	static LPDIRECTINPUTDEVICE8		m_apDIDevJoypad[NUM_JOYPAD_MAX];							// DirectInputオブジェクトへのポインタ
	DIJOYSTATE2						m_aJoypadState[NUM_JOYPAD_MAX];								// ジョイパッド状態を受け取るワーク
	bool							m_aKeyStateJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];				// ジョイパッドの押下状態を保持するワーク
	bool							m_aKeyStateTriggerJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];		// ジョイパッドのトリガー状態を保持するワーク
	bool							m_aKeyStateReleaseJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];		// ジョイパッドのリリース状態を保持するワーク
	bool							m_aKeyStateRepeatJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];		// ジョイパッドのリピート状態を保持するワーク
	int								m_aKeyStateRepeatCntJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];	// ジョイパッドのリピートカウンタ
	D3DXVECTOR3						m_aKeyStateAxis[NUM_JOYPAD_MAX];
	static int						m_nCntPad;
};

#endif