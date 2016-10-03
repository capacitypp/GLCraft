#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "Mouse.h"
#include "Matrix.h"

using namespace std;

void Mouse::update(void)
{
	//押しっぱなしにすると，カウンターが増加する
	if (left)
		leftCnt++;
	else
		leftCnt = 0;
}

void Mouse::updatePlayerDirection(Player& player)
{
	int dx = x - bx;
	int dy = -(y - by);
	//10pixelで1度
	double dphi = dx / 10.0 / 180 * M_PI;
	double dtheta = dy / 10.0 / 180 * M_PI;

	Vector& direction = player.getDirection();

	//上を向く単位ベクトル
	Vector Vy;
	Vy.getY() = 1.0;
	//Y軸，プレイヤーの目線と垂直なベクトルを求める
	Vector v = -Vy.outerProduct(direction);
	Matrix R;
	R[1][0] = v.getZ();
	R[0][1] = -R[1][0];
	R[0][2] = v.getY();
	R[2][0] = -R[0][2];
	R[2][1] = v.getX();
	R[1][2] = -R[2][1];
	//単位行列
	Matrix I;
	I[0][0] = I[1][1] = I[2][2] = 1.0;
	//目線を上に回転する回転行列
	Matrix M = I + R * sin(dtheta) + R * R * (1 - cos(dtheta));
	//目線をY軸周りに回転する回転行列
	Matrix Ry;
	Ry[0][0] = Ry[2][2] = cos(-dphi);
	Ry[0][2] = sin(-dphi);
	Ry[2][0] = -Ry[0][2];
	Ry[1][1] = 1.0;

	//目線の方向を回転する
	direction = M * Ry * direction;
	//正規化する
	direction /= direction.norm();
}

