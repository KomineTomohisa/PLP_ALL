#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <conio.h>

/*�v���g�^�C�v�錾--------------------------------------------------------------------------------------------------------------------------------------------------*/
/*input����ǂݍ��ފ֐�*/
void Read_kumitsuke_Dir();
void Read_buhin_zokusei();
void Read_tyukanseihin_zokusei();
void Read_kumituke();
void Read_Sessyoku();

void Make_jigmat();

/*����p�g�t�����X�g���쐬���郁�C���̊֐�*/
void Make_BothHands_Kumitukelist(fp2);

/*����p�g�t�����X�g�ɕK�v�Ȋ֐�*/
void Found_BH2Parts();
void Make_BH2Pair();
void Check_BH2Pair_Dir(int, int, char *);
void Check_BHPair_Kumituke();
void Write_BH_Kumilist_BH2(FILE *);
void Write_BH_Kumilist_BP(FILE *);
void Write_BH_Kumilist_BH1(FILE *);
void Write_BH_Kumilist_Tegumi(FILE *);
void Write_KouguParts(FILE *);

/*�V�X�e���𓮂�����ŕK�v�ƂȂ�֐�*/
void ShowFinMsg(char *);
void Del_Com(char *);//�ǂݍ��񂾕�����́u,�v�i�J���}�j���u �v�i���p�X�y�[�X�j�ɒu��������֐�
void GetNowPath(char *, char *);

/*�萔�̐錾--------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define STR_MAX 1000
#define DEF_PAR_FNAME "�V�X�e���p�����[�^.txt" 
#define FGETS_MAX_LENGTH 20000
#define MaxN 8000 //��舵���ő�̕��i����8000�Ƃ���
#define MaxMM 1000000 //��舵���ő�̒��Ԑ��i��1,000,000�Ƃ���
#define MaxDN 300 //CSV�t�@�C����s�ɋL�������ő�̕�����(���i����64���i�Ȃ̂�200�p�ӂ���)
#define MaxKN 1000				//�ő�̑g�t����1,000�Ƃ���
#define Kumitsuke 0
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679
#define Flag_num 1
#define gosa 2

/*�\���̂̐錾------------------------------------------------------------------------------------------------------------------------------------------------------*/
struct buhinzokusei
{
	int Parts;
	double centerX;
	double centerY;
	double centerZ;
	double lengthX;
	double lengthY;
	double lengthZ;
	double Weight;
	int Tool;
	int KouguType;
	int Atsukai;
	int Okikata;
}buhinzokusei[MaxN];

struct kumitsuke_Dir
{
	int Num;
	int Parts1;
	int Parts2;
	char Dir[5];
}kumitsuke_Dir[MaxN];

struct sessyokusen_tokusei
{
	int Num;
	int Parts1;
	int Parts2;
	int Non_fix;
	int Multi_position;
	int Asymmetry;
	int Contact_point;
	int Special;
	int Assembly_hard;
	int Assembly_impossible;
	int Hand_input;
	int Sum;
}sessyoku[MaxN];

struct buhin_tokusei
{
	int Parts;
	int Shapechange;
	int Care;
	int Dir;
}buhin_tokusei[MaxN];

struct tyukanseihin_zokusei
{
	int Num;
	int Parts[MaxN];
}tyukanzokusei[MaxN];

struct Check_sessyokuDir
{
	int Num;
	int Parts1;
	int Parts2;
	char Dir[5];
}Check_sessyokuDir[2][MaxMM / 2];

struct BH2_Parts
{
	int sessyokusen1;
	int sessyokusen2;
	int Parts1;
	int Parts2;
	int kumituke1[MaxN];
	int kumituke2[MaxN];
	int kumitukesu1;
	int kumitukesu2;
}BH2Parts[10000];

struct kumituke1_tokusei
{
	int Num;
	int Oya_Parts;
	int Ko_Parts;
	int OyaKo_Flag;//0:�e�q�ύX�s�A1:�e�q�ύX��
}kumitoku1[2000][MaxN];

struct kumituke2_tokusei
{
	int Num;
	int Oya_Parts;
	int Ko_Parts;
	int OyaKo_Flag;//0:�e�q�ύX�s�A1:�e�q�ύX��
}kumitoku2[2000][MaxN];

struct BigProducts
{
	int Num;
	int Oya;
	int Ko;
	int sagyou_num;//�e���i�q���i�ǂ��炪�啨�Ȃ̂�

}BP[MaxMM];

struct KouguParts
{
	int parts;
	int tool;
	int kougutype;
	int atsukai;
	int okikata;
	//int jiritsu;
	//int antei;
}KouguParts[MaxN];

struct Kumituke
{
	int k_num;				//�ڐG���ԍ�
	int p1;					//���i1
	int p2;					//���i2
	int p3;					//�g�����i
	int c1[MaxN];			//���i1�̍\�����i
	int c2[MaxN];			//���i2�̍\�����i
	int c3[MaxN];			//���i2�̍\�����i
	int flug[MaxN];			//�ڐG���̃t���O
	int Sessyoku;			//�g�p����Ă���ڐG���ԍ��̍ŏ��l
	char Dir[3];			//�e�g�t���̑g�t������
	int CS_L[MaxN];			//�g�t�����Ƃ̐ڐG�L����Ԃ̕]��(��)
	int CS_H[MaxN];			//�g�t�����Ƃ̐ڐG�L����Ԃ̕]��(����)
	int Contact_Space_L;	//�g�t�����Ƃ̐ڐG�L����Ԃ̕]��(��)
	int Contact_Space_H;	//�g�t�����Ƃ̐ڐG�L����Ԃ̕]��(����)
	int Contact_Space;		//�ڐG�L����Ԃ̕]��
	int Special_Assembly;	//�َ�g�t���̕]��
	int Assembly_Power;		//�g�t���͂̕]��
	int Asymmetric;			//����p���ʒu���߂̕]��
	int Contact_Point;		//�ڐG�_

}A_Kumituke[MaxKN];

//�O���[�o���ϐ��̐錾
char SEIHIN_Name[STR_MAX], inPath_kumitsuke_Dir[STR_MAX], inPath_jigmat[STR_MAX], outDir[STR_MAX], inPath_buhin_zokusei[STR_MAX], inPath_buhin_tokusei[STR_MAX], inPath_sessyokusen_tokusei[STR_MAX], inPath_kumitsuke_list[STR_MAX], inPath_tyukan_zokusei[STR_MAX], inPath_input[STR_MAX];
char inPath_kumitsukeyouin[STR_MAX], inPath_buhinyouin[STR_MAX], inPath_kumituke[STR_MAX],inPath_Sessyoku[STR_MAX];
int buhinsu = 0, sessyokusensu = 0, kumitsukesu = 0, tyukansu = 0;//���i���A�ڐG���̑����A�g�t���̑����A���Ԑ��i�̑���
int BP_count = 0, BH1_count = 0, Tegumi_count = 0, BH2_count = 0;
int /*BP[MaxMM], */BH1[MaxMM], Tegumi[MaxMM], BH2[MaxMM];//�e�����Ƃɕ��ނ��ꂽ�g�t���̊i�[��
int BH2_Parts[MaxN]/*���Â炳���Ȃ�����2���i���̕��i�Q*/, BH2P_count = 0/*���Â炳���Ȃ�����2���i���̕��i�̃J�E���g*/;
int SPair_count/*�����ɕt����ڐG���̃y�A���J�E���g*/, SPair_count_sum = 0/*�ڐG���̃y�A�̑���*/;
int kouguparts_num = 0;//�H���p���镔�i�̐�
int B_youinsu = 0;//���i�����v���]���̐�
int mode = 0;
int kumi = 0;//�g�t���̐�
int N;//���i��
int K;//�g�t����
int S;//�ڐG����
int File_Name;			//���o�`���i�ԗ��F1�A���i�Q�F2�A����@�F3�j


int main(int argc, char *argv[])
{
	FILE *fp1;//������
	char NowPath[STR_MAX]; //�ŏI�I�Ƀp�����[�^�t�@�C���̃t���p�X������
	int i = 0;

	//���̃v���O�����̂���p�X���擾���āANowPath�ɓ����
	GetNowPath(NowPath, argv[0]);
	strcat(NowPath, "Input\\");	//�p�����[�^�t�@�C���̒u�����.exe������t�H���_����Input�Ƃ���
	strcat(NowPath, DEF_PAR_FNAME); //NowPath �� �p�����[�^�t�@�C�������������āA�t���p�X�ɂ���

/************************************************************************************************/
/*�p�����[�^�t�@�C�����J��*/
/************************************************************************************************/
	// �p�����[�^�t�@�C���̎d�l
	//  1�s�ځF���i��
	//  2�s�ځF�t�@�C���̓��o�`��
	//  3�s�ځF���͂���t�H���_�̃p�X
	//  4�s�ځF�o�͂���t�H���_�̃p�X

	fp1 = fopen(NowPath, "r");
	if (fp1 == NULL) ShowFinMsg("�p�����[�^�t�@�C��������܂���"); // �t�@�C�����J���Ȃ���ΏI��
	if (fscanf(fp1, "%s", SEIHIN_Name) == EOF) ShowFinMsg("���i�����s���ł�");
	if (fscanf(fp1, "%d", &File_Name) == EOF) ShowFinMsg("���o�`������͂��Ă�������");
	if (fscanf(fp1, "%s", inPath_input) == EOF) ShowFinMsg("input�̃A�h���X���s���ł�");
	sprintf(inPath_buhin_zokusei, "%s\\%s_���i����.csv", inPath_input, SEIHIN_Name);
	//sprintf(inPath_kumitsuke_Dir, "%s\\%s_�ڐG�֌W_�g���ʒu���.csv", inPath_input, SEIHIN_Name);
	//sprintf(inPath_kumitsuke_list, "%s\\%s_�g�t�\.csv", inPath_input, SEIHIN_Name);

	sprintf(inPath_Sessyoku, "%s\\%s_�ڐG�֌W_�g���ʒu���.csv", inPath_input, SEIHIN_Name);

	strcpy(inPath_kumituke, inPath_input);
	strcat(inPath_kumituke, "\\");
	strcat(inPath_kumituke, SEIHIN_Name);

	if (File_Name == 1)//�ԗ��̂Ƃ�
	{
		strcat(inPath_kumituke, "_�ԗ�_�g�t��.csv");
	}
	else if (File_Name == 2)//���i�Q�̂Ƃ�
	{
		strcat(inPath_kumituke, "_���i�Q_�g�t��.csv");
	}
	else if (File_Name == 3)//�����̂Ƃ�
	{
		strcat(inPath_kumituke, "_����_�g�t��.csv");
	}


	
	if (fscanf(fp1, "%s", outDir) == EOF) ShowFinMsg("�o�͐�t�H���_�̃p�X���s���ł�");
	fclose(fp1);
	//�e�t�@�C����ǂݍ���
	printf("�C���v�b�g�t�@�C��%s\n", inPath_Sessyoku);
	//puts("�C���v�b�g�t�@�C���@�ڐG�֌W_�g���ʒu���");
	Read_Sessyoku();

	printf("�C���v�b�g�t�@�C��%s\n", inPath_buhin_zokusei);
	//puts("�C���v�b�g�t�@�C���@���i����");
	Read_buhin_zokusei();
	//puts("�g�t������");
	//Read_kumitsuke_Dir();

	printf("�C���v�b�g�t�@�C��%s\n", inPath_kumituke);
	//puts("�C���v�b�g�t�@�C���@�g�t��");
	Read_kumituke();
	

	//���蓯���g�t�����X�g���쐬����
	Make_BothHands_Kumitukelist();
	//puts("�A�E�g�v�b�g�t�@�C���@����g�t���X�g���쐬���܂���");

}

/*�e�t�@�C����ǂݍ��ފ֐�------------------------------------------------------------------------------------------------------------------------------------------------------*/

void Read_Sessyoku()
{
	FILE* fp1;

	char* temp;
	
	temp = (char*)malloc(20000);

	int i = 0;

	fp1 = fopen(inPath_Sessyoku, "r");//�ڐG�֌W�g���ʒu���̃t�@�C����ǂݍ��݃��[�h�ŊJ��
	if (fp1 == NULL) {
		if (i != 0) { printf("\n"); }
		ShowFinMsg("�ڐG�֌W_�g���ʒu���̃t�@�C�����J���܂���B\n�V�X�e�����I�����܂��B\n");	// �t�@�C�����J���Ȃ���ΏI��
	}

	fgets(temp, FGETS_MAX_LENGTH, fp1);

	fgets(temp, FGETS_MAX_LENGTH, fp1);
	sscanf(temp, "%d", &S);//�ڐG������ǂݍ���


	free(temp);
	fclose(fp1);

}

void Read_buhin_zokusei()
{
	FILE *fp;
	char a[FGETS_MAX_LENGTH], b[FGETS_MAX_LENGTH];
	char* temp;

	temp = (char*)malloc(20000);

	int c = 0, j = 0;
	double d = 0, e = 0, f = 0, g = 0, h = 0, i = 0, k = 0;
	fp = fopen(inPath_buhin_zokusei, "r");

	fgets(temp, FGETS_MAX_LENGTH, fp);

	fgets(temp, FGETS_MAX_LENGTH, fp);
	sscanf(temp, "%d", &N);

	while (fgets(temp, FGETS_MAX_LENGTH, fp) != NULL)
	{
		Del_Com(temp);
		/*���i�ԍ��A���S�_�A���i�̑傫���̓ǂݎ��*/
		sscanf(temp, "%d%s%s%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%lf%d%s%d%d%d%d%d", &c, &a, &b, &buhinzokusei[buhinsu].Parts, &buhinzokusei[buhinsu].centerX, &buhinzokusei[buhinsu].centerY, &buhinzokusei[buhinsu].centerZ, &buhinzokusei[buhinsu].lengthX, &buhinzokusei[buhinsu].lengthY, &buhinzokusei[buhinsu].lengthZ,
			&d, &e, &f, &g, &h, &i, &j, &buhinzokusei[buhinsu].Weight, &buhinzokusei[buhinsu].Tool,&a,&c,&buhinzokusei[buhinsu].KouguType,&c,&buhinzokusei[buhinsu].Okikata,&buhinzokusei[buhinsu].Atsukai);

		//printf("%d,%s,%s,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%d,%lf,%d\n", c, a, b, buhinzokusei[buhinsu].Parts, buhinzokusei[buhinsu].centerX, buhinzokusei[buhinsu].centerY, buhinzokusei[buhinsu].centerZ, buhinzokusei[buhinsu].lengthX, buhinzokusei[buhinsu].lengthY, buhinzokusei[buhinsu].lengthZ,
			//d, e, f, g, h, i, j, buhinzokusei[buhinsu].Weight, buhinzokusei[buhinsu].Tool);

		if (buhinzokusei[buhinsu].Tool != 0)//�H���p���镔�i�̎�
		{
			KouguParts[kouguparts_num].parts = buhinzokusei[buhinsu].Parts;
			KouguParts[kouguparts_num].tool = buhinzokusei[buhinsu].Tool;
			KouguParts[kouguparts_num].okikata = buhinzokusei[buhinsu].Okikata;
			KouguParts[kouguparts_num].atsukai = buhinzokusei[buhinsu].Atsukai;
			KouguParts[kouguparts_num].kougutype = buhinzokusei[buhinsu].KouguType;
			kouguparts_num++;
		}

		buhinsu++;//���i�����L�^
	}
	free(temp);
	fclose(fp);
}

void Read_kumitsuke_Dir()
{
	FILE *fp;

	char* temp;

	temp = (char*)malloc(20000);

	fp = fopen(inPath_kumitsuke_Dir, "r");

	fgets(temp, FGETS_MAX_LENGTH, fp);
	fgets(temp, FGETS_MAX_LENGTH, fp);
	while (fgets(temp, FGETS_MAX_LENGTH, fp) != NULL)
	{
		Del_Com(temp);
		sscanf(temp, "%d%d%d%s", &kumitsuke_Dir[sessyokusensu].Num, &kumitsuke_Dir[sessyokusensu].Parts1, &kumitsuke_Dir[sessyokusensu].Parts2, &kumitsuke_Dir[sessyokusensu].Dir);
		//printf("%d,%d,%d,%s\n", kumitsuke_Dir[sessyokusensu].Num, kumitsuke_Dir[sessyokusensu].Parts1, kumitsuke_Dir[sessyokusensu].Parts2, kumitsuke_Dir[sessyokusensu].Dir);
		sessyokusensu++;
	}
	free(temp);
	fclose(fp);
}

void Read_kumituke()
{
	FILE* fp1;

	char* temp;

	temp = (char*)malloc(20000);

	int i = 0;

	char * s;
	int trash;


	fp1 = fopen(inPath_kumituke, "r");//�ڐG�֌W�g���ʒu���̃t�@�C����ǂݍ��݃��[�h�ŊJ��
	if (fp1 == NULL) {
		if (i != 0) { printf("\n"); }
		ShowFinMsg("�g�t���̃t�@�C�����J���܂���B\n�V�X�e�����I�����܂��B\n");	// �t�@�C�����J���Ȃ���ΏI��
	}

	fgets(temp, FGETS_MAX_LENGTH, fp1);//1�s�ǂݍ���

	int j = 1;

	while (fgets(temp, FGETS_MAX_LENGTH, fp1) != NULL)//�g�t���̃t�@�C���̍Ō�܂ŌJ��Ԃ�
	{
		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
		s = temp;

		sscanf(s, "%d", &A_Kumituke[j].k_num); s = strchr(&s[1], ' ');	//�g�t���ԍ���ǂݍ���	//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
		sscanf(s, "%d", &trash); s = strchr(&s[1], ' ');				//�s�v�ȏ���ǂݍ���	//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
		sscanf(s, "%d", &A_Kumituke[j].p1); s = strchr(&s[1], ' ');	//���i1�̕��i�ԍ���ǂݍ���	//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
		sscanf(s, "%d", &A_Kumituke[j].p2); s = strchr(&s[1], ' ');	//���i2�̕��i�ԍ���ǂݍ���	//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
		sscanf(s, "%d", &A_Kumituke[j].p3); s = strchr(&s[1], ' ');	//�g�����i�̕��i�ԍ���ǂݍ���	//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���

		//���i1�̍\�����i�̓ǂݍ���
		for (int k = 0; k < N; k++)	//���i���J��Ԃ�
		{
			sscanf(s, "%d", &A_Kumituke[j].c1[k]); s = strchr(&s[1], ' ');		//�\�����i�̕��i�ԍ���ǂݍ���	//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
		}

		//���i2�̍\�����i�̓ǂݍ���
		for (int k = 0; k < N; k++)	//���i���J��Ԃ�
		{
			sscanf(s, "%d", &A_Kumituke[j].c2[k]); s = strchr(&s[1], ' ');		//�\�����i�̕��i�ԍ���ǂݍ���	//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
		}

		//�g�����i�̍\�����i�̓ǂݍ���
		for (int k = 0; k < N; k++)	//���i���J��Ԃ�
		{
			sscanf(s, "%d", &A_Kumituke[j].c3[k]); s = strchr(&s[1], ' ');		//�\�����i�̕��i�ԍ���ǂݍ���	//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
		}

		//��������ڐG���ԍ��̓ǂݍ���

		for (int k = 1; k <= S; k++)	//�ڐG�����J��Ԃ�
		{
			sscanf(s, "%d", &A_Kumituke[j].flug[k]); s = strchr(&s[1], ' ');		//�ڐG���̑��݃t���O����ǂݍ���	//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ�

			if (A_Kumituke[j].flug[k] == 1)	//���݃t���O���u1�v�̂Ƃ�
			{
				A_Kumituke[j].Sessyoku = k;	//�ڐG���ԍ�����

				break;	//for���𔲂���=�ŏ��ɗ����Ă���t���O���i�[
			}
		}

		j++;	//�z��̒l�̒ǉ�
	}

	K = j;//�g�t�����̊i�[

	free(temp);
	fclose(fp1);
}

/*����p�g�t�����X�g���쐬����֐�--------------------------------------------------------------------------------------------------------------------------------------------------*/
void Make_BothHands_Kumitukelist()
{
	FILE *fp;//�o�̓t�@�C��
	char WkD[STR_MAX], *fname = WkD;
	int kouguflag = 0;

	//�o�͐�̃t���p�X��WkD�ɑ������
	strcpy(WkD, outDir);
	strcat(WkD, "\\");
	strcat(WkD, SEIHIN_Name);
	strcat(WkD, "_����g�t�����X�g.csv");

	fp = fopen(fname, "w");
	printf("�A�E�g�v�b�g�t�@�C���@%s\n", fname);
	Write_KouguParts(fp);

	fclose(fp);

}

/*�H��g�t����t�^����֐�--------------------------------------------------------------------------------------------------------------------------------------------------*/
void Write_KouguParts(fp)
{
	int i, j;
	//printf("��g�g�t���̌����o�͂���i�J�n�j\n");

	fprintf(fp, "�H��\n");
	fprintf(fp, "���݃t���O,�ڐG���ԍ�,�g�t���ԍ�,�e���i,�q���ԍ�,�H�,�H��p�^�[��\n");
	//printf("�g�t���̐��@%d�@\n\n", K);
	for (i = 0; i < K; i++)
	{
		for (j = 0;j < kouguparts_num;j++)
		{
			if (A_Kumituke[i].p1 == KouguParts[j].parts)
			{
				fprintf(fp, "%d,%d,%d,%d,%d,%d,%d\n", Flag_num, A_Kumituke[i].Sessyoku, A_Kumituke[i].k_num, A_Kumituke[i].p2, A_Kumituke[i].p1, KouguParts[j].tool, -2);//�H��p�^�[����2�ɂ���
			}
			else if (A_Kumituke[i].p2 == KouguParts[j].parts)
			{
				fprintf(fp, "%d,%d,%d,%d,%d,%d,%d\n", Flag_num, A_Kumituke[i].Sessyoku, A_Kumituke[i].k_num, A_Kumituke[i].p1, A_Kumituke[i].p2, KouguParts[j].tool, -2);//�H��p�^�[����2�ɂ���
			}
		}
	}
}

//�V�X�e���𓮂�����ŕK�v�ƂȂ�֐�------------------------------------------------------------------------------------------------------------
void ShowFinMsg(char *msg) 
{
	int c;
	printf("%s\n", msg);
	printf("(Enter�L�[�������Ă��������B)");
	c = getchar();
	exit(1);
}

void Del_Com(char *s)//�ǂݍ��񂾕�����́u,�v�i�J���}�j���u �v�i���p�X�y�[�X�j�ɒu��������֐�
{
	int	i;

	i = 0;
	while (s[i] != '\0') {
		if (s[i] == ',') s[i] = ' ';
		i++;
	}
}

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