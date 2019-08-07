//======================================================================================================================
//
// ���U���g���� [result.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "manager.h"

//======================================================================================================================
// �N���X��`
//======================================================================================================================
class CResult : public CModeBase
{
public:
	CResult() {}
	~CResult() {}

	enum RESULTSTATE
	{//���U���g���
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