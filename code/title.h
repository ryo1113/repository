//======================================================================================================================
//
// タイトル処理 [title.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "manager.h"

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CTitle : public CModeBase
{
public:
	CTitle() {}
	~CTitle() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	int m_nFadeCount;

};

#endif