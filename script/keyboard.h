//===========================================================================================================
//
// ���͏��� [keyboard.h]
// Author:RYO KANDA
//
//===========================================================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//===========================================================================================================
// �C���N���[�h�t�@�C��
//===========================================================================================================
#include "main.h"
#include "input.h"

//===========================================================================================================
// �}�N����`
//===========================================================================================================
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔

//===========================================================================================================
// �N���X��`
//===========================================================================================================
class CKeyboard : public CInput
{
public:
	CKeyboard();
	~CKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);
	bool GetKeyboardRepeat(int nKey);
	bool GetKeyboardRelease(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̃����[�X��񃏁[�N
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̃��s�[�g��񃏁[�N

	int	m_aKeyStateRepeatCnt[NUM_KEY_MAX];		// �L�[�{�[�h�̃��s�[�g�J�E���^

};

#endif
