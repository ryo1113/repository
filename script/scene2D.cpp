//======================================================================================================================
//
// 処理 [scene.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "scene2D.h"
#include "manager.h"

#include "bullet.h"

#include "camera.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// 構造体定義
//======================================================================================================================

//======================================================================================================================
// プロトタイプ宣言
//======================================================================================================================

//======================================================================================================================
// メンバ変数
//======================================================================================================================

// コンストラクタ
CScene2D::CScene2D(OBJTYPE objtype, OBJECT obj) : CScene::CScene(objtype, obj)
{
	if (obj == OBJECT_NONE)
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	}
}

// デストラクタ
CScene2D::~CScene2D()
{

}

CScene2D *CScene2D::Create()
{
	CScene2D *pScene;

	pScene = new CScene2D;

	pScene->Init();

	return pScene;
}

//======================================================================================================================
// 初期化処理
//======================================================================================================================
void CScene2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 0.0f);

	m_rotDifference = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_fLength = sqrtf((m_size.x / 2 * m_size.x / 2) + (m_size.y / 2 * m_size.y / 2));

	//テクスチャアニメーション管理変数
	m_nHeight = 1;
	m_nWide = 1;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	this->pMoveTex = NULL;

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 赤、緑、青	、アルファ値
}


//======================================================================================================================
// 終了処理
//======================================================================================================================
void CScene2D::Uninit()
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CScene2D::Update()
{

}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CScene2D::Draw()
{
	SetVertex();

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

//======================================================================================================================
// 当たり判定
//======================================================================================================================
bool CScene2D::HitShapeCollision(CScene2D *scene2D)
{	
	// 半径
	float fRadiusA = (m_size.x * m_scale.x + m_size.y * m_scale.y) / 4.0f;

	float fRadiusB;

	// テクスチャの余白調節のため
	if (scene2D->m_size.x > scene2D->m_size.y)
	{
		fRadiusB = (scene2D->m_size.y * scene2D->m_scale.y) / 2.0f;
	}
	else
	{
		fRadiusB = (scene2D->m_size.x * scene2D->m_scale.x) / 2.0f;
	}

	float fRadius = fRadiusA + fRadiusB;

	if (D3DXVec3Dot(&(m_pos - scene2D->m_pos), &(m_pos - scene2D->m_pos)) > fRadius * fRadius)
	{// 半径+半径より遠いいか
		return false;
	}

	return true;
}

//======================================================================================================================
// 向こうとする向き
//======================================================================================================================
void CScene2D::Rot()
{
	// 向くまでの角度
	m_rotDifference.z = m_rotDirection.z - m_rot.z;

	// 180度以上回転の時
	Marginalize(&m_rotDifference.z);

	// 向く方向にする
	if (fabsf(m_rotDifference.z) <= 0.05f)
	{
		m_rot.z = m_rotDirection.z;
	}
	else
	{
		m_rot.z += m_rotDifference.z * 0.05f;
	}

	// 360度以上回転しないように(数字的に)
	Marginalize(&m_rot.z);
}

//======================================================================================================================
// 回転の限界(遠回りしないよう)
//======================================================================================================================
void CScene2D::Marginalize(float *frot)
{
	if (*frot > D3DX_PI)
	{
		*frot -= D3DX_PI * 2;
	}
	if (*frot < -D3DX_PI)
	{
		*frot += D3DX_PI * 2;
	}
}

//======================================================================================================================
// 見えないとこで消す
//======================================================================================================================
bool CScene2D::UnlookJudge()
{
	if (m_pos.x + m_fLength * m_scale.x < 0.0f ||
		m_pos.x - m_fLength * m_scale.x > SCREEN_WIDTH ||
		m_pos.y + m_fLength * m_scale.y < 0.0f ||
		m_pos.y - m_fLength * m_scale.y > SCREEN_HEIGHT)
	{
		return true;
	}
	return false;
}

//======================================================================================================================
// キーボード移動
//======================================================================================================================
D3DXVECTOR3 CScene2D::MoveVector(D3DXVECTOR3 vector, float fSpeed)
{
	D3DXVECTOR3 NormalizeMove;

	//正規化する
	D3DXVec3Normalize(&NormalizeMove, &vector);

	//進む距離
	NormalizeMove *= fSpeed;

	return NormalizeMove;
}

//======================================================================================================================
// ゲームパッド移動
//======================================================================================================================
D3DXVECTOR3 CScene2D::Move(float fAngle, float fSpeed)
{
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ラジアンでの計算
	move.x += sinf(fAngle) * fSpeed;
	move.y -= cosf(fAngle) * fSpeed;

	return move;
}

//======================================================================================================================
// マウス移動
//======================================================================================================================
D3DXVECTOR3 CScene2D::MoveMouse(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 move = pos - m_pos;

	return move;
}

//======================================================================================================================
// Set
//======================================================================================================================
void CScene2D::SetCollar(D3DXCOLOR collar)
{
	this->m_col = collar;
}

//------------------------------------------------------------------------------
// テクスチャ
//------------------------------------------------------------------------------
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 Texture)
{
	m_pTexture = Texture;
}

//------------------------------------------------------------------------------
// 頂点座標更新
//------------------------------------------------------------------------------
void CScene2D::Vertex(VERTEX_2D *pVtx)
{
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = m_col;	// 赤、緑、青	、アルファ値
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	if (this->pMoveTex)
	{
		//頂点の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI),
								  m_pos.y + (m_fLength * m_scale.y) * cosf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI), 0.0f);

		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI),
								  m_pos.y + (m_fLength * m_scale.y) * cosf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI), 0.0f);
		
		pVtx[2].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z),
								  m_pos.y + (m_fLength * m_scale.y) * cosf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z), 0.0f);

		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z),
								  m_pos.y + (m_fLength * m_scale.y) * cosf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z), 0.0f);

		this->SetVertexTex_Scroll(pVtx);
	}
	else
	{
		//頂点の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI) + CCamera::GetCamera()->x,
								  m_pos.y + (m_fLength * m_scale.y) * cosf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI), 0.0f);

		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI) + CCamera::GetCamera()->x,
								  m_pos.y + (m_fLength * m_scale.y) * cosf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI), 0.0f);

		pVtx[2].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z) + CCamera::GetCamera()->x,
								  m_pos.y + (m_fLength * m_scale.y) * cosf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z), 0.0f);

		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z) + CCamera::GetCamera()->x,
								  m_pos.y + (m_fLength * m_scale.y) * cosf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z), 0.0f);

		this->SetVertexTex(pVtx);
	}
}

//------------------------------------------------------------------------------
// 頂点座標更新
//------------------------------------------------------------------------------
void CScene2D::SetVertex()
{
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	Vertex(pVtx);
	
	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

void CScene2D::SetVertexTex(VERTEX_2D * pVtx)
{
	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2( m_nPatternAnim % m_nWide      * (1.0f / m_nWide), (m_nPatternAnim / m_nWide) * (1.0f / m_nHeight));
	pVtx[1].tex = D3DXVECTOR2((m_nPatternAnim % m_nWide + 1) * (1.0f / m_nWide), (m_nPatternAnim / m_nWide) * (1.0f / m_nHeight));
	pVtx[2].tex = D3DXVECTOR2( m_nPatternAnim % m_nWide      * (1.0f / m_nWide), (m_nPatternAnim / m_nWide) * (1.0f / m_nHeight) + (1.0f / m_nHeight));
	pVtx[3].tex = D3DXVECTOR2((m_nPatternAnim % m_nWide + 1) * (1.0f / m_nWide), (m_nPatternAnim / m_nWide) * (1.0f / m_nHeight) + (1.0f / m_nHeight));
}

void CScene2D::SetVertexTex_Scroll(VERTEX_2D * pVtx)
{
	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(this->pMoveTex->x + 1.0f, this->pMoveTex->y + 0.0f);
	pVtx[1].tex = D3DXVECTOR2(this->pMoveTex->x + 0.0f, this->pMoveTex->y + 0.0f);
	pVtx[2].tex = D3DXVECTOR2(this->pMoveTex->x + 1.0f, this->pMoveTex->y + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(this->pMoveTex->x + 0.0f, this->pMoveTex->y + 1.0f);
}


void CScene2D::BindMoveTex(D3DXVECTOR3 * pMTex)
{
	this->pMoveTex = pMTex;
}

//------------------------------------------------------------------------------
// テクスチャアニメーションの初期化
//------------------------------------------------------------------------------
void CScene2D::SetTextureAnimationTex(int nHeight, int nWide)
{
	m_nHeight = nHeight;
	m_nWide = nWide;
}

//------------------------------------------------------------------------------
// テクスチャアニメーション
//------------------------------------------------------------------------------
bool CScene2D::SetTextureAnimation(int nFrame, bool bLoop)
{
	if (++m_nCounterAnim % nFrame == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % (m_nHeight * m_nWide);

		if (m_nPatternAnim % (m_nHeight * m_nWide) == 0 && !bLoop)
		{
			return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
// 場所
//------------------------------------------------------------------------------
void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//------------------------------------------------------------------------------
// 回転
//------------------------------------------------------------------------------
void CScene2D::SetRot(D3DXVECTOR3 rot)
{
	m_rotDirection = rot;
}

//------------------------------------------------------------------------------
// スケール
//------------------------------------------------------------------------------
void CScene2D::SetScale(D3DXVECTOR3 scale)
{
	m_scale.x = scale.x;
	m_scale.y = scale.y;
	m_scale.z = scale.z;
}

//------------------------------------------------------------------------------
// サイズ
//------------------------------------------------------------------------------
void CScene2D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;

	m_fLength = sqrtf((m_size.x / 2 * m_size.x / 2) + (m_size.y / 2 * m_size.y / 2));
}

//======================================================================================================================
// Get
//======================================================================================================================

//------------------------------------------------------------------------------
// 場所
//------------------------------------------------------------------------------
D3DXVECTOR3 CScene2D::GetPos()
{
	return m_pos;
}

//------------------------------------------------------------------------------
// 回転
//------------------------------------------------------------------------------
D3DXVECTOR3 CScene2D::GetRot()
{
	return m_rot;
}

//------------------------------------------------------------------------------
// サイズ
//------------------------------------------------------------------------------
D3DXVECTOR3 CScene2D::GetSize()
{
	return m_size;
}

//------------------------------------------------------------------------------
// スケール
//------------------------------------------------------------------------------
D3DXVECTOR3 CScene2D::GetScale()
{
	return m_scale;
}

//------------------------------------------------------------------------------
// スケール
//------------------------------------------------------------------------------
float CScene2D::GetLength()
{
	return m_fLength;
}
