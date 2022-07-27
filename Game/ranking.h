#pragma once
#include<stdio.h>

//ランキング表示　最大数は 5
#define MAX_RANKING_DATA 5

//ランキングデータ(構造体)
struct RankingData
{
	int no;          //順位
	char name[10];   //name は10byte(9文字)
	long score;      //スコア
};


class Ranking
{
public:
	void SortRanking();  //ランキング並べ替え
	int ReadRanking();   //ランキングデータ読み込み
	int SaveRanking();   //ランキングデータ保存

	void DrawRanking();  //ランキング描画
	int GetFifthScore();  //ランキング5番目のスコア取得

	//"KeyBoard"で使用
	void SetRanking(char* name, int* GameState, int Score);  //ランキングデータセット・画面遷移

private:
	FILE* fp;      //ファイル構造体

	RankingData g_Ranking[MAX_RANKING_DATA];     //ランキングデータ変数
};

extern Ranking ranking;