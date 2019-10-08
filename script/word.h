//======================================================================================================================
//
// 処理[word.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _WORD_H_
#define _WORD_H_

#include "scene2D.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CWord : public CScene2D
{
public:
	CWord();
	~CWord();

	enum WORD_TYPE
	{
		WORD_BLACK_BG = -1,
		WORD_TITLE,
		WORD_RESULT,
		WORD_ENTER,
		WORD_TUTORIAL_BG,
		WORD_RANK,
		WORD_BULLET_00,
		WORD_BULLET_01,
		WORD_CONTINUE,
		WORD_RETRY,
		WORD_QUIT,
		WORD_MAX
	};

	static HRESULT Load();
	static void Unload();
	static CWord *Create(WORD_TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[WORD_MAX];

	D3DXVECTOR3 m_texpos;
	WORD_TYPE m_Type;

	float fFlashα;

	int nCntFlash;
};

#endif