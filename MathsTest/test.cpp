#include "pch.h"
#include "mathslib.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4.h"
#include "quaternion.h"
#include "utils.h"


#define EPSILON 0.01f

#pragma region Utils

TEST(Utils, DegToRad)
{
	EXPECT_NEAR(DegToRad(157), 2.74017f, EPSILON);
}

TEST(Utils, RadToDeg)
{
	EXPECT_NEAR(RadToDeg(0.74f), 42.39888f, EPSILON);
}

TEST(Utils, Lerp)
{
	EXPECT_EQ(Lerp(2, 7, 0.3f), 3.5f);
}

TEST(Utils, InvLerp)
{
	EXPECT_EQ(InvLerp(2, 7, 3.5f), 0.3f);
}

TEST(Utils, Remap)
{
	EXPECT_EQ(Remap(0, 10, 2, 5, 2.75f), 2.825f);
}

TEST(Utils, Q_rsqrt)
{
	EXPECT_NEAR(Q_rsqrt(2), 0.707, EPSILON);
	EXPECT_NEAR(Q_rsqrt(4), 0.5, EPSILON);
}

TEST(Utils, Q_rsqrt0)
{
	EXPECT_NEAR(Q_rsqrt0(2, 0), 0.707, EPSILON);
	EXPECT_NEAR(Q_rsqrt0(4, 0), 0.5, EPSILON);
	EXPECT_NEAR(Q_rsqrt0(2, 1), 0.707, 0.001f);
	EXPECT_NEAR(Q_rsqrt0(4, 1), 0.499, 0.001f);
}

#pragma endregion Utils

#pragma region Vector2

TEST(Vector2, Normalize)
{
	Vector2 vec1(1, 1);
	Vector2 zero(0, 0);
	EXPECT_NEAR(vec1.Normalize().x, 0.707107f, EPSILON);
	EXPECT_NEAR(vec1.Normalize().y, 0.707107f, EPSILON);
	EXPECT_EQ(zero.Normalize(), zero);
}

TEST(Vector2, Length)
{
	Vector2 vec1(1, 1);
	EXPECT_NEAR(vec1.Length(), 1.41421, EPSILON);
}

TEST(Vector2, LengthSquare)
{
	Vector2 vec1(1, 1);
	EXPECT_NEAR(vec1.LengthSq(), 2.0f, EPSILON);
}

TEST(Vector2, DotProduct)
{
	Vector2 vec1(1, 1);
	Vector2 vec2(2, 2);
	EXPECT_EQ(vec1.Dot(vec1, vec2), 4);
}

TEST(Vector2, Operators_Add_Float)
{
	Vector2 a(1.0f, 2.0f);
	EXPECT_EQ((a + 9.3f), Vector2(10.3f, 11.3f));
}

TEST(Vector2, Operators_Substract_Float)
{
	Vector2 a(1.0f, 2.0f);
	EXPECT_EQ((a - 9.3f), Vector2(-8.3f, -7.3f));
}

TEST(Vector2, Operators_Multiply_Float)
{
	Vector2 a(1.0f, 2.0f);
	EXPECT_NEAR((a * 9.3f).x, 9.3f, EPSILON);
	EXPECT_NEAR((a * 9.3f).y, 18.6f, EPSILON);
}

TEST(Vector2, Operators_Divide_Float)
{
	Vector2 a(1.0f, 2.0f);
	EXPECT_NEAR((a / 9.3f).x, 0.107f, EPSILON);
	EXPECT_NEAR((a / 9.3f).y, 0.215f, EPSILON);
	EXPECT_EQ((a / 0.0f), Vector2(0.0f, 0.0f));
}

TEST(Vector2, Operators_Add_Vector2)
{
	Vector2 a(1.0f, 2.0f);
	Vector2 b(-1.3f, 2.0f);
	EXPECT_NEAR((a + b).x, -0.3f, EPSILON);
	EXPECT_NEAR((a + b).y, 4.0f, EPSILON);
}

TEST(Vector2, Operators_Substract_Vector2)
{
	Vector2 a(1.0f, 2.0f);
	Vector2 b(-1.3f, 2.0f);
	EXPECT_EQ((a - b), Vector2(2.3f, 0.0f));
}

TEST(Vector2, Operator_Equal_Vector2)
{
	Vector2 a(1.f, 2.f);
	Vector2 b(1.f, 2.f);
	EXPECT_EQ(a, b);
}

TEST(Vector2, Operator_NotEqual_Vector2)
{
	Vector2 a(1.f, 2.f);
	Vector2 b(-0.f, 1.f);
	EXPECT_NE(a.x, b.x);
	EXPECT_NE(a.y, b.y);
}

#pragma endregion Vector2

#pragma region Vector3

TEST(Vector3, Constructor) 
{
	Vector3 v(1.f, 2.f, 3.f);
	EXPECT_NEAR(v.x, 1.f, EPSILON);
	EXPECT_NEAR(v.y, 2.f, EPSILON);
	EXPECT_NEAR(v.z, 3.f, EPSILON);
}

TEST(Vector3, Length) 
{
	Vector3 v(1.f, 2.f, 3.f);
	EXPECT_NEAR(v.Length(), 3.74166f, EPSILON);
}

TEST(Vector3, LengthSq) 
{
	Vector3 v(1.f, 2.f, 3.f);
	EXPECT_NEAR(v.LengthSq(), 14.0f, EPSILON);
}

TEST(Vector3, Normalize)
{
	Vector3 a(1.0f, 2.0f, 3.0f);
	Vector3 zero(0.0f, 0.0f, 0.0f);
	EXPECT_NEAR(a.Normalize().x, 0.27f, EPSILON);
	EXPECT_NEAR(a.Normalize().y, 0.53f, EPSILON);
	EXPECT_NEAR(a.Normalize().z, 0.80f, EPSILON);
	EXPECT_EQ(zero.Normalize(), zero, EPSILON);
}

TEST(Vector3, Dot) 
{
	Vector3 a(1.0f, 2.0f, 3.0f);
	Vector3 b(3.0f, 2.0f, 1.0f);
	EXPECT_EQ(Vector3::Dot(a, b), 10.0f);
}

TEST(Vector3, Cross)
{
	Vector3 a(1.0f, 2.0f, 3.0f);
	Vector3 b(3.0f, 2.0f, 1.0f);
	Vector3 c(-4.0f, 8.0f, -4.0f);
	EXPECT_NEAR(Vector3::Cross(a, b).x, c.x, EPSILON);
	EXPECT_NEAR(Vector3::Cross(a, b).y, c.y, EPSILON);
	EXPECT_NEAR(Vector3::Cross(a, b).z, c.z, EPSILON);
}

TEST(Vector3, Operators_Add_Float)
{
	Vector3 a(1.0f, 2.0f, 3.0f);
	EXPECT_EQ((a + 9.3f), Vector3(10.3f, 11.3f, 12.3f));

}

TEST(Vector3, Operators_Substract_Float)
{
	Vector3 a(1.0f, 2.0f, 3.0f);
	EXPECT_EQ((a - 9.3f), Vector3(-8.3f, -7.3f, -6.3f));
}

TEST(Vector3, Operators_Multiply_Float)
{
	Vector3 a(1.0f, 2.0f, 3.0f);
	EXPECT_NEAR((a * 9.3f).x, 9.3f, EPSILON);
	EXPECT_NEAR((a * 9.3f).y, 18.6f, EPSILON);
	EXPECT_NEAR((a * 9.3f).z, 27.9f, EPSILON);
}

TEST(Vector3, Operators_Divide_Float)
{
	Vector3 a(1.0f, 2.0f, 3.0f);
	EXPECT_NEAR((a / 9.3f).x, 0.107f, EPSILON);
	EXPECT_NEAR((a / 9.3f).y, 0.215f, EPSILON);
	EXPECT_NEAR((a / 9.3f).z, 0.322f, EPSILON);
	EXPECT_EQ((a / 0.0f), Vector3(0.0f, 0.0f, 0.0f));
}

TEST(Vector3, Operators_Add_Vector3)
{
	Vector3 a(1.0f, 2.0f, 3.0f);
	Vector3 b(-1.3f, 2.0f, 57.4f);
	EXPECT_NEAR((a + b).x, -0.3f, EPSILON);
	EXPECT_NEAR((a + b).y, 4.0f, EPSILON);
	EXPECT_NEAR((a + b).z, 60.4f, EPSILON);
}

TEST(Vector3, Operators_Substract_Vector3)
{
	Vector3 a(1.0f, 2.0f, 3.0f);
	Vector3 b(-1.3f, 2.0f, 57.4f);
	EXPECT_EQ((a - b), Vector3(2.3f, 0.0f, -54.4f));
}

TEST(Vector3, Operator_Equal_Vector3)
{
	Vector3 a(1.f, 2.f, 3.f);
	Vector3 b(1.f, 2.f, 3.f);
	EXPECT_EQ(a, b);
}

TEST(Vector3, Operator_NotEqual_Vector3)
{
	Vector3 a(1.f, 2.f, 3.f);
	Vector3 b(-0.f, 1.f, -4.9f);
	EXPECT_NE(a.x, b.x);
	EXPECT_NE(a.y, b.y);
	EXPECT_NE(a.z, b.z);
}

#pragma endregion Vector3

#pragma region Vector4

TEST(Vector4, Constructor)
{
	Vector4 v(1.f, 2.f, 3.f, 4.f);
	EXPECT_NEAR(v.x, 1.f, EPSILON);
	EXPECT_NEAR(v.y, 2.f, EPSILON);
	EXPECT_NEAR(v.z, 3.f, EPSILON);
	EXPECT_NEAR(v.w, 4.f, EPSILON);
}

TEST(Vector4, Length)
{
	Vector4 v(1.f, 2.f, 3.f, 4.f);
	EXPECT_NEAR(v.Length(), 5.47f, EPSILON);
}

TEST(Vector4, LenghtSq)
{
	Vector4 v(1.f, 2.f, 3.f, 4.f);
	EXPECT_NEAR(v.LengthSq(), 30.f, EPSILON);
}

TEST(Vector4, Normalize)
{
	Vector4 v(1.f, 2.f, 3.f, 4.f);
	Vector4 zero(0.0f, 0.0f, 0.0f, 0.0f);
	EXPECT_NEAR(v.Normalize().x, 0.18f, EPSILON);
	EXPECT_NEAR(v.Normalize().y, 0.36f, EPSILON);
	EXPECT_NEAR(v.Normalize().z, 0.54f, EPSILON);
	EXPECT_NEAR(v.Normalize().w, 0.73f, EPSILON);
	EXPECT_EQ(zero.Normalize(), zero);
}

TEST(Vector4, Operator_Add_Float)
{
	Vector4 a(1.f, 2.f, 3.f, 4.f);
	Vector4 b = a + 3.78f;
	EXPECT_NEAR(b.x, 4.78f, EPSILON);
	EXPECT_NEAR(b.y, 5.78f, EPSILON);
	EXPECT_NEAR(b.z, 6.78f, EPSILON);
	EXPECT_NEAR(b.w, 7.78f, EPSILON);
}

TEST(Vector4, Operator_Substract_Float)
{
	Vector4 a(1.f, 2.f, 3.f, 4.f);
	Vector4 b = a - 3.78f;
	EXPECT_NEAR(b.x, -2.78f, EPSILON);
	EXPECT_NEAR(b.y, -1.78f, EPSILON);
	EXPECT_NEAR(b.z, -0.78f, EPSILON);
	EXPECT_NEAR(b.w, 0.22f, EPSILON);
}

TEST(Vector4, Operator_Multiply_Float)
{
	Vector4 a(1.f, 2.f, 3.f, 4.f);
	Vector4 b = a * 3.78f;
	EXPECT_NEAR(b.x, 3.78f, EPSILON);
	EXPECT_NEAR(b.y, 7.56f, EPSILON);
	EXPECT_NEAR(b.z, 11.34f, EPSILON);
	EXPECT_NEAR(b.w, 15.12f, EPSILON);
}

TEST(Vector4, Operator_Divide_Float)
{
	Vector4 a(1.f, 2.f, 3.f, 4.f);
	Vector4 b = a / 3.78f;
	EXPECT_NEAR(b.x, 0.26f, EPSILON);
	EXPECT_NEAR(b.y, 0.52f, EPSILON);
	EXPECT_NEAR(b.z, 0.79f, EPSILON);
	EXPECT_NEAR(b.w, 1.05f, EPSILON);
	EXPECT_EQ(a / 0.0f, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
}

TEST(Vector4, Operator_Add_Vector4)
{
	Vector4 a(1.f, 2.f, 3.f, 4.f);
	Vector4 b(3.5f, 2.f, -1.5f, 0.78f);
	EXPECT_NEAR((a + b).x, 4.5f, EPSILON);
	EXPECT_NEAR((a + b).y, 4.f, EPSILON);
	EXPECT_NEAR((a + b).z, 1.5f, EPSILON);
	EXPECT_NEAR((a + b).w, 4.78f, EPSILON);
}

TEST(Vector4, Operator_Substract_Vector4)
{
	Vector4 a(1.f, 2.f, 3.f, 4.f);
	Vector4 b(3.5f, 2.f, -1.5f, 0.78f);
	EXPECT_NEAR((a - b).x, -2.5f, EPSILON);
	EXPECT_NEAR((a - b).y, 0.f, EPSILON);
	EXPECT_NEAR((a - b).z, 4.5f, EPSILON);
	EXPECT_NEAR((a - b).w, 3.22f, EPSILON);
}

TEST(Vector4, Operator_Equal_Vector4)
{
	Vector4 a(1.f, -0.f, 3.f, -4.9f);
	Vector4 b(1.f, -0.f, 3.f, -4.9f);
	EXPECT_EQ(a.x, b.x);
	EXPECT_EQ(a.y, b.y);
	EXPECT_EQ(a.z, b.z);
	EXPECT_EQ(a.z, b.z);
}

TEST(Vector4, Operator_NotEqual_Vector4)
{
	Vector4 a(1.f, 2.f, 3.f, -4.9f);
	Vector4 b(-0.f, 1.f, -4.9f, 2.5f);
	EXPECT_NE(a.x, b.x);
	EXPECT_NE(a.y, b.y);
	EXPECT_NE(a.z, b.z);
	EXPECT_NE(a.w, b.w);
}

TEST(Vector4, Operator_Multiply_Matrix_Vector)
{
	Vector4 a(1.f, 2.f, 3.f, -4.9f);
	Matrix4 b(2.9f, 1, -2, -3.5f, 0, 0.5f, -0.78f, 2.3f, 4, 6, 3, 75, 1, -67, 4, 32);
	EXPECT_NEAR((b * a).x, 16.05f, EPSILON);
	EXPECT_NEAR((b * a).y, -12.61f, EPSILON);
	EXPECT_NEAR((b * a).z, -342.5f, EPSILON);
	EXPECT_NEAR((b * a).w, -277.8f, EPSILON);
}

#pragma endregion Vector4

#pragma region Matrix4

TEST(Matrix4, Constructor)
{
	Matrix4 test = Matrix4(1);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			EXPECT_EQ(test.data[i][j], 1);
	test = Matrix4(0.f, 1.f, 2.f, 3.f, 1.f, 2.f, 3.f, 4.f, 2.f, 3.f, 4.f, 5.f, 3.f, 4.f, 5.f, 6.f);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			EXPECT_EQ(test.data[i][j], i + j);
}

TEST(Matrix4, Identity)
{
	EXPECT_EQ(Matrix4::Identity(), Matrix4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f));
}

TEST(Matrix4, Transpose)
{
	Matrix4 test = Matrix4(-2.f, 6.f, 7.f, 3.f, 2.f, 0.f, 4.f, 0.f, 9.f, 2.f, 43.f, 2.f, 7.f, 1.f, 3.f, 98.2f);
	test = test.Transpose();
	EXPECT_EQ(test, Matrix4(-2.f, 2.f, 9.f, 7.f, 6.f, 0.f, 2.f, 1.f, 7.f, 4.f, 43.f, 3.f, 3.f, 0.f, 2.f, 98.2f));
}

TEST(Matrix4, Translate)
{
	Matrix4 test = Matrix4(-2.f, 6.f, 7.f, 3.f, 2.f, 0.f, 4.f, 0.f, 9.f, 2.f, 43.f, 2.f, 7.f, 1.f, 3.f, 98.2f);
	test = test.Translate(Vector3(10.f, -5.f, 3.1f));
	EXPECT_EQ(test, Matrix4(-2.f, 6.f, 7.f, -25.3000011f, 2.f, 0.f, 4.f, 32.4000015f, 9.f, 2.f, 43.f, 215.300003f, 7.f, 1.f, 3.f, 172.5f)); //precision float 
}

TEST(Matrix4, Rotate)
{
	Matrix4 test = Matrix4(-2.f, 6.f, 7.f, 3.f, 2.f, 0.f, 4.f, 0.f, 9.f, 2.f, 43.f, 2.f, 7.f, 1.f, 3.f, 98.2f);
	test = test.Rotate(Vector3(3.14f, 3.14f/2.f, -3.14f / 2.f));
	EXPECT_EQ(test, Matrix4(-7.00478f, 1.98567f, 5.99919f, 3, -4, -1.99999f, -0.00796273f, 0, -43.0016f, -9.0047f, 1.94425f, 2, -3.00079f, -7.00237f, 0.980886f, 98.2f)); //precision float 
}

TEST(Matrix4, Scale)
{
	Matrix4 test = Matrix4(-2, 5, 3, 2, 10, 5.5f, 6, 1, 1, 2, 1, 3, 2, 46, 20, 5);
	test = test.Scale(Vector3(2, -4, 1.5f));
	EXPECT_EQ(test, Matrix4(-4, -20, 4.5f, 2, 20, -22, 9, 1, 2, -8, 1.5f, 3, 4, -184, 30, 5));
}

TEST(Matrix4, TRS)
{
	Matrix4 test = Matrix4::Identity();
	test = test.TRS(Vector3(10.f, -5.f, 3.5f), Vector3(3.14f, 3.14f / 2.f, -3.14f / 2.f), Vector3(2, -4, 1.5f));
	EXPECT_EQ(test, Matrix4(0.0000012681f, 3.99998879f, -0.00358323079f, 10, -0.00159254810f, 0.00955528114f, 1.49999535f, -5, -1.99999940f, -0.00000507242, -0.00119440991f, 3.5f, 0, 0, 0, 1));
}

TEST(Matrix4, TRS_Quaternion)
{
	Matrix4 test1;
	test1 = test1.TRS(Vector3(10.f, -5.f, 3.5f), Quaternion::EulerToQuaternion(Vector3(3.14f, 3.14f / 2.f, -3.14f / 2.f)), Vector3(2, -4, 1.5f));
	Matrix4 result(0);
	/*
	EXPECT_NEAR(test1.data[0][0], result.data[0][0], EPSILON);
	EXPECT_NEAR(test1.data[0][1], result.data[0][1], EPSILON);
	EXPECT_NEAR(test1.data[0][2], result.data[0][2], EPSILON);
	EXPECT_NEAR(test1.data[0][3], result.data[0][3], EPSILON);

	EXPECT_NEAR(test1.data[1][0], result.data[1][0], EPSILON);
	EXPECT_NEAR(test1.data[1][1], result.data[1][1], EPSILON);
	EXPECT_NEAR(test1.data[1][2], result.data[1][2], EPSILON);
	EXPECT_NEAR(test1.data[1][3], result.data[1][3], EPSILON);

	EXPECT_NEAR(test1.data[2][0], result.data[2][0], EPSILON);
	EXPECT_NEAR(test1.data[2][1], result.data[2][1], EPSILON);
	EXPECT_NEAR(test1.data[2][2], result.data[2][2], EPSILON);
	EXPECT_NEAR(test1.data[2][3], result.data[2][3], EPSILON);

	EXPECT_NEAR(test1.data[3][0], result.data[3][0], EPSILON);
	EXPECT_NEAR(test1.data[3][1], result.data[3][1], EPSILON);
	EXPECT_NEAR(test1.data[3][2], result.data[3][2], EPSILON);
	EXPECT_NEAR(test1.data[3][3], result.data[3][3], EPSILON);
	*/
}

TEST(Matrix4, Projection)
{
	Matrix4 test = Matrix4::Projection(1.57079637f, 800.f / 600.f, 0.100000001f, 100);
	EXPECT_EQ(test, Matrix4(0.75f, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1.00200200, -0.200200200, 0, 0, -1, 0));
}

TEST(Matrix4, LookAt)
{
	Matrix4 test = Matrix4::LookAt(Vector3(0, 0, 5), Vector3(0, 0, -1), Vector3(0, 1, 0));
	EXPECT_EQ(test, Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -5, 0, 0, 0, 1));
}

TEST(Matrix4, Operator_Add_Float)
{
	Matrix4 test = Matrix4(1.f, 2.f, 3.f, -4.f, 5.f, 2.5f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
	test = test + 5;
	EXPECT_EQ(test, Matrix4(6.f, 7.f, 8.f, 1.f, 10.f, 7.5f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f, 20.f));
}

TEST(Matrix4, Operator_Substract_Float)
{
	Matrix4 test = Matrix4(1.f, 2.f, 3.f, -4.f, 5.f, 2.5f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
	test = test - 5;
	EXPECT_EQ(test, Matrix4(-4.f, -3.f, -2.f, -9.f, 0.f, -2.5f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f));
}

TEST(Matrix4, Operator_Multiply_Float)
{
	Matrix4 test = Matrix4(1.f, 2.f, 3.f, -4.f, 5.f, 2.5f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
	test = test * 3;
	EXPECT_EQ(test, Matrix4(3.f, 6.f, 9.f, -12.f, 15.f, 7.5f, 18.f, 21.f, 24.f, 27.f, 30.f, 33.f, 36.f, 39.f, 42.f, 45.f));
}

TEST(Matrix4, Operator_Divide_Float)
{
	Matrix4 test = Matrix4(1.f, 2.f, 4.f, -4.f, 5.f, 3.f, 6.f, 8.f, 10.f, 12.f, 16.f, 20.f, 24.f, 28.f, 32.f, 36.f);
	test = test / 4;
	EXPECT_EQ(test, Matrix4(0.25f, 0.5f, 1.f, -1.f, 1.25f, 0.75f, 1.5f, 2.f, 2.5f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f));
	EXPECT_EQ(test / 0.0f, Matrix4(0));
}

TEST(Matrix4, Operator_Add_Matrix4)
{
	Matrix4 test1 = Matrix4(1.f, 2.f, 4.f, -4.f, 5.f, 3.f, 6.f, 8.f, 10.f, 12.f, 16.f, 20.f, 24.f, 28.f, 32.f, 36.f);
	Matrix4 test2 = Matrix4(-1.f, -2.f, -4.f, 4.f, -5.f, -3.f, -6.f, -8.f, -10.f, -12.f, -16.f, -20.f, -24.f, -28.f, -32.f, -36.f);
	test1 = test1 + test2;
	EXPECT_EQ(test1, Matrix4(0));
}

TEST(Matrix4, Operator_Substract_Matrix4)
{
	Matrix4 test = Matrix4(1.f, 2.f, 4.f, -4.f, 5.f, 3.f, 6.f, 8.f, 10.f, 12.f, 16.f, 20.f, 24.f, 28.f, 32.f, 36.f);
	test = test - test;
	EXPECT_EQ(test, Matrix4(0));
}

TEST(Matrix4, Operator_Multiply_Matrix4)
{
	Matrix4 test = Matrix4(
		1, 0, 0, 0, 
		0, 1, 0, 0, 
		0, 0, 1, 0, 
		0, 0, 0, 1);
	Matrix4 test1 = Matrix4(
		1, 0, 0, 3,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	test = test * test1;
	EXPECT_EQ(test, Matrix4(1, 0, 0, 3, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
}

TEST(Matrix4, Operator_Equal_Matrix4)
{
	Matrix4 test1 = Matrix4(1.f, 2.f, 4.f, -4.f, 5.f, 3.f, 6.f, 8.f, 10.f, 12.f, 16.f, 20.f, 24.f, 28.f, 32.f, 36.f);
	Matrix4 test2 = Matrix4(1.f, 2.f, 4.f, -4.f, 5.f, 3.f, 6.f, 8.f, 10.f, 12.f, 16.f, 20.f, 24.f, 28.f, 32.f, 36.f);
	EXPECT_EQ(test1, test2);
}

TEST(Matrix4, Operator_Not_Equal_Matrix4)
{
	Matrix4 test1 = Matrix4(1.f, 2.f, 4.f, -4.f, 5.f, 3.f, 6.f, 8.f, 10.f, 12.f, 16.f, 20.f, 24.f, 28.f, 32.f, 36.f);
	Matrix4 test2 = Matrix4(1.f, 2.f, 4.f, -4.f, 5.f, 3.f, 6.f, 8.f, 10.f, 12.f, 16.f, 20.f, 24.f, 28.f, 0.f, 36.f);
	EXPECT_NE(test1, test2);
}

#pragma endregion Matrix4

#pragma region Quaternion

TEST(Quaternion, Constructor)
{
	Quaternion test1 = Quaternion(1.0f, Vector3(1.0f, 2.0f, 3.0f));
	EXPECT_EQ(1.0f, test1.w);
	EXPECT_EQ(Vector3(1.0f, 2.0f, 3.0f), Vector3(test1.x, test1.y, test1.z));
}

TEST(Quaternion, Norm)
{
	Quaternion test1 = Quaternion(1.0f, Vector3(1.0f, 2.0f, 3.0f));
	EXPECT_NEAR(3.87f, test1.Norm(), EPSILON);
}

TEST(Quaternion, Normalized)
{
	Quaternion test1 = Quaternion(1.0f, Vector3(1.0f, 2.0f, 3.0f));
	test1 = test1.Normalized();
	Quaternion result = Quaternion(0.25f, Vector3(0.25f, 0.51f, 0.77f));
	EXPECT_NEAR(test1.w, result.w, EPSILON);
	EXPECT_NEAR(test1.x, result.x, EPSILON);
	EXPECT_NEAR(test1.y, result.y, EPSILON);
	EXPECT_NEAR(test1.z, result.z, EPSILON);
}

TEST(Quaternion, Conjugate)
{
	Quaternion test1 = Quaternion(1.0f, Vector3(1.0f, 2.0f, 3.0f));
	EXPECT_EQ(Quaternion(1.0f, Vector3(-1.0f, -2.0f, -3.0f)), test1.Conjugate());
}

TEST(Quaternion, Inverse)
{
	Quaternion test1 = Quaternion(1.0f, Vector3(1.0f, 2.0f, 3.0f));
	test1 = test1.Inverse();
	Quaternion result = Quaternion(0.067f, Vector3(-0.067f, -0.133f, -0.2f));
	EXPECT_NEAR(test1.w, result.w, EPSILON);
	EXPECT_NEAR(test1.x, result.x, EPSILON);
	EXPECT_NEAR(test1.y, result.y, EPSILON);
	EXPECT_NEAR(test1.z, result.z, EPSILON);
}

TEST(Quaternion, Rotate_Quaternion_Around_Axis)
{
	Quaternion test1 = Quaternion(0.0f, Vector3(1.0f, 0.0f, 0.0f));
	test1 = test1.RotateQuaternionAroundAxis(90, Vector3(0, 1, 0));
	Quaternion result = Quaternion(0.0f, Vector3(-0.0f, -0.0f, -1.0f));
	EXPECT_NEAR(test1.w, result.w, EPSILON);
	EXPECT_NEAR(test1.x, result.x, EPSILON);
	EXPECT_NEAR(test1.y, result.y, EPSILON);
	EXPECT_NEAR(test1.z, result.z, EPSILON);
}

TEST(Quaternion, Quaternion_to_Euler)
{
	Quaternion test = Quaternion::EulerToQuaternion(Vector3(3.14f, 3.14f / 2.f, 3.14f / 4.f));
	Vector3 test1 = Vector3::QuaternionToEuler(test);
	Vector3 result(3.14f, 3.14f / 2.f, 3.14f / 4.f);
	EXPECT_NEAR(test1.x, result.x, EPSILON);
	EXPECT_NEAR(test1.y, result.y, EPSILON);
	EXPECT_NEAR(test1.z, result.z, EPSILON);
}

TEST(Quaternion, Operator_Multiply_Quaternion)
{
	Quaternion test1 = Quaternion(1.0f, Vector3(1.0f, 2.0f, 3.0f));
	Quaternion test2 = Quaternion(-1.0f, Vector3(1.5f, 3.0f, -3.0f));
	test1 = test1 * test2;
	Quaternion result = Quaternion(0.5f, Vector3(-14.5f, 8.5f, -6.f));
	EXPECT_NEAR(test1.w, result.w, EPSILON);
	EXPECT_NEAR(test1.x, result.x, EPSILON);
	EXPECT_NEAR(test1.y, result.y, EPSILON);
	EXPECT_NEAR(test1.z, result.z, EPSILON);
}

TEST(Quaternion, Operator_Equal_Quaternion)
{
	Quaternion test1 = Quaternion(1.0f, Vector3(1.0f, 2.0f, 3.0f));
	EXPECT_EQ(test1, test1);
}

TEST(Quaternion, Operator_Not_Equal_Quaternion)
{
	Quaternion test1 = Quaternion(1.0f, Vector3(1.0f, 2.0f, 3.0f));
	Quaternion test2 = Quaternion(2.0f, Vector3(1.0f, 2.0f, 3.0f));
	EXPECT_NE(test1, test2);
}
#pragma endregion Quaternion

