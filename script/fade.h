//======================================================================================================================
//
// �t�F�[�h���� [fade.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _FADE_H_
#define _FADE_H_

//======================================================================================================================
// �C���N���[�h�t�@�C��
//======================================================================================================================
#include "manager.h"

//======================================================================================================================
// �N���X��`
//======================================================================================================================
class CFade
{
public:
	CFade() {}
	~CFade() {}

	enum FADE
	{
		FADE_NONE = 0,		// �����Ȃ����
		FADE_IN,			// �t�F�[�h�C������
		FADE_OUT,			// �t�F�[�h�A�E�g����
		FADE_MAX
	};
	static CFade *Create(CManager::MODE modeNext);

	void InitFade(CManager::MODE modeNext);
	void UninitFade();
	void UpdateFade();
	void DrawFade();

	FADE GetFade();

private:
	void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffFade = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	FADE					m_fade;						// �t�F�[�h���
	CManager::MODE			m_modeNext;					// ���̉�ʁi���[�h�j
	D3DXCOLOR				m_colorFade;				// �t�F�[�h�F
};

#endif
