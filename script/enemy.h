//======================================================================================================================
//
// �G�̏���[enemy.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "scene2D.h"

class CEnemy;

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define PART3_NUM					(30)

//======================================================================================================================
// �N���X��`
//======================================================================================================================
class CEnemy : public CScene2D
{
public:
	CEnemy();
	~CEnemy();

	enum ENEMYTYPE
	{
		ENEMYTYPE_PART1,
		ENEMYTYPE_PART2,
		ENEMYTYPE_PART3,
		ENEMYTYPE_MAX
	};

	enum ENEMYSTATE
	{//���L�������
		ENEMYSTATE_NORMAL = 0,			//�ʏ�
		ENEMYSTATE_DEATH,				//���񂾂Ƃ�
		ENEMYSTATE_MAX
	};

	static HRESULT Load();
	static void Unload();
	static CEnemy *Create(D3DXVECTOR3 pos, ENEMYTYPE type, float fspeed = 1.0f);

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void HitEnemy(bool bscore = true);
	static int GetNumEnemy();

private:
	void MoveSpeedEnemy();
	void HitAllEnemy();

	static LPDIRECT3DTEXTURE9 m_pTexture[];

	static int nNumEnemy;

	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;

	ENEMYTYPE m_Type;
	int m_nTime;
	float m_fSpeed;
};

#endif