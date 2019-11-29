//======================================================================================================================
//
// �G�̏���[enemy.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"
#include "enemy.h"

#include "game.h"
#include "camera.h"

#include "bullet.h"
#include "explosion.h"
#include "player.h"

#include "score.h"
#include "number.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================

//======================================================================================================================
// �����o�ϐ�
//======================================================================================================================
LPDIRECT3DTEXTURE9 CEnemyBase::m_pTexture[ENEMYTYPE_MAX] = {};
int CEnemyBase::nNumEnemy = 0;


// �R���X�g���N�^
CEnemyBase::CEnemyBase() : CScene2D::CScene2D(OBJTYPE_ENEMY)
{
	nNumEnemy++;
}

// �f�X�g���N�^
CEnemyBase::~CEnemyBase()
{

}

HRESULT CEnemyBase::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_00.png", &m_pTexture[ENEMYTYPE_PART1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_01.png", &m_pTexture[ENEMYTYPE_PART2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_02.png", &m_pTexture[ENEMYTYPE_PART3]);

	for (int nCnt = 0; nCnt < ENEMYTYPE_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] == NULL)
		{
			return -1;
		}
	}

	return S_OK;
}

void CEnemyBase::Unload()
{
	for (int nCnt = 0; nCnt < ENEMYTYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//======================================================================================================================
// ������
//======================================================================================================================
void CEnemyBase::Init()
{
	CScene2D::Init();

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nTime = CManager::Random(0, 360);
}

//======================================================================================================================
// �I������
//======================================================================================================================
void CEnemyBase::Uninit()
{
	nNumEnemy--;

	CScene2D::Uninit();

	this->Release();
}

//======================================================================================================================
// �X�V����
//======================================================================================================================
void CEnemyBase::Update()
{
	m_nTime++;

	// �ړ��X�V
	D3DXVECTOR3 pos = GetPos();

	m_move.x -= m_move.x * 1 / 4;
	m_move.y -= m_move.y * 1 / 4;

	if (fabsf(m_move.x) < 0.1f)
	{
		m_move.x = 0;
	}
	if (fabsf(m_move.y) < 0.1f)
	{
		m_move.y = 0;
	}

	SetPos(pos + m_move);

	if (GetPos().x + GetSize().x <= 0.0f - CCamera::GetCamera()->x || fabsf(GetPos().y - SCREEN_HEIGHT / 2) >= SCREEN_HEIGHT)
	{// ��ʊO�i���j
		this->Uninit();
	}
}

//======================================================================================================================
// �`�揈��
//======================================================================================================================
void CEnemyBase::Draw()
{
	CScene2D::Draw();
}

//======================================================================================================================
// ��e����
//======================================================================================================================
void CEnemyBase::HitEnemy(bool bscore)
{
	// ��������
	CExplosion::Create(GetPos());
	CManager::SetSound(CSound::SOUND_LABEL_SE_ENEMYHIT);

	if (bscore)
	{// ���Z�X�R�A
		int nScore = 0;
		D3DXCOLOR col;

		CNumber *pScore = CScore::GetScore();

		switch (m_Type)
		{
		case CEnemyBase::ENEMYTYPE_PART1:
			nScore = 2000;
			col = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
			break;

		case CEnemyBase::ENEMYTYPE_PART2:
			nScore = 10000;
			col = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);
			break;

		case CEnemyBase::ENEMYTYPE_PART3:
			nScore = 500;
			col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
		
		((CScore*)pScore)->AddScore(nScore);

		// �X�R�A�̌���MAX_DIGIT�ȏ�ɂ��Ȃ�
		CGame::ScorePlusUI(GetPos() + *CCamera::GetCamera(), col, nScore);
	}

	this->Uninit();
}

//======================================================================================================================
// �^�C�v�擾
//======================================================================================================================
CEnemyBase::ENEMYTYPE CEnemyBase::GetType()
{
	return m_Type;
}


//======================================================================================================================
// �x�N�^�[�R�̂��ꂼ��̊|���Z
//======================================================================================================================
D3DXVECTOR3 CEnemyBase::Vec3xVec3(D3DXVECTOR3 A, D3DXVECTOR3 B)
{
	D3DXVECTOR3 Vec;

	Vec.x = A.x * B.x;
	Vec.y = A.y * B.y;
	Vec.z = A.z * B.z;

	return Vec;
}

//======================================================================================================================
// �G���̎擾
//======================================================================================================================
int CEnemyBase::GetNumEnemy()
{
	return nNumEnemy;
}
