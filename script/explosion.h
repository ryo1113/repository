//======================================================================================================================
//
// 処理[explosion.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "scene2D.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// 構造体定義
//======================================================================================================================

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CExplosion : public CScene2D
{
public:
	CExplosion();
	~CExplosion();

	static HRESULT Load();
	static void Unload();
	static CExplosion *Create(D3DXVECTOR3 pos);

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

#endif