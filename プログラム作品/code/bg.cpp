//======================================================================================================================
//
// 背景処理[bg.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"

#include "bg.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// プロトタイプ宣言
//======================================================================================================================

//======================================================================================================================
// メンバ変数
//======================================================================================================================
LPDIRECT3DTEXTURE9 CBg::m_pTexture = {};

// コンストラクタ
CBg::CBg() : CScene2D::CScene2D(OBJTYPE_BG)
{

}

// デストラクタ
CBg::~CBg()
{

}

HRESULT CBg::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg_00.png", &m_pTexture);

	if (m_pTexture == NULL)
	{
			return -1;
	}

	return S_OK;
}

void CBg::Unload()
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
CBg *CBg::Create(float fSpeed)
{
	CBg *pBg;

	pBg = new CBg;

	pBg->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	pBg->Init();
	pBg->BindTexture(m_pTexture);

	pBg->m_fSpeed = fSpeed;

	return pBg;
}

//======================================================================================================================
// 初期化
//======================================================================================================================
void CBg::Init()
{
	m_texpos = GetPos();
	m_move = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	CScene2D::Init();

	this->BindMoveTex(&m_texpos);
}

//======================================================================================================================
// 終了処理
//======================================================================================================================
void CBg::Uninit()
{
	CScene2D::Uninit();
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CBg::Update()
{
	MoveSpeed(m_fSpeed);
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CBg::Draw()
{
	CScene2D::Draw();
}

//======================================================================================================================
// 移動処理
//======================================================================================================================
void CBg::MoveSpeed(float fSpeed)
{
	m_texpos += m_move * fSpeed;
}
