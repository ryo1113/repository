//======================================================================================================================
//
// カメラ処理[camera.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//======================================================================================================================
// インクルードファイル
//======================================================================================================================
#include "main.h"

//======================================================================================================================
// 構造体定義
//======================================================================================================================
class CCamera
{
public:
	CCamera();
	~CCamera();

	static CCamera *Create();

	void Init(void);
	static void MoveCamera(D3DXVECTOR3 move);
	static D3DXVECTOR3 *GetCamera(void);

private:
	static D3DXVECTOR3 m_pos;

	static int nScreenMax;
};

#endif