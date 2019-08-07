//======================================================================================================================
//
// 処理[word.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"

#include "word.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define FLASH_CONT	(40)

//======================================================================================================================
// メンバ変数
//======================================================================================================================
LPDIRECT3DTEXTURE9 CWord::m_pTexture[WORD_MAX] = {};

// コンストラクタ
CWord::CWord() : CScene2D::CScene2D(OBJTYPE_UI)
{

}

// デストラクタ
CWord::~CWord()
{

}

HRESULT CWord::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enter.png", &m_pTexture[2]); 
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking.png", &m_pTexture[3]);

	for (int nCnt = 0; nCnt < WORD_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] == NULL)
		{
			return -1;
		}
	}

	return S_OK;
}

void CWord::Unload()
{
	for (int nCnt = 0; nCnt < WORD_MAX; nCnt++)
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
CWord *CWord::Create(WORD_TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CWord *pWord;

	pWord = new CWord;

	pWord->SetPos(pos);
	pWord->SetSize(size);

	pWord->Init();
	pWord->m_Type = type;
	pWord->BindTexture(m_pTexture[type]);

	return pWord;
}

//======================================================================================================================
// 初期化
//======================================================================================================================
void CWord::Init()
{
	CScene2D::Init();

	nCntFlash = 0;
	fFlashα = 1.0f / FLASH_CONT;

	m_texpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->BindMoveTex(&m_texpos);
}

//======================================================================================================================
// 開放処理
//======================================================================================================================
void CWord::Uninit()
{
	CScene2D::Uninit();
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CWord::Update()
{
	if (this->m_Type == WORD_ENTER)
	{
		D3DXCOLOR col = GetCollar();

		col.a -= fFlashα;

		if (++nCntFlash == FLASH_CONT)
		{
			nCntFlash = 0;
			fFlashα *= -1;
		}

		SetCollar(col);
	}
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CWord::Draw()
{
	CScene2D::Draw();
}
