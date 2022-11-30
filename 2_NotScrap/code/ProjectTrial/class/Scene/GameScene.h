#pragma once
#include <memory>
#include <list>
#include<array>
#include "BaseScene.h"
#include "../Common/Geometry.h"
#include "../Obj/Obj.h"
#include"../../tmx/TmxObj.h"

constexpr int COIN_MAX = 3;
constexpr int DOOR_MAX = 2;

class Camera;
class Player;
class Enemy;
class EnemyShoot;
class Coin;
class PressurePlate;
class PressureDoor;
class ElectricPole;
class Gear;
class MagBox;
class Goal;

class GameScene :
	public BaseScene

{
public:
	GameScene();
	~GameScene();

	State GetPlayerState(void);
	Vector3 GetShakePlayer(void);
	int GetScreenID(void);
	std::vector<VERTEX2DSHADER>& GetTrailData(void);

	//�J�����̈ʒu�擾
	Vector3 GetCameraPos(void);

	//�J�����̒����_�ʒu�擾
	Vector3 GetCameraTargetPos(void);
private:
	//�֐�---------------------------------------
	bool Init(void)override;											//������
	void Init3D(void);

	uniqueScene Update(double delta, uniqueScene ownScene)override;		//�X�V

	void DrawOwnScreen(double delta)override;							//�`��

	Scene GetSceneID(void) override { return Scene::Game; };			//�V�[��ID��Ԃ�

	//�v���C���[��������
	std::shared_ptr<Player> DismemberPlayer(std::shared_ptr<Player>& player);

	//�v���C���[���S����
	std::shared_ptr<Player> DeathPlayer(std::shared_ptr<Player>& player);

	//�v���C���[�̍X�V
	uniqueScene UpdatePlayer(uniqueScene& ownScene);

	//�I�u�W�F�N�g����
	void CreateObject(void);
	void CreateGimmickObject(void);	//�M�~�b�N
	void CreateEneymyObject(void);	//�G
	void CreateCoinObject(void);	//�R�C��
	void CreateGoalObject(void);	//�S�[��
	void CreatePlayerObject(void);	//�v���C���[

	//�͈͓��`�F�b�N(�͈͓��Ftrue/�͈͊O�Ffalse)
	bool CheckWithinRange(Vector3 objpos, Vector3 playerpos);

	//�I�u�W�F�N�g�X�V
	uniqueScene UpdateObject(uniqueScene& ownScene);

	//�I�u�W�F�N�g�`��
	void DrawObject(void);

	//�c�ݕ`��
	void DrawDistortion(int ps, int screen, int tex1, const std::vector<VERTEX2DSHADER>& vertex);

	//�ϐ�----------------------------------------

	//�N���X���
	std::unique_ptr<PressurePlate> pressurePlate_;		//������
	std::unique_ptr<PressureDoor> pressureDoor_;		//�h�A
	std::unique_ptr<ElectricPole> electricPole_;		//�d�����u
	std::vector<std::unique_ptr<Goal>> goalList_;		//�S�[��
	std::vector<std::shared_ptr<Player>> playerList_;	//�v���C���[
	EneymyList enemyList_;								//�G
	GimmickList gimmickList_;							//�M�~�b�N

	MagBoxList magBoxList_;								//���΂̔�
	CoinList coinList_;									//�R�C��
	std::unique_ptr<Camera> camera_;					//�J����

	//���C�g�n���h��
	int lightHandle_;

	int ShadowMapHandle_;
	//�N���A����
	double clearTime_;
	double bestClearTime_;
	//�t�F�[�h�C������t���O
	bool fadeFlag_;

	//�V�F�[�_�n���h��
	int psCrackedScreen_;		//��ʊ���

	//�V�F�[�_�֌W
	int ps;		//�s�N�Z��
	int renderTarget_;	//�����_�����O�^�[�Q�b�g

	//�G�t�F�N�g
	Effect dust_;
	float Adjust;

	Vector2 pos;

	//�V�F�[�_�֌W
	int vsDistortionHandle_;	//�c��(�s�N�Z���V�F�[�_)
	int psDistortionHandle_;	//�c��(���_�V�F�[�_)
	//�萔
	int cBuff_;
	float* writer_;
};
