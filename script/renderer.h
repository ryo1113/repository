//===========================================================================================================
//
// レンダリング処理 [renderer.h]
// Author:RYO KANDA
//
//===========================================================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//===========================================================================================================
// インクルードファイル
//===========================================================================================================
#include "manager.h"

class CFade;

//===========================================================================================================
// マクロ定義
//===========================================================================================================

//===========================================================================================================
// クラス定義
//===========================================================================================================
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();

#ifdef _DEBUG
	static int m_nCountFPS;		// FPSカウンタ

	void DrawFPS();
#endif
	static void SetFade(CManager::MODE modeNext);

	LPDIRECT3DDEVICE9 GetDevice(void);

protected:

private:
	static CFade *m_Fade;

#ifdef _DEBUG
	LPD3DXFONT			m_pFont = NULL;			// フォントへのポインタ
#endif

	LPDIRECT3D9	m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

};

#endif