//======================================================================================================================
//
// �Q�c�P���`�揈�� [scene.cpp]
// Author:RYO KANDA
//
//======================================================================================================================
#include "scene2D.h"
#include "manager.h"

#include "bullet.h"

#include "camera.h"

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define ROTDIF_FRAME(num)			(num / 60.00f)		// �X���[�Y�ɉ�]���鑬�x

//======================================================================================================================
// �����o�ϐ�
//======================================================================================================================

// �R���X�g���N�^
CScene2D::CScene2D(OBJTYPE objtype, OBJECT obj) : CScene::CScene(objtype, obj)
{
	if (obj == OBJECT_NONE)
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	}
}

// �f�X�g���N�^
CScene2D::~CScene2D()
{

}

CScene2D *CScene2D::Create()
{
	CScene2D *pScene;

	pScene = new CScene2D;

	pScene->Init();

	return pScene;
}

//======================================================================================================================
// ����������
//======================================================================================================================
void CScene2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 0.0f);

	m_rotDifference = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_fLength = sqrtf((m_size.x / 2 * m_size.x / 2) + (m_size.y / 2 * m_size.y / 2));

	//�e�N�X�`���A�j���[�V�����Ǘ��ϐ�
	m_nHeight = 1;
	m_nWide = 1;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	this->pMoveTex = NULL;

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �ԁA�΁A��	�A�A���t�@�l
}


//======================================================================================================================
// �I������
//======================================================================================================================
void CScene2D::Uninit()
{
	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//======================================================================================================================
// �X�V����
//======================================================================================================================
void CScene2D::Update()
{

}

//======================================================================================================================
// �`�揈��
//======================================================================================================================
void CScene2D::Draw()
{
	SetVertex();

	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//======================================================================================================================
// �����蔻��(�~�Ɖ~)
//======================================================================================================================
bool CScene2D::HitShapeCollision(CScene2D *scene2D)
{	
	// ���a
	float fRadiusA = (m_size.x * m_scale.x + m_size.y * m_scale.y) / 3.0f;

	float fRadiusB;

	// �e�N�X�`���̗]�����߂̂���
	if (scene2D->m_size.x > scene2D->m_size.y)
	{
		fRadiusB = (scene2D->m_size.y * scene2D->m_scale.y) / 2.0f;
	}
	else
	{
		fRadiusB = (scene2D->m_size.x * scene2D->m_scale.x) / 2.0f;
	}

	float fRadius = fRadiusA + fRadiusB;
	
	// ���a+���a��艓������
	if (D3DXVec3LengthSq(&(m_pos - scene2D->m_pos)) > fRadius * fRadius)
	{
		return false;
	}

	return true;
}

//======================================================================================================================
// �����蔻��(�l�p�Ɖ~)
//======================================================================================================================
bool CScene2D::HitBoxCollision(CScene2D *scene2D)
{
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[4] =
	{
		pVtx[0].pos,
		pVtx[1].pos,
		pVtx[3].pos,
		pVtx[2].pos
	};

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	// �����蔻�蒲��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pos[nCnt].x -= (pos[nCnt].x - GetPos().x) / 1.5f;
		pos[nCnt].y -= (pos[nCnt].y - GetPos().y) / 1.1f;
	}

	D3DXVECTOR3 dif = {};
	D3DXVECTOR3 posdif = {};

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		posdif = scene2D->GetPos() - pos[nCnt];

		// �~�̒��ɒ��_���Ȃ���
		if (scene2D->GetLength() * scene2D->GetLength() >= D3DXVec3LengthSq(&posdif))
		{
			return true;
		}
	}

	// �����`�̒��ɉ~�̒��S�����邩
	float fRot[2] = {};

	posdif = scene2D->GetPos() - pos[3];
	dif = pos[0] - pos[3];

	fRot[0] = atan2f(D3DXVec2CCW(&(D3DXVECTOR2)posdif, &(D3DXVECTOR2)dif), D3DXVec3Dot(&posdif, &dif));

	posdif = scene2D->GetPos() - pos[1];
	dif = pos[2] - pos[1];

	fRot[1] = atan2f(D3DXVec2CCW(&(D3DXVECTOR2)posdif, &(D3DXVECTOR2)dif), D3DXVec3Dot(&posdif, &dif));

	if (0 <= fRot[0] && fRot[0] <= D3DX_PI / 2.0f && 0 <= fRot[1] && fRot[1] <= D3DX_PI / 2)
	{
		return true;
	}

	// �����Ɖ~�̔���
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		posdif = pos[nCnt] - scene2D->GetPos();

		dif = pos[(nCnt + 1) % 4] - pos[nCnt];

		float fDif = -D3DXVec3Dot(&dif, &posdif) / D3DXVec3LengthSq(&dif);

		D3DXVec3Add(&dif, &pos[nCnt], &(dif * ((fDif < 0) ? 0 : (fDif > 1) ? 1 : fDif)));

		fDif = D3DXVec3LengthSq(&(scene2D->GetPos() - dif));

		if (fDif < (scene2D->GetLength() * scene2D->GetLength()))
		{
  			return true;
		}
	}

	return false;
}

//======================================================================================================================
// �������Ƃ������(�摜)
//======================================================================================================================
void CScene2D::Rot(float num)
{
	// �����܂ł̊p�x
	m_rotDifference.z = m_rotDirection.z - m_rot.z;

	// 180�x�ȏ��]�̎�
	Marginalize(&m_rotDifference.z);

	// ���������ɂ���
	if (fabsf(m_rotDifference.z) <= D3DXToRadian(3))
	{
		m_rot.z = m_rotDirection.z;
	}
	else
	{
		m_rot.z += m_rotDifference.z * ROTDIF_FRAME(num);
	}

	// 360�x�ȏ��]���Ȃ��悤��(�����I��)
	Marginalize(&m_rot.z);
}

//======================================================================================================================
// ��]�̌��E(����肵�Ȃ��悤)
//======================================================================================================================
void CScene2D::Marginalize(float *rot)
{
	if (*rot > D3DX_PI)
	{
		*rot -= D3DX_PI * 2;
	}
	if (*rot < -D3DX_PI)
	{
		*rot += D3DX_PI * 2;
	}
}

//======================================================================================================================
// �����Ȃ��Ƃ��ŏ���
//======================================================================================================================
bool CScene2D::UnlookJudge()
{
	if (m_pos.x + m_fLength * m_scale.x < 0.0f		   - CCamera::GetCamera()->x ||
		m_pos.x - m_fLength * m_scale.x > SCREEN_WIDTH - CCamera::GetCamera()->x ||
		m_pos.y + m_fLength * m_scale.y < 0.0f ||
		m_pos.y - m_fLength * m_scale.y > SCREEN_HEIGHT)
	{
		return true;
	}
	return false;
}

//======================================================================================================================
// �L�[�{�[�h�ړ�
//======================================================================================================================
D3DXVECTOR3 CScene2D::MoveVector(D3DXVECTOR3 vector, float fSpeed)
{
	D3DXVECTOR3 NormalizeMove;

	//���K������
	D3DXVec3Normalize(&NormalizeMove, &vector);

	//�i�ދ���
	NormalizeMove *= fSpeed;

	return NormalizeMove;
}

//======================================================================================================================
// �Q�[���p�b�h�ړ�
//======================================================================================================================
D3DXVECTOR3 CScene2D::Move(float fAngle, float fSpeed)
{
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���W�A���ł̌v�Z
	move.x += sinf(fAngle) * fSpeed;
	move.y -= cosf(fAngle) * fSpeed;

	return move;
}

//======================================================================================================================
// �}�E�X�ړ�
//======================================================================================================================
D3DXVECTOR3 CScene2D::MoveMouse(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 move = pos - m_pos;

	return move;
}

//======================================================================================================================
// Set
//======================================================================================================================

//------------------------------------------------------------------------------
//	�J���[
//------------------------------------------------------------------------------
void CScene2D::SetCollar(D3DXCOLOR collar)
{
	this->m_col = collar;
}

//------------------------------------------------------------------------------
// �e�N�X�`��
//------------------------------------------------------------------------------
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 Texture)
{
	this->m_pTexture = Texture;
}

//------------------------------------------------------------------------------
// ���_���W�X�V
//------------------------------------------------------------------------------
void CScene2D::Vertex(VERTEX_2D *pVtx)
{
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = m_col;	// �ԁA�΁A��	�A�A���t�@�l
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	if (this->pMoveTex)
	{
		//���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI),
								  m_pos.y + (m_fLength * m_scale.y) * cosf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI), 0.0f);

		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI),
								  m_pos.y + (m_fLength * m_scale.y) * cosf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI), 0.0f);
		
		pVtx[2].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z),
								  m_pos.y + (m_fLength * m_scale.y) * cosf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z), 0.0f);

		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z),
								  m_pos.y + (m_fLength * m_scale.y) * cosf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z), 0.0f);

		this->SetVertexTex_Scroll(pVtx);
	}
	else
	{
		//���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI) + CCamera::GetCamera()->x,
								  m_pos.y + (m_fLength * m_scale.y) * cosf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI), 0.0f);

		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI) + CCamera::GetCamera()->x,
								  m_pos.y + (m_fLength * m_scale.y) * cosf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z + D3DX_PI), 0.0f);

		pVtx[2].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z) + CCamera::GetCamera()->x,
								  m_pos.y + (m_fLength * m_scale.y) * cosf(-atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z), 0.0f);

		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_fLength * m_scale.x) * sinf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z) + CCamera::GetCamera()->x,
								  m_pos.y + (m_fLength * m_scale.y) * cosf(atan2f(m_size.x / 2, m_size.y / 2) + m_rot.z), 0.0f);

		this->SetVertexTex(pVtx);
	}
}

//------------------------------------------------------------------------------
// ���_���W�X�V
//------------------------------------------------------------------------------
void CScene2D::SetVertex()
{
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	Vertex(pVtx);
	
	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

void CScene2D::SetVertexTex(VERTEX_2D * pVtx)
{
	//�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2( m_nPatternAnim % m_nWide      * (1.0f / m_nWide), (m_nPatternAnim / m_nWide) * (1.0f / m_nHeight));
	pVtx[1].tex = D3DXVECTOR2((m_nPatternAnim % m_nWide + 1) * (1.0f / m_nWide), (m_nPatternAnim / m_nWide) * (1.0f / m_nHeight));
	pVtx[2].tex = D3DXVECTOR2( m_nPatternAnim % m_nWide      * (1.0f / m_nWide), (m_nPatternAnim / m_nWide) * (1.0f / m_nHeight) + (1.0f / m_nHeight));
	pVtx[3].tex = D3DXVECTOR2((m_nPatternAnim % m_nWide + 1) * (1.0f / m_nWide), (m_nPatternAnim / m_nWide) * (1.0f / m_nHeight) + (1.0f / m_nHeight));
}

void CScene2D::SetVertexTex_Scroll(VERTEX_2D * pVtx)
{
	//�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(this->pMoveTex->x + 0.0f, this->pMoveTex->y + 0.0f);
	pVtx[1].tex = D3DXVECTOR2(this->pMoveTex->x + 1.0f, this->pMoveTex->y + 0.0f);
	pVtx[2].tex = D3DXVECTOR2(this->pMoveTex->x + 0.0f, this->pMoveTex->y + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(this->pMoveTex->x + 1.0f, this->pMoveTex->y + 1.0f);
}


void CScene2D::BindMoveTex(D3DXVECTOR3 * pMTex)
{
	this->pMoveTex = pMTex;
}

//------------------------------------------------------------------------------
// �e�N�X�`���A�j���[�V�����̏�����
//------------------------------------------------------------------------------
void CScene2D::SetTextureAnimationTex(int nHeight, int nWide)
{
	m_nHeight = nHeight;
	m_nWide = nWide;
}

//------------------------------------------------------------------------------
// �e�N�X�`���A�j���[�V����
//------------------------------------------------------------------------------
bool CScene2D::SetTextureAnimation(int nFrame, bool bLoop)
{
	if (++m_nCounterAnim % nFrame == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % (m_nHeight * m_nWide);

		if (m_nPatternAnim % (m_nHeight * m_nWide) == 0 && !bLoop)
		{
			return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
// �ꏊ
//------------------------------------------------------------------------------
void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//------------------------------------------------------------------------------
// ��](�������)
//------------------------------------------------------------------------------
void CScene2D::SetDifRot(D3DXVECTOR3 rot)
{
	m_rotDirection = rot;
}

//------------------------------------------------------------------------------
// ��]
//------------------------------------------------------------------------------
void CScene2D::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//------------------------------------------------------------------------------
// �X�P�[��
//------------------------------------------------------------------------------
void CScene2D::SetScale(D3DXVECTOR3 scale)
{
	m_scale.x = scale.x;
	m_scale.y = scale.y;
	m_scale.z = scale.z;
}

//------------------------------------------------------------------------------
// �T�C�Y
//------------------------------------------------------------------------------
void CScene2D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;

	m_fLength = sqrtf((m_size.x / 2 * m_size.x / 2) + (m_size.y / 2 * m_size.y / 2));
}

//======================================================================================================================
// Get
//======================================================================================================================

//------------------------------------------------------------------------------
// �ꏊ
//------------------------------------------------------------------------------
D3DXVECTOR3 CScene2D::GetPos()
{
	return m_pos;
}

//------------------------------------------------------------------------------
// ��]
//------------------------------------------------------------------------------
D3DXVECTOR3 CScene2D::GetRot()
{
	return m_rot;
}

//------------------------------------------------------------------------------
// �T�C�Y
//------------------------------------------------------------------------------
D3DXVECTOR3 CScene2D::GetSize()
{
	return m_size;
}

//------------------------------------------------------------------------------
// �X�P�[��
//------------------------------------------------------------------------------
D3DXVECTOR3 CScene2D::GetScale()
{
	return m_scale;
}

//------------------------------------------------------------------------------
// �J���[
//------------------------------------------------------------------------------
D3DXCOLOR CScene2D::GetCollar()
{
	return m_col;
}

//------------------------------------------------------------------------------
// ���a�̒���
//------------------------------------------------------------------------------
float CScene2D::GetLength()
{
	return m_fLength;
}