//======================================================================================================================
//
// �e����[bullet.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "manager.h"

#include "bullet.h"
#include "effect.h"
#include "explosion.h"

#include "enemy.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define	BULLET_SPEED					(8.4f)
#define TURN_FRAME_BULLET				(3.0f)

#define ROT_LIMIT						(D3DXToRadian(90))

//======================================================================================================================
// �����o�ϐ�
//======================================================================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = {};
LPDIRECT3DVERTEXBUFFER9 CBullet::m_pVtxBuff = {};

CScene *CBullet::m_pBulletScene[MAX_BULLET] = {};
int CBullet::m_nNumAll = 0;

// �R���X�g���N�^
CBullet::CBullet() : CScene2D::CScene2D(OBJTYPE_BULLET, OBJECT_BULLET)
{
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (!m_pBulletScene[nCnt])
		{
			m_pBulletScene[nCnt] = this;

			m_nID = nCnt;
			this->m_nDrawIdx = nCnt;

			m_nNumAll++;
			break;
		}
	}

	if (!m_pVtxBuff)
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	}
}

// �f�X�g���N�^
CBullet::~CBullet()
{
	if (m_nNumAll == 1)
	{
		//���_�o�b�t�@�̊J��
		if (m_pVtxBuff != NULL)
		{
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}
	}
	m_nNumAll--;
}

HRESULT CBullet::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet_00.png", &m_pTexture);

	if (m_pTexture == NULL)
	{
		return -1;
	}

	return S_OK;
}

void CBullet::Unload()
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//======================================================================================================================
// ����
//======================================================================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type)
{
	if (!GetShotNum())
		return NULL;

	CBullet *pBullet;

	pBullet = new CBullet;

	D3DXVec3Normalize(&pBullet->m_move, &move);
	pBullet->SetPos(pos);
	pBullet->m_type = type;

	pBullet->Init();
	pBullet->BindTexture(NULL);

	return pBullet;
}

//======================================================================================================================
// ������
//======================================================================================================================
void CBullet::Init()
{
	m_nCntLife = 120;

	CScene2D::Init();

	SetTextureAnimationTex(1, 4);

	switch (m_type)
	{
	case CBullet::BULLETTYPE_NONE:

		SetSize(D3DXVECTOR3(40.0f, 40.0f, 0.0f));
		SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		CManager::SetSound(CSound::SOUND_LABEL_SE_SHOT_00);

		break;
	case CBullet::BULLETTYPE_HOMING:

		SetSize(D3DXVECTOR3(30.0f, 30.0f, 0.0f));
		CManager::SetSound(CSound::SOUND_LABEL_SE_SHOT_01);

		break;
	}
}

//======================================================================================================================
// �I������
//======================================================================================================================
void CBullet::Uninit()
{
	CScene2D::Uninit();

	this->Release(m_pBulletScene);
}

//======================================================================================================================
// �X�V����
//======================================================================================================================
void CBullet::Update()
{
	SetTextureAnimation(5);

	switch (m_type)
	{
	case CBullet::BULLETTYPE_NONE:

		this->MoveSpeed();

		// �G�t�F�N�g����
		CEffect::Create(GetPos(), CEffect::EFFECTTYPE_LASER);

		break;

	case CBullet::BULLETTYPE_HOMING:

		// �ǔ��e�̈ړ�
		m_move = this->TrackingBullet();
		SetPos(GetPos() + m_move);

		// �G�t�F�N�g����
		CEffect::Create(GetPos(), CEffect::EFFECTTYPE_NONE);

		break;
	}

	// �G�ɓ�����
	for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
	{
		CScene *pScene = GetScene(OBJTYPE_ENEMY, nCnt);

		if (!pScene)
			continue;

		CEnemyBase *pEnemy = (CEnemyBase*)pScene;

  		if (HitShapeCollision(pEnemy))
		{
			pEnemy->HitEnemy();

			if (m_type != BULLETTYPE_NONE)
			{
				this->Uninit();
				return;
			}
			break;
		}
	}

	if (--m_nCntLife == 0)
	{
		// ��������
		CExplosion::Create(GetPos());

		this->Uninit();
	}
}

//======================================================================================================================
// �`�揈��
//======================================================================================================================
void CBullet::Draw()
{

}

void CBullet::DrawAll()
{
	if (!m_pVtxBuff)
	{
		return;
	}

	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++, pVtx += 4)
	{
		if (m_pBulletScene[nCnt])
		{
			m_pBulletScene[nCnt]->Vertex(pVtx);
		}
	}

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (m_pBulletScene[nCnt])
		{
			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}

//======================================================================================================================
// �e�̌��E���𔻒�
//======================================================================================================================
bool CBullet::GetShotNum()
{
	if (MAX_BULLET > m_nNumAll)
		return true;

	return false;
}

//======================================================================================================================
// �ړ�����
//======================================================================================================================
void CBullet::MoveSpeed()
{
	D3DXVECTOR3 pos = GetPos();

	pos += m_move * BULLET_SPEED;

	SetPos(pos);
}

//======================================================================================================================
// �ǔ��e�̈ړ�
//======================================================================================================================
D3DXVECTOR3 CBullet::TrackingBullet()
{
	D3DXVECTOR3 rot = this->TrackingRot();

	SetDifRot(D3DXVECTOR3(0.0f, 0.0f, atan2f(rot.y, rot.x)));

	// �i�ޕ���
	Rot(TURN_FRAME_BULLET);

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	move.x = cosf(GetRot().z) * BULLET_SPEED;
	move.y = sinf(GetRot().z) * BULLET_SPEED;

	return move;
}

//======================================================================================================================
// �ǔ��e�̕���
//======================================================================================================================
D3DXVECTOR3 CBullet::TrackingRot()
{
	D3DXVECTOR3 Distance = D3DXVECTOR3(10000.0f, 0.0f, 0.0f);

	for (int nCnt = 0; nCnt < MAX_2D; nCnt++)
	{
		CScene *pScene = GetScene(OBJTYPE_ENEMY, nCnt);

		if (!pScene)
			continue;

		CEnemyBase *pEnemy = (CEnemyBase*)pScene;

		D3DXVECTOR3 Dis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVec3Subtract(&Dis, &pEnemy->GetPos(), &GetPos());

		if (D3DXVec3LengthSq(&Distance) > D3DXVec3LengthSq(&Dis) && fabs(atan2f(Dis.y, Dis.x) - GetRot().z) < ROT_LIMIT / 2.0f)
		{
			Distance = Dis;
		}
	}
	return Distance;
}
