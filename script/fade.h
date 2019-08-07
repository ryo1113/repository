//======================================================================================================================
//
// フェード処理 [fade.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _FADE_H_
#define _FADE_H_

//======================================================================================================================
// インクルードファイル
//======================================================================================================================
#include "manager.h"

//======================================================================================================================
// フェードの状態
//======================================================================================================================
class CFade
{
public:
	CFade() {}
	~CFade() {}

	enum FADE
	{
		FADE_NONE = 0,		// 何もない状態
		FADE_IN,			// フェードイン処理
		FADE_OUT,			// フェードアウト処理
		FADE_MAX
	};
	static CFade *Create(CManager::MODE modeNext);

	void InitFade(CManager::MODE modeNext);
	void UninitFade();
	void UpdateFade();
	void DrawFade();

	FADE GetFade();

private:
	void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffFade = NULL;		// 頂点バッファへのポインタ
	FADE					m_fade;						// フェード状態
	CManager::MODE			m_modeNext;					// 次の画面（モード）
	D3DXCOLOR				m_colorFade;				// フェード色
};

#endif
