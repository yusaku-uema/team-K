/********************************************************************
**   �Q�[���W����
** �@�󔠂��A�P���Q�[��
********************************************************************/
#include"DxLib.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include"keyboard.h"
#include"ranking.h"

/***********************************************
 * �萔��錾
 ***********************************************/
 //��ʗ̈�̑傫��
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//��������
const int TIMELIMIT = 30000;
//�󔠂̌�
const int TREASUREBOX_MAX = 7;

//�v���C���[�̂��
const int PLAYER_SPEED = 3;
const int PLAYER_IMAGE_TIME = 8;

//�w�i�̂��
const int STAGE_NO = 0; //�w�i�̘L���̒���
const int PLAYER_HP = 5;

//�G�L����
const int ENEMY_MAX = 5;
const int ENEMY_IMAGE_TIME = 8;

/***********************************************
 * �ϐ��̐錾
 ***********************************************/
int g_OldKey;  // �O��̓��̓L�[ 
int g_NowKey;  // ����̓��̓L�[ 
int g_KeyFlg;  // ���̓L�[��� 

int g_WalkOldKey = 0;

int g_GameState = 0;  // �Q�[�����[�h 

int g_GameMode = 0; //�Q�[�����[�h�i�_����������j

int g_TitleImage; // �摜�p�ϐ� 
int g_Menu; //���j���[���
int	g_MenuNumber = 0;		// ���j���[�J�[�\���ʒu
int g_MenuY;				// ���j���[�J�[�\���̂x���W

int g_EnemyQuantity; //�G�L�����̐�
int g_BoxQuantity;  //�󔠂̌�
int g_Score = 0; //�X�R�A

int g_TakaraPosition; //�󔠂̕\���Ɏg���i�_��
int g_OpenBox;//�󔠂��J�������̕ϐ��i�_��
int g_NowStage; //���݂̊K��

boolean g_Blinking; //�v���C���[�̓_�ŏ����i�_�����g��
boolean g_AcceptKey;

int g_WaitTime = 0; //�҂�����
int g_StartTime;   // �X�^�[�g����
int Time;   // ���ݎ���

int g_TakaraBako[3]; //�󔠂̉摜
int g_Player[16];
int g_EnemyImage[3][6];
int g_Arrow;  //�v���C���[�̖��̉摜
int g_Applec; //�^�C�g���J�[�\���ϐ��@�����Ȃ���

int g_StageImage;   //�Q�[�����C���w�i
int g_HelpImage; //�w���v�w�i
int g_EndImage;//�G���h�w�i
int g_RankingImage; //�����L���O�w�i
int g_RoadImage;
int g_RoadImage2;
int g_KeyImage; //���̉摜�i�_�����ǉ�
int g_MimicImage; //�~�~�b�N�̉摜�i�_�����ǉ�
int g_SlimeImage; //
int g_HeartImage;
int g_HeartImage1; //�n�[�g
int g_DrawStageImages; //�X�e�[�W�ŏ��̂�
int g_DrawStageImages1; //���ڈȍ~�͂��̉摜
int g_DrawStageno; //��
int g_PlayerIkon;//�v���C���[�A�C�R���w�i
int g_GameOverImage;//�Q�[���I�o�[�w�i

int g_PosY; //���v�{���񂪎g���܂�
int Font, Font1, Font3, Font4, Font5;//�w���v��ʂƃG���h��ʂ̃t�H���g�ύX
//���̍\����
int s_TitleBGM;//�^�C�g��BGM�p�ϐ�
int s_RoadBGM;//�L��BGM�p�ϐ�

//���̍\����
struct ARROW
{
    int x, y;      //���Wx,y
    int no;
    int oldkey;
};
//���
struct ARROW g_arrow;

//�v���C���[�̍\����
struct PLAYER
{
    int x, y;      //���Wx,y
    int w, h;
    int img;
    int imgtime;
    int hp;
    int flg;
};
//�v���C���[
struct PLAYER g_player;

struct ENEMY
{
    int x, y;
    int w, h;
    int direction;
    int img;
    int imgtime;
    int speed;
    int type;
};
struct ENEMY g_enemy[ENEMY_MAX];
struct ENEMY g_enemy1 = { 0, 0, 50, 50, TRUE, 3, ENEMY_IMAGE_TIME, 0, 0 };
struct ENEMY g_enemy2 = { 0, 0, 50, 50, TRUE, 3, ENEMY_IMAGE_TIME, 0, 1 };
struct ENEMY g_enemy3 = { 0, 0, 57, 60, TRUE, 3, ENEMY_IMAGE_TIME, 0, 2 };

//�L���̔w�i�摜�̍\���́i�_�����g���j
struct STAGE
{
    int no;
    int img;
    int x;
    int y;
};
struct STAGE g_stage;


//�󔠂̍\����
struct TAKARA {
    int flg; //�g�p�t���O
    int type; //�^�C�v
    int img; //�摜
    int x, y; //���Wx,y
    int point; //��̓��_
};
//�G�@
struct TAKARA g_treasurebox[TREASUREBOX_MAX];
struct TAKARA g_treasurebox00 = { TRUE,0,0,30,300,0 };

/***********************************************
 * �֐��̃v���g�^�C�v�錾
 ***********************************************/
void GameInit(void); //�Q�[����������
void StageInit(void); //�K�w���Ƃ̏�������

void GameMain(void); //�Q�[�����C������
void DrawEnd(void); //�Q�[���G���h
void DrawHelp(void); //�w���v���
void DrawGameTitle(void); //�^�C�g���`�揈��
void DrawGameOver(void); //�Q�[���I�[�o
int LoadImages(); //�摜�ǂݍ���
void UIView();
void TimeCount();
void BackScrool(int a); //�w�i�摜�X�N���[������
void BackImage(); //�󔠑I�����̔w�i�摜
void OpenTreasureBox();  //�󔠂��J�������̊֐��i�_��
void PlayerControl(); //�v���C���[�̏���
void ArrowControl();  //�v���C���[�̖��̏���
void TakaraControl(); //�󔠂̏���
void DrawStage(); //�X�e�[�W�̏��߂ɕ\�������i�_��

void InputRanking();  //�����L���O����
void DrawRanking();   //�����L���O�`��

void EnemyControl();
int HitBoxPlayer(PLAYER* p, ENEMY* e);

int LoadSounds(); //�����ǂݍ���

/***********************************************
 * �v���O�����̊J�n
 **********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    //�^�C�g����ݒ�
    SetMainWindowText("�A�P���_���W�����N�i���j");
    //�E�B���h�E���[�h�ŋN������ 
    ChangeWindowMode(TRUE);

    // DX ���C�u�����̏���������:�G���[���N�����璼���ɏI�� 
    if (DxLib_Init() == -1) return -1;

    Font = CreateFontToHandle("���C���I", 30, 9, DX_FONTTYPE_ANTIALIASING_EDGE);
    SetDrawScreen(DX_SCREEN_BACK);

    //�t�H���g�̌����ڑւ���悤
    Font = CreateFontToHandle("���C���I", 30, 9, DX_FONTTYPE_ANTIALIASING_EDGE);
    Font1 = CreateFontToHandle("���C���I", 50, 9, DX_FONTTYPE_ANTIALIASING_EDGE);
    Font3 = CreateFontToHandle("���C���I", 50, 9, DX_FONTTYPE_ANTIALIASING_EDGE);//"���C���I"  ��30pt,����3�̃t�H���g���쐬
    Font4 = CreateFontToHandle("���C���I", 30, 9, DX_FONTTYPE_ANTIALIASING_EDGE);//"���C���I"  ��30pt,����3�̃t�H���g���쐬
    Font5 = CreateFontToHandle("���C���I", 20, 9, DX_FONTTYPE_ANTIALIASING_EDGE);//"���C���I"  ��30pt,����3�̃t�H���g���쐬
    if (LoadImages() == -1) return -1; //�摜�Ǎ��݊֐����Ăяo��
    if (LoadSounds() == -1) return -1; //�����Ǎ��݊֐����Ăяo��
    if (ranking.ReadRanking() == -1) return -1;
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
            g_DrawStageno = 0;
            break;
        case 1:
            GameInit();
            break;
        case 2:
            DrawRanking();
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

        if (g_GameState != 0) {
            StopSoundMem(s_TitleBGM);
        }
        if (g_GameState != 5) {
            StopSoundMem(s_RoadBGM);
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

    if (CheckSoundMem(s_TitleBGM) != 1) {
        ChangeVolumeSoundMem(70, s_TitleBGM);
        PlaySoundMem(s_TitleBGM, DX_PLAYTYPE_LOOP, TRUE);
    }

    //���j���[�J�[�\���ړ�����
    if (g_KeyFlg & PAD_INPUT_DOWN) {
        if (++g_MenuNumber > 3) g_MenuNumber = 0;
    }
    if (g_KeyFlg & PAD_INPUT_UP) {
        if (--g_MenuNumber < 0) g_MenuNumber = 3;
    }

    //�y�L�[�Ń��j���[�I��
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = g_MenuNumber + 1;
    DrawBox(160, 245, 480, 450, GetColor(255, 255, 255), TRUE); //�����ȃ{�b�N�X�ǉ�
    //�^�C�g���摜�\��
    DrawGraph(0, 0, g_TitleImage, FALSE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(160, 245, 480, 450, GetColor(255, 255, 255), TRUE); //�����ȃ{�b�N�X�ǉ�
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    g_MenuY = g_MenuNumber * 52;
    //���j���[�J�[�\���i�O�p�`�j�̕\��
    if (g_MenuNumber == 0) { //�Q�[���X�^�[�g�̎��̃J�[�\��
        DrawTriangle(180, 255 + g_MenuY, 200, 270 + g_MenuY, 180, 285 + g_MenuY, GetColor(0, 0, 225), TRUE);
    }
    if (g_MenuNumber == 1) {
        DrawTriangle(210, 255 + g_MenuY, 230, 270 + g_MenuY, 210, 285 + g_MenuY, GetColor(0, 0, 225), TRUE);
    }
    if (g_MenuNumber == 2 || g_MenuNumber == 3) {
        DrawTriangle(240, 255 + g_MenuY, 260, 270 + g_MenuY, 240, 285 + g_MenuY, GetColor(0, 0, 225), TRUE);
    }
    DrawBox(160, 245, 480, 450, GetColor(0, 0, 0), FALSE); //�����ȃ{�b�N�X�ɍ�������ǉ�
    SetFontSize(40);
    //�Q�[���^�C�g�����ڂ���\��
    DrawStringToHandle(100, 130, "�Q�[���^�C�g���\��", GetColor(255, 255, 255), Font1);

    DrawStringToHandle(210, 255, "�Q�[���X�^�[�g", GetColor(255, 0, 0), Font);
    DrawStringToHandle(240, 305, "�����L���O", GetColor(255, 0, 0), Font);
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

    //���݂̃X�e�[�W
    g_NowStage = 1;

    //�ŏ��̃X�e�[�W�͕󔠂�2��
    g_BoxQuantity = 2;

    //���݂̌o�ߎ��Ԃ𓾂�
    g_StartTime = GetNowCount();

    //�v���C���[��������
    g_player = { 290, 400, 70, 90, 13, PLAYER_IMAGE_TIME, PLAYER_HP, TRUE };

    //�X�e�[�W���Ƃ̏�������
    StageInit();

    //�L�[�{�[�h�i�����L���O���́j��������
    keyboard.KeyBoardInit();

    //�Q�[�����C��������
    g_GameState = 5;
}

/***********************************************
 * �K�w���Ƃ̏�������
 ***********************************************/
void StageInit(void)
{
    //�󔠂��J���Ă邩�̏���
    g_OpenBox = -1;

    //�󔠂̏�������
    for (int i = 0; i < g_BoxQuantity; i++)
    {
        g_treasurebox[i] = g_treasurebox00;
        g_treasurebox[i].point = GetRand(3); //�����_���Œl��ς��遨�O����
    }
    g_treasurebox[GetRand(g_BoxQuantity - 1)].point = 0; //�����I�ɕ󔠈�ɂO�_��������

    //�v���C���[���̏�������
    g_arrow.no = 0;
    g_arrow.y = g_treasurebox00.y - 100;

    //�_�ł̎��Ɏg���ϐ�
    g_Blinking = TRUE;  //FALSE�̎��̓v���C���[��\�����Ȃ��i�_�ł̎��Ɏg�p�j

    //�v���C���[��������
    g_player = { 290, 400, 70, 90, 13, PLAYER_IMAGE_TIME, g_player.hp, TRUE };


    g_EnemyQuantity = GetRand(ENEMY_MAX);
    int g_EnemyType = GetRand(2);
    //�G�l�~�[�̏�������
    for (int i = 0; i < g_EnemyQuantity; i++)
    {
        if (g_EnemyType == 1)g_enemy[i] = g_enemy1;
        if (g_EnemyType == 2)g_enemy[i] = g_enemy3;
        if (g_EnemyType == 0)g_enemy[i] = g_enemy2;

        g_enemy[i].x = GetRand(290) + 145;
        g_enemy[i].y = -200 + ((GetRand(10) * 50));
        g_enemy[i].speed = GetRand(2) + 1;
    }

    //�w�i�摜(�L���j�̏�������
    g_stage = { STAGE_NO, g_RoadImage, 0, 0 };

    g_AcceptKey = TRUE;

    //�Q�[�����[�h��0�ɂ���
    g_GameMode = 0;  //�O�̏ꍇ�X�e�[�W�\����ʂ�
}

/***********************************************
 * �Q�[�����C��
 ***********************************************/
void GameMain(void)
{
    switch (g_GameMode) {
    case 0:
        DrawStage();
        break;
    case 1:
        BackScrool(0);
        EnemyControl();
        PlayerControl();
        if (CheckSoundMem(s_RoadBGM) != 1) {
            ChangeVolumeSoundMem(70, s_RoadBGM);
            PlaySoundMem(s_RoadBGM, DX_PLAYTYPE_LOOP, TRUE);
        }
        break;
    case 2:
        BackImage();
        TakaraControl();
        ArrowControl();
        if (g_AcceptKey == FALSE)OpenTreasureBox();
        break;
    }

    //�X�y�[�X�L�[�Ń��j���[�ɖ߂�@�Q�[�����C������^�C�g���ɖ߂�ǉ�
    if (g_KeyFlg & PAD_INPUT_M)g_GameState = 6;
    SetFontSize(16);
    //DrawString(150, 450, "---�X�y�[�X�L�[�������ăQ�[���I�[�o�[��---", 0xffffff, 0);
}

/***********************************************
 *�Q�[���X�e�[�W�\��
 ***********************************************/
void DrawStage()
{
    int ay, by;

    if (g_NowStage <= 1)
    {
        DrawGraph(0, 0, g_DrawStageImages, FALSE);
        ay = 170, by = 270;
    }
    else if (g_NowStage >= 2)
    {
        DrawGraph(0, 100, g_DrawStageImages1, FALSE);
        ay = 40, by = 430;
    }
    SetFontSize(50);
    DrawFormatString(280, ay, GetColor(255, 255, 255), "%d�K", g_NowStage);
    DrawFormatString(290, by, GetColor(255, 255, 255), "�~", g_NowStage);
    DrawFormatString(360, by, GetColor(255, 255, 255), "%d", g_player.hp);
    DrawGraph(230, by, g_HeartImage, TRUE);

    if (++g_WaitTime > 100 || g_KeyFlg & PAD_INPUT_A)
    {
        g_GameMode = 1;
        g_WaitTime = 0;
    }
}

/***********************************************
 *�Q�[���I�[�o�[��ʕ`�揈��
 ***********************************************/
void DrawGameOver(void)
{
    //BackScrool();
    DrawBox(150, 150, 490, 330, 0x009900, TRUE);
    DrawBox(150, 150, 490, 330, 0x000000, FALSE);

    SetFontSize(20);
    DrawString(220, 170, "�Q�[���I�[�o�[", 0xcc0000);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawStringToHandle(130, 100, "GAME   OVER", GetColor(255,0, 0), Font3);
    SetFontSize(35);

    DrawFormatString(180, 250, 0xFFFFFF, "�ŏI�K�w = %02d�K", g_NowStage);

    SetFontSize(20);
    DrawString(150, 450, "---A�{�^���������ă^�C�g���֖߂� ---", 0xffffff, 0);//z�L�[�Ŗ߂�
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = 0;
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

    for (int i = 0; i < g_EnemyQuantity; i++)
    {
        g_enemy[i].y += (PLAYER_SPEED * a);
        DrawGraph(g_enemy[i].x, g_enemy[i].y, g_EnemyImage[g_enemy[i].type][g_enemy[i].img], TRUE);
        DrawFormatString(g_enemy[i].x, g_enemy[i].y - 30, GetColor(255, 255, 255), "%d", g_enemy[i].type);
    }

    if (g_stage.y >= 480)
    {
        g_stage.y = 0;
        g_stage.no--;
        if (g_stage.no < 0)g_stage.img = g_RoadImage2;
    }
}

void BackImage()
{
    DrawGraph(0, 100, g_StageImage, FALSE);
}

//***********************************************
//* �G�l�~�[�̊֐�
// * ���@���F�Ȃ�
// * �߂�l�F�Ȃ�
// ***********************************************/
void EnemyControl()
{


    for (int i = 0; i < g_EnemyQuantity; i++)
    {
        g_enemy[i].imgtime--;
        if (g_enemy[i].imgtime <= 0)
        {
            g_enemy[i].img++;
            g_enemy[i].imgtime = ENEMY_IMAGE_TIME;
        }

        if (g_enemy[i].direction == TRUE)
        {
            g_enemy[i].x += g_enemy[i].speed;
            if (g_enemy[i].img > 5)g_enemy[i].img = 3;
        }
        else if (g_enemy[i].direction == FALSE)
        {
            g_enemy[i].x -= g_enemy[i].speed;
            if (g_enemy[i].img > 2)g_enemy[i].img = 0;
        }

        if (g_enemy[i].x > (490 - g_enemy[i].w))
        {
            g_enemy[i].direction = FALSE;
            g_enemy[i].img = 0;
        }
        else if (g_enemy[i].x < 150)
        {
            g_enemy[i].direction = TRUE;
            g_enemy[i].img = 3;
        }

        if (HitBoxPlayer(&g_player, &g_enemy[i]) == TRUE)
        {
            if (g_player.flg == TRUE)g_player.hp--;
            if (g_player.hp <= 0)
            {
                g_WaitTime = 0;
                g_GameState = 6;
            }
                
            if (g_player.flg == TRUE)g_AcceptKey = FALSE; //�}���I�݂����Ɉ�u�~�߂�
            g_player.flg = FALSE;

        }
    }
}

//***********************************************
//* �����蔻��̊֐�
// * ���@���F����
// * �߂�l�F����
// ***********************************************/
int HitBoxPlayer(PLAYER* p, ENEMY* e)
{
    //x,y�͒��S���W�Ƃ���
    int sx1 = p->x + 15;
    int sy1 = p->y + 30;
    int sx2 = sx1 + p->w - 30;
    int sy2 = sy1 + p->h - 40;

    int dx1 = e->x + 10;
    int dy1 = e->y + 10;
    int dx2 = dx1 + e->w - 20;
    int dy2 = dy1 + e->h - 20;

    //�Z�`���d�Ȃ��Ă���΂�����
    if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
        return TRUE;
    }
    return FALSE;
}


/***********************************************
 * �v���C���[�̏���
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void PlayerControl()
{
    if (g_AcceptKey == TRUE)
    {
        if (g_NowKey & PAD_INPUT_UP)
        {
            if (g_WalkOldKey != 8)g_player.img = 12;
            g_WalkOldKey = 8;
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
            if (g_WalkOldKey != 1)g_player.img = 0;
            g_WalkOldKey = 1;
            if (g_player.imgtime <= 0)
            {
                g_player.img++;
                if (g_player.img > 3)g_player.img = 0;
            }

            g_player.y += PLAYER_SPEED;
        }
        else if (g_NowKey & PAD_INPUT_LEFT)
        {
            if (g_WalkOldKey != 2) g_player.img = 8;
            g_WalkOldKey = 2;
            if (g_player.imgtime <= 0)
            {
                g_player.img++;
                if (g_player.img > 11)g_player.img = 8;
            }
            g_player.x -= PLAYER_SPEED;
        }
        else if (g_NowKey & PAD_INPUT_RIGHT)
        {
            if (g_WalkOldKey != 4) g_player.img = 4;
            g_WalkOldKey = 4;
            if (g_player.imgtime <= 0)
            {
                g_player.img++;
                if (g_player.img > 7)g_player.img = 4;
            }
            g_player.x += PLAYER_SPEED;
        }
    }

    if (g_player.flg == FALSE)
    {
        if (g_WaitTime >= 20)g_AcceptKey = TRUE;
        if (g_WaitTime % 10 == 0 || g_WaitTime == 0)g_Blinking = ~g_Blinking;
        if (++g_WaitTime > 100)
        {
            g_player.flg = TRUE;
            g_Blinking = TRUE;
            g_WaitTime = 0;
        }
    }

    if (g_player.imgtime <= 0)g_player.imgtime = PLAYER_IMAGE_TIME;
    g_player.imgtime--;

    DrawFormatString(0, 60, 0x111FFF, "%d", g_WalkOldKey);

    if (g_player.x > 440)g_player.x = 440;
    if (g_player.x < 150)g_player.x = 150;
    if (g_player.y > 400)g_player.y = 400;
    if (g_player.y < 59 && g_player.x >= 270 && g_player.x <= 320)g_GameMode = 2;
    if (g_player.y < 60)g_player.y = 60;

    for (int i = 0; i < g_player.hp; i++)
    {
        DrawGraph(55 * i, 10, g_HeartImage, TRUE);
    }

    DrawFormatString(0, 0, 0x111FFF, "�L�����摜 = %d", g_player.img);
    DrawFormatString(0, 20, 0x111FFF, "X = %d", g_player.x);
    DrawFormatString(0, 40, 0x111FFF, "Y = %d", g_player.y);
    if (g_Blinking == TRUE)DrawGraph(g_player.x, g_player.y, g_Player[g_player.img], TRUE);
}


/***********************************************
 * ���̏���
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void ArrowControl()
{
    if (g_AcceptKey == TRUE)
    {
        if (g_KeyFlg & PAD_INPUT_LEFT)
        {
            g_arrow.no--;
            if (g_arrow.no < 0) g_arrow.no = g_BoxQuantity - 1;
        }
        else if (g_KeyFlg & PAD_INPUT_RIGHT)
        {
            g_arrow.no++;
            if (g_arrow.no > g_BoxQuantity - 1) g_arrow.no = 0;
        }

        else if (g_KeyFlg & PAD_INPUT_A)
        {
            if (g_treasurebox[g_arrow.no].flg == TRUE)
            {
                g_Score += g_treasurebox[g_arrow.no].point;
            }
            g_OpenBox = g_arrow.no;
            g_AcceptKey = FALSE;
            g_KeyFlg = 0;
        }
    }

    if (g_OpenBox <= -1)
    {
        SetFontSize(40);
        /*g_arrow.x = g_treasurebox[g_arrow.no].x;*/
        DrawString(150, 400, "�ǂ�ɂ��悤���ȁH", 0xffffff, TRUE);
    }
    
  
    g_arrow.x = g_treasurebox[g_arrow.no].x;

    for (int i = 0; i < g_player.hp; i++)
    {
        DrawGraph(55 * i, 10, g_HeartImage, TRUE);
    }

    DrawGraph(g_arrow.x, g_arrow.y, g_Arrow, TRUE);
}

/***********************************************
 * �󔠂̕\��
 * ��  ��:�Ȃ�
 * �߂�l:TRUE�����AFALSE���s
 ***********************************************/
void TakaraControl()
{
    g_TakaraPosition = (640 - ((g_BoxQuantity * 60) + ((g_BoxQuantity - 1) * 20))) / 2;

    for (int i = 0; i < g_BoxQuantity; i++)
    {
        g_treasurebox[i].x = i * 80 + g_TakaraPosition;
        DrawGraph(g_treasurebox[i].x, g_treasurebox[i].y, g_TakaraBako[g_treasurebox[i].img], TRUE); //�󔠂̕\��
        DrawFormatString(g_treasurebox[i].x, g_treasurebox[i].y - 20, 0xFFFFFF, "%d", g_treasurebox[i].point);
    }
}

/***********************************************
 * �󔠂��J�����Ƃ�
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void OpenTreasureBox()
{
    SetFontSize(40);

    if (g_treasurebox[g_OpenBox].point <= 0) //�󔠂̒��g���J�M�i0�j��������
    {
        g_treasurebox[g_OpenBox].img = 1;  //����̕󔠂̉摜�ɐ؂�ւ���
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);        //�u�����h���[�h����(128/255)�ɐݒ�
        DrawBox(180, 100, 460, 380, GetColor(255, 255, 255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        DrawGraph(180, 100, g_KeyImage, TRUE); //���̉摜��\��������

        DrawString(150, 400, "������[�I�J�M���I", 0xffffff, TRUE);
    }

    else if (g_treasurebox[g_OpenBox].point >= 2) //�󔠂̒��g���~�~�b�N�������ꍇ
    {
        if (g_treasurebox[g_OpenBox].flg == TRUE)
        {
            g_treasurebox[g_OpenBox].img = 2;  //�~�~�b�N�̉摜�ɐ؂�ւ���
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);        //�u�����h���[�h����(128/255)�ɐݒ�
            DrawBox(180, 100, 460, 380, GetColor(255, 255, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawGraph(180, 100, g_MimicImage, TRUE);  //�~�~�b�N�̉摜��\��

            DrawString(160, 400, "�~�b�A�~�~�b�N���I", 0xffffff, TRUE);
        }

        else if (g_treasurebox[g_OpenBox].flg == FALSE)
        {
            DrawString(160, 400, "�G��Ƃ����񂾁I", 0xffffff, TRUE);
        }
    }
      if (g_treasurebox[g_OpenBox].point == 1)
      {
          if (g_treasurebox[g_OpenBox].flg == TRUE)
          {
              g_treasurebox[g_OpenBox].img = 1;  //�~�~�b�N�̉摜�ɐ؂�ւ���
              SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);        //�u�����h���[�h����(128/255)�ɐݒ�
              DrawBox(180, 100, 460, 380, GetColor(255, 255, 255), TRUE);
              SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
              DrawGraph(180, 100, g_HeartImage1, TRUE);  //�~�~�b�N�̉摜��\��
              if (g_player.hp == PLAYER_HP)//HP��MAX�̎��ɕ\������
              { 
                  DrawString(230, 400, "HP��MAX��!", 0xffffff, TRUE);
              }
              else if (g_player.hp < PLAYER_HP) //HP�������Ă������ɕ\������B
              { 
                  DrawString(160, 400, "HP���P�񕜂���!", 0xffffff, TRUE);
              }
             
          }
          else if (g_treasurebox[g_OpenBox].flg == FALSE)
          {
              DrawString(180, 400, "���g��������ۂ��I", 0xffffff, TRUE);
          }
      }

    if (g_KeyFlg & PAD_INPUT_A) //������A�C�e���̉摜���\������Ă���Ƃ�Z�L�[��������
    {
        if (g_treasurebox[g_OpenBox].point > 1)  //�~�~�b�N�������ꍇ
        {
            if (g_treasurebox[g_OpenBox].flg == TRUE) g_player.hp--; //�v���C���[��HP���}�C�i�X1����
            g_treasurebox[g_OpenBox].flg = FALSE;
            if (g_player.hp <= 0)g_GameState = 6;  //HP��0�ɂȂ������Q�[���I�[�o�[�ɂ���
        }
        else if (g_treasurebox[g_OpenBox].point == 1)
        {
            if (g_treasurebox[g_OpenBox].flg == TRUE)g_player.hp++; //�v���C���[��HP���v���X�P����
            if (g_player.hp > PLAYER_HP)g_player.hp = PLAYER_HP;
            g_treasurebox[g_OpenBox].flg = FALSE;
        }
        else //������������͂��낢�돉��������
        {
            g_NowStage++; //�X�e�[�W��1����

            g_BoxQuantity = GetRand(TREASUREBOX_MAX - 2) + 2;

            StageInit();
        }

        g_AcceptKey = TRUE; //TRUE�ɂ���ƕ󔠂�I���ł���悤�ɂȂ�
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

    ////UI�uTIME�v�\��
    //SetFontSize(50);
    //DrawString(520, 40, "TIME", 0xffffff, 0);

    ////UI�uSCORE�v�\��
    //SetFontSize(45);
    //DrawString(510, 320, "SCORE", 0xffffff, 0);
    //DrawFormatString(510, 360, 0x00ffff, "%d", g_Score);
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

    DrawStringToHandle(200, 100 + g_PosY, "�^�C�g��", GetColor(255, 255, 255), Font3);
   
    DrawStringToHandle(250, 200 + g_PosY, "�����", GetColor(255, 255, 255), Font4);

    DrawStringToHandle(150, 260 + g_PosY, "       ��ԁ@�Z�Z�Z����", GetColor(255, 255, 255), Font5);
    DrawFormatString(0, 0, 0x00ffff, "%d",g_WaitTime);
    if (g_WaitTime > 2300) {
        DrawStringToHandle(0, 240, "Thank you for Playing", GetColor(255, 255, 255), Font1);
    }
    DrawStringToHandle(150, 290 + g_PosY, "       �_���@�Z�Z�Z����", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 320 + g_PosY, "       ���v�{�@�Z�Z�Z����", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 350 + g_PosY, "       ��[�@�Z�Z�Z����", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 380 + g_PosY, "       �����@�Z�Z�Z����", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 410 + g_PosY, "       ���Ñ��@�Z�Z�Z����", GetColor(255, 255, 255), Font5);

    //�Q�p�[�g
  
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
    DrawStringToHandle(150, 150, "�v���C���[����ɂ���", GetColor(255, 0, 0), Font4);
    DrawStringToHandle(240, 250, "�󔠂�I��", GetColor(255, 0, 0), Font4);
    //�X�y�[�X�L�[�Ń��j���[�ɖ߂�
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;
    DrawStringToHandle(190, 200, "�\���L�[��X�e�B�b�N�ňړ�", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(100, 300, "�J�������󔠂�I��������@�͏\���L�[��A", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(100, 320, "�X�e�B�b�N�Ŏ�肽���󔠂�I�ԁB", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(100, 350, "�J�������󔠂̌���@A�{�^��", GetColor(255, 255, 255), Font5);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);        //�u�����h���[�h����(128/255)�ɐݒ�
    DrawBox(50, 25, 590, 500, GetColor(255, 255, 255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(150, 425, "---A�{�^���Ń^�C�g���Ɉړ�---", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 455, "---B�{�^���Ń^�C�g���Ɉړ�---", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(200, 50, "�w���v���", GetColor(255, 255, 255), Font3);

    //�傫���������o��
    DrawStringToHandle(150, 150, "�v���C���[����ɂ���", GetColor(255, 255, 255), Font4);
    DrawStringToHandle(300, 250, "��", GetColor(255, 255, 255), Font4);
    //A�Ń��j���[�ɖ߂�
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = 0;
    //B�{�^���ŃQ�[���X�^�[�g
    if (g_KeyFlg & PAD_INPUT_B) g_GameState = 1;
    //���������o��
    DrawStringToHandle(250, 200, "�\���L�[�ňړ�", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(100, 300, "�J�[�\���ړ��@�\���L�[�Ŏ�肽���󔠂�I��", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(225, 350, "�󔠂̌���@B�{�^��", GetColor(255, 255, 255), Font5);

    //�����̕\���i�_�Łj
    if (++g_WaitTime < 30)
    {
        DrawStringToHandle(150, 425, "---B�{�^���ŃQ�[�����C���Ɉړ�---", GetColor(255, 255, 255), Font5);
    }
    else if (g_WaitTime > 60)
    {
        g_WaitTime = 0;
    }
}


/***********************************************
 * �����L���O���͏���
 ***********************************************/
void InputRanking()
{
    keyboard.DrawKeyBoard();
    keyboard.KeyBoardControl(g_NowKey);
    keyboard.Push_B_Key(g_NowKey, &g_GameState, g_Score);
}
/***********************************************
 * �����L���O�`�揈��
 ***********************************************/
void DrawRanking()
{
    // B�{�^�� or X�L�[ �Ń^�C�g���ɖ߂�
    if (g_KeyFlg & PAD_INPUT_B) g_GameState = 0;

    //�����L���O�摜�\��
    DrawGraph(0, 0, g_RankingImage, FALSE);

    ranking.DrawRanking();

    // �����̕\��(�_��)
    if (++g_WaitTime < 30)
    {
        SetFontSize(24);
        DrawString(150, 450, "--  B�{�^���������Ė߂�  --", 0xffffed);
    }
    else if (g_WaitTime > 60) g_WaitTime = 0;
}

/***********************************************
 * �摜�ǂݍ���
 ***********************************************/
int LoadImages()
{
    //�^�C�g�� �^�C�g���摜�ւ��܂����B
    if ((g_TitleImage = LoadGraph("images/Title.png")) == -1) return -1;

    //���j���[
   // if ((g_Applec = LoadGraph("images/Applec.png")) == -1) return -1;

    //�X�e�[�W�w�i
    if ((g_StageImage = LoadGraph("images/haikei1.png")) == -1)return -1;
    if ((g_DrawStageImages = LoadGraph("images/doukutu.png")) == -1)return -1;
    if ((g_DrawStageImages1 = LoadGraph("images/doukutu1 .png")) == -1)return -1;

    //�L���̉摜
    if ((g_RoadImage = LoadGraph("images/road7.png")) == -1)return -1;
    if ((g_RoadImage2 = LoadGraph("images/road8.png")) == -1)return -1;

    //�G���h��ʔw�i
    if ((g_EndImage = LoadGraph("images/EndImage.png")) == -1)return -1;
    //�����L���O�w�i
    if ((g_RankingImage = LoadGraph("images/rank_back.png")) == -1) return -1;
    //�󔠂̉摜
    if (LoadDivGraph("images/takarabako.png", 3, 3, 1, 60, 60, g_TakaraBako) == -1) return -1;
    //�G�P�̉摜
    if (LoadDivGraph("images/marimo.png", 6, 3, 2, 50, 50, g_EnemyImage[0]) == -1) return -1;
    if (LoadDivGraph("images/koumori.png", 6, 3, 2, 50, 50, g_EnemyImage[1]) == -1) return -1;
    if (LoadDivGraph("images/obake.png", 6, 3, 2, 57, 60, g_EnemyImage[2]) == -1) return -1;
    //���̉摜
    if ((g_KeyImage = LoadGraph("images/Key.png")) == -1)return -1;
    //�~�~�b�N�̉摜
    if ((g_MimicImage = LoadGraph("images/Mimic.png")) == -1)return -1;
    //�n�[�g�̉摜
    if ((g_HeartImage = LoadGraph("images/heart.png")) == -1)return -1;
    if ((g_HeartImage1 = LoadGraph("images/HP.png")) == -1)return -1;


    //�L�[�{�[�h���X
    if (keyboard.LoadImgae() == -1) return -1;

    //�v���C���[���摜
    if ((g_Arrow = LoadGraph("images/Arrow.png")) == -1)return -1;

    //�w���v���
    if ((g_HelpImage = LoadGraph("images/Help.png")) == -1)return -1;
    //�v���C���[�摜
    if (LoadDivGraph("images/player.png", 16, 4, 4, 70, 90, g_Player) == -1) return -1;

    return 0;
}

/***********************************************
 * �����ǂݍ���
 ***********************************************/
int LoadSounds()
{
    //�^�C�g�� �^�C�g���摜�ւ��܂����B
    //if ((s_TitleBGM = LoadSoundMem("BGM/see.mp3")) == -1) return -1;

    //�L�[�{�[�h
    if (keyboard.LoadSounds() == -1) return -1;

    ////���j���[
    //if ((g_Applec = LoadGraph("images/Applec.png")) == -1) return -1;

    ////�X�e�[�W�w�i
    //if ((g_StageImage = LoadGraph("images/haikei.png")) == -1)return -1;

    //�L���̉摜
    if ((s_RoadBGM = LoadSoundMem("BGM/Flutter.mp3")) == -1) return -1;

    ////�G���h��ʔw�i
    //if ((g_EndImage = LoadGraph("images/EndImage.png")) == -1)return -1;
    ////�󔠂̉摜
    //if (LoadDivGraph("images/takarabako.png", 3, 3, 1, 60, 60, g_TakaraBako) == -1) return -1;

    ////���̉摜
    //if ((g_KeyImage = LoadGraph("images/Key.png")) == -1)return -1;
    ////�~�~�b�N�̉摜
    //if ((g_MimicImage = LoadGraph("images/Mimic.png")) == -1)return -1;
    ////�n�[�g�̉摜
    //if ((g_HeartImage = LoadGraph("images/heart.png")) == -1)return -1;

    ////�L�[�{�[�h���X
    //if (keyboard.LoadImgae() == -1) return -1;

    ////�v���C���[���摜
    //if ((g_Arrow = LoadGraph("images/Arrow.png")) == -1)return -1;

    ////�w���v���
    //if ((g_HelpImage = LoadGraph("images/Help.png")) == -1)return -1;
    ////�v���C���[�摜
    //if (LoadDivGraph("images/player.png", 16, 4, 4, 70, 90, g_Player) == -1) return -1;

    return 0;
}