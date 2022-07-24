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
//ランキング表示数
const int MAX_RANKING_DATA = 5;
/***********************************************
 * 変数の宣言
 ***********************************************/
int g_OldKey;  // 前回の入力キー 
int g_NowKey;  // 今回の入力キー 
int g_KeyFlg;  // 入力キー情報 

int g_GameState = 0;  // ゲームモード 
int g_TitleImage; // 画像用変数 
int g_Menu; //メニュー画面
int	g_MenuNumber = 0;		// メニューカーソル位置
int g_MenuY;				// メニューカーソルのＹ座標

int g_Score = 0; //スコア

int g_WaitTime = 0; //待ち時間
int g_StartTime;   // スタート時間
int Time;   // 現在時間

int g_TakaraBako[4]; //宝箱の画像
int g_Arrow;  //プレイヤーの矢印の画像
int g_Applec; //タイトルカーソル変数　消さないで

int g_StageImage;   //ゲームメイン背景

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

//ランキングデータ 構造体
struct RankingData
{
    int no;          //順位
    char name[10];   //name は10byte(9文字)
    long score;      //スコア
};
//ランキングデータ
RankingData g_Ranking[MAX_RANKING_DATA];

/***********************************************
 * 関数のプロトタイプ宣言
 ***********************************************/
void GameInit(void); //ゲーム初期処理
void GameMain(void); //ゲームメイン処理
void DrawEnd(void); //ゲームエンド
void DrawHelp(void); //ヘルプ画面
void DrawGameTitle(void); //タイトル描画処理
void DrawGameOver(void); //ゲームオーバ

void DrawRanking(void);  //ランキング表示画面
int ReadRanking();       //ランキング読み込み
int SaveRanking();       //ランキング保存
void SortRanking();      //ランキング並べ替え

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
    if (ReadRanking() == -1) return -1;//ランキング読込み関数を呼び出し

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
  
    //メニューカーソル移動処理
    if (g_KeyFlg & PAD_INPUT_DOWN) {
        if (++g_MenuNumber > 3) g_MenuNumber = 0;
    }
    if (g_KeyFlg & PAD_INPUT_UP) {
        if (--g_MenuNumber < 0) g_MenuNumber = 2;
    }

    //Ｚキーでメニュー選択
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = g_MenuNumber + 1;

    //タイトル画像表示
    DrawGraph(0, 0, g_TitleImage, FALSE);

    //メニューカーソル（三角形）の表示
    g_MenuY = g_MenuNumber * 52;
    DrawTriangle(240, 255 + g_MenuY, 260, 270 + g_MenuY, 240, 285 + g_MenuY, GetColor(255, 0, 0), TRUE);

    SetFontSize(40);
    //ゲームタイトルを載せる予定
    DrawString(100, 0, "ゲームタイトル決定後反映", 0xffffff, 0);

    //ステージ選択
    SetFontSize(30);
    DrawString(265, 255, "ゲームスタート", 0xffffff, 0);
    DrawString(265, 307, "ランキング", 0xffffff, 0);
    DrawString(265, 359, "ヘルプ", 0xffffff, 0);
    DrawString(265, 411, "エンド", 0xffffff, 0);

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
 * ゲームエンド描画処理
 ***********************************************/
void DrawEnd(void)
{
    ////エンド画像表示予定
    //DrawGraph(0, 0, g_EndImage, FALSE);

    SetFontSize(40);
    DrawString(100, 255,"Thank you for Playing", 0xffffff, 0);

    //タイムの加算処理＆終了（3秒後）
    if (++g_WaitTime > 180)g_GameState = 99;
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
 * ランキング描画処理
 ***********************************************/
void DrawRanking(void)
{
    //スペースキーでメニューに戻る
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;

    //ランキング画像表示
    //DrawGraph(0, 0, g_RankingImage, FALSE);

    // ランキング一覧を表示
    SetFontSize(30);
    DrawString(50, 100, "ランキング画面仮", 0xffffff);
    for (int i = 0; i < MAX_RANKING_DATA; i++)
    {
        DrawFormatString(50, 170 + i * 35, 0xffffff, "%2d   %10s     %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
    }

    // 文字の表示(点滅)
    if (++g_WaitTime < 30)
    {
        SetFontSize(24);
        DrawString(60, 450, "--  スペースキーを押してタイトルへ戻る  --", 0xffffff);
    }
    else if (g_WaitTime > 60) g_WaitTime = 0; //メニューへ
}

/***********************************************
 * ランキング読み込み
 ***********************************************/
int ReadRanking()
{
    //ファイルポインタ
    FILE* fp;

    //ファイルオープン
    if ((fopen_s(&fp, "dat/rankingdata.txt", "r")) != 0)
    {
        //エラー処理
        printf("Ranking Data Error\n");
        return -1;
    }

    //ランキングデータ配分列データを読み込む
    for (int i = 0; i < MAX_RANKING_DATA; i++)
    {
        fscanf_s(fp, "%2d %10s %10d", &g_Ranking[i].no, g_Ranking[i].name, 10, &g_Ranking[i].score);
    }

    //ファイルクローズ
    fclose(fp);

    return 0;
}

/***********************************************
 * ランキング保存
 ***********************************************/
int SaveRanking()
{
    //ファイルポインタ
    FILE* fp;

    //ファイルオープン
    if ((fopen_s(&fp, "dat/rankingdata.txt", "w")) != 0)
    {
        //エラー処理
        printf("Ranking Data Error\n");
        return -1;
    }

    // ランキングデータ分配列データを書き込む
    for (int i = 0; i < MAX_RANKING_DATA; i++)
    {
        fprintf(fp, "%2d %10s %10d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
    }

    //ファイルクローズ
    fclose(fp);

    return 0;
}

/***********************************************
 * ランキング並べ替え
 ***********************************************/
void SortRanking()
{
    //並べ替え用
    RankingData work;
    int i, j;

    //選択法ソート
    for (i = 0; i < MAX_RANKING_DATA - 1; i++)
    {
        for (j = i + 1; j < MAX_RANKING_DATA; j++)
        {
            if (g_Ranking[i].score <= g_Ranking[j].score)
            {
                work = g_Ranking[i];
                g_Ranking[i] = g_Ranking[j];
                g_Ranking[j] = work;
            }
        }
    }

    // 順位付け
    for (i = 0; i < MAX_RANKING_DATA; i++)
    {
        g_Ranking[i].no = 1;
    }

    // 得点が同じ場合は、同じ順位とする
    // 同順位があった場合の次の順位はデータ個数が加算された順位とする
    for (i = 0; i < MAX_RANKING_DATA - 1; i++)
    {
        for (j = i + 1; j < MAX_RANKING_DATA; j++)
        {
            if (g_Ranking[i].score > g_Ranking[j].score)
            {
                g_Ranking[j].no++;
            }
        }
    }
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