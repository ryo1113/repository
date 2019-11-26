//======================================================================================================================
//
// 敵の処理[enemy.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "scene2D.h"

class CNumber;

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CEnemyBase : public CScene2D
{
public:
	CEnemyBase();
	virtual ~CEnemyBase();

	enum ENEMYTYPE
	{
		ENEMYTYPE_PART1,
		ENEMYTYPE_PART2,
		ENEMYTYPE_PART3,
		ENEMYTYPE_MAX
	};

	static HRESULT Load();
	static void Unload();

	virtual void Init();
	void Uninit();
	virtual void Update();
	void Draw();

	void HitEnemy(bool bscore = true);
	ENEMYTYPE GetType();

	static int GetNumEnemy();

protected:
	D3DXVECTOR3 Vec3xVec3(const D3DXVECTOR3 A, const D3DXVECTOR3 B);

	static LPDIRECT3DTEXTURE9 m_pTexture[];

	D3DXVECTOR3 m_move;

	ENEMYTYPE m_Type;
	int m_nTime;
	float m_fSpeed;

private:
	static int nNumEnemy;

	CNumber *m_Num[4];
};

#endif