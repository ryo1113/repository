//======================================================================================================================
//
// ポーズ処理 [pause.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "manager.h"

class CWord;

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define PAUSE_WORD			(4)

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CPause : public CModeBase
{
public:
	CPause() {}
	~CPause() {}

	enum PAUSE_STATE
	{
		PAUSE_STATE_NONE,
		PAUSE_STATE_CONTINUE,
		PAUSE_STATE_RETRY,
		PAUSE_STATE_QUIT
	};

	static CPause *Create();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static PAUSE_STATE GetPauseState() { return m_State; }
	static void SetPauseState(PAUSE_STATE state) { m_State = state; }

private:
	CWord *m_pWord[PAUSE_WORD];

	static PAUSE_STATE m_State;
};

#endif