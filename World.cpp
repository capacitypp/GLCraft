#include <random>
#include <thread>
#include <fstream>

#include "World.h"
#include "BlockManager.h"
#include "DisplayListManager.h"
#include "Matrix.h"

#include <gl\glut.h>

using namespace std;

//�؂̐���
void World::generateWood(int x, int y, int z)
{
	random_device rnd;
	mt19937 mt(rnd());
	uniform_int_distribution<int> rand46(4, 6);

	//�؂̊��̍���
	int height = rand46(mt);
	//�؂̗t�̐���
	for (int i = -2; i <= 2; i++)
	for (int j = -2; j <= 2; j++)
	for (int k = 0; k < 2; k++)
	if (rand46(mt) % 4)
		chunk[x + i][y + height - 2 - k][z + j] = WorldBlock(4);
	chunk[x - 1][y + height - 1][z] = WorldBlock(4);
	chunk[x + 1][y + height - 1][z] = WorldBlock(4);
	chunk[x][y + height - 1][z - 1] = WorldBlock(4);
	chunk[x][y + height - 1][z + 1] = WorldBlock(4);
	chunk[x][y + height][z] = WorldBlock(4);
	//�؂̊��̐���
	for (int i = 0; i < height; i++)
		chunk[x][y + i][z] = WorldBlock(3);
}

//�����_���Ȉʒu�ɖ؂𐶐�����
void World::generateWoods(void)
{
	random_device rnd;
	mt19937 mt(rnd());
	uniform_int_distribution<int> rand099(0, 99);
	for (int i = 2; i < WORLD_WIDTH - 2; i++)
		for (int k = 2; k < WORLD_WIDTH - 2; k++)
		if (!rand099(mt))
			generateWood(i, GROUND_HEIGHT, k);

}

//�`�����N�̓ǂݍ���
void World::readChunk(void)
{
	ifstream file;
	file.open("chunk.dat", ifstream::binary);
	for (int i = 0; i < WORLD_WIDTH; i++) {
		for (int j = 0; j < WORLD_HEIGHT; j++) {
			for (int k = 0; k < WORLD_WIDTH; k++) {
				int blockID;
				file.read((char*)&blockID, sizeof(blockID));
				chunk[i][j][k] = WorldBlock(blockID);
			}
		}
	}
}

//�A�C�e���X���b�g�̓ǂݍ���
void World::readItemSlot(void)
{
	ifstream file;
	file.open("itemSlot.dat", ifstream::binary);
	int blockID, num;
	ItemSlot& itemSlot = player.getItemSlot();
	for (int i = 0; i < MAX_ITEMSLOT; i++) {
		ItemStack& itemStack = itemSlot.getItemStack(i);
		file.read((char*)&blockID, sizeof(blockID));
		file.read((char*)&num, sizeof(num));
		itemStack = ItemStack(blockID, num);
	}
}

//�v���C���[�̏��̓ǂݍ���
void World::readPlayerPosition(void)
{
	ifstream file;
	file.open("player.dat", ifstream::binary);
	double value;
	Vector& playerPosition = player.getPosition();
	file.read((char*)&value, sizeof(value));
	playerPosition.getX() = value;
	file.read((char*)&value, sizeof(value));
	playerPosition.getY() = value;
	file.read((char*)&value, sizeof(value));
	playerPosition.getZ() = value;
	Vector& playerDirection = player.getDirection();
	file.read((char*)&value, sizeof(value));
	playerDirection.getX() = value;
	file.read((char*)&value, sizeof(value));
	playerDirection.getY() = value;
	file.read((char*)&value, sizeof(value));
	playerDirection.getZ() = value;
}

//�`�����N�̐���
void World::generateChunk(void)
{
	//�Z�[�u�f�[�^������΁C�ǂݍ���
	ifstream file("chunk.dat");
	if (!file.fail()) {
		readChunk();
		readItemSlot();
		readPlayerPosition();
		return;
	}
	//�n�ʁE��C�̐���
	for (int i = 0; i < WORLD_WIDTH; i++) {
		for (int j = 0; j < WORLD_HEIGHT; j++) {
			for (int k = 0; k < WORLD_WIDTH; k++) {
				if (j >= GROUND_HEIGHT)
					chunk[i][j][k] = WorldBlock(0);
				else
					chunk[i][j][k] = WorldBlock(2);
			}
		}
	}
	//�؂̐���
	generateWoods();
}

void World::registerDisplayList(void)
{
	displayListID = DisplayListManager::getNewID();
	updateDisplayList();
}

void World::updateDisplayList(void)
{
	glNewList(displayListID, GL_COMPILE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glPushMatrix();
	glTranslated(-WORLD_WIDTH / 2, 0, -WORLD_WIDTH / 2);
	for (int j = 0; j < WORLD_HEIGHT; j++) {
		glPushMatrix();
		for (int i = 0; i < WORLD_WIDTH; i++) {
			glPushMatrix();
			for (int k = 0; k < WORLD_WIDTH; k++) {
				const WorldBlock& worldBlock = chunk[i][j][k];
				//�����Ȃ��u���b�N�͖���
				if (!worldBlock.isVisible()) {
					glTranslated(0, 0, 1);
					continue;
				}
				const Block& block = BlockManager::getBlock(worldBlock.getBlockID());
				block.draw();
				glTranslated(0, 0, 1);
			}
			glPopMatrix();
			glTranslated(1, 0, 0);
		}
		glPopMatrix();
		glTranslated(0, 1, 0);
	}
	glPopMatrix();
	glDisable(GL_BLEND);
	glEndList();
}

void World::threadUpdateChunkVisible(void)
{
	for (int j = 0; j < WORLD_HEIGHT; j++) {
		for (int i = 0; i < WORLD_WIDTH; i++) {
			for (int k = 0; k < WORLD_WIDTH; k++) {
				WorldBlock& worldBlock = chunk[i][j][k];
				worldBlock.isVisible() = false;
				if (worldBlock.getBlockID() == 0)
					continue;
				Vector blockPosition(i - WORLD_WIDTH / 2, j, k - WORLD_WIDTH / 2);
				//�������̃u���b�N���`��ΏۂƂ���i���������Ƃ��Ɉڂ�悤�ɂ��邽��&�o�b�N���̓����蔻��̂��߁j
				Vector blockDirection = blockPosition - player.getPosition() + player.getDirection() * 5;
				if (blockDirection * player.getDirection() < 0)
					continue;
				if (blockDirection.norm() > 20)
					continue;
				worldBlock.isVisible() = true;
			}
		}
	}
	isBusyUpdateChunkVisible = false;
}

void World::updateChunkVisible(void)
{
	//�X�V���Ȃ烊�^�[��
	if (isBusyUpdateChunkVisible)
		return;
	isBusyUpdateChunkVisible = true;
	thread th(&World::threadUpdateChunkVisible, this);
	th.detach();
}

//�X�V��҂�
void World::waitForUpdateChunkVisible(void)
{
	while (isBusyUpdateChunkVisible);
}

void World::draw(void) const
{
	glEnable(GL_TEXTURE_2D);
	glCallList(displayListID);
	glDisable(GL_TEXTURE_2D);
}

//�v���C���[�𓮂���
bool World::tryMovePlayer(const Vector& moveVector)
{
	Vector moveDirection(moveVector);
	//�ړ��O�̈ʒu
	Vector beforePosition = player.getPosition();
	//�ړ�����
	player.getPosition() += moveDirection;

	//�u���b�N�ƏՓ˂�����
	if (isHitChunk()) {
		//��x�ʒu��߂�
		player.getPosition() = beforePosition;
		Vector blockDirection = getBlockDirection(lastHitBlockPosition);
		//�Փ˕����̐������������߂̃t�B���^�[
		Vector filter = Vector(1, 1, 1) - Vector(fabs(blockDirection.getX()), fabs(blockDirection.getY()), fabs(blockDirection.getZ()));
		//�Փ˕����̐���������
		moveDirection = moveDirection.componentProduct(filter);
		//�Փ˂��Ȃ������Ɉړ�����
		player.getPosition() += moveDirection;
		//���̃u���b�N�ƏՓ˂�����
		if (isHitChunk())
			player.getPosition() = beforePosition;
		return false;
	}
	//�S���Փ˂��Ȃ�������true��Ԃ�
	return true;
}

void World::movePlayer(void)
{
	//�W�����v����
	if (!player.isFlying() && keyboard.getKey(' ')) {
		player.getVelocity() += Vector(0, sqrt(2 * GRAVITY_ACCELERATION) + 0.5, 0);
		player.isFlying() = true;
	}
	//���x�ɏ]���Ĉړ�����
	player.getVelocity() += Vector(0, -GRAVITY_ACCELERATION / fps.get(), 0);
	Vector moveDirection = player.getVelocity() / fps.get();
	//�n�ʂɗ���������
	if (!tryMovePlayer(moveDirection)) {
		player.isFlying() = false;
		player.getVelocity() = Vector(0, 0, 0);
	}
	
	double speed = IDEAL_FPS / fps.get() * 0.15;
	//�L�[�{�[�h�ɂ��ړ�����
	Vector keyboardVector;
	if (keyboard.getKey('d') || keyboard.getKey('D'))
		keyboardVector.getZ() = 1.0;
	if (keyboard.getKey('a') || keyboard.getKey('A'))
		keyboardVector.getZ() = -1.0;
	if (keyboard.getKey('w') || keyboard.getKey('W'))
		keyboardVector.getX() = 1.0;
	if (keyboard.getKey('s') || keyboard.getKey('S'))
		keyboardVector.getX() = -1.0;
	if (keyboardVector.norm() < 0.1)
		return;
	//�p�x�ɕϊ�
	double theta = atan2(keyboardVector.getZ(), keyboardVector.getX());
	//��]�s��
	Matrix Ry;
	Ry[0][0] = Ry[2][2] = cos(-theta);
	Ry[0][2] = sin(-theta);
	Ry[2][0] = -Ry[0][2];
	Ry[1][1] = 1.0;
	//�ڐ���Y�������𖳎����邽�߂̍s��
	Matrix M;
	M[0][0] = M[2][2] = 1.0;
	Vector beforePosition = player.getPosition();
	moveDirection = Ry * M * player.getDirection() * speed;
	//�ړ�����
	tryMovePlayer(moveDirection);
}

void World::leftClickPlayer(void)
{
	//���N���b�N���Ă��Ȃ�������
	if (!mouse.getLeft())
		return;
	//�ǂ̃u���b�N�ɂ����[�`���͂��Ă��Ȃ�������
	if (!isHitReachChunk()) {
		mouse.getLeftCount() = 0;
		return;
	}
	//���̎��ԍ��N���b�N���Ă�����
	if (mouse.getLeftCount() > 15) {
		mouse.getLeftCount() = 0;
		WorldBlock& worldBlock = chunk[lastHitBlockIdx[0]][lastHitBlockIdx[1]][lastHitBlockIdx[2]];
		//�A�C�e�����X�|�[��������
		spawnItemBlock(worldBlock.getBlockID(), lastHitBlockPosition + Vector(0.5, 0.5, 0.5));
		//�u���b�N����C�u���b�N�ɒu��
		worldBlock = WorldBlock(0);
		//�f�B�X�v���C���X�g�̍X�V
		updateDisplayList();
	}
}

void World::rightClickPlayer(void)
{
	static int cnt = 0;
	//�E�N���b�N���Ă��Ȃ�������
	if (!mouse.getRight()) {
		cnt = IDEAL_FPS / 4;
		return;
	}
	ItemSlot& itemSlot = player.getItemSlot();
	ItemStack& itemStack = itemSlot.getCurrentItemStack();
	//�I�����Ă���X���b�g�ɃA�C�e�����Ȃ�������
	if (!itemStack.getNum())
		return;
	//�ǂ̃u���b�N�ɂ����[�`���͂��Ă��Ȃ�������
	if (!isHitReachChunk())
		return;
	//�����N���b�N�ŕ����̃u���b�N���u����Ȃ��悤�ɂ��邽�߂̏���
	if (++cnt < (int)fps.get() / 4)
		return;
	//�u���b�N��u����
	int face = lastHitBlockFace;
	
	//���[�`�̓͂����u���b�N�̈ʒu
	int x = lastHitBlockIdx[0];
	int y = lastHitBlockIdx[1];
	int z = lastHitBlockIdx[2];
	//�u���b�N��u���ʒu�̎擾
	switch (face) {
	case 0:
		y++;
		break;
	case 1:
		z--;
		break;
	case 2:
		x++;
		break;
	case 3:
		z++;
		break;
	case 4:
		x--;
		break;
	case 5:
		y--;
		break;
	}
	//�u���b�N��u��
	chunk[x][y][z] = WorldBlock(itemStack.getBlockID());
	//�X���b�g����A�C�e�������炷
	itemSlot.useItem();
	//�f�B�X�v���C���X�g�̍X�V
	updateDisplayList();
	cnt = 0;
}

void World::keyboardPlayer(void)
{
	player.updateItemSlot(keyboard);
}

bool World::isHitChunk(void)
{
	_isLastHitReach = false;
	const Vector& playerPosition = player.getPosition();
	for (int j = 0; j < WORLD_HEIGHT; j++) {
		for (int i = 0; i < WORLD_WIDTH; i++) {
			for (int k = 0; k < WORLD_WIDTH; k++) {
				const WorldBlock& worldBlock = chunk[i][j][k];
				Vector blockPosition(i - WORLD_WIDTH / 2, j, k - WORLD_WIDTH / 2);
				if (worldBlock.isHitPlayer(blockPosition, playerPosition)) {
					lastHitBlockPosition = blockPosition;
					return true;
				}
			}
		}
	}
	return false;
}

bool World::isHitReachChunk(void)
{
	_isLastHitReach = false;
	lastHitBlockDistance = 100;
	for (int j = 0; j < WORLD_HEIGHT; j++) {
		for (int i = 0; i < WORLD_WIDTH; i++) {
			for (int k = 0; k < WORLD_WIDTH; k++) {
				const WorldBlock& worldBlock = chunk[i][j][k];
				Vector blockPosition(i - WORLD_WIDTH / 2, j, k - WORLD_WIDTH / 2);
				if (worldBlock.isHitReach(blockPosition, player)) {
					//�u���b�N�̒��S�ʒu
					Vector blockCenterPosition = blockPosition + Vector(0.5, 0.5, 0.5);
					//�u���b�N�ƃv���C���[�̋���
					double distance = (blockCenterPosition - (player.getPosition() + player.getEyePosition())).norm();
					//��ԋ������߂��u���b�N�����߂�
					if (distance > lastHitBlockDistance)
						continue;
					lastHitBlockDistance = distance;
					lastHitBlockPosition = blockPosition;
					lastHitBlockIdx[0] = i;
					lastHitBlockIdx[1] = j;
					lastHitBlockIdx[2] = k;
					lastHitBlockFace = Block::getLastHitReachFace();
					_isLastHitReach = true;
				}
			}
		}
	}
	if (_isLastHitReach)
		return true;
	return false;
}

void World::drawItemBlocks(void) const
{
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	for (int i = 0; i < MAX_ITEMBLOCK; i++) {
		const ItemBlock& itemBlock = itemBlocks[i];
		if (!itemBlock.getFlag())
			continue;
		itemBlock.draw();
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void World::updateItemBlocks(void)
{
	//��������
	for (int i = 0; i < MAX_ITEMBLOCK; i++) {
		ItemBlock& itemBlock = itemBlocks[i];
		if (!itemBlock.getFlag())
			continue;
		Vector& velocity = itemBlock.getVelocity();
		velocity += Vector(0, -GRAVITY_ACCELERATION / fps.get(), 0);
		Vector moveDirection = velocity / fps.get();
		Vector& position = itemBlock.getPosition();
		Vector beforePosition(position);
		position += moveDirection;
		if (position.getY() < 0) {
			itemBlock = ItemBlock();
			continue;
		}
		Vector bottomPosition = position - Vector(0, 0.3, 0);
		bool hitFlag = false;
		for (int j = 0; j < WORLD_WIDTH; j++) {
			for (int k = 0; k < WORLD_HEIGHT; k++) {
				for (int l = 0; l < WORLD_WIDTH; l++) {
					WorldBlock& worldBlock = chunk[j][k][l];
					Vector blockPosition(j - WORLD_WIDTH / 2, k, l - WORLD_WIDTH / 2);
					if (worldBlock.isHit(blockPosition, bottomPosition)) {
						position = beforePosition;
						velocity = Vector(0, 0, 0);
						hitFlag = true;
						break;
					}
				}
				if (hitFlag)
					break;
			}
			if (hitFlag)
				break;
		}
		//��]����
		itemBlock.update();
	}

	//�v���C���[�ɂ����W
	const Vector& playerPosition = player.getPosition() + Vector(0, PLAYER_HEIGHT / 2, 0);
	for (int i = 0; i < MAX_ITEMBLOCK; i++) {
		ItemBlock& itemBlock = itemBlocks[i];
		if ((itemBlock.getPosition() - playerPosition).norm() < 1.5) {
			if (player.getItemSlot().addItem(itemBlock.getBlockID()))
				itemBlock = ItemBlock();
		}
	}
}

void World::spawnItemBlock(int blockID, const Vector& position)
{
	for (int i = 0; i < MAX_ITEMBLOCK; i++) {
		ItemBlock& itemBlock = itemBlocks[i];
		if (itemBlock.getFlag())
			continue;
		itemBlock = ItemBlock(blockID, position);
		break;
	}
}

//�v���C���[�ɑ΂���u���b�N�̕������C���W���ɉ������P�ʃx�N�g���ŕԂ�
Vector World::getBlockDirection(const Vector& blockPosition)
{
	Vector blockDirection = blockPosition + Vector(0.5, 0.5, 0.5) - player.getPosition() - Vector(0, PLAYER_HEIGHT / 2, 0);
	double x = blockDirection.getX();
	double y = blockDirection.getY();
	double z = blockDirection.getZ();
	if (fabs(x) > fabs(y)) {
		if (fabs(x) > fabs(z)) {
			if (x > 0)
				return Vector(1, 0, 0);
			return Vector(-1, 0, 0);
		}
		if (z > 0)
			return Vector(0, 0, 1);
		return Vector(0, 0, -1);
	}
	else if (fabs(y) > fabs(z)) {
		if (y > 0)
			return Vector(0, 1, 0);
		return Vector(0, -1, 0);
	}
	else {
		if (z > 0)
			return Vector(0, 0, 1);
		return Vector(0, 0, -1);
	}
}

//�`�����N�f�[�^�̕ۑ�
void World::writeChunk(void) const
{
	ofstream file;
	file.open("chunk.dat", ofstream::binary);
	for (int i = 0; i < WORLD_WIDTH; i++) {
		for (int j = 0; j < WORLD_HEIGHT; j++) {
			for (int k = 0; k < WORLD_WIDTH; k++) {
				int blockID = chunk[i][j][k].getBlockID();
				file.write((const char*)&blockID, sizeof(blockID));
			}
		}
	}
}

//�A�C�e���X���b�g�̃f�[�^�̕ۑ�
void World::writeItemSlot(void) const
{
	ofstream file;
	file.open("itemSlot.dat", ofstream::binary);
	const ItemSlot& itemSlot = player.getItemSlot();
	for (int i = 0; i < MAX_ITEMSLOT; i++) {
		const ItemStack& itemStack = itemSlot.getItemStack(i);
		int blockID = itemStack.getBlockID();
		int num = itemStack.getNum();
		file.write((const char*)&blockID, sizeof(blockID));
		file.write((const char*)&num, sizeof(num));
	}
}

//�v���C���[�̏��̕ۑ�
void World::writePlayerPosition(void) const
{
	ofstream file;
	file.open("player.dat", ofstream::binary);
	double value;
	const Vector& playerPosition = player.getPosition();
	value = playerPosition.getX();
	file.write((const char*)&value, sizeof(value));
	value = playerPosition.getY();
	file.write((const char*)&value, sizeof(value));
	value = playerPosition.getZ();
	file.write((const char*)&value, sizeof(value));
	const Vector& playerDirection = player.getDirection();
	value = playerDirection.getX();
	file.write((const char*)&value, sizeof(value));
	value = playerDirection.getY();
	file.write((const char*)&value, sizeof(value));
	value = playerDirection.getZ();
	file.write((const char*)&value, sizeof(value));
}
