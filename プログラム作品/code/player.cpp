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

#include "bullet.h"
#include "enemy.h"
#include "camera.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define PLAYER_SPEED			(1.5f)
#define PLAYER_GRAVITY			(0.4f)

#define TURN_FRAME_PLAYER		(1.5f)

#define POD_ROT(e)				(D3DXToRadian(180 / 8 * e))

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
	SetPos(D3DXVECTOR3(300, 50, 0.0f));

	CScene2D::Init();

	m_nType = 0;
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
	Rot(TURN_FRAME_PLAYER);

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

	// 敵に当たる
	for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
	{
		CScene *pScene = GetScene(OBJTYPE_ENEMY, nCnt);

		if (!pScene)
			continue;

		CEnemyBase *pEnemy = (CEnemyBase*)pScene;

		if (HitShapeCollision(pEnemy))
		{
			pEnemy->HitEnemy();

			this->HitPlayer();

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
// 自機に弾衝突処理
//======================================================================================================================
void CPlayer::HitPlayer()
{
	this->Uninit();

	CRenderer::SetFade(MODE_RESULT);
}

//======================================================================================================================
// 移動処理
//======================================================================================================================
void CPlayer::MoveSpeedPlayer()
{
	CKeyboard *pKey = CManager::GetInputKeyboard();
	CPad *pPad = CManager::GetInputPad();

	//キーボード
	if (pKey->GetKeyboardPress(DIK_W) && pKey->GetKeyboardPress(DIK_S)){}
	else if (pKey->GetKeyboardPress(DIK_D) && pKey->GetKeyboardPress(DIK_A)){}

	else if (pKey->GetKeyboardPress(DIK_D) && pKey->GetKeyboardPress(DIK_S))
	{// 右下
		m_move += MoveVector(D3DXVECTOR3(1.0f, 1.0f, 0.0f), PLAYER_SPEED);
		SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -1.0f / 8.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_D) && pKey->GetKeyboardPress(DIK_W))
	{// 右上
		m_move += MoveVector(D3DXVECTOR3(1.0f, -1.0f, 0.0f), PLAYER_SPEED);
		SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 16.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_A) && pKey->GetKeyboardPress(DIK_W))
	{// 左上
		m_move += MoveVector(D3DXVECTOR3(-1.0f, -1.0f, 0.0f), PLAYER_SPEED);
		SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 6.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_A) && pKey->GetKeyboardPress(DIK_S))
	{// 左下
		m_move += MoveVector(D3DXVECTOR3(-1.0f, 1.0f, 0.0f), PLAYER_SPEED);
		SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 3.0f));
	}

	else if (pKey->GetKeyboardPress(DIK_D))
	{// 右
		m_move += MoveVector(D3DXVECTOR3(1.0f, 0.0f, 0.0f), PLAYER_SPEED);
		SetDifRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_A))
	{// 左
		m_move += MoveVector(D3DXVECTOR3(-1.0f, 0.0f, 0.0f), PLAYER_SPEED);
		SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 4.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_W))
	{// 上
		m_move += MoveVector(D3DXVECTOR3(0.0f, -1.0f, 0.0f), PLAYER_SPEED);
		SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 8.0f));
	}
	else if (pKey->GetKeyboardPress(DIK_S))
	{// 下
		m_move += MoveVector(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PLAYER_SPEED);
		SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -1.0f / 4.0f));
	}
	else
	{
		SetDifRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// 入力された X、Y
	float fH, fV;
	pPad->GetJoypadStickLeft(0, &fH, &fV);

	//ゲームパッド
	if (fH != 0 || fV != 0)
	{
		// 移動
		m_move += Move(atan2f(fH, fV), PLAYER_SPEED);

		float fRot = -atan2f(fH, fV);

		{// 向き
			if (-POD_ROT(1) < fRot && fRot < POD_ROT(1))
			{// 上
				SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 8.0f));
			}
			else if (POD_ROT(1) < fRot && fRot < POD_ROT(3))
			{// 左上
				SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 6.0f));
			}
			else if (POD_ROT(3) < fRot && fRot < POD_ROT(5))
			{// 左
				SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 4.0f));
			}
			else if (POD_ROT(5) < fRot && fRot < POD_ROT(7))
			{// 左下
				SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 3.0f));
			}

			else if (POD_ROT(7) <= fRot || fRot <= -POD_ROT(7))
			{// 下
				SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -1.0f / 4.0f));
			}
			else if (-POD_ROT(7) < fRot && fRot < -POD_ROT(5))
			{// 右下
				SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -1.0f / 8.0f));
			}
			else if (-POD_ROT(5) < fRot && fRot < -POD_ROT(3))
			{// 右
				SetDifRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if (-POD_ROT(3) < fRot && fRot < -POD_ROT(1))
			{// 右上
				SetDifRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.0f / 16.0f));
			}
		}
	}
}

//======================================================================================================================
// 弾の種類と発射
//======================================================================================================================
void CPlayer::BulletShot()
{
	CKeyboard *pKey = CManager::GetInputKeyboard();
	CPad *pPad = CManager::GetInputPad();

	if (pKey->GetKeyboardTrigger(DIK_LSHIFT) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_Y))
	{
		m_nType += 1;

		if (m_nType >= CBullet::BULLETTYPE_MAX)
		{
			m_nType = 0;
		}
	}

	if (pKey->GetKeyboardTrigger(DIK_SPACE) || pPad->GetJoypadTrigger(0, CPad::JOYPADKEY_RIGHT_SHOULDER))
	{
		CBullet::Create(GetPos() + D3DXVECTOR3(GetLength() * cosf(-GetRot().z),
											   GetLength() * sinf(-GetRot().z), 0.0f),
						D3DXVECTOR3(GetLength() * cosf(-GetRot().z), GetLength() * sinf(-GetRot().z), 0.0f), (CBullet::BULLETTYPE)m_nType);
	}
}

//======================================================================================================================
// 動きの制限
//======================================================================================================================
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
