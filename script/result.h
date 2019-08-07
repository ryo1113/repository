//======================================================================================================================
//
// リザルト処理 [result.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "manager.h"

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CResult : public CModeBase
{
public:
	CResult() {}
	~CResult() {}

	enum RESULTSTATE
	{//リザルト画面
		RESULTSTATE_WIN = 0,
		RESULTSTATE_LOSE,
		RESULTSTATE_MAX
	};

	static CResult *Create();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:

	void SetResultState(RESULTSTATE state);
	RESULTSTATE GetResultState(void);

	RESULTSTATE g_ResultState = RESULTSTATE_MAX;
	int m_FadeCount;
};

#endif