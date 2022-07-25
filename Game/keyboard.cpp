#include"DxLib.h"
#include"keyboard.h"
//#include"ranking.h"

KeyBoard keyboard;

#define ALPHA_MAX 26      //アルファベット総数

#define OUT_WIDTH 45      //画面左端～キーボードまでの幅
#define OUT_HEIGHT 200    //画面上端～キーボードまでの高さ
#define SPACE 10          //キー間のスペース
#define KEY_WIDTH 40      //ノーマルキーの幅

const int CurStdX = OUT_WIDTH + SPACE + 5;    //カーソルのX座標基準    (「5」はカーソルの幅 ）
const int CurStdY = OUT_HEIGHT + SPACE + 5;   //カーソルのY座標基準    

//入力文字    ※わかりやすくするため半分（実際に表示されているキーボードと同じ配置）
const char AllStr[5][ALPHA_MAX / 2 + 1] = {
	"ABCDEFGHIJKLM",
	"NOPQRSTUVWXYZ",
	"abcdefghijklm",
	"nopqrstuvwxyz",
	"0123456789"
};

//初期処理
void KeyBoard::KeyBoardInit()
{
	//移動量
	movekeyX = 0;
	movekeyY = 0;

	//カーソルの初期位置は「A」
	PUSH_NOW = KEY_TYPE::NORMAL;

	//入力文字列　初期化
	for (int i = 0; i < 10; i++)
	{
		InputName[i] = 0;
	}
	InputName[9] = '\0';

	namePos = -1;

	//削除・確定フラグ
	PushFlg = FALSE;
}

//画像読み込み
int KeyBoard::LoadImgae()
{
	//背景
	if ((backimage = LoadGraph("images/KeyBoard/back02.png")) == -1) return -1;
	//キーボード
	if ((keyboardimage = LoadGraph("images/KeyBoard/OPEN_Board4.png")) == -1) return -1;

	//分割読み込み　押された/押してない が連結した画像

	//ノーマルカーソル
	if ((LoadDivGraph("images/KeyBoard/N_Link_Normal.png", 2, 2, 1, 40, 40, Cursorimage)) == -1) return -1;

	//「×」カーソル
	if ((LoadDivGraph("images/KeyBoard/N_Link_Cancel.png", 2, 2, 1, 70, 40, Cancelimage)) == -1) return -1;

	//「OK」カーソル
	if ((LoadDivGraph("images/KeyBoard/N_Link_Space.png", 2, 2, 1, 200, 40, OKimage)) == -1) return -1;
}

//描画
void KeyBoard::DrawKeyBoard()
{
	//背景
	DrawGraph(0, 0, backimage, FALSE);
	//キーボード
	DrawGraph(45, OUT_HEIGHT, keyboardimage, TRUE);

	//test
	//DrawTestInfo();

	DrawInputInfo();

	// ノーマル(A～Z,a～z,0～9)・「×」・「ＯＫ」によって画像変化　 switch文で操作
	// 　　　　　　　　押す・押さないによって画像変化　　　　　　　 画像配列を PushFlg で操作
	switch (PUSH_NOW)
	{
	case KEY_TYPE::NORMAL:    //ノーマルカーソル
		DrawGraph(CurStdX + KEY_WIDTH * movekeyX, CurStdY + KEY_WIDTH * movekeyY, Cursorimage[PushFlg], TRUE);
		break;

	case KEY_TYPE::CANCEL:    //「×」カーソル
		DrawGraph(CurStdX + KEY_WIDTH * movekeyX + 20, CurStdY + KEY_WIDTH * movekeyY, Cancelimage[PushFlg], TRUE);
		break;

	case KEY_TYPE::DONE:      //「OK」カーソル
		DrawGraph(CurStdX + KEY_WIDTH * movekeyX, CurStdY + KEY_WIDTH * movekeyY, OKimage[PushFlg], TRUE);
		break;

	default:
		break;
	}
}


//移動
void KeyBoard::KeyBoardControl(int NowKey)
{

	//フレーム数カウント
	frame++;

	//→ 右 
	if (NowKey & PAD_INPUT_RIGHT)
	{
		if (InputControl() == 0)
		{
			movekeyX++;     //タイミング調整 + 移動
		}
		if (movekeyX >= 12) movekeyX = 12;   //右端でストップ

		PUSH_NOW = KEY_TYPE::NORMAL;         //現在のキーはノーマル
	}

	//← 左
	if (NowKey & PAD_INPUT_LEFT)
	{
		if (InputControl() == 0)
		{
			movekeyX--;     //タイミング調整 + 移動
		}

		if (movekeyX <= 0) movekeyX = 0;     //左端でストップ

		PUSH_NOW = KEY_TYPE::NORMAL;         //現在のキーはノーマル
	}

	//↑ 上
	if (NowKey & PAD_INPUT_UP)
	{

		if (InputControl() == 0)
		{
			movekeyY--;     //タイミング調整 + 移動
		}
		if (movekeyY <= 0) movekeyY = 0;     //上端でストップ

		PUSH_NOW = KEY_TYPE::NORMAL;         //現在のキーはノーマル
	}

	//↓ 下
	if (NowKey & PAD_INPUT_DOWN)
	{

		if (InputControl() == 0)
		{
			movekeyY++;     //タイミング調整 + 移動
		}

		PUSH_NOW = KEY_TYPE::NORMAL;         //現在のキーはノーマル
	}

	//「×」ボタン   「a～z」段より下 かつ 「9」キーより右側
	if (movekeyY == 4 && movekeyX >= 10)
	{
		movekeyX = 10;                       //ボタンの位置

		PUSH_NOW = KEY_TYPE::CANCEL;         //現在のキーはキャンセル「×」
	}

	//「OK」ボタン   キーボード最下段より下
	if (movekeyY >= 5)
	{
		movekeyX = 4;                        //ボタンの位置
		movekeyY = 5;

		PUSH_NOW = KEY_TYPE::DONE;           //現在のキーはDONE「OK」
	}

	//Bボタンで確定
	//testPush_B_Key2(NowKey);
}

//Bボタンが押された時の処理 テスト2
void KeyBoard::Push_B_Key(int NowKey, int* GameState, int Score)
{
	//"押した瞬間"を判定
	static int push_B;

	if (NowKey & PAD_INPUT_B)
	{
		push_B++;

		if (PUSH_NOW == KEY_TYPE::NORMAL)
		{
			//"押した瞬間"のみ入力
			if (push_B >= 1 && push_B < 8)
			{
				//B押されたよフラグ　TRUE
				PushFlg = TRUE;

				//入力タイミング
				if (InputControl() == 0)
				{
					++namePos;
					//上限は9文字
					if (namePos >= 8) namePos = 8;

					//配列に入力
					InputName[namePos] = AllStr[movekeyY][movekeyX];
				}
			}
		}
		else if (PUSH_NOW == KEY_TYPE::CANCEL) //「×」キー上で押して一文字削除
		{
			//B押されたよフラグ　TRUE
			PushFlg = TRUE;

			//入力タイミング
			if (InputControl() == 0)
			{
				InputName[namePos] = 0;

				//０文字以下にはならない
				if (namePos >= 0) namePos--;
			}
		}
		else if (PUSH_NOW == KEY_TYPE::DONE)   //「OK」キー上で押して確定（ランキング変数に代入）
		{
			//B押されたよフラグ　TRUE
			PushFlg = TRUE;

			//入力タイミング
			if (InputControl() == 0)
			{
				//未入力での確定は不可
				if (InputName[0] != '\0')
				{
					InputName[namePos + 1] = '\0';

					//ランキングname変数=InputName;
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

//入力情報表示
void KeyBoard::DrawInputInfo()
{
	//if (InputName[0] == 0) InputName[0] = '|';

	if (InputName[0] == 0)
	{
		//SetDrawBlendMode
		SetFontSize(20);
		DrawString(200, 125, "・ ・ 名前を入力 ・ ・", 0xffffff);
	}

	for (int i = 0; InputName[i] != '\0'; i++)
	{
		SetFontSize(30);

		DrawFormatString(220 + 20 * i, 120, 0xffffff, " %c", InputName[i]);
	}
}

//コントローラー調整
int KeyBoard::InputControl()
{
	int timing = 8;
	return frame % timing;
}


//＊test用＊　各種情報表示
void KeyBoard::DrawTestInfo()
{
	//InputNameチェック
	//if (InputName[0] == 0) InputName[0] = '|';
	for (int i = 0; InputName[i] != '\0'; i++)
	{
		SetFontSize(20);
		DrawString(0, 0, "NAME : ", 0xffffff);
		DrawFormatString(80 + 15 * i, 0, 0xffffff, "%c", InputName[i]);
	}


	//PushFlgチェック
	DrawFormatString(0, 30, 0xffffff, "Push : %s", PushFlg == FALSE ? "押されてない" : "押されてる");
}
