#include <iostream>

#include "Mouse.h"
#include "Player.h"
#include "World.h"
#include "StringManager.h"
#include "FPS.h"
#include "TextureManager.h"
#include "Keyboard.h"

#include "Block.h"
#include "BlockManager.h"

#include <GL/glut.h>

using namespace std;

Player player;
FPS fps;
Keyboard keyboard;
Mouse mouse;
World world(player, fps, keyboard, mouse);

void display(void)
{
	//FPSの更新
	fps.update();

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//画面の中心に十字を表示する
	glBegin(GL_LINES);
	glVertex3d(-0.0025, 0, -0.1);
	glVertex3d(0.0025, 0, -0.1);
	glVertex3d(0, -0.0025, -0.1);
	glVertex3d(0, 0.0025, -0.1);
	glEnd();
	//アイテムスロットの描画
	player.drawItemSlot();
	//FPSの描画
	StringManager::draw(0.66, 0.54, fps.get());

	//プレイヤーの目線に合わせる
	const Vector& direction = player.getDirection();
	gluLookAt(0, 0, 0, direction.getX(), direction.getY(), direction.getZ(), 0, 1, 0);
	const Vector& playerPosition = player.getPosition();
	glTranslated(-playerPosition.getX(), -playerPosition.getY(), -playerPosition.getZ());
	const Vector& eyePosition = player.getEyePosition();
	glTranslated(-eyePosition.getX(), -eyePosition.getY(), -eyePosition.getZ());

	//チャンクを描画
	world.draw();
	//ブロックの破壊中なら，破壊エフェクトを描画
	if (world.isLastHitReach()) {
		Vector position = world.getLastHitBlockPosition();
		glPushMatrix();
		glTranslated(position.getX(), position.getY(), position.getZ());
		glEnable(GL_TEXTURE_2D);
		BlockManager::getBlock(6).draw(mouse.getLeftCount());
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	//アイテムを描画
	world.drawItemBlocks();
	//単位ベクトルを描画
	glTranslated(0, 9, 0);
	glBegin(GL_LINES);
	glColor3d(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	glColor3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 1, 0);
	glColor3d(0, 0, 1);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 1);
	glColor3d(1, 1, 1);
	glEnd();
	//CVIPブロックを回転させながら描画
	glTranslated(0, 2, 0);
	static double yAngle = 0.0;
	yAngle += 1.0;
	glRotated(yAngle, 0, 1, 0);
	static double xAngle = 0.0;
	xAngle += 1.0;
	glRotated(xAngle, 1, 0, 0);
	glTranslated(-0.5, -0.5, -0.5);
	glEnable(GL_TEXTURE_2D);
	BlockManager::getBlock(5).draw();
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)w / (double)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void printComment(void)
{
	cout << "**********************************************" << endl;
	cout << "w, s, a, d : 前後左右に移動" << endl;
	cout << "Space      : ジャンプ" << endl;
	cout << "マウス移動 : 視点変更" << endl;
	cout << "左クリック : ブロック破壊" << endl;
	cout << "右クリック : ブロック設置" << endl;
	cout << "数字キー   : ブロック選択" << endl;
	cout << "p          : セーブ" << endl;
	cout << "**********************************************" << endl;
	cout << "上に表示されているパスにIrfanViewで出力したバイナリPPMファイルを置くと，テクスチャを差し替えることができます．" << endl;
}

void init(void)
{
	glClearColor((double)0x00 / 0xFF, (double)0xBF / 0xFF, (double)0xFF / 0xFF, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//マウスカーソルを画面の中央に移動させる
	glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	//テクスチャ・ディスプレイリストの登録
	TextureManager::init();
	BlockManager::init();
	player.getItemSlot().registerTexture();
	world.registerDisplayList();
	world.updateChunkVisible();
	//チャンクのディスプレイリストを更新
	world.waitForUpdateChunkVisible();
	world.updateDisplayList();
	//説明の表示
	printComment();
}

void key(unsigned char key, int x, int y)
{
	switch (key) {
	//Escキーで終了
	case '\033':
		exit(0);
	//セーブ
	case 'p':
		world.writeChunk();
		world.writeItemSlot();
		world.writePlayerPosition();
		cout << "セーブしました．" << endl;
		break;
	}
	keyboard.down(key);
}

void keyUp(unsigned char key, int x, int y)
{
	keyboard.up(key);
}

void mouseFunc(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		mouse.getLeft() = (state == GLUT_DOWN) ? true : false;
		break;
	case GLUT_RIGHT_BUTTON:
		mouse.getRight() = (state == GLUT_DOWN) ? true : false;
		break;
	}
}

void motion(int x, int y)
{
	mouse.setPosition(x, y);
}

static void timer(int dummy)
{
	glutTimerFunc(1, timer, 0);
	glutPostRedisplay();
	//プレイヤーの目線の変更
	mouse.updatePlayerDirection(player);
	//マウスの状態の更新
	mouse.update();
	//マウスカーソルを画面の中央に保つ
	glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	//プレイヤーの移動
	world.movePlayer();
	//左クリックの処理
	world.leftClickPlayer();
	//右クリックの処理
	world.rightClickPlayer();
	//キーボードの処理
	world.keyboardPlayer();
	//アイテムの処理
	world.updateItemBlocks();
	
	//定期的にチャンクのディスプレイリストを更新する
	static int worldUpdateCnt = 0;
	if (worldUpdateCnt++ >= 25) {
		worldUpdateCnt = 0;
		world.updateDisplayList();
	}
	
}

void idle(void)
{
	//チャンクのブロックの「見える/見えない」を更新する
	static int cnt = 0;
	if (++cnt >= 5) {
		cnt = 0;
		world.updateChunkVisible();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("*inecraft");
	//カーソルを非表示にする
	glutSetCursor(GLUT_CURSOR_NONE);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouseFunc);
	glutPassiveMotionFunc(motion);
	glutTimerFunc(16, timer, 0);
	glutIdleFunc(idle);
	init();
	glutMainLoop();

	return 0;
}
