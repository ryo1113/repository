//======================================================================================================================
//
// �T�E���h���� [sound.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//======================================================================================================================
// �N���X��`
//======================================================================================================================
class CSound
{
public:
	CSound() {}
	~CSound() {}

	// �T�E���h�t�@�C��
	enum SOUND_LABEL
	{
		SOUND_LABEL_BGM000 = 0,		// BGM title
		SOUND_LABEL_BGM001,			// BGM tutorial
		SOUND_LABEL_BGM002,			// BGM game
		SOUND_LABEL_BGM003,			// BGM result
		SOUND_LABEL_SE_SHOT_00,		// �V���b�g0
		SOUND_LABEL_SE_SHOT_01,		// �V���b�g1
		SOUND_LABEL_SE_ENEMYHIT,	// �G�q�b�g
		SOUND_LABEL_SE_ENTER,		// �G���^�[
		SOUND_LABEL_SE_SELECT,		// �I����
		SOUND_LABEL_MAX,
	};

	HRESULT InitSound(HWND hWnd);
	void UninitSound();

	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound();
	
private:
	struct SOUNDPARAM
	{// �p�����[�^�\���̒�`
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	};

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = {};										// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = {};					// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};		// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};						// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};						// �I�[�f�B�I�f�[�^�T�C�Y

	// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title.wav", -1 },			// BGM title
		{ "data/BGM/tutorial.wav", -1 },		// BGM tutorial
		{ "data/BGM/game.wav", -1 },			// BGM game
		{ "data/BGM/resrut.wav", -1 },			// BGM result
		{ "data/SE/shot_00.wav",0 },			// �V���b�g0
		{ "data/SE/shot_01.wav",0 },			// �V���b�g1
		{ "data/SE/enemyhit.wav",0 },			// �G�q�b�g
		{ "data/SE/enter.wav",0 },				// �G���^�[
		{ "data/SE/pauseselect.wav",0}			// �I����
	};
};

#endif
