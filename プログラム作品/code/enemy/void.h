//======================================================================================================================
//
// 敵の処理[void.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _EnemyVoid_H_
#define _EnemyVoid_H_

#include "../enemy.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CEnemyVoid : public CEnemyBase
{
public:
	CEnemyVoid() {}
	~CEnemyVoid() {}

	static CEnemyVoid *Create(D3DXVECTOR3 pos, float fspeed = 1.0f);

	void Init();
	void Update();

private:
	D3DXVECTOR3 m_moverot;

};

#endif