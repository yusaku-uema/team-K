#include"DxLib.h"
#include"ranking.h"

Ranking ranking;

//ランキング並べ替え
void Ranking::SortRanking()
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

//ランキングデータ読み込み
int Ranking::ReadRanking()
{
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

//ランキングデータ保存
int Ranking::SaveRanking()
{
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

//ランキング描画
void Ranking::DrawRanking()
{
	// ランキング一覧を表示
	SetFontSize(30);
	for (int i = 0; i < MAX_RANKING_DATA; i++)
	{
		DrawFormatString(50, 170 + i * 35, 0xffd700, "%2d   %10s     %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}
}

//↓↓　"KeyBoard"で使用　↓↓

//ランキングデータセット・画面遷移
void Ranking::SetRanking(char* name, int* GameState, int Score)
{
	strcpy_s(g_Ranking[4].name, 10, name);

	g_Ranking[MAX_RANKING_DATA - 1].score = Score;	// ランキングデータの5番目にスコアを登録

	SortRanking();		// ランキング並べ替え
	SaveRanking();		// ランキングデータの保存
	*GameState = 2;		// ゲームモードの変更
}
