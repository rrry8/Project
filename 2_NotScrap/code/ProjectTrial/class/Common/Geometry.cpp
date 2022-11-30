#include"Geometry.h"
#include<cmath>


void
Rect::Draw() {
	DxLib::DrawBox(Left() * 2, Top() * 2, Right() * 2, Bottom() * 2, 0xffffffff, false);
}

void
Rect::Draw(Vector2& offset) {
	DxLib::DrawBox((Left() + offset.x) * 2, (Top() + offset.y) * 2, (Right() + offset.x) * 2, (Bottom() + offset.y) * 2, 0xffffffff, false);
}

void
Vector2::operator*=(float scale) {
	Vector2(x *= scale, y *= scale);
}

Vector2
Vector2::operator*(float scale) {
	return Vector2(x*scale, y*scale);
}

Vector2 Vector2::operator*(Vector2& vec)
{
	return Vector2(x * vec.x, y * vec.y);
}


Vector2 Vector2::operator/(float scale)
{
	return { x / scale, y / scale };
}

bool Vector2::operator==(const Vector2& vec) const
{
	return (x == vec.x && y == vec.y);
}

bool Vector2::operator!=(const Vector2& vec) const
{
	return !(x == vec.x && y == vec.y);

}

bool Vector2::operator<=(const Vector2& vec) const
{
	return (x <= vec.x && y <= vec.y);

}

bool Vector2::operator<(const Vector2& vec) const
{
	return (x < vec.x&& y < vec.y);

}

bool Vector2::operator>=(const Vector2& vec) const
{
	return (x >= vec.x && y >= vec.y);

}

bool Vector2::operator>(const Vector2& vec) const
{
	return  (x > vec.x && y > vec.y);

}

Vector2 operator+(const Vector2& va, const Vector2 vb) {
	Vector2 ret;
	ret.x = va.x + vb.x;
	ret.y = va.y + vb.y;
	return ret;
}

Vector2 operator-(const Vector2& va, const Vector2 vb) {
	return Vector2(va.x - vb.x, va.y - vb.y);
}

float
Vector2::Magnitude()const {
	return hypot(x, y);
}


void
Vector2::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}


Vector2
Vector2::Normalized() {
	float mag = Magnitude();
	return Vector2(x / mag, y / mag);
}



///内積を返す
float
Dot(const Vector2& va, const Vector2& vb) {
	return va.x*vb.x + va.y*vb.y;
}

///外積を返す
float
Cross(const Vector2& va, const Vector2& vb) {
	return va.x*vb.y - vb.x*va.y;
}

float Dot(const Vector3& va, const Vector3& vb)
{
	return va.x * vb.x +
		va.y * vb.y +
		va.z * vb.z;
}

Vector3 Cross(const Vector3& va, const Vector3& vb)
{
	Vector3 t;
	t.x = va.y * vb.z - va.z * vb.y;
	t.y = va.z * vb.x - va.x * vb.z;
	t.z = va.x * vb.y - va.y * vb.x;
	return t;
}

Vector3 ConvertVECTORtoVector3(const VECTOR& v3)
{
	Vector3 v;
	v.x = v3.x;
	v.y = v3.y;
	v.z = v3.z;
	return v;
}

double ClampD(double value, double minValue, double maxValue)
{
	return min(max(value, minValue), maxValue);
}

///内積演算子
float
operator*(const Vector2& va, const Vector2& vb) {
	return Dot(va, vb);
}

///外積演算子
float
operator%(const Vector2& va, const Vector2& vb) {
	return Cross(va, vb);
}


void
Vector2::operator+=(const Vector2& v) {
	 Vector2(x += v.x, y += v.y);

}

void Vector2::operator+=(const float v)
{
	x += v;
	y += v;
}

void
Vector2::operator-=(const Vector2& v) {
	 Vector2(x -= v.x, y -= v.y);

}


///２つの行列の乗算を返す
///@param lmat 左辺値(行列)
///@param rmat 右辺値(行列)
///@attention 乗算の順序に注意してください
Matrix
MultipleMat(const Matrix& lmat, const Matrix& rmat) {
	Matrix ret = {};

	for (int k = 0; k <= 2; ++k) {
		for (int j = 0; j <= 2; ++j) {
			for (int i = 0; i <= 2; ++i) {
				ret.m[k][j] += lmat.m[k][i] * rmat.m[i][j];
			}
		}
	}

	return ret;

}

Matrix operator*(const Matrix& lmat, const Matrix& rmat)
{
	return MultipleMat(lmat,rmat);
}

///ベクトルに対して行列乗算を適用し、結果のベクトルを返す
///@param mat 行列
///@param vec ベクトル
Vector2
MultipleVec(const Matrix& mat, const Vector2& vec) {
	Vector2 ret = {};
	ret.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2];
	ret.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2];
	return ret;
}

Vector2 operator*(const Matrix& mat, const Vector2& vec)
{
	return MultipleVec(mat, vec);
}


///単位行列を返す
Matrix IdentityMat() {
	Matrix ret = {};
	ret.m[0][0] = ret.m[1][1] = ret.m[2][2] = 1;
	return ret;
}

///平行移動行列を返す
///@param x X方向平行移動量
///@param y Y方向平行移動量
Matrix TranslateMat(float x, float y) {
	Matrix ret = {};
	ret = IdentityMat();
	ret.m[0][2] = x;
	ret.m[1][2] = y;
	return ret;
}

///回転行列を返す
///@param angle 回転角度
Matrix RotateMat(float angle) {
	Matrix ret = {};
	ret = IdentityMat();
	ret.m[0][0] = cos(angle);
	ret.m[0][1] = -sin(angle);
	ret.m[1][0] = sin(angle);
	ret.m[1][1] = cos(angle);
	return ret;
}

void Vector2Int::operator+=(const Vector2Int& v)
{
	x += v.x;
	y += v.y;
}

Vector2Int Vector2Int::operator+(const Vector2Int& v)
{
	
	return Vector2Int(x + v.x,y + v.y);
}

Vector2Int Vector2Int::operator-(const Vector2Int& v)
{
	return Vector2Int(x - v.x, y - v.y);
}

void Vector2Int::operator-=(const Vector2Int& v)
{
	x -= v.x;
	y -= v.y;
}

void Vector2Int::operator*=(int scale)
{
	x *= scale;
	y *= scale;
}

Vector2Int Vector2Int::operator*(int scale)
{
	return Vector2Int(x * scale, y * scale);
}

bool Vector2Int::operator==(const Vector2Int& vec) const
{
	return (x == vec.x && y == vec.y);
}

bool Vector2Int::operator!=(const Vector2Int& vec) const
{
	return !(x == vec.x && y == vec.y);
}

bool Vector2Int::operator<=(const Vector2Int& vec) const
{
	return (x <= vec.x && y <= vec.y);
}

bool Vector2Int::operator<(const Vector2Int& vec) const
{
	return (x < vec.x&& y < vec.y);
}

bool Vector2Int::operator>=(const Vector2Int& vec) const
{
	return (x >= vec.x && y >= vec.y);

}

bool Vector2Int::operator>(const Vector2Int& vec) const
{
	return  (x > vec.x && y > vec.y);

}

Vector3 Line::vec()
{
	return end - p;
}

float Vector3::Magnitude() const
{
	return std::hypot(x, y, z);//hypot(x, y,z);
}

void Vector3::Normalize()
{
	float mag = Magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}

Vector3 Vector3::Normalized()
{
	float mag = Magnitude();
	return Vector3(x / mag, y / mag, z / mag);
}

Vector3 Vector3::toDiopter()
{
	return Vector3(x * DX_PI_F / 180.0f, y * DX_PI_F / 180.0f, z * DX_PI_F / 180.0f);
}

VECTOR Vector3::toVECTOR()
{
	return VECTOR(x,y,z);
}

btVector3 Vector3::tobtVec()
{
	return btVector3(x, y, z);
}

void Vector3::operator+=(const Vector3& v)
{
	Vector3(x += v.x, y += v.y, z += v.z);
}

void Vector3::operator-=(const Vector3& v)
{
	Vector3(x -= v.x, y -= v.y, z -= v.z);
}

void Vector3::operator*=(float scale)
{
	Vector3(x *= scale, y *= scale, z *= scale);
}

Vector3 Vector3::operator*(float scale)
{
	return Vector3(x * scale,y * scale,z * scale);
}

Vector3 Vector3::operator*(Vector3& vec)
{
	return Vector3(x * vec.x, y * vec.y, z * vec.z);
}

Vector3 Vector3::operator=(const Vector3& v)
{
	return Vector3(x = v.x, y = v.y, z = v.z);
}

Vector3 Vector3::operator/(float scale)
{
	return Vector3(x / scale, y / scale, z / scale);
}

Vector3 operator+(const Vector3& va, const Vector3 vb)
{
	return Vector3(va.x + vb.x, va.y + vb.y,va.z + vb.z);
}

Vector3 operator-(const Vector3& va, const Vector3 vb)
{
	return Vector3(va.x - vb.x, va.y - vb.y, va.z - vb.z);
}
bool Vector3::operator==(const Vector3& vec) const
{
	return (x == vec.x && y == vec.y && z == vec.z);
}

bool Vector3::operator!=(const Vector3& vec) const
{
	return !(x == vec.x && y == vec.y && z == vec.z);
}

bool Vector3::operator<=(const Vector3& vec) const
{
	return (x <= vec.x && y <= vec.y && z <= vec.z);
}

bool Vector3::operator<(const Vector3& vec) const
{
	return (x < vec.x && y < vec.y && z < vec.z);
}

bool Vector3::operator>=(const Vector3& vec) const
{
	return (x >= vec.x && y >= vec.y && z >= vec.z);
}

bool Vector3::operator>(const Vector3& vec) const
{
	return (x > vec.x && y > vec.y && z > vec.z);
}
