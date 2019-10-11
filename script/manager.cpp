//======================================================================================================================
//
// manager���� [manager.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

#include "input.h"
#include "pad.h"
#include "keyboard.h"

#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "pause.h"

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
// �����o�ϐ�
//======================================================================================================================
CRenderer *CManager::m_pRenderer = NULL;

CKeyboard *CManager::m_pInputKeyboard = NULL;
CPad *CManager::m_pInputPad = NULL;

CModeBase *CManager::m_pMode = {};
CManager::MODE CManager::m_Mode = {};
CPause *CManager::m_pPause = {};

CSound *CManager::m_pSound = {};

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
	if (FAILED(m_pRenderer->Init(hWnd, FALSE)))			// FALSE�Ńt�����
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

	m_pSound = new CSound;
	m_pSound->InitSound(hWnd);

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

	CManager::SetMode(MODE_TITLE);
	m_Mode = MODE_TITLE;

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
	
	if (m_pMode)
	{
		m_pMode->Uninit();
	}

	if (m_pPause)
	{
		m_pPause->Uninit();
	}

	if (m_pSound)
	{
		m_pSound->UninitSound();
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

	m_pInputPad->Update();

	m_pRenderer->Update();

	if (m_pMode)
	{
		if (m_pPause)
		{
			m_pPause->Update();
		}
		else
		{
			m_pMode->Update();
		}
	}
}

void CManager::Draw()
{
	m_pRenderer->Draw();

	if (m_pMode)
	{
		m_pMode->Draw();
	}

	if (m_pPause)
	{
		m_pPause->Draw();
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

//======================================================================================================================
// ���[�h�̐ݒ�
//======================================================================================================================
void CManager::SetMode(MODE mode)
{
	if (m_pMode && (mode != MODE_PAUSE && (m_Mode != MODE_PAUSE || CPause::GetPauseState() != CPause::PAUSE_STATE_CONTINUE)))
	{
		m_pSound->StopSound();

		m_pMode->Uninit();

		if (m_pPause)
		{
			delete m_pPause;
			m_pPause = NULL;
		}

		CScene::ReleaseAll();

		delete m_pMode;
		m_pMode = NULL;
	}

	m_Mode = mode;

	if (mode == MODE_PAUSE || CPause::GetPauseState() != CPause::PAUSE_STATE_CONTINUE)
	{
		switch (mode)
		{
		case MODE_TITLE:
			m_pMode = CTitle::Create();
			CManager::SetSound(CSound::SOUND_LABEL_BGM000);
			break;

		case MODE_TUTORIAL:
			m_pMode = CTutorial::Create();
			CManager::SetSound(CSound::SOUND_LABEL_BGM001);
			break;

		case MODE_GAME:
			m_pMode = CGame::Create();
			CManager::SetSound(CSound::SOUND_LABEL_BGM002);
			break;

		case MODE_RESULT:
			m_pMode = CResult::Create();
			CManager::SetSound(CSound::SOUND_LABEL_BGM003);
			break;

		case MODE_PAUSE:
			m_pPause = CPause::Create();
			break;

		}
	}
	else
	{
		if (m_pPause)
		{
			CPause::SetPauseState(CPause::PAUSE_STATE_NONE);
			m_pPause->Uninit();

			delete m_pPause;
			m_pPause = NULL;
		}
	}
}

//======================================================================================================================
// ���[�h�̎擾
//======================================================================================================================
CManager::MODE CManager::GetMode()
{
	return m_Mode;
}

//======================================================================================================================
// �����_�����l�̎擾
//======================================================================================================================
int CManager::random(int min, int max)
{
	// ����������
	static std::mt19937_64 create(0);

	// ��l���z���� (int) �̕��z������
	std::uniform_int_distribution<int> nGet(min, max);

	// �����𐶐�
	return nGet(create);
}

void CManager::SetSound(CSound::SOUND_LABEL label)
{
	m_pSound->PlaySound(label);
}
