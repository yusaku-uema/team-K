#pragma once

//キーの種類
enum class KEY_TYPE
{
	NONE = 0,   // 入力0（なにも押されていない）
	NORMAL,     // A～B,a～b,1～9
	CANCEL,     //「×」(ひとつ消す)
	DONE,       //「OK」(確定)

	MISS = 99
};

class KeyBoard
{
public:
	//初期処理
	void KeyBoardInit();

	//画像読み込み
	int LoadImgae();

	//効果音読み込み
	int LoadSounds();

	//描画
	void DrawKeyBoard();

	//移動
	void KeyBoardControl(int NowKey);

	//Bボタンが押された時の処理
	void Push_B_Key(int NowKey, int* GameState, int Score);

	//入力情報表示
	void DrawInputInfo();

	//コントローラー調整
	int InputControl();

	//＊test用＊　各種情報表示
	void DrawTestInfo();

private:
	/*--------------------　UI　--------------------*/
		//背景画像
	int backimage = 0;
	//キーボード画像
	int keyboardimage = 0;

	//ノーマルカーソル画像  0 : 通常時　　1 : 押されたとき
	int Cursorimage[2] = { 0 };

	//「×」カーソル画像
	int Cancelimage[2] = { 0 };

	//「OK」カーソル画像
	int OKimage[2] = { 0 };

	//SE
	int SE_push = 0;   //通常入力
	int SE_cancel = 0; //「×」
	int SE_ok = 0;     //「ok」

	//移動量
	int movekeyX = 0;
	int movekeyY = 0;

	//キーの状態を保持
	KEY_TYPE PUSH_NOW;
	/*------------------------------------------------*/

	/*--------------------　他　----------------------*/
		//フレームをカウント
	int frame = 0;

	//入力情報
	char InputName[10];

	//InputName用
	int namePos;

	//Bが　押されている/押されてない フラグ    TRUE:押されている　FALSE:押されていない
	bool PushFlg;

	/*------------------------------------------------*/
};

extern KeyBoard keyboard;



