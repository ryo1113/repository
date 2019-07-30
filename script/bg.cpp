//======================================================================================================================
//
// 処理[bg.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"

#include "bg.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define MAX_TEX		(1)

//======================================================================================================================
// プロトタイプ宣言
//======================================================================================================================

//======================================================================================================================
// メンバ変数
//======================================================================================================================
LPDIRECT3DTEXTURE9 CBg::m_pTexture[MAX_TEX] = {};

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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg_00.png", &m_pTexture[0]);

	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		if (m_pTexture[nCnt] == NULL)
		{
			return -1;
		}
	}

	return S_OK;
}

void CBg::Unload()
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
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
CBg *CBg::Create(int nType, float fSpeed)
{
	CBg *pBg;

	pBg = new CBg;

	pBg->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	pBg->Init();
	pBg->BindTexture(m_pTexture[nType]);

	pBg->SetSpeed(fSpeed);

	return pBg;
}

//======================================================================================================================
// 初期化
//======================================================================================================================
void CBg::Init()
{
	m_texpos = GetPos();
	m_move = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	CScene2D::Init();

	this->BindMoveTex(&m_texpos);
}

//======================================================================================================================
// 開放処理
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

void CBg::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

//======================================================================================================================
// 移動処理
//======================================================================================================================
void CBg::MoveSpeed(float fSpeed)
{
	m_texpos += m_move * fSpeed;
}
