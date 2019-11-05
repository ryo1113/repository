//======================================================================================================================
//
// 弾処理[bullet.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "scene2D.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CBullet : public CScene2D
{
public:
	CBullet();
	~CBullet();

	enum BULLETTYPE
	{
		BULLETTYPE_NONE = 0,
		BULLETTYPE_HOMING,
		BULLETTYPE_MAX
	};

	static HRESULT Load();
	static void Unload();
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type);

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void DrawAll();

	static CScene *m_pBulletScene[];

private:
	void MoveSpeed();
	void TrackingBullet();

	static bool GetShotNum();

	static LPDIRECT3DTEXTURE9 m_pTexture;
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;

	D3DXVECTOR3 m_move;

	BULLETTYPE m_type;
	int m_nCntLife;

	static int m_nNumAll;			// 全体の数
	int m_nID;						// 自分の番号
};

#endif