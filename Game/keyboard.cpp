#include"DxLib.h"
#include"keyboard.h"
//#include"ranking.h"

KeyBoard keyboard;

#define ALPHA_MAX 26      //�A���t�@�x�b�g����

#define OUT_WIDTH 45      //��ʍ��[�`�L�[�{�[�h�܂ł̕�
#define OUT_HEIGHT 200    //��ʏ�[�`�L�[�{�[�h�܂ł̍���
#define SPACE 10          //�L�[�Ԃ̃X�y�[�X
#define KEY_WIDTH 40      //�m�[�}���L�[�̕�

const int CurStdX = OUT_WIDTH + SPACE + 5;    //�J�[�\����X���W�    (�u5�v�̓J�[�\���̕� �j
const int CurStdY = OUT_HEIGHT + SPACE + 5;   //�J�[�\����Y���W�    

//���͕���    ���킩��₷�����邽�ߔ����i���ۂɕ\������Ă���L�[�{�[�h�Ɠ����z�u�j
const char AllStr[5][ALPHA_MAX / 2 + 1] = {
	"ABCDEFGHIJKLM",
	"NOPQRSTUVWXYZ",
	"abcdefghijklm",
	"nopqrstuvwxyz",
	"0123456789"
};

//��������
void KeyBoard::KeyBoardInit()
{
	//�ړ���
	movekeyX = 0;
	movekeyY = 0;

	//�J�[�\���̏����ʒu�́uA�v
	PUSH_NOW = KEY_TYPE::NORMAL;

	//���͕�����@������
	for (int i = 0; i < 10; i++)
	{
		InputName[i] = 0;
	}
	InputName[9] = '\0';

	namePos = -1;

	//�폜�E�m��t���O
	PushFlg = FALSE;
}

//�摜�ǂݍ���
int KeyBoard::LoadImgae()
{
	//�w�i
	if ((backimage = LoadGraph("images/KeyBoard/back02.png")) == -1) return -1;
	//�L�[�{�[�h
	if ((keyboardimage = LoadGraph("images/KeyBoard/OPEN_Board4.png")) == -1) return -1;

	//�����ǂݍ��݁@�����ꂽ/�����ĂȂ� ���A�������摜

	//�m�[�}���J�[�\��
	if ((LoadDivGraph("images/KeyBoard/N_Link_Normal.png", 2, 2, 1, 40, 40, Cursorimage)) == -1) return -1;

	//�u�~�v�J�[�\��
	if ((LoadDivGraph("images/KeyBoard/N_Link_Cancel.png", 2, 2, 1, 70, 40, Cancelimage)) == -1) return -1;

	//�uOK�v�J�[�\��
	if ((LoadDivGraph("images/KeyBoard/N_Link_Space.png", 2, 2, 1, 200, 40, OKimage)) == -1) return -1;
}

//�`��
void KeyBoard::DrawKeyBoard()
{
	//�w�i
	DrawGraph(0, 0, backimage, FALSE);
	//�L�[�{�[�h
	DrawGraph(45, OUT_HEIGHT, keyboardimage, TRUE);

	//test
	//DrawTestInfo();

	DrawInputInfo();

	// �m�[�}��(A�`Z,a�`z,0�`9)�E�u�~�v�E�u�n�j�v�ɂ���ĉ摜�ω��@ switch���ő���
	// �@�@�@�@�@�@�@�@�����E�����Ȃ��ɂ���ĉ摜�ω��@�@�@�@�@�@�@ �摜�z��� PushFlg �ő���
	switch (PUSH_NOW)
	{
	case KEY_TYPE::NORMAL:    //�m�[�}���J�[�\��
		DrawGraph(CurStdX + KEY_WIDTH * movekeyX, CurStdY + KEY_WIDTH * movekeyY, Cursorimage[PushFlg], TRUE);
		break;

	case KEY_TYPE::CANCEL:    //�u�~�v�J�[�\��
		DrawGraph(CurStdX + KEY_WIDTH * movekeyX + 20, CurStdY + KEY_WIDTH * movekeyY, Cancelimage[PushFlg], TRUE);
		break;

	case KEY_TYPE::DONE:      //�uOK�v�J�[�\��
		DrawGraph(CurStdX + KEY_WIDTH * movekeyX, CurStdY + KEY_WIDTH * movekeyY, OKimage[PushFlg], TRUE);
		break;

	default:
		break;
	}
}


//�ړ�
void KeyBoard::KeyBoardControl(int NowKey)
{

	//�t���[�����J�E���g
	frame++;

	//�� �E 
	if (NowKey & PAD_INPUT_RIGHT)
	{
		if (InputControl() == 0)
		{
			movekeyX++;     //�^�C�~���O���� + �ړ�
		}
		if (movekeyX >= 12) movekeyX = 12;   //�E�[�ŃX�g�b�v

		PUSH_NOW = KEY_TYPE::NORMAL;         //���݂̃L�[�̓m�[�}��
	}

	//�� ��
	if (NowKey & PAD_INPUT_LEFT)
	{
		if (InputControl() == 0)
		{
			movekeyX--;     //�^�C�~���O���� + �ړ�
		}

		if (movekeyX <= 0) movekeyX = 0;     //���[�ŃX�g�b�v

		PUSH_NOW = KEY_TYPE::NORMAL;         //���݂̃L�[�̓m�[�}��
	}

	//�� ��
	if (NowKey & PAD_INPUT_UP)
	{

		if (InputControl() == 0)
		{
			movekeyY--;     //�^�C�~���O���� + �ړ�
		}
		if (movekeyY <= 0) movekeyY = 0;     //��[�ŃX�g�b�v

		PUSH_NOW = KEY_TYPE::NORMAL;         //���݂̃L�[�̓m�[�}��
	}

	//�� ��
	if (NowKey & PAD_INPUT_DOWN)
	{

		if (InputControl() == 0)
		{
			movekeyY++;     //�^�C�~���O���� + �ړ�
		}

		PUSH_NOW = KEY_TYPE::NORMAL;         //���݂̃L�[�̓m�[�}��
	}

	//�u�~�v�{�^��   �ua�`z�v�i��艺 ���� �u9�v�L�[���E��
	if (movekeyY == 4 && movekeyX >= 10)
	{
		movekeyX = 10;                       //�{�^���̈ʒu

		PUSH_NOW = KEY_TYPE::CANCEL;         //���݂̃L�[�̓L�����Z���u�~�v
	}

	//�uOK�v�{�^��   �L�[�{�[�h�ŉ��i��艺
	if (movekeyY >= 5)
	{
		movekeyX = 4;                        //�{�^���̈ʒu
		movekeyY = 5;

		PUSH_NOW = KEY_TYPE::DONE;           //���݂̃L�[��DONE�uOK�v
	}

	//B�{�^���Ŋm��
	//testPush_B_Key2(NowKey);
}

//B�{�^���������ꂽ���̏��� �e�X�g2
void KeyBoard::Push_B_Key(int NowKey, int* GameState, int Score)
{
	//"�������u��"�𔻒�
	static int push_B;

	if (NowKey & PAD_INPUT_B)
	{
		push_B++;

		if (PUSH_NOW == KEY_TYPE::NORMAL)
		{
			//"�������u��"�̂ݓ���
			if (push_B >= 1 && push_B < 8)
			{
				//B�����ꂽ��t���O�@TRUE
				PushFlg = TRUE;

				//���̓^�C�~���O
				if (InputControl() == 0)
				{
					++namePos;
					//�����9����
					if (namePos >= 8) namePos = 8;

					//�z��ɓ���
					InputName[namePos] = AllStr[movekeyY][movekeyX];
				}
			}
		}
		else if (PUSH_NOW == KEY_TYPE::CANCEL) //�u�~�v�L�[��ŉ����Ĉꕶ���폜
		{
			//B�����ꂽ��t���O�@TRUE
			PushFlg = TRUE;

			//���̓^�C�~���O
			if (InputControl() == 0)
			{
				InputName[namePos] = 0;

				//�O�����ȉ��ɂ͂Ȃ�Ȃ�
				if (namePos >= 0) namePos--;
			}
		}
		else if (PUSH_NOW == KEY_TYPE::DONE)   //�uOK�v�L�[��ŉ����Ċm��i�����L���O�ϐ��ɑ���j
		{
			//B�����ꂽ��t���O�@TRUE
			PushFlg = TRUE;

			//���̓^�C�~���O
			if (InputControl() == 0)
			{
				//�����͂ł̊m��͕s��
				if (InputName[0] != '\0')
				{
					InputName[namePos + 1] = '\0';

					//�����L���Oname�ϐ�=InputName;
					//ranking.SetRankingName(InputName);
					//ranking.EndRanking(GameState, Score);
				}
			}
		}
	}
	else
	{
		if (push_B > 0) push_B = -1;
		else push_B = 0;

		PushFlg = FALSE;
	}
}

//���͏��\��
void KeyBoard::DrawInputInfo()
{
	//if (InputName[0] == 0) InputName[0] = '|';

	if (InputName[0] == 0)
	{
		//SetDrawBlendMode
		SetFontSize(20);
		DrawString(200, 125, "�E �E ���O����� �E �E", 0xffffff);
	}

	for (int i = 0; InputName[i] != '\0'; i++)
	{
		SetFontSize(30);

		DrawFormatString(220 + 20 * i, 120, 0xffffff, " %c", InputName[i]);
	}
}

//�R���g���[���[����
int KeyBoard::InputControl()
{
	int timing = 8;
	return frame % timing;
}


//��test�p���@�e����\��
void KeyBoard::DrawTestInfo()
{
	//InputName�`�F�b�N
	//if (InputName[0] == 0) InputName[0] = '|';
	for (int i = 0; InputName[i] != '\0'; i++)
	{
		SetFontSize(20);
		DrawString(0, 0, "NAME : ", 0xffffff);
		DrawFormatString(80 + 15 * i, 0, 0xffffff, "%c", InputName[i]);
	}


	//PushFlg�`�F�b�N
	DrawFormatString(0, 30, 0xffffff, "Push : %s", PushFlg == FALSE ? "������ĂȂ�" : "������Ă�");
}
