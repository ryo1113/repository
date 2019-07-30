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
#include "renderer.h"

class CPad;
class CKeyboard;
class CMouse;

//===========================================================================================================
// �}�N����`
//===========================================================================================================

//===========================================================================================================
// �N���X��`
//===========================================================================================================
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

protected:

private:
	static CRenderer *m_pRenderer;

	static CKeyboard *m_pInputKeyboard;
	static CPad *m_pInputPad;
	static CMouse *m_pInputMouse;
};

#endif