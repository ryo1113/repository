//======================================================================================================================
//
// 爆発処理[explosion.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "explosion.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// プロトタイプ宣言
//======================================================================================================================

//======================================================================================================================
// メンバ変数
//======================================================================================================================
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = {};

// コンストラクタ
CExplosion::CExplosion() : CScene2D::CScene2D(OBJTYPE_EXPLOSION)
{

}

// デストラクタ
CExplosion::~CExplosion()
{

}

HRESULT CExplosion::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion.png", &m_pTexture);

	if (m_pTexture == NULL)
	{
		return -1;
	}

	return S_OK;
}

void CExplosion::Unload()
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
CExplosion *CExplosion::Create(D3DXVECTOR3 pos)
{
	CExplosion *pExplosion;

	pExplosion = new CExplosion;

	pExplosion->SetPos(pos);
	pExplosion->Init();
	pExplosion->BindTexture(m_pTexture);

	return pExplosion;
}

//======================================================================================================================
// 初期化
//======================================================================================================================
void CExplosion::Init()
{
	SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));

	CScene2D::Init();

	SetTextureAnimationTex(3, 3);
}

//======================================================================================================================
// 終了処理
//======================================================================================================================
void CExplosion::Uninit()
{
	CScene2D::Uninit();

	this->Release();
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CExplosion::Update()
{
	if (!SetTextureAnimation(5, false))
	{
		this->Uninit();
	}
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CExplosion::Draw()
{
	CScene2D::Draw();
}
