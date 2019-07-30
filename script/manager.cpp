//======================================================================================================================
//
// ���� [manager.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "scene2D.h"

#include "input.h"
#include "pad.h"
#include "keyboard.h"
#include "mouse.h"

#include "number.h"

#include "bg.h"
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "explosion.h"
#include "player.h"
#include "score.h"

#include "camera.h"

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

// �R���X�g���N�^
CManager::CManager()
{
}

// �f�X�g���N�^
CManager::~CManager()
{
}

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

	CBg::Create(0, 0.0005f);

	CPlayer::Create();

	CEnemy::Create(D3DXVECTOR3(640.0f, 150.0f, 0.0f));

	CScore::Create(D3DXVECTOR3(1250.0f, 60.0f, 0.0f), D3DXVECTOR3(60.0f, 100.0f, 0.0f));

	CCamera::Create();

	return S_OK;
}

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
	// �e�N�X�`���j��
	CBullet::Unload();
	CEffect::Unload();
	CEnemy::Unload();
	CExplosion::Unload();
	CPlayer::Unload();
	CBg::Unload();

	CScene::ReleaseAll();
}

void CManager::Update()
{
	CCamera::MoveCamera(D3DXVECTOR3(-0.2f, 0.0f, 0.0f));

	m_pInputKeyboard->Update();

	m_pInputMouse->Update();

	m_pInputPad->Update();

	m_pRenderer->Update();
}

void CManager::Draw()
{
	m_pRenderer->Draw();
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
