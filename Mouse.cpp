#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "Mouse.h"
#include "Matrix.h"

using namespace std;

void Mouse::update(void)
{
	//�������ςȂ��ɂ���ƁC�J�E���^�[����������
	if (left)
		leftCnt++;
	else
		leftCnt = 0;
}

void Mouse::updatePlayerDirection(Player& player)
{
	int dx = x - bx;
	int dy = -(y - by);
	//10pixel��1�x
	double dphi = dx / 10.0 / 180 * M_PI;
	double dtheta = dy / 10.0 / 180 * M_PI;

	Vector& direction = player.getDirection();

	//��������P�ʃx�N�g��
	Vector Vy;
	Vy.getY() = 1.0;
	//Y���C�v���C���[�̖ڐ��Ɛ����ȃx�N�g�������߂�
	Vector v = -Vy.outerProduct(direction);
	Matrix R;
	R[1][0] = v.getZ();
	R[0][1] = -R[1][0];
	R[0][2] = v.getY();
	R[2][0] = -R[0][2];
	R[2][1] = v.getX();
	R[1][2] = -R[2][1];
	//�P�ʍs��
	Matrix I;
	I[0][0] = I[1][1] = I[2][2] = 1.0;
	//�ڐ�����ɉ�]�����]�s��
	Matrix M = I + R * sin(dtheta) + R * R * (1 - cos(dtheta));
	//�ڐ���Y������ɉ�]�����]�s��
	Matrix Ry;
	Ry[0][0] = Ry[2][2] = cos(-dphi);
	Ry[0][2] = sin(-dphi);
	Ry[2][0] = -Ry[0][2];
	Ry[1][1] = 1.0;

	//�ڐ��̕�������]����
	direction = M * Ry * direction;
	//���K������
	direction /= direction.norm();
}

