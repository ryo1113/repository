//======================================================================================================================
//
// �Q�[���p�b�h���� [pad.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _PAD_H_
#define _PAD_H_

//======================================================================================================================
// �C���N���[�h�t�@�C��
//======================================================================================================================
#include "input.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define	COUNT_WAIT_REPEAT	(20)	// ���s�[�g�J�n�܂ł̑҂�����
#define	INTERVAL_REPEAT		(1)		// ���s�[�g�Ԋu

#define NUM_JOYPAD_MAX		(1)		// �p�b�h�ő吔
#define JOY_MAX_RANGE		(1024)	// �p�b�h�L���͈�

//======================================================================================================================
// �N���X��`
//======================================================================================================================
class CPad : public CInput
{
public:
	CPad();
	~CPad();

	//===========================================================================================================
	// �L�[�̎��
	//===========================================================================================================
	enum JOYPADKEY
	{
		JOYPADKEY_X = 0,			// [0] �w�{�^��
		JOYPADKEY_Y,				// [1] �x�{�^��
		JOYPADKEY_A,				// [2] �`�{�^��
		JOYPADKEY_B,				// [3] �a�{�^��
		JOYPADKEY_LEFT_SHOULDER,	// [4] ����O�{�^��
		JOYPADKEY_RIGHT_SHOULDER,	// [5] �E��O�{�^��
		JOYPADKEY_LEFT_TRIGGER,		// [6] ���g���K�[
		JOYPADKEY_RIGHT_TRIGGER,	// [7] �E�g���K�[
		JOYPADKEY_LEFT_THUMB,		// [8] ���X�e�B�b�N
		JOYPADKEY_RIGHT_THUMB,		// [9] �E�X�e�B�b�N
		JOYPADKEY_BACK,				// [10] �o�b�N�{�^��
		JOYPADKEY_START,			// [11] �X�^�[�g�{�^��
		JOYPADKEY_LEFT,				// [12] �����L�[[��]
		JOYPADKEY_RIGHT,			// [13] �����L�[[�E]
		JOYPADKEY_UP,				// [14] �����L�[[��]
		JOYPADKEY_DOWN,				// [15] �����L�[[��]
		JOYPADKEY_MAX				// [16] �L�[�̎�ސ�
	};

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetJoypadPress(int nIDPad, JOYPADKEY key);
	bool GetJoypadTrigger(int nIDPad, JOYPADKEY key);
	bool GetJoypadRelease(int nIDPad, JOYPADKEY key);
	bool GetJoypadRepeat(int nIDPad, JOYPADKEY key);

	int GetJoypadTriggerLeft(int nIDPad);
	int GetJoypadTriggerRight(int nIDPad);

	void GetJoypadStickLeft(int nIDPad, float *pValueH, float *pValueV);
	void GetJoypadStickRight(int nIDPad, float *pValueH, float *pValueV);

private:
	static BOOL CALLBACK EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);
	static BOOL CALLBACK EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

	void SetKeyStateJoypad(int nIDPad);

	BYTE							m_aJoyKeyState[JOYPADKEY_MAX];			// �W���C�p�b�h�̓��͏�񃏁[�N
	int								m_nCountWaitRepeat = 0;					// ���s�[�g�҂�����
	static int						m_nIntervalRepeat;						// ���s�[�g�Ԋu

	static LPDIRECTINPUTDEVICE8		m_apDIDevJoypad[NUM_JOYPAD_MAX];							// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	DIJOYSTATE2						m_aJoypadState[NUM_JOYPAD_MAX];								// �W���C�p�b�h��Ԃ��󂯎�郏�[�N
	bool							m_aKeyStateJoypad[NUM_JOYPAD_MAX][JOYPADKEY_MAX];				// �W���C�p�b�h�̉�����Ԃ�ێ����郏�[�N
	bool							m_aKeyStateTriggerJoypad[NUM_JOYPAD_MAX][JOYPADKEY_MAX];		// �W���C�p�b�h�̃g���K�[��Ԃ�ێ����郏�[�N
	bool							m_aKeyStateReleaseJoypad[NUM_JOYPAD_MAX][JOYPADKEY_MAX];		// �W���C�p�b�h�̃����[�X��Ԃ�ێ����郏�[�N
	bool							m_aKeyStateRepeatJoypad[NUM_JOYPAD_MAX][JOYPADKEY_MAX];		// �W���C�p�b�h�̃��s�[�g��Ԃ�ێ����郏�[�N
	int								m_aKeyStateRepeatCntJoypad[NUM_JOYPAD_MAX][JOYPADKEY_MAX];	// �W���C�p�b�h�̃��s�[�g�J�E���^
	D3DXVECTOR3						m_aKeyStateAxis[NUM_JOYPAD_MAX];
	static int						m_nCntPad;
};

#endif