#include <random>
#include <thread>
#include <fstream>

#include "World.h"
#include "BlockManager.h"
#include "DisplayListManager.h"
#include "Matrix.h"

#include <gl\glut.h>

using namespace std;

//木の生成
void World::generateWood(int x, int y, int z)
{
	random_device rnd;
	mt19937 mt(rnd());
	uniform_int_distribution<int> rand46(4, 6);

	//木の幹の高さ
	int height = rand46(mt);
	//木の葉の生成
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
	//木の幹の生成
	for (int i = 0; i < height; i++)
		chunk[x][y + i][z] = WorldBlock(3);
}

//ランダムな位置に木を生成する
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

//チャンクの読み込み
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

//アイテムスロットの読み込み
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

//プレイヤーの情報の読み込み
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

//チャンクの生成
void World::generateChunk(void)
{
	//セーブデータがあれば，読み込む
	ifstream file("chunk.dat");
	if (!file.fail()) {
		readChunk();
		readItemSlot();
		readPlayerPosition();
		return;
	}
	//地面・空気の生成
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
	//木の生成
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
				//見えないブロックは無視
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
				//少し後ろのブロックも描画対象とする（横を向くときに移るようにするため&バック時の当たり判定のため）
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
	//更新中ならリターン
	if (isBusyUpdateChunkVisible)
		return;
	isBusyUpdateChunkVisible = true;
	thread th(&World::threadUpdateChunkVisible, this);
	th.detach();
}

//更新を待つ
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

//プレイヤーを動かす
bool World::tryMovePlayer(const Vector& moveVector)
{
	Vector moveDirection(moveVector);
	//移動前の位置
	Vector beforePosition = player.getPosition();
	//移動する
	player.getPosition() += moveDirection;

	//ブロックと衝突したら
	if (isHitChunk()) {
		//一度位置を戻す
		player.getPosition() = beforePosition;
		Vector blockDirection = getBlockDirection(lastHitBlockPosition);
		//衝突方向の成分を消すためのフィルター
		Vector filter = Vector(1, 1, 1) - Vector(fabs(blockDirection.getX()), fabs(blockDirection.getY()), fabs(blockDirection.getZ()));
		//衝突方向の成分を消す
		moveDirection = moveDirection.componentProduct(filter);
		//衝突しない方向に移動する
		player.getPosition() += moveDirection;
		//他のブロックと衝突したら
		if (isHitChunk())
			player.getPosition() = beforePosition;
		return false;
	}
	//全く衝突しなかったらtrueを返す
	return true;
}

void World::movePlayer(void)
{
	//ジャンプする
	if (!player.isFlying() && keyboard.getKey(' ')) {
		player.getVelocity() += Vector(0, sqrt(2 * GRAVITY_ACCELERATION) + 0.5, 0);
		player.isFlying() = true;
	}
	//速度に従って移動する
	player.getVelocity() += Vector(0, -GRAVITY_ACCELERATION / fps.get(), 0);
	Vector moveDirection = player.getVelocity() / fps.get();
	//地面に落下したら
	if (!tryMovePlayer(moveDirection)) {
		player.isFlying() = false;
		player.getVelocity() = Vector(0, 0, 0);
	}
	
	double speed = IDEAL_FPS / fps.get() * 0.15;
	//キーボードによる移動方向
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
	//角度に変換
	double theta = atan2(keyboardVector.getZ(), keyboardVector.getX());
	//回転行列
	Matrix Ry;
	Ry[0][0] = Ry[2][2] = cos(-theta);
	Ry[0][2] = sin(-theta);
	Ry[2][0] = -Ry[0][2];
	Ry[1][1] = 1.0;
	//目線のY軸成分を無視するための行列
	Matrix M;
	M[0][0] = M[2][2] = 1.0;
	Vector beforePosition = player.getPosition();
	moveDirection = Ry * M * player.getDirection() * speed;
	//移動する
	tryMovePlayer(moveDirection);
}

void World::leftClickPlayer(void)
{
	//左クリックしていなかったら
	if (!mouse.getLeft())
		return;
	//どのブロックにもリーチが届いていなかったら
	if (!isHitReachChunk()) {
		mouse.getLeftCount() = 0;
		return;
	}
	//一定の時間左クリックしていたら
	if (mouse.getLeftCount() > 15) {
		mouse.getLeftCount() = 0;
		WorldBlock& worldBlock = chunk[lastHitBlockIdx[0]][lastHitBlockIdx[1]][lastHitBlockIdx[2]];
		//アイテムをスポーンさせる
		spawnItemBlock(worldBlock.getBlockID(), lastHitBlockPosition + Vector(0.5, 0.5, 0.5));
		//ブロックを空気ブロックに置換
		worldBlock = WorldBlock(0);
		//ディスプレイリストの更新
		updateDisplayList();
	}
}

void World::rightClickPlayer(void)
{
	static int cnt = 0;
	//右クリックしていなかったら
	if (!mouse.getRight()) {
		cnt = IDEAL_FPS / 4;
		return;
	}
	ItemSlot& itemSlot = player.getItemSlot();
	ItemStack& itemStack = itemSlot.getCurrentItemStack();
	//選択しているスロットにアイテムがなかったら
	if (!itemStack.getNum())
		return;
	//どのブロックにもリーチが届いていなかったら
	if (!isHitReachChunk())
		return;
	//ワンクリックで複数個のブロックが置かれないようにするための処理
	if (++cnt < (int)fps.get() / 4)
		return;
	//ブロックを置く面
	int face = lastHitBlockFace;
	
	//リーチの届いたブロックの位置
	int x = lastHitBlockIdx[0];
	int y = lastHitBlockIdx[1];
	int z = lastHitBlockIdx[2];
	//ブロックを置く位置の取得
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
	//ブロックを置く
	chunk[x][y][z] = WorldBlock(itemStack.getBlockID());
	//スロットからアイテムを減らす
	itemSlot.useItem();
	//ディスプレイリストの更新
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
					//ブロックの中心位置
					Vector blockCenterPosition = blockPosition + Vector(0.5, 0.5, 0.5);
					//ブロックとプレイヤーの距離
					double distance = (blockCenterPosition - (player.getPosition() + player.getEyePosition())).norm();
					//一番距離が近いブロックを求める
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
	//落下処理
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
		//回転処理
		itemBlock.update();
	}

	//プレイヤーによる収集
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

//プレイヤーに対するブロックの方向を，座標軸に沿った単位ベクトルで返す
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

//チャンクデータの保存
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

//アイテムスロットのデータの保存
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

//プレイヤーの情報の保存
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
