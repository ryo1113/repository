//======================================================================================================================
//
// 数字描画処理[number.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "renderer.h"

#include "number.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// メンバ変数
//======================================================================================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = {};

// コンストラクタ
CNumber::CNumber() : CScene::CScene(OBJTYPE_UI)
{

}

// デストラクタ
CNumber::~CNumber()
{

}

HRESULT CNumber::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number.png", &m_pTexture);

	if (m_pTexture == NULL)
	{
		return -1;
	}

	return S_OK;
}

void CNumber::Unload()
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
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CNumber *pNum;

	pNum = new CNumber;

	pNum->m_pos = pos;
	pNum->m_size = size;

	pNum->Init();

	return pNum;
}

//======================================================================================================================
// 初期化
//======================================================================================================================
void CNumber::Init()
{
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nNumber = 0;

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 赤、緑、青	、アルファ値
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(m_nNumber * 0.2f		 , m_nNumber / 5 * 0.5f);
	pVtx[1].tex = D3DXVECTOR2(m_nNumber * 0.2f + 0.2f, m_nNumber / 5 * 0.5f);
	pVtx[2].tex = D3DXVECTOR2(m_nNumber * 0.2f		 , m_nNumber / 5 * 0.5f + 0.5f);
	pVtx[3].tex = D3DXVECTOR2(m_nNumber * 0.2f + 0.2f, m_nNumber / 5 * 0.5f + 0.5f);

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//======================================================================================================================
// 終了処理
//======================================================================================================================
void CNumber::Uninit()
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	this->Release();
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CNumber::Update()
{

}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CNumber::Draw()
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void CNumber::SetNum(int num)
{
	m_nNumber = num;

	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(m_nNumber * 0.2f		 , m_nNumber / 5 * 0.5f);
	pVtx[1].tex = D3DXVECTOR2(m_nNumber * 0.2f + 0.2f, m_nNumber / 5 * 0.5f);
	pVtx[2].tex = D3DXVECTOR2(m_nNumber * 0.2f		 , m_nNumber / 5 * 0.5f + 0.5f);
	pVtx[3].tex = D3DXVECTOR2(m_nNumber * 0.2f + 0.2f, m_nNumber / 5 * 0.5f + 0.5f);

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

void CNumber::SetPos(D3DXVECTOR3 pos)
{
	m_pos += pos;

	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

void CNumber::SetColor(D3DXCOLOR col)
{
	m_col = col;

	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

D3DXCOLOR CNumber::GetColor()
{
	return m_col;
}

