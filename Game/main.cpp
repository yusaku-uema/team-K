/********************************************************************
**   ゲームジャム
** 　宝箱をアケルゲーム
********************************************************************/
#include"DxLib.h"
#include<math.h>
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

/***********************************************
 * 変数の宣言
 ***********************************************/
int g_OldKey;  // 前回の入力キー 
int g_NowKey;  // 今回の入力キー 
int g_KeyFlg;  // 入力キー情報 
int g_GameState = 0;  // ゲームモード 
int g_TitleImage; // 画像用変数 
int g_Menu; //メニュー画面
int g_Score = 0; //スコア
int g_WaitTime = 0; //待ち時間
int g_StartTime;   // スタート時間
int Time;   // 現在時間

int g_TakaraBako[4]; //宝箱の画像
int g_Arrow;  //プレイヤーの矢印の画像
int g_Applec; //タイトルカーソル変数　消さないで

int g_StageImage;

//ステック
int AX, AY;
struct DINPUT_JOYSTATE
{
    int		X;		// スティックのＸ軸パラメータ( -1000～1000 )
    int		Y;		// スティックのＹ軸パラメータ( -1000～1000 )
    int		Z;		// スティックのＺ軸パラメータ( -1000～1000 )
    int		Rx;		// スティックのＸ軸回転パラメータ( -1000～1000 )
    int		Ry;		// スティックのＹ軸回転パラメータ( -1000～1000 )
    int		Rz;		// スティックのＺ軸回転パラメータ( -1000～1000 )
    int		Slider[2];	// スライダー二つ
    unsigned int	POV[4];	// ハットスイッチ４つ
                    // ( 0xffffffff:入力なし 0:上 4500:右上 9000:右 13500:右下
                    //		 18000:下 22500:左下 27000:左 31500:左上 )
    unsigned char	Buttons[32];	// ボタン３２個( 押されたボタンは 128 になる )
};


//プレイヤー矢印の構造体
struct ARROW
{
    int x, y;      //座標x,y
    int no;
    int oldkey;
};
//プレイヤー
struct ARROW g_arrow;


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
void DrawGameTitle(void); //タイトル描画処理
void DrawGameOver(void); //ゲームオーバ
int LoadImages(); //画像読み込み
void UIView();
void TimeCount();
void BackScrool(); //背景画像スクロール処理
void PlayerControl(); //プレイヤーの処理
void TakaraControl(); //宝箱の処理

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

    if (LoadImages() == -1) return -1; //画像読込み関数を呼び出し

      //ゲームループ 
    while (ProcessMessage() == 0 && g_GameState != 99) {
        //キー入力取得 
        g_OldKey = g_NowKey;
        g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1); //ゲームパッドを持ってきていない人用に後でゲームパッドのみに変更
        g_KeyFlg = g_NowKey & ~g_OldKey;

        //左右のアナログ入力状態を取得する
        GetJoypadAnalogInput(&AX, &AY, DX_INPUT_PAD1);

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
    //static int MenuNo = 0;

    ////メニューカーソル移動処理
    //if (g_KeyFlg & PAD_INPUT_DOWN) {
    //    if (++MenuNo > 3)MenuNo = 0;
    //}
    //if (g_KeyFlg & PAD_INPUT_UP) {
    //    if (--MenuNo < 0)MenuNo = 3;
    //}
    ////zキー、Aボタンでメニュー選択
    //if (g_KeyFlg & PAD_INPUT_A && MenuNo == 0)g_GameState = MenuNo + 1; //作ってないからスタート以外押せません
    // //メニュー
    //DrawGraph(310, 220 + MenuNo * 50, g_Applec, TRUE);

    //タイトル画像表示
    DrawGraph(0, 0, g_TitleImage, FALSE);

    SetFontSize(20);
    DrawString(150, 200, "---ZキーやAボタンを押してゲームスタート---", 0xffffff, 0);
    if (g_KeyFlg & PAD_INPUT_A) {
        g_GameState = 1;
    }
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

    //現在の経過時間を得る
    g_StartTime = GetNowCount();

    //ゲームメイン処理へ
    g_GameState = 5;
}

/***********************************************
 * ゲームメイン
 ***********************************************/
void GameMain(void)
{
    BackScrool();
    PlayerControl();
    TakaraControl();
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
    BackScrool();

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
 * 背景画像スクロール処理
 * 引  数:なし
 * 戻り値:なし
 ***********************************************/
void BackScrool()
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
            if (g_takara[i].point <= 0) g_takara[i].img = 2;
            else if (g_takara[i].point > 0)g_takara[i].img = 3;
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
 * 画像読み込み
 ***********************************************/
int LoadImages()
{
    //タイトル タイトル画像替えました。
    if ((g_TitleImage = LoadGraph("images/Title.png")) == -1) return -1;

    //メニュー
    if ((g_Applec = LoadGraph("images/Applec.png")) == -1) return -1;

    //ステージ背景
    if ((g_StageImage = LoadGraph("images/haikei.png")) == -1)return -1;

    //宝箱の画像
    if (LoadDivGraph("images/TakaraBako.png", 4, 2, 2, 60, 60, g_TakaraBako) == -1) return -1;

    //プレイヤー矢印画像
    if ((g_Arrow = LoadGraph("images/Arrow.png")) == -1)return -1;

    return 0;
}