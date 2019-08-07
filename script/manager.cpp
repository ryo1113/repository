//======================================================================================================================
//
// ���� [manager.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

#include "input.h"
#include "pad.h"
#include "keyboard.h"
#include "mouse.h"

#include "title.h"
#include "game.h"
#include "result.h"

#include "number.h"
#include "word.h"

#include "bg.h"
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "explosion.h"
#include "player.h"
#include "score.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================

//======================================================================================================================
// �\���̒�`
//======================================================================================================================

//======================================================================================================================
// �v���g�^�C�v�錾
//======================================================================================================================

//======================================================================================================================
// �����o�ϐ�
//======================================================================================================================
CRenderer *CManager::m_pRenderer = NULL;
CKeyboard *CManager::m_pInputKeyboard = NULL;
CPad *CManager::m_pInputPad = NULL;
CMouse *CManager::m_pInputMouse = NULL;

CModeBase *CManager::m_pMode = new CTitle;
CManager::MODE CManager::m_Mode = MODE_TITLE;

// �R���X�g���N�^
CManager::CManager()
{
}

// �f�X�g���N�^
CManager::~CManager()
{
}

//======================================================================================================================
// ������
//======================================================================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	m_pRenderer = new CRenderer;
	// ����������(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return -1;
	}

	m_pInputKeyboard = new CKeyboard;
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return -1;
	}

	m_pInputPad = new CPad;
	if (FAILED(m_pInputPad->Init(hInstance, hWnd)))
	{
		return -1;
	}

	m_pInputMouse = new CMouse;
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return -1;
	}

	// �e�N�X�`���Ǘ�(�Ȃ��Ƃ�)
	if (FAILED(CBullet::Load()))
	{
		MessageBox(hWnd, "�e�N�X�`��������܂���", "Bullet", MB_ICONWARNING);// �m�F�p
		return -1;
	}
	if (FAILED(CPlayer::Load()))
	{
		MessageBox(hWnd, "�e�N�X�`��������܂���", "Player", MB_ICONWARNING);// �m�F�p
		return -1;
	}
	if (FAILED(CExplosion::Load()))
	{
		MessageBox(hWnd, "�e�N�X�`��������܂���", "Explosion", MB_ICONWARNING);// �m�F�p
		return -1;
	}
	if (FAILED(CEffect::Load()))
	{
		MessageBox(hWnd, "�e�N�X�`��������܂���", "Effect", MB_ICONWARNING);// �m�F�p
		return -1;
	}
	if (FAILED(CBg::Load()))
	{
		MessageBox(hWnd, "�e�N�X�`��������܂���", "Bg", MB_ICONWARNING);// �m�F�p
		return -1;
	}
	if (FAILED(CEnemy::Load()))
	{
		MessageBox(hWnd, "�e�N�X�`��������܂���", "Enemy", MB_ICONWARNING);// �m�F�p
		return -1;
	}
	if (FAILED(CNumber::Load()))
	{
		MessageBox(hWnd, "�e�N�X�`��������܂���", "Number", MB_ICONWARNING);// �m�F�p
		return -1;
	}
	if (FAILED(CWord::Load()))
	{
		MessageBox(hWnd, "�e�N�X�`��������܂���", "Word", MB_ICONWARNING);// �m�F�p
		return -1;
	}

	if (m_pMode)
	{
		m_pMode->Init();
	}

	return S_OK;
}

//======================================================================================================================
// �I������
//======================================================================================================================
void CManager::Uninit()
{
	if (m_pRenderer != NULL)
	{
		// �I������
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (m_pInputKeyboard != NULL)
	{
		// �I������
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (m_pInputPad != NULL)
	{
		// �I������
		m_pInputPad->Uninit();

		delete m_pInputPad;
		m_pInputPad = NULL;
	}
	
	if (m_pInputMouse != NULL)
	{
		// �I������
		m_pInputMouse->Uninit();

		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	if (m_pMode)
	{
		m_pMode->Uninit();
	}

	// �e�N�X�`���j��
	CBullet::Unload();
	CEffect::Unload();
	CEnemy::Unload();
	CExplosion::Unload();
	CPlayer::Unload();
	CBg::Unload();
	CWord::Unload();

	CScene::ReleaseAll();
}

//======================================================================================================================
// �X�V����
//======================================================================================================================
void CManager::Update()
{
	m_pInputKeyboard->Update();

	m_pInputMouse->Update();

	m_pInputPad->Update();

	m_pRenderer->Update();

	if (m_pMode)
	{
		m_pMode->Update();
	}
}

void CManager::Draw()
{
	m_pRenderer->Draw();

	if (m_pMode)
	{
		m_pMode->Draw();
	}
}

CRenderer *CManager::GetRenderer()
{
	return m_pRenderer;
}

CKeyboard *CManager::GetInputKeyboard()
{
	return m_pInputKeyboard;
}

CPad *CManager::GetInputPad()
{
	return m_pInputPad;
}

CMouse *CManager::GetInputMouse()
{
	return m_pInputMouse;
}

//======================================================================================================================
// ���[�h�̐ݒ�
//======================================================================================================================
void CManager::SetMode(MODE mode)
{
	if (m_pMode)
	{
		CScene::ReleaseAll();

		delete m_pMode;
		m_pMode = NULL;
	}

	m_Mode = mode;

	switch (mode)
	{
	case MODE_TITLE:
		m_pMode = CTitle::Create();
		break;

	case MODE_TUTORIAL:
		break;

	case MODE_GAME:
		m_pMode = CGame::Create();
		break;

	case MODE_RESULT:
		m_pMode = CResult::Create();
		break;

	case MODE_RANKING:
		break;
	}
}

//======================================================================================================================
// ���[�h�̎擾
//======================================================================================================================
CManager::MODE CManager::GetMode()
{
	return m_Mode;
}
