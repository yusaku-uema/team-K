#pragma once

//�L�[�̎��
enum class KEY_TYPE
{
	NONE = 0,   // ����0�i�Ȃɂ�������Ă��Ȃ��j
	NORMAL,     // A�`B,a�`b,1�`9
	CANCEL,     //�u�~�v(�ЂƂ���)
	DONE,       //�uOK�v(�m��)

	MISS = 99
};

class KeyBoard
{
public:
	//��������
	void KeyBoardInit();

	//�摜�ǂݍ���
	int LoadImgae();

	//���ʉ��ǂݍ���
	int LoadSounds();

	//�`��
	void DrawKeyBoard();

	//�ړ�
	void KeyBoardControl(int NowKey);

	//B�{�^���������ꂽ���̏���
	void Push_B_Key(int NowKey, int* GameState, int Score);

	//���͏��\��
	void DrawInputInfo();

	//�R���g���[���[����
	int InputControl();

	//��test�p���@�e����\��
	void DrawTestInfo();

private:
	/*--------------------�@UI�@--------------------*/
		//�w�i�摜
	int backimage = 0;
	//�L�[�{�[�h�摜
	int keyboardimage = 0;

	//�m�[�}���J�[�\���摜  0 : �ʏ펞�@�@1 : �����ꂽ�Ƃ�
	int Cursorimage[2] = { 0 };

	//�u�~�v�J�[�\���摜
	int Cancelimage[2] = { 0 };

	//�uOK�v�J�[�\���摜
	int OKimage[2] = { 0 };

	//SE
	int SE_push = 0;   //�ʏ����
	int SE_cancel = 0; //�u�~�v
	int SE_ok = 0;     //�uok�v

	//�ړ���
	int movekeyX = 0;
	int movekeyY = 0;

	//�L�[�̏�Ԃ�ێ�
	KEY_TYPE PUSH_NOW;
	/*------------------------------------------------*/

	/*--------------------�@���@----------------------*/
		//�t���[�����J�E���g
	int frame = 0;

	//���͏��
	char InputName[10];

	//InputName�p
	int namePos;

	//B���@������Ă���/������ĂȂ� �t���O    TRUE:������Ă���@FALSE:������Ă��Ȃ�
	bool PushFlg;

	/*------------------------------------------------*/
};

extern KeyBoard keyboard;



