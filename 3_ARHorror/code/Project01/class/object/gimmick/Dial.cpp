#include "Dial.h"
#include "../../common/AnimationController.h"

constexpr float STOP_ANGLE = 36.0f;
constexpr float MAX_ANGLE = 360.0f;
constexpr float ROT_SPEED = 2.0f;

Dial::Dial(Vector3 pos, Vector3 angle, std::string name, int num)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	Init();
}

Dial::~Dial()
{
}

void Dial::Init(void)
{
	//停止角度
	stopAngle_ = 0.0f;

	//モデルパス
	path_ = "./resource/model/gimmick/Dial.mv1";

	//位置設定
	MV1SetPosition(lpModelMng.GetID(path_)[num_],pos_.toVECTOR());

	//拡大値設定
	scale_ = {0.5f,0.5f,0.5f };
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));

	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };

	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());
}

void Dial::Update(void)
{
	//インタラクトされたら
	if (interactFlg_)
	{
		//アニメーション再生
		isPlayFlg_ = true;
		interactFlg_ = false;
	}

	//停止中
	if (!isPlayFlg_)
	{
		//停止位置を決める
		stopAngle_ = angle_.x + Deg2RadF(STOP_ANGLE);

		//angle_が360度になったら
		if (angle_.x >= Deg2RadF(MAX_ANGLE))
		{
			//angle_の値が増えすぎないよう0に戻す
			angle_.x = 0.0f;
		}
		//現在のダイヤル番号(現在の角度/停止位置)
		dialNumber_ = static_cast<int>(angle_.x / Deg2RadF(STOP_ANGLE));
	}
	else	//進行中
	{
		//angle_が停止角度より小さいなら
		if(angle_.x <= stopAngle_)
		{
			//回転
			angle_.x += Deg2RadF(ROT_SPEED);
		}
		else	//停止角度を超えたら
		{
			//停止する
			isPlayFlg_ = false;
		}
 
	}
	//回転更新
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());

}

//描画
void Dial::Draw(void)
{
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
}

//開放
void Dial::Release(void)
{
}
//モデル番号を返す
int Dial::GetModelNumber(void)
{
	return num_;
}
//モデルも文字列を返す
std::string Dial::GetModelString(void)
{
	return path_;
}
