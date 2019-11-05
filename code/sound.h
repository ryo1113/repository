//======================================================================================================================
//
// サウンド処理 [sound.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CSound
{
public:
	CSound() {}
	~CSound() {}

	// サウンドファイル
	enum SOUND_LABEL
	{
		SOUND_LABEL_BGM000 = 0,		// BGM title
		SOUND_LABEL_BGM001,			// BGM tutorial
		SOUND_LABEL_BGM002,			// BGM game
		SOUND_LABEL_BGM003,			// BGM result
		SOUND_LABEL_SE_SHOT_00,		// ショット0
		SOUND_LABEL_SE_SHOT_01,		// ショット1
		SOUND_LABEL_SE_ENEMYHIT,	// 敵ヒット
		SOUND_LABEL_SE_ENTER,		// エンター
		SOUND_LABEL_SE_SELECT,		// 選択音
		SOUND_LABEL_MAX,
	};

	HRESULT InitSound(HWND hWnd);
	void UninitSound();

	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound();
	
private:
	struct SOUNDPARAM
	{// パラメータ構造体定義
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	};

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = {};										// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = {};					// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};		// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};						// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};						// オーディオデータサイズ

	// 各音素材のパラメータ
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title.wav", -1 },			// BGM title
		{ "data/BGM/tutorial.wav", -1 },		// BGM tutorial
		{ "data/BGM/game.wav", -1 },			// BGM game
		{ "data/BGM/resrut.wav", -1 },			// BGM result
		{ "data/SE/shot_00.wav",0 },			// ショット0
		{ "data/SE/shot_01.wav",0 },			// ショット1
		{ "data/SE/enemyhit.wav",0 },			// 敵ヒット
		{ "data/SE/enter.wav",0 },				// エンター
		{ "data/SE/pauseselect.wav",0}			// 選択音
	};
};

#endif
