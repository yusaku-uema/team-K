#pragma once
#include<stdio.h>

//�����L���O�\���@�ő吔�� 5
#define MAX_RANKING_DATA 5

//�����L���O�f�[�^(�\����)
struct RankingData
{
	int no;          //����
	char name[10];   //name ��10byte(9����)
	long score;      //�X�R�A
};


class Ranking
{
public:
	void SortRanking();  //�����L���O���בւ�
	int ReadRanking();   //�����L���O�f�[�^�ǂݍ���
	int SaveRanking();   //�����L���O�f�[�^�ۑ�

	void DrawRanking();  //�����L���O�`��
	int GetFifthScore();  //�����L���O5�Ԗڂ̃X�R�A�擾

	//"KeyBoard"�Ŏg�p
	void SetRanking(char* name, int* GameState, int Score);  //�����L���O�f�[�^�Z�b�g�E��ʑJ��

private:
	FILE* fp;      //�t�@�C���\����

	RankingData g_Ranking[MAX_RANKING_DATA];     //�����L���O�f�[�^�ϐ�
};

extern Ranking ranking;