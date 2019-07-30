//======================================================================================================================
//
// 処理[bullet.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"

#include "bullet.h"
#include "effect.h"
#include "explosion.h"

#include "enemy.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// プロトタイプ宣言
//======================================================================================================================

//======================================================================================================================
// メンバ変数
//======================================================================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = {};
LPDIRECT3DVERTEXBUFFER9 CBullet::m_pVtxBuff = {};

CScene *CBullet::m_pBulletScene[MAX_BULLET] = {};
int CBullet::m_nNumAll = 0;

// コンストラクタ
CBullet::CBullet() : CScene2D::CScene2D(OBJTYPE_BULLET, OBJECT_BULLET)
{
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (!m_pBulletScene[nCnt])
		{
			m_pBulletScene[nCnt] = this;

			m_nID = nCnt;
			this->m_nDrawIdx = nCnt;

			m_nNumAll++;
			break;
		}
	}

	if (!m_pVtxBuff)
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	}
}

// デストラクタ
CBullet::~CBullet()
{
	if (m_nNumAll == 1)
	{
		//頂点バッファの開放
		if (m_pVtxBuff != NULL)
		{
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}
	}
	m_nNumAll--;
}

HRESULT CBullet::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet_00.png", &m_pTexture);

	if (m_pTexture == NULL)
	{
		return -1;
	}

	return S_OK;
}

void CBullet::Unload()
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//======================================================================================================================
// 生成
//======================================================================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	if (!GetShotNum())
		return NULL;

	CBullet *pBullet;

	pBullet = new CBullet;

	pBullet->m_move = move;
	pBullet->SetPos(pos);
	pBullet->Init();
	pBullet->BindTexture(m_pTexture);

	return pBullet;
}

//======================================================================================================================
// 初期化
//======================================================================================================================
void CBullet::Init()
{
	m_nCntLife = 120;

	SetSize(D3DXVECTOR3(30.0f, 30.0f, 0.0f));

	CScene2D::Init();

	SetTextureAnimationTex(1, 4);
}

//======================================================================================================================
// 開放処理
//======================================================================================================================
void CBullet::Uninit()
{
	CScene2D::Uninit();

	this->Release(m_pBulletScene);
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CBullet::Update()
{
	MoveSpeed();

	SetTextureAnimation(5, true);

	// エフェクト生成
	CEffect::Create(GetPos());


	for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
	{
		CScene *pScene = GetScene(OBJTYPE_ENEMY, nCnt);

		if (!pScene)
			continue;

		CEnemy *pEnemy = (CEnemy*)pScene;

		if (HitShapeCollision(pEnemy))
		{
			pEnemy->HitEnemy();

			this->Uninit();
		}
	}

	if (--m_nCntLife == 0 || UnlookJudge())
	{
		if (m_nCntLife == 0)
		{
			// 爆発生成
			CExplosion::Create(GetPos());
		}

		this->Uninit();
	}
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CBullet::Draw()
{

}

void CBullet::DrawAll()
{
	if (!m_pVtxBuff)
	{
		return;
	}

	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++, pVtx += 4)
	{
		if (m_pBulletScene[nCnt])
		{
			m_pBulletScene[nCnt]->Vertex(pVtx);
		}
	}

	//頂点データをアンロック
	m_pVtxBuff->Unlock();

	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (m_pBulletScene[nCnt])
		{
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}

bool CBullet::GetShotNum()
{
	if (MAX_BULLET > m_nNumAll)
		return true;

	return false;
}

//======================================================================================================================
// 移動処理
//======================================================================================================================
void CBullet::MoveSpeed()
{
	D3DXVECTOR3 pos = GetPos();

	pos += m_move;

	SetPos(pos);
}
