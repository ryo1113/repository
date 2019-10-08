//======================================================================================================================
//
// ���͏��� [keyboard.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "keyboard.h"
#include "manager.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================

//======================================================================================================================
// �O���[�o���ϐ�
//======================================================================================================================

//======================================================================================================================
// �L�[�{�[�h�̏�����
//======================================================================================================================
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�i�L�[�{�[�h�j�̍쐬
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
		
	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

//======================================================================================================================
// �L�[�{�[�h�̏I������
//======================================================================================================================
void CKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//======================================================================================================================
// �L�[�{�[�h�̍X�V����
//======================================================================================================================
void CKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̓��͏��
	
	// �f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// �L�[�g���K�[���𐶐�
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// �L�[�����[�X���𐶐�
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];

			// �L�[���s�[�g���𐶐�
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if (aKeyState[nCntKey])
			{
				m_aKeyStateRepeatCnt[nCntKey]++;

				if (m_aKeyStateRepeatCnt[nCntKey] >= 20 && (m_aKeyStateRepeatCnt[nCntKey] % 10 == 0 || m_aKeyStateRepeatCnt[nCntKey] > 50))
				{
					m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCnt[nCntKey] = 0;

				m_aKeyStateRepeat[nCntKey] = 0;
			}

			m_aKeyState[nCntKey] = aKeyState[nCntKey];		//�L�[�v���X���ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();				//�L�[�{�[�h�ւ̃A�N�Z�X�����擾
	}
}

//======================================================================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//======================================================================================================================
bool CKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//======================================================================================================================
// �g���K�[�̃v���X��Ԃ��擾
//======================================================================================================================
bool CKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//======================================================================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//======================================================================================================================
bool CKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//======================================================================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//======================================================================================================================
bool CKeyboard::GetKeyboardRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

CKeyboard::CKeyboard()
{

}

CKeyboard::~CKeyboard()
{

}
