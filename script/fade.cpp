//======================================================================================================================
//
// フェード処理 [fade.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "fade.h"
#include "manager.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define	TEXTURE_FADE	"data/TEXTURE/fade000.jpg"		// 読み込むテクスチャファイル名

//======================================================================================================================
// プロトタイプ宣言
//======================================================================================================================
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);

//======================================================================================================================
// グローバル変数
//======================================================================================================================
LPDIRECT3DTEXTURE9		g_pTextureFade = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		// 頂点バッファへのポインタ
FADE					g_fade;						// フェード状態
MODE					g_modeNext;					// 次の画面（モード）
D3DXCOLOR				g_colorFade;				// フェード色

//======================================================================================================================
// 初期化処理
//======================================================================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	g_fade = FADE_OUT;

	g_modeNext = modeNext;

	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// 黒い画面（不透明）

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_FADE,		// ファイルの名前
								&g_pTextureFade);	// 読み込むメモリー

	// 頂点情報の設定
	MakeVertexFade(pDevice);
}

//======================================================================================================================
// 終了処理
//======================================================================================================================
void UninitFade(void)
{
	// テクスチャの開放
	if(g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	// 頂点バッファの開放
	if(g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//======================================================================================================================
// 更新処理
//======================================================================================================================
void UpdateFade(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{
			g_colorFade.a -= 0.03f;

			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if (g_fade == FADE_OUT)
		{
			g_colorFade.a += 0.03f;

			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;
				
				SetMode(g_modeNext);
			}
		}
	}
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// 頂点データをアンロックする
	g_pVtxBuffFade->Unlock();
}

//======================================================================================================================
// タイトル画面
//======================================================================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//======================================================================================================================
// 頂点の作成
//======================================================================================================================
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffFade, NULL);
	
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	

	// 頂点データをアンロックする
	g_pVtxBuffFade->Unlock();
}

//======================================================================================================================
// フェードの状態設定
//======================================================================================================================
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;

	g_modeNext = modeNext;
	
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//黒い画面

}

//======================================================================================================================
// フェードの状態取得
//======================================================================================================================
FADE GetFade(void)
{
	return g_fade;
}

