#include "global.h"
#include "Block.h"

using namespace std;

//�Ō�ɍ�(�E)�N���b�N�����u���b�N�̖�
int Block::lastHitReachFace;

//�����֐�
int Block::sign(double x) const
{
	if (x > 0)
		return 1;
	if (x < 0)
		return -1;
	return 0;
}

//�v���C���[�̃��[�`�ƕ��ʂ̓����蔻��
bool Block::isHit(const Vector& vertex1, const Vector& vertex2, const Vector& vertex3, const Player& player) const
{
	//���ʂ̖@���x�N�g��
	Vector normal = (vertex2 - vertex1).outerProduct(vertex3 - vertex1);
	//�v���C���[�̖ڂ̈ʒu
	Vector playerPosition = player.getPosition() + player.getEyePosition();
	//�v���C���[�̃��[�`�̏I�_�x�N�g��
	Vector v1 = playerPosition + player.getDirection() * PLAYER_REACH - vertex1;
	//�v���C���[�̃��[�`�̎n�_�x�N�g��
	Vector v2 = playerPosition - vertex1;
	if (sign(v1 * normal) != sign(v2 * normal))
		return true;
	return false;
}

//�l�p�`�Ɠ_�̓����蔻��
bool Block::isHit(const Vector& vertex1, const Vector& vertex2, const Vector& vertex3, const Vector& vertex4, const Vector& position) const
{
	double x;
	//�Ӄx�N�g�����猩�ē_����ɓ��������ɂ��� -> �l�p�`���_���܂�
	Vector v1 = vertex2 - vertex1;
	Vector v2 = position - vertex1;
	x = v1 * v2;
	v1 = vertex3 - vertex2;
	v2 = position - vertex2;
	if (sign(x) != sign(v1 * v2))
		return false;
	v1 = vertex4 - vertex3;
	v2 = position - vertex3;
	if (sign(x) != sign(v1 * v2))
		return false;
	v1 = vertex1 - vertex4;
	v2 = position - vertex4;
	if (sign(x) != sign(v1 * v2))
		return false;
	return true;
}

//�v���C���[�̃��[�`�Ǝl�p�`�̓����蔻��
bool Block::isHit(const Vector& vertex1, const Vector& vertex2, const Vector& vertex3, const Vector& vertex4, const Player& player, Vector& position) const
{
	//���ʂƂ̓����蔻��
	if (!isHit(vertex1, vertex2, vertex3, player))
		return false;

	//�v���C���[�̖ڂ̈ʒu
	Vector playerPosition = player.getPosition() + player.getEyePosition();
	//�v���C���[�̃��[�`�ƕ��ʂ̌�_�����߂�
	double dist1 = getDistance(vertex1, vertex2, vertex3, playerPosition);
	double dist2 = getDistance(vertex1, vertex2, vertex3, playerPosition + player.getDirection() * PLAYER_REACH);
	position = playerPosition + player.getDirection() * (PLAYER_REACH * dist1 / (dist1 + dist2));

	//�l�p�`�Ƃ̓����蔻��
	return isHit(vertex1, vertex2, vertex3, vertex4, position);
}

//�_�ƕ��ʂ̋���
double Block::getDistance(const Vector& vertex1, const Vector& vertex2, const Vector& vertex3, const Vector& playerPosition) const
{
	Vector v = vertex1 - playerPosition;
	//���ʂ̒P�ʖ@���x�N�g��
	Vector normal = (vertex2 - vertex1).outerProduct(vertex3 - vertex1);
	normal /= normal.norm();
	//�_�ƕ��ʂ̋���
	double distance = v * normal;
	//���ς����ɂȂ�ꍇ
	if (distance < 0)
		distance *= -1;
	return distance;
}

//�e�N�X�`���̐ݒ�
void Block::registerTexture(const Image& image, GLuint id)
{
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//�~�b�v�}�b�v�𗘗p����ꍇ
	//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getWidth(), image.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

//�e�N�X�`����ʂɓ\��t����
void Block::drawTexture(GLuint id, const Vector& p1, const Vector& p2, const Vector& p3, const Vector& p4) const
{
	glBindTexture(GL_TEXTURE_2D, id);
//	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(p1.getX(), p1.getY(), p1.getZ());
	glTexCoord2d(1, 0);
	glVertex3d(p2.getX(), p2.getY(), p2.getZ());
	glTexCoord2d(1, 1);
	glVertex3d(p3.getX(), p3.getY(), p3.getZ());
	glTexCoord2d(0, 1);
	glVertex3d(p4.getX(), p4.getY(), p4.getZ());
	glEnd();
//	glDisable(GL_TEXTURE_2D);
}

//�u���b�N�Ɠ_�̓����蔻��
bool Block::isHit(const Vector& blockPosition, const Vector& position) const
{
	if (position.getX() < blockPosition.getX() || position.getX() > blockPosition.getX() + 1)
		return false;
	if (position.getY() < blockPosition.getY() || position.getY() > blockPosition.getY() + 1)
		return false;
	if (position.getZ() < blockPosition.getZ() || position.getZ() > blockPosition.getZ() + 1)
		return false;
	return true;
}

//�u���b�N�ƃv���C���[�̓����蔻��
bool Block::isHitPlayer(const Vector& blockPosition, const Vector& playerPosition) const
{
	//�n�ʂ̓����蔻��
	if (blockPosition.getY() + 0.95 < playerPosition.getY())
		return false;
	//�V��̓����蔻��
	if (blockPosition.getY() > playerPosition.getY() + PLAYER_HEIGHT)
		return false;
	//�ǂ̓����蔻��
	Vector blockVector = Vector(1, 0, 0);
	Vector playerVector = playerPosition + Vector(PLAYER_WIDTH / 2, 0, 0) - blockPosition;
	if (blockVector * playerVector < 0)
		return false;
	blockVector = Vector(-1, 0, 0);
	playerVector = playerPosition + Vector(-PLAYER_WIDTH / 2, 0, 0) - (blockPosition + Vector(1, 0, 0));
	if (blockVector * playerVector < 0)
		return false;
	blockVector = Vector(0, 0, 1);
	playerVector = playerPosition + Vector(0, 0, PLAYER_WIDTH / 2) - blockPosition;
	if (blockVector * playerVector < 0)
		return false;
	blockVector = Vector(0, 0, -1);
	playerVector = playerPosition + Vector(0, 0, -PLAYER_WIDTH / 2) - (blockPosition + Vector(0, 0, 1));
	if (blockVector * playerVector < 0)
		return false;
	return true;
}

//�u���b�N�ƃv���C���[�̃��[�`�̓����蔻��
bool Block::isHitReach(const Vector& blockPosition, const Player& player) const
{
	//�W�I���g�����
	Vector geometry[] = {
		blockPosition,
		blockPosition + Vector(1, 0, 0),
		blockPosition + Vector(1, 0, 1),
		blockPosition + Vector(0, 0, 1),
		blockPosition + Vector(0, 1, 0),
		blockPosition + Vector(1, 1, 0),
		blockPosition + Vector(1, 1, 1),
		blockPosition + Vector(0, 1, 1),
	};
	//�g�|���W���
	static int topology[][4] = {
		{ 7, 6, 5, 4 },
		{ 4, 5, 1, 0 },
		{ 5, 6, 2, 1 },
		{ 6, 7, 3, 2 },
		{ 7, 4, 0, 3 },
		{ 0, 1, 2, 3 },
	};
	Vector position;
	Vector nearestPosition(WORLD_WIDTH / 2, WORLD_HEIGHT, WORLD_WIDTH / 2);
	const Vector& playerPosition = player.getPosition();
	bool hitFlag = false;
	
	//6�ʂƂ̓����蔻��
	for (int i = 0; i < sizeof(topology) / sizeof(topology[0]); i++) {
		if (isHit(geometry[topology[i][0]], geometry[topology[i][1]], geometry[topology[i][2]], geometry[topology[i][3]], player, position)) {
			hitFlag = true;
			//�������Ă���ʂň�ԋ������߂��ʂƃ��[�`�̌�_�����߂�
			if ((position - playerPosition).norm() < (nearestPosition - playerPosition).norm()) {
				nearestPosition = position;
			}
		}
	}
	if (!hitFlag)
		return false;
	//�������Ă���ʂň�ԋ߂��ʂ����߂�
	Vector displacement = nearestPosition - blockPosition;
	if (fabs(displacement.getY() - 1) < 0.01)
		lastHitReachFace = 0;
	else if (fabs(displacement.getZ()) < 0.01)
		lastHitReachFace = 1;
	else if (fabs(displacement.getX() - 1) < 0.01)
		lastHitReachFace = 2;
	else if (fabs(displacement.getZ() - 1) < 0.01)
		lastHitReachFace = 3;
	else if (fabs(displacement.getX()) < 0.01)
		lastHitReachFace = 4;
	else
		lastHitReachFace = 5;

	return true;
}
