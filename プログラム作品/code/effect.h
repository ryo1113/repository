//======================================================================================================================
//
// エフェクト処理[effect.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

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
class CEffect : public CScene2D
{
public:

	enum EFFECTTYPE
	{
		EFFECTTYPE_NONE,
		EFFECTTYPE_LASER,
		EFFECTTYPE_MAX
	};

	CEffect();
	~CEffect();

	static HRESULT Load();
	static void Unload();
	static CEffect *Create(D3DXVECTOR3 pos, EFFECTTYPE type);

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void DrawAll();

	static CScene *m_pEffectScene[];

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;

	D3DXVECTOR3 m_move;

	static int m_nNumAll;

	EFFECTTYPE m_type;
	int m_nID;			// 自分の番号
	int m_nLife;
};

#endif