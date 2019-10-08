//===========================================================================================================
//
// レンダリング処理 [manager.h]
// Author:RYO KANDA
//
//===========================================================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//===========================================================================================================
// インクルードファイル
//===========================================================================================================
#include "main.h"
#include "sound.h"

#include <time.h>
#include <random>

class CRenderer;

class CPad;
class CKeyboard;
class CMouse;

class CPause;

//===========================================================================================================
// マクロ定義
//===========================================================================================================

//===========================================================================================================
// クラス定義
//===========================================================================================================
class CModeBase
{
public:
	CModeBase() {}
	~CModeBase() {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();

	static int random(int min, int max);

	static void SetSound(CSound::SOUND_LABEL label = CSound::SOUND_LABEL_SE_ENTER);

	static CRenderer *GetRenderer();

	static CKeyboard *GetInputKeyboard();
	static CPad *GetInputPad();
	static CMouse *GetInputMouse();

	enum MODE
	{//画面モードの種類
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_PAUSE,
		MODE_MAX
	};

	static void SetMode(MODE mode);
	static MODE GetMode();

protected:

private:
	static MODE m_Mode;				// 現在のモード
	static CModeBase *m_pMode;		// 現在のモードのポインタ
	static CPause *m_pPause;		// ポーズ用

	static CRenderer *m_pRenderer;

	static CKeyboard *m_pInputKeyboard;
	static CPad *m_pInputPad;
	static CMouse *m_pInputMouse;

	static CSound *m_pSound;
};

#endif