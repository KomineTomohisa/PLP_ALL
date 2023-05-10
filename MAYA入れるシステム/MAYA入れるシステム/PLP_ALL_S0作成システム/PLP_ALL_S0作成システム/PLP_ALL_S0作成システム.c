#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<conio.h>

//�萔�̐錾 --------------------------------------------------
#define SysVer "Ver.1.10 2023/01/21"
#define PW "plp0831"
#define PWNum 7
#define M_PI 3.14159;
#define DEF_PAR_FNAME "PLP_ALL_S0�쐬�V�X�e���p�����[�^.txt"					//�g�p����p�����[�^�t�@�C���̖���
#define STR_MAX 500														//�����̍ő�̕��i����500�Ƃ���
#define FGETS_MAX_LENGTH 20000											//�����̍ő啶������20000�Ƃ���
#define MaxMM 50000														//��舵���ő�̑g�t���⓮��̐���50000�Ƃ���
#define MaxM 50000				//��舵���ő�̒��Ԑ��i����50000�Ƃ���
#define Max_Junjo 200000
#define MaxN 8000				//��舵���ő�̕��i����8000�Ƃ���
#define MaxKN 50000 
#define MaxKM 200


//�p�X���[�h�֌W
#define PWNum 8
#define SFTN 3	 //�������V�t�g���鐔

char SEIHIN_Name[STR_MAX], outDir[STR_MAX];
char inPath_Input[STR_MAX];
char inPath_Input_Junjo[STR_MAX];
char inPath_Input_Buhin[STR_MAX];
char NowPath[STR_MAX];
char OutPath[STR_MAX];
int s_File;		//�t�@�C���̍ő�ԍ�
int D_model;    //���o�`��
int N;						//�ǂݍ��񂾐��i�̕��i��
int K;			//�H�
int J;			//���


//�v���g�^�C�v�錾 --------------------------------------------------

//�K���K�v�Ȃ���

void GetNowPath(char* NP, char* S);		//���݂̃t�@�C������肷��֐�
void ShowFinMsg(char*);					//�������o�́H����֐�
void ReadPara(char* S);					//�g�p����p�����[�^�t�@�C����ǂݍ��ފ֐�
void Del_Com(char*);
char* Conv_Char(int);//int�^�̕ϐ���char�^�̕�����ɕϊ�����֐�

//����̊֐�
void Read_BZ();							//���i������ǂݍ��ފ֐�
void Read_Junjo();					//�g��������ǂݍ��ފ֐�

int Jig_Count(int);						//����̌����J�E���g

void Write_S0();						//S0�������o���֐�

//�p�X���[�h//
int PWCheck(int, char*);
void PWG(char*);

//***�\����***//

//���i�����p�̍\����
struct Buhin
{
	int Bu_num;//����
	char Buhin_Name[STR_MAX];//���i��
	char CAD_file[STR_MAX];//CAD�t�@�C��
	int Buhin_num;//���i�ԍ�
	double Center_X;//���SX
	double Center_Y;//���SY
	double Center_Z;//���SZ
	double Length_X;//���i�̒����iX�����j
	double Length_Y;//���i�̒����iY�����j
	double Length_Z;//���i�̒����iZ�����j
	double Xmin;
	double Xmax;
	double Ymin;
	double Ymax;
	double Zmin;
	double Zmax;
	int Color_num;//�F�ԍ�
	double weight;//�d��
	int kogu_num;//�H��ԍ�
	int kogu_name;//�H�
	int situkan;//����
	int tokutyo;//����(1:�˂��A2:O�AC�ݸށA3:E�ݸށA4:�΂ˁA5:ů�)
	int OK;
	int Size_Small;//������
	int Thin;//����
	int Size_Big;//�傫��
	int Weight;//�d��
	int Handling_Careful;//�戵���ӕ��i
	int Shape_Change;//�n�[�l�X
	int Misassembly_dir;//��g�t�����Ӂi�����j
	int Misassembly_sim;//��g�t�����Ӂi�ގ��j
};
struct Buhin R_B[MaxN];

struct KJUNJO {
	long long kumij;			//�g�������ԍ�
	int kuminum[MaxKN];	//�g�t���ԍ�
	int p1[MaxKN];	//���i1
	int p2[MaxKN];	//���i2
	int p3[MaxKN];	//�g���ĕ��i
	char Oya_Pos1[MaxKN][3];
	char Oya_Pos2[MaxKN][3];
	int Jig[MaxKN];
	int KouguNum[MaxKN];
	int DousaNum[MaxKN];
};
struct KJUNJO R_KJUNJO;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// �v���O��������
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void main(int argc, char* argv[])
{
	printf("argv[0]:%s\n", argv[0]);		//.exe���o��
	printf("NowPath:%s\n", NowPath);		//NowPath���o��
	GetNowPath(NowPath, argv[0]);			//NowPath����肷��

	strcat(NowPath, "Input\\");				//�p�����[�^�t�@�C���̒u�����.exe������t�H���_����Input�Ƃ���
	strcat(NowPath, DEF_PAR_FNAME);			//�p�����[�^�t�@�C����T��
	printf("\n");							//���s
	printf("NowPath:%s\n", NowPath);		//NowPath���o��
	printf("\n");							//���s

	/************************************************************************************************/
	/*�p�X���[�h�̔F��*/
	/************************************************************************************************/
	//if (PWCheck(argc, argv[1]) == 0) return 0; //�p�X���[�h����v���Ȃ�������0���Ԃ��Ă��� �� �v���O�����̏I�� (= return 0)

	ReadPara(NowPath);						//�p�����[�^�t�@�C����ǂݍ���

}

void ReadPara(char* NowPath)
{
	FILE* fp1;//�t�@�C���|�C���^�̐錾


	/************************************************************************************************/
	/*�p�����[�^�t�@�C�����J��*/
	/************************************************************************************************/
	// �p�����[�^�t�@�C���̎d�l
	//  1�s�ځF���i��
	// 	2�s�ځF�g�������t�@�C���̓��o�`��
	//  3�s�ځF�g�������t�@�C���̍ő�ԍ�
	//  4�s�ځF���͂���t�H���_�̃p�X
	//  5�s�ځF�o�͂���t�H���_�̃p�X


	fp1 = fopen(NowPath, "r");
	if (fp1 == NULL) ShowFinMsg("�p�����[�^�t�@�C��������܂���");				 // �t�@�C�����J���Ȃ���ΏI��
	if (fscanf(fp1, "%s", SEIHIN_Name) == EOF) ShowFinMsg("���i�����s���ł�");
	if (fscanf(fp1, "%d", &D_model) == EOF) ShowFinMsg("���o�`������͂��Ă�������");
	if (fscanf(fp1, "%d", &s_File) == EOF) ShowFinMsg("�t�@�C���̍ő�ԍ�����͂��Ă�������");
	if (fscanf(fp1, "%s", inPath_Input) == EOF) ShowFinMsg("���͐�t�H���_�̃p�X���s���ł�");
	if (fscanf(fp1, "%s", outDir) == EOF) ShowFinMsg("�o�͐�t�H���_�̃p�X���s���ł�");
	fclose(fp1);

	//"���i����"�ǂݍ��ݗp�p�X�쐬
	strcpy(inPath_Input_Buhin, inPath_Input);
	strcat(inPath_Input_Buhin, "\\");
	strcat(inPath_Input_Buhin, SEIHIN_Name);
	strcat(inPath_Input_Buhin, "_���i����.csv");

	//Output�p�p�X�쐬
	strcpy(OutPath, outDir);
	strcat(OutPath, "\\");
	strcat(OutPath, SEIHIN_Name);

	//"�g������"�ǂݍ��ݗp�p�X�쐬
	strcpy(inPath_Input_Junjo, inPath_Input);
	strcat(inPath_Input_Junjo, "\\");
	strcat(inPath_Input_Junjo, SEIHIN_Name);

	if (D_model == 1)//�ԗ�
	{
		strcat(inPath_Input_Junjo, "_�ԗ�_����t�g������");
		strcat(OutPath, "_�ԗ�_���C�A�E�g.csv");
	}
	else if (D_model == 2)//���i�Q
	{
		strcat(inPath_Input_Junjo, "_���i�Q_����t�g������");
		strcat(OutPath, "_���i�Q_���C�A�E�g.csv");
	}
	else//����
	{
		strcat(inPath_Input_Junjo, "_����_����t�g������");
		strcat(OutPath, "_����_���C�A�E�g.csv");
	}

	printf("���i���F%s\n", SEIHIN_Name);
	for (int j = 1; j <= 3; j++)
	{
		if (j == D_model)
		{
			if (j == 1)//�ԗ�
			{
				printf("���o�`���F�ԗ�\n");
			}
			else if (j == 2)//���i�Q
			{
				printf("���o�`���F���i�Q\n");
			}
			else//����
			{
				printf("���o�`���F����\n");
			}
		}
	}
	printf("�t�@�C���̍ő�ԍ��F%d\n", s_File);
	printf("Input(�g������)���F%s\n", inPath_Input_Junjo);
	printf("Input(���i����)���F%s\n", inPath_Input_Buhin);
	printf("Output���F%s\n\n", OutPath);

	//���i�����̓ǂݍ���
	Read_BZ();
	printf("���i�����ǂݍ��݊���\n");

	//�����]���t�@�C���̓ǂݍ���
	Read_Junjo();
	printf("�g�������ǂݍ��݊���\n");

	//S0�쐬�֐�
	Write_S0();
	printf("\nS0�̍쐬���������܂����B\n");

	//�I���̏���
	printf("(Enter�L�[�������Ă�������)\n");
	unsigned char key;
	while (1) {
		key = getch();
		if (key == 0x0d) break;
	}
}

void Read_Junjo()
{
	FILE* fp1;

	char* temp;

	int First_Time = 0;//�n�߂͔�΂�
	int flg;			//�t���O
	int j;
	int Knumber;
	int Jig;
	int temp_Jig = 0;
	char K[STR_MAX];

	temp = (char*)malloc(20000);

	for (int file = 0; file <= s_File; file++)
	{
		
		strcat(inPath_Input_Junjo, Conv_Char(file));
		strcat(inPath_Input_Junjo, ".csv");

		fp1 = fopen(inPath_Input_Junjo, "r");						//NowPath�̃t�@�C�����J���āA�ǂݍ���
		if (fp1 == NULL)ShowFinMsg("�g������������܂���");		//�t�@�C�����J���Ȃ���ΏI��

		First_Time = 0;

		while (fgets(temp, FGETS_MAX_LENGTH, fp1) != NULL)//�g�������̃t�@�C���̍Ō�܂ŌJ��Ԃ�
		{
			Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
			if (strstr(temp, "K") != NULL) //�ǂݍ��񂾍s��K�̕���������Ƃ�
			{
				if (First_Time != 0) 
				{
					Knumber = j - 1;

					Jig = Jig_Count(Knumber);

					if (Jig >= temp_Jig)
					{
						temp_Jig = Jig;
					}

				}
				else //�ŏ���K�̂Ƃ�
				{
					First_Time++;
				}

				j = 1;
				sscanf(temp, "%c%lld%d", &K, &R_KJUNJO.kumij, &flg);//K,�g�������ԍ�,���݃t���O�̓ǂݍ���

			}
			else
			{
				sscanf(temp, "%d%d%d%d%s%s%d%d%d", &R_KJUNJO.kuminum[j], &R_KJUNJO.p1[j], &R_KJUNJO.p2[j], &R_KJUNJO.p3[j], R_KJUNJO.Oya_Pos1[j], R_KJUNJO.Oya_Pos2[j], &R_KJUNJO.Jig[j], &R_KJUNJO.KouguNum[j], &R_KJUNJO.DousaNum[j]);

				j++;
			}

		}

		fclose(fp1);

	}

	J = temp_Jig;//�i�[

	free(temp);
}

void Read_BZ()
{
	FILE* fp1;

	int temp_Kougu;
	int flag;
	int count = 0;

	char* temp, * s;

	temp = (char*)malloc(20000);

	fp1 = fopen(inPath_Input_Buhin, "r");						//NowPath�̃t�@�C�����J���āA�ǂݍ���
	if (fp1 == NULL)ShowFinMsg("���i����������܂���");		//�t�@�C�����J���Ȃ���ΏI��

	//�ȍ~�́A�J�����t�@�C�������i�����ł���O��ŏ���������

	//1�s�ڂ̓ǂݍ���
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	//2�s�ڂ̓ǂݍ���
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	sscanf(temp, "%d", &N);

	for (int i = 1; i <= N; i++)
	{
		flag = 0;

		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
		s = temp;

		sscanf(s, "%d%s%s%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%d%d%d%d%d%d", &R_B[i].Bu_num, &R_B[i].Buhin_Name, &R_B[i].CAD_file, &R_B[i].Buhin_num, &R_B[i].Center_X, &R_B[i].Center_Y, &R_B[i].Center_Z, &R_B[i].Length_X, &R_B[i].Length_Y, &R_B[i].Length_Z, &R_B[i].Xmin, &R_B[i].Xmax, &R_B[i].Ymin, &R_B[i].Ymax, &R_B[i].Zmin, &R_B[i].Zmax, &R_B[i].Color_num, &R_B[i].weight, &R_B[i].kogu_num, &R_B[i].kogu_name, &R_B[i].situkan, &R_B[i].tokutyo, &R_B[i].OK);

		//�H��J�E���g�p�̏���
		if (R_B[i].kogu_num != 0)
		{
			for (int j = i - 1; j >= 1; j--)
			{
				if (R_B[j].kogu_num == R_B[i].kogu_num)//���܂łɂ��̍H��p�����Ă���Ƃ�
				{
					flag = 1;
					break;
				}
			}
			if (flag == 0)//���܂łɂ��̍H�����x���g�p���Ȃ��Ƃ�
			{
				temp_Kougu = R_B[i].kogu_num;
				count++;
			}
		}

	}
	K = count;//�i�[

	free(temp);

	fclose(fp1);
}

int Jig_Count(int Knumber)
{
	int temp;
	int count = 0;//����J�E���g�p
	for (int i = 1; i <= Knumber; i++)
	{
		if (i == 1)
		{
			count++;
			temp = R_KJUNJO.Jig[i];
		}
		else
		{
			if (temp != R_KJUNJO.Jig[i])//����؂�ւ�����Ƃ�
			{
				count++;
			}
		}

	}

	return count;
}

void Write_S0()
{
	FILE* fp1;

	int Jig = -100;//����̊�ϐ�
	int Tray = 9000;//�g���C�̊�ϐ�
	int Kougu = -10;//�H��̊�ϐ�

	fp1 = fopen(OutPath, "w");						//Output�̃t�@�C�����J���āA��������

	//�w�b�_�̏o��
	fprintf(fp1, "1\n");										
	fprintf(fp1, "1,1,1\n");										//��ƎҔԍ�,���C���ԍ�,�H���ԍ�
	fprintf(fp1, "100,100,2,0,0\n");								//�X�e�b�v��,�L���X�e�b�v��,���x�w��,���`�F�b�N,��Ԕԍ�
	fprintf(fp1, "%d,%d,%d\n", N, K, J);							//���i��,�H�,���
	fprintf(fp1, "0,0,0\n");										//���ԕ��i��
	fprintf(fp1, "0,0,0\n");										//�������ԕ��i��
	fprintf(fp1, "1,%d,0\n\n", N + 1);								//���̐�,�g���C�̐�(���i���{���u����),�ʉߓ_��

	for (int i = 1; i <= 2 + N; i++)//���i�̏ڍׂ̏o��
	{
		if (i == 1)//�E��
		{
			fprintf(fp1, "0,200,0,845,0,0,0\n");
		}
		else if (i == 2)//����
		{
			fprintf(fp1, "0,-200,0,845,0,0,0\n");
		}
		else
		{
			fprintf(fp1, "%d,0,0,0,0,0,0\n", i - 2);
		}
	}

	for (int i = 0; i < K; i++)
	{
		fprintf(fp1, "%d,0,0,0,0,0,0\n", Kougu - i);
	}

	for (int i = 1; i <= J; i++)//����̏ڍׂ̏o��
	{
		fprintf(fp1, "%d,0,0,0,0,0,0,0,0,0,23,1\n",Jig-i);
	}
	
	fprintf(fp1, "8001,480,480,350,850,21,0,50,0,0\n");//���ڍׂ̏o��

	for (int i = 1; i <= N + 2; i++)//�g���C&���u����&�����i��
	{
		if (i == N + 1)
		{
			fprintf(fp1, "%d,1,1,12,0,0,0,5,0,0,0,0,0,180\n", Tray + i);
		}
		else if (i == N + 2)
		{
			fprintf(fp1, "%d,1,1,4,0,0,0,5,0,0,0,0,0,180\n", Tray + i);
		}
		else
		{
			fprintf(fp1, "%d,1,2,3,0,0,0,5,0,0,0,0,0,180\n", Tray + i);
		}
	}
	


}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//Work
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void GetNowPath(char* NP, char* S)
{
	char* p;
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

//�N�����̃p�X���[�h���m�F���邽�߂̊֐�
int PWCheck(int N, char* S)
{
	char pw[PWNum + 1];
	PWG(pw); //�p�X���[�h�𐶐�����pw�ɑ������

	if (N >= 2)
	{           /*������2�����Ȃ�p�X���[�h�͂Ȃ��̂ŋN�����Ȃ� (= return 0) */
		if (strncmp(S, pw, PWNum) == 0)
		{	/*�p�X���[�h����v������N������ (= return 1) */
			return 1;
		}
	}
	return 0; /*�N�����Ȃ� (= return 0) */
}

/******************************************************************************
'�p�X���[�h�������[��
'���s����yyyymmdd �� ddyyyymm
'dd ��3�Ŋ������]��� r �Ƃ���
'ddyyyymm��n�����ڂ��A
'ASCII�R�[�h�\��
'10 + r + 3 * (n - 1)
'�������炵�������ɕϊ�����
'******************************************************************************/
void PWG(char* s)
{
	time_t timer;
	struct tm* date;
	char str[PWNum + 1]; //8���� + null

	int i, r;

	timer = time(NULL);          /* �o�ߎ��Ԃ��擾 */
	date = localtime(&timer);    /* �o�ߎ��Ԃ����Ԃ�\���\���� date �ɕϊ� */

	//r�̌v�Z : ���t �� 3�Ŋ��������܂�
	strftime(str, PWNum + 1, "%d", date);
	r = atoi(str) % SFTN;

	//PW���� : ���t �� �p�X���[�h
	strftime(str, PWNum + 1, "%d%Y%m", date);
	for (i = 0; i < PWNum; i++)
	{
		s[i] = str[i] + 10 + r + SFTN * i;
	}
	s[i] = '\0';
}

void ShowFinMsg(char* msg) {
	printf("%s\n", msg);
	printf("(Enter�L�[�������Ă�������)\n");
	unsigned char key;
	while (1) {
		key = getch();
		if (key == 0x0d) break;
	}
}

//�ǂݍ��񂾕�����́u,�v�i�J���}�j���u �v�i���p�X�y�[�X�j�ɒu��������֐�
void Del_Com(char* s) {
	int	i;

	i = 0;
	while (s[i] != '\0') {
		if (s[i] == ',') s[i] = ' ';
		i++;
	}
}

//�ǂݍ��񂾕�����́u �v�i���p�X�y�[�X�j���u*�v�i�A�X�^���X�N�j�ɒu��������֐�
void Change_Space(char* s) {
	int	i;

	i = 0;
	while (s[i] != '\0') {
		if (s[i] == ' ') s[i] = '*';
		i++;
	}
}

//int�^�̕ϐ���char�^�̕�����ɕϊ�����֐�
char* Conv_Char(int no) {
	char buf[10];
	sprintf(buf, "%d", no);
	return buf;
}