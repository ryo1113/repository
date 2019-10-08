//======================================================================================================================
//
// ����[word.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"

#include "word.h"

#include "keyboard.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define FLASH_CONT				(40)
#define TRANSPARENT_BULLET		(0.5f)

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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title.png", &m_pTexture[WORD_TITLE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.png", &m_pTexture[WORD_RESULT]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enter.png", &m_pTexture[WORD_ENTER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_pTexture[WORD_TUTORIAL_BG]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking.png", &m_pTexture[WORD_RANK]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Bullet_image_00.png", &m_pTexture[WORD_BULLET_00]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Bullet_image_01.png", &m_pTexture[WORD_BULLET_01]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_00.png", &m_pTexture[WORD_CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_01.png", &m_pTexture[WORD_RETRY]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_02.png", &m_pTexture[WORD_QUIT]);

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
	pWord->m_Type = type;

	pWord->Init();

	if (WORD_BLACK_BG != type)
	{
		pWord->BindTexture(m_pTexture[type]);
	}
	else
	{
		pWord->SetCollar(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f));
	}
	
	return pWord;
}

//======================================================================================================================
// ������
//======================================================================================================================
void CWord::Init()
{
	CScene2D::Init();

	if (this->m_Type == WORD_BULLET_01)
	{
		SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, TRANSPARENT_BULLET));
	}

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

	CScene2D::Release();
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

		if (++nCntFlash == FLASH_CONT || col.a <= 0.0f || col.a >= 1.0f)
		{
			nCntFlash = 0;
			fFlash�� *= -1;
		}

		SetCollar(col);
	}


	if (this->m_Type == WORD_BULLET_00 || this->m_Type == WORD_BULLET_01)
	{
		CKeyboard *pKey = CManager::GetInputKeyboard();

		if (pKey->GetKeyboardTrigger(DIK_LSHIFT))
		{
			if (GetCollar().a == 1.0f)
			{
				SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, TRANSPARENT_BULLET));
			}			
			else if (GetCollar().a == TRANSPARENT_BULLET)
			{
				SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
}

//======================================================================================================================
// �`�揈��
//======================================================================================================================
void CWord::Draw()
{
	CScene2D::Draw();
}
