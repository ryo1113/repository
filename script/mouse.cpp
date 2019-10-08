//======================================================================================================================
//
// �}�E�X���͏��� [mouse.cpp]
// Author:RYO KANDA
//
//======================================================================================================================

//======================================================================================================================
// �C���N���[�h�t�@�C��
//======================================================================================================================
#include "mouse.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================

// �R���X�g���N�^
CMouse::CMouse()
{

}

// �f�X�g���N�^
CMouse::~CMouse()
{

}

//======================================================================================================================
// �}�E�X�̏���������
//======================================================================================================================
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL);
	m_pDevice->SetDataFormat(&c_dfDIMouse2); // �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�

	m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// �f�o�C�X�̐ݒ�    
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j    
	m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// ���͐���J�n    
	m_pDevice->Acquire();

	// ���
	m_hWnd = hWnd;

	return S_OK;
}

//======================================================================================================================
// �}�E�X�̏I������
//======================================================================================================================
void CMouse::Uninit()
{
	CInput::Uninit();
}

//======================================================================================================================
// �}�E�X�̍X�V����
//======================================================================================================================
void CMouse::Update()
{
	// �ϐ��錾
	POINT point;

	// �l�̏�����    
	m_mouse.mouseState.rgbButtons[0] = 0;

	GetCursorPos(&point);
	ScreenToClient(m_hWnd, &point);

	m_mouse.pos.x = (float)point.x;
	m_mouse.pos.y = (float)point.y;

	// �l�̍X�V 
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_mouse.mouseState)))
	{
		for (int nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
		{
			// �r���I�_���a���ǂ������f
			m_aButtonStateTrigger[nCntButton] = (m_aButtonState[nCntButton] ^ m_mouse.mouseState.rgbButtons[nCntButton]) & m_mouse.mouseState.rgbButtons[nCntButton];

			// �L�[�v���X���ۑ�
			m_aButtonState[nCntButton] = m_mouse.mouseState.rgbButtons[nCntButton];	
		}
	}
	else
	{
		m_pDevice->Acquire(); // �P���ڂ�Q���ڂɃG���[���o�邪�������Ă悢�B 
	}
}

//======================================================================================================================
// �}�E�X�̃v���X��Ԃ��擾
//======================================================================================================================
bool CMouse::GetMousePress(MOUSEBUTTON nButton)
{
	return (m_mouse.mouseState.rgbButtons[nButton] & 0x80) ? true : false;
}

//======================================================================================================================
// �}�E�X�̃g���K�[����
//======================================================================================================================
bool CMouse::GetMousePressTrigger(MOUSEBUTTON nButton)
{
	return (m_aButtonStateTrigger[nButton] & 0x80) ? true : false;
}

//======================================================================================================================
// �}�E�X��X���W�̎擾
//======================================================================================================================
LONG CMouse::GetMouseX()
{
	return (LONG)m_mouse.pos.x;
}

//======================================================================================================================
// �}�E�X��Y���W�̎擾
//======================================================================================================================
LONG CMouse::GetMouseY()
{
	return (LONG)m_mouse.pos.y;
}