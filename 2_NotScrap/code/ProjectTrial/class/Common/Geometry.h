#pragma once
#include<DxLib.h>
#include<btBulletDynamicsCommon.h>

//�T�C�Y��\���\����
struct Size {
	float w;//��
	float h;//����
};

//���W��\���\����
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(float inx,float iny):x(inx),y(iny){}
	float x, y;
	///�x�N�g���̑傫����Ԃ��܂�
	float Magnitude()const;
	
	///���K��(�傫�����P��)���܂�
	void Normalize();

	///���K���x�N�g����Ԃ��܂�
	Vector2 Normalized();

	void operator+=(const Vector2& v);
	void operator+=(const float v);

	void operator-=(const Vector2& v);
	void operator*=(float scale);
	Vector2 operator*(float scale);
	Vector2 operator*(Vector2& vec);

	Vector2 operator/(float scale);
	Vector2 operator-() {
		return Vector2(-x, -y);
	}

	//��r���Z�q
	bool operator ==(const Vector2& vec)const;
	bool operator !=(const Vector2& vec)const;
	bool operator <=(const Vector2& vec)const;
	bool operator <(const Vector2& vec)const;
	bool operator >=(const Vector2& vec)const;
	bool operator >(const Vector2& vec)const;

};

struct Vector2Int {
	Vector2Int():x(0),y(0){}
	Vector2Int(int inx,int iny):x(inx),y(iny){}
	int x, y;

	void operator+=(const Vector2Int& v);
	void operator-=(const Vector2Int& v);
	void operator*=(int scale);
	Vector2Int operator+(const Vector2Int& v);
	Vector2Int operator-(const Vector2Int& v);
	Vector2Int operator*(int scale);
	Vector2Int operator-() {
		return Vector2Int(-x, -y);
	}
	Vector2Int operator+() {
		return Vector2Int(+x, +y);
	}

	//��r���Z�q
	bool operator ==(const Vector2Int& vec)const;
	bool operator !=(const Vector2Int& vec)const;
	bool operator <=(const Vector2Int& vec)const;
	bool operator <(const Vector2Int& vec)const;
	bool operator >=(const Vector2Int& vec)const;
	bool operator >(const Vector2Int& vec)const;
};

Vector2 operator+(const Vector2& va, const Vector2 vb);
Vector2 operator-(const Vector2& va, const Vector2 vb);

struct Vector3 {
	Vector3() :x(0), y(0), z(0) {}
	Vector3(float inx, float iny,float inz) :x(inx), y(iny) ,z(inz) {}
	float x, y, z;

	///�x�N�g���̑傫����Ԃ��܂�
	float Magnitude()const;
	
	///���K��(�傫�����P��)���܂�
	void Normalize();

	///���K���x�N�g����Ԃ��܂�
	Vector3 Normalized();
	Vector3 toDiopter();
	VECTOR toVECTOR();
	btVector3 tobtVec();
	void operator+=(const Vector3& v);
	void operator-=(const Vector3& v);
	void operator*=(float scale);
	Vector3 operator*(float scale);
	Vector3 operator*(Vector3& vec);
	Vector3 operator=(const Vector3& v);

	Vector3 operator/(float scale);
	Vector3 operator-() {
		return Vector3(-x, -y,-z);
	}

	//��r���Z�q
	bool operator ==(const Vector3& vec)const;
	bool operator !=(const Vector3& vec)const;
	bool operator <=(const Vector3& vec)const;
	bool operator <(const Vector3& vec)const;
	bool operator >=(const Vector3& vec)const;
	bool operator >(const Vector3& vec)const;

};
Vector3 operator+(const Vector3& va, const Vector3 vb);
Vector3 operator-(const Vector3& va, const Vector3 vb);

struct Line
{
	Vector3 p;		//�n�_
	Vector3 end;	//�I�_
	Vector3 vec();
};



///���ς�Ԃ�
float Dot(const Vector2& va, const Vector2& vb);

///�O�ς�Ԃ�
float Cross(const Vector2& va, const Vector2& vb);

///���ς�Ԃ�
float Dot(const Vector3& va, const Vector3& vb);

///�O�ς�Ԃ�
Vector3 Cross(const Vector3& va, const Vector3& vb);

//Vector3�ɕϊ�
Vector3 ConvertVECTORtoVector3(const VECTOR& v3);

double ClampD(double value, double minValue = 0.0, double maxValue = 1000.0);

///���ω��Z�q
float operator*(const Vector2& va, const Vector2& vb);

///�O�ω��Z�q
float operator%(const Vector2& va, const Vector2& vb);

//�Ƃ肠�����u���W�v���ĈӖ����ƃx�N�^���
//Position�̂ق����悭�ˁH���ė��R�ł��̖��O
typedef Vector2 Position2;
typedef Vector3 Position3;


//�~
struct Circle {
	float radius;//���a
	Position2 pos; //���S���W
	Circle() :radius(0), pos(0, 0) {}
	Circle(float r, const Position2& p) :radius(r), pos(p) {}
};
//
/////����\���\����
//struct Sphere {
//	float radius;//���a
//	Position3 pos; //���S���W
//	Sphere() :radius(0), pos(0, 0, 0) {}
//	Sphere(float r, Position3& p) :radius(r), pos(p) {}
//};
////�J�v�Z��
//struct Capsule {
//	float radius;//���a
//	Position2 posA; //�[�_A
//	Position2 posB; //�[�_B
//	Capsule() :radius(0), posA(0, 0), posB(0, 0) {}
//	Capsule(float r, const Position2& a, const Position2& b):
//		radius(r), posA(a), posB(b) {}
//	Capsule(float r, float ax,float ay, float bx,float by) :
//		radius(r), posA(ax,ay), posB(bx,by) {}
//};

struct Rect {
	Position2 pos; //���S���W
	int w, h;//��,����
	Rect() : pos(0, 0), w(0), h(0) {}
	Rect(float x, float y, int inw, int inh) :
		pos(x, y), w(inw), h(inh) {}
	Rect(Position2& inpos, int inw, int inh) :
		pos(inpos), w(inw), h(inh)
	{}
	void SetCenter(float x, float y) {
		pos.x = x;
		pos.y = y;
	}
	void SetCenter(const Position2& inpos) {
		pos.x = inpos.x;
		pos.y = inpos.y;
	}
	Vector2 Center() {
		return pos;
	}
	float Left() { return pos.x - w / 2; }
	float Top() { return pos.y - h / 2; }
	float Right() { return pos.x + w / 2; }
	float Bottom() { return pos.y + h / 2; }
	void Draw();//�����̋�`��`�悷��
	void Draw(Vector2& offset);//�����̋�`��`�悷��(�I�t�Z�b�g�t��)
};

struct Matrix {
	float m[3][3];
};

///�P�ʍs���Ԃ�
Matrix IdentityMat();

///���s�ړ��s���Ԃ�
///@param x X�������s�ړ���
///@param y Y�������s�ړ���
Matrix TranslateMat(float x, float y);

///��]�s���Ԃ�
///@param angle ��]�p�x
Matrix RotateMat(float angle);

///�Q�̍s��̏�Z��Ԃ�
///@param lmat ���Ӓl(�s��)
///@param rmat �E�Ӓl(�s��)
///@attention ��Z�̏����ɒ��ӂ��Ă�������
Matrix MultipleMat(const Matrix& lmat, const Matrix& rmat);
Matrix operator*(const Matrix& lmat, const Matrix& rmat);

///�x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
///@param mat �s��
///@param vec �x�N�g��
Vector2 MultipleVec(const Matrix& mat, const Vector2& vec);
Vector2 operator*(const Matrix& mat, const Vector2& vec);




