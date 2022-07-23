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
int g_Score = 0; //�X�R�A
int g_WaitTime = 0; //�҂�����
int g_StartTime;   // �X�^�[�g����
int Time;   // ���ݎ���

int g_TakaraBako[4]; //�󔠂̉摜
int g_Arrow;  //�v���C���[�̖��̉摜
int g_Applec; //�^�C�g���J�[�\���ϐ��@�����Ȃ���

int g_StageImage;

//�X�e�b�N
int AX, AY;
struct DINPUT_JOYSTATE
{
    int		X;		// �X�e�B�b�N�̂w���p�����[�^( -1000�`1000 )
    int		Y;		// �X�e�B�b�N�̂x���p�����[�^( -1000�`1000 )
    int		Z;		// �X�e�B�b�N�̂y���p�����[�^( -1000�`1000 )
    int		Rx;		// �X�e�B�b�N�̂w����]�p�����[�^( -1000�`1000 )
    int		Ry;		// �X�e�B�b�N�̂x����]�p�����[�^( -1000�`1000 )
    int		Rz;		// �X�e�B�b�N�̂y����]�p�����[�^( -1000�`1000 )
    int		Slider[2];	// �X���C�_�[���
    unsigned int	POV[4];	// �n�b�g�X�C�b�`�S��
                    // ( 0xffffffff:���͂Ȃ� 0:�� 4500:�E�� 9000:�E 13500:�E��
                    //		 18000:�� 22500:���� 27000:�� 31500:���� )
    unsigned char	Buttons[32];	// �{�^���R�Q��( �����ꂽ�{�^���� 128 �ɂȂ� )
};


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

    if (LoadImages() == -1) return -1; //�摜�Ǎ��݊֐����Ăяo��

      //�Q�[�����[�v 
    while (ProcessMessage() == 0 && g_GameState != 99) {
        //�L�[���͎擾 
        g_OldKey = g_NowKey;
        g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1); //�Q�[���p�b�h�������Ă��Ă��Ȃ��l�p�Ɍ�ŃQ�[���p�b�h�݂̂ɕύX
        g_KeyFlg = g_NowKey & ~g_OldKey;

        //���E�̃A�i���O���͏�Ԃ��擾����
        GetJoypadAnalogInput(&AX, &AY, DX_INPUT_PAD1);

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
            //DrawHelp();
            break;
        case 4:
            //DrawEnd();
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
    //static int MenuNo = 0;

    ////���j���[�J�[�\���ړ�����
    //if (g_KeyFlg & PAD_INPUT_DOWN) {
    //    if (++MenuNo > 3)MenuNo = 0;
    //}
    //if (g_KeyFlg & PAD_INPUT_UP) {
    //    if (--MenuNo < 0)MenuNo = 3;
    //}
    ////z�L�[�AA�{�^���Ń��j���[�I��
    //if (g_KeyFlg & PAD_INPUT_A && MenuNo == 0)g_GameState = MenuNo + 1; //����ĂȂ�����X�^�[�g�ȊO�����܂���
    // //���j���[
    //DrawGraph(310, 220 + MenuNo * 50, g_Applec, TRUE);

    //�^�C�g���摜�\��
    DrawGraph(0, 0, g_TitleImage, FALSE);

    SetFontSize(20);
    DrawString(150, 200, "---Z�L�[��A�{�^���������ăQ�[���X�^�[�g---", 0xffffff, 0);
    if (g_KeyFlg & PAD_INPUT_A) {
        g_GameState = 1;
    }
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

    //�󔠂̉摜
    if (LoadDivGraph("images/TakaraBako.png", 4, 2, 2, 60, 60, g_TakaraBako) == -1) return -1;

    //�v���C���[���摜
    if ((g_Arrow = LoadGraph("images/Arrow.png")) == -1)return -1;

    return 0;
}