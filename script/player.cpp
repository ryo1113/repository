//======================================================================================================================
//
// プレイヤー処理[player.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "player.h"

#include "keyboard.h"
#include "pad.h"
#include "mouse.h"

#include "bullet.h"
#include "enemy.h"
#include "camera.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define PLAYER_SPEED		(1.0f)
#define PLAYER_GRAVITY		(0.4f)

//======================================================================================================================
// プロトタイプ宣言
//======================================================================================================================

//======================================================================================================================
// メンバ変数
//======================================================================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = {};

// コンストラクタ
CPlayer::CPlayer() : CScene2D::CScene2D(OBJTYPE_PLAYER)
{

}

// デストラクタ
CPlayer::~CPlayer()
{

}

HRESULT CPlayer::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_pTexture);

	if (m_pTexture == NULL)
	{
		return -1;
	}

	return S_OK;
}

void CPlayer::Unload()
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
CPlayer *CPlayer::Create()
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;

	pPlayer->Init();
	pPlayer->BindTexture(m_pTexture);

	return pPlayer;
}

//======================================================================================================================
// 初期化
//======================================================================================================================
void CPlayer::Init()
{
	SetSize(D3DXVECTOR3(150.0f, 70.0f, 0.0f));
	SetPos(D3DXVECTOR3(200, 300, 0.0f));

	CScene2D::Init();
}

//======================================================================================================================
// プレイヤの開放処理
//======================================================================================================================
void CPlayer::Uninit()
{
	CScene2D::Uninit();

	CScene::Release();
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void CPlayer::Update()
{
	BulletShot();

	D3DXVECTOR3 pos = GetPos();

	m_move.x -= m_move.x * 1 / 5;
	m_move.y -= m_move.y * 1 / 5;

	MoveSpeedPlayer();

	// 向きの処理
	Rot();

	if (fabsf(m_move.x) < 0.01f)
	{
		m_move.x = 0;
	}
	if (fabsf(m_move.y) < 0.01f)
	{
		m_move.y = 0;
	}

	ScreenLimit(pos);

	m_move += D3DXVECTOR3(0.0f, PLAYER_GRAVITY, 0.0f);

	pos += m_move;

	SetPos(pos);

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

			CRenderer::SetFade(CManager::MODE_RESULT);

			break;
		}
	}
}

//======================================================================================================================
// 描画処理
//======================================================================================================================
void CPlayer::Draw()
{
	CScene2D::Draw();
}

//======================================================================================================================
// 移動処理
//======================================================================================================================
void CPlayer::MoveSpeedPlayer()
{
	CKeyboard *pKey = CManager::GetInputKeyboard();
	CPad *pPad = CManager::GetInputPad();
	CMouse *pMouse = CManager::GetInputMouse();

	//キーボード
	if (pKey->GetKeyboardPress(DIK_W) && pKey->GetKeyboardPress(DIK_S)){}
	else if (pKey->GetKeyboardPress(DIK_D) && pKey->GetKeyboardPress(DIK_A)){}

	else if (pKey->GetKeyboardPress(DIK_D) && pKey->GetKeyboardPress(DIK_S))
	{
		m_move += MoveVector(D3DXVECTOR3(1.0f, 1.0f, 0.0f), PLAYER_SPEED);
		SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -1.0f / 8.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_D) && pKey->GetKeyboardPress(DIK_W))
	{
		m_move += MoveVector(D3DXVECTOR3(1.0f, -1.0f, 0.0f), PLAYER_SPEED);
		SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 16.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_A) && pKey->GetKeyboardPress(DIK_W))
	{
		m_move += MoveVector(D3DXVECTOR3(-1.0f, -1.0f, 0.0f), PLAYER_SPEED);
		SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 6.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_A) && pKey->GetKeyboardPress(DIK_S))
	{
		m_move += MoveVector(D3DXVECTOR3(-1.0f, 1.0f, 0.0f), PLAYER_SPEED);
		SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 3.0f));
	}

	else if (pKey->GetKeyboardPress(DIK_D))
	{
		m_move += MoveVector(D3DXVECTOR3(1.0f, 0.0f, 0.0f), PLAYER_SPEED);
		SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_A))
	{
		m_move += MoveVector(D3DXVECTOR3(-1.0f, 0.0f, 0.0f), PLAYER_SPEED);
		SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 4.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_W))
	{
		m_move += MoveVector(D3DXVECTOR3(0.0f, -1.0f, 0.0f), PLAYER_SPEED);
		SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 8.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_S))
	{
		m_move += MoveVector(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PLAYER_SPEED);
		SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -1.0f / 4.0f));
	}
	else
	{
		SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	float fV, fH;
	pPad->GetJoypadStickLeft(0, &fH, &fV);

	//ゲームパッド
	if (fH != 0 || fV != 0)
	{
		// 移動
		m_move += Move(atan2f(fH, fV), PLAYER_SPEED);

		// 向き
		SetRot(D3DXVECTOR3(0.0f, 0.0f, -atan2f(fH, fV)));
	}
	
	if (pKey->GetKeyboardTrigger(DIK_G))
	{ 
		SetScale(D3DXVECTOR3(2.0f, 2.0f, 0.0f));
	}

	if (pKey->GetKeyboardPress(DIK_H))
	{
		SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.005f) + GetRot());
	}

	//fH = (float)pMouse->GetMouseX();
	//fV = (float)pMouse->GetMouseY();

	////マウス
	//if (fH != 0 || fV != 0)
	//{
	//	m_move = MoveMouse(D3DXVECTOR3(fH, fV, 0.0f));
	//} 
}

//======================================================================================================================
// 弾の発射
//======================================================================================================================
void CPlayer::BulletShot()
{
	CKeyboard *pKey = CManager::GetInputKeyboard();

	if (pKey->GetKeyboardTrigger(DIK_SPACE))
	{
		CBullet::Create(GetPos() + D3DXVECTOR3(GetSize().x * 0.5f, 0.0f, 0.0f),
						D3DXVECTOR3(5.0f, 0.0f, 0.0f));
	}
}

void CPlayer::ScreenLimit(D3DXVECTOR3 &pos)
{
	float fLenght = GetLength();
	float fCamera = CCamera::GetCamera()->x;
	D3DXVECTOR3 Scale = GetScale();

	if (pos.x - fLenght * Scale.x < 0.0f - fCamera)
	{
		pos.x = fLenght * Scale.x - fCamera;
	}
	if (pos.x + fLenght * Scale.x >= SCREEN_WIDTH - fCamera)
	{
		pos.x = SCREEN_WIDTH - fLenght * Scale.x - fCamera;
	}
	if (pos.y - fLenght * Scale.y <= 0.0f)
	{
		pos.y = fLenght * Scale.y;
	}
	if (pos.y + fLenght * Scale.y >= SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - fLenght * Scale.y;
	}
}
