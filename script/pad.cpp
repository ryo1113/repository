//======================================================================================================================
//
// �Q�[���p�b�h���� [pad.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "pad.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================

//======================================================================================================================
// �����o�ϐ�������
//======================================================================================================================
LPDIRECTINPUTDEVICE8 CPad::m_apDIDevJoypad[NUM_JOYPAD_MAX] = {};
int CPad::m_nCntPad = 0;
int CPad::m_nIntervalRepeat = INTERVAL_REPEAT;

//======================================================================================================================
// ���͏����̏�����
//======================================================================================================================
HRESULT CPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (m_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);

		m_nCountWaitRepeat = COUNT_WAIT_REPEAT;
	}

	for (m_nCntPad = 0; m_nCntPad < NUM_JOYPAD_MAX; m_nCntPad++)
	{
		hr = m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallbackJoypad, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || m_apDIDevJoypad[m_nCntPad] == NULL)
		{
			MessageBox(hWnd, "�W���C�p�b�h������܂���", "�x��", MB_ICONWARNING);// �m�F�p
			return hr;
		}

		// �f�[�^�t�H�[�}�b�g��ݒ�
		hr = m_apDIDevJoypad[m_nCntPad]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		hr = m_apDIDevJoypad[m_nCntPad]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h�̋������[�h��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		hr = m_apDIDevJoypad[m_nCntPad]->EnumObjects(EnumAxesCallbackJoypad, NULL, DIDFT_AXIS);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h��������܂���ł���", "�x��", MB_ICONWARNING);
			return hr;
		}

		// �f�o�C�X�̐ݒ�
		DIPROPRANGE diprg;

		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);

		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

		// �w�����͈̔͂��w��
		m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

		// �x�����͈̔͂��w��
		m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		diprg.diph.dwObj = DIJOFS_Z;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

		// �y�����͈̔͂��w��
		m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		diprg.diph.dwObj = DIJOFS_RZ;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

		// �q�y�����͈̔͂��w��
		m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
		m_apDIDevJoypad[m_nCntPad]->Acquire();

		// ���[�N�N���A
		memset(&m_aJoypadState[m_nCntPad], 0, sizeof m_aJoypadState[m_nCntPad]);
		memset(m_aKeyStateJoypad[m_nCntPad], 0, sizeof m_aKeyStateJoypad[m_nCntPad]);
		memset(m_aKeyStateTriggerJoypad[m_nCntPad], 0, sizeof m_aKeyStateTriggerJoypad[m_nCntPad]);
		memset(m_aKeyStateReleaseJoypad[m_nCntPad], 0, sizeof m_aKeyStateReleaseJoypad[m_nCntPad]);
		memset(m_aKeyStateRepeatJoypad[m_nCntPad], 0, sizeof m_aKeyStateRepeatJoypad[m_nCntPad]);
	}

	return hr;
}

//======================================================================================================================
// ���͏����̏I������
//======================================================================================================================
void CPad::Uninit(void)
{
	CInput::Uninit();

	// �W���C�p�b�h�̏I������
	for (int nCntJoypad = 0; nCntJoypad < NUM_JOYPAD_MAX; nCntJoypad++)
	{
		if (m_apDIDevJoypad[nCntJoypad] != NULL)
		{// �f�o�C�X�I�u�W�F�N�g�̊J��
			m_apDIDevJoypad[nCntJoypad]->Release();
			m_apDIDevJoypad[nCntJoypad] = NULL;
		}
	}
}

//======================================================================================================================
// ���͏����̍X�V����
//======================================================================================================================
void CPad::Update(void)
{
	HRESULT hr;
	bool aKeyStateOld[JOYPADKEY_MAX];

	for (int nCntJoypad = 0; nCntJoypad < NUM_JOYPAD_MAX; nCntJoypad++)
	{
		if (m_apDIDevJoypad[nCntJoypad] == NULL)
		{
			return;
		}

		// �O��̃f�[�^��ۑ�
		for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
		{
			aKeyStateOld[nCntKey] = m_aKeyStateJoypad[nCntJoypad][nCntKey];
		}

		// �f�o�C�X����f�[�^���擾
		hr = m_apDIDevJoypad[nCntJoypad]->GetDeviceState(sizeof(m_aJoypadState[nCntJoypad]), &m_aJoypadState[nCntJoypad]);

		if (SUCCEEDED(hr))
		{
			// �L�[���ݒ�
			SetKeyStateJoypad(nCntJoypad);

			//	���ʒu���L�^
			m_aKeyStateAxis[nCntJoypad].x = (float)m_aJoypadState[nCntJoypad].lX / JOY_MAX_RANGE;
			m_aKeyStateAxis[nCntJoypad].y = (float)m_aJoypadState[nCntJoypad].lY / JOY_MAX_RANGE;
			m_aKeyStateAxis[nCntJoypad].z = (float)m_aJoypadState[nCntJoypad].lZ / JOY_MAX_RANGE;

			for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
			{
				// �g���K�[���̍쐬
				m_aKeyStateTriggerJoypad[nCntJoypad][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateJoypad[nCntJoypad][nCntKey]) & m_aKeyStateJoypad[nCntJoypad][nCntKey];

				// �����[�X���̍쐬
				m_aKeyStateReleaseJoypad[nCntJoypad][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateJoypad[nCntJoypad][nCntKey]) & !m_aKeyStateJoypad[nCntJoypad][nCntKey];

				// ���s�[�g���̍쐬
				m_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = m_aKeyStateTriggerJoypad[nCntJoypad][nCntKey];

				if (m_aKeyStateJoypad[nCntJoypad][nCntKey])
				{
					m_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey]++;

					if (((m_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] - m_nCountWaitRepeat) % m_nIntervalRepeat) == 0 && (m_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] % 10 == 0 || m_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] > 50))
					{
						m_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = m_aKeyStateJoypad[nCntJoypad][nCntKey];
					}
				}
				else
				{
					m_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] = 0;
					m_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = 0;
				}
			}
		}
		else
		{
			m_apDIDevJoypad[nCntJoypad]->Acquire();
		}
	}
}

//======================================================================================================================
// �W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�
//======================================================================================================================
BOOL CALLBACK CPad::EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	static GUID pad_discrimination[NUM_JOYPAD_MAX];	// �e�f�o�C�X�̎��ʎq���i�[
	DIDEVCAPS	diDevCaps;							// �f�o�C�X���
	HRESULT		hRes;

	for (int nCnt = 0; nCnt < NUM_JOYPAD_MAX; nCnt++)
	{
		if (pad_discrimination[nCnt] == lpddi->guidInstance)
		{
			return DIENUM_CONTINUE;			// �񋓂𑱂���
		}
	}

	// �W���C�X�e�B�b�N�p�f�o�C�X�I�u�W�F�N�g���쐬
	hRes = m_pInput->CreateDevice(lpddi->guidInstance, &m_apDIDevJoypad[m_nCntPad], NULL);
	if (FAILED(hRes))
	{
		return DIENUM_CONTINUE;			// �񋓂𑱂���
	}

	// �W���C�X�e�B�b�N�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hRes = m_apDIDevJoypad[m_nCntPad]->GetCapabilities(&diDevCaps);
	if (FAILED(hRes))
	{
		if (m_apDIDevJoypad[m_nCntPad])
		{
			m_apDIDevJoypad[m_nCntPad]->Release();
			m_apDIDevJoypad[m_nCntPad] = NULL;
		}
		return DIENUM_CONTINUE;			// �񋓂𑱂���
	}

	// �f�o�C�X�̎��ʎq��ۑ�
	pad_discrimination[m_nCntPad] = lpddi->guidInstance;

	return DIENUM_STOP;		// ���̃f�o�C�X���g���̂ŗ񋓂��I������
}

BOOL CALLBACK CPad::EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	HRESULT hr;

	DIPROPRANGE diprg;

	// �X�e�B�b�N���̒l�͈̔͂�ݒ�i-32768�`32767�j
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -32768;
	diprg.lMax = 32768;

	if (m_apDIDevJoypad[m_nCntPad] != NULL)
	{
		hr = m_apDIDevJoypad[m_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (FAILED(hr))
		{
			return DIENUM_STOP;
		}
	}

	return DIENUM_CONTINUE;
}

//======================================================================================================================
// �W���C�p�b�h�̃L�[���ݒ�
//======================================================================================================================
void CPad::SetKeyStateJoypad(int nIDPad)
{
	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 225 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 315 * 100)
	{// �\���L�[[��]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = false;
	}

	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 45 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 135 * 100)
	{// �\���L�[[�E]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = false;
	}

	if ((m_aJoypadState[nIDPad].rgdwPOV[0] >= 315 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 360 * 100) || (m_aJoypadState[nIDPad].rgdwPOV[0] >= 0 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 45 * 100))
	{// �\���L�[[��]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = false;
	}

	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 135 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 225 * 100)
	{// �\���L�[[��]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = false;
	}

	for (int nKey = JOYPADKEY_X; nKey <= JOYPADKEY_START; nKey++)
	{
		if (m_aJoypadState[nIDPad].rgbButtons[nKey])
		{// �{�^����������Ă���
			m_aKeyStateJoypad[nIDPad][nKey] = true;
		}
		else
		{
			m_aKeyStateJoypad[nIDPad][nKey] = false;
		}
	}
}

//======================================================================================================================
// �W���C�p�b�h���擾�Q������
//======================================================================================================================
bool CPad::GetJoypadPress(int nIDPad, JOYPADKEY key)
{
	return m_aKeyStateJoypad[nIDPad][key];
}

//======================================================================================================================
// �W���C�p�b�h���擾�Q����
//======================================================================================================================
bool CPad::GetJoypadTrigger(int nIDPad, JOYPADKEY key)
{
	return m_aKeyStateTriggerJoypad[nIDPad][key];
}

//======================================================================================================================
// �W���C�p�b�h���擾�Q����
//======================================================================================================================
bool CPad::GetJoypadRelease(int nIDPad, JOYPADKEY key)
{
	return m_aKeyStateReleaseJoypad[nIDPad][key];
}

//======================================================================================================================
// �W���C�p�b�h���擾�Q�J��Ԃ�
//======================================================================================================================
bool CPad::GetJoypadRepeat(int nIDPad, JOYPADKEY key)
{
	return m_aKeyStateRepeatJoypad[nIDPad][key];
}

//======================================================================================================================
// �W���C�p�b�h�̃g���K�[(��)�̒l�擾
//======================================================================================================================
int CPad::GetJoypadTriggerLeft(int nIDPad)
{
	return m_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_LEFT_TRIGGER];
}

//======================================================================================================================
// �W���C�p�b�h�̃g���K�[(�E)�̒l�擾
//======================================================================================================================
int CPad::GetJoypadTriggerRight(int nIDPad)
{
	return m_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_RIGHT_TRIGGER];
}

//======================================================================================================================
// �W���C�p�b�h�̃A�i���O�X�e�B�b�N(��)�̒l�擾
//======================================================================================================================
void CPad::GetJoypadStickLeft(int nIDPad, float *pValueH, float *pValueV)
{
	*pValueH = (float)m_aJoypadState[nIDPad].lX;
	*pValueV = (float)-m_aJoypadState[nIDPad].lY;
}

//======================================================================================================================
// �W���C�p�b�h�̃A�i���O�X�e�B�b�N(�E)�̒l�擾
//======================================================================================================================
void CPad::GetJoypadStickRight(int nIDPad, float *pValueH, float *pValueV)
{
	*pValueH = (float)m_aJoypadState[nIDPad].lZ;
	*pValueV = (float)-m_aJoypadState[nIDPad].lRz;
}

// �R���X�g���N�^
CPad::CPad()
{

}

// �f�X�g���N�^
CPad::~CPad()
{

}
