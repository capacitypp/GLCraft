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

#include <gl/glut.h>

using namespace std;

Player player;
FPS fps;
Keyboard keyboard;
Mouse mouse;
World world(player, fps, keyboard, mouse);

void display(void)
{
	//FPS�̍X�V
	fps.update();

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//��ʂ̒��S�ɏ\����\������
	glBegin(GL_LINES);
	glVertex3d(-0.0025, 0, -0.1);
	glVertex3d(0.0025, 0, -0.1);
	glVertex3d(0, -0.0025, -0.1);
	glVertex3d(0, 0.0025, -0.1);
	glEnd();
	//�A�C�e���X���b�g�̕`��
	player.drawItemSlot();
	//FPS�̕`��
	StringManager::draw(0.66, 0.54, fps.get());

	//�v���C���[�̖ڐ��ɍ��킹��
	const Vector& direction = player.getDirection();
	gluLookAt(0, 0, 0, direction.getX(), direction.getY(), direction.getZ(), 0, 1, 0);
	const Vector& playerPosition = player.getPosition();
	glTranslated(-playerPosition.getX(), -playerPosition.getY(), -playerPosition.getZ());
	const Vector& eyePosition = player.getEyePosition();
	glTranslated(-eyePosition.getX(), -eyePosition.getY(), -eyePosition.getZ());

	//�`�����N��`��
	world.draw();
	//�u���b�N�̔j�󒆂Ȃ�C�j��G�t�F�N�g��`��
	if (world.isLastHitReach()) {
		Vector position = world.getLastHitBlockPosition();
		glPushMatrix();
		glTranslated(position.getX(), position.getY(), position.getZ());
		glEnable(GL_TEXTURE_2D);
		BlockManager::getBlock(6).draw(mouse.getLeftCount());
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	//�A�C�e����`��
	world.drawItemBlocks();
	//�P�ʃx�N�g����`��
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
	//CVIP�u���b�N����]�����Ȃ���`��
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
	cout << "w, s, a, d : �O�㍶�E�Ɉړ�" << endl;
	cout << "Space      : �W�����v" << endl;
	cout << "�}�E�X�ړ� : ���_�ύX" << endl;
	cout << "���N���b�N : �u���b�N�j��" << endl;
	cout << "�E�N���b�N : �u���b�N�ݒu" << endl;
	cout << "�����L�[   : �u���b�N�I��" << endl;
	cout << "p          : �Z�[�u" << endl;
	cout << "**********************************************" << endl;
	cout << "��ɕ\������Ă���p�X��IrfanView�ŏo�͂����o�C�i��PPM�t�@�C����u���ƁC�e�N�X�`���������ւ��邱�Ƃ��ł��܂��D" << endl;
}

void init(void)
{
	glClearColor((double)0x00 / 0xFF, (double)0xBF / 0xFF, (double)0xFF / 0xFF, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//�}�E�X�J�[�\������ʂ̒����Ɉړ�������
	glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	//�e�N�X�`���E�f�B�X�v���C���X�g�̓o�^
	TextureManager::init();
	BlockManager::init();
	player.getItemSlot().registerTexture();
	world.registerDisplayList();
	world.updateChunkVisible();
	//�`�����N�̃f�B�X�v���C���X�g���X�V
	world.waitForUpdateChunkVisible();
	world.updateDisplayList();
	//�����̕\��
	printComment();
}

void key(unsigned char key, int x, int y)
{
	switch (key) {
	//Esc�L�[�ŏI��
	case '\033':
		exit(0);
	//�Z�[�u
	case 'p':
		world.writeChunk();
		world.writeItemSlot();
		world.writePlayerPosition();
		cout << "�Z�[�u���܂����D" << endl;
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
	//�v���C���[�̖ڐ��̕ύX
	mouse.updatePlayerDirection(player);
	//�}�E�X�̏�Ԃ̍X�V
	mouse.update();
	//�}�E�X�J�[�\������ʂ̒����ɕۂ�
	glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	//�v���C���[�̈ړ�
	world.movePlayer();
	//���N���b�N�̏���
	world.leftClickPlayer();
	//�E�N���b�N�̏���
	world.rightClickPlayer();
	//�L�[�{�[�h�̏���
	world.keyboardPlayer();
	//�A�C�e���̏���
	world.updateItemBlocks();
	
	//����I�Ƀ`�����N�̃f�B�X�v���C���X�g���X�V����
	static int worldUpdateCnt = 0;
	if (worldUpdateCnt++ >= 25) {
		worldUpdateCnt = 0;
		world.updateDisplayList();
	}
	
}

void idle(void)
{
	//�`�����N�̃u���b�N�́u������/�����Ȃ��v���X�V����
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
	//�J�[�\�����\���ɂ���
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
