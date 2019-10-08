//======================================================================================================================
//
// 処理[effect.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "effect.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define BULLET_LIFE			(10)

//======================================================================================================================
// メンバ変数
//======================================================================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = {};
LPDIRECT3DVERTEXBUFFER9 CEffect::m_pVtxBuff = {};

CScene *CEffect::m_pEffectScene[MAX_EFFECT] = {};
int CEffect::m_nNumAll = 0;

// コンストラクタ
CEffect::CEffect() : CScene2D::CScene2D(OBJTYPE_EFFECT, OBJECT_EFFECT)
{
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (!m_pEffectScene[nCnt])
		{
			m_pEffectScene[nCnt] = this;

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
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	}
}

// デストラクタ
CEffect::~CEffect()
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

HRESULT CEffect::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_00.jpg", &m_pTexture);

	if (m_pTexture == NULL)
	{
		return -1;
	}

	return S_OK;
}

void CEffect::Unload()
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
CEffect *CEffect::Create(D3DXVECTOR3 pos, EFFECTTYPE type)
{
	if (MAX_EFFECT <= m_nNumAll)
		return NULL;

	CEffect *pEffect;

	pEffect = new CEffect;

	pEffect->SetPos(pos);
	pEffect->m_type = type;
	pEffect->Init();
	pEffect->BindTexture(NULL);

	return pEffect;
}

//======================================================================================================================
// 初期化
//======================================================================================================================
void CEffect::Init()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = BULLET_LIFE;

	CScene2D::Init();

	switch (m_type)
	{
	case CEffect::EFFECTTYPE_NONE:

		SetSize(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

		SetCollar(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		break;

	case CEffect::EFFECTTYPE_LASER:

		SetSize(D3DXVECTOR3(15.0f, 15.0f, 0.0f));

		SetCollar(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		break;
	}
}

//======================================================================================================================
// 開放処理
//======================================================================================================================
void CEffect::Uninit()
{
	CScene2D::Uninit();

	this->Release(m_pEffectScene);
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CEffect::Update()
{
	switch (m_type)
	{
	case CEffect::EFFECTTYPE_NONE:

		SetSize(GetSize() + D3DXVECTOR3(-0.7f, -0.7f, 0.0f));

		if (GetSize().x < 0.1f || GetSize().y < 0.1f)
		{
			this->Uninit();
		}

		break;

	case CEffect::EFFECTTYPE_LASER:

		if (--m_nLife <= 0)
		{
			this->Uninit();
		}

		break;
	}
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CEffect::Draw()
{

}

void CEffect::DrawAll()
{
	if (!m_pVtxBuff)
	{
		return;
	}

	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++, pVtx += 4)
	{
		if (m_pEffectScene[nCnt])
		{
			m_pEffectScene[nCnt]->Vertex(pVtx);
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

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);				// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定							//JPG用
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);				// αディスティネイションカラーの指定 

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (m_pEffectScene[nCnt])
		{
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	// レンダーステート(通常ブレンド処理)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DBLENDOP_ADD);	// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定						//テクスチャ背景なし３行
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αディスティネイションカラーの指定
}
