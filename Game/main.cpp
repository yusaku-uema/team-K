/********************************************************************
**   �Q�[���W����
** �@�󔠂��A�P���Q�[��
********************************************************************/
#include"DxLib.h"
#include<math.h>
/***********************************************
 * �萔��錾
 ***********************************************/
 //��ʗ̈�̑傫��
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//��������
const int TIMELIMIT = 30000;
//�󔠂̌�
const int ENEMY_MAX = 6;
const int TAKARA_TIME = 100;

/***********************************************
 * �ϐ��̐錾
 ***********************************************/
int g_OldKey;  // �O��̓��̓L�[ 
int g_NowKey;  // ����̓��̓L�[ 
int g_KeyFlg;  // ���̓L�[��� 

int g_GameState = 0;  // �Q�[�����[�h 
int g_TitleImage; // �摜�p�ϐ� 
int g_Menu; //���j���[���
int	g_MenuNumber = 0;		// ���j���[�J�[�\���ʒu
int g_MenuY;				// ���j���[�J�[�\���̂x���W

int g_Score = 0; //�X�R�A

int g_WaitTime = 0; //�҂�����
int g_StartTime;   // �X�^�[�g����
int Time;   // ���ݎ���

int g_TakaraBako[4]; //�󔠂̉摜
int g_Arrow;  //�v���C���[�̖��̉摜
int g_Applec; //�^�C�g���J�[�\���ϐ��@�����Ȃ���

int g_StageImage;   //�Q�[�����C���w�i
int g_HelpImage; //�w���v�w�i
int g_EndImage;//�G���h�w�i

int g_PosY; //���v�{���񂪎g���܂�
int Font3, Font4, Font5;//�w���v��ʂƃG���h��ʂ̃t�H���g�ύX
//�v���C���[���̍\����
struct ARROW
{
    int x, y;      //���Wx,y
    int no;
    int oldkey;
};
//�v���C���[
struct ARROW g_arrow;


//�󔠂̍\����
struct TAKARA {
    int flg; //�g�p�t���O
    int type; //�^�C�v
    int img; //�摜
    int x, y; //���Wx,y,��w,����h
    int point; //��̓��_
    int time;
};
//�G�@
struct TAKARA g_takara[ENEMY_MAX];
struct TAKARA g_enemy00 = { TRUE,0,0,30,200,0, TAKARA_TIME };

/***********************************************
 * �֐��̃v���g�^�C�v�錾
 ***********************************************/
void GameInit(void); //�Q�[����������
void GameMain(void); //�Q�[�����C������
void DrawEnd(void); //�Q�[���G���h
void DrawHelp(void); //�w���v���
void DrawGameTitle(void); //�^�C�g���`�揈��
void DrawGameOver(void); //�Q�[���I�[�o
int LoadImages(); //�摜�ǂݍ���
void UIView();
void TimeCount();
void BackScrool(); //�w�i�摜�X�N���[������
void PlayerControl(); //�v���C���[�̏���
void TakaraControl(); //�󔠂̏���

/***********************************************
 * �v���O�����̊J�n
 **********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    //�^�C�g����ݒ�
    SetMainWindowText("�󔠂�������");
    //�E�B���h�E���[�h�ŋN������ 
    ChangeWindowMode(TRUE);

    // DX ���C�u�����̏���������:�G���[���N�����璼���ɏI�� 
    if (DxLib_Init() == -1) return -1;

    //�`����ʂ𗠂ɂ��� 
    SetDrawScreen(DX_SCREEN_BACK);

    //�t�H���g�̌����ڑւ���悤
    Font3 = CreateFontToHandle("���C���I", 50, 9, DX_FONTTYPE_ANTIALIASING_EDGE);//"���C���I"  ��30pt,����3�̃t�H���g���쐬
    Font4 = CreateFontToHandle("���C���I", 30, 9, DX_FONTTYPE_ANTIALIASING_EDGE);//"���C���I"  ��30pt,����3�̃t�H���g���쐬
    Font5 = CreateFontToHandle("���C���I", 20, 9, DX_FONTTYPE_ANTIALIASING_EDGE);//"���C���I"  ��30pt,����3�̃t�H���g���쐬
    if (LoadImages() == -1) return -1; //�摜�Ǎ��݊֐����Ăяo��

      //�Q�[�����[�v 
    while (ProcessMessage() == 0 && g_GameState != 99) {
        //�L�[���͎擾 
        g_OldKey = g_NowKey;
        g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1); //�Q�[���p�b�h�������Ă��Ă��Ȃ��l�p�Ɍ�ŃQ�[���p�b�h�݂̂ɕύX
        g_KeyFlg = g_NowKey & ~g_OldKey;

        // ��ʂ̏����� 
        ClearDrawScreen();

        switch (g_GameState) {
        case 0:
            DrawGameTitle(); //�Q�[���^�C�g���`�揈��
            break;
        case 1:
            GameInit();
            break;
        case 2:
            //DrawRanking();
            break;
        case 3:
            DrawHelp();
            break;
        case 4:
            DrawEnd();
            break;
        case 5:
            GameMain();
            break;
        case 6:
            DrawGameOver();
            break;
        case 7:
            //InputRanking();
            break;
        }

        //����ʂ̓��e��\��ʂɔ��f���܂� 
        ScreenFlip();
    }
    //DX ���C�u�����g�p�̏I������ 
    DxLib_End();

    //�\�t�g�̏I�� 
    return 0;
}

/***********************************************
 * �Q�[���^�C�g���\��
 ***********************************************/
void DrawGameTitle(void) {
  
    //���j���[�J�[�\���ړ�����
    if (g_KeyFlg & PAD_INPUT_DOWN) {
        if (++g_MenuNumber > 3) g_MenuNumber = 0;
    }
    if (g_KeyFlg & PAD_INPUT_UP) {
        if (--g_MenuNumber < 0) g_MenuNumber = 2;
    }

    //�y�L�[�Ń��j���[�I��
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = g_MenuNumber + 1;

    //�^�C�g���摜�\��
    DrawGraph(0, 0, g_TitleImage, FALSE);

    //���j���[�J�[�\���i�O�p�`�j�̕\��
    g_MenuY = g_MenuNumber * 52;
    DrawTriangle(240, 255 + g_MenuY, 260, 270 + g_MenuY, 240, 285 + g_MenuY, GetColor(255, 0, 0), TRUE);

    SetFontSize(40);
    //�Q�[���^�C�g�����ڂ���\��
    DrawString(100, 0, "�Q�[���^�C�g������㔽�f", 0xffffff, 0);

    //�X�e�[�W�I��
    SetFontSize(30);
    DrawString(265, 255, "�Q�[���X�^�[�g", 0xffffff, 0);
    DrawString(265, 307, "�����L���O", 0xffffff, 0);
    DrawString(265, 359, "�w���v", 0xffffff, 0);
    DrawString(265, 411, "�G���h", 0xffffff, 0);

}

/***********************************************
 * �Q�[����������
 ***********************************************/
void GameInit(void)
{
    //�X�R�A�̏�������6
    g_Score = 0;

    //�G�l�~�[�̏�������
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_takara[i] = g_enemy00;
        g_takara[i].point = GetRand(5) * 1000;
    }

    //�v���C���[���̏�������
    g_arrow.y = g_takara[g_arrow.no].y - 100;
    g_arrow.x = g_takara[g_arrow.no].x;
    g_arrow.no = 0;

    //���݂̌o�ߎ��Ԃ𓾂�
    g_StartTime = GetNowCount();

    //�Q�[�����C��������
    g_GameState = 5;
}

/***********************************************
 * �Q�[�����C��
 ***********************************************/
void GameMain(void)
{
    BackScrool();
    PlayerControl();
    TakaraControl();
    UIView();
    TimeCount();

    //�X�y�[�X�L�[�Ń��j���[�ɖ߂�@�Q�[�����C������^�C�g���ɖ߂�ǉ�
    if (g_KeyFlg & PAD_INPUT_M)g_GameState = 6;
    SetFontSize(16);
    DrawString(150, 450, "---�X�y�[�X�L�[�������ăQ�[���I�[�o�[��---", 0xffffff, 0);
}

/***********************************************
 *�Q�[���I�[�o�[��ʕ`�揈��
 ***********************************************/
void DrawGameOver(void)
{
    BackScrool();

    //�X�y�[�X�L�[�Ń��j���[�ɖ߂�
    if (g_KeyFlg & PAD_INPUT_M)   g_GameState = 0;

    DrawBox(150, 150, 490, 330, 0x009900, TRUE);
    DrawBox(150, 150, 490, 330, 0x000000, FALSE);

    SetFontSize(20);
    DrawString(220, 170, "�Q�[���I�[�o�[", 0xcc0000);
    SetFontSize(35);

    DrawFormatString(180, 250, 0xFFFFFF, "�X�R�A = %6d", g_Score);

    SetFontSize(20);
    DrawString(150, 450, "---�X�y�[�X�L�[�������ă^�C�g���֖߂� ---", 0xffffff, 0);
}



/***********************************************
 * �w�i�摜�X�N���[������
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void BackScrool()
{
    DrawGraph(0, 0, g_StageImage, FALSE);
}

/***********************************************
 * �v���C���[�̏���
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void PlayerControl()
{


    if (g_KeyFlg & PAD_INPUT_LEFT)
    {
        g_arrow.no--;
        if (g_arrow.no < 0) g_arrow.no = 5;
    }
    else if (g_KeyFlg & PAD_INPUT_RIGHT)
    {
        g_arrow.no++;
        if (g_arrow.no > 5) g_arrow.no = 0;
    }

    else if (g_KeyFlg & PAD_INPUT_A)
    {
        if (g_takara[g_arrow.no].flg == TRUE)
        {
            g_Score += g_takara[g_arrow.no].point;
            g_takara[g_arrow.no].flg = FALSE;
        }
    }

    g_arrow.x = g_takara[g_arrow.no].x;

    DrawGraph(g_arrow.x, g_arrow.y, g_Arrow, TRUE);
}

/***********************************************
 * �󔠂̐���
 * ��  ��:�Ȃ�
 * �߂�l:TRUE�����AFALSE���s
 ***********************************************/
void TakaraControl()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_takara[i].x = i * 70 + 30;
        DrawGraph(g_takara[i].x, g_takara[i].y, g_TakaraBako[g_takara[i].img], TRUE); //�G�̕\��


        if (g_takara[i].flg == FALSE)
        {
            if (g_takara[i].point <= 0) g_takara[i].img = 2;
            else if (g_takara[i].point > 0)g_takara[i].img = 3;
            g_takara[i].time--;
            DrawFormatString(g_takara[i].x, g_takara[i].y - 60, 0x00ffff, "%d�_", g_takara[i].point);

            if (g_takara[i].time <= 0)
            {
                g_takara[i].flg = TRUE;
                g_takara[i].point = GetRand(5) * 1000;
                g_takara[i].time = TAKARA_TIME;
                g_takara[i].img = 0;
            }
        }
    }
}

void TimeCount(void)
{
    //�������Ԃ��߂�����Q�[���I�[�o�[
    Time = TIMELIMIT - (GetNowCount() - g_StartTime);
    if (Time <= 0)
    {
        g_GameState = 6;
    }
    SetFontSize(50);
    DrawFormatString(570, 100, 0xffffff, "%2d", Time / 1000);
}

void UIView(void)
{
    //UI�uTIME�v�\��
    SetFontSize(50);
    DrawString(520, 40, "TIME", 0xffffff, 0);

    //UI�uSCORE�v�\��
    SetFontSize(45);
    DrawString(510, 320, "SCORE", 0xffffff, 0);
    DrawFormatString(510, 360, 0x00ffff, "%d", g_Score);
}

/***********************************************
 * �Q�[���G���h�`�揈��
 ***********************************************/
void DrawEnd(void)
{

    //�G���h�摜�\��
    DrawGraph(0, 0, g_EndImage, FALSE);

    //�G���f�B���O�\��
    if (++g_WaitTime < 10000) g_PosY = 400 - g_WaitTime / 2;

    //�傫���������o��
    DrawStringToHandle(200, 100 + g_PosY, "�^�C�g��", GetColor(255, 255, 255), Font3);

    DrawStringToHandle(250, 200 + g_PosY, "�����", GetColor(255, 255, 255), Font4);

    DrawStringToHandle(150, 260 + g_PosY, "       ��ԁ@�Z�Z�Z����", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 290 + g_PosY, "       �_���@�Z�Z�Z����", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 320 + g_PosY, "       ���v�{�@�Z�Z�Z����", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 350 + g_PosY, "       ��[�@�Z�Z�Z����", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 380 + g_PosY, "       �����@�Z�Z�Z����", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 410 + g_PosY, "       ���Ñ��@�Z�Z�Z����", GetColor(255, 255, 255), Font5);

    //�Q�p�[�g
    DrawStringToHandle(225, 550 + g_PosY, "�f�ޗ��p", GetColor(255, 255, 255), Font4);
    DrawStringToHandle(205, 600 + g_PosY, "BGM�@ �Z�Z�Z", GetColor(255, 255, 255), Font5);
    //DrawStringToHandle(150, 650 + g_PosY, "   �@�@�@ �@     ", GetColor(255, 255, 255), Font01);
    DrawStringToHandle(205, 700 + g_PosY, "SE    �Z�Z�Z", GetColor(255, 255, 255), Font5);
    //DrawStringToHandle(150, 750 + g_PosY, "    �@�@�@�@     ", GetColor(255, 255, 255), Font01);

    //�^�C���̉��Z�������I��
    if (++g_WaitTime > 2500)g_GameState = 99;
}

/***********************************************
 * �Q�[���w���v�`�揈��
 ***********************************************/
void DrawHelp(void)
{
    //�X�y�[�X�L�[�Ń��j���[�ɖ߂�
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;

	////�^�C�g����ʕ\��
	DrawGraph(0, 0, g_HelpImage, FALSE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);        //�u�����h���[�h����(128/255)�ɐݒ�
	DrawBox(50, 25, 590, 500, GetColor(255, 255, 255), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(200, 50, "�w���v���", GetColor(255, 255, 255), Font3);

	//�傫���������o��
	DrawStringToHandle(150, 150, "�v���C���[����ɂ���", GetColor(255, 255, 255), Font4);
	DrawStringToHandle(300, 250, "��", GetColor(255, 255, 255), Font4);

	//���������o��
	DrawStringToHandle(250, 200, "�\���L�[�ňړ�", GetColor(255, 255, 255), Font5);
	DrawStringToHandle(100, 300, "�J�[�\���ړ��@�\���L�[�Ŏ�肽���󔠂�I��", GetColor(255, 255, 255), Font5);
	DrawStringToHandle(225, 350, "�󔠂̌���@B�{�^��", GetColor(255, 255, 255), Font5);

	//�����̕\���i�_�Łj
	if (++g_WaitTime < 30) {

		DrawStringToHandle(150, 425, "---B�{�^���ŃQ�[�����C���Ɉړ�---", GetColor(255, 255, 255), Font5);
	}
	else if (g_WaitTime > 60) {
		g_WaitTime = 0;
	}

  
}

/***********************************************
 * �摜�ǂݍ���
 ***********************************************/
int LoadImages()
{
    //�^�C�g�� �^�C�g���摜�ւ��܂����B
    if ((g_TitleImage = LoadGraph("images/Title.png")) == -1) return -1;

    //���j���[
    if ((g_Applec = LoadGraph("images/Applec.png")) == -1) return -1;

    //�X�e�[�W�w�i
    if ((g_StageImage = LoadGraph("images/haikei.png")) == -1)return -1;

    //�w���v��ʔw�i
    if ((g_HelpImage = LoadGraph("images/Help.png")) == -1)return -1;

    //�G���h��ʔw�i
    if ((g_EndImage = LoadGraph("images/EndImage.png")) == -1)return -1;
    //�󔠂̉摜
    if (LoadDivGraph("images/TakaraBako.png", 4, 2, 2, 60, 60, g_TakaraBako) == -1) return -1;

    //�v���C���[���摜
    if ((g_Arrow = LoadGraph("images/Arrow.png")) == -1)return -1;

    return 0;
}