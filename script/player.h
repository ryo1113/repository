//======================================================================================================================
//
// プレイヤーの処理[player.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene2D.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define PLAYER_SIZE_X		(50)					// ポリゴンの幅
#define PLAYER_SIZE_Y		(75)					// ポリゴンの高さ

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CPlayer : public CScene2D
{
public:
	CPlayer();
	~CPlayer();

	static HRESULT Load();
	static void Unload();
	static CPlayer *Create();

	enum PLAYERSTATE
	{//自キャラ状態
		PLAYERSTATE_NORMAL = 0,			//通常
		PLAYERSTATE_DEATH,				//死んだとき
		PLAYERSTATE_MAX
	};

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	void MoveSpeedPlayer();
	void BulletShot();
	void ScreenLimit(D3DXVECTOR3 &pos);

	static LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXVECTOR3 m_move;
	int m_nType;
};

#endif