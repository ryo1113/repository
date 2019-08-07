//===========================================================================================================
//
// �����_�����O���� [manager.h]
// Author:RYO KANDA
//
//===========================================================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//===========================================================================================================
// �C���N���[�h�t�@�C��
//===========================================================================================================
#include "main.h"

class CRenderer;

class CPad;
class CKeyboard;
class CMouse;

//===========================================================================================================
// �}�N����`
//===========================================================================================================

//===========================================================================================================
// �N���X��`
//===========================================================================================================
class CModeBase
{
public:
	CModeBase() {}
	~CModeBase() {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

private:

};

class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();

	static CRenderer *GetRenderer();

	static CKeyboard *GetInputKeyboard();
	static CPad *GetInputPad();
	static CMouse *GetInputMouse();

	enum MODE
	{//��ʃ��[�h�̎��
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,
		MODE_MAX
	};

	static void SetMode(MODE mode);
	static MODE GetMode();

protected:

private:
	static MODE m_Mode;		//���݂̃��[�h
	static CModeBase *m_pMode;

	static CRenderer *m_pRenderer;

	static CKeyboard *m_pInputKeyboard;
	static CPad *m_pInputPad;
	static CMouse *m_pInputMouse;
};

#endif