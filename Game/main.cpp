/********************************************************************
**   ゲームジャム
** 　宝箱をアケルゲーム
********************************************************************/
#include"DxLib.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include"keyboard.h"
#include"ranking.h"

/***********************************************
 * 定数を宣言
 ***********************************************/
 //画面領域の大きさ
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//制限時間
const int TIMELIMIT = 30000;
//宝箱の個数
const int TREASUREBOX_MAX = 7;

//プレイヤーのやつ
const int PLAYER_SPEED = 3;
const int PLAYER_IMAGE_TIME = 8;

//背景のやつ
const int STAGE_NO = 0; //背景の廊下の長さ
const int PLAYER_HP = 5;

//敵キャラ
const int ENEMY_MAX = 5;
const int ENEMY_IMAGE_TIME = 8;

/***********************************************
 * 変数の宣言
 ***********************************************/
int g_OldKey;  // 前回の入力キー 
int g_NowKey;  // 今回の入力キー 
int g_KeyFlg;  // 入力キー情報 

int g_WalkOldKey = 0;

int g_GameState = 0;  // ゲームモード 

int g_GameMode = 0; //ゲームモード（神里が作った）

int g_TitleImage; // 画像用変数 
int g_Menu; //メニュー画面
int	g_MenuNumber = 0;		// メニューカーソル位置
int g_MenuY;				// メニューカーソルのＹ座標

int g_EnemyQuantity; //敵キャラの数
int g_BoxQuantity;  //宝箱の個数
int g_Score = 0; //スコア

int g_TakaraPosition; //宝箱の表示に使う（神里
int g_OpenBox;//宝箱を開けた時の変数（神里
int g_NowStage; //現在の階数

boolean g_Blinking; //プレイヤーの点滅処理（神里が使う
boolean g_AcceptKey;

int g_WaitTime = 0; //待ち時間
int g_StartTime;   // スタート時間
int Time;   // 現在時間

int g_TakaraBako[3]; //宝箱の画像
int g_Player[16];
int g_EnemyImage[3][6];
int g_Arrow;  //プレイヤーの矢印の画像
int g_Applec; //タイトルカーソル変数　消さないで

int g_StageImage;   //ゲームメイン背景
int g_HelpImage; //ヘルプ背景
int g_EndImage;//エンド背景
int g_RankingImage; //ランキング背景
int g_RoadImage;
int g_RoadImage2;
int g_KeyImage; //鍵の画像（神里が追加
int g_MimicImage; //ミミックの画像（神里が追加
int g_SlimeImage; //
int g_HeartImage;
int g_HeartImage1; //ハート
int g_DrawStageImages; //ステージ最初のみ
int g_DrawStageImages1; //二回目以降はこの画像
int g_DrawStageno; //回数
int g_PlayerIkon;//プレイヤーアイコン背景
int g_GameOverImage;//ゲームオバー背景

int g_PosY; //佐久本さんが使います
int Font, Font1, Font3, Font4, Font5;//ヘルプ画面とエンド画面のフォント変更
//矢印の構造体
int s_TitleBGM;//タイトルBGM用変数
int s_RoadBGM;//廊下BGM用変数

//矢印の構造体
struct ARROW
{
    int x, y;      //座標x,y
    int no;
    int oldkey;
};
//矢印
struct ARROW g_arrow;

//プレイヤーの構造体
struct PLAYER
{
    int x, y;      //座標x,y
    int w, h;
    int img;
    int imgtime;
    int hp;
    int flg;
};
//プレイヤー
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

//廊下の背景画像の構造体（神里が使う）
struct STAGE
{
    int no;
    int img;
    int x;
    int y;
};
struct STAGE g_stage;


//宝箱の構造体
struct TAKARA {
    int flg; //使用フラグ
    int type; //タイプ
    int img; //画像
    int x, y; //座標x,y
    int point; //宝の得点
};
//敵機
struct TAKARA g_treasurebox[TREASUREBOX_MAX];
struct TAKARA g_treasurebox00 = { TRUE,0,0,30,300,0 };

/***********************************************
 * 関数のプロトタイプ宣言
 ***********************************************/
void GameInit(void); //ゲーム初期処理
void StageInit(void); //階層ごとの初期処理

void GameMain(void); //ゲームメイン処理
void DrawEnd(void); //ゲームエンド
void DrawHelp(void); //ヘルプ画面
void DrawGameTitle(void); //タイトル描画処理
void DrawGameOver(void); //ゲームオーバ
int LoadImages(); //画像読み込み
void UIView();
void TimeCount();
void BackScrool(int a); //背景画像スクロール処理
void BackImage(); //宝箱選択時の背景画像
void OpenTreasureBox();  //宝箱を開けた時の関数（神里
void PlayerControl(); //プレイヤーの処理
void ArrowControl();  //プレイヤーの矢印の処理
void TakaraControl(); //宝箱の処理
void DrawStage(); //ステージの初めに表示される（神里

void InputRanking();  //ランキング入力
void DrawRanking();   //ランキング描画

void EnemyControl();
int HitBoxPlayer(PLAYER* p, ENEMY* e);

int LoadSounds(); //音声読み込み

/***********************************************
 * プログラムの開始
 **********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    //タイトルを設定
    SetMainWindowText("アケロダンジョン君（仮）");
    //ウィンドウモードで起動する 
    ChangeWindowMode(TRUE);

    // DX ライブラリの初期化処理:エラーが起きたら直ちに終了 
    if (DxLib_Init() == -1) return -1;

    Font = CreateFontToHandle("メイリオ", 30, 9, DX_FONTTYPE_ANTIALIASING_EDGE);
    SetDrawScreen(DX_SCREEN_BACK);

    //フォントの見た目替えるよう
    Font = CreateFontToHandle("メイリオ", 30, 9, DX_FONTTYPE_ANTIALIASING_EDGE);
    Font1 = CreateFontToHandle("メイリオ", 50, 9, DX_FONTTYPE_ANTIALIASING_EDGE);
    Font3 = CreateFontToHandle("メイリオ", 50, 9, DX_FONTTYPE_ANTIALIASING_EDGE);//"メイリオ"  の30pt,太さ3のフォントを作成
    Font4 = CreateFontToHandle("メイリオ", 30, 9, DX_FONTTYPE_ANTIALIASING_EDGE);//"メイリオ"  の30pt,太さ3のフォントを作成
    Font5 = CreateFontToHandle("メイリオ", 20, 9, DX_FONTTYPE_ANTIALIASING_EDGE);//"メイリオ"  の30pt,太さ3のフォントを作成
    if (LoadImages() == -1) return -1; //画像読込み関数を呼び出し
    if (LoadSounds() == -1) return -1; //音声読込み関数を呼び出し
    if (ranking.ReadRanking() == -1) return -1;
    //ゲームループ 
    while (ProcessMessage() == 0 && g_GameState != 99) {
        //キー入力取得 
        g_OldKey = g_NowKey;
        g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1); //ゲームパッドを持ってきていない人用に後でゲームパッドのみに変更
        g_KeyFlg = g_NowKey & ~g_OldKey;

        // 画面の初期化 
        ClearDrawScreen();
        switch (g_GameState) {
        case 0:
            DrawGameTitle(); //ゲームタイトル描画処理
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

        //裏画面の内容を表画面に反映します 
        ScreenFlip();
    }
    //DX ライブラリ使用の終了処理 
    DxLib_End();

    //ソフトの終了 
    return 0;
}

/***********************************************
 * ゲームタイトル表示
 ***********************************************/
void DrawGameTitle(void) {

    if (CheckSoundMem(s_TitleBGM) != 1) {
        ChangeVolumeSoundMem(70, s_TitleBGM);
        PlaySoundMem(s_TitleBGM, DX_PLAYTYPE_LOOP, TRUE);
    }

    //メニューカーソル移動処理
    if (g_KeyFlg & PAD_INPUT_DOWN) {
        if (++g_MenuNumber > 3) g_MenuNumber = 0;
    }
    if (g_KeyFlg & PAD_INPUT_UP) {
        if (--g_MenuNumber < 0) g_MenuNumber = 3;
    }

    //Ｚキーでメニュー選択
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = g_MenuNumber + 1;
    DrawBox(160, 245, 480, 450, GetColor(255, 255, 255), TRUE); //透明なボックス追加
    //タイトル画像表示
    DrawGraph(0, 0, g_TitleImage, FALSE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(160, 245, 480, 450, GetColor(255, 255, 255), TRUE); //透明なボックス追加
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    g_MenuY = g_MenuNumber * 52;
    //メニューカーソル（三角形）の表示
    if (g_MenuNumber == 0) { //ゲームスタートの時のカーソル
        DrawTriangle(180, 255 + g_MenuY, 200, 270 + g_MenuY, 180, 285 + g_MenuY, GetColor(0, 0, 225), TRUE);
    }
    if (g_MenuNumber == 1) {
        DrawTriangle(210, 255 + g_MenuY, 230, 270 + g_MenuY, 210, 285 + g_MenuY, GetColor(0, 0, 225), TRUE);
    }
    if (g_MenuNumber == 2 || g_MenuNumber == 3) {
        DrawTriangle(240, 255 + g_MenuY, 260, 270 + g_MenuY, 240, 285 + g_MenuY, GetColor(0, 0, 225), TRUE);
    }
    DrawBox(160, 245, 480, 450, GetColor(0, 0, 0), FALSE); //透明なボックスに黒い線を追加
    SetFontSize(40);
    //ゲームタイトルを載せる予定
    DrawStringToHandle(100, 130, "ゲームタイトル予定", GetColor(255, 255, 255), Font1);

    DrawStringToHandle(210, 255, "ゲームスタート", GetColor(255, 0, 0), Font);
    DrawStringToHandle(240, 305, "ランキング", GetColor(255, 0, 0), Font);
    DrawStringToHandle(275, 355, "ヘルプ", GetColor(255, 0, 0), Font);
    DrawStringToHandle(275, 405, "エンド", GetColor(255, 0, 0), Font);
}

/***********************************************
 * ゲーム初期処理
 ***********************************************/
void GameInit(void)
{
    //スコアの初期処理6
    g_Score = 0;

    //現在のステージ
    g_NowStage = 1;

    //最初のステージは宝箱は2個
    g_BoxQuantity = 2;

    //現在の経過時間を得る
    g_StartTime = GetNowCount();

    //プレイヤー初期処理
    g_player = { 290, 400, 70, 90, 13, PLAYER_IMAGE_TIME, PLAYER_HP, TRUE };

    //ステージごとの初期処理
    StageInit();

    //キーボード（ランキング入力）初期処理
    keyboard.KeyBoardInit();

    //ゲームメイン処理へ
    g_GameState = 5;
}

/***********************************************
 * 階層ごとの初期処理
 ***********************************************/
void StageInit(void)
{
    //宝箱を開けてるかの処理
    g_OpenBox = -1;

    //宝箱の初期処理
    for (int i = 0; i < g_BoxQuantity; i++)
    {
        g_treasurebox[i] = g_treasurebox00;
        g_treasurebox[i].point = GetRand(3); //ランダムで値を変える→０が鍵
    }
    g_treasurebox[GetRand(g_BoxQuantity - 1)].point = 0; //強制的に宝箱一つに０点を代入する

    //プレイヤー矢印の初期処理
    g_arrow.no = 0;
    g_arrow.y = g_treasurebox00.y - 100;

    //点滅の時に使う変数
    g_Blinking = TRUE;  //FALSEの時はプレイヤーを表示しない（点滅の時に使用）

    //プレイヤー初期処理
    g_player = { 290, 400, 70, 90, 13, PLAYER_IMAGE_TIME, g_player.hp, TRUE };


    g_EnemyQuantity = GetRand(ENEMY_MAX);
    int g_EnemyType = GetRand(2);
    //エネミーの初期処理
    for (int i = 0; i < g_EnemyQuantity; i++)
    {
        if (g_EnemyType == 1)g_enemy[i] = g_enemy1;
        if (g_EnemyType == 2)g_enemy[i] = g_enemy3;
        if (g_EnemyType == 0)g_enemy[i] = g_enemy2;

        g_enemy[i].x = GetRand(290) + 145;
        g_enemy[i].y = -200 + ((GetRand(10) * 50));
        g_enemy[i].speed = GetRand(2) + 1;
    }

    //背景画像(廊下）の初期処理
    g_stage = { STAGE_NO, g_RoadImage, 0, 0 };

    g_AcceptKey = TRUE;

    //ゲームモードを0にする
    g_GameMode = 0;  //０の場合ステージ表示画面へ
}

/***********************************************
 * ゲームメイン
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

    //スペースキーでメニューに戻る　ゲームメインからタイトルに戻る追加
    if (g_KeyFlg & PAD_INPUT_M)g_GameState = 6;
    SetFontSize(16);
    //DrawString(150, 450, "---スペースキーを押してゲームオーバーへ---", 0xffffff, 0);
}

/***********************************************
 *ゲームステージ表示
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
    DrawFormatString(280, ay, GetColor(255, 255, 255), "%d階", g_NowStage);
    DrawFormatString(290, by, GetColor(255, 255, 255), "×", g_NowStage);
    DrawFormatString(360, by, GetColor(255, 255, 255), "%d", g_player.hp);
    DrawGraph(230, by, g_HeartImage, TRUE);

    if (++g_WaitTime > 100 || g_KeyFlg & PAD_INPUT_A)
    {
        g_GameMode = 1;
        g_WaitTime = 0;
    }
}

/***********************************************
 *ゲームオーバー画面描画処理
 ***********************************************/
void DrawGameOver(void)
{
    //BackScrool();
    DrawBox(150, 150, 490, 330, 0x009900, TRUE);
    DrawBox(150, 150, 490, 330, 0x000000, FALSE);

    SetFontSize(20);
    DrawString(220, 170, "ゲームオーバー", 0xcc0000);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawStringToHandle(130, 100, "GAME   OVER", GetColor(255,0, 0), Font3);
    SetFontSize(35);

    DrawFormatString(180, 250, 0xFFFFFF, "最終階層 = %02d階", g_NowStage);

    SetFontSize(20);
    DrawString(150, 450, "---Aボタンを押してタイトルへ戻る ---", 0xffffff, 0);//zキーで戻る
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = 0;
}



/***********************************************
 * 背景画像スクロール処理
 * 引  数:なし
 * 戻り値:なし
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
//* エネミーの関数
// * 引　数：なし
// * 戻り値：なし
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
                
            if (g_player.flg == TRUE)g_AcceptKey = FALSE; //マリオみたいに一瞬止める
            g_player.flg = FALSE;

        }
    }
}

//***********************************************
//* 当たり判定の関数
// * 引　数：あり
// * 戻り値：あり
// ***********************************************/
int HitBoxPlayer(PLAYER* p, ENEMY* e)
{
    //x,yは中心座標とする
    int sx1 = p->x + 15;
    int sy1 = p->y + 30;
    int sx2 = sx1 + p->w - 30;
    int sy2 = sy1 + p->h - 40;

    int dx1 = e->x + 10;
    int dy1 = e->y + 10;
    int dx2 = dx1 + e->w - 20;
    int dy2 = dy1 + e->h - 20;

    //短形が重なっていればあたり
    if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
        return TRUE;
    }
    return FALSE;
}


/***********************************************
 * プレイヤーの処理
 * 引  数:なし
 * 戻り値:なし
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

    DrawFormatString(0, 0, 0x111FFF, "キャラ画像 = %d", g_player.img);
    DrawFormatString(0, 20, 0x111FFF, "X = %d", g_player.x);
    DrawFormatString(0, 40, 0x111FFF, "Y = %d", g_player.y);
    if (g_Blinking == TRUE)DrawGraph(g_player.x, g_player.y, g_Player[g_player.img], TRUE);
}


/***********************************************
 * 矢印の処理
 * 引  数:なし
 * 戻り値:なし
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
        DrawString(150, 400, "どれにしようかな？", 0xffffff, TRUE);
    }
    
  
    g_arrow.x = g_treasurebox[g_arrow.no].x;

    for (int i = 0; i < g_player.hp; i++)
    {
        DrawGraph(55 * i, 10, g_HeartImage, TRUE);
    }

    DrawGraph(g_arrow.x, g_arrow.y, g_Arrow, TRUE);
}

/***********************************************
 * 宝箱の表示
 * 引  数:なし
 * 戻り値:TRUE成功、FALSE失敗
 ***********************************************/
void TakaraControl()
{
    g_TakaraPosition = (640 - ((g_BoxQuantity * 60) + ((g_BoxQuantity - 1) * 20))) / 2;

    for (int i = 0; i < g_BoxQuantity; i++)
    {
        g_treasurebox[i].x = i * 80 + g_TakaraPosition;
        DrawGraph(g_treasurebox[i].x, g_treasurebox[i].y, g_TakaraBako[g_treasurebox[i].img], TRUE); //宝箱の表示
        DrawFormatString(g_treasurebox[i].x, g_treasurebox[i].y - 20, 0xFFFFFF, "%d", g_treasurebox[i].point);
    }
}

/***********************************************
 * 宝箱を開けたとき
 * 引  数:なし
 * 戻り値:なし
 ***********************************************/
void OpenTreasureBox()
{
    SetFontSize(40);

    if (g_treasurebox[g_OpenBox].point <= 0) //宝箱の中身がカギ（0）だった時
    {
        g_treasurebox[g_OpenBox].img = 1;  //からの宝箱の画像に切り替える
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);        //ブレンドモードをα(128/255)に設定
        DrawBox(180, 100, 460, 380, GetColor(255, 255, 255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        DrawGraph(180, 100, g_KeyImage, TRUE); //鍵の画像を表示させる

        DrawString(150, 400, "やったー！カギだ！", 0xffffff, TRUE);
    }

    else if (g_treasurebox[g_OpenBox].point >= 2) //宝箱の中身がミミックだった場合
    {
        if (g_treasurebox[g_OpenBox].flg == TRUE)
        {
            g_treasurebox[g_OpenBox].img = 2;  //ミミックの画像に切り替える
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);        //ブレンドモードをα(128/255)に設定
            DrawBox(180, 100, 460, 380, GetColor(255, 255, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawGraph(180, 100, g_MimicImage, TRUE);  //ミミックの画像を表示

            DrawString(160, 400, "ミッ、ミミックだ！", 0xffffff, TRUE);
        }

        else if (g_treasurebox[g_OpenBox].flg == FALSE)
        {
            DrawString(160, 400, "触るときけんだ！", 0xffffff, TRUE);
        }
    }
      if (g_treasurebox[g_OpenBox].point == 1)
      {
          if (g_treasurebox[g_OpenBox].flg == TRUE)
          {
              g_treasurebox[g_OpenBox].img = 1;  //ミミックの画像に切り替える
              SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);        //ブレンドモードをα(128/255)に設定
              DrawBox(180, 100, 460, 380, GetColor(255, 255, 255), TRUE);
              SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
              DrawGraph(180, 100, g_HeartImage1, TRUE);  //ミミックの画像を表示
              if (g_player.hp == PLAYER_HP)//HPがMAXの時に表示する
              { 
                  DrawString(230, 400, "HPがMAXだ!", 0xffffff, TRUE);
              }
              else if (g_player.hp < PLAYER_HP) //HPが減っていた時に表示する。
              { 
                  DrawString(160, 400, "HPが１回復した!", 0xffffff, TRUE);
              }
             
          }
          else if (g_treasurebox[g_OpenBox].flg == FALSE)
          {
              DrawString(180, 400, "中身がからっぽだ！", 0xffffff, TRUE);
          }
      }

    if (g_KeyFlg & PAD_INPUT_A) //取ったアイテムの画像が表示されているときZキーを押すと
    {
        if (g_treasurebox[g_OpenBox].point > 1)  //ミミックだった場合
        {
            if (g_treasurebox[g_OpenBox].flg == TRUE) g_player.hp--; //プレイヤーのHPをマイナス1する
            g_treasurebox[g_OpenBox].flg = FALSE;
            if (g_player.hp <= 0)g_GameState = 6;  //HPが0になった時ゲームオーバーにする
        }
        else if (g_treasurebox[g_OpenBox].point == 1)
        {
            if (g_treasurebox[g_OpenBox].flg == TRUE)g_player.hp++; //プレイヤーのHPをプラス１する
            if (g_player.hp > PLAYER_HP)g_player.hp = PLAYER_HP;
            g_treasurebox[g_OpenBox].flg = FALSE;
        }
        else //鍵を取った時はいろいろ初期化する
        {
            g_NowStage++; //ステージに1足す

            g_BoxQuantity = GetRand(TREASUREBOX_MAX - 2) + 2;

            StageInit();
        }

        g_AcceptKey = TRUE; //TRUEにすると宝箱を選択できるようになる
    }
}

void TimeCount(void)
{
    //制限時間を過ぎたらゲームオーバー
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

    ////UI「TIME」表示
    //SetFontSize(50);
    //DrawString(520, 40, "TIME", 0xffffff, 0);

    ////UI「SCORE」表示
    //SetFontSize(45);
    //DrawString(510, 320, "SCORE", 0xffffff, 0);
    //DrawFormatString(510, 360, 0x00ffff, "%d", g_Score);
}

/***********************************************
 * ゲームエンド描画処理
 ***********************************************/
void DrawEnd(void)
{

    //エンド画像表示
    DrawGraph(0, 0, g_EndImage, FALSE);

    //エンディング表示
    if (++g_WaitTime < 10000) g_PosY = 400 - g_WaitTime / 2;

    DrawStringToHandle(200, 100 + g_PosY, "タイトル", GetColor(255, 255, 255), Font3);
   
    DrawStringToHandle(250, 200 + g_PosY, "制作者", GetColor(255, 255, 255), Font4);

    DrawStringToHandle(150, 260 + g_PosY, "       上間　〇〇〇さん", GetColor(255, 255, 255), Font5);
    DrawFormatString(0, 0, 0x00ffff, "%d",g_WaitTime);
    if (g_WaitTime > 2300) {
        DrawStringToHandle(0, 240, "Thank you for Playing", GetColor(255, 255, 255), Font1);
    }
    DrawStringToHandle(150, 290 + g_PosY, "       神里　〇〇〇さん", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 320 + g_PosY, "       佐久本　〇〇〇さん", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 350 + g_PosY, "       川端　〇〇〇さん", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 380 + g_PosY, "       安里　〇〇〇さん", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 410 + g_PosY, "       名嘉村　〇〇〇さん", GetColor(255, 255, 255), Font5);

    //２パート
  
    //DrawStringToHandle(150, 650 + g_PosY, "   　　　 　     ", GetColor(255, 255, 255), Font01);
    DrawStringToHandle(205, 700 + g_PosY, "SE    〇〇〇", GetColor(255, 255, 255), Font5);
    //DrawStringToHandle(150, 750 + g_PosY, "    　　　　     ", GetColor(255, 255, 255), Font01);

    //タイムの加算処理＆終了
    if (++g_WaitTime > 2500)g_GameState = 99;
}

/***********************************************
 * ゲームヘルプ描画処理
 ***********************************************/
void DrawHelp(void)
{
    DrawStringToHandle(150, 150, "プレイヤー操作について", GetColor(255, 0, 0), Font4);
    DrawStringToHandle(240, 250, "宝箱を選択", GetColor(255, 0, 0), Font4);
    //スペースキーでメニューに戻る
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;
    DrawStringToHandle(190, 200, "十字キーやスティックで移動", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(100, 300, "開けたい宝箱を選択する方法は十字キーや、", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(100, 320, "スティックで取りたい宝箱を選ぶ。", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(100, 350, "開けたい宝箱の決定　Aボタン", GetColor(255, 255, 255), Font5);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);        //ブレンドモードをα(128/255)に設定
    DrawBox(50, 25, 590, 500, GetColor(255, 255, 255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawStringToHandle(150, 425, "---Aボタンでタイトルに移動---", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(150, 455, "---Bボタンでタイトルに移動---", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(200, 50, "ヘルプ画面", GetColor(255, 255, 255), Font3);

    //大きい文字見出し
    DrawStringToHandle(150, 150, "プレイヤー操作について", GetColor(255, 255, 255), Font4);
    DrawStringToHandle(300, 250, "宝箱", GetColor(255, 255, 255), Font4);
    //Aでメニューに戻る
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = 0;
    //Bボタンでゲームスタート
    if (g_KeyFlg & PAD_INPUT_B) g_GameState = 1;
    //小さい見出し
    DrawStringToHandle(250, 200, "十字キーで移動", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(100, 300, "カーソル移動　十字キーで取りたい宝箱を選ぶ", GetColor(255, 255, 255), Font5);
    DrawStringToHandle(225, 350, "宝箱の決定　Bボタン", GetColor(255, 255, 255), Font5);

    //文字の表示（点滅）
    if (++g_WaitTime < 30)
    {
        DrawStringToHandle(150, 425, "---Bボタンでゲームメインに移動---", GetColor(255, 255, 255), Font5);
    }
    else if (g_WaitTime > 60)
    {
        g_WaitTime = 0;
    }
}


/***********************************************
 * ランキング入力処理
 ***********************************************/
void InputRanking()
{
    keyboard.DrawKeyBoard();
    keyboard.KeyBoardControl(g_NowKey);
    keyboard.Push_B_Key(g_NowKey, &g_GameState, g_Score);
}
/***********************************************
 * ランキング描画処理
 ***********************************************/
void DrawRanking()
{
    // Bボタン or Xキー でタイトルに戻る
    if (g_KeyFlg & PAD_INPUT_B) g_GameState = 0;

    //ランキング画像表示
    DrawGraph(0, 0, g_RankingImage, FALSE);

    ranking.DrawRanking();

    // 文字の表示(点滅)
    if (++g_WaitTime < 30)
    {
        SetFontSize(24);
        DrawString(150, 450, "--  Bボタンを押して戻る  --", 0xffffed);
    }
    else if (g_WaitTime > 60) g_WaitTime = 0;
}

/***********************************************
 * 画像読み込み
 ***********************************************/
int LoadImages()
{
    //タイトル タイトル画像替えました。
    if ((g_TitleImage = LoadGraph("images/Title.png")) == -1) return -1;

    //メニュー
   // if ((g_Applec = LoadGraph("images/Applec.png")) == -1) return -1;

    //ステージ背景
    if ((g_StageImage = LoadGraph("images/haikei1.png")) == -1)return -1;
    if ((g_DrawStageImages = LoadGraph("images/doukutu.png")) == -1)return -1;
    if ((g_DrawStageImages1 = LoadGraph("images/doukutu1 .png")) == -1)return -1;

    //廊下の画像
    if ((g_RoadImage = LoadGraph("images/road7.png")) == -1)return -1;
    if ((g_RoadImage2 = LoadGraph("images/road8.png")) == -1)return -1;

    //エンド画面背景
    if ((g_EndImage = LoadGraph("images/EndImage.png")) == -1)return -1;
    //ランキング背景
    if ((g_RankingImage = LoadGraph("images/rank_back.png")) == -1) return -1;
    //宝箱の画像
    if (LoadDivGraph("images/takarabako.png", 3, 3, 1, 60, 60, g_TakaraBako) == -1) return -1;
    //敵１の画像
    if (LoadDivGraph("images/marimo.png", 6, 3, 2, 50, 50, g_EnemyImage[0]) == -1) return -1;
    if (LoadDivGraph("images/koumori.png", 6, 3, 2, 50, 50, g_EnemyImage[1]) == -1) return -1;
    if (LoadDivGraph("images/obake.png", 6, 3, 2, 57, 60, g_EnemyImage[2]) == -1) return -1;
    //鍵の画像
    if ((g_KeyImage = LoadGraph("images/Key.png")) == -1)return -1;
    //ミミックの画像
    if ((g_MimicImage = LoadGraph("images/Mimic.png")) == -1)return -1;
    //ハートの画像
    if ((g_HeartImage = LoadGraph("images/heart.png")) == -1)return -1;
    if ((g_HeartImage1 = LoadGraph("images/HP.png")) == -1)return -1;


    //キーボード諸々
    if (keyboard.LoadImgae() == -1) return -1;

    //プレイヤー矢印画像
    if ((g_Arrow = LoadGraph("images/Arrow.png")) == -1)return -1;

    //ヘルプ画面
    if ((g_HelpImage = LoadGraph("images/Help.png")) == -1)return -1;
    //プレイヤー画像
    if (LoadDivGraph("images/player.png", 16, 4, 4, 70, 90, g_Player) == -1) return -1;

    return 0;
}

/***********************************************
 * 音声読み込み
 ***********************************************/
int LoadSounds()
{
    //タイトル タイトル画像替えました。
    //if ((s_TitleBGM = LoadSoundMem("BGM/see.mp3")) == -1) return -1;

    //キーボード
    if (keyboard.LoadSounds() == -1) return -1;

    ////メニュー
    //if ((g_Applec = LoadGraph("images/Applec.png")) == -1) return -1;

    ////ステージ背景
    //if ((g_StageImage = LoadGraph("images/haikei.png")) == -1)return -1;

    //廊下の画像
    if ((s_RoadBGM = LoadSoundMem("BGM/Flutter.mp3")) == -1) return -1;

    ////エンド画面背景
    //if ((g_EndImage = LoadGraph("images/EndImage.png")) == -1)return -1;
    ////宝箱の画像
    //if (LoadDivGraph("images/takarabako.png", 3, 3, 1, 60, 60, g_TakaraBako) == -1) return -1;

    ////鍵の画像
    //if ((g_KeyImage = LoadGraph("images/Key.png")) == -1)return -1;
    ////ミミックの画像
    //if ((g_MimicImage = LoadGraph("images/Mimic.png")) == -1)return -1;
    ////ハートの画像
    //if ((g_HeartImage = LoadGraph("images/heart.png")) == -1)return -1;

    ////キーボード諸々
    //if (keyboard.LoadImgae() == -1) return -1;

    ////プレイヤー矢印画像
    //if ((g_Arrow = LoadGraph("images/Arrow.png")) == -1)return -1;

    ////ヘルプ画面
    //if ((g_HelpImage = LoadGraph("images/Help.png")) == -1)return -1;
    ////プレイヤー画像
    //if (LoadDivGraph("images/player.png", 16, 4, 4, 70, 90, g_Player) == -1) return -1;

    return 0;
}