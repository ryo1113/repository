//======================================================================================================================
//
// 数字描画処理[number.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "scene.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CNumber : public CScene
{
public:
	CNumber();
	~CNumber();

	void Vertex(VERTEX_2D *pVtx) {}

	static HRESULT Load();
	static void Unload();
	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetNum(int num);

	void SetPos(D3DXVECTOR3 pos);
	void SetColor(D3DXCOLOR col);
	D3DXCOLOR GetColor();

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	D3DXCOLOR m_col;

	int m_nNumber;
};

#endif