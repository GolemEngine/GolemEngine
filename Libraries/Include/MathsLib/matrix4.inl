#include "matrix4.h"

#include <iomanip>

#include "vector3.h"

Matrix4::Matrix4() 
{ 
	data[0][0] = 0.0f;
	data[0][1] = 0.0f;
	data[0][2] = 0.0f;
	data[0][3] = 0.0f;
	data[1][0] = 0.0f;
	data[1][1] = 0.0f;
	data[1][2] = 0.0f;
	data[1][3] = 0.0f;
	data[2][0] = 0.0f;
	data[2][1] = 0.0f;
	data[2][2] = 0.0f;
	data[2][3] = 0.0f;
	data[3][0] = 0.0f;
	data[3][1] = 0.0f;
	data[3][2] = 0.0f;
	data[3][3] = 0.0f;
}

Matrix4::~Matrix4() { }

Matrix4::Matrix4(
	float _a, float _b, float _c, float _d,
	float _e, float _f, float _g, float _h,
	float _i, float _j, float _k, float _l,
	float _m, float _n, float _o, float _p
)
{
	data[0][0] = _a;
	data[0][1] = _b;
	data[0][2] = _c;
	data[0][3] = _d;
	data[1][0] = _e;
	data[1][1] = _f;
	data[1][2] = _g;
	data[1][3] = _h;
	data[2][0] = _i;
	data[2][1] = _j;
	data[2][2] = _k;
	data[2][3] = _l;
	data[3][0] = _m;
	data[3][1] = _n;
	data[3][2] = _o;
	data[3][3] = _p;
}

Matrix4::Matrix4(float _number)
{
	data[0][0] = data[0][1] = data[0][2] = data[0][3] = data[1][0] = data[1][1] = data[1][2] = data[1][3] = data[2][0] = data[2][1] = data[2][2] = data[2][3] = data[3][0] = data[3][1] = data[3][2] = data[3][3] = _number;
}

Matrix4 Matrix4::Identity()
{
	return Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

Matrix4 Matrix4::Transpose() const
{
	Matrix4 Transpose;

	for (int i = 0; i < 4; i++) {
		// assign Vector4 as new column of matrix
		float temp[4];
		for (int j = 0; j < 4; j++) {
			temp[j] = data[i][j];
		}
		// set the temp Vector4 to the correct lign of the tranposed matrix
		for (int k = 0; k < 4; k++) {
			Transpose.data[k][i] = temp[k];
		}
	}
	return Transpose;
}

float Matrix4::GetDeterminant() const
{
	return 
		data[0][3] * data[1][2] * data[2][1] * data[3][0] - data[0][2] * data[1][3] * data[2][1] * data[3][0] -
		data[0][3] * data[1][1] * data[2][2] * data[3][0] + data[0][1] * data[1][3] * data[2][2] * data[3][0] +
		data[0][2] * data[1][1] * data[2][3] * data[3][0] - data[0][1] * data[1][2] * data[2][3] * data[3][0] -
		data[0][3] * data[1][2] * data[2][0] * data[3][1] + data[0][2] * data[1][3] * data[2][0] * data[3][1] +
		data[0][3] * data[1][0] * data[2][2] * data[3][1] - data[0][0] * data[1][3] * data[2][2] * data[3][1] -
		data[0][2] * data[1][0] * data[2][3] * data[3][1] + data[0][0] * data[1][2] * data[2][3] * data[3][1] +
		data[0][3] * data[1][1] * data[2][0] * data[3][2] - data[0][1] * data[1][3] * data[2][0] * data[3][2] -
		data[0][3] * data[1][0] * data[2][1] * data[3][2] + data[0][0] * data[1][3] * data[2][1] * data[3][2] +
		data[0][1] * data[1][0] * data[2][3] * data[3][2] - data[0][0] * data[1][1] * data[2][3] * data[3][2] -
		data[0][2] * data[1][1] * data[2][0] * data[3][3] + data[0][1] * data[1][2] * data[2][0] * data[3][3] +
		data[0][2] * data[1][0] * data[2][1] * data[3][3] - data[0][0] * data[1][2] * data[2][1] * data[3][3] -
		data[0][1] * data[1][0] * data[2][2] * data[3][3] + data[0][0] * data[1][1] * data[2][2] * data[3][3];
}


Matrix4 Matrix4::Inverse() const
{
	Matrix4 inv;
	float det;
	inv.data[0][0] = data[1][1] * data[2][2] * data[3][3] - data[1][1] * data[2][3] * data[3][2] - data[2][1] * data[1][2] * data[3][3] + data[2][1] * data[1][3] * data[3][2] + data[3][1] * data[1][2] * data[2][3] - data[3][1] * data[1][3] * data[2][2];
	inv.data[1][0] = -data[1][0] * data[2][2] * data[3][3] + data[1][0] * data[2][3] * data[3][2] + data[2][0] * data[1][2] * data[3][3] - data[2][0] * data[1][3] * data[3][2] - data[3][0] * data[1][2] * data[2][3] + data[3][0] * data[1][3] * data[2][2];
	inv.data[2][0] = data[1][0] * data[2][1] * data[3][3] - data[1][0] * data[2][3] * data[3][1] - data[2][0] * data[1][1] * data[3][3] + data[2][0] * data[1][3] * data[3][1] + data[3][0] * data[1][1] * data[2][3] - data[3][0] * data[1][3] * data[2][1];
	inv.data[3][0] = -data[1][0] * data[2][1] * data[3][2] + data[1][0] * data[2][2] * data[3][1] + data[2][0] * data[1][1] * data[3][2] - data[2][0] * data[1][2] * data[3][1] - data[3][0] * data[1][1] * data[2][2] + data[3][0] * data[1][2] * data[2][1];
	inv.data[0][1] = -data[0][1] * data[2][2] * data[3][3] + data[0][1] * data[2][3] * data[3][2] + data[2][1] * data[0][2] * data[3][3] - data[2][1] * data[0][3] * data[3][2] - data[3][1] * data[0][2] * data[2][3] + data[3][1] * data[0][3] * data[2][2];
	inv.data[1][1] = data[0][0] * data[2][2] * data[3][3] - data[0][0] * data[2][3] * data[3][2] - data[2][0] * data[0][2] * data[3][3] + data[2][0] * data[0][3] * data[3][2] + data[3][0] * data[0][2] * data[2][3] - data[3][0] * data[0][3] * data[2][2];
	inv.data[2][1] = -data[0][0] * data[2][1] * data[3][3] + data[0][0] * data[2][3] * data[3][1] + data[2][0] * data[0][1] * data[3][3] - data[2][0] * data[0][3] * data[3][1] - data[3][0] * data[0][1] * data[2][3] + data[3][0] * data[0][3] * data[2][1];
	inv.data[3][1] = data[0][0] * data[2][1] * data[3][2] - data[0][0] * data[2][2] * data[3][1] - data[2][0] * data[0][1] * data[3][2] + data[2][0] * data[0][2] * data[3][1] + data[3][0] * data[0][1] * data[2][2] - data[3][0] * data[0][2] * data[2][1];
	inv.data[0][2] = data[0][1] * data[1][2] * data[3][3] - data[0][1] * data[1][3] * data[3][2] - data[1][1] * data[0][2] * data[3][3] + data[1][1] * data[0][3] * data[3][2] + data[3][1] * data[0][2] * data[1][3] - data[3][1] * data[0][3] * data[1][2];
	inv.data[1][2] = -data[0][0] * data[1][2] * data[3][3] + data[0][0] * data[1][3] * data[3][2] + data[1][0] * data[0][2] * data[3][3] - data[1][0] * data[0][3] * data[3][2] - data[3][0] * data[0][2] * data[1][3] + data[3][0] * data[0][3] * data[1][2];
	inv.data[2][2] = data[0][0] * data[1][1] * data[3][3] - data[0][0] * data[1][3] * data[3][1] - data[1][0] * data[0][1] * data[3][3] + data[1][0] * data[0][3] * data[3][1] + data[3][0] * data[0][1] * data[1][3] - data[3][0] * data[0][3] * data[1][1];
	inv.data[3][2] = -data[0][0] * data[1][1] * data[3][2] + data[0][0] * data[1][2] * data[3][1] + data[1][0] * data[0][1] * data[3][2] - data[1][0] * data[0][2] * data[3][1] - data[3][0] * data[0][1] * data[1][2] + data[3][0] * data[0][2] * data[1][1];
	inv.data[0][3] = -data[0][1] * data[1][2] * data[2][3] + data[0][1] * data[1][3] * data[2][2] + data[1][1] * data[0][2] * data[2][3] - data[1][1] * data[0][3] * data[2][2] - data[2][1] * data[0][2] * data[1][3] + data[2][1] * data[0][3] * data[1][2];
	inv.data[1][3] = data[0][0] * data[1][2] * data[2][3] - data[0][0] * data[1][3] * data[2][2] - data[1][0] * data[0][2] * data[2][3] + data[1][0] * data[0][3] * data[2][2] + data[2][0] * data[0][2] * data[1][3] - data[2][0] * data[0][3] * data[1][2];
	inv.data[2][3] = -data[0][0] * data[1][1] * data[2][3] + data[0][0] * data[1][3] * data[2][1] + data[1][0] * data[0][1] * data[2][3] - data[1][0] * data[0][3] * data[2][1] - data[2][0] * data[0][1] * data[1][3] + data[2][0] * data[0][3] * data[1][1];
	inv.data[3][3] = data[0][0] * data[1][1] * data[2][2] - data[0][0] * data[1][2] * data[2][1] - data[1][0] * data[0][1] * data[2][2] + data[1][0] * data[0][2] * data[2][1] + data[2][0] * data[0][1] * data[1][2] - data[2][0] * data[0][2] * data[1][1];

	det = data[0][0] * inv.data[0][0] + data[0][1] * inv.data[1][0] + data[0][2] * inv.data[2][0] + data[0][3] * inv.data[3][0];
	if (det == 0)
	{
		std::cout << "The Matrix can't be inverted, determinant is 0" << std::endl;
		return Matrix4(0);
	}
	det = 1.0f / det;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			inv.data[i][j] = inv.data[i][j] * det;

	return inv;
}

Matrix4 Matrix4::Translate(Vector3 _t) const
{
	Matrix4 translate(
		1, 0, 0, _t.x,
		0, 1, 0, _t.y,
		0, 0, 1, _t.z,
		0, 0, 0, 1
	);
	return *this * translate;
}

Matrix4 Matrix4::Rotate(Vector3 _XYZrad) const
{
	Matrix4 rotateX(
		1, 0, 0, 0,
		0, cos(_XYZrad.x), -sin(_XYZrad.x), 0,
		0, sin(_XYZrad.x), cos(_XYZrad.x), 0,
		0, 0, 0, 1
	);
	Matrix4 rotateY(
		cos(_XYZrad.y), 0, sin(_XYZrad.y), 0,
		0, 1, 0, 0,
		-sin(_XYZrad.y), 0, cos(_XYZrad.y), 0,
		0, 0, 0, 1
	);
	Matrix4 rotateZ(
		cos(_XYZrad.z), -sin(_XYZrad.z), 0, 0,
		sin(_XYZrad.z), cos(_XYZrad.z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	Matrix4 rotate = rotateZ * rotateY * rotateX;
	return (*this) * rotate;
}

Matrix4 Matrix4::Rotate(Quaternion _q) const
{
	const float_t xx = _q.x * _q.x;
	const float_t yy = _q.y * _q.y;
	const float_t zz = _q.z * _q.z;

	const float_t xy = _q.x * _q.y;
	const float_t wz = _q.z * _q.w;
	const float_t xz = _q.z * _q.x;
	const float_t wy = _q.y * _q.w;
	const float_t yz = _q.y * _q.z;
	const float_t wx = _q.x * _q.w;

	return (*this) * Matrix4(
		1.f - 2.f * (yy + zz), 2.f * (xy - wz), 2.f * (xz + wy), 0.f,
		2.f * (xy + wz), 1.f - 2.f * (zz + xx), 2.f * (yz - wx), 0.f,
		2.f * (xz - wy), 2.f * (yz + wx), 1.f - 2.f * (yy + xx), 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}


Matrix4 Matrix4::Scale(Vector3 _scale) const
{
	Matrix4 scale(
		_scale.x, 0, 0, 0,
		0, _scale.y, 0, 0,
		0, 0, _scale.z, 0,
		0, 0, 0, 1
	);
	return (*this) * scale;
}

Matrix4 Matrix4::ExtractRotationAndScale(const Matrix4& _viewMatrix) const
{
	// Extract rotation and scale by taking the upper-left 3x3 submatrix
	return Matrix4(
		_viewMatrix.data[0][0], _viewMatrix.data[0][1], _viewMatrix.data[0][2], 0.0f,
		_viewMatrix.data[1][0], _viewMatrix.data[1][1], _viewMatrix.data[1][2], 0.0f,
		_viewMatrix.data[2][0], _viewMatrix.data[2][1], _viewMatrix.data[2][2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Vector3 Matrix4::TrsToPosition()
{
	return Vector3(data[0][3], data[1][3], data[2][3]);
}

Quaternion Matrix4::TrsToRotation()
{
	float trace = data[0][0] + data[1][1] + data[2][2];
	Quaternion quat;

	if (trace > 0)
	{
		float S = sqrt(trace + 1.0f) * 2.0f;
		quat.w = 0.25f * S;
		quat.x = (data[2][1] - data[1][2]) / S;
		quat.y = (data[0][2] - data[2][0]) / S;
		quat.z = (data[1][0] - data[0][1]) / S;
	}
	else if ((data[0][0] > data[1][1]) && (data[0][0] > data[2][2])) 
	{
		float S = sqrt(1.0f + data[0][0] - data[1][1] - data[2][2]) * 2.0f;
		quat.w = (data[2][1] - data[1][2]) / S;
		quat.x = 0.25f * S;
		quat.y = (data[0][1] + data[1][0]) / S;
		quat.z = (data[0][2] + data[2][0]) / S;
	}
	else if (data[1][1] > data[2][2]) 
	{
		float S = sqrt(1.0f + data[1][1] - data[0][0] - data[2][2]) * 2.0f;
		quat.w = (data[0][2] - data[2][0]) / S;
		quat.x = (data[0][1] + data[1][0]) / S;
		quat.y = 0.25f * S;
		quat.z = (data[1][2] + data[2][1]) / S;
	}
	else 
	{
		float S = sqrt(1.0f + data[2][2] - data[0][0] - data[1][1]) * 2.0f;
		quat.w = (data[1][0] - data[0][1]) / S;
		quat.x = (data[0][2] + data[2][0]) / S;
		quat.y = (data[1][2] + data[2][1]) / S;
		quat.z = 0.25f * S;
	}

	return quat;
}

Vector3 Matrix4::TrsToScaling()
{
	float scaleX = std::sqrt(data[0][0] * data[0][0] + data[0][1] * data[0][1] + data[0][2] * data[0][2]);
	float scaleY = std::sqrt(data[1][0] * data[1][0] + data[1][1] * data[1][1] + data[1][2] * data[1][2]);
	float scaleZ = std::sqrt(data[2][0] * data[2][0] + data[2][1] * data[2][1] + data[2][2] * data[2][2]);

	return Vector3(scaleX, scaleY, scaleZ);
}

Matrix4 Matrix4::TRS(Vector3 _translate, Vector3 _XYZrad, Vector3 _scale)
{
	Matrix4 result = Matrix4::Identity();

	result.data[0][3] = _translate.x;
	result.data[1][3] = _translate.y;
	result.data[2][3] = _translate.z;

	result = result.Rotate(_XYZrad);

	result = result.Scale(_scale);

	return result;
}

Matrix4 Matrix4::TRS(Vector3 _translate, Quaternion _rotate, Vector3 _scale)
{
	Matrix4 result = Matrix4::Identity();

	result.data[0][3] = _translate.x;
	result.data[1][3] = _translate.y;
	result.data[2][3] = _translate.z;

	result = result.Rotate(_rotate);

	result = result.Scale(_scale);

	return result;
}


Matrix4 Matrix4::Projection(float _fov, float _aspectRatio, float _zNear, float _zFar)
{
	float f = std::tan(_fov / 2.0f);
	float matData00 = 1 / (f * _aspectRatio);
	float matData22 = (-_zNear - _zFar) / (_zNear - _zFar);
	float matData23 = (2 * _zFar * _zNear) / (_zNear - _zFar);

	Matrix4 temp(
		matData00, 0, 0, 0,
		0, 1 / f, 0, 0,
		0, 0, -matData22, matData23,
		0, 0, -1, 0
	);
	return temp;
}

Matrix4 Matrix4::LookAt(Vector3 _cameraPos, Vector3 _targetPos, Vector3 _up)
{
	Vector3 Up = _up.Normalize();

	Vector3 Z = (_cameraPos - _targetPos).Normalize();
	Vector3 X = Vector3::Cross(Up, Z).Normalize();
	Vector3 Y = Vector3::Cross(Z, X);

	Matrix4 temp(
		X.x, X.y, X.z, 0,
		Y.x, Y.y, Y.z, 0,
		Z.x, Z.y, Z.z, 0,
		0, 0, 0, 1
	);

	temp.data[0][3] = Vector3::Dot(X, _cameraPos) * -1;
	temp.data[1][3] = Vector3::Dot(Y, _cameraPos) * -1;
	temp.data[2][3] = Vector3::Dot(Z, _cameraPos) * -1;

	return temp;
}

#pragma region Operators

Matrix4 operator+(Matrix4 _A, float _n)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			_A.data[i][j] += _n;
	return _A;
}

Matrix4 operator-(Matrix4 _A, float _n)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			_A.data[i][j] -= _n;
	return _A;
}

Matrix4 operator*(Matrix4 _A, float _n)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			_A.data[i][j] *= _n;
	return _A;
}

Matrix4 operator/(Matrix4 _A, float _s)
{
	if (_s == 0.0f)
	{
		std::cout << "ERROR: Divide by 0 is illegal." << std::endl;
		return Matrix4(0);
	}
	return _A * (1.0f / _s);
}

Matrix4 operator+(Matrix4 _A, Matrix4 _B)
{
	Matrix4 temp(0);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp.data[i][j] = _A.data[i][j] + _B.data[i][j];
	return temp;
}

Matrix4 operator-(Matrix4 _A, Matrix4 _B)
{
	Matrix4 temp(0);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp.data[i][j] = _A.data[i][j] - _B.data[i][j];
	return temp;
}

Matrix4 operator*(Matrix4 _A, Matrix4 _B)
{
	Matrix4 temp(0);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				temp.data[i][j] += _A.data[i][k] * _B.data[k][j];
	return temp;
}

bool operator==(Matrix4 _A, Matrix4 _B)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (fabs(_A.data[i][j] - _B.data[i][j]) > 10e-5f)
				return false;
	return true;
}

std::ostream& operator<<(std::ostream& _os, const Matrix4& _matrix4)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			_os << _matrix4.data[i][j] << " ";
		_os << "\n";
	}
	return _os;
}

#pragma endregion Operators