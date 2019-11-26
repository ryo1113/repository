//======================================================================================================================
//
// 敵の処理[enemy.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "void.h"

#include "../scene.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define PART3_OWNNUM				(10)					// 群れの数

#define VIEWING						(60)				// 死角
#define VIEW_RANGE					(150.0f)			// 視野の半径

#define SEPARATE					(D3DXToRadian(60))	// 回避する角度
#define SEPARATE_RANGE				(100.0f)			// 分離する半径

#define OBSTACLE					(D3DXToRadian(60))	// 障害物を回避する角度(最大)
#define OBSTACLE_RANGE				(150.0f)			// 障害物を検知する距離

//======================================================================================================================
// メンバ変数
//======================================================================================================================

//======================================================================================================================
// 敵の生成（群れ）
//======================================================================================================================
CEnemyVoid * CEnemyVoid::Create(D3DXVECTOR3 pos, float fspeed)
{
	CEnemyVoid *pEnemy;

	for (int nCnt = 0; nCnt < PART3_OWNNUM; nCnt++)
	{
		pEnemy = new CEnemyVoid;

		pEnemy->SetPos(pos + D3DXVECTOR3((float)CManager::Random(-100, 100), (float)CManager::Random(-100, 100), 0.0f));
		pEnemy->m_Type = CEnemyBase::ENEMYTYPE_PART3;

		pEnemy->Init();

		pEnemy->m_fSpeed = fspeed;
		pEnemy->BindTexture(m_pTexture[CEnemyBase::ENEMYTYPE_PART3]);
	}

	return pEnemy;
}

//======================================================================================================================
// 初期化処理（群れ）
//======================================================================================================================
void CEnemyVoid::Init()
{
	CEnemyBase::Init();

	SetSize(D3DXVECTOR3(40.0f, 40.0f, 0.0f));
	SetTextureAnimationTex(2, 4);

	m_move = D3DXVECTOR3((float)CManager::Random(-10, -2), (float)CManager::Random(-5, 5), 0.0f);

	m_moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//======================================================================================================================
// 更新処理（群れ）
//======================================================================================================================
void CEnemyVoid::Update()
{
	// テクスチャアニメーシ更新
	SetTextureAnimation(3);

	int nNum = 0;

	D3DXVECTOR3 avoid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 avoidDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 average = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
	{
		CScene *pScene = GetScene(OBJTYPE_ENEMY, nCnt);

		// NULL か 自分 か
		if (!pScene || GetID() == nCnt)
			continue;

		CEnemyBase *pEnemy = (CEnemyBase*)pScene;
		D3DXVECTOR3 Dif = pEnemy->GetPos() - GetPos();

		float fLength = D3DXVec3Length(&Dif);

		if (pEnemy->GetType() != ENEMYTYPE_PART3 && fLength > VIEW_RANGE)
			continue;

		CEnemyVoid *pVoid = (CEnemyVoid*)pEnemy;

		float fTargetDir = atan2f(-Dif.y, Dif.x) - m_moverot.z;
		Marginalize(&fTargetDir);

		// 視野角外
		float fDir[2] = { D3DXToRadian(-VIEWING / 2),D3DXToRadian(VIEWING / 2) };
		for (int nCnt = 0; nCnt < 2; nCnt++) Marginalize(&fDir[nCnt]);

		if (fDir[0] <= fTargetDir && fTargetDir <= fDir[1])
			continue;

		nNum++;

		pos += Dif;

 		float fDef = pVoid->m_moverot.z - m_moverot.z;
		average.z += fDef;
		Marginalize(&average.z);

		if (fLength <= SEPARATE_RANGE)
		{// 逆方向
			float fDifRot = (fTargetDir < 0.0f) ? -1.0f : 1.0f;
			float fDir = (SEPARATE_RANGE * 2.0f - fLength) / SEPARATE_RANGE;

			avoid.z += fDir * SEPARATE * fDifRot;
		}
	}

	// 回避
	for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
	{
		CScene *pScene = GetScene(OBJTYPE_ENEMY, nCnt);

		if (!pScene)
			continue;

		CEnemyBase *pEnemy = (CEnemyBase*)pScene;

		if (pEnemy->GetType() == ENEMYTYPE_PART3)
			continue;

		D3DXVECTOR3 Dif = pEnemy->GetPos() - GetPos();

		D3DXVECTOR3 check = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		D3DXVECTOR3 RotDir = D3DXVECTOR3(cosf(m_moverot.z), -sinf(m_moverot.z), 0.0f);

		check = Vec3xVec3(Dif, RotDir);
		check = Vec3xVec3(check, RotDir);

		float fLength = D3DXVec3Length(&check);

		if (fLength > pEnemy->GetLength())
			continue;

		if (D3DXVec3LengthSq(&(check - Dif)) >= OBSTACLE_RANGE * OBSTACLE_RANGE)
			continue;

		float fDisDir = atan2f(-Dif.y, Dif.x) - m_moverot.z;
		Marginalize(&fDisDir);

		float fDifRot = (fDisDir <= D3DX_PI) ? -1.0f : 1.0f;
		float fDir = (pEnemy->GetLength() - fLength) / pEnemy->GetLength() * 2.0f;

		avoidDir.z += fDir * OBSTACLE * fDifRot;
	}

	float fDir = 0.0f;

	if (0 < nNum)
	{
		// 自分を加算
		nNum++;

		// 位置
		pos /= (float)nNum;
		float fposrot = atan2f(-pos.y, pos.x) - m_moverot.z;
		Marginalize(&fposrot);
		fDir += fposrot;

		// 方向
		average.z /= (float)nNum;
		fDir += average.z;

		fDir += avoid.z;

		fDir += avoidDir.z;

	}
	else
	{
		fDir += avoidDir.z;
		m_move.x = -(m_fSpeed - 1.0f);
	}

	Marginalize(&fDir);

	m_moverot.z += fDir;
	Marginalize(&m_moverot.z);

	m_move.x += cosf(m_moverot.z) * m_fSpeed - 1.0f;
	m_move.y += -sinf(m_moverot.z) * m_fSpeed;

	CEnemyBase::Update();
}
