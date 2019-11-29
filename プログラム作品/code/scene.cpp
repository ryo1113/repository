//======================================================================================================================
//
// �S�̂̏��� [scene.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "scene.h"
#include "renderer.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================

//======================================================================================================================
// �����o�ϐ�
//======================================================================================================================
CScene *CScene::m_apScene[OBJTYPE_MAX][MAX_2D] = {};
CScene *CScene::m_apSceneDrow[OBJTYPE_MAX][MAX_2D] = {};
int CScene::m_nNumAll = 0;

// �R���X�g���N�^
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

// �f�X�g���N�^
CScene::~CScene()
{

}

//======================================================================================================================
// �S�č폜���鏈��
//======================================================================================================================
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

//======================================================================================================================
// �S�Ă̍X�V����
//======================================================================================================================
void CScene::UpdateAll()
{
	if (CManager::GetMode() == MODE_PAUSE)
	{
		for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
		{
			if (!m_apScene[OBJTYPE_UI][nCnt])
				continue;

			m_apScene[OBJTYPE_UI][nCnt]->Update();
		}
	}
	else
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
}

//======================================================================================================================
// �S�Ă̕`�揈��
//======================================================================================================================
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

int CScene::GetID()
{
	return m_nID;
}

void CScene::SetObjType(OBJTYPE objtype)
{
	m_eObjType = objtype;
}

CScene::OBJTYPE CScene::GetObjType(OBJTYPE objtype,int num)
{
	return m_apScene[objtype][num]->m_eObjType;
}

//======================================================================================================================
// �I������
//======================================================================================================================
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
