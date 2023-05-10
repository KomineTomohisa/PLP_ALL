#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h>

//�v���g�^�C�v�錾 --------------------------------------------------
void ShowFinMsg(char *);
void Del_Com(char *);
void GetNowPath(char *, char *);
void Del_Com(char *);
void Write_kumijun();
void Shuffle_oyako(FILE*);
void Read_sametime();
void Count_oyako_shuffle();

//�萔�̐錾 --------------------------------------------------
#define DEF_PAR_FNAME "�V�X�e���p�����[�^.txt" 
#define STR_MAX 1000
#define FGETS_MAX_LENGTH 20000
#define MaxN 1000 //��舵���ő�̕��i����64�Ƃ���
#define MaxMM 1000000


//�\���̂̐錾 --------------------------------------------------
struct kumijun//�g��������s�������Ă����\����
{
	int kuminum;//�g�t���ԍ�
	int oya;//�e���i
	int ko;//�q���i
	int tyukan;//�ł��钆�Ԑ��i
	char dir[10];//�g�t�������
	char XLineDir[3];
	char ZLineDir[3];
	int jig;//����ԍ�
	int base;//�x�[�X���i
	int bothhands_type;//�����ɑg�t����g�t�����ǂ���(�P�Ȃ�e���i������A�Q�Ȃ�q���i������)
	int kougu;
}kumijun[MaxN];

struct kumihead//�g�������̑g�������ԍ��Ȃǂ������Ă���s�����Ă����\����
{
	char K[2];//�ŏ���K
	int KJnum;//�g�������ԍ�
	int flag;//���݃t���O
	int x;
}kumihead;

struct same_time_parts//��������ɏ�����Ă��镔�i�ԍ������Ă����\����
{
	int flag[2];//���݃t���O
	int sessyoku[2];
	int Num[2];//�g�t���ԍ�
	int parts1[2];//�����ɑg�t���������i1
	int parts2[2];//�����ɑg�t���������i2
	int bothhands_type[2];//�e�q�w��̏��
	int kougu;
}sametime;

/*struct checked_kumijun//�A�������g��������2�s�̒��ɓ����ɑg�t���镔�i��4����g�������ŏo�͂������̂�ۑ����Ă�������
{
	int sum_kuminum;//�g�t���ԍ��̍��v�l
	int kuminum[MaxN];//�e�s�̑g�t���ԍ�
}check_kumij[100000];*/

struct sametime_kumituke_Pair
{
	int Pair_num1;//�g�t���̃y�A�̐�
	int Pair_num2;//
	//int sessyoku[MaxN][2];
	int Num[MaxN][2];//�g�t���ԍ�
	int parts1[MaxN][2];//���i1
	int parts2[MaxN][2];//���i2
	int bothhands_type[MaxN][2];//�e�q�֌W
}kumitsuke_Pair[MaxN];

struct BigProducts
{
	//int sessyoku;
	int Num;//�g�t���ԍ�
	int parts1;
	int parts2;
	int oyakoflug;
}BPkumituke[MaxN];

struct Support
{
	//int sessyoku;
	int Num;
	int parts1;
	int parts2;
	int oyakoflug;
}Support[MaxN];

struct Tegumi
{
	int Num;
	int parts1;
	int parts2;
	int oyakoflug;
}Tegumi[MaxN];

struct Kougu
{
	int num;
	int parts1;
	int parts2;
	int kougunum;
	int pattern;
}Kougu[MaxN];

//�O���[�o���ϐ��̐錾 --------------------------------------------------
char SEIHIN_Name[STR_MAX], outDir[STR_MAX], inPath_kumijun[STR_MAX], inPath_douji[STR_MAX], inPath[STR_MAX],outPath[STR_MAX];
int douji[MaxN][3], seiyakusu = 0/*����̐����L�^���Ă����ϐ�*/, gyousu = 0, change_oyako = 0, variation = 0, rearrange[MaxN/2]/*�e�q�w��Ȃ��̓����ɑg�t���镔�i��64���i32�g*/;
int Pair = 0, BPcount = 0, Supportcount = 0, Tegumicount = 0, Kougucount = 0;
int NewNOS0 = 0;
int tegumijig = 0;
int max_kumijun = 0;
int Filename = 0;
char dir_default= 'D';

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// �v���O��������
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int main(int argc, char *argv[])
{
	FILE *fp1;
	int parts1, parts2;
	char NowPath[STR_MAX]; //�ŏI�I�Ƀp�����[�^�t�@�C���̃t���p�X������
	char MsgS[STR_MAX]; //���b�Z�[�W�p�A�����^�z��
	double mode = 0;

	//���̃v���O�����̂���p�X���擾���āANowPath�ɓ����
	GetNowPath(NowPath, argv[0]);
	strcat(NowPath, "Input\\");	//�p�����[�^�t�@�C���̒u�����.exe������t�H���_����Input�Ƃ���
	strcat(NowPath, DEF_PAR_FNAME); //NowPath �� �p�����[�^�t�@�C�������������āA�t���p�X�ɂ���

	/************************************************************************************************/
	/*�p�����[�^�t�@�C�����J��*/
	/************************************************************************************************/
	// �p�����[�^�t�@�C���̎d�l
	//  1�s�ځF���i��
	//  2�s�ځF�g�t���t�@�C�������܂߂��t���p�X
	//  3�s�ځF�g�������̃t�@�C�������܂߂��t���p�X
	//  4�s�ځF�������ʂ��o�͂���t�H���_�̃p�X�i�t�@�C�����́u��������.csv�v�Ƃ���j

	fp1 = fopen(NowPath, "r");
	if (fp1 == NULL) ShowFinMsg("�p�����[�^�t�@�C��������܂���"); // �t�@�C�����J���Ȃ���ΏI��
	if (fscanf(fp1, "%s", SEIHIN_Name) == EOF) ShowFinMsg("���i�����s���ł�");
	//if (fscanf(fp1, "%d", &tegumijig) == EOF) ShowFinMsg("��g�݂��s���ʒu�̏�񂪑���܂���");
	if (fscanf(fp1, "%d", &Filename) == EOF) ShowFinMsg("�ǂݎ��t�@�C�������L�q���Ă�������");
	if (fscanf(fp1, "%d", &max_kumijun) == EOF) ShowFinMsg("�ǂݎ��g�������t�@�C���̍ő吔���L�q���Ă�������");
	if (fscanf(fp1, "%s", inPath) == EOF) ShowFinMsg("INPUT�̃p�X���s���ł�");
	if (fscanf(fp1, "%s", outPath) == EOF) ShowFinMsg("OUTPATH�̃p�X���s���ł�");
	fclose(fp1);

	//printf("�ǂݎ��ő�̃t�@�C���ԍ���%d�ł�\n", max_kumijun);

	//puts("���i������͂��Ă�������");
	//scanf("%d", &N);

	Read_sametime();

	Write_kumijun();


	
	puts("�o�͊���");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// ���[�e�B���e�B
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void Write_kumijun()//�g��������ǂݍ��ފ֐�
{
	//puts("�g������������");

	FILE*fp1;
	FILE*fp2;
	FILE*fp3;
	int i = 0, j = 0, k = 0, a = 0, b = 0, c = 0, d = 0, hantei = 0, totalhantei = 0, kanousu = 0, hanbetsu = 0, sametime[MaxN][3], tmp = 0;
	int karioki = 0, totalkarioki = 0, foregoing = 0/*�ߋ��ɏo�͂����g�������̐�*/, sum_samekuminum = 0/*�g�t���ԍ��̍��v�l*/, already = 0/*���ɏo�͂����g�������Ɠ����Ƃ���1�ɂ���*/, same = 0/*���ɓ����g�t�����o�Ă��Ă���̂��̔���*/;
	int tegumihantei = 0, o = 0, t = 0, s = 0, BPflag = 0, Tflag = 0, Sflag = 0, BH2flag = 0;/*�啨�A�v�����A��g�݂ɓ��Ă͂܂������ǂ����̔���*/;
	char WkD[STR_MAX], WkD2[STR_MAX];
	char temp[FGETS_MAX_LENGTH];
	int kumijuncount = 0;
	int kougu_buhin;
	
	//�o�͐�̃t���p�X��WkD�ɑ������

	for (kumijuncount = 0;kumijuncount <= max_kumijun;kumijuncount++)
	{
		//strcpy(WkD, outDir);
		//strcat(WkD, "\\");
		//strcat(WkD, SEIHIN_Name);
		if (Filename == 1)
		{
			sprintf(WkD, "%s\\%s_�ԗ�_����t�g������%d.csv", outPath, SEIHIN_Name, kumijuncount);

			sprintf(WkD2, "%s\\%s_�ԗ�_�g������%d_J.csv", inPath, SEIHIN_Name, kumijuncount);
		}
		else if(Filename == 2)
		{
			sprintf(WkD, "%s\\%s_���i�Q_����t�g������%d.csv", outPath, SEIHIN_Name, kumijuncount);

			sprintf(WkD2, "%s\\%s_���i�Q_�g������%d_J.csv", inPath, SEIHIN_Name, kumijuncount);
		}
		else if (Filename == 3)
		{
			sprintf(WkD, "%s\\%s_����_����t�g������%d.csv", outPath, SEIHIN_Name, kumijuncount);
			
			sprintf(WkD2, "%s\\%s_����_�g������%d_J.csv", inPath, SEIHIN_Name, kumijuncount);
		}
		char* fname1 = WkD;
		char* fname2 = WkD2;
		fp1 = fopen(fname2, "r");//�g�������t�@�C����ǂݍ��݃��[�h�ŊJ��
		printf("�C���v�b�g�t�@�C���@%s\n", fname2);
		if (fp1 != NULL)
		{
			fp2 = fopen(fname1, "w");//�������ʂ̃t�@�C�����������݃��[�h�ŊJ��
			printf("�A�E�g�v�b�g�t�@�C���@%s\n", fname1);
			//fp3 = fopen(fname2, "w");//�������ʂ̃t�@�C�����������݃��[�h�ŊJ��

			fgets(temp, FGETS_MAX_LENGTH, fp1);
			Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
			sscanf(temp, "%s%d%d%d", &kumihead.K, &kumihead.KJnum, &kumihead.flag, &kumihead.x);

			while (fgets(temp, FGETS_MAX_LENGTH, fp1) != NULL)//�t�@�C���̍Ō�܂�
			{
				if (strstr(temp, "K") != NULL)//�ǂݍ��񂾍s��K�̕���������Ƃ�
				{
				onemore:
					//�H���p�����g�����ǂ���
					for (a = 0; a < gyousu; a++)
					{
						kumijun[a].kougu = 0;
						for (k = 0; k < Kougucount; k++)
						{
							if (kumijun[a].kuminum == Kougu[k].num)
							{
								kumijun[a].ko = kougu_buhin = Kougu[k].parts2;
								kumijun[a].oya = Kougu[k].parts1;
								kumijun[a].bothhands_type = Kougu[k].pattern;
								kumijun[a].kougu = Kougu[k].kougunum;
								break;
							}
						}
					}
					for (i = 0; i < gyousu; i++)//2�s���Ȃ̂�1�̑g�������̍s��-1
					{
					out:
						BH2flag = BPflag = Tflag = Sflag = 0;


						for (j = 0; j < Pair; j++)
						{
							for (a = 0; a < kumitsuke_Pair[j].Pair_num1; a++)
							{
								for (b = 0; b < kumitsuke_Pair[j].Pair_num2; b++)
								{
									if (kougu_buhin != kumitsuke_Pair[j].parts1[a][0] && kougu_buhin != kumitsuke_Pair[j].parts2[a][0] && kougu_buhin != kumitsuke_Pair[j].parts1[b][1] && kougu_buhin != kumitsuke_Pair[j].parts2[b][1])
									{
										//printf("kumitsuke=%d kumitsuke_Pair1=%d,kumijun=&d  kumitsuke_Pair2=%d\n", kumijun[i].kuminum, kumitsuke_Pair[j].Num[a][0], kumijun[i + 1].kuminum, kumitsuke_Pair[j].Num[b][1]);

										if (kumijun[i].kuminum == kumitsuke_Pair[j].Num[a][0] && kumijun[i + 1].kuminum == kumitsuke_Pair[j].Num[b][1])
										{
											hantei++;
											if (kumitsuke_Pair[j].bothhands_type[a][0] == 1 && kumitsuke_Pair[j].bothhands_type[b][1] == 1)//�g�t�����e���i�A�q���i�����ւ��Ă�����2���i�g�t���ɂȂ鎞
											{//�O�̍�Ƃ������ƂŁA�ł������Ԑ��i�����̑g�t���̎q���i�ɂȂ�Ƃ�
												/*if ((kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[a][0]) || ((kumijun[i - 2].tyukan == kumitsuke_Pair[j].parts2[a][0] && kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[b][1]) && kumijun[i - 1].bothhands_type > 0))*/
												if ((kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[a][0] || (kumijun[i - 2].tyukan == kumitsuke_Pair[j].parts2[a][0] && kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[b][1])) && kumijun[i - 1].bothhands_type > 0)
												{
													kumijun[i].bothhands_type = 2;//�q���i������
													kumijun[i + 1].bothhands_type = 2;
													//�e�q�֌W�̓���ւ�
													kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts2[a][0];
													kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts2[b][1];

													kumijun[i].ko = kumitsuke_Pair[j].parts1[a][0];
													kumijun[i + 1].ko = kumitsuke_Pair[j].parts1[b][1];

													//����őg���Ă邱�Ƃ̂ł���2�s�ōŏ��̍s�łł������Ԑ��i���q���i�ɂȂ��Ă���Ƃ�(�e2�q1)
													if (kumijun[i].tyukan == kumijun[i + 1].ko)
													{
														kumijun[i].bothhands_type = 1;//�e���i������
														kumijun[i + 1].bothhands_type = 1;

													}

												}
												else
												{
													kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts1[a][0];
													kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts1[b][1];

													kumijun[i].ko = kumitsuke_Pair[j].parts2[a][0];
													kumijun[i + 1].ko = kumitsuke_Pair[j].parts2[b][1];

													kumijun[i].bothhands_type = 2;//�q���i������
													kumijun[i + 1].bothhands_type = 2;


												}

												////����őg���Ă邱�Ƃ̂ł���2�s�ōŏ��̍s�łł������Ԑ��i���q���i�ɂȂ��Ă���Ƃ�(�e2�q1)
												//if (kumijun[i].tyukan == kumijun[i + 1].ko)
												//{
												//	kumijun[i].bothhands_type = 1;//�e���i������
												//	kumijun[i + 1].bothhands_type = 1;
												//}
											}
											else//�e���i�ɂ��Â炳�����鎞
											{

												kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts1[a][0];//�g�t�����X�g�ł͗���őg�t���邱�Ƃ̂ł��镔�i�����炩���ߎq���i�ɂȂ��Ă���̂�OK
												kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts1[b][1];

												kumijun[i].ko = kumitsuke_Pair[j].parts2[a][0];
												kumijun[i + 1].ko = kumitsuke_Pair[j].parts2[b][1];
												//����őg���Ă邱�Ƃ̂ł���2�s�ōŏ��̍s�łł������Ԑ��i���q���i�ɂȂ��Ă���Ƃ�(�e2�q1)
												if (kumijun[i].tyukan == kumijun[i + 1].ko)
												{
													kumijun[i].bothhands_type = 1;//�e���i������
													kumijun[i + 1].bothhands_type = 1;

												}
												else
												{
													//�e�q�֌W�̏���������
													kumijun[i].bothhands_type = 2;//�q���i������
													kumijun[i + 1].bothhands_type = 2;
												}
											}
											i += 2;//out�ɍs���̂�i++����Ȃ�����+2����
											BH2flag = 1;//����2���i�̃t���O��t���Ă���(����Ȃ�����)
											goto out;
										}
										else if (kumijun[i].kuminum == kumitsuke_Pair[j].Num[b][1] && kumijun[i + 1].kuminum == kumitsuke_Pair[j].Num[a][0])//�v���� ����ł�1�C5��5�C1�̑g�����̐ڐG�����I�΂�邽�߁A�����2���i�g���Ă�ۂɓ����g�������ɂȂ�\������B�����������ɐ��񂪂������Ă���ꍇ���̂悤�ɂ��Ȃ��Ə������I�΂�Ȃ����Ƃ�����
										{
											if (kumitsuke_Pair[j].bothhands_type[a][0] == 1 && kumitsuke_Pair[j].bothhands_type[b][1] == 1)//�g�t�����e���i�A�q���i�����ւ��Ă�����2���i�g�t���ɂȂ鎞
											{//�O�̍�Ƃ������ƂŁA�ł������Ԑ��i�����̑g�t���̎q���i�ɂȂ�Ƃ�
												/*if ((kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[b][1]) || ((kumijun[i - 2].tyukan == kumitsuke_Pair[j].parts2[b][1] && kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[a][0]) && kumijun[i - 1].bothhands_type > 0))*/
												if ((kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[b][1] || (kumijun[i - 2].tyukan == kumitsuke_Pair[j].parts2[b][1] && kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[a][0])) && kumijun[i - 1].bothhands_type > 0)
												{
													kumijun[i].bothhands_type = 2;//�q���i������
													kumijun[i + 1].bothhands_type = 2;
													//�e�q�֌W�̓���ւ�
													kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts2[b][1];
													kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts2[a][0];

													kumijun[i].ko = kumitsuke_Pair[j].parts1[b][1];
													kumijun[i + 1].ko = kumitsuke_Pair[j].parts1[a][0];

													//����őg���Ă邱�Ƃ̂ł���2�s�ōŏ��̍s�łł������Ԑ��i���q���i�ɂȂ��Ă���Ƃ�(�e2�q1)

													if (kumijun[i].tyukan == kumijun[i + 1].ko)
													{
														kumijun[i].bothhands_type = 1;//�e���i������
														kumijun[i + 1].bothhands_type = 1;
													}
												}

												else
												{
													kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts1[b][1];
													kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts1[a][0];

													kumijun[i].ko = kumitsuke_Pair[j].parts2[b][1];
													kumijun[i + 1].ko = kumitsuke_Pair[j].parts2[a][0];
													//����őg���Ă邱�Ƃ̂ł���2�s�ōŏ��̍s�łł������Ԑ��i���q���i�ɂȂ��Ă���Ƃ�(�e2�q1)
													if (kumijun[i].tyukan == kumijun[i + 1].ko)
													{
														kumijun[i].bothhands_type = 1;//�e���i������
														kumijun[i + 1].bothhands_type = 1;

													}
													//�e�q�֌W�̏���������
													else
													{
														kumijun[i].bothhands_type = 2;//�q���i������
														kumijun[i + 1].bothhands_type = 2;
													}


												}
												////����őg���Ă邱�Ƃ̂ł���2�s�ōŏ��̍s�łł������Ԑ��i���q���i�ɂȂ��Ă���Ƃ�(�e2�q1)
												//if (kumijun[i].tyukan == kumijun[i + 1].ko)
												//{
												//	kumijun[i].bothhands_type = 1;//�e���i������
												//	kumijun[i + 1].bothhands_type = 1;
												//}
											}
											else//�e���i�ɂ��Â炳�����鎞
											{

												kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts1[b][1];//�g�t�����X�g�ł͗���őg�t���邱�Ƃ̂ł��镔�i�����炩���ߎq���i�ɂȂ��Ă���̂�OK
												kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts1[a][0];

												kumijun[i].ko = kumitsuke_Pair[j].parts2[b][1];
												kumijun[i + 1].ko = kumitsuke_Pair[j].parts2[a][0];
												if (kumijun[i].tyukan == kumijun[i + 1].ko)
												{
													kumijun[i].bothhands_type = 1;//�e���i������
													kumijun[i + 1].bothhands_type = 1;

												}
												else
												{
													//�e�q�֌W�̏���������
													kumijun[i].bothhands_type = 2;//�q���i������
													kumijun[i + 1].bothhands_type = 2;
												}

											}
											i += 2;//out�ɍs���̂�i++����Ȃ�����+2����
											BH2flag = 1;//����2���i�̃t���O��t���Ă���(����Ȃ�����)
											goto out;

										}
									}
								}
							}
						}
						if (BH2flag == 0)
						{
							for (o = 0; o < BPcount; o++)
							{
								if (kumijun[i].kuminum == BPkumituke[o].Num)
								{
									if (BPkumituke[o].oyakoflug == 1)//�����g�t�����X�g�̐e���i�������d���Ƃ�
									{
										if (kumijun[i].oya == BPkumituke[o].parts1) kumijun[i].bothhands_type = 6;//�e���i���d��
										else kumijun[i].bothhands_type = 6;//�q���i���d��
									}
									else if (BPkumituke[o].oyakoflug == 2)//�����g�t�����X�g�̎q���i�������d���Ƃ�
									{
										if (kumijun[i].ko == BPkumituke[o].parts2) kumijun[i].bothhands_type = 5;//�q���i���d��
										else kumijun[i].bothhands_type = 7;//�e���i���d��
									}
									else if (BPkumituke[o].oyakoflug == 3) kumijun[i].bothhands_type = 7;//�e���i�q���i���ɏd���Ƃ�
									BPflag = 1;
									break;
								}
							}
						}
						if (BPflag == 0 && BH2flag == 0)
						{
							for (s = 0; s < Supportcount; s++)
							{
								if (kumijun[i].kuminum == Support[s].Num)
								{
									Sflag = 1;
									if (Support[s].oyakoflug == 0)
									{
										kumijun[i].oya = kumijun[i].base = Support[s].parts1;
										kumijun[i].ko = Support[s].parts2;
									}
									kumijun[i].bothhands_type = 4;
									break;
								}
							}
						}
						if (BPflag == 0 && Sflag == 0 && BH2flag == 0)
						{
							for (t = 0; t < Tegumicount; t++)
							{
								if (kumijun[i].kuminum == Tegumi[t].Num)
								{
									Tflag = 1;
									//kumijun[i].jig = tegumijig;
									kumijun[i].bothhands_type = 3;
								}
							}
						}
					}

					for (a = 0; a < gyousu; a++)
					{
						//printf("��g��\n");
						if (kumijun[a].bothhands_type == 3)
						{
							if ((kumijun[a + 1].bothhands_type != 3) || (kumijun[a + 1].bothhands_type == 3 && kumijun[a].tyukan != kumijun[a + 1].oya))
							{
								for (k = a; k < gyousu; k++)
								{
									if (kumijun[a].tyukan == kumijun[k].oya)
									{
										printf("%d\n", kumijun[a].tyukan);
										kumijun[k].base = kumijun[k].ko;
										kumijun[k].ko = kumijun[k].oya;
										kumijun[k].oya = kumijun[k].base;
										break;
									}
								}
							}
						}
					}
									

					printf("K%d\r", kumihead.KJnum);
					
					
					
						fprintf(fp2, "%s,%d,%d,%d,�e���i�̎p��(X������),�e���i�̎p��(Z������),����ԍ�,�H��ԍ�,����ԍ�\n", kumihead.K, kumihead.KJnum, kumihead.flag, kumihead.x);
						for (i = 0;i < gyousu;i++)
						{
							fprintf(fp2, "%d,%d,%d,%d,%c,%c,%d,%d,%d\n", kumijun[i].kuminum, kumijun[i].oya, kumijun[i].ko, kumijun[i].tyukan, dir_default, dir_default, kumijun[i].jig, kumijun[i].kougu, kumijun[i].bothhands_type);
						}

					//fprintf(fp3, "K,%d,%d\n", kumihead.KJnum, hantei);


				overlap://goto���̃��x��

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					sscanf(temp, "%s%d%d%d", &kumihead.K, &kumihead.KJnum, &kumihead.flag, &kumihead.x);
					//printf("K %d\n",kumihead.KJnum);
					gyousu = 0;
					hantei = 0;
				}
				else//�ǂݍ��񂾍s�̒���K�������Ă��Ȃ��Ƃ�(�g�������̍s�̓r���̎�)
				{
					//�t�@�C���̓ǂݍ���
					Del_Com(temp);
					
					
					sscanf(temp, "%d%d%d%d%s%s%s%d%d%d", &kumijun[gyousu].kuminum, &kumijun[gyousu].oya, &kumijun[gyousu].ko, &kumijun[gyousu].tyukan,&kumijun[gyousu].dir,&kumijun[gyousu].XLineDir, &kumijun[gyousu].ZLineDir, &kumijun[gyousu].jig, &kumijun[gyousu].kougu, &kumijun[gyousu].bothhands_type); //�g�t���ԍ��A���i1�A���i2�A���Ԑ��i�̓ǂݍ���
					//printf("1���%d 2���%d 3���%d 4���%d 5���%s 6���%s 7���%s 8���%d 9���%d 10���%d", kumijun[gyousu].kuminum, kumijun[gyousu].oya, kumijun[gyousu].ko, kumijun[gyousu].tyukan,kumijun[gyousu].dir, kumijun[gyousu].XLineDir, kumijun[gyousu].ZLineDir, kumijun[gyousu].jig, kumijun[gyousu].kougu, kumijun[gyousu].bothhands_type);
					kumijun[gyousu].bothhands_type = -1;																																															 //fprintf(fp2, "%d,%d,%d,%d,%s,%d,%d\n", kumijun[gyousu].kuminum, kumijun[gyousu].oya, kumijun[gyousu].ko, kumijun[gyousu].tyukan, kumijun[gyousu].dir, kumijun[gyousu].jig, kumijun[gyousu].base);
					gyousu++;//�g�������̍s���𑝂₷
				}
			}
			d++;
			if (d == 1) goto onemore;
			fclose(fp1);
			fclose(fp2);
			//fclose(fp3);
		}
	}
}

void Read_sametime()//��������t�@�C����ǂݍ���
{
	FILE*fp;
	char temp[FGETS_MAX_LENGTH];
	int a, i = 0, P1 = 0, P2 = 0;//sametime.flag[0], sametime.sessyoku[0], sametime.Num[0], sametime.parts1[0], sametime.parts2[0];
	int BP = 0/*�啨�g�t��*/, BH2 = 1/*����2���i�g�t��*/, Supportflag = 0/*�v�����g�t��*/, tegumi = 0/*��g��*/, kougu = 0/*�H��*/;
	int sessyokusenNum = 0;
	char WkD[STR_MAX];

	sprintf(WkD, "%s\\%s_����g�t�����X�g.csv", inPath, SEIHIN_Name);
	//printf("%s", WkD);
	char* fname = WkD;
	fp = fopen(fname, "r");
	printf("�C���v�b�g�t�@�C���@%s\n", fname);
	//fp = fopen(inPath_douji, "r");//��������t�@�C����ǂݍ��݃��[�h�ŊJ��
	//fgets(temp, FGETS_MAX_LENGTH, fp);//�t�@�C���̈�s�ڂ��΂�           
	//fgets(temp, FGETS_MAX_LENGTH, fp);
	//fgets(temp, FGETS_MAX_LENGTH, fp);                                                 �É��C��4��13��
	while (fgets(temp, FGETS_MAX_LENGTH, fp) != NULL)//�t�@�C���̍Ō�܂œǂݎ��
	{
		//printf("%s\n", temp);
		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
		//printf("a%d,b%d,c%d,d%d,e%d,f%d ", sametime.flag[0], sametime.sessyoku[0], sametime.Num[0], sametime.parts1[0], sametime.parts2[0], sametime.bothhands_type[0]);//�\���̂Ƀt�@�C���̈�s�����[
		//printf("g%d, h%d, i%d, j%d, k%d, l%d\n", sametime.flag[1], sametime.sessyoku[1], sametime.Num[1], sametime.parts1[1], sametime.parts2[1], sametime.bothhands_type[1]);
		if (strstr(temp, "�啨") != NULL)
		{
			//����2���i�̍Ō�̃y�A��ۑ�
			kumitsuke_Pair[Pair].Pair_num1 = P1;
			kumitsuke_Pair[Pair].Pair_num2 = P2;

			printf("Pair=%d,P1=%d,P2=%d\n", Pair, P1, P2);

			P1 = 0;
			P2 = 0;
			Pair++;
			fgets(temp, FGETS_MAX_LENGTH, fp);
			Del_Com(temp);
			BH2 = 0;
			BP = 1;
			//fgets(temp, FGETS_MAX_LENGTH, fp);
		}
		else if (strstr(temp, "�v����") != NULL)
		{
			fgets(temp, FGETS_MAX_LENGTH, fp);
			Del_Com(temp);
			BH2 = 0;
			BP = 0;
			Supportflag = 1;
			//fgets(temp, FGETS_MAX_LENGTH, fp);
		}
		else if (strstr(temp, "��g��") != NULL)
		{
			fgets(temp, FGETS_MAX_LENGTH, fp);
			Del_Com(temp);
			BH2 = 0;
			BP = 0;
			Supportflag = 0;
			tegumi = 1;
			//fgets(temp, FGETS_MAX_LENGTH, fp);
		}
		else if (strstr(temp, "�H��") != NULL)
		{
			//printf("�H��g�p\n");
			fgets(temp, FGETS_MAX_LENGTH, fp);
			Del_Com(temp);
			BH2 = 0;
			BP = 0;
			Supportflag = 0;
			tegumi = 0;
			kougu = 1;
			//fgets(temp, FGETS_MAX_LENGTH, fp);
		}
		else if (BH2 == 1)//����2���i�g�t���̓ǂݍ���
		{
			sscanf(temp, "%d%d%d%d%d%d%d%d%d%d%d%d", &sametime.flag[0], &sametime.sessyoku[0], &sametime.Num[0], &sametime.parts1[0], &sametime.parts2[0], &sametime.bothhands_type[0], &sametime.flag[1], &sametime.sessyoku[1], &sametime.Num[1], &sametime.parts1[1], &sametime.parts2[1], &sametime.bothhands_type[1]);//�\���̂Ƀt�@�C���̈�s�����[
			if (sametime.flag[0] == 1 && sametime.flag[1] == 1)
			{
				for (i = 0;i < 2;i++)
				{
					//kumitsuke_Pair[Pair].sessyoku[P1][i] = sametime.sessyoku[i];//�y�A1�ڂ̐ڐG��[0],2�ڂ̐ڐG��[1]
					sessyokusenNum = sametime.sessyoku[i];
					kumitsuke_Pair[Pair].Num[P1][i] = sametime.Num[i];//�y�A1�ڂ̑g�t���ԍ�[0],2�ڂ̑g�t���ԍ�[1]
					kumitsuke_Pair[Pair].parts1[P1][i] = sametime.parts1[i];//�y�A1�ڂ̐e���i,2�ڂ̐e���i
					kumitsuke_Pair[Pair].parts2[P1][i] = sametime.parts2[i];//�y�A1�ڂ̎q���i,2�ڂ̎q���i
					kumitsuke_Pair[Pair].bothhands_type[P1][i] = sametime.bothhands_type[i];//�y�A�̐e�q�w��̏��
				}

				printf("%d,%d,%d,%d,%d  %d,%d,%d,%d,%d\n", sessyokusenNum, kumitsuke_Pair[Pair].Num[P1][0], kumitsuke_Pair[Pair].parts1[P1][0], kumitsuke_Pair[Pair].parts2[P1][0], kumitsuke_Pair[Pair].bothhands_type[P1][0],
					sessyokusenNum, kumitsuke_Pair[Pair].Num[P2][1], kumitsuke_Pair[Pair].parts1[P2][1], kumitsuke_Pair[Pair].parts2[P2][1], kumitsuke_Pair[Pair].bothhands_type[P2][1]);
				P1++;
				P2++;
			}
			else if (sametime.flag[0] == 1 && sametime.flag[1] == 0)
			{
				//kumitsuke_Pair[Pair].sessyoku[P1][0] = sametime.sessyoku[0];
				sessyokusenNum = sametime.sessyoku[0];
				kumitsuke_Pair[Pair].Num[P1][0] = sametime.Num[0];
				kumitsuke_Pair[Pair].parts1[P1][0] = sametime.parts1[0];
				kumitsuke_Pair[Pair].parts2[P1][0] = sametime.parts2[0];
				kumitsuke_Pair[Pair].bothhands_type[P1][0] = sametime.bothhands_type[0];

				printf("%d,%d,%d,%d,%d\n", sessyokusenNum, kumitsuke_Pair[Pair].Num[P1][0], kumitsuke_Pair[Pair].parts1[P1][0], kumitsuke_Pair[Pair].parts2[P1][0], kumitsuke_Pair[Pair].bothhands_type[P1][0]);
				P1++;
			}
			else if (sametime.flag[0] == 0 && sametime.flag[1] == 1)
			{
				//kumitsuke_Pair[Pair].sessyoku[P2][1] = sametime.sessyoku[1];
				sessyokusenNum = sametime.sessyoku[1];
				kumitsuke_Pair[Pair].Num[P2][1] = sametime.Num[1];
				kumitsuke_Pair[Pair].parts1[P2][1] = sametime.parts1[1];
				kumitsuke_Pair[Pair].parts2[P2][1] = sametime.parts2[1];
				kumitsuke_Pair[Pair].bothhands_type[P2][1] = sametime.bothhands_type[1];

				printf("�@�@�@�@�@�@%d,%d,%d,%d,%d\n", sessyokusenNum, kumitsuke_Pair[Pair].Num[P2][1], kumitsuke_Pair[Pair].parts1[P2][1], kumitsuke_Pair[Pair].parts2[P2][1], kumitsuke_Pair[Pair].bothhands_type[P2][1]);

				P2++;
			}

			if (sametime.flag[0] > 1)
			{
				kumitsuke_Pair[Pair].Pair_num1 = P1;
				kumitsuke_Pair[Pair].Pair_num2 = P2;

				printf("Pair=%d,P1=%d,P2=%d\n", Pair, P1, P2);

				P1 = 0;
				P2 = 0;
				Pair++;
			}
		}
		else if (BP == 1)
		{
			sscanf(temp, "%d%d%d%d%d%d", &sametime.flag[0], &sametime.sessyoku[0], &sametime.Num[0], &sametime.parts1[0], &sametime.parts2[0], &sametime.bothhands_type[0]);//�\���̂Ƀt�@�C���̈�s�����[
			if (sametime.flag[0] == 1)
			{
				BPkumituke[BPcount].Num = sametime.Num[0];
				BPkumituke[BPcount].parts1 = sametime.parts1[0];
				BPkumituke[BPcount].parts2 = sametime.parts2[0];
				BPkumituke[BPcount].oyakoflug = sametime.bothhands_type[0];

				BPcount++;
			}
		}
		else if (Supportflag == 1)
		{
			sscanf(temp, "%d%d%d%d%d%d", &sametime.flag[0], &sametime.sessyoku[0], &sametime.Num[0], &sametime.parts1[0], &sametime.parts2[0], &sametime.bothhands_type[0]);//�\���̂Ƀt�@�C���̈�s�����[
			if (sametime.flag[0] == 1)
			{
				Support[Supportcount].Num = sametime.Num[0];
				Support[Supportcount].parts1 = sametime.parts1[0];
				Support[Supportcount].parts2 = sametime.parts2[0];
				Support[Supportcount].oyakoflug = sametime.bothhands_type[0];
				Supportcount++;
			}
		}
		else if (tegumi == 1)
		{
			sscanf(temp, "%d%d%d%d%d%d", &sametime.flag[0], &sametime.sessyoku[0], &sametime.Num[0], &sametime.parts1[0], &sametime.parts2[0], &sametime.bothhands_type[0]);//�\���̂Ƀt�@�C���̈�s�����[
			if (sametime.flag[0] == 1)
			{
				printf("%d\n", sametime.Num[0]);
				Tegumi[Tegumicount].Num = sametime.Num[0];
				Tegumi[Tegumicount].parts1 = sametime.parts1[0];
				Tegumi[Tegumicount].parts2 = sametime.parts2[0];
				Tegumi[Tegumicount].oyakoflug = sametime.bothhands_type[0];
				Tegumicount++;
			}
		}
		else if (kougu == 1)
		{
			sscanf(temp, "%d%d%d%d%d%d%d", &sametime.flag[0], &sametime.sessyoku[0], &sametime.Num[0], &sametime.parts1[0], &sametime.parts2[0], &sametime.bothhands_type[0],&sametime.kougu);//�\���̂Ƀt�@�C���̈�s�����[
			if (sametime.flag[0] == 1)
			{
				Kougu[Kougucount].num = sametime.Num[0];
				Kougu[Kougucount].parts1= sametime.parts1[0];
				Kougu[Kougucount].parts2= sametime.parts2[0];
				Kougu[Kougucount].kougunum = sametime.bothhands_type[0];
				Kougu[Kougucount].pattern = sametime.kougu;
				Kougucount++;
			}
		}
	}
	//printf("Pair=%d,P1=%d,P2=%d\n", Pair, P1, P2);
	fclose(fp);

}

void Count_oyako_shuffle()
{
	int i = 0, j = 0, count = 0, k = 0, remainder = 0;

	for (i = 0;i < variation;i++)
	{
		for (j = 0;j < change_oyako;j++)
		{
			rearrange[j] = (i >> j) & 1;
			printf("%d,%d %d\n", i, j, rearrange[j]);
		}
		puts("");
	}

	/*for (i = 0;i < variation;i++)
	{
		remainder = i;

		for (j = variation/2;j > 0;j/=2)
		{
			if (remainder / j == 1)
			{
				rearrange[i][k] = 1;
			}
			else
			{
				rearrange[i][k] = 0;
			}

			remainder %= j;
			k++;
		}
		k = 0
		*/
}

void Shuffle_oyako(FILE*fp1)//�e�q�֌W�̎w�肪�Ȃ��ꍇ�ɐe�q�֌W�����ւ����g���������o�͂��邽�߂̊֐�
{
	int i = 0, j = 0, k = 0, shuffle = 0, a = 0;

	for (i = 0;i < variation;i++)
	{
		fprintf(fp1, "%s,%d%03d,%d,%d,�g�t������,����ԍ�,�x�[�X���i\n", kumihead.K, kumihead.KJnum, i + 1, kumihead.flag, kumihead.x);
		for (k = 0;k < gyousu;k++)
		{
			shuffle = (i >> j) & 1;//�ϐ�i��2�i����j�̈ʂ�1���ǂ���(1�Ȃ�P��Ԃ��A0�Ȃ�0��Ԃ�)
			if (kumijun[k].bothhands_type == 0 && kumijun[k + 1].bothhands_type == 0)
			{
				if (shuffle == 0) for (a = 0;a < 2;a++)fprintf(fp1, "%d,%d,%d,%d,%s,%d,%d,1\n", kumijun[k + a].kuminum, kumijun[k + a].oya, kumijun[k + a].ko, kumijun[k + a].tyukan, kumijun[k + a].dir, kumijun[k + a].jig, kumijun[k + a].base);
				if (shuffle == 1) for (a = 0;a < 2;a++) fprintf(fp1, "%d,%d,%d,%d,%s,%d,%d,2\n", kumijun[k + a].kuminum, kumijun[k + a].ko, kumijun[k + a].oya, kumijun[k + a].tyukan, kumijun[k + a].dir, kumijun[k + a].jig, kumijun[k + a].ko);
				j++;
				k++;
			}
			else
			{
				fprintf(fp1, "%d,%d,%d,%d,%s,%d,%d,%d\n", kumijun[k].kuminum, kumijun[k].oya, kumijun[k].ko, kumijun[k].tyukan, kumijun[k].dir, kumijun[k].jig, kumijun[k].base, kumijun[k].bothhands_type);
			}
		}
		j = 0;
	}

}


//���b�Z�[�W��\�����āA�v���O�������I�����邽�߂̊֐�
void ShowFinMsg(char *msg) {
	int c;
	printf("%s\n", msg);
	printf("(Enter�L�[�������Ă�������)");
	c = getchar();
	exit(EXIT_FAILURE);
}

//�ǂݍ��񂾕�����́u,�v�i�J���}�j���u �v�i���p�X�y�[�X�j�ɒu��������֐�
void Del_Com(char *s) {
	int	i;

	i = 0;
	while (s[i] != '\0') {
		if (s[i] == ',') s[i] = ' ';
		i++;
	}
}

//���̃v���O�����̒u���Ă���p�X��Ԃ����߂̊֐�
void GetNowPath(char *NP, char *S) {
	char *p;
	int i;

	p = strrchr(S, '\\');
	// strrchr
	// ������̍Ōォ�當������������
	// �y�����z
	// const char *s�@�F�@�����Ώە�����
	// int c�@�F�@��������
	// �y�߂�l�z
	// ���������Ƃ��@�F�@��v�����̃A�h���X
	// ������Ȃ������Ƃ��@�F�@NULL

	for (i = 0; i <= (p - S); i++) {
		NP[i] = S[i];
	}
	NP[i] = '\0'; // �Ō��NULL��t��
}
