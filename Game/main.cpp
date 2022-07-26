/********************************************************************
**   ゲームジャム
** 　宝箱をアケルゲーム
********************************************************************/
#include"DxLib.h"
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
const int ENEMY_MAX = 6;
const int TAKARA_TIME = 100;

//プレイヤーのやつ
const int PLAYER_SPEED = 3;
const int PLAYER_IMAGE_TIME = 8;

//背景のやつ
const int STAGE_NO = 1; //背景の廊下の長さ

/***********************************************
 * 変数の宣言
 ***********************************************/
int g_OldKey;  // 前回の入力キー 
int g_NowKey;  // 今回の入力キー 
int g_KeyFlg;  // 入力キー情報
int Font,Font1; //フォント
int g_Player[16];

int g_GameState = 0;  // ゲームモード 

int g_GameMode = 0; //ゲームモード（神里が作った）

int g_TitleImage; // 画像用変数 
int g_Menu; //メニュー画面
int	g_MenuNumber = 0;		// メニューカーソル位置
int g_Score = 0; //スコア

int g_WaitTime = 0; //待ち時間
int g_StartTime;   // スタート時間
int Time;   // 現在時間

int g_TakaraBako[4]; //宝箱の画像
int g_Arrow;  //プレイヤーの矢印の画像
int g_cursor; //タイトルカーソル変数　

int g_StageImage;   //ゲームメイン背景

int g_RoadImage;
int g_RoadImage2;
int g_kakusibeya;

//プレイヤー矢印の構造体
struct ARROW
{
    int x, y;      //座標x,y
    int no;
    int oldkey;
};
//プレイヤー
struct ARROW g_arrow;

//プレイヤーの構造体
struct PLAYER
{
    int x, y;      //座標x,y
    int img;
    int imgtime;
};
//プレイヤー
struct PLAYER g_player;

//廊下の背景画像の構造体（神里が使う）
struct STAGE
{
    int no;
    int img;
    int y;
    int x;
};
struct STAGE g_stage;


//宝箱の構造体
struct TAKARA {
    int flg; //使用フラグ
    int type; //タイプ
    int img; //画像
    int x, y; //座標x,y,幅w,高さh
    int point; //宝の得点
    int time;
};
//敵機
struct TAKARA g_takara[ENEMY_MAX];
struct TAKARA g_enemy00 = { TRUE,0,0,30,200,0, TAKARA_TIME };

/***********************************************
 * 関数のプロトタイプ宣言
 ***********************************************/
void GameInit(void); //ゲーム初期処理
void GameMain(void); //ゲームメイン処理
void DrawEnd(void); //ゲームエンド
void DrawHelp(void); //ヘルプ画面
void DrawGameTitle(void); //タイトル描画処理
void DrawGameOver(void); //ゲームオーバ
int LoadImages(); //画像読み込み
void UIView();
void TimeCount();
void BackScrool(int a); //背景画像スクロール処理
void BackImage();
void PlayerControl(); //プレイヤーの処理
void ArrowControl();  //プレイヤーの矢印の処理
void TakaraControl(); //宝箱の処理

void InputRanking();  //ランキング入力
void DrawRanking();   //ランキング描画
/***********************************************
 * プログラムの開始
 **********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    //タイトルを設定
    SetMainWindowText("宝箱をあけろ");
    //ウィンドウモードで起動する 
    ChangeWindowMode(TRUE);

    // DX ライブラリの初期化処理:エラーが起きたら直ちに終了 
    if (DxLib_Init() == -1) return -1;

    //描画先画面を裏にする 
    SetDrawScreen(DX_SCREEN_BACK);

    Font = CreateFontToHandle("メイリオ", 30, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);//"メイリオ"  の30pt,太さ3のフォントを作成 
    Font1= CreateFontToHandle("メイリオ", 50, 14, DX_FONTTYPE_ANTIALIASING_EDGE);
    if (LoadImages() == -1) return -1; //画像読込み関数を呼び出し
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

    //メニューカーソル移動処理
    if (g_KeyFlg & PAD_INPUT_DOWN) {
        if (++g_MenuNumber > 3) g_MenuNumber = 0;
    }
    if (g_KeyFlg & PAD_INPUT_UP) {
        if (--g_MenuNumber < 0) g_MenuNumber = 3;
    }

    //Ｚキーでメニュー選択
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = g_MenuNumber + 1;

    //タイトル画像表示
    DrawGraph(0, 0, g_TitleImage, FALSE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(160, 255, 480, 450, GetColor(255,255,255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //メニューカーソル（三角形）の表示
    if (g_MenuNumber == 0 || g_MenuNumber == 1) {
        DrawGraph(160, 255 + g_MenuNumber * 50, g_cursor, TRUE);
    }
    if (g_MenuNumber == 2 || g_MenuNumber == 3) {
        DrawGraph(225, 255 + g_MenuNumber * 50, g_cursor, TRUE);
    }

    SetFontSize(40);
    //ゲームタイトルを載せる予定
    DrawStringToHandle(100, 130, "ゲームタイトル予定", GetColor(255, 255, 255), Font1);
    
    DrawStringToHandle(210, 255, "ゲームスタート", GetColor(255, 0, 0), Font);
    DrawStringToHandle(210, 305, "ランキング", GetColor(255, 0, 0), Font);
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

    //エネミーの初期処理
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_takara[i] = g_enemy00;
        g_takara[i].point = GetRand(5) * 1000;
    }

    //プレイヤー矢印の初期処理
    g_arrow.y = g_takara[g_arrow.no].y - 100;
    g_arrow.x = g_takara[g_arrow.no].x;
    g_arrow.no = 0;

    //プレイヤー初期処理
    g_player = { 139, 200, 13, PLAYER_IMAGE_TIME };

    //背景画像(廊下）の初期処理
    g_stage = { STAGE_NO, g_RoadImage, 0, 0 };

    g_GameMode = 0;

    //現在の経過時間を得る
    g_StartTime = GetNowCount();

    //キーボード（ランキング入力）初期処理
    keyboard.KeyBoardInit();

    //ゲームメイン処理へ
    g_GameState = 5;
}

/***********************************************
 * ゲームメイン
 ***********************************************/
void GameMain(void)
{
    switch (g_GameMode) {
    case 0:
        BackScrool(0);
        PlayerControl();
        break;
    case 1:
        BackImage();
        TakaraControl();
        ArrowControl();
        break;
    }
    
    UIView();
    TimeCount();

    //スペースキーでメニューに戻る　ゲームメインからタイトルに戻る追加
    if (g_KeyFlg & PAD_INPUT_M)g_GameState = 6;
    SetFontSize(16);
    DrawString(150, 450, "---スペースキーを押してゲームオーバーへ---", 0xffffff, 0);
}

/***********************************************
 *ゲームオーバー画面描画処理
 ***********************************************/
void DrawGameOver(void)
{
    //BackScrool();

    //スペースキーでメニューに戻る
    if (g_KeyFlg & PAD_INPUT_M)   g_GameState = 0;

    DrawBox(150, 150, 490, 330, 0x009900, TRUE);
    DrawBox(150, 150, 490, 330, 0x000000, FALSE);

    SetFontSize(20);
    DrawString(220, 170, "ゲームオーバー", 0xcc0000);
    SetFontSize(35);

    DrawFormatString(180, 250, 0xFFFFFF, "スコア = %6d", g_Score);

    SetFontSize(20);
    DrawString(150, 450, "---スペースキーを押してタイトルへ戻る ---", 0xffffff, 0);
}



/***********************************************
 * 背景画像スクロール処理(廊下）
 * 引  数:あり
 * 戻り値:なし
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
 * プレイヤーの処理
 * 引  数:なし
 * 戻り値:なし
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
    if (g_player.y < 60)g_GameMode = 1;

    DrawFormatString(0, 0, 0x111FFF, "キャラ画像 = %d", g_player.img);
    DrawFormatString(0, 20, 0x111FFF, "X = %d", g_player.x);
    DrawFormatString(0, 40, 0x111FFF, "Y = %d", g_player.y);
    DrawGraph(g_player.x, g_player.y, g_Player[g_player.img], TRUE);
}


/***********************************************
 * 矢印の処理
 * 引  数:なし
 * 戻り値:なし
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
 * 宝箱の生成
 * 引  数:なし
 * 戻り値:TRUE成功、FALSE失敗
 ***********************************************/
void TakaraControl()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_takara[i].x = i * 70 + 30;
        DrawGraph(g_takara[i].x, g_takara[i].y, g_TakaraBako[g_takara[i].img], TRUE); //敵の表示


        if (g_takara[i].flg == FALSE)
        {
            if (g_takara[i].point <= 0) g_takara[i].img = 1;
            else if (g_takara[i].point > 0)g_takara[i].img = 2;
            g_takara[i].time--;
            DrawFormatString(g_takara[i].x, g_takara[i].y - 60, 0x00ffff, "%d点", g_takara[i].point);

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
    //UI「TIME」表示
    SetFontSize(50);
    DrawString(520, 40, "TIME", 0xffffff, 0);

    //UI「SCORE」表示
    SetFontSize(45);
    DrawString(510, 320, "SCORE", 0xffffff, 0);
    DrawFormatString(510, 360, 0x00ffff, "%d", g_Score);
}

/***********************************************
 * ゲームエンド描画処理
 ***********************************************/
void DrawEnd(void)
{
    //エンド画像表示予定
    DrawGraph(0, 0, g_kakusibeya, FALSE);

    // ********** フォントのロード **********
    LPCSTR font_path = "azukiP.ttf"; // 読み込むフォントファイルのパス
    if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
    }
    else {
        // フォント読込エラー処理
        MessageBox(NULL, "フォント読込失敗", "", MB_OK);
    }

    SetFontSize(40);
    ChangeFont("あずきフォントP", DX_CHARSET_DEFAULT);
    /*DrawString(100, 255,"Thank you for Playing", 0xffffff, 0);*/
    DrawString(255, 30, "チームK", 0xffffff, 0);
    DrawString(100, 150, "安里和也", 0xffffff, 0);
    DrawString(100, 100, "名嘉村亜澄真", 0xffffff, 0);
    DrawString(400, 100, "川畑勇快", 0xffffff, 0);
    DrawString(400, 150, "神里晃汰", 0xffffff, 0);
    DrawString(100, 200, "上間優咲", 0xffffff, 0);
    DrawString(400, 200, "佐久本盛杏", 0xffffff, 0);
    DrawString(220, 255, "フリー素材", 0xffffff, 0);
    DrawString(250, 300, "?", 0xffffff, 0);
    DrawString(310, 300, "?", 0xffffff, 0);
    DrawString(370, 300, "?", 0xffffff, 0);
    DrawString(370, 350, "?", 0xffffff, 0);
    DrawString(250, 350, "?", 0xffffff, 0);
    DrawString(310, 350, "?", 0xffffff, 0);


    // ********** フォントのアンロード **********
    if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
    }
    else {
        MessageBox(NULL, "remove failure", "", MB_OK);
    }
    

    //タイムの加算処理＆終了（3秒後）
    if (++g_WaitTime > 300)g_GameState = 99;
}

/***********************************************
 * ゲームヘルプ描画処理
 ***********************************************/
void DrawHelp(void)
{
    //スペースキーでメニューに戻る
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;

    //タイトル画像表示
    //DrawGraph(0, 0, g_TitleImage, FALSE);
    SetFontSize(20);
    DrawString(100, 120, "ヘルプ画面仮", 0xffffff, 0);
    DrawString(100, 255, "スペースキーを押してタイトルへ戻る ", 0xffffff, 0);
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
    //DrawGraph(0, 0, g_RankingImage, FALSE);

    ranking.DrawRanking();

    // 文字の表示(点滅)
    if (++g_WaitTime < 30)
    {
        SetFontSize(24);
        DrawString(150, 450, "--  Press B button or X Key  --", 0xFF0000);
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
    if ((g_cursor = LoadGraph("images/cursor.png")) == -1) return -1;

    //ステージ背景
    if ((g_StageImage = LoadGraph("images/haikei.png")) == -1)return -1;
    
    //廊下の画像
    if ((g_RoadImage = LoadGraph("images/road3.png")) == -1)return -1;
    if ((g_RoadImage2 = LoadGraph("images/road4.png")) == -1)return -1;

    //宝箱の画像
    if (LoadDivGraph("images/TakaraBako.png", 3, 3, 1, 60, 60, g_TakaraBako) == -1) return -1;

    //プレイヤー矢印画像
    if ((g_Arrow = LoadGraph("images/Arrow.png")) == -1)return -1;

    //プレイヤー画像
    if (LoadDivGraph("images/player.png", 16, 4, 4, 70, 90, g_Player) == -1) return -1;

    //エンド画像
    if ((g_kakusibeya = LoadGraph("images/kakusibeya.png")) == -1)return -1;

    //キーボード諸々
    if (keyboard.LoadImgae() == -1) return -1;

    return 0;
}