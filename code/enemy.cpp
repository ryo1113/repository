//======================================================================================================================
//
// 敵の処理[enemy.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "enemy.h"

#include "game.h"
#include "camera.h"

#include "bullet.h"
#include "explosion.h"
#include "player.h"
#include "score.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define PART3_OWNNUM				(10)

#define VIEWING						(60)				// 死角
#define VIEW_RANGE					(150.0f)			// 視野の半径

#define SEPARATE					(D3DXToRadian(60))	// 回避する角度
#define SEPARATE_RANGE				(100.0f)			// 分離する半径

#define OBSTACLE					(D3DXToRadian(60))	// 障害物を回避する角度(最大)
#define OBSTACLE_RANGE				(360.0f)			// 障害物を検知する角度

//======================================================================================================================
// メンバ変数
//======================================================================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[ENEMYTYPE_MAX] = {};
int CEnemy::nNumEnemy = 0;


// コンストラクタ
CEnemy::CEnemy() : CScene2D::CScene2D(OBJTYPE_ENEMY)
{
	nNumEnemy++;
}

// デストラクタ
CEnemy::~CEnemy()
{

}

HRESULT CEnemy::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_00.png", &m_pTexture[ENEMYTYPE_PART1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_01.png", &m_pTexture[ENEMYTYPE_PART2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_02.png", &m_pTexture[ENEMYTYPE_PART3]);

	for (int nCnt = 0; nCnt < ENEMYTYPE_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] == NULL)
		{
			return -1;
		}
	}

	return S_OK;
}

void CEnemy::Unload()
{
	for (int nCnt = 0; nCnt < ENEMYTYPE_MAX; nCnt++)
	{
		//テクスチャの開放
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//======================================================================================================================
// 生成
//======================================================================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, ENEMYTYPE type, float fspeed)
{
	CEnemy *pEnemy;

	switch (type)
	{
	case CEnemy::ENEMYTYPE_PART3:

		for (int nCnt = 0; nCnt < PART3_OWNNUM; nCnt++)
		{
			pEnemy = new CEnemy;

			pEnemy->SetPos(pos + D3DXVECTOR3((float)CManager::Random(-100, 100), (float)CManager::Random(-100, 100), 0.0f));
			pEnemy->m_Type = type;
				  
			pEnemy->Init();
				  
			pEnemy->m_fSpeed = fspeed;
			pEnemy->BindTexture(m_pTexture[type]);
		}

		break;

	default:

		pEnemy = new CEnemy;
		pEnemy->SetPos(pos);
		pEnemy->m_Type = type;

		pEnemy->Init();

		pEnemy->m_fSpeed = fspeed;
		pEnemy->BindTexture(m_pTexture[type]);

		break;
	}

	return pEnemy;
}

//======================================================================================================================
// 初期化
//======================================================================================================================
void CEnemy::Init()
{
	CScene2D::Init();

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (m_Type)
	{
	case CEnemy::ENEMYTYPE_PART1:
		SetSize(D3DXVECTOR3(75.0f, 100.0f, 0.0f));
		SetTextureAnimationTex(2, 4);
		break;

	case CEnemy::ENEMYTYPE_PART2:
		SetSize(D3DXVECTOR3(300.0f, 150.0f, 0.0f));
		break;

	case CEnemy::ENEMYTYPE_PART3:
		SetSize(D3DXVECTOR3(40.0f, 40.0f, 0.0f));
		SetTextureAnimationTex(2, 4);

		m_move = D3DXVECTOR3((float)CManager::Random(-10, -2), (float)CManager::Random(-5, 5), 0.0f);
	}

	m_nTime = rand() % 360;
}

//======================================================================================================================
// 開放処理
//======================================================================================================================
void CEnemy::Uninit()
{
	nNumEnemy--;

	CScene2D::Uninit();

	this->Release();
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CEnemy::Update()
{
	// テクスチャアニメーシ更新
	if (m_Type == ENEMYTYPE_PART1)
	{
		SetTextureAnimation(5);
	}
	else if (m_Type == ENEMYTYPE_PART3)
	{
		SetTextureAnimation(3);
	}

	if (m_Type == ENEMYTYPE_PART2)
	{// バードストライク
		HitAllEnemy();
	}

	// 移動更新
	D3DXVECTOR3 pos = GetPos();

	MoveSpeedEnemy();

	m_move.x -= m_move.x * 1 / 4;
	m_move.y -= m_move.y * 1 / 4;

	if (fabsf(m_move.x) < 0.1f)
	{
		m_move.x = 0;
	}
	if (fabsf(m_move.y) < 0.1f)
	{
		m_move.y = 0;
	}

	SetPos(pos + m_move);

	if (GetPos().x + GetSize().x / 1.5f <= 0.0f - CCamera::GetCamera()->x || fabsf(GetPos().y - SCREEN_HEIGHT / 2) >= SCREEN_HEIGHT)
	{// 画面外（左）
		Uninit();
	}
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CEnemy::Draw()
{
	CScene2D::Draw();
}

//======================================================================================================================
// 被弾処理
//======================================================================================================================
void CEnemy::HitEnemy(bool bscore)
{
	// 爆発生成
	CExplosion::Create(GetPos());
	CManager::SetSound(CSound::SOUND_LABEL_SE_ENEMYHIT);

	if (bscore)
	{// 加算スコア
		
		CNumber *pScore = CScore::GetScore();

		switch (m_Type)
		{
		case CEnemy::ENEMYTYPE_PART1:
			((CScore*)pScore)->AddScore(3000);
			break;

		case CEnemy::ENEMYTYPE_PART2:
			((CScore*)pScore)->AddScore(5000);
			break;

		case CEnemy::ENEMYTYPE_PART3:
			((CScore*)pScore)->AddScore(500);
			break;
		}
	}

	this->Uninit();
}

//======================================================================================================================
// 移動処理
//======================================================================================================================
void CEnemy::MoveSpeedEnemy()
{
	m_nTime++;

	switch (m_Type)
	{
	case ENEMYTYPE_PART1:
		m_move += D3DXVECTOR3(-1.0f, cos(m_nTime * D3DX_PI / 30.0f) * 0.5f, 0.0f) * m_fSpeed;
		break;

	case ENEMYTYPE_PART2:
		m_move += D3DXVECTOR3(-3.0f, cos(m_nTime * D3DX_PI / 90.0f) * 1.0f, 0.0f) * m_fSpeed;
		break;

	case ENEMYTYPE_PART3:

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

			CEnemy *pEnemy = (CEnemy*)pScene;
			D3DXVECTOR3 Dif = pEnemy->GetPos() - GetPos();

			float fLength = D3DXVec3Length(&Dif);

			if (pEnemy->m_Type != ENEMYTYPE_PART3 || fLength > VIEW_RANGE)
				continue;

			float fTargetDir = atan2f(-Dif.y, Dif.x) - m_rot.z;
			Marginalize(&fTargetDir);

			// 視野角外
			float fDir[2] = { D3DXToRadian(- VIEWING / 2),D3DXToRadian( VIEWING / 2) };
			for (int nCnt = 0; nCnt < 2; nCnt++) Marginalize(&fDir[nCnt]);

			if (fDir[0] <= fTargetDir && fTargetDir <= fDir[1])
				continue;

			nNum++;

			pos += Dif;

			float fDef = pEnemy->m_rot.z - m_rot.z;
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

			CEnemy *pEnemy = (CEnemy*)pScene;

			if (pEnemy->m_Type == ENEMYTYPE_PART3)
				continue;

			D3DXVECTOR3 Dif = pEnemy->GetPos() - GetPos();

			D3DXVECTOR3 check = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			D3DXVECTOR3 RotDir = D3DXVECTOR3(cosf(m_rot.z), -sinf(m_rot.z), 0.0f);
			
			check = Vec3xVec3(Dif, RotDir);
			check = Vec3xVec3(check, RotDir);

			float fLength = D3DXVec3Length(&check);

			if (fLength > pEnemy->GetLength())
				continue;

			if (D3DXVec3LengthSq(&(check - Dif)) >= OBSTACLE_RANGE * OBSTACLE_RANGE)
				continue;

			float fDisDir = atan2f(-Dif.y, Dif.x) - m_rot.z;
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
			float fposrot = atan2f(-pos.y, pos.x) - m_rot.z;
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
			fDir = avoidDir.z;
		}

		while ((fDir < -D3DX_PI) || (fDir > D3DX_PI))
		{
			Marginalize(&fDir);
		}

		m_rot.z += fDir;
		Marginalize(&m_rot.z);

		m_move.x += cosf(m_rot.z) * m_fSpeed - 1.0f;
		m_move.y += -sinf(m_rot.z) * m_fSpeed;

		break;
	}
}

//======================================================================================================================
// ヘリと全て
//======================================================================================================================
void CEnemy::HitAllEnemy()
{
	// 敵に当たる
	for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
	{
		CScene *pScene = GetScene(OBJTYPE_ENEMY, nCnt);

		if (!pScene)
			continue;

		CEnemy *pEnemy = (CEnemy*)pScene;

		if(pEnemy->m_Type == ENEMYTYPE_PART2)
			continue;

		if (HitShapeCollision(pEnemy))
		{
			pEnemy->HitEnemy(false);

			break;
		}
	}

	// 自機に当たる
	for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
	{
		CScene *pScene = GetScene(OBJTYPE_PLAYER, nCnt);

		if (!pScene)
			continue;

		CPlayer *pPlayer = (CPlayer*)pScene;

		if (HitBoxCollision(pPlayer))
		{
			pPlayer->HitPlayer();

			break;
		}
	}
}

//======================================================================================================================
// ベクター３のそれぞれの掛け算
//======================================================================================================================
D3DXVECTOR3 CEnemy::Vec3xVec3(D3DXVECTOR3 A, D3DXVECTOR3 B)
{
	D3DXVECTOR3 Vec;

	Vec.x = A.x * B.x;
	Vec.y = A.y * B.y;
	Vec.z = A.z * B.z;

	return Vec;
}

//======================================================================================================================
// 敵数の取得
//======================================================================================================================
int CEnemy::GetNumEnemy()
{
	return nNumEnemy;
}
