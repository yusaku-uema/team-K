#include"DxLib.h"
#include"ranking.h"

Ranking ranking;

//�����L���O���בւ�
void Ranking::SortRanking()
{
	//���בւ��p
	RankingData work;
	int i, j;

	//�I��@�\�[�g
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

	// ���ʕt��
	for (i = 0; i < MAX_RANKING_DATA; i++)
	{
		g_Ranking[i].no = 1;
	}

	// ���_�������ꍇ�́A�������ʂƂ���
	// �����ʂ��������ꍇ�̎��̏��ʂ̓f�[�^�������Z���ꂽ���ʂƂ���
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

//�����L���O�f�[�^�ǂݍ���
int Ranking::ReadRanking()
{
	//�t�@�C���I�[�v��
	if ((fopen_s(&fp, "dat/rankingdata.txt", "r")) != 0)
	{
		//�G���[����
		printf("Ranking Data Error\n");
		return -1;
	}

	//�����L���O�f�[�^�z����f�[�^��ǂݍ���
	for (int i = 0; i < MAX_RANKING_DATA; i++)
	{
		fscanf_s(fp, "%2d %10s %10d", &g_Ranking[i].no, g_Ranking[i].name, 10, &g_Ranking[i].score);
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	return 0;
}

//�����L���O�f�[�^�ۑ�
int Ranking::SaveRanking()
{
	//�t�@�C���I�[�v��
	if ((fopen_s(&fp, "dat/rankingdata.txt", "w")) != 0)
	{
		//�G���[����
		printf("Ranking Data Error\n");
		return -1;
	}

	// �����L���O�f�[�^���z��f�[�^����������
	for (int i = 0; i < MAX_RANKING_DATA; i++)
	{
		fprintf(fp, "%2d %10s %10d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	return 0;
}

//�����L���O�`��
void Ranking::DrawRanking()
{
	// �����L���O�ꗗ��\��
	SetFontSize(30);
	for (int i = 0; i < MAX_RANKING_DATA; i++)
	{
		DrawFormatString(50, 170 + i * 35, 0xffd700, "%2d   %10s     %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}
}

//�����@"KeyBoard"�Ŏg�p�@����

//�����L���O�f�[�^�Z�b�g�E��ʑJ��
void Ranking::SetRanking(char* name, int* GameState, int Score)
{
	strcpy_s(g_Ranking[4].name, 10, name);

	g_Ranking[MAX_RANKING_DATA - 1].score = Score;	// �����L���O�f�[�^��5�ԖڂɃX�R�A��o�^

	SortRanking();		// �����L���O���בւ�
	SaveRanking();		// �����L���O�f�[�^�̕ۑ�
	*GameState = 2;		// �Q�[�����[�h�̕ύX
}
