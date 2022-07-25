/********************************************************************
**   �Q�[���W����
** �@�󔠂��A�P���Q�[��
********************************************************************/
#include"DxLib.h"
#include<math.h>
#include"keyboard.h"

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

//�v���C���[�̂��
const int PLAYER_SPEED = 3;
const int PLAYER_IMAGE_TIME = 8;

//�w�i�̂��
const int STAGE_NO = 2; //�w�i�̘L���̒���

/***********************************************
 * �ϐ��̐錾
 ***********************************************/
int g_OldKey;  // �O��̓��̓L�[ 
int g_NowKey;  // ����̓��̓L�[ 
int g_KeyFlg;  // ���̓L�[��� 

int g_GameState = 0;  // �Q�[�����[�h 

int g_GameMode = 0; //�Q�[�����[�h�i�_����������j

int g_TitleImage; // �摜�p�ϐ� 
int g_Menu; //���j���[���
int	g_MenuNumber = 0;		// ���j���[�J�[�\���ʒu
int g_MenuY;				// ���j���[�J�[�\���̂x���W
 
int g_Score = 0; //�X�R�A

int g_WaitTime = 0; //�҂�����
int g_StartTime;   // �X�^�[�g����
int Time;   // ���ݎ���

int g_TakaraBako[4]; //�󔠂̉摜
int g_Player[16];
int g_Arrow;  //�v���C���[�̖��̉摜
int g_Applec; //�^�C�g���J�[�\���ϐ��@�����Ȃ���

int g_StageImage;   //�Q�[�����C���w�i
int g_HelpImage; //�w���v�w�i
int g_EndImage;//�G���h�w�i
int g_RoadImage;
int g_RoadImage2;
int g_PosY; //���v�{���񂪎g���܂�
int Font,Font1,Font3, Font4, Font5;//�w���v��ʂƃG���h��ʂ̃t�H���g�ύX
//�v���C���[���̍\����
struct ARROW
{
    int x, y;      //���Wx,y
    int no;
    int oldkey;
};
//�v���C���[
struct ARROW g_arrow;

//�v���C���[�̍\����
struct PLAYER
{
    int x, y;      //���Wx,y
    int img;
    int imgtime;
};
//�v���C���[
struct PLAYER g_player;

//�L���̔w�i�摜�̍\���́i�_�����g���j
struct STAGE
{
    int no;
    int img;
    int y;
    int x;
};
struct STAGE g_stage;


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
void BackScrool(int a); //�w�i�摜�X�N���[������
void BackImage();
void PlayerControl(); //�v���C���[�̏���
void ArrowControl();  //�v���C���[�̖��̏���
void TakaraControl(); //�󔠂̏���

void InputRanking();  //�����L���O����
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
    Font =  CreateFontToHandle("���C���I", 30, 9, DX_FONTTYPE_ANTIALIASING_EDGE);
    Font1 = CreateFontToHandle("���C���I", 50, 9, DX_FONTTYPE_ANTIALIASING_EDGE);
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
            InputRanking();
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
        if (--g_MenuNumber < 0) g_MenuNumber = 3;
    }

    //�y�L�[�Ń��j���[�I��
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = g_MenuNumber + 1;

    //�^�C�g���摜�\��
    DrawGraph(0, 0, g_TitleImage, FALSE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(160, 255, 480, 450, GetColor(255,255,255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    g_MenuY = g_MenuNumber * 52;
    //���j���[�J�[�\���i�O�p�`�j�̕\��
    if (g_MenuNumber == 0 || g_MenuNumber == 1) {
        DrawTriangle(180, 255 + g_MenuY, 200, 270 + g_MenuY, 180, 285 + g_MenuY, GetColor(0, 0, 225), TRUE);
    }
    if (g_MenuNumber == 2 || g_MenuNumber == 3) {
        DrawTriangle(240, 255 + g_MenuY, 260, 270 + g_MenuY, 240, 285 + g_MenuY, GetColor(0, 0, 225), TRUE);
    }
    DrawBox(160, 255, 480, 450, GetColor(0, 0, 0), FALSE);
    SetFontSize(40);
    //�Q�[���^�C�g�����ڂ���\��
    DrawStringToHandle(100, 130, "�Q�[���^�C�g���\��",GetColor(255, 255, 255), Font1);
   
    DrawStringToHandle(210, 255, "�Q�[���X�^�[�g",GetColor(255, 0, 0), Font);
    DrawStringToHandle(210, 305, "�����L���O", GetColor(255, 0, 0), Font);
    DrawStringToHandle(275, 355, "�w���v", GetColor(255, 0, 0), Font);
    DrawStringToHandle(275, 405, "�G���h", GetColor(255, 0, 0), Font);
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

    //�v���C���[��������
    g_player = { 139, 200, 13, PLAYER_IMAGE_TIME };

    //�w�i�摜(�L���j�̏�������
    g_stage = { STAGE_NO, g_RoadImage, 0, 0 };

    g_GameMode = 0;

    //���݂̌o�ߎ��Ԃ𓾂�
    g_StartTime = GetNowCount();

    //�L�[�{�[�h��������
    keyboard.KeyBoardInit();

    //�Q�[�����C��������
    g_GameState = 5;
}

/***********************************************
 * �Q�[�����C��
 ***********************************************/
void GameMain(void)
{
    switch (g_GameMode) {
    case 0:
       /* BackScrool(0);
        PlayerControl();
        break;*/
    case 1:
        BackImage();
        TakaraControl();
        ArrowControl();
        break;
    }
    
    /*UIView();
    TimeCount();*/

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
    //BackScrool();

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
void BackScrool(int a)
{
    DrawGraph(g_stage.x, g_stage.y, g_stage.img, FALSE);
    if (g_stage.no > 0)DrawGraph(g_stage.x, g_stage.y - 480, g_stage.img, FALSE);
    else if (g_stage.no <= 0) DrawGraph(g_stage.x, g_stage.y - 480, g_RoadImage2, FALSE);

    g_stage.y += (PLAYER_SPEED * a);

    if (g_stage.y >= 480)
    {
        g_stage.y = 0;
        g_stage.no--;
        if (g_stage.no < 0)g_stage.img = g_RoadImage2;
    }
}

void BackImage()
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
    if (g_player.imgtime <= 0)g_player.imgtime = PLAYER_IMAGE_TIME;
    g_player.imgtime--;

    if (g_NowKey & PAD_INPUT_UP)
    {
        if (g_NowKey != g_OldKey)g_player.img = 12;
        if (g_player.imgtime <= 0)
        {
            g_player.img++;
            if (g_player.img > 15)g_player.img = 12;
        }
        if (g_player.y > 200 || g_stage.no < 0)g_player.y -= PLAYER_SPEED;
        if (g_player.y <= 200 && g_stage.no >= 0)
        {
            g_player.y = 200;
            BackScrool(1);
        }
    }
    else if (g_NowKey & PAD_INPUT_DOWN)
    {
        if (g_NowKey != g_OldKey)g_player.img = 0;
        if (g_player.imgtime <= 0)
        {
            g_player.img++;
            if (g_player.img > 3)g_player.img = 0;
        }

        g_player.y += PLAYER_SPEED;
    }
    else if (g_NowKey & PAD_INPUT_LEFT)
    {
        if (g_NowKey != g_OldKey) g_player.img = 8;
        if (g_player.imgtime <= 0)
        {
            g_player.img++;
            if (g_player.img > 11)g_player.img = 8;
        }
        g_player.x -= PLAYER_SPEED;
    }
    else if (g_NowKey & PAD_INPUT_RIGHT)
    {
        if (g_NowKey != g_OldKey) g_player.img = 4;
        if (g_player.imgtime <= 0)
        {
            g_player.img++;
            if (g_player.img > 7)g_player.img = 4;
        }
        g_player.x += PLAYER_SPEED;
    }

    if (g_player.x > 280)g_player.x = 280;
    if (g_player.x < 0)g_player.x = 0;
    if (g_player.y > 400)g_player.y = 400;
    if (g_player.y < 59 && g_player.x >= 130 && g_player.x <= 150)g_GameMode = 1;
    if (g_player.y < 60)g_player.y = 60;

    DrawFormatString(0, 0, 0x111FFF, "�L�����摜 = %d", g_player.img);
    DrawFormatString(0, 20, 0x111FFF, "X = %d", g_player.x);
    DrawFormatString(0, 40, 0x111FFF, "Y = %d", g_player.y);
    DrawGraph(g_player.x, g_player.y, g_Player[g_player.img], TRUE);
}


/***********************************************
 * ���̏���
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void ArrowControl()
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

void InputRanking()
{
    keyboard.DrawKeyBoard();
    keyboard.KeyBoardControl(g_NowKey);
    keyboard.Push_B_Key(g_NowKey, &g_GameState, g_Score);
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
    
    //�L���̉摜
    if ((g_RoadImage = LoadGraph("images/road3.png")) == -1)return -1;
    if ((g_RoadImage2 = LoadGraph("images/road4.png")) == -1)return -1;

    //�G���h��ʔw�i
    if ((g_EndImage = LoadGraph("images/EndImage.png")) == -1)return -1;
    //�󔠂̉摜
    if (LoadDivGraph("images/TakaraBako.png", 4, 2, 2, 60, 60, g_TakaraBako) == -1) return -1;

    //�v���C���[���摜
    if ((g_Arrow = LoadGraph("images/Arrow.png")) == -1)return -1;

    //�v���C���[�摜
    if (LoadDivGraph("images/player.png", 16, 4, 4, 70, 90, g_Player) == -1) return -1;

    //�L�[�{�[�h���X�摜
    if (keyboard.LoadImgae() == -1)return -1;

    return 0;
}