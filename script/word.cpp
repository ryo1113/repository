//======================================================================================================================
//
// ����[word.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"

#include "word.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define FLASH_CONT	(40)

//======================================================================================================================
// �����o�ϐ�
//======================================================================================================================
LPDIRECT3DTEXTURE9 CWord::m_pTexture[WORD_MAX] = {};

// �R���X�g���N�^
CWord::CWord() : CScene2D::CScene2D(OBJTYPE_UI)
{

}

// �f�X�g���N�^
CWord::~CWord()
{

}

HRESULT CWord::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enter.png", &m_pTexture[2]); 
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking.png", &m_pTexture[3]);

	for (int nCnt = 0; nCnt < WORD_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] == NULL)
		{
			return -1;
		}
	}

	return S_OK;
}

void CWord::Unload()
{
	for (int nCnt = 0; nCnt < WORD_MAX; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//======================================================================================================================
// ����
//======================================================================================================================
CWord *CWord::Create(WORD_TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CWord *pWord;

	pWord = new CWord;

	pWord->SetPos(pos);
	pWord->SetSize(size);

	pWord->Init();
	pWord->m_Type = type;
	pWord->BindTexture(m_pTexture[type]);

	return pWord;
}

//======================================================================================================================
// ������
//======================================================================================================================
void CWord::Init()
{
	CScene2D::Init();

	nCntFlash = 0;
	fFlash�� = 1.0f / FLASH_CONT;

	m_texpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->BindMoveTex(&m_texpos);
}

//======================================================================================================================
// �J������
//======================================================================================================================
void CWord::Uninit()
{
	CScene2D::Uninit();
}

//======================================================================================================================
// �X�V����
//======================================================================================================================
void CWord::Update()
{
	if (this->m_Type == WORD_ENTER)
	{
		D3DXCOLOR col = GetCollar();

		col.a -= fFlash��;

		if (++nCntFlash == FLASH_CONT)
		{
			nCntFlash = 0;
			fFlash�� *= -1;
		}

		SetCollar(col);
	}
}

//======================================================================================================================
// �`�揈��
//======================================================================================================================
void CWord::Draw()
{
	CScene2D::Draw();
}
