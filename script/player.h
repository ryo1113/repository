//======================================================================================================================
//
// �v���C���[�̏���[player.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene2D.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define PLAYER_SIZE_X		(50)					// �|���S���̕�
#define PLAYER_SIZE_Y		(75)					// �|���S���̍���

//======================================================================================================================
// �N���X��`
//======================================================================================================================
class CPlayer : public CScene2D
{
public:
	CPlayer();
	~CPlayer();

	static HRESULT Load();
	static void Unload();
	static CPlayer *Create();

	enum PLAYERSTATE
	{//���L�������
		PLAYERSTATE_NORMAL = 0,			//�ʏ�
		PLAYERSTATE_DEATH,				//���񂾂Ƃ�
		PLAYERSTATE_MAX
	};

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	void MoveSpeedPlayer();
	void BulletShot();
	void ScreenLimit(D3DXVECTOR3 &pos);

	static LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXVECTOR3 m_move;
	int m_nType;
};

#endif