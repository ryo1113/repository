//======================================================================================================================
//
// 処理 [scene.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "scene.h"
#include "renderer.h"

//======================================================================================================================
// マクロ定義
//======================================================================================================================

//======================================================================================================================
// 構造体定義
//======================================================================================================================

//======================================================================================================================
// プロトタイプ宣言
//======================================================================================================================

//======================================================================================================================
// メンバ変数
//======================================================================================================================
CScene *CScene::m_apScene[OBJTYPE_MAX][MAX_2D] = {};
CScene *CScene::m_apSceneDrow[OBJTYPE_MAX][MAX_2D] = {};
int CScene::m_nNumAll = 0;


// コンストラクタ
CScene::CScene(OBJTYPE objtype, OBJECT obj)
{
	for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
	{
		if (obj == OBJECT_NONE)
		{
			if (!m_apSceneDrow[objtype][nCnt])
			{
				m_apSceneDrow[objtype][nCnt] = this;
				this->m_nDrawIdx = nCnt;

				break;
			}
		}
	}

	for (int nCount = 0; nCount < MAX_2D; nCount++)
	{
		if (!m_apScene[objtype][nCount])
		{
			m_apScene[objtype][nCount] = this;

			this->m_nID = nCount;

			m_nNumAll++;
			break;
		}
	}
	m_eObjType = objtype;
}

// デストラクタ
CScene::~CScene()
{

}

void CScene::ReleaseAll()
{
	for (int nCount = 0; nCount < OBJTYPE_MAX; nCount++)
	{
		for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
		{
			if (!m_apScene[nCount][nCnt])
				continue;

			m_apScene[nCount][nCnt]->Uninit();

			if (!m_apScene[nCount][nCnt])
				continue;

			m_apScene[nCount][nCnt]->Release();
		}
	}
}

void CScene::UpdateAll()
{
	for (int nCount = 0; nCount < OBJTYPE_MAX; nCount++)
	{
		for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
		{
			if (!m_apScene[nCount][nCnt])
				continue;

			m_apScene[nCount][nCnt]->Update();
		}
	}
}

void CScene::DrawAll()
{
	for (int nCount = 0; nCount < OBJTYPE_MAX; nCount++)
	{
		for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
		{
			if (!m_apSceneDrow[nCount][nCnt])
				continue;

			m_apSceneDrow[nCount][nCnt]->Draw();
		}
	}
}

CScene *CScene::GetScene(OBJTYPE objtype, int num)
{
	return m_apScene[objtype][num];
}

void CScene::SetObjType(OBJTYPE objtype)
{
	m_eObjType = objtype;
}

CScene::OBJTYPE CScene::GetObjType(OBJTYPE objtype,int num)
{
	return m_apScene[objtype][num]->m_eObjType;
}

void CScene::Release(CScene **ppBeloneArray)
{
	if (!m_apScene[m_eObjType][m_nID])
		return;

	OBJTYPE eObjType = m_eObjType;

	int nID = m_nID;

	if (!(ppBeloneArray == m_apSceneDrow[0]))
	{
		if (ppBeloneArray[this->m_nDrawIdx])
		{
			ppBeloneArray[this->m_nDrawIdx] = NULL;
		}
	}
	else
	{
		m_apSceneDrow[eObjType][this->m_nDrawIdx] = NULL;
	}

	delete m_apScene[eObjType][nID];
	m_apScene[eObjType][nID] = NULL;

	m_nNumAll--;
}
