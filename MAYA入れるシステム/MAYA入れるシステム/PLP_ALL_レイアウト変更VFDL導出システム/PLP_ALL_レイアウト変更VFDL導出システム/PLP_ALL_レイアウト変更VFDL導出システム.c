#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<conio.h>



//�萔�̐錾 --------------------------------------------------
#define SysVer "Ver.1.10 2021/01/26"
#define PW "plp0831"
#define PWNum 7
#define M_PI 3.14159;
#define DEF_PAR_FNAME "���C�A�E�g�ύXVFDL���o�V�X�e���p�����[�^.txt"					//�g�p����p�����[�^�t�@�C���̖���
#define STR_MAX 500														//�����̍ő�̕��i����500�Ƃ���
#define FGETS_MAX_LENGTH 20000											//�����̍ő啶������20000�Ƃ���
#define MaxMM 50000														//��舵���ő�̑g�t���⓮��̐���50000�Ƃ���
#define Max_VFDL 200000		
#define MaxKN 50000 
#define MaxKM 200
#define MaxExcel 1048576;												//Excel�̍ő�s��

//�p�X���[�h�֌W
#define PWNum 8
#define SFTN 3	 //�������V�t�g���鐔

//�O���[�o���ϐ��̐錾 --------------------------------------------------

char SEIHIN_Name[STR_MAX], outDir1[STR_MAX], outDir2[STR_MAX];
char inPath_Input1[STR_MAX];
char inPath_Input2[STR_MAX];
char inPath_Input_VFDL[STR_MAX];
char inPath_Input_Junjo[STR_MAX];
char inPath_Input_Buhin_Zokusei[STR_MAX];
char inPath_Input_Tyukan[STR_MAX];
char inPath_Input[STR_MAX];
char OutPath[STR_MAX];
char outPath_TimeHyouka[STR_MAX];

//char-------------------------------------------------------------------

char NowPath[STR_MAX];								//�ŏI�I�Ƀp�����[�^�t�@�C���̃t���p�X������
char SEIHIN_Name[STR_MAX];							//�n���h�����O�]���V�[�g���̐��i��������


//int,double--------------------------------------------------------------

int s_File;		//�t�@�C���̍ő�ԍ�
int D_model;    //���o�`��
int N;				//�S���i�̐�
int T;				//�S���Ԑ��i�̐�
int A;				//�ڐG���̐�
int K;				//�g�t���̐�
//int VFDL_Count;		//VFDL�̐�
int G;				//�����ʒu�̍s��
int STEPNUM = 0;	//�X�e�b�v��
int Jignum = 0;	    //���
int Beforejignum;//S0�ɂ����Ď���̏ڍׂɓ���O�̐�
int Beforetraynum;//S0�ɂ����ăg���[�̏ڍׂɓ���O�̐�
int Beforefinishboxnum;//S0�ɂ����Ċ����i���̏ڍׂɓ���O�̐�
int Beforekougunum;//S0�ɂ����čH��̏ڍׂɓ���O�̐�
int c = 0;
int countJ = 0;//�������Ƃ̎���̃J�E���g�p
int countF = 0;//�O��ɔz�u���镔�i�����J�E���g
int Reach_count = 0;//"�L�΂�"�J�E���g�p
int Bring_count = 0;//"�^��"�J�E���g�p
int Turn_count = 0;//"��������"�J�E���g�p
int Grab_count = 0;//"�͂ݎ��","�͂�"�J�E���g�p
int Release_count = 0;//"����"�J�E���g�p
int Leave_count = 0;//"�u������"�J�E���g�p
int Attach_count = 0;//"�t����"�J�E���g�p
int Remove_count = 0;//"�O��"�J�E���g�p

double STime_Reach;//"�L�΂�"���Ԓl�����]���p
double STime_Bring;//"�^��"���Ԓl�����]���p
double STime_Turn;//"��������"���Ԓl�����]���p
double STime_Grab;//"�͂ݎ��","�͂�"���Ԓl�����]���p
double STime_Release;//"����"���Ԓl�����]���p
double STime_Leave;//"�u������"���Ԓl�����]���p
double STime_Attach;//"�t����"���Ԓl�����]���p
double STime_Remove;//"�O��"���Ԓl�����]���p

double All_Angle = 0;//����]�p�x

double R_Kyori;//�E��ړ�����
double L_Kyori;//����ړ�����




//�v���g�^�C�v�錾 --------------------------------------------------

//�K���K�v�Ȃ���

void GetNowPath(char* NP, char* S);		//���݂̃t�@�C������肷��֐�
void ShowFinMsg(char*);					//�������o�́H����֐�
void ReadPara(char* S);					//�g�p����p�����[�^�t�@�C����ǂݍ��ފ֐�
void Del_Com(char*);
char* Conv_Char(int);//int�^�̕ϐ���char�^�̕�����ɕϊ�����֐�


//***����̊֐�***//

//Input���ǂݍ��ݗp
void Read_VFDL();						//VFDL��ǂݍ��ފ֐�

void Read_Buhin_Zokusei();				//���i������ǂݍ��ފ֐�

void Read_Tyukan();						//���Ԑ��i������ǂݍ��ފ֐�

void Read_Junjo(int);						//�g��������ǂݍ��ފ֐�

//Output���ǂݍ��ݗp
void Write_VFDL(int, int*, int);             //VFDL�������o���֐�

void Write_TimeHyouka(int, FILE*);				//���Ԓl�]���������o���֐�


//���Ԓl�v�Z�p
void Time_Reach(int);						//�u�L�΂��v�̎��Ԓl���o�֐�

void Time_Bring(int);						//�u�^�ԁv�̎��Ԓl���o�֐�

void Time_grab(int);						//�u�͂ݎ��v�̎��Ԓl���o�֐�

void Time_Assemble(int);					//�u�g���Ă�v�̎��Ԓl���o�֐�

void Time_Turn(int);						//�u���������v�̎��Ԓl���o�֐�


//���C�A�E�g�p�֐�
void BoxJig_Hyouka();						//���i���E����]���p�֐�

void S0_Change(int);						//S0�ύX�p�֐�

void C_Change(int);							//�ω����̃��C�A�E�g�ύX�p�֐�

void Kyori_Change(int);						//���싗���̕ύX�p�֐�

//���בւ��p
void swap(int);

//�g�t���ʒu�ύX�֐�
void Kumituke_Position(int);				//�g�t���ʒu���i�[����֐�

//�p�X���[�h//
int PWCheck(int, char*);
void PWG(char*);

//***�\����***//

//�w�b�_�Ɋւ���\����
struct H
{
	int W;		//��ƎҔԍ�
	int L;		//���C���ԍ�
	int P;		//�H���ԍ�
	int H;		//�X�e�b�v��
	int Ht;		//�L���X�e�b�v��
	int V;		//���x�w��
	int C;		//���`�F�b�N
	int S;		//��Ԕԍ�
	int PA;		//���i��
	int T;		//�H�
	int J;		//���
	int MI;		//���ԕ��i��
	int MO;		//�������ԕ��i��
	int MO1;
	int MO2;
	int TA;		//���̐�
	int TR;		//�g���C�̐�
	int WD;		//�ʉߓ_��
	int RA;		//�I�̐�
};
struct H R_H;

//�����ʒu�Ɋւ���\����
struct S0
{
	//int�^
	int N;//���ʔԍ�
	int C;//�F�ԍ�
	int S;//�������
	int X;
	int Y;
	//double�^
	double lx;//�ʒux
	double ly;//�ʒuy
	double lz;//�ʒuz
	double Hx;//����x
	double Hy;//����y
	double Hz;//����z
	double Bx;//�u���b�N�`��̃T�C�Yx
	double By;//�u���b�N�`��̃T�C�Yy
	double Bz;//�u���b�N�`��̃T�C�Yz

};
struct S0 R_S0[100];

//�ω��Ɋւ���\����
struct C
{
	//int�^
	int SEQ;	//�ω��̘A��
	int NF;		//���̌�ɑ������߂̐�
	int HL;		//�n�C���C�g�\�������镔�i�ԍ�
	int Flag;	//�{���̃X�e�b�v��1,VFDL�쐬�ɔ����֋X�㐶�����X�e�b�v��0
	int AC;		//���L�̐F
	int AS;		//���L�̕���
	int TurnNum;
	//double�^
	double T;		//�ω��̎���
	double SKyori;
	double SKakudo;
	double STime;

};
struct C R_C[1000];

//VFDL�Ɋւ���\����
struct VFDL_Info              //VFDL�̏����i�[����\����
{
	char M[STR_MAX];
	char Motion[STR_MAX];	//����
	char Hand[STR_MAX];				//�g�p���Ă����i���肩�E�肩�j
	char Place1[STR_MAX];   //�ꏊ1
	char Place2[STR_MAX];   //�ꏊ2
	double BeforeX;//����O�̏��X
	double BeforeY;//����O�̏��Y
	double BeforeZ;//����O�̏��Z
	double AfterX;//�����̏��X
	double AfterY;//�����̏��Y
	double AfterZ;//�����̏��Z
	int OyaP;//�e���i
	int KoP;//�q���i
	int HandShape;			//��̌`
	char BeforeJig[STR_MAX];		//�x����
	char Sentan_Num[STR_MAX];		//��[��
	int Genten0X;			//���_��x���W
	int Genten0Y;			//���_��y���W
	int Genten0Z;			//���_��z���W
	int Color;				//�F�ԍ�
	char Kensa_Cont[STR_MAX];		//�������e
	int Kensa_Table;		//�����\
	int Pen;				//�y���ԍ�
	char Sup1[STR_MAX];				//�x��1
	char Sup1_2[STR_MAX];				//�x��1
	char Sup2[STR_MAX];				//�x��2
	char Sup3[STR_MAX];				//�x��3
	char Sup4[STR_MAX];				//�x��4
	int Assy1;				//�g���i1
	int Assy2;				//�g���i2
	int Assy12;				//�g���i12
	int Jig;				//���H��
	int Dansuu;				//�i��
	char updown[STR_MAX];			//��邩���邩
	char frontback[STR_MAX];			//�O���ォ
	int angle1;				//��]�O�̊p�x
	int angle2;				//��]��̊p�x
	int Zamen_height;		//���ʂ̍���
	int angle1_X;			//�u��]����v�̉�]�O�̏��1
	int angle2_X;			//�u��]����v�̉�]�O�̏��2
	int angle3_X;			//�u��]����v�̉�]�O�̏��3
	int angle1_Y;			//�u��]����v�̉�]��̏��1
	int angle2_Y;			//�u��]����v�̉�]��̏��2
	int angle3_Y;			//�u��]����v�̉�]��̏��3
	char sp_KoP[STR_MAX];			//������̑Ώە�
	int KoP1;				//�Ώە��̏��1
	int KoP2;				//�Ώە��̏��2
	int KoP3;				//�Ώە��̏��3
	int KoP4;
	int KoP5;
	int angle_num;			//�A���O���ԍ�
	double Kyori;				//�u�J�����A���O���v�ɂ����鋗�����
	char Syunzi_Keika[STR_MAX];		//�u���܂��͌o��(�J�����A���O��)
	int Tokaritsu;			//���ߗ�
	int marker_num;			//�}�[�J�[�ԍ�
	int entyu_num;			//�~���ԍ�
	char Hyouzi_Kesu[STR_MAX];		//�\���܂��͏���(�~��)
	double Dousa_Kyori;
	//���s�p
	char S_M_L[STR_MAX];//�����̑�E���E��
	char F_B_S[STR_MAX];//��������
	int Case;
	char Case2[STR_MAX];
	int Tukaten[10];
	int flag;
	double X_Dif;//X���W�̍�
	double Y_Dif;//Y���W�̍�
	double Z_Dif;//Z���W�̍�
	double OFFSET;//APP��OFFSET�̋���
};
struct VFDL_Info Step[FGETS_MAX_LENGTH];

//���i���E����Ɋւ���\����
struct Box_Jig
{
	int Box_hyouka;//���i���̕]��(0:���Â炳�v����"������",2:���Â炳�v����"�傫��",1:����ȊO)
	int Box_hand;//���i���ǂ���̎�ň�����(0:����,1:�E��)
	int Number;//���i���o�����鏇�Ԃ�����
	double Box_x;//���i����x�T�C�Y
	double Box_y;//���i����y�T�C�Y
	double Box_z;//���i����z�T�C�Y
	double Jig_x;//�����x�T�C�Y
	double Jig_y;//�����y�T�C�Y
	double Jig_z;//�����z�T�C�Y
	double Volume;//���i�̑̐�
	int num;//���i�ԍ�
};
struct Box_Jig BJ[STR_MAX];

struct KJUNJO {
	long long kumij;			//�g�������ԍ�
	int kuminum[MaxKN];	//�g�t���ԍ�
	int p1[MaxKN];	//���i1
	int p2[MaxKN];	//���i2
	int p3[MaxKN];	//�g���ĕ��i
	char Kumituke_Pos[MaxKN][3];//�g�t������
	char Oya_Pos1[MaxKN][3];//�e���i�̎p���iX�����j
	char Oya_Pos2[MaxKN][3];//�e���i�̎p���iZ�����j
	int Jig[MaxKN];
	int KouguNum[MaxKN];
	int DousaNum[MaxKN];

	int Dir[MaxKN];//����ɕt�������p�̕���(1:X,2:Y,3:Z)
};
struct KJUNJO R_KJUNJO;

//���C�A�E�g�Ɋւ���\����
struct Rayout
{
	double Box_x[STR_MAX];//���i����x�T�C�Y
	double Box_y[STR_MAX];//���i����y�T�C�Y
	double Box_z[STR_MAX];//���i����z�T�C�Y
	double Box_xCo[STR_MAX];//�����x���W
	double Box_yCo[STR_MAX];//�����y���W
	double Box_zCo[STR_MAX];//�����z���W
	double Jig_x[STR_MAX];//�����x�T�C�Y
	double Jig_y[STR_MAX];//�����y�T�C�Y
	double Jig_z[STR_MAX];//�����z�T�C�Y
	double Jig_xCo[STR_MAX];//�����x���W
	double Jig_yCo[STR_MAX];//�����y���W
	double Jig_zCo[STR_MAX];//�����z���W
	double Parts_xCo[STR_MAX];//���i��x���W
	double Parts_yCo[STR_MAX];//���i��y���W
	double Parts_zCo[STR_MAX];//���i��y���W
	double finish_centerX;//�����i���̒��S�_��X���W
	double finish_centerY;//�����i���̒��S�_��Y���W
	double finish_centerZ;//�����i���̒��S�_��Z���W
	double karioki_centerX;//���u����̒��S�_��X���W
	double karioki_centerY;//���u����̒��S�_��Y���W
	double karioki_centerZ;//���u����̒��S�_��Z���W
	int num[STR_MAX];
	int Box_inclination[STR_MAX];//���i���̌X��
};
struct Rayout Ray;
//���i�����Ɋւ���\����
struct Buhin
{
	int Bu_num;
	char Buhin_Name[100];
	char CAD_file[100];
	int Buhin_num;
	double Center_X;
	double Center_Y;
	double Center_Z;
	double Long_X;
	double Long_Y;
	double Long_Z;
	double Xmin;
	double Xmax;
	double Ymin;
	double Ymax;
	double Zmin;
	double Zmax;
	double Xkotei;
	double Zkotei;
	double Ykotei;
	int Color_num;
	int weight;
	int kogu_num;
	int kogu_name;
	int situkan;
	int tokutyo;
	int OK;
	int Box_type;//���i���̎��
	int Buhinsu;//���i���ɓ���镔�i�̐�
};
struct Buhin R_B[100];

struct Tyukan
{
	int Tyunum;//���Ԑ��i�ԍ�
	double lx;//����X
	double ly;//����Y
	double lz;//����Z
	double Xkotei;//����X�Œ�i���בւ��O�j
	double Ykotei;//����Y�Œ�i���בւ��O�j
	double Zkotei;//����Z�Œ�i���בւ��O�j
};
struct Tyukan R_T[STR_MAX];

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
	//	2�s�ځFVFDL�t�@�C���̓��o�`���i�P�F�ԗ��A�Q�F���i�Q�A�R�F�����j
	//	3�s�ځFVFDL�t�@�C���̍ő�ԍ�
	//  4�s�ځFINPUT����t�H���_�̃p�X(VFDL�p)
	//  5�s�ځFINPUT����t�H���_�̃p�X(Data�p)
	//  6�s�ځFOUTPUT����t�H���_�̃p�X(VFDL�p)
	//	7�s�ځFOUTPUT����t�H���_�̃p�X(���Ԓl�����]���p)

	fp1 = fopen(NowPath, "r");
	if (fp1 == NULL) ShowFinMsg("�p�����[�^�t�@�C��������܂���"); // �t�@�C�����J���Ȃ���ΏI��
	if (fscanf(fp1, "%s", SEIHIN_Name) == EOF) ShowFinMsg("���i�����s���ł�");
	if (fscanf(fp1, "%d", &D_model) == EOF) ShowFinMsg("���o�`������͂��Ă�������");
	if (fscanf(fp1, "%d", &s_File) == EOF) ShowFinMsg("VFDL�t�@�C���̍ő�ԍ�����͂��Ă�������");
	if (fscanf(fp1, "%s", inPath_Input1) == EOF) ShowFinMsg("���͐�t�H���_�̃p�X(VFDL)���s���ł�");
	if (fscanf(fp1, "%s", inPath_Input2) == EOF) ShowFinMsg("���͐�t�H���_�̃p�X(Data)���s���ł�");
	if (fscanf(fp1, "%s", outDir1) == EOF) ShowFinMsg("�o�͐�t�H���_�̃p�X�@���s���ł�");
	if (fscanf(fp1, "%s", outDir2) == EOF) ShowFinMsg("�o�͐�t�H���_�̃p�X�A���s���ł�");
	fclose(fp1);

	//"���i����"�ǂݍ��ݗp�p�X�쐬
	strcpy(inPath_Input_Buhin_Zokusei, inPath_Input2);
	strcat(inPath_Input_Buhin_Zokusei, "\\");
	strcat(inPath_Input_Buhin_Zokusei, SEIHIN_Name);
	strcat(inPath_Input_Buhin_Zokusei, "_���i����.csv");
	
	//"���Ԑ��i����"�ǂݍ��ݗp�p�X�쐬
	strcpy(inPath_Input_Tyukan, inPath_Input2);
	strcat(inPath_Input_Tyukan, "\\");
	strcat(inPath_Input_Tyukan, SEIHIN_Name);

	//VFDL�ǂݍ��ݗp�p�X�쐬
	strcpy(inPath_Input_VFDL, inPath_Input1);
	strcat(inPath_Input_VFDL, "\\");
	strcat(inPath_Input_VFDL, SEIHIN_Name);

	//�g�������ǂݍ��ݗp�p�X�쐬
	strcpy(inPath_Input_Junjo, inPath_Input2);
	strcat(inPath_Input_Junjo, "\\");
	strcat(inPath_Input_Junjo, SEIHIN_Name);

	//Output�p�p�X(VFDL�p)�쐬
	strcpy(OutPath, outDir1);
	strcat(OutPath, "\\");
	strcat(OutPath, SEIHIN_Name);


	if (D_model == 1)//�ԗ�
	{
		strcat(inPath_Input_Tyukan, "_�ԗ�_���Ԑ��i����.csv");
		strcat(inPath_Input_VFDL, "_�ԗ�_VFDL");
		strcat(inPath_Input_Junjo, "_�ԗ�_�g������");
		strcat(OutPath, "�ԗ�_���C�A�E�g�ύXVFDL");
	}
	else if (D_model == 2)//���i�Q
	{
		strcat(inPath_Input_Tyukan, "_���i�Q_���Ԑ��i����.csv");
		strcat(inPath_Input_VFDL, "_���i�Q_VFDL");
		strcat(inPath_Input_Junjo, "_���i�Q_�g������");
		strcat(OutPath, "���i�Q_���C�A�E�g�ύXVFDL");
	}
	else//����
	{
		strcat(inPath_Input_Tyukan, "_����_���Ԑ��i����.csv");
		strcat(inPath_Input_VFDL, "_����_VFDL");
		strcat(inPath_Input_Junjo, "_����_�g������");
		strcat(OutPath, "����_���C�A�E�g�ύXVFDL");
	}

	//Output�p�p�X(�����]���t�@�C���p)�쐬
	strcpy(outPath_TimeHyouka, outDir2);
	strcat(outPath_TimeHyouka, "\\");
	strcat(outPath_TimeHyouka, SEIHIN_Name);
	strcat(outPath_TimeHyouka, "_���Ԓl�����]��.csv");

	printf("���i���F%s\n", SEIHIN_Name);
	printf("VFDL�t�@�C���̍ő�ԍ��F%d\n", s_File);
	printf("VFDL�t�@�C���̓��o�`���i1�F�ԗ�,2�F���i�Q,3�F�����j�F%d\n", D_model);
	printf("Input(VFDL)���F%s\n", inPath_Input1);
	printf("Input(Data)���F%s\n", inPath_Input2);
	printf("Input(���i����)���F%s\n", inPath_Input_Buhin_Zokusei);
	printf("Output(VFDL)���F%s\n\n", OutPath);
	printf("Output(�����]��)���F%s\n\n", OutPath);

	//���i�����̓ǂݍ���
	Read_Buhin_Zokusei();
	printf("���i�����ǂݍ��݊���\n");

	//���Ԑ��i�����̓ǂݍ���
	Read_Tyukan();
	printf("���Ԑ��i�����ǂݍ��݊���\n");

	//����t�g�������̓ǂݍ���
	//Read_Junjo();
	//printf("����t�g�������ǂݍ��݊���\n");

	//VFDL�̓ǂݍ���
	printf("�]�����쐬���ł�\n");
	Read_VFDL();
	printf("\n�]���̍쐬���������܂����B\n");

	//�I���̏���
	printf("(Enter�L�[�������Ă�������)\n");
	unsigned char key;
	while (1) {
		key = getch();
		if (key == 0x0d) break;
	}
}

void Read_VFDL()           //VFDL��ǂݍ��ފ֐�
{
	FILE* fp1;
	FILE* fp2;

	char* temp, * s;
	char* trash;

	int i = 0;
	int k = 0;
	int l = 0;
	int m = 0;
	int d = 0;
	int n[STR_MAX] = { 0 };
	int count = 0;
	int reset = 0;
	char InPath[STR_MAX];

	temp = (char*)malloc(20000);
	trash = (char*)malloc(20000);

	fp2 = fopen(outPath_TimeHyouka, "w");

	for (int vfdlnum = 1; vfdlnum <= s_File; vfdlnum++)//�p�����[�^�Ŏw�肳�ꂽVFDL�̐�������
	{
		//������
		STEPNUM = 0;
		STime_Bring = 0.0;
		STime_Reach = 0.0;
		STime_Release = 0.0;
		STime_Leave = 0.0;
		STime_Attach = 0.0;
		STime_Remove = 0.0;
		STime_Turn = 0.0;
		STime_Grab = 0.0;
		L_Kyori = 0.0;
		R_Kyori = 0.0;
		All_Angle = 0.0;

		Reach_count = 0;
		Bring_count = 0;
		Turn_count = 0;
		Grab_count = 0;
		Release_count = 0;
		Leave_count = 0;
		Attach_count = 0;
		Remove_count = 0;

		sprintf(InPath, "%s%d.csv", inPath_Input_VFDL, vfdlnum);


		//���͐�̃t���p�X��InPath�ɑ������
		/*strcpy(InPath, inPath_Input_VFDL);
		strcat(InPath, Conv_Char(i));
		strcat(InPath, "_���l���");
		strcat(InPath, ".csv");*/

		fp1 = fopen(InPath, "r");						//NowPath�̃t�@�C�����J���āA�ǂݍ���
		if (fp1 == NULL)ShowFinMsg("VFDL������܂���");		//�t�@�C�����J���Ȃ���ΏI��

		printf("���݊J���Ă���VFDL %d\r", vfdlnum);

		//�ȍ~�́A�J�����t�@�C����VFDL�ł���O��ŏ���������

		//1�s�ڂ̓ǂݍ���
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
		s = temp;

		sscanf(s, "%d%d%d", &R_H.W, &R_H.L, &R_H.P);

		//2�s�ڂ̓ǂݍ���
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
		s = temp;

		sscanf(s, "%d%d%d%d%d", &R_H.H, &R_H.Ht, &R_H.V, &R_H.C, &R_H.S);	//�X�e�b�v��,�L���X�e�b�v��,���x�w��,���`�F�b�N,��Ԕԍ�

		//3�s�ڂ̓ǂݍ���
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
		s = temp;

		sscanf(s, "%d%d%d", &R_H.PA, &R_H.T, &R_H.J);

		//4�s�ڂ̓ǂݍ���
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
		s = temp;

		sscanf(s, "%d", &R_H.MI);

		//5�s�ڂ̓ǂݍ���
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
		s = temp;

		sscanf(s, "%d%d%d", &R_H.MO, &R_H.MO1, &R_H.MO2);

		//6�s�ڂ̓ǂݍ���
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
		s = temp;

		sscanf(s, "%d%d%d", &R_H.TA, &R_H.TR, &R_H.WD);

		//7�s�ڂ̓ǂݍ���
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		G = 2 + R_H.PA + R_H.T + R_H.J + R_H.MI + R_H.TA + R_H.TR + R_H.WD + R_H.RA;

		//8�s�ڂ���F����1�s�O�܂�
		for (i = 1; i <= G; i++)
		{

			fgets(temp, FGETS_MAX_LENGTH, fp1);

			Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
			s = temp;

			sscanf(s, "%d", &R_S0[i].N);
			s = strchr(&s[1], ' ');		//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���

			//�ȈՎ���̏��
			if (R_S0[i].N >= -299 && R_S0[i].N < -100)
			{
				sscanf(s, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%d", &R_S0[i].lx, &R_S0[i].ly, &R_S0[i].lz, &R_S0[i].Hx, &R_S0[i].Hy, &R_S0[i].Hz, &R_S0[i].Bx, &R_S0[i].By, &R_S0[i].Bz, &R_S0[i].C, &R_S0[i].S);
			}
			//���̏��
			else if (R_S0[i].N >= 8000 && R_S0[i].N < 9000)
			{
				sscanf(s, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%d", &R_S0[i].lx, &R_S0[i].ly, &R_S0[i].lz, &R_S0[i].Hx, &R_S0[i].Hy, &R_S0[i].Hz, &R_S0[i].Bx, &R_S0[i].By, &R_S0[i].Bz, &R_S0[i].C);
			}
			//�g���C�̏��
			else if (R_S0[i].N >= 9000 && R_S0[i].N < 10000)
			{
				sscanf(s, "%lf%lf%lf%lf%lf%lf%d%lf%lf%lf%d%d%d", &R_S0[i].lx, &R_S0[i].ly, &R_S0[i].lz, &R_S0[i].Hx, &R_S0[i].Hy, &R_S0[i].Hz, &R_S0[i].S, &R_S0[i].Bx, &R_S0[i].By, &R_S0[i].Bz, &R_S0[i].C, &R_S0[i].X, &R_S0[i].Y);
			}

			else
			{
				sscanf(s, "%lf%lf%lf%d%d%d", &R_S0[i].lx, &R_S0[i].ly, &R_S0[i].lz, &R_S0[i].C, &R_S0[i].X, &R_S0[i].Y);
			}
		}
		STEPNUM++;

		//F����1�s�O�̓ǂݍ���

		fgets(temp, FGETS_MAX_LENGTH, fp1);

		while (fgets(temp, FGETS_MAX_LENGTH, fp1) != NULL)	//�c��̑S�Ă̍s��ǂݍ���
		{
			Del_Com(temp);
			s = temp;
			sscanf(s, "%s", &Step[STEPNUM].M);		//��������ǂݍ���

			s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���


			if (strcmp(Step[STEPNUM].M, "F") == 0)	//������̐擪��F�̂Ƃ�
			{

				sscanf(s, "%d%d%d%d%lf%d", &R_C[STEPNUM].SEQ, &R_C[STEPNUM].NF, &R_C[STEPNUM].HL, &R_C[STEPNUM].Flag, &R_C[STEPNUM].T, &R_C[STEPNUM].AC);	//�ω���1�s�ڂ�ǂݍ���

				fgets(temp, FGETS_MAX_LENGTH, fp1);

				Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
				s = temp;

				sscanf(s, "%lf%lf%d%lf", &R_C[STEPNUM].SKyori, &R_C[STEPNUM].SKakudo, &R_C[STEPNUM].TurnNum, &R_C[STEPNUM].STime);//�ω���2�s�ڂ�ǂݍ���

				fgets(temp, FGETS_MAX_LENGTH, fp1);

				Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
				s = temp;

				sscanf(s, "%s", &Step[STEPNUM].Motion);	//�����ǂݍ���

				if (strcmp(Step[STEPNUM].Motion, "�L�΂�") == 0)	//���삪'�L�΂�'�̂Ƃ�
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//A�Ɋւ������ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//B�Ɋւ������ǂݍ���

				}
				else if (strcmp(Step[STEPNUM].Motion, "�^��") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//A�Ɋւ������ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//B�Ɋւ������ǂݍ���


				}
				else if (strcmp(Step[STEPNUM].Motion, "�g���ړ�����") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%s%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//A�Ɋւ������ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%s%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//B�Ɋւ������ǂݍ���

				}
				else if (strcmp(Step[STEPNUM].Motion, "��������") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//�Ώە���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//X�Ɋւ������ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//Y�Ɋւ������ǂݍ���

					//printf("%d%d%d%d%d%d\n", Step[STEPNUM].RTurn0X, Step[STEPNUM].RTurn0Y, Step[STEPNUM].RTurn0Z, Step[STEPNUM].RTurn1X, Step[STEPNUM].RTurn1Y, Step[STEPNUM].RTurn1Z);


				}
				else if (strcmp(Step[STEPNUM].Motion, "�͂�") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//�Ώە���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].HandShape);		//��̌`��ǂݍ���

					R_C[STEPNUM].T = 2.0 * 0.036;			//�����̕��i��͂�=�P�[�X1A

					STime_Grab += R_C[STEPNUM].T;//���v���ԗp


				}
				else if (strcmp(Step[STEPNUM].Motion, "�͂ݎ��") == 0)
				{

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//�Ώە���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].BeforeJig);		//�x����̔ԍ���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);		    //'����'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].HandShape);		//��̌`��ǂݍ���


				}
				else if (strcmp(Step[STEPNUM].Motion, "����") == 0)
				{

					R_C[STEPNUM].T = 0.072;

					STime_Release += R_C[STEPNUM].T;//���v���ԗp

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//�Ώە���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].HandShape);		//��̌`��ǂݍ���


				}
				else if (strcmp(Step[STEPNUM].Motion, "�u������") == 0)
				{

					R_C[STEPNUM].T = 0.2736;

					STime_Leave += R_C[STEPNUM].T;//���v���ԗp

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//�Ώە���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].BeforeJig);		//�x����̔ԍ���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].HandShape);		//��̌`��ǂݍ���


				}
				else if (strcmp(Step[STEPNUM].Motion, "�t����") == 0)
				{
					//�ԗ��p
					R_C[STEPNUM].T = 0.2016;

					STime_Attach += R_C[STEPNUM].T;//���v���ԗp

					//printf("�t����%d�̎��Ԓl����͂��Ă�������:", j);			//���̓���ԍ��́u�t����v�̎��Ԓl���͂�v������

					//sscanf("%lf", &R_C[j].T);

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//�Ώە���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].BeforeJig);		//�x����̔ԍ���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

					Jignum++;//������J�E���g

				}
				else if (strcmp(Step[STEPNUM].Motion, "�O��") == 0)
				{
					//printf("�O��%d�̎��Ԓl����͂��Ă�������:", j);			//���̓���ԍ��́u�O���v�̎��Ԓl���͂�v������
					//sscanf("%lf", &R_C[j].T);
					//�ԗ��p
					R_C[STEPNUM].T = 0.144;

					STime_Remove += R_C[STEPNUM].T;//���v���ԗp

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//�Ώە���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].BeforeJig);		//�x����̔ԍ���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);		    //'����'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

				}
				else if (strcmp(Step[STEPNUM].Motion, "��[��ύX����") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);		    //'�̈ʒu��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Sentan_Num);		//��[�Ԃ�ǂݍ���

				}
				else if (strcmp(Step[STEPNUM].Motion, "�g���Ă�") == 0)
				{
					Step[STEPNUM].flag = 0;
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Sup1);		//�x��1��ǂݍ���

					if (strcmp(Step[STEPNUM].Sup1, "����") == 0 || strcmp(Step[STEPNUM].Sup1, "�E��") == 0 || strcmp(Step[STEPNUM].Sup1, "�E��Q����") == 0)//�H��p
					{
						Step[STEPNUM].flag = 1;
						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%s", &Step[STEPNUM].Sup1_2);		//�x��1��ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%d", &Step[STEPNUM].Assy1);		//�g���i1��ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%s", &Step[STEPNUM].Sup2);		//�x��2��ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%d", &Step[STEPNUM].Assy2);		//�g���i2��ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%d", &Step[STEPNUM].Jig);		//�g�p���Ă�����̏���ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%d", &Step[STEPNUM].angle_num);		//�g�p���Ă���H��̏���ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%s", &Step[STEPNUM].Sup3);		//�x��3��ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%d", &Step[STEPNUM].Case);		//�g�p���Ă�����̏���ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%s", &Step[STEPNUM].Sup4);		//�x��4��ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%d", &Step[STEPNUM].Assy12);		//�g���i12��ǂݍ���
					}
					else
					{
						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%d", &Step[STEPNUM].Assy1);		//�g���i1��ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%s", &Step[STEPNUM].Sup2);		//�x��2��ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%d", &Step[STEPNUM].Assy2);		//�g���i2��ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

						if (strcmp(Step[STEPNUM].Hand, "�E��Q����") != 0)    //��̏�񂪉E��Q����ł͂Ȃ��Ƃ�
						{
							Step[STEPNUM].Jig = atoi(Step[STEPNUM].Hand);

							s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
							sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���
						}

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%s", &Step[STEPNUM].Sup3);		//�x��3��ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%s", &Step[STEPNUM].Sup4);		//�x��4��ǂݍ���

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%d", &Step[STEPNUM].Assy12);		//�g���i12��ǂݍ���
					}

				}
				else if (strcmp(Step[STEPNUM].Motion, "���_��ύX����") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//�Ώە���ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%s%d%d%d", trash, &Step[STEPNUM].Genten0X, &Step[STEPNUM].Genten0Y, &Step[STEPNUM].Genten0Z);//���_�̍��W����ǂݍ���

				}
				else if (strcmp(Step[STEPNUM].Motion, "��������") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//�Ώە���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].Color);		//�F�ԍ���ǂݍ���

				}
				else if (strcmp(Step[STEPNUM].Motion, "�L������") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Kensa_Cont);		//�������e��ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].Kensa_Table);		//�����\��ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].Pen);		//�y���ԍ���ǂݍ���	

				}
				else if (strcmp(Step[STEPNUM].Motion, "����") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Place1);		    //�ꏊ1��ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);				//'����'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Place2);		    //�ꏊ2��ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);				//'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].angle1);		    //�p�x1��ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);				//'����'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].angle2);		    //�p�x2��ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);				//'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);				//'����'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].S_M_L);		    //�����̑傫����ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);				//'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].F_B_S);		    //���s�̌�����ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);				//'������'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);				//���ꂪ�Ȃ��Ə�肭�����Ȃ�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].Case);

					//�ʉߓ_�����邩�ǂ����̏ꍇ����
					if (Step[STEPNUM].Case != '\0')
					{
						Step[STEPNUM].Tukaten[n[STEPNUM]] = Step[STEPNUM].Case;

						s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
						sscanf(s, "%s", &Step[STEPNUM].Case2);

						//�ʉߓ_��2�ȏ゠�邩�ǂ����̏ꍇ����
						while (strcmp(Step[STEPNUM].Case2, "��ʂ���") != 0)
						{
							n[STEPNUM]++;
							Step[STEPNUM].Tukaten[n[STEPNUM]] = atoi(Step[STEPNUM].Case2);

							s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
							sscanf(s, "%s", &Step[STEPNUM].Case2);

						}

					}
					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%s%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//A�n�_�Ɋւ������ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%s%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//B�n�_�Ɋւ������ǂݍ���

				}
				else if (strcmp(Step[STEPNUM].Motion, "�K�i") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].Dansuu);		//�K�i�̒i����ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'�i'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].updown);		//���̂�������̂���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);		    //'����'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'A'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);		    //'���s'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'B'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].frontback);		//�O����납��ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//A�Ɋւ������ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//B�Ɋւ������ǂݍ���


				}
				else if (strcmp(Step[STEPNUM].Motion, "���") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].angle1);		//���̊p�x��ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);		    //'����'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].angle2);		//��]��̊p�x��ǂݍ���


				}
				else if (strcmp(Step[STEPNUM].Motion, "����") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].Zamen_height);		//���ʂ̍�����ǂݍ���

				}
				else if (strcmp(Step[STEPNUM].Motion, "����") == 0)
				{

				}
				else if (strcmp(Step[STEPNUM].Motion, "��]����") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//�Ώە���ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%s%d%d%d", trash, &Step[STEPNUM].angle1_X, &Step[STEPNUM].angle2_X, &Step[STEPNUM].angle3_X);//X�̍��W����ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%s%d%d%d", trash, &Step[STEPNUM].angle1_Y, &Step[STEPNUM].angle2_Y, &Step[STEPNUM].angle3_Y);//Y�̍��W����ǂݍ���

				}
				else if (strcmp(Step[STEPNUM].Motion, "���ɖ߂�") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].sp_KoP);		//�Ώە���ǂݍ���

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%s%d%d%d", trash, &Step[STEPNUM].KoP1, &Step[STEPNUM].KoP2, &Step[STEPNUM].KoP3);//�Ώە��̏���ǂݍ���


				}
				else if (strcmp(Step[STEPNUM].Motion, "����") == 0)
				{
					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
					s = temp;

					sscanf(s, "%s%d%d%d%d%d", trash, &Step[STEPNUM].KoP1, &Step[STEPNUM].KoP2, &Step[STEPNUM].KoP3, &Step[STEPNUM].KoP4, &Step[STEPNUM].KoP5);//�Ώە��̏���ǂݍ���


				}
				else if (strcmp(Step[STEPNUM].Motion, "��ױݸ��") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].angle_num);		//�A���O���ԍ���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);		    //'����'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%lf", &Step[STEPNUM].Kyori);		//������ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Syunzi_Keika);		//'�u��''�o��'��ǂݍ���


				}
				else if (strcmp(Step[STEPNUM].Motion, "���߂���") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

					if ((strcmp(Step[STEPNUM].Hand, "����") != 0) || ((strcmp(Step[STEPNUM].Hand, "�E��") != 0)) || ((strcmp(Step[STEPNUM].Hand, "����Q�E��") != 0)))
					{
						Step[STEPNUM].Jig = atoi(Step[STEPNUM].Hand);
					}

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", trash);		    //'���ߗ�'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].Tokaritsu);		//���ߗ���ǂݍ���


				}
				else if (strcmp(Step[STEPNUM].Motion, "ϰ��") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].marker_num);		//�}�[�J�[�ԍ���ǂݍ���

				}
				else if (strcmp(Step[STEPNUM].Motion, "�~��") == 0)
				{
					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%d", &Step[STEPNUM].entyu_num);		//�~���ԍ���ǂݍ���

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

					s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
					sscanf(s, "%s", &Step[STEPNUM].Hyouzi_Kesu);		//'�\������'�܂���'����'��ǂݍ���


				}

				STEPNUM++;

			}
			else if (strcmp(Step[STEPNUM].M, "����") == 0)
			{
				Step[STEPNUM].Motion[0] = '\0';

				R_C[STEPNUM].T = 0.072;

				sscanf(s, "%d", &Step[STEPNUM].KoP);		//�Ώە���ǂݍ���

				s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
				sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

				s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
				sscanf(s, "%s", &Step[STEPNUM].Hand);		//�g�p���Ă����̏���ǂݍ���

				s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
				sscanf(s, "%c", trash);		    //'��'��ǂݍ���Ŏ̂Ă�

				s = strchr(&s[1], ' ');				//�󔒂̕�����T���āA�����𕶎���̊J�n�_�ɂ���
				sscanf(s, "%d", &Step[STEPNUM].HandShape);		//��̌`��ǂݍ���

				STEPNUM++;

			}

		}

		Time_Turn(STEPNUM);

		Time_grab(STEPNUM);

		Time_Assemble(STEPNUM);
		
		Read_Junjo(vfdlnum);
		
		Kumituke_Position(STEPNUM);
		
		S0_Change(STEPNUM);
		
		C_Change(STEPNUM);
		
		Kyori_Change(STEPNUM);

		Time_Reach(STEPNUM);

		Time_Bring(STEPNUM);

		Write_VFDL(STEPNUM, n, vfdlnum);

		Write_TimeHyouka(STEPNUM, fp2);

		//F��ǂݍ��ޕϐ��̏������@����������Ȃ��Ɩԗ��ŉ��Ȃ�
		for (int reset = 1; reset <= STEPNUM; reset++)
		{
			Step[reset].M[0] = '\0';
		}

		fclose(fp1);

		STEPNUM = 0;
		Jignum = 0;
	}

	fclose(fp2);

	free(temp);
	free(trash);
}

void Read_Buhin_Zokusei()
{
	FILE* fp1;
	char* temp, * s;     //�e�L�X�g�̓��e��ǂݍ���
	char* trash;
	int i = 0;
	int j = 0;

	temp = (char*)malloc(20000);
	trash = (char*)malloc(20000);


	fp1 = fopen(inPath_Input_Buhin_Zokusei, "r");						//NowPath�̃t�@�C�����J���āA�ǂݍ���
	if (fp1 == NULL)ShowFinMsg("���i����������܂���");		//�t�@�C�����J���Ȃ���ΏI��

	//�ȍ~�́A�J�����t�@�C�������i�����ł���O��ŏ���������

	//1�s�ڂ̓ǂݍ���
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	//2�s�ڂ̓ǂݍ���
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	sscanf(temp, "%d", &N);

	for (i = 1; i <= N; i++)
	{
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
		s = temp;

		sscanf(s, "%d%s%s%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%d%d%d%d%d%d%d%d", &R_B[i].Bu_num, &R_B[i].Buhin_Name, &R_B[i].CAD_file, &R_B[i].Buhin_num, &R_B[i].Center_X, &R_B[i].Center_Y, &R_B[i].Center_Z, &R_B[i].Long_X, &R_B[i].Long_Y, &R_B[i].Long_Z, &R_B[i].Xmin, &R_B[i].Xmax, &R_B[i].Ymin, &R_B[i].Ymax, &R_B[i].Zmin, &R_B[i].Zmax, &R_B[i].Color_num, &R_B[i].weight, &R_B[i].kogu_num, &R_B[i].kogu_name, &R_B[i].situkan, &R_B[i].tokutyo, &R_B[i].OK, &R_B[i].Box_type, &R_B[i].Buhinsu);
		R_B[i].Xkotei = R_B[i].Long_X;//���בւ��O�Ɋi�[
		R_B[i].Ykotei = R_B[i].Long_Y;//���בւ��O�Ɋi�[
		R_B[i].Zkotei = R_B[i].Long_Z;//���בւ��O�Ɋi�[
		if (R_B[i].Box_type == 1)
		{
			printf("%d�͐���̕��i���ł�(���F%d)\n", i, R_B[i].Buhinsu);
		}
		else
		{
			printf("%d�͏W���̕��i���ł�(���F%d)\n", i, R_B[i].Buhinsu);
		}
	}

	BoxJig_Hyouka();

	free(temp);
	free(trash);

	fclose(fp1);
}

void Read_Tyukan()
{
	FILE* fp1;
	char* temp, * s;     //�e�L�X�g�̓��e��ǂݍ���
	int trashi;
	char* trash;
	double temps;

	temp = (char*)malloc(20000);
	trash = (char*)malloc(20000);
	printf("%s\n", inPath_Input_Tyukan);
	fp1 = fopen(inPath_Input_Tyukan, "r");						//NowPath�̃t�@�C�����J���āA�ǂݍ���
	if (fp1 == NULL)ShowFinMsg("���Ԑ��i����������܂���");		//�t�@�C�����J���Ȃ���ΏI��

	//1�s�ڂ̓ǂݍ���
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
	s = temp;

	sscanf(s, "%s%d", trash, &T);

	//2�s�ڂ̓ǂݍ���
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	for (int i = 1; i <= T; i++)
	{
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csv�t�@�C���̓f�[�^���ƂɃJ���}�������`�ŕۑ�����Ă���̂ŁA�J���}������
		s = temp;

		sscanf(s, "%d%d%lf%lf%lf", &R_T[i].Tyunum, &trashi, &R_T[i].lx, &R_T[i].ly, &R_T[i].lz);

		//���בւ��O�Ɋi�[
		R_T[i].Xkotei = R_T[i].lx;
		R_T[i].Ykotei = R_T[i].ly;
		R_T[i].Zkotei = R_T[i].lz;
	}

	//���ёւ�
	for (int i = 1; i <= T; i++)
	{
		if (R_T[i].lx < R_T[i].ly)
		{
			temps = R_T[i].lx;
			R_T[i].lx = R_T[i].ly;
			R_T[i].ly = temps;
		}

		if (R_T[i].lx < R_T[i].lz)
		{
			temps = R_T[i].lx;
			R_T[i].lx = R_T[i].lz;
			R_T[i].lz = temps;
		}

		if (R_T[i].ly < R_T[i].lz)
		{
			temps = R_T[i].ly;
			R_T[i].ly = R_T[i].lz;
			R_T[i].lz = temps;
		}
	}

	free(temp);
	free(trash);

	fclose(fp1);
}

void Read_Junjo(int J)
{
	FILE* fp1;

	char* temp;
	
	int First_Time = 0;//�n�߂͔�΂�
	int File_num;//�t�@�C����
	int Knumber;
	int flg;
	int flag = 0;
	int j = 0;
	int count = 0;

	char InPath[STR_MAX];

	//�t�@�C�����̌v�Z
	File_num = (N * s_File) / MaxExcel;

	temp = (char*)malloc(20000);

	for (int file = 0; file <= File_num; file++)
	{

		sprintf(InPath, "%s%d_J.csv", inPath_Input_Junjo, file);

		fp1 = fopen(InPath, "r");						//NowPath�̃t�@�C�����J���āA�ǂݍ���
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
				}
				else //�ŏ���K�̂Ƃ�
				{
					First_Time++;
				}

				count++;

				if (J == count)//�Y������g������
				{
					sscanf(temp, "%c%lld%d", &K, &R_KJUNJO.kumij, &flg);//K,�g�������ԍ�,���݃t���O�̓ǂݍ���
				}

				j = 1;
				
			}
			else
			{

				if (J == count)//�Y������g������
				{
					sscanf(temp, "%d%d%d%d%s%s%s%d%d%d", &R_KJUNJO.kuminum[j], &R_KJUNJO.p1[j], &R_KJUNJO.p2[j], &R_KJUNJO.p3[j], R_KJUNJO.Kumituke_Pos[j], R_KJUNJO.Oya_Pos1[j], R_KJUNJO.Oya_Pos2[j], &R_KJUNJO.Jig[j], &R_KJUNJO.KouguNum[j], &R_KJUNJO.DousaNum[j]);

					//�e���i�̕������i�[���鏈��
					if ((strcmp(R_KJUNJO.Oya_Pos1[j], "��") == 0 || strcmp(R_KJUNJO.Oya_Pos1[j], "��") == 0))//�e���i�̎p���iX�������j����܂��͉�
					{
						R_KJUNJO.Dir[j] = 1;//X��������
					}
					else if ((strcmp(R_KJUNJO.Oya_Pos2[j], "��") == 0 || strcmp(R_KJUNJO.Oya_Pos2[j], "��") == 0))//�e���i�̎p���iZ�������j����܂��͉�
					{
						R_KJUNJO.Dir[j] = 3;//Z��������
					}
					else
					{
						R_KJUNJO.Dir[j] = 2;//Y��������
					}

					if (j == N - 1)//�ŏI�s�̎�
					{
						break;
					}
				}

				j++;
			}
		}

		fclose(fp1);
	}

	free(temp);

}

//�g�t���ʒu�ύX�p�֐�
void Kumituke_Position(int STEP)
{
	double temp = 0.0;
	int tempJig;

	Beforejignum = 2 + R_H.PA + R_H.T;

	for (int i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "�g���ړ�����") == 0)//"�g���ړ�����"�̎�
		{
			for (int Jig = 1; Jig <= R_H.J; Jig++)
			{
				tempJig = atoi(Step[i + 1].Sup4);
				if (Step[i + 1].flag == 1)//�H��g�t���̂Ƃ�
				{
					for (int j = i; j >= 1; j--)
					{
						if (strcmp(Step[j].Motion, "�^��") == 0 && strcmp(Step[j].Hand, "����") == 0)//����ŉ^�ԍ�Ƃ�T��
						{
							temp = Step[j].AfterZ;//�i�[
							break;
						}
					}
					if (R_S0[Beforejignum + Jig].N == Step[i + 1].Case)//"�g���Ă�"�����T���ɂ���
					{
						Step[Step[i + 1].Assy2].X_Dif = R_S0[Beforejignum + Jig].lx - Step[i].BeforeX;//����̒��S�_����X�̍������߂�
						Step[Step[i + 1].Assy2].Y_Dif = R_S0[Beforejignum + Jig].ly - Step[i].BeforeY;//����̒��S�_����Y�̍������߂�
						Step[Step[i + 1].Assy2].Z_Dif = R_S0[Beforejignum + Jig].lz - Step[i].BeforeZ;//����̒��S�_����Z�̍������߂�
						break;
					}
				}
				else
				{
					for (int j = i; j >= 1; j--)
					{
						if (strcmp(Step[j].Motion, "�^��") == 0 && strcmp(Step[j].Hand, "����") == 0)//����ŉ^�ԍ�Ƃ�T��
						{
							temp = Step[j].AfterZ;//�i�[
							break;
						}
					}
					if (R_S0[Beforejignum + Jig].N == tempJig)//"�g���Ă�"�����T���ɂ���
					{
						Step[Step[i + 1].Assy2].X_Dif = R_S0[Beforejignum + Jig].lx - Step[i].BeforeX;//����̒��S�_����X�̍������߂�
						Step[Step[i + 1].Assy2].Y_Dif = R_S0[Beforejignum + Jig].ly - Step[i].BeforeY;//����̒��S�_����Y�̍������߂�
						Step[Step[i + 1].Assy2].Z_Dif = temp - Step[i].BeforeZ;//����̒��S�_����Z�̍������߂�
						break;
					}
				}
				
			}
			Step[i].OFFSET = Step[i].BeforeZ - Step[i].AfterZ;

		}
	}
}

//���i���E����]���֐�
void BoxJig_Hyouka()	//���i���E����]���p�֐�
{
	int temp;
	double temp2;
	int Box[3];//���i���i�W���j�̑̐�
	double Box_capa[3];

	Box[0] = 95 * 100 * 50;//���i���i�W���j���̑̐ς����߂�
	Box[1] = 96 * 160 * 72;//���i���i�W���j���̑̐ς����߂�
	Box[2] = 145 * 200 * 90;//���i���i�W���j��̑̐ς����߂�

	for (int i = 1; i <= N; i++)
	{
		BJ[i].Volume = R_B[i].Long_X * R_B[i].Long_Y * R_B[i].Long_Z;//���i�̑̐ς����߂�

		BJ[i].num = i;

		//**����̃T�C�Y����**//
		BJ[i].Jig_x = R_B[i].Long_X + 20; //�����x�T�C�Y�̌���
		BJ[i].Jig_y = R_B[i].Long_Y + 20; //�����y�T�C�Y�̌���
		BJ[i].Jig_z = 20; //�����z�T�C�Y�̌���i�Œ�l�j

		swap(i);//�傫�����ɕ��ёւ�

		//**���i���̃T�C�Y�����߂鏈��**//
		if (R_B[i].Box_type == 1)//���񂳂��镔�i���̎�
		{
			if (R_B[i].Buhinsu % 2 == 0)//�z�肷�镔�i���������̎�
			{
				for (int k = 1; k <= 10; k++)
				{
					if ((R_B[i].Buhinsu > k * k + k) && (R_B[i].Buhinsu <= ((k + 1) * (k + 1))))
					{
						BJ[i].Box_x = (R_B[i].Xkotei + 20) * k; //���i����x�T�C�Y�̌���=�����`�T�C�Y
						BJ[i].Box_y = (R_B[i].Ykotei + 25) * k; //���i����y�T�C�Y�̌���=�����`�T�C�Y
						BJ[i].Box_z = 50; //���i����z�T�C�Y�̌���
						break;
					}
					else if (R_B[i].Buhinsu > k * k && R_B[i].Buhinsu <= k * k + k)
					{
						BJ[i].Box_x = (R_B[i].Xkotei + 20) * k; //���i����x�T�C�Y�̌���
						BJ[i].Box_y = (R_B[i].Ykotei + 25) * (k + 1); //���i����y�T�C�Y�̌���
						BJ[i].Box_z = 50; //���i����z�T�C�Y�̌���
						break;
					}
				}
			}
			else//�z�肷�镔�i������̎�
			{
				for (int k = 1; k <= 10; k++)
				{
					if ((R_B[i].Buhinsu > k * k + k) && (R_B[i].Buhinsu <= ((k + 1) * (k + 1))))
					{
						BJ[i].Box_x = (R_B[i].Xkotei + 20) * k; //���i����x�T�C�Y�̌���=�����`�T�C�Y
						BJ[i].Box_y = (R_B[i].Ykotei + 25) * k; //���i����y�T�C�Y�̌���=�����`�T�C�Y
						BJ[i].Box_z = 50; //���i����z�T�C�Y�̌���
						break;
					}
					else if (R_B[i].Buhinsu > k * k && R_B[i].Buhinsu <= k * k + k)
					{
						BJ[i].Box_x = (R_B[i].Xkotei + 20) * k; //���i����x�T�C�Y�̌���
						BJ[i].Box_y = (R_B[i].Ykotei + 25) * (k + 1); //���i����y�T�C�Y�̌���
						BJ[i].Box_z = 50; //���i����z�T�C�Y�̌���
						break;
					}
					else if (R_B[i].Buhinsu == 1)
					{
						BJ[i].Box_x = (R_B[i].Xkotei + 30) * 1; //���i����x�T�C�Y�̌���=�����`�T�C�Y
						BJ[i].Box_y = (R_B[i].Ykotei + 30) * 1; //���i����y�T�C�Y�̌���=�����`�T�C�Y
						BJ[i].Box_z = 50; //���i����z�T�C�Y�̌���
						break;
					}
				}
			}
		}
		else//�W���̕��i���̎�
		{
			for (int B = 0; B <= 2; B++)
			{
				Box_capa[B] = Box[B] / BJ[i].Volume;//���ꂼ��̕��i���ɂ����鋖�e�����Z�o
			}
			for (int B2 = 0; B2 <= 2; B2++)
			{
				if (Box_capa[B2] > R_B[i].Buhinsu)//���e��>�z����̎�
				{
					if (B2 == 0)//�ł����������i���̎�
					{
						BJ[i].Box_x = 95; //���i����x�T�C�Y�̌���
						BJ[i].Box_y = 100; //���i����y�T�C�Y�̌���
						BJ[i].Box_z = 50; //���i����z�T�C�Y�̌���
					}
					else if (B2 == 1)//2�Ԗڂɏ��������i���̎�
					{
						BJ[i].Box_x = 96; //���i����x�T�C�Y�̌���
						BJ[i].Box_y = 160; //���i����y�T�C�Y�̌���
						BJ[i].Box_z = 72; //���i����z�T�C�Y�̌���
					}
					else//�ł��傫�����i���̎�
					{
						BJ[i].Box_x = 145; //���i����x�T�C�Y�̌���
						BJ[i].Box_y = 200; //���i����y�T�C�Y�̌���
						BJ[i].Box_z = 90; //���i����z�T�C�Y�̌���
					}
					break;
				}
				else//�ǂ̕��i���ɂ��Y�����Ȃ��Ƃ�
				{
					printf("���i%d�̕��i�����G���[�ł�\n", i);
				}
			}

		}

		//**���i���P�i�ڂ��Q�i�ڂ������߂鏈��**//
		if (R_B[i].Long_X <= 21.2 && R_B[i].Long_Y <= 14.9)//���Â炳�v���̏������ɊY�����镔�i�̂Ƃ�
		{
			BJ[i].Box_hyouka = 0; //�őO��ɔz�u�i�]���ԍ�0�j
		}
		else //����ȊO�̕��i�̂Ƃ�
		{
			BJ[i].Box_hyouka = 1; //2��ڂɔz�u�i�]���ԍ�1�j
		}

	}

	//2�i�ڂ���1�i�ڂɔz�u����(2�i�ڂ������Ȃ�����)���߂̑O����
	for (int j = 1; j <= N; j++)
	{
		for (int k = j + 1; k <= N; k++)
		{
			if (BJ[j].Volume > BJ[k].Volume)
			{
				//�̐Ϗ��ɕ��ёւ���
				temp = BJ[j].num;
				BJ[j].num = BJ[k].num;
				BJ[k].num = temp;
				temp2 = BJ[j].Volume;
				BJ[j].Volume = BJ[k].Volume;
				BJ[k].Volume = temp2;

			}
		}
	}
}

//�u�L�΂��v���Ԓl���o�p�֐�
void Time_Reach(int STEP)
{
	int flag = 0;

	for (int i = 1; i <= STEP; i++)
	{
		flag = 0;
		if (strcmp(Step[i].Motion, "�L�΂�") == 0 || strcmp(Step[i].M, "�L�΂�") == 0)//���삪�u�L�΂��v�̂Ƃ�
		{
			if (Step[i].BeforeX == Step[i].AfterX && Step[i].BeforeY == Step[i].AfterY && Step[i].BeforeZ == Step[i].AfterZ)//��������"�L�΂�"�̂Ƃ�
			{
				R_C[i].T = 0.0;
			}
			else
			{
				for (int j = i + 1; j <= STEP; j++)
				{

					//**�u�L�΂��v�ˁu�͂ݎ��v�̎��́u�L�΂��v�̎��Ԓl���o**//
					if (strcmp(Step[j].Motion, "�͂ݎ��") == 0)			//���̓��삪�u�͂ݎ��v�̎�
					{
						//**���i���̕��i�Ɏ��L�΂�=�P�[�XC,D�̎��Ԓl��K�p**//
						if (strncmp(Step[j].BeforeJig, "���i�u����", 10) == 0) //5�����ڂ܂ł��u���i�u����v�ň�v���邩
						{

							if (Step[i].Dousa_Kyori <= 20)
							{
								R_C[i].T = 2.0 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 20 && Step[i].Dousa_Kyori <= 40)
							{
								R_C[i].T = 5.1 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 40 && Step[i].Dousa_Kyori <= 60)
							{
								R_C[i].T = 6.5 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 60 && Step[i].Dousa_Kyori <= 80)
							{
								R_C[i].T = 7.5 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 80 && Step[i].Dousa_Kyori <= 100)
							{
								R_C[i].T = 8.4 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 100 && Step[i].Dousa_Kyori <= 120)
							{
								R_C[i].T = 9.1 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 120 && Step[i].Dousa_Kyori <= 140)
							{
								R_C[i].T = 9.7 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 140 && Step[i].Dousa_Kyori <= 160)
							{
								R_C[i].T = 10.3 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 160 && Step[i].Dousa_Kyori <= 180)
							{
								R_C[i].T = 10.8 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 180 && Step[i].Dousa_Kyori <= 200)
							{
								R_C[i].T = 11.4 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 200 && Step[i].Dousa_Kyori <= 220)
							{
								R_C[i].T = 11.9 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 220 && Step[i].Dousa_Kyori <= 240)
							{
								R_C[i].T = 12.5 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 240 && Step[i].Dousa_Kyori <= 260)
							{
								R_C[i].T = 13.0 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 260 && Step[i].Dousa_Kyori <= 280)
							{
								R_C[i].T = 13.6 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 280 && Step[i].Dousa_Kyori <= 300)
							{
								R_C[i].T = 14.1 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 300 && Step[i].Dousa_Kyori <= 350)
							{
								R_C[i].T = 15.5 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 350 && Step[i].Dousa_Kyori <= 400)
							{
								R_C[i].T = 16.8 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 400 && Step[i].Dousa_Kyori <= 450)
							{
								R_C[i].T = 18.2 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 450 && Step[i].Dousa_Kyori <= 500)
							{
								R_C[i].T = 19.6 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 500 && Step[i].Dousa_Kyori <= 550)
							{
								R_C[i].T = 20.9 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 550 && Step[i].Dousa_Kyori <= 600)
							{
								R_C[i].T = 22.3 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 600 && Step[i].Dousa_Kyori <= 650)
							{
								R_C[i].T = 23.6 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 650 && Step[i].Dousa_Kyori <= 700)
							{
								R_C[i].T = 25.0 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 700 && Step[i].Dousa_Kyori <= 750)
							{
								R_C[i].T = 26.4 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 750 && Step[i].Dousa_Kyori <= 800)
							{
								R_C[i].T = 27.7 * 0.036;
							}
						}
						//�H��Ɏ��L�΂� = �P�[�XA
						else
						{
							if (Step[i].Dousa_Kyori <= 20)
							{
								R_C[i].T = 2.0 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 20 && Step[i].Dousa_Kyori <= 40)
							{
								R_C[i].T = 3.4 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 40 && Step[i].Dousa_Kyori <= 60)
							{
								R_C[i].T = 4.5 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 60 && Step[i].Dousa_Kyori <= 80)
							{
								R_C[i].T = 5.5 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 80 && Step[i].Dousa_Kyori <= 100)
							{
								R_C[i].T = 6.1 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 100 && Step[i].Dousa_Kyori <= 120)
							{
								R_C[i].T = 6.4 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 120 && Step[i].Dousa_Kyori <= 140)
							{
								R_C[i].T = 6.8 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 140 && Step[i].Dousa_Kyori <= 160)
							{
								R_C[i].T = 7.1 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 160 && Step[i].Dousa_Kyori <= 180)
							{
								R_C[i].T = 7.5 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 180 && Step[i].Dousa_Kyori <= 200)
							{
								R_C[i].T = 7.8 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 200 && Step[i].Dousa_Kyori <= 220)
							{
								R_C[i].T = 8.1 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 220 && Step[i].Dousa_Kyori <= 240)
							{
								R_C[i].T = 8.5 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 240 && Step[i].Dousa_Kyori <= 260)
							{
								R_C[i].T = 8.8 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 260 && Step[i].Dousa_Kyori <= 280)
							{
								R_C[i].T = 9.2 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 280 && Step[i].Dousa_Kyori <= 300)
							{
								R_C[i].T = 9.5 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 300 && Step[i].Dousa_Kyori <= 350)
							{
								R_C[i].T = 10.4 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 350 && Step[i].Dousa_Kyori <= 400)
							{
								R_C[i].T = 11.3 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 400 && Step[i].Dousa_Kyori <= 450)
							{
								R_C[i].T = 12.1 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 450 && Step[i].Dousa_Kyori <= 500)
							{
								R_C[i].T = 13.0 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 500 && Step[i].Dousa_Kyori <= 550)
							{
								R_C[i].T = 13.9 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 550 && Step[i].Dousa_Kyori <= 600)
							{
								R_C[i].T = 14.7 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 600 && Step[i].Dousa_Kyori <= 650)
							{
								R_C[i].T = 15.6 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 650 && Step[i].Dousa_Kyori <= 700)
							{
								R_C[i].T = 16.5 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 700 && Step[i].Dousa_Kyori <= 750)
							{
								R_C[i].T = 17.3 * 0.036;
							}
							else if (Step[i].Dousa_Kyori > 750 && Step[i].Dousa_Kyori <= 800)
							{
								R_C[i].T = 18.2 * 0.036;
							}
						}
						break;
					}
					//**�u�L�΂��v�ˁu�͂ށv�̎��́u�L�΂��v�̎��Ԓl���o(�P�[�XA)**//
					else if (strcmp(Step[j].Motion, "�͂�") == 0)
					{
						if (Step[i].Dousa_Kyori <= 20)
						{
							R_C[i].T = 2.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 20 && Step[i].Dousa_Kyori <= 40)
						{
							R_C[i].T = 3.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 40 && Step[i].Dousa_Kyori <= 60)
						{
							R_C[i].T = 4.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 60 && Step[i].Dousa_Kyori <= 80)
						{
							R_C[i].T = 5.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 80 && Step[i].Dousa_Kyori <= 100)
						{
							R_C[i].T = 6.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 100 && Step[i].Dousa_Kyori <= 120)
						{
							R_C[i].T = 6.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 120 && Step[i].Dousa_Kyori <= 140)
						{
							R_C[i].T = 6.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 140 && Step[i].Dousa_Kyori <= 160)
						{
							R_C[i].T = 7.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 160 && Step[i].Dousa_Kyori <= 180)
						{
							R_C[i].T = 7.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 180 && Step[i].Dousa_Kyori <= 200)
						{
							R_C[i].T = 7.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 200 && Step[i].Dousa_Kyori <= 220)
						{
							R_C[i].T = 8.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 220 && Step[i].Dousa_Kyori <= 240)
						{
							R_C[i].T = 8.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 240 && Step[i].Dousa_Kyori <= 260)
						{
							R_C[i].T = 8.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 260 && Step[i].Dousa_Kyori <= 280)
						{
							R_C[i].T = 9.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 280 && Step[i].Dousa_Kyori <= 300)
						{
							R_C[i].T = 9.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 300 && Step[i].Dousa_Kyori <= 350)
						{
							R_C[i].T = 10.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 350 && Step[i].Dousa_Kyori <= 400)
						{
							R_C[i].T = 11.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 400 && Step[i].Dousa_Kyori <= 450)
						{
							R_C[i].T = 12.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 450 && Step[i].Dousa_Kyori <= 500)
						{
							R_C[i].T = 13.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 500 && Step[i].Dousa_Kyori <= 550)
						{
							R_C[i].T = 13.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 550 && Step[i].Dousa_Kyori <= 600)
						{
							R_C[i].T = 14.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 600 && Step[i].Dousa_Kyori <= 650)
						{
							R_C[i].T = 15.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 650 && Step[i].Dousa_Kyori <= 700)
						{
							R_C[i].T = 16.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 700 && Step[i].Dousa_Kyori <= 750)
						{
							R_C[i].T = 17.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 750 && Step[i].Dousa_Kyori <= 800)
						{
							R_C[i].T = 18.2 * 0.036;
						}
						break;
					}
					//**�u�L�΂��v�ˁu�����v�̎��́u�L�΂��v�̎��Ԓl���o(�P�[�XA)**//
					else if (strcmp(Step[j].Motion, "����") == 0)
					{
						if (Step[i].Dousa_Kyori <= 20)
						{
							R_C[i].T = 2.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 20 && Step[i].Dousa_Kyori <= 40)
						{
							R_C[i].T = 3.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 40 && Step[i].Dousa_Kyori <= 60)
						{
							R_C[i].T = 4.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 60 && Step[i].Dousa_Kyori <= 80)
						{
							R_C[i].T = 5.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 80 && Step[i].Dousa_Kyori <= 100)
						{
							R_C[i].T = 6.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 100 && Step[i].Dousa_Kyori <= 120)
						{
							R_C[i].T = 6.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 120 && Step[i].Dousa_Kyori <= 140)
						{
							R_C[i].T = 6.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 140 && Step[i].Dousa_Kyori <= 160)
						{
							R_C[i].T = 7.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 160 && Step[i].Dousa_Kyori <= 180)
						{
							R_C[i].T = 7.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 180 && Step[i].Dousa_Kyori <= 200)
						{
							R_C[i].T = 7.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 200 && Step[i].Dousa_Kyori <= 220)
						{
							R_C[i].T = 8.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 220 && Step[i].Dousa_Kyori <= 240)
						{
							R_C[i].T = 8.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 240 && Step[i].Dousa_Kyori <= 260)
						{
							R_C[i].T = 8.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 260 && Step[i].Dousa_Kyori <= 280)
						{
							R_C[i].T = 9.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 280 && Step[i].Dousa_Kyori <= 300)
						{
							R_C[i].T = 9.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 300 && Step[i].Dousa_Kyori <= 350)
						{
							R_C[i].T = 10.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 350 && Step[i].Dousa_Kyori <= 400)
						{
							R_C[i].T = 11.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 400 && Step[i].Dousa_Kyori <= 450)
						{
							R_C[i].T = 12.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 450 && Step[i].Dousa_Kyori <= 500)
						{
							R_C[i].T = 13.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 500 && Step[i].Dousa_Kyori <= 550)
						{
							R_C[i].T = 13.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 550 && Step[i].Dousa_Kyori <= 600)
						{
							R_C[i].T = 14.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 600 && Step[i].Dousa_Kyori <= 650)
						{
							R_C[i].T = 15.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 650 && Step[i].Dousa_Kyori <= 700)
						{
							R_C[i].T = 16.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 700 && Step[i].Dousa_Kyori <= 750)
						{
							R_C[i].T = 17.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 750 && Step[i].Dousa_Kyori <= 800)
						{
							R_C[i].T = 18.2 * 0.036;
						}
						break;
					}
					//**�u�L�΂��v�ˁu�L�΂��v�̎��́u�L�΂��v�̎��Ԓl���o(�P�[�XA)**//
					else if (strcmp(Step[j].Motion, "�L�΂�") == 0)
					{
						flag = 1;

						//**�O�̓���́u�L�΂��v�̎��Ԓl���P�[�XA��**//
						if (Step[i].Dousa_Kyori <= 20)
						{
							R_C[i].T = 2.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 20 && Step[i].Dousa_Kyori <= 40)
						{
							R_C[i].T = 3.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 40 && Step[i].Dousa_Kyori <= 60)
						{
							R_C[i].T = 4.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 60 && Step[i].Dousa_Kyori <= 80)
						{
							R_C[i].T = 5.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 80 && Step[i].Dousa_Kyori <= 100)
						{
							R_C[i].T = 6.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 100 && Step[i].Dousa_Kyori <= 120)
						{
							R_C[i].T = 6.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 120 && Step[i].Dousa_Kyori <= 140)
						{
							R_C[i].T = 6.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 140 && Step[i].Dousa_Kyori <= 160)
						{
							R_C[i].T = 7.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 160 && Step[i].Dousa_Kyori <= 180)
						{
							R_C[i].T = 7.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 180 && Step[i].Dousa_Kyori <= 200)
						{
							R_C[i].T = 7.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 200 && Step[i].Dousa_Kyori <= 220)
						{
							R_C[i].T = 8.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 220 && Step[i].Dousa_Kyori <= 240)
						{
							R_C[i].T = 8.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 240 && Step[i].Dousa_Kyori <= 260)
						{
							R_C[i].T = 8.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 260 && Step[i].Dousa_Kyori <= 280)
						{
							R_C[i].T = 9.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 280 && Step[i].Dousa_Kyori <= 300)
						{
							R_C[i].T = 9.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 300 && Step[i].Dousa_Kyori <= 350)
						{
							R_C[i].T = 10.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 350 && Step[i].Dousa_Kyori <= 400)
						{
							R_C[i].T = 11.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 400 && Step[i].Dousa_Kyori <= 450)
						{
							R_C[i].T = 12.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 450 && Step[i].Dousa_Kyori <= 500)
						{
							R_C[i].T = 13.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 500 && Step[i].Dousa_Kyori <= 550)
						{
							R_C[i].T = 13.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 550 && Step[i].Dousa_Kyori <= 600)
						{
							R_C[i].T = 14.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 600 && Step[i].Dousa_Kyori <= 650)
						{
							R_C[i].T = 15.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 650 && Step[i].Dousa_Kyori <= 700)
						{
							R_C[i].T = 16.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 700 && Step[i].Dousa_Kyori <= 750)
						{
							R_C[i].T = 17.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 750 && Step[i].Dousa_Kyori <= 800)
						{
							R_C[i].T = 18.2 * 0.036;
						}


						//���̓���ԍ��́u�L�΂��v�̎��Ԓl���P�[�XA��//
						if (Step[j].Dousa_Kyori <= 20)
						{
							R_C[j].T = 2.0 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 20 && Step[j].Dousa_Kyori <= 40)
						{
							R_C[j].T = 3.4 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 40 && Step[j].Dousa_Kyori <= 60)
						{
							R_C[j].T = 4.5 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 60 && Step[j].Dousa_Kyori <= 80)
						{
							R_C[j].T = 5.5 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 80 && Step[j].Dousa_Kyori <= 100)
						{
							R_C[j].T = 6.1 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 100 && Step[j].Dousa_Kyori <= 120)
						{
							R_C[j].T = 6.4 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 120 && Step[j].Dousa_Kyori <= 140)
						{
							R_C[j].T = 6.8 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 140 && Step[j].Dousa_Kyori <= 160)
						{
							R_C[j].T = 7.1 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 160 && Step[j].Dousa_Kyori <= 180)
						{
							R_C[j].T = 7.5 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 180 && Step[j].Dousa_Kyori <= 200)
						{
							R_C[j].T = 7.8 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 200 && Step[j].Dousa_Kyori <= 220)
						{
							R_C[j].T = 8.1 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 220 && Step[j].Dousa_Kyori <= 240)
						{
							R_C[j].T = 8.5 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 240 && Step[j].Dousa_Kyori <= 260)
						{
							R_C[j].T = 8.8 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 260 && Step[j].Dousa_Kyori <= 280)
						{
							R_C[j - 1].T = 9.2 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 280 && Step[j].Dousa_Kyori <= 300)
						{
							R_C[j].T = 9.5 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 300 && Step[j].Dousa_Kyori <= 350)
						{
							R_C[j].T = 10.4 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 350 && Step[j].Dousa_Kyori <= 400)
						{
							R_C[j].T = 11.3 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 400 && Step[j].Dousa_Kyori <= 450)
						{
							R_C[j].T = 12.1 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 450 && Step[j].Dousa_Kyori <= 500)
						{
							R_C[j].T = 13.0 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 500 && Step[j].Dousa_Kyori <= 550)
						{
							R_C[j].T = 13.9 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 550 && Step[j].Dousa_Kyori <= 600)
						{
							R_C[j].T = 14.7 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 600 && Step[j].Dousa_Kyori <= 650)
						{
							R_C[j].T = 15.6 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 650 && Step[j].Dousa_Kyori <= 700)
						{
							R_C[j].T = 16.5 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 700 && Step[j].Dousa_Kyori <= 750)
						{
							R_C[j].T = 17.3 * 0.036;
						}
						else if (Step[j].Dousa_Kyori > 750 && Step[j].Dousa_Kyori <= 800)
						{
							R_C[j].T = 18.2 * 0.036;
						}
						break;
					}
				}
			}
			if (flag == 1)
			{
				STime_Reach += R_C[i].T;//���v���ԗp
				STime_Reach += R_C[i + 1].T;//���v���ԗp
			}
			else
			{
				STime_Reach += R_C[i].T;//���v���ԗp
			}

		}
	}
}

//�u���������v���Ԓl���o�֐�
void Time_Turn(int STEP)
{
	for (int i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "��������") == 0)//���삪�u���������v�̂Ƃ�
		{
			if (fabs(Step[i].BeforeX) == 90 || fabs(Step[i].BeforeY) == 90 || fabs(Step[i].BeforeZ) == 90 || fabs(Step[i].AfterX) == 90 || fabs(Step[i].AfterY) == 90 || fabs(Step[i].AfterZ) == 90)
			{
				R_C[i].T = 5.4 * 0.036;
			}
			else if (fabs(Step[i].BeforeX) == 135 || fabs(Step[i].BeforeY) == 135 || fabs(Step[i].BeforeZ) == 135 || fabs(Step[i].AfterX) == 135 || fabs(Step[i].AfterY) == 135 || fabs(Step[i].AfterZ) == 135)
			{
				R_C[i].T = 7.4 * 0.036;
			}
			else if (fabs(Step[i].BeforeX) == 180 || fabs(Step[i].BeforeY) == 180 || fabs(Step[i].BeforeZ) == 180 || fabs(Step[i].AfterX) == 180 || fabs(Step[i].AfterY) == 180 || fabs(Step[i].AfterZ) == 180)
			{
				R_C[i].T = 9.4 * 0.036;
			}

			STime_Turn += R_C[i].T;//���v���ԗp

			All_Angle = All_Angle + Step[i].BeforeX + Step[i].BeforeY + Step[i].BeforeZ + Step[i].AfterX + Step[i].AfterY + Step[i].AfterZ;//����]�p�x�p
		}
	}
}

//�u�^�ԁv���Ԓl���o�p�֐�
void Time_Bring(int STEP)
{
	for (int i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "�^��") == 0 || strcmp(Step[i].M, "�^��") == 0)//���삪�u�^�ԁv�̂Ƃ�
		{
			if (Step[i].BeforeX == Step[i].AfterX && Step[i].BeforeY == Step[i].AfterY && Step[i].BeforeZ == Step[i].AfterZ)//��������"�^��"�̂Ƃ�
			{
				R_C[i].T = 0.0;
			}
			else
			{
				for (int j = i + 1; j <= STEP; j++)
				{
					//�u�^�ԁv�ˁu�g���Ă�v�̎��́u�^�ԁv�̎��Ԓl���o�i�P�[�XC�j
					if (strcmp(Step[j].Motion, "�g���Ă�") == 0)
					{
						if (Step[i].Dousa_Kyori <= 20)
						{
							R_C[i].T = 2.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 20 && Step[i].Dousa_Kyori <= 40)
						{
							R_C[i].T = 4.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 40 && Step[i].Dousa_Kyori <= 60)
						{
							R_C[i].T = 5.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 60 && Step[i].Dousa_Kyori <= 80)
						{
							R_C[i].T = 6.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 80 && Step[i].Dousa_Kyori <= 100)
						{
							R_C[i].T = 7.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 100 && Step[i].Dousa_Kyori <= 120)
						{
							R_C[i].T = 8.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 120 && Step[i].Dousa_Kyori <= 140)
						{
							R_C[i].T = 9.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 140 && Step[i].Dousa_Kyori <= 160)
						{
							R_C[i].T = 10.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 160 && Step[i].Dousa_Kyori <= 180)
						{
							R_C[i].T = 11.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 180 && Step[i].Dousa_Kyori <= 200)
						{
							R_C[i].T = 11.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 200 && Step[i].Dousa_Kyori <= 220)
						{
							R_C[i].T = 12.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 220 && Step[i].Dousa_Kyori <= 240)
						{
							R_C[i].T = 13.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 240 && Step[i].Dousa_Kyori <= 260)
						{
							R_C[i].T = 13.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 260 && Step[i].Dousa_Kyori <= 280)
						{
							R_C[i].T = 14.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 280 && Step[i].Dousa_Kyori <= 300)
						{
							R_C[i].T = 15.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 300 && Step[i].Dousa_Kyori <= 350)
						{
							R_C[i].T = 16.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 350 && Step[i].Dousa_Kyori <= 400)
						{
							R_C[i].T = 18.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 400 && Step[i].Dousa_Kyori <= 450)
						{
							R_C[i].T = 20.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 450 && Step[i].Dousa_Kyori <= 500)
						{
							R_C[i].T = 21.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 500 && Step[i].Dousa_Kyori <= 550)
						{
							R_C[i].T = 23.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 550 && Step[i].Dousa_Kyori <= 600)
						{
							R_C[i].T = 25.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 600 && Step[i].Dousa_Kyori <= 650)
						{
							R_C[i].T = 26.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 650 && Step[i].Dousa_Kyori <= 700)
						{
							R_C[i].T = 28.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 700 && Step[i].Dousa_Kyori <= 750)
						{
							R_C[i].T = 30.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 750 && Step[i].Dousa_Kyori <= 800)
						{
							R_C[i].T = 32.0 * 0.036;
						}
						break;
					}
					//�u�^�ԁv�ˁu�t����v�̎��́u�^�ԁv�̎��Ԓl���o�i�P�[�XC�j
					else if (strcmp(Step[j].Motion, "�t����") == 0)
					{
						if (Step[i].Dousa_Kyori <= 20)
						{
							R_C[i].T = 2.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 20 && Step[i].Dousa_Kyori <= 40)
						{
							R_C[i].T = 4.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 40 && Step[i].Dousa_Kyori <= 60)
						{
							R_C[i].T = 5.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 60 && Step[i].Dousa_Kyori <= 80)
						{
							R_C[i].T = 6.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 80 && Step[i].Dousa_Kyori <= 100)
						{
							R_C[i].T = 7.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 100 && Step[i].Dousa_Kyori <= 120)
						{
							R_C[i].T = 8.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 120 && Step[i].Dousa_Kyori <= 140)
						{
							R_C[i].T = 9.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 140 && Step[i].Dousa_Kyori <= 160)
						{
							R_C[i].T = 10.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 160 && Step[i].Dousa_Kyori <= 180)
						{
							R_C[i].T = 11.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 180 && Step[i].Dousa_Kyori <= 200)
						{
							R_C[i].T = 11.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 200 && Step[i].Dousa_Kyori <= 220)
						{
							R_C[i].T = 12.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 220 && Step[i].Dousa_Kyori <= 240)
						{
							R_C[i].T = 13.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 240 && Step[i].Dousa_Kyori <= 260)
						{
							R_C[i].T = 13.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 260 && Step[i].Dousa_Kyori <= 280)
						{
							R_C[i].T = 14.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 280 && Step[i].Dousa_Kyori <= 300)
						{
							R_C[i].T = 15.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 300 && Step[i].Dousa_Kyori <= 350)
						{
							R_C[i].T = 16.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 350 && Step[i].Dousa_Kyori <= 400)
						{
							R_C[i].T = 18.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 400 && Step[i].Dousa_Kyori <= 450)
						{
							R_C[i].T = 20.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 450 && Step[i].Dousa_Kyori <= 500)
						{
							R_C[i].T = 21.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 500 && Step[i].Dousa_Kyori <= 550)
						{
							R_C[i].T = 23.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 550 && Step[i].Dousa_Kyori <= 600)
						{
							R_C[i].T = 25.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 600 && Step[i].Dousa_Kyori <= 650)
						{
							R_C[i].T = 26.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 650 && Step[i].Dousa_Kyori <= 700)
						{
							R_C[i].T = 28.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 700 && Step[i].Dousa_Kyori <= 750)
						{
							R_C[i].T = 30.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 750 && Step[i].Dousa_Kyori <= 800)
						{
							R_C[i].T = 32.0 * 0.036;
						}
						break;
					}
					//�u�^�ԁv�ˁu�g���ړ�����v�̎��́u�^�ԁv�̎��Ԓl���o�i�P�[�XC�j
					else if (strcmp(Step[j].Motion, "�g���ړ�����") == 0)
					{

						if (Step[i].Dousa_Kyori <= 20)
						{
							R_C[i].T = 2.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 20 && Step[i].Dousa_Kyori <= 40)
						{
							R_C[i].T = 4.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 40 && Step[i].Dousa_Kyori <= 60)
						{
							R_C[i].T = 5.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 60 && Step[i].Dousa_Kyori <= 80)
						{
							R_C[i].T = 6.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 80 && Step[i].Dousa_Kyori <= 100)
						{
							R_C[i].T = 7.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 100 && Step[i].Dousa_Kyori <= 120)
						{
							R_C[i].T = 8.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 120 && Step[i].Dousa_Kyori <= 140)
						{
							R_C[i].T = 9.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 140 && Step[i].Dousa_Kyori <= 160)
						{
							R_C[i].T = 10.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 160 && Step[i].Dousa_Kyori <= 180)
						{
							R_C[i].T = 11.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 180 && Step[i].Dousa_Kyori <= 200)
						{
							R_C[i].T = 11.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 200 && Step[i].Dousa_Kyori <= 220)
						{
							R_C[i].T = 12.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 220 && Step[i].Dousa_Kyori <= 240)
						{
							R_C[i].T = 13.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 240 && Step[i].Dousa_Kyori <= 260)
						{
							R_C[i].T = 13.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 260 && Step[i].Dousa_Kyori <= 280)
						{
							R_C[i].T = 14.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 280 && Step[i].Dousa_Kyori <= 300)
						{
							R_C[i].T = 15.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 300 && Step[i].Dousa_Kyori <= 350)
						{
							R_C[i].T = 16.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 350 && Step[i].Dousa_Kyori <= 400)
						{
							R_C[i].T = 18.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 400 && Step[i].Dousa_Kyori <= 450)
						{
							R_C[i].T = 20.1 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 450 && Step[i].Dousa_Kyori <= 500)
						{
							R_C[i].T = 21.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 500 && Step[i].Dousa_Kyori <= 550)
						{
							R_C[i].T = 23.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 550 && Step[i].Dousa_Kyori <= 600)
						{
							R_C[i].T = 25.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 600 && Step[i].Dousa_Kyori <= 650)
						{
							R_C[i].T = 26.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 650 && Step[i].Dousa_Kyori <= 700)
						{
							R_C[i].T = 28.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 700 && Step[i].Dousa_Kyori <= 750)
						{
							R_C[i].T = 30.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 750 && Step[i].Dousa_Kyori <= 800)
						{
							R_C[i].T = 32.0 * 0.036;
						}
						break;
					}
					//�u�^�ԁv�ˁu�u�������v�̎��́u�^�ԁv�̎��Ԓl���o�i�P�[�XB�j
					else if (strcmp(Step[j].Motion, "�u������") == 0)
					{
						if (Step[i].Dousa_Kyori <= 20)
						{
							R_C[i].T = 2.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 20 && Step[i].Dousa_Kyori <= 40)
						{
							R_C[i].T = 4.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 40 && Step[i].Dousa_Kyori <= 60)
						{
							R_C[i].T = 5.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 60 && Step[i].Dousa_Kyori <= 80)
						{
							R_C[i].T = 5.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 80 && Step[i].Dousa_Kyori <= 100)
						{
							R_C[i].T = 6.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 100 && Step[i].Dousa_Kyori <= 120)
						{
							R_C[i].T = 7.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 120 && Step[i].Dousa_Kyori <= 140)
						{
							R_C[i].T = 8.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 140 && Step[i].Dousa_Kyori <= 160)
						{
							R_C[i].T = 9.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 160 && Step[i].Dousa_Kyori <= 180)
						{
							R_C[i].T = 9.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 180 && Step[i].Dousa_Kyori <= 200)
						{
							R_C[i].T = 10.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 200 && Step[i].Dousa_Kyori <= 220)
						{
							R_C[i].T = 11.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 220 && Step[i].Dousa_Kyori <= 240)
						{
							R_C[i].T = 11.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 240 && Step[i].Dousa_Kyori <= 260)
						{
							R_C[i].T = 12.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 260 && Step[i].Dousa_Kyori <= 280)
						{
							R_C[i].T = 12.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 280 && Step[i].Dousa_Kyori <= 300)
						{
							R_C[i].T = 13.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 300 && Step[i].Dousa_Kyori <= 350)
						{
							R_C[i].T = 14.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 350 && Step[i].Dousa_Kyori <= 400)
						{
							R_C[i].T = 15.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 400 && Step[i].Dousa_Kyori <= 450)
						{
							R_C[i].T = 16.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 450 && Step[i].Dousa_Kyori <= 500)
						{
							R_C[i].T = 18.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 500 && Step[i].Dousa_Kyori <= 550)
						{
							R_C[i].T = 19.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 550 && Step[i].Dousa_Kyori <= 600)
						{
							R_C[i].T = 20.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 600 && Step[i].Dousa_Kyori <= 650)
						{
							R_C[i].T = 21.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 650 && Step[i].Dousa_Kyori <= 700)
						{
							R_C[i].T = 22.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 700 && Step[i].Dousa_Kyori <= 750)
						{
							R_C[i].T = 24.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 750 && Step[i].Dousa_Kyori <= 800)
						{
							R_C[i].T = 25.2 * 0.036;
						}
						break;
					}
					//�u�^�ԁv�ˁu�����v�̎��́u�^�ԁv�̎��Ԓl���o�i�P�[�XB�j
					else if (strcmp(Step[j].Motion, "����") == 0)
					{
						if (Step[i].Dousa_Kyori <= 20)
						{
							R_C[i].T = 2.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 20 && Step[i].Dousa_Kyori <= 40)
						{
							R_C[i].T = 4.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 40 && Step[i].Dousa_Kyori <= 60)
						{
							R_C[i].T = 5.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 60 && Step[i].Dousa_Kyori <= 80)
						{
							R_C[i].T = 5.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 80 && Step[i].Dousa_Kyori <= 100)
						{
							R_C[i].T = 6.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 100 && Step[i].Dousa_Kyori <= 120)
						{
							R_C[i].T = 7.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 120 && Step[i].Dousa_Kyori <= 140)
						{
							R_C[i].T = 8.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 140 && Step[i].Dousa_Kyori <= 160)
						{
							R_C[i].T = 9.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 160 && Step[i].Dousa_Kyori <= 180)
						{
							R_C[i].T = 9.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 180 && Step[i].Dousa_Kyori <= 200)
						{
							R_C[i].T = 10.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 200 && Step[i].Dousa_Kyori <= 220)
						{
							R_C[i].T = 11.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 220 && Step[i].Dousa_Kyori <= 240)
						{
							R_C[i].T = 11.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 240 && Step[i].Dousa_Kyori <= 260)
						{
							R_C[i].T = 12.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 260 && Step[i].Dousa_Kyori <= 280)
						{
							R_C[i].T = 12.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 280 && Step[i].Dousa_Kyori <= 300)
						{
							R_C[i].T = 13.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 300 && Step[i].Dousa_Kyori <= 350)
						{
							R_C[i].T = 14.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 350 && Step[i].Dousa_Kyori <= 400)
						{
							R_C[i].T = 15.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 400 && Step[i].Dousa_Kyori <= 450)
						{
							R_C[i].T = 16.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 450 && Step[i].Dousa_Kyori <= 500)
						{
							R_C[i].T = 18.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 500 && Step[i].Dousa_Kyori <= 550)
						{
							R_C[i].T = 19.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 550 && Step[i].Dousa_Kyori <= 600)
						{
							R_C[i].T = 20.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 600 && Step[i].Dousa_Kyori <= 650)
						{
							R_C[i].T = 21.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 650 && Step[i].Dousa_Kyori <= 700)
						{
							R_C[i].T = 22.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 700 && Step[i].Dousa_Kyori <= 750)
						{
							R_C[i].T = 24.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 750 && Step[i].Dousa_Kyori <= 800)
						{
							R_C[i].T = 25.2 * 0.036;
						}
						break;
					}
					//�u�^�ԁv�ˁu�^�ԁv�̎���1�ڂ́u�^�ԁv�̎��Ԓl���o�i�P�[�XB�j
					else if (strcmp(Step[j].Motion, "�^��") == 0)
					{
						if (Step[i].Dousa_Kyori <= 20)
						{
							R_C[i].T = 2.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 20 && Step[i].Dousa_Kyori <= 40)
						{
							R_C[i].T = 4.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 40 && Step[i].Dousa_Kyori <= 60)
						{
							R_C[i].T = 5.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 60 && Step[i].Dousa_Kyori <= 80)
						{
							R_C[i].T = 5.9 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 80 && Step[i].Dousa_Kyori <= 100)
						{
							R_C[i].T = 6.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 100 && Step[i].Dousa_Kyori <= 120)
						{
							R_C[i].T = 7.7 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 120 && Step[i].Dousa_Kyori <= 140)
						{
							R_C[i].T = 8.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 140 && Step[i].Dousa_Kyori <= 160)
						{
							R_C[i].T = 9.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 160 && Step[i].Dousa_Kyori <= 180)
						{
							R_C[i].T = 9.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 180 && Step[i].Dousa_Kyori <= 200)
						{
							R_C[i].T = 10.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 200 && Step[i].Dousa_Kyori <= 220)
						{
							R_C[i].T = 11.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 220 && Step[i].Dousa_Kyori <= 240)
						{
							R_C[i].T = 11.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 240 && Step[i].Dousa_Kyori <= 260)
						{
							R_C[i].T = 12.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 260 && Step[i].Dousa_Kyori <= 280)
						{
							R_C[i].T = 12.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 280 && Step[i].Dousa_Kyori <= 300)
						{
							R_C[i].T = 13.3 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 300 && Step[i].Dousa_Kyori <= 350)
						{
							R_C[i].T = 14.5 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 350 && Step[i].Dousa_Kyori <= 400)
						{
							R_C[i].T = 15.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 400 && Step[i].Dousa_Kyori <= 450)
						{
							R_C[i].T = 16.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 450 && Step[i].Dousa_Kyori <= 500)
						{
							R_C[i].T = 18.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 500 && Step[i].Dousa_Kyori <= 550)
						{
							R_C[i].T = 19.2 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 550 && Step[i].Dousa_Kyori <= 600)
						{
							R_C[i].T = 20.4 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 600 && Step[i].Dousa_Kyori <= 650)
						{
							R_C[i].T = 21.6 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 650 && Step[i].Dousa_Kyori <= 700)
						{
							R_C[i].T = 22.8 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 700 && Step[i].Dousa_Kyori <= 750)
						{
							R_C[i].T = 24.0 * 0.036;
						}
						else if (Step[i].Dousa_Kyori > 750 && Step[i].Dousa_Kyori <= 800)
						{
							R_C[i].T = 25.2 * 0.036;
						}
						break;
					}
				}
			}
			STime_Bring += R_C[i].T;//���v���ԗp
		}
	}
}

//�u�͂ݎ��v���Ԓl���o�p�֐�
void Time_grab(int STEP)
{

	for (int i = 1; i <= STEP; i++)
	{

		if (strcmp(Step[i].Motion, "�͂ݎ��") == 0 || strcmp(Step[i].M, "�͂ݎ��") == 0)//���삪�u�͂ݎ��v�̂Ƃ�
		{
			R_C[i].T = 0.126;

			STime_Grab += R_C[i].T;//���v���ԗp

			//	if (strncmp(Step[i].BeforeJig, "���i�u����", 5) == 0)		//���i���̕��i��͂ނƂ�
			//		//if (strcmp(Step[j].BeforeJig, "���i�u����") == 0)		//���i���̕��i��͂ނƂ�
			//	{
			//		printf("���i%d�͑��̂��̂Ɠ��荬�����Ă܂����iYes:1,No:2�j:", Step[i].KoP);
			//		scanf("%d", &Step[i].Case4_1);

			//		if (Step[i].Case4_1 == 1)//�P�[�X4�n�ɊY���������
			//		{

			//			if (R_B[Step[i].KoP].Long_X < 6 && R_B[Step[i].KoP].Long_Y < 6 && R_B[Step[i].KoP].Long_Z < 3)	//�P�[�X4C
			//			{
			//				R_C[i].T = 12.9 * 0.036;
			//			}
			//			else if (R_B[Step[i].KoP].Long_X < 25 && R_B[Step[i].KoP].Long_Y < 25 && R_B[Step[i].KoP].Long_Z < 25)		//�P�[�X4B
			//			{
			//				R_C[i].T = 9.1 * 0.036;
			//			}
			//			else//�P�[�X4A
			//			{
			//				R_C[i].T = 7.3 * 0.036;
			//			}

			//		}
			//		else if (Step[i].Case4_1 == 2)//�P�[�X1�n�ɊY���������
			//		{
			//			printf("���i%d�̉~���`�̒�ʂ�1���ʂɏ�Q������܂����iYes:1,No:2�j:", Step[i].KoP);
			//			scanf("%d", &Step[i].Case4_2);

			//			if (Step[i].Case4_2 == 1)//�P�[�X1C�n�ɊY���������
			//			{
			//				printf("���i%d�̒��a(mm)����͂��Ă�������(����)�F", Step[i].KoP);
			//				scanf("%d", &Step[i].Case1C);

			//				if (Step[i].Case1C < 6)		//���a��6�o�����������Ƃ�
			//				{
			//					R_C[i].T = 10.8 * 0.036;
			//				}
			//				else if (Step[i].Case1C >= 6 && Step[i].Case1C < 12)		//���a��6�o����12�o�܂ł̂���
			//				{
			//					R_C[i].T = 8.7 * 0.036;
			//				}
			//				else if (Step[i].Case1C >= 12)
			//				{
			//					R_C[i].T = 7.3 * 0.036;
			//				}

			//			}
			//			else if (Step[i].Case4_2 == 2)//�P�[�X1A�܂���1B�ɊY���������
			//			{
			//				//swap(i);
			//				if (R_B[Step[i].KoP].Long_X * R_B[Step[i].KoP].Long_Y < 316)		//���Â炳�v���́u�������v�ɊY���������
			//				{
			//					R_C[i].T = 3.5 * 0.036;
			//				}
			//				else
			//				{
			//					R_C[i].T = 2.0 * 0.036;
			//				}
			//			}
			//		}

			//	}

			//	else//�H���͂ނƂ�
			//	{
			//		R_C[i].T = 2.0 * 0.036;			//�H���͂�=�P�[�X1A
			//	}
		}
	}
}

//�u�g���Ă�v�A�u�g���ړ�����v���Ԓl���o�p�֐�
void Time_Assemble(int STEP)
{
	for (int i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "�g���Ă�") == 0 || strcmp(Step[i].M, "�g���Ă�") == 0)//���삪�u�g���Ă�v�̂Ƃ�
		{
			R_C[i].T = 0.5;//�u�g���Ă�v�̎��Ԓl��0.5[sec]

			R_C[i - 1].T = 0.5;//�u�g���ړ�����v�̎��Ԓl��0.5[sec]

			//for (int k = 1; k <= K; k++)
			//{
			//	if ((Step[i].Assy1 == R_K.p1[k] && Step[i].Assy2 == R_K.p2[k]) || (Step[i].Assy2 == R_K.p1[k] && Step[i].Assy1 == R_K.p2[k]))//�g�ݕt���镔�i�̏�񂩂�ڐG�ԍ�����肷��
			//	{

			//		for (int l = 1; l <= A; l++)
			//		{
			//			//�ǂ̐ڐG���𖞂����̂�
			//			if (R_K.Sessyoku[k][l] == 1)
			//			{
			//				if (R_Se[l].Time > R_C[i].T)//�����̐ڐG���𖞂����ꍇ�A�傫�����̎��Ԓl��p����
			//				{
			//					R_C[i].T = R_Se[l].Time;
			//				}
			//			}
			//		}
			//	}

			//}
		}
	}

}

//���ёւ��֐�
void swap(int i)
{
	double temp;

	if (R_B[i].Long_X < R_B[i].Long_Y)
	{
		temp = R_B[i].Long_X;
		R_B[i].Long_X = R_B[i].Long_Y;
		R_B[i].Long_Y = temp;
	}

	if (R_B[i].Long_X < R_B[i].Long_Z)
	{
		temp = R_B[i].Long_X;
		R_B[i].Long_X = R_B[i].Long_Z;
		R_B[i].Long_Z = temp;
	}

	if (R_B[i].Long_Y < R_B[i].Long_Z)
	{
		temp = R_B[i].Long_Y;
		R_B[i].Long_Y = R_B[i].Long_Z;
		R_B[i].Long_Z = temp;
	}

}

//S0�ύX�p�֐�
void S0_Change(int STEP)
{
	int countB = 0;//���i���J�E���g�p
	int count = 0;
	int countFR = 0;
	int countFR2 = 0;
	int countR = 0;//�E�舵�����i�J�E���g
	int countR1 = 0;//����1��ڃJ�E���g
	int countR2 = 0;//����2��ڃJ�E���g
	int countL1 = 0;//�E��1��ڃJ�E���g
	int countL2 = 0;//�E��2��ڃJ�E���g
	int Jigflag = 0;
	int center;//���S����
	int centerflow;//���S����牽����邩
	int BeforeJig = 0;
	int flag = 0;
	double tempX;
	double tempR1;
	double tempR2;
	double tempL1;
	double tempL2;
	double tempMax = 0.0;//�����i���̈ʒu�p
	double tempMax2 = 0.0;
	double tempBoxsizeL1;
	double tempBoxsizeL2;

	countJ = 0;

	R_H.TR = R_H.TR + 1;//�g���C�̐���1���₷�i���u����p�j

	for (int i = 1; i <= STEP; i++)
	{
		//if (strcmp(Step[i].Motion, "�͂ݎ��") == 0 || strcmp(Step[i].Motion, "�͂�") == 0)//���삪�u�͂ݎ��v���́u�͂ށv�̂Ƃ�
		if (strcmp(Step[i].Motion, "�͂ݎ��") == 0)//���삪�u�͂ݎ��]�̂Ƃ�
		{
			for (int j = 1; j <= N; j++)
			{
				if (Step[i].KoP == j)//���ޑΏۂ��P���i��������
				{
					countB++;
					BJ[countB].Number = j;//���i�̏o�������L�^

					if (strcmp(Step[i].Hand, "����") == 0)
					{
						BJ[j].Box_hand = 0;
					}
					else if (strcmp(Step[i].Hand, "�E��") == 0)
					{
						if (BJ[j].Box_hyouka == 0)//���̕��i���őO��ɔz�u���鎞
						{
							countF++;//�J�E���g
						}
						BJ[j].Box_hand = 1;
						countR++;//�E��ň������i���J�E���g
					}
				}
			}
		}
		else if (strcmp(Step[i].Motion, "�t����") == 0 && strcmp(Step[i].BeforeJig, "-10") == 0)//���삪�u�t����v���Ώۂ��H��̂Ƃ��i�H��p�^�[��1�p�j
		{
			for (int j = 1; j <= N; j++)
			{

				if (Step[i].KoP == j)//���ޑΏۂ��P���i��������
				{
					for (int k = 1; k <= countB; k++)
					{
						if (Step[i].KoP == BJ[k].Number)
						{
							flag = 1;
							break;
						}
					}
					if (flag == 0)
					{
						countB++;
						BJ[countB].Number = j;//���i�̏o�������L�^

						if (strcmp(Step[i].Hand, "����") == 0)
						{
							BJ[j].Box_hand = 0;
						}
						else if (strcmp(Step[i].Hand, "�E��") == 0)
						{
							if (BJ[j].Box_hyouka == 0)//���̕��i���őO��ɔz�u���鎞
							{
								countF++;//�J�E���g
							}
							BJ[j].Box_hand = 1;
							countR++;//�E��ň������i���J�E���g
						}
					}
				}

			}
			flag = 0;
		}

		if (strcmp(Step[i].Motion, "�t����") == 0 && atoi(Step[i].BeforeJig) != BeforeJig && atoi(Step[i].BeforeJig) != -10)
		{
			Jigflag = 0;
			for (int k = 1; k <= N; k++)
			{
				if (Step[i].KoP == k)//����ɕt���镔�i���P���i�������Ƃ����x�[�X���i���ω������Ƃ�
				{
					Jigflag = 1;
					break;
				}
				else if (Step[i].KoP != k)//����ɕt���镔�i�����Ԑ��i�������Ƃ����p�����ω������Ƃ�
				{
					Jigflag = 0;
				}
			}
			if (Jigflag == 1)
			{
				//�x�[�X���i�ɍ��킹������ɕύX
				countJ++;
				Ray.Jig_x[countJ] = BJ[Step[i].KoP].Jig_x;
				Ray.Jig_y[countJ] = BJ[Step[i].KoP].Jig_y;
				Ray.Jig_z[countJ] = BJ[Step[i].KoP].Jig_z;
			}
			else if (Jigflag == 0)
			{
				for (int Tyukan = 1; Tyukan <= T; Tyukan++)
				{
					if (Step[i].KoP == R_T[Tyukan].Tyunum)//�Y�����钆�Ԑ��i��T��
					{
						countJ++;
						Ray.Jig_x[countJ] = R_T[Tyukan].lx + 20;
						Ray.Jig_y[countJ] = R_T[Tyukan].ly + 20;
						Ray.Jig_z[countJ] = 20;
						break;
					}
				}
			}
			BeforeJig = atoi(Step[i].BeforeJig);
		}
	}

	//**�E2��ڂ̕��i�����ꍇ1��ڂɔz�u���鏈��**//
	if (countR / 2 != countF)//(�E��ň������i��)/2���E�O��̌��ƈ�v���ĂȂ��Ƃ�=1��ڂ�2��ڂ̃o�����X�������Ƃ�
	{
		for (int j = 1; j <= N; j++)
		{
			if (BJ[j].Box_hand == 1 && BJ[j].Box_hyouka == 1)//2��ڊ��E���ɔz�u���镔�i�̂Ƃ�
			{
				countFR++;
				Ray.num[countFR] = j;//2��ډE���̕��i���i�[				
			}
		}
		//(�E��ň������i��)/2���E�O��̌��ƈ�v����܂őO��Ɏ����Ă���
	}

	for (int k = 1; k <= N; k++)
	{
		for (int l = 1; l <= countFR; l++)
		{
			if (Ray.num[k] == BJ[l].num)//2��ډE���̕��i�̒����畔�i������������1��ڂɔz�u����
			{
				countFR2++;
				BJ[Ray.num[k]].Box_hyouka = 0;//1��ڂɔz�u����
				break;
			}
		}
		if (countFR2 == (countR / 2 - countF))//1��ڂ�2��ڂ̃o�����X���ǂ��Ȃ�����I��
		{
			break;
		}
	}

	//****������W�̌���****//
	if (countJ % 2 == 1)//����̐���"�"�̂Ƃ�
	{
		center = (countJ + 1) / 2;
		centerflow = countJ / 2;
		//�����ɔz�u�����������(0,125,850)�ɂ���
		Ray.Jig_xCo[center] = tempX = 0;
		Ray.Jig_yCo[center] = 125;
		Ray.Jig_zCo[center] = 850;

		if (centerflow != 0)//���1�ȏ�
		{
			//**��Ǝ҂��猩�ĉE���̎���ɂ���**//
			for (int Rodd = 0; Rodd < centerflow; Rodd++)
			{
				//printf("%4.2lf,%4.2lf\n", Ray.Jig_x[center - x] / 2, Ray.Jig_x[center - x - 1] / 2);
				Ray.Jig_xCo[center - Rodd - 1] = tempX + (Ray.Jig_x[center - Rodd] / 2) + 64 + (Ray.Jig_x[center - Rodd - 1] / 2);
				tempX = Ray.Jig_xCo[center - Rodd - 1];
				Ray.Jig_yCo[center - Rodd - 1] = 125;
				Ray.Jig_zCo[center - Rodd - 1] = 850;
			}

			//**��Ǝ҂��猩�č����̎���ɂ���**//
			tempX = 0;//��𒆐S�ɖ߂�
			for (int Lodd = 0; Lodd < centerflow; Lodd++)
			{
				Ray.Jig_xCo[center + Lodd + 1] = tempX - (Ray.Jig_x[center + Lodd] / 2) - 64 - (Ray.Jig_x[center + Lodd + 1] / 2);
				tempX = Ray.Jig_xCo[center + Lodd + 1];
				Ray.Jig_yCo[center + Lodd + 1] = 125;
				Ray.Jig_zCo[center + Lodd + 1] = 850;
			}
		}
	}
	else//����̐���"����"�̂Ƃ�
	{
		centerflow = countJ / 2;
		tempX = 0;
		//**��Ǝ҂��猩�ĉE���̎���ɂ���**//
		for (int Reven = 0; Reven < centerflow; Reven++)
		{
			if (Reven == 0)
			{
				Ray.Jig_xCo[centerflow - Reven] = 32 + (Ray.Jig_x[centerflow - Reven] / 2);
				tempX = Ray.Jig_xCo[centerflow - Reven];
				Ray.Jig_yCo[centerflow - Reven] = 125;
				Ray.Jig_zCo[centerflow - Reven] = 850;
			}
			else
			{
				Ray.Jig_xCo[centerflow - Reven] = tempX + (Ray.Jig_x[centerflow - Reven + 1] / 2) + 64 + (Ray.Jig_x[centerflow - Reven] / 2);
				tempX = Ray.Jig_xCo[centerflow - Reven];
				Ray.Jig_yCo[centerflow - Reven] = 125;
				Ray.Jig_zCo[centerflow - Reven] = 850;
			}
		}
		//**��Ǝ҂��猩�č����̎���ɂ���**//
		for (int Leven = 1; Leven <= centerflow; Leven++)
		{
			if (Leven == 1)
			{
				Ray.Jig_xCo[centerflow + Leven] = -32 - (Ray.Jig_x[centerflow + Leven] / 2);
				tempX = Ray.Jig_xCo[centerflow + Leven];
				Ray.Jig_yCo[centerflow + Leven] = 125;
				Ray.Jig_zCo[centerflow + Leven] = 850;
			}
			else
			{
				Ray.Jig_xCo[centerflow + Leven] = tempX - (Ray.Jig_x[centerflow + Leven - 1] / 2) - 64 - (Ray.Jig_x[centerflow + Leven] / 2);
				tempX = Ray.Jig_xCo[centerflow + Leven];
				Ray.Jig_yCo[centerflow + Leven] = 125;
				Ray.Jig_zCo[centerflow + Leven] = 850;
			}
		}
	}

	//***����̍��W�E�傫������***//
	Beforejignum = 2 + R_H.PA + R_H.T;
	count++;
	for (int x = 1; x <= R_H.J; x++)
	{
		if (count <= countJ)//�g�p���鎡��̍��W����
		{
			R_S0[Beforejignum + x].lx = Ray.Jig_xCo[count];
			R_S0[Beforejignum + x].ly = Ray.Jig_yCo[count];
			R_S0[Beforejignum + x].lz = Ray.Jig_zCo[count];
			R_S0[Beforejignum + x].Bx = Ray.Jig_x[count];
			R_S0[Beforejignum + x].By = Ray.Jig_y[count];
			R_S0[Beforejignum + x].Bz = Ray.Jig_z[count];
			count++;
		}
		else//�g�p���Ȃ�����͑S��0
		{
			R_S0[Beforejignum + x].lx = 0.0;
			R_S0[Beforejignum + x].ly = 0.0;
			R_S0[Beforejignum + x].lz = 0.0;
			R_S0[Beforejignum + x].Bx = 0.0;
			R_S0[Beforejignum + x].By = 0.0;
			R_S0[Beforejignum + x].Bz = 0.0;
		}
	}

	//***���̑傫��������***//
	R_S0[Beforejignum + R_H.J + 1].lx = 1000;//x���W

	tempR1 = 0.0;//������
	tempR2 = 0.0;

	//****���i���E���i�̍��W����****//
	for (int y = 1; y <= N; y++)//�o�����鏇��
	{
		if (BJ[BJ[y].Number].Box_hand == 0)//���i��������ň����Ƃ�
		{
			if (BJ[BJ[y].Number].Box_hyouka == 0)//���i��"������"�Ƃ�=1���
			{
				//printf("%4.2lfaiueo\n", BJ[BJ[y].Number].Box_x);
				Ray.Box_xCo[BJ[y].Number] = tempR1 - BJ[BJ[y].Number].Box_x;
				tempR1 = Ray.Box_xCo[BJ[y].Number];
				Ray.Box_yCo[BJ[y].Number] = 300;
				Ray.Box_zCo[BJ[y].Number] = 850;
				Ray.Parts_xCo[BJ[y].Number] = tempR1 + (BJ[BJ[y].Number].Box_x / 2);
				Ray.Parts_yCo[BJ[y].Number] = Ray.Box_yCo[BJ[y].Number] + (BJ[BJ[y].Number].Box_y) / 2;
				Ray.Parts_zCo[BJ[y].Number] = 860;
				Ray.Box_inclination[BJ[y].Number] = 0;//���i���̌X�����w��
				tempMax = Ray.Box_xCo[BJ[y].Number];//�����i���p�̊i�[
			}
			else if (BJ[BJ[y].Number].Box_hyouka == 1)//����ȊO=2���
			{
				Ray.Box_xCo[BJ[y].Number] = tempR2 - BJ[BJ[y].Number].Box_x;
				tempR2 = Ray.Box_xCo[BJ[y].Number];
				Ray.Box_yCo[BJ[y].Number] = 300;
				Ray.Box_zCo[BJ[y].Number] = 1050;
				Ray.Parts_xCo[BJ[y].Number] = tempR2 + (BJ[BJ[y].Number].Box_x / 2);
				Ray.Parts_yCo[BJ[y].Number] = Ray.Box_yCo[BJ[y].Number] - 10 + (BJ[BJ[y].Number].Box_y) / 2;
				//Ray.Parts_zCo[BJ[y].Number] = Ray.Box_zCo[BJ[y].Number] + (BJ[BJ[y].Number].Box_y * sin(20 * 3.141592 / 180) / 2) + (R_B[y].Ykotei / 2) * tan(20 * 3.141592 / 180) + R_B[y].Zkotei / 2;
				Ray.Parts_zCo[BJ[y].Number] = 1100;
				if (sqrt(((Ray.Parts_xCo[BJ[y].Number] + 200) * (Ray.Parts_xCo[BJ[y].Number] + 200) + (Ray.Parts_yCo[BJ[y].Number] * Ray.Parts_yCo[BJ[y].Number]))) > 600)//���i�̈ʒu���ő��ƈ�𒴂����ꍇ
				{

				}
				Ray.Box_inclination[BJ[y].Number] = -20;//���i���̌X�����w��

			}
		}
		else//���i�����E��ň����Ƃ�
		{
			if (BJ[BJ[y].Number].Box_hyouka == 0)//���i��"������"�Ƃ�=1���
			{
				if (countL1 == 0)
				{
					Ray.Box_xCo[BJ[y].Number] = 0;
					tempL1 = Ray.Box_xCo[BJ[y].Number];
					tempBoxsizeL1 = BJ[BJ[y].Number].Box_x;
					Ray.Box_yCo[BJ[y].Number] = 300;
					Ray.Box_zCo[BJ[y].Number] = 850;
					Ray.Parts_xCo[BJ[y].Number] = tempL1 + (tempBoxsizeL1) / 2;
					Ray.Parts_yCo[BJ[y].Number] = Ray.Box_yCo[BJ[y].Number] + (BJ[BJ[y].Number].Box_y) / 2;
					Ray.Parts_zCo[BJ[y].Number] = 860;
					countL1++;
				}
				else
				{
					Ray.Box_xCo[BJ[y].Number] = tempL1 + tempBoxsizeL1;
					tempL1 = Ray.Box_xCo[BJ[y].Number];
					tempBoxsizeL1 = BJ[BJ[y].Number].Box_x;
					Ray.Box_yCo[BJ[y].Number] = 300;
					Ray.Box_zCo[BJ[y].Number] = 850;
					Ray.Parts_xCo[BJ[y].Number] = tempL1 + (tempBoxsizeL1) / 2;
					Ray.Parts_yCo[BJ[y].Number] = Ray.Box_yCo[BJ[y].Number] + (BJ[BJ[y].Number].Box_y) / 2;
					Ray.Parts_zCo[BJ[y].Number] = 860;
				}
				Ray.Box_inclination[BJ[y].Number] = 0;//���i���̌X�����w��

			}
			else if (BJ[BJ[y].Number].Box_hyouka == 1)//����ȊO=2���
			{
				if (countL2 == 0)//���S������1�ڂ̕��i��
				{
					Ray.Box_xCo[BJ[y].Number] = 0;
					tempL2 = Ray.Box_xCo[BJ[y].Number];
					tempBoxsizeL2 = BJ[BJ[y].Number].Box_x;
					Ray.Box_yCo[BJ[y].Number] = 300;
					Ray.Box_zCo[BJ[y].Number] = 1050;
					Ray.Parts_xCo[BJ[y].Number] = tempL2 + (tempBoxsizeL2) / 2;
					Ray.Parts_yCo[BJ[y].Number] = Ray.Box_yCo[BJ[y].Number] + (BJ[BJ[y].Number].Box_y) / 2;
					Ray.Parts_zCo[BJ[y].Number] = 1100;
					//Ray.Parts_zCo[BJ[y].Number] = Ray.Box_zCo[BJ[y].Number] + (BJ[BJ[y].Number].Box_y * sin(20 * 3.141592 / 180) / 2) + (R_B[y].Ykotei / 2) * tan(20 * 3.141592 / 180) + R_B[y].Zkotei / 2;
					countL2++;
				}
				else
				{
					Ray.Box_xCo[BJ[y].Number] = tempL2 + tempBoxsizeL2;
					tempL2 = Ray.Box_xCo[BJ[y].Number];
					tempBoxsizeL2 = BJ[BJ[y].Number].Box_x;
					Ray.Box_yCo[BJ[y].Number] = 300;
					Ray.Box_zCo[BJ[y].Number] = 1050;
					Ray.Parts_xCo[BJ[y].Number] = tempL2 + (tempBoxsizeL2) / 2;
					Ray.Parts_yCo[BJ[y].Number] = Ray.Box_yCo[BJ[y].Number] + (BJ[BJ[y].Number].Box_y) / 2;
					//Ray.Parts_zCo[BJ[y].Number] = Ray.Box_zCo[BJ[y].Number] + (BJ[BJ[y].Number].Box_y * sin(20 * 3.141592 / 180) / 2) + (R_B[y].Ykotei / 2) * tan(20 * 3.141592 / 180) + R_B[y].Zkotei / 2;
					Ray.Parts_zCo[BJ[y].Number] = 1100;
				}
				if (sqrt(((Ray.Parts_xCo[BJ[y].Number] - 200) * (Ray.Parts_xCo[BJ[y].Number] - 200) + (Ray.Parts_yCo[BJ[y].Number] * Ray.Parts_yCo[BJ[y].Number]))) > 600)//���i�̈ʒu���ő��ƈ�𒴂����ꍇ
				{

				}
				Ray.Box_inclination[BJ[y].Number] = -20;//���i���̌X�����w��

			}
		}
		if (y == N)
		{
			tempMax2 = tempMax;//�ł����̕��i���̍��W���i�[
		}
	}

	//***���i���̍��W�E�傫������***//
	Beforetraynum = 2 + R_H.PA + R_H.T + R_H.J + R_H.MI + R_H.TA;
	for (int tray = 1; tray <= N; tray++)//�o�����镔�i
	{
		if (R_B[tray].Box_type == 1)
		{
			R_S0[tray + Beforetraynum].lz = 4;
		}
		R_S0[tray + Beforetraynum].ly = 2;
		R_S0[tray + Beforetraynum].Bx = Ray.Box_xCo[tray];
		R_S0[tray + Beforetraynum].By = Ray.Box_yCo[tray];
		R_S0[tray + Beforetraynum].Bz = Ray.Box_zCo[tray];
		R_S0[tray + Beforetraynum].Hx = BJ[tray].Box_x;
		R_S0[tray + Beforetraynum].Hy = BJ[tray].Box_y;
		R_S0[tray + Beforetraynum].Hz = BJ[tray].Box_z;
		R_S0[tray + Beforetraynum].C = Ray.Box_inclination[tray];
	}

	//***���i�̍��W����***//
	for (int Parts = 1; Parts <= N; Parts++)
	{
		R_S0[2 + Parts].lx = Ray.Parts_xCo[Parts];
		R_S0[2 + Parts].ly = Ray.Parts_yCo[Parts];
		R_S0[2 + Parts].lz = Ray.Parts_zCo[Parts];
	}

	//***�����i���̍��W�E�傫������***//
	Beforefinishboxnum = 2 + R_H.PA + R_H.T + R_H.J + R_H.MI + R_H.TA + N;
	R_S0[1 + Beforefinishboxnum].Bx = tempMax2 - 350;//X���W(�ł����̕��i���̐^���ɔz�u����)
	R_S0[1 + Beforefinishboxnum].By = 200;//Y���W
	R_S0[1 + Beforefinishboxnum].Bz = 850;//Z���W
	R_S0[1 + Beforefinishboxnum].Hx = R_T[1].lx + 10;//X�̒���(�����i�̑傫��+10mm)
	R_S0[1 + Beforefinishboxnum].Hy = R_T[1].ly + 10;//Y�̒���(�����i�̑傫��+10mm)
	R_S0[1 + Beforefinishboxnum].Hz = 20;//Z�̒���
	R_S0[1 + Beforefinishboxnum].C = 0;//�X��

	//***�����i���̒��S�_�̌���***//
	Ray.finish_centerX = R_S0[1 + Beforefinishboxnum].Bx + (R_S0[1 + Beforefinishboxnum].Hx / 2);
	Ray.finish_centerY = R_S0[1 + Beforefinishboxnum].By + (R_S0[1 + Beforefinishboxnum].Hy / 2);
	Ray.finish_centerZ = 855;

	//***���u����̍��W�E�傫������***//
	R_S0[2 + Beforefinishboxnum].Bx = R_S0[Beforejignum + countJ].lx - R_S0[Beforejignum + countJ].Bx - 100;//X���W
	R_S0[2 + Beforefinishboxnum].By = R_S0[Beforejignum + countJ].ly - 25;//Y���W
	R_S0[2 + Beforefinishboxnum].Bz = R_S0[Beforejignum + countJ].lz;//Z���W
	R_S0[2 + Beforefinishboxnum].Hx = 100;//X�̒���
	R_S0[2 + Beforefinishboxnum].Hy = 80;//Y�̒���
	R_S0[2 + Beforefinishboxnum].Hz = 25;//Z�̒���
	R_S0[2 + Beforefinishboxnum].C = 0;//�X��
	R_S0[2 + Beforefinishboxnum].S = 5;//����
	R_S0[2 + Beforefinishboxnum].N = R_S0[1 + Beforefinishboxnum].N + 1;//�ԍ�
	R_S0[2 + Beforefinishboxnum].lx = 1;
	R_S0[2 + Beforefinishboxnum].ly = 1;
	R_S0[2 + Beforefinishboxnum].lz = 4;//�F

	//***���u����̒��S�_�̌���***//
	Ray.karioki_centerX = R_S0[2 + Beforefinishboxnum].Bx + (R_S0[2 + Beforefinishboxnum].Hx / 2);
	Ray.karioki_centerY = R_S0[2 + Beforefinishboxnum].By + (R_S0[2 + Beforefinishboxnum].Hy / 2);
	Ray.karioki_centerZ = 855;

	Beforekougunum = 2 + R_H.PA;

	//***�H��̍��W������
	for (int Kougu = 1; Kougu <= R_H.T; Kougu++)
	{
		R_S0[Beforekougunum + Kougu].lx = 200;//�E��̏����ʒu
		R_S0[Beforekougunum + Kougu].ly = 0;
		R_S0[Beforekougunum + Kougu].lz = 1240;//������390mm�̈ʒu
	}

}

//������W�ύX�֐�
void C_Change(int STEP)
{
	int i;
	int j;
	int k;
	int l;
	int tempJig;//int�ϊ��p
	int flag = 0;
	double temp;
	double temp_Buhin = 0.0;//���i�̒������i�[���Ă����ϐ�

	for (i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "�L�΂�") == 0)//���삪"�L�΂�"�̂Ƃ�
		{
			if (strcmp(Step[i + 1].Motion, "�͂ݎ��") == 0)//���̓��삪"�͂ݎ��"�̂Ƃ�
			{
				for (k = 1; k <= N; k++)
				{
					if (Step[i + 1].KoP == k)//�͂ݎ����̂��P���i��������
					{
						Step[i].AfterX = R_S0[2 + k].lx;//�L�΂����X���W������
						Step[i].AfterY = R_S0[2 + k].ly;//�L�΂����Y���W������
						Step[i].AfterZ = R_S0[2 + k].lz;//�L�΂����Z���W������

						for (l = i; l <= STEP; l++)
						{
							if (strcmp(Step[l].Motion, "�^��") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//���̓��삪"�^��"���p����肪��v������
							{
								Step[l].BeforeX = Step[i].AfterX;//�^�ԑO��X���W������
								Step[l].BeforeY = Step[i].AfterY;//�^�ԑO��Y���W������
								Step[l].BeforeZ = Step[i].AfterZ;//�^�ԑO��Z���W������
								break;
							}
						}
					}
					else//�P���i�ł͂Ȃ��Ƃ��������̕��i��͂ݎ��or���u����(��������or�P�Ǝ���)�̕��i��͂ݎ��
					{
						for (int Jig = 1; Jig <= R_H.J; Jig++)
						{
							tempJig = atoi(Step[i + 1].BeforeJig);
							if (R_S0[Beforejignum + Jig].N == tempJig)
							{
								Step[i].AfterX = R_S0[Beforejignum + Jig].lx;//�L�΂����X���W������
								Step[i].AfterY = R_S0[Beforejignum + Jig].ly;//�L�΂����Y���W������
								Step[i].AfterZ = R_S0[Beforejignum + Jig].lz;//�L�΂����Y���W������
								flag = 1;
								break;
							}
						}
						if (flag == 0)//���u����ɂ��镔�i��͂ݎ��Ƃ�
						{
							for (int beforedousa = i; beforedousa >= 1; beforedousa--)
							{
								if ((strcmp(Step[beforedousa].Motion, "�u������") == 0 && Step[i + 1].KoP == Step[beforedousa].KoP)
									|| (strcmp(Step[beforedousa].Motion, "�g���Ă�") == 0 && Step[i + 1].KoP == Step[beforedousa].Assy12))//�ȑO�̓��삩��("�u������"���Ώە�������̂���)||("�g���Ă�"���Ώە�������̂���)��T��
								{
									Step[i].AfterX = Step[beforedousa - 1].AfterX;//�u�������O�̉^�Ԃ�B�ƈ�v
									Step[i].AfterY = Step[beforedousa - 1].AfterY;
									Step[i].AfterZ = Step[beforedousa - 1].AfterZ;
									break;
								}
							}
						}
						flag = 0;
						for (l = i; l <= STEP; l++)
						{
							if (strcmp(Step[l].Motion, "�^��") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//���̓��삪"�^��"���p����肪��v������
							{

								Step[l].BeforeX = Step[i].AfterX;//�^�ԑO��X���W������
								Step[l].BeforeY = Step[i].AfterY;//�^�ԑO��Y���W������
								Step[l].BeforeZ = Step[i].AfterZ;//�^�ԑO��Y���W������
								break;
							}
						}
					}
				}
			}
			else if (strcmp(Step[i + 1].Motion, "�͂�") == 0)//���̓��삪"�͂�"�̂Ƃ�
			{
				if (strcmp(Step[i + 2].Motion, "�O��") == 0)//���̎��̓��삪"�O��"�̂Ƃ�
				{
					for (int Jig2 = 1; Jig2 <= R_H.J; Jig2++)
					{
						tempJig = atoi(Step[i + 2].BeforeJig);
						if (R_S0[Beforejignum + Jig2].N == tempJig)
						{
							Step[i].AfterX = R_S0[Beforejignum + Jig2].lx;//�L�΂����X���W������
							Step[i].AfterY = R_S0[Beforejignum + Jig2].ly;//�L�΂����Y���W������
							Step[i].AfterZ = R_S0[Beforejignum + Jig2].lz;//�L�΂����Z���W������
							break;
						}
					}
					for (l = i; l <= STEP; l++)
					{
						if (strcmp(Step[l].Motion, "�^��") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//���̓��삪"�^��"���p����肪��v������
						{
							Step[l].BeforeX = Step[i].AfterX;//�^�ԑO��X���W������
							Step[l].BeforeY = Step[i].AfterY;//�^�ԑO��Y���W������
							Step[l].BeforeZ = Step[i].AfterZ;//�^�ԑO��Z���W������
							break;
						}
					}
				}
				else//�H���͂ނ܂��͍H��g�t���ŕ��i���x����Ƃ�
				{
					for (int Kougu = 1; Kougu <= R_H.T; Kougu++)
					{
						if (Step[i + 1].KoP == R_S0[Beforekougunum + Kougu].N)//�H���͂ނƂ�
						{
							flag = 1;
							Step[i].AfterX = R_S0[Beforekougunum + Kougu].lx;//�L�΂����X���W������
							Step[i].AfterY = R_S0[Beforekougunum + Kougu].ly;//�L�΂����Y���W������
							Step[i].AfterZ = R_S0[Beforekougunum + Kougu].lz;//�L�΂����Z���W������
							break;
						}
					}
					if (flag == 0)//�H��g�t���ŕ��i���x����Ƃ�
					{
						for (int beforedousa = i; beforedousa >= 1; beforedousa--)
						{
							if (strcmp(Step[beforedousa].Motion, "�͂ݎ��") == 0 && Step[beforedousa].KoP == Step[i + 1].KoP)//�ΏۂƂ��Ă���͂ݎ���T��
							{
								Step[i].AfterX = Step[beforedousa + 1].AfterX;
								Step[i].AfterY = Step[beforedousa + 1].AfterY;
								Step[i].AfterZ = Step[beforedousa + 1].AfterZ;
							}
						}
					}
					for (l = i + 1; l <= STEP; l++)
					{
						if ((strcmp(Step[l].Motion, "�^��") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0) || (strcmp(Step[l].Motion, "�L�΂�") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0))//���̓��삪"�^��"���p����肪��v������
						{
							Step[l].BeforeX = Step[i].AfterX;//�^�ԑO��X���W������
							Step[l].BeforeY = Step[i].AfterY;//�^�ԑO��Y���W������
							Step[l].BeforeZ = Step[i].AfterZ;//�^�ԑO��Z���W������
							break;
						}
					}
				}
			}
			else if (strcmp(Step[i + 1].Motion, "�t����") == 0)//���̓��삪"�t����"�̂Ƃ�
			{
				for (k = 1; k <= N; k++)
				{
					if (Step[i + 1].KoP == k)//�͂ݎ����̂��P���i��������
					{
						Step[i].AfterX = R_S0[2 + k].lx;//�L�΂����X���W������
						Step[i].AfterY = R_S0[2 + k].ly;//�L�΂����Y���W������
						Step[i].AfterZ = R_S0[2 + k].lz;//�L�΂����Z���W������

						for (l = i; l <= STEP; l++)
						{

							if (strcmp(Step[l].Motion, "�^��") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//���̓��삪"�^��"���p����肪��v������
							{
								Step[l].BeforeX = Step[i].AfterX;//�^�ԑO��X���W������
								Step[l].BeforeY = Step[i].AfterY;//�^�ԑO��Y���W������
								Step[l].BeforeZ = Step[i].AfterZ;//�^�ԑO��Z���W������
								break;
							}
						}
					}
				}
			}
			else if (strcmp(Step[i + 1].Motion, "�L�΂�") == 0)//���̓��삪"�L�΂�"�̂Ƃ�
			{
				//i��i+1�̐L�΂���B�������ʒu�ɂ���

				break;
			}
		}
		else if (strcmp(Step[i].Motion, "�^��") == 0)//���삪"�^��"�̂Ƃ�
		{
			for (j = i; j <= STEP; j++)//"��������"�p
			{
				if (strcmp(Step[j].Motion, "�u������") == 0)//���ɏo�Ă��铮�삪"�u������"�̂Ƃ�
				{
					if (Step[j].KoP == -10)
					{
						for (int Kougu = 1; Kougu <= R_H.T; Kougu++)
						{
							if (Step[j].KoP == R_S0[Beforekougunum + Kougu].N)
							{
								Step[i].AfterX = R_S0[Beforekougunum + Kougu].lx;//�L�΂����X���W������
								Step[i].AfterY = R_S0[Beforekougunum + Kougu].ly;//�L�΂����Y���W������
								Step[i].AfterZ = R_S0[Beforekougunum + Kougu].lz;//�L�΂����Y���W������
								break;
							}
						}
					}
					else
					{
						if (strcmp(Step[j + 1].Motion, "�L�΂�") == 0 && strcmp(Step[j + 2].Motion, "�L�΂�") == 0)//�����i���ɒu������
						{
							Step[i].AfterX = Ray.finish_centerX;//�^�񂾌��X���W������
							Step[i].AfterY = Ray.finish_centerY;//�^�񂾌��Y���W������
							Step[i].AfterZ = Ray.finish_centerZ;//�^�񂾌��Z���W������
						}
						else//���u����ɒu�������Ƃ�
						{
							Step[i].AfterX = Ray.karioki_centerX;//�^�񂾌��X���W=��ԍ��̎���̍����ɔz�u
							Step[i].AfterY = Ray.karioki_centerY;//�^�񂾌��Y���W������
							Step[i].AfterZ = Ray.karioki_centerZ;//�^�񂾌��Z���W������
						}

					}
					for (l = i; l <= STEP; l++)
					{
						if (strcmp(Step[l].Motion, "�L�΂�") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//���̓��삪"�L�΂�"���p����肪��v������
						{
							Step[l].BeforeX = Step[i].AfterX;//�^�ԑO��X���W������
							Step[l].BeforeY = Step[i].AfterY;//�^�ԑO��Y���W������
							Step[l].BeforeZ = Step[i].AfterZ;//�^�ԑO��Y���W������
							break;
						}
					}
					break;
				}
				else if (strcmp(Step[j].Motion, "�t����") == 0)//���ɏo�Ă��铮�삪"�t����"�̂Ƃ�
				{
					
					for (int k = 1; k <= N - 1; k++)
					{
						if (atoi(Step[j].BeforeJig) == R_KJUNJO.Jig[k])//�g���������玡�����������
						{

							if (R_KJUNJO.Dir[k] == 1)//����ɕt�����i�̕�����X�����̎�
							{
								if (Step[j].KoP <= N)//����ɕt�����i���P���i�̎�
								{
									for (int B1 = 1; B1 <= N; B1++)
									{
										if (Step[j].KoP == R_B[B1].Bu_num)//����ɕt�����i��T��
										{
											temp_Buhin = R_B[B1].Xkotei / 2;

											break;
										}
									}
								}
								else//����ɕt�����i�����Ԑ��i�̎�
								{
									for (int B2 = 1; B2 <= T; B2++)
									{
										if (Step[j].KoP == R_T[B2].Tyunum)
										{
											temp_Buhin = R_T[B2].Xkotei / 2;

											break;
										}
									}
								}
							}
							else if (R_KJUNJO.Dir[k] == 2)//����ɕt�����i�̕�����Y�����̎�
							{
								if (Step[j].KoP <= N)//����ɕt�����i���P���i�̎�
								{
									for (int B1 = 1; B1 <= N; B1++)
									{
										if (Step[j].KoP == R_B[B1].Bu_num)//����ɕt�����i��T��
										{
											temp_Buhin = R_B[B1].Ykotei / 2;

											break;
										}
									}
								}
								else//����ɕt�����i�����Ԑ��i�̎�
								{
									for (int B2 = 1; B2 <= T; B2++)
									{
										if (Step[j].KoP == R_T[B2].Tyunum)
										{
											temp_Buhin = R_T[B2].Ykotei / 2;

											break;
										}
									}
								}
							}
							else//����ɕt�����i�̕�����Z�����̎�
							{
								if (Step[j].KoP <= N)//����ɕt�����i���P���i�̎�
								{
									for (int B1 = 1; B1 <= N; B1++)
									{
										if (Step[j].KoP == R_B[B1].Bu_num)//����ɕt�����i��T��
										{
											temp_Buhin = R_B[B1].Zkotei / 2;

											break;
										}
									}
								}
								else//����ɕt�����i�����Ԑ��i�̎�
								{
									for (int B2 = 1; B2 <= T; B2++)
									{
										if (Step[j].KoP == R_T[B2].Tyunum)
										{
											temp_Buhin = R_T[B2].Zkotei / 2;

											break;
										}
									}
								}
							}
							
							break;
						}
					}
					
					for (int Jig3 = 1; Jig3 <= R_H.J; Jig3++)
					{
						tempJig = atoi(Step[j].BeforeJig);
						if (R_S0[Beforejignum + Jig3].N == tempJig)
						{
							flag = 1;
							Step[i].AfterX = R_S0[Beforejignum + Jig3].lx;//�^�񂾌��X���W������
							Step[i].AfterY = R_S0[Beforejignum + Jig3].ly;//�^�񂾌��Y���W������
							Step[i].AfterZ = R_S0[Beforejignum + Jig3].lz + R_S0[Beforejignum + Jig3].Bz + temp_Buhin;//�^�񂾌��Z���W�����聚
							break;
						}
					}
					if (flag == 0)//�H��ɕ��i��t����Ƃ�
					{
						for (int m = i - 1; m >= 1; m--)
						{
							if (strcmp(Step[m].Motion, "�^��") == 0)
							{
								temp = Step[m].AfterZ;
								break;
							}
						}
						for (k = j; k <= STEP; k++)
						{
							if (strcmp(Step[k].Motion, "�g���Ă�") == 0)
							{
								for (int Jig4 = 1; Jig4 <= R_H.J; Jig4++)
								{
									if (R_S0[Beforejignum + Jig4].N == Step[k].Case)//"�g���Ă�"�����T���ɂ���
									{
										Step[i].AfterX = R_S0[Beforejignum + Jig4].lx;//�^�񂾌��X���W������
										Step[i].AfterY = R_S0[Beforejignum + Jig4].ly;//�^�񂾌��Y���W������
										Step[i].AfterZ = temp;//�^�񂾌��Z���W������
										break;
									}
								}
								break;
							}
						}
					}
					for (l = i; l <= STEP; l++)
					{
						if (strcmp(Step[l].Motion, "�L�΂�") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//���̓��삪"�L�΂�"���p����肪��v������
						{
							Step[l].BeforeX = Step[i].AfterX;//�L�΂��O��X���W������
							Step[l].BeforeY = Step[i].AfterY;//�L�΂��O��Y���W������
							Step[l].BeforeZ = Step[i].AfterZ;//�L�΂��O��Z���W������
							break;
						}
					}
					flag = 0;
					break;
				}
				else if (strcmp(Step[j].Motion, "�g���ړ�����") == 0)//���ɏo�Ă��铮�삪"�g���ړ�����"�̂Ƃ�
				{
					for (int Jig4 = 1; Jig4 <= R_H.J; Jig4++)
					{
						tempJig = atoi(Step[j + 1].Sup4);
						if (Step[j + 1].flag == 1)//�H��g�t���̂Ƃ�
						{
							if (R_S0[Beforejignum + Jig4].N == Step[j + 1].Case)//"�g���Ă�"�����T���ɂ���
							{
								Step[i].AfterX = R_S0[Beforejignum + Jig4].lx - Step[Step[j + 1].Assy2].X_Dif;//�^�񂾌��X���W������
								Step[i].AfterY = R_S0[Beforejignum + Jig4].ly - Step[Step[j + 1].Assy2].Y_Dif;//�^�񂾌��Y���W������
								Step[i].AfterZ = R_S0[Beforejignum + Jig4].lz - Step[Step[j + 1].Assy2].Z_Dif + R_S0[Beforejignum + Jig4].Bz + temp_Buhin;//�^�񂾌��Z���W�����聚

								//temp_Buhin = 0.0;
								//Step[i].RIchiX2 = Step[i].RIchiX1;
								//Step[i].RIchiY2 = Step[i].RIchiX1;
								break;
							}
						}
						else
						{
							if (R_S0[Beforejignum + Jig4].N == tempJig)//"�g���Ă�"�����T���ɂ���
							{
								Step[i].AfterX = R_S0[Beforejignum + Jig4].lx - Step[Step[j + 1].Assy2].X_Dif;//�^�񂾌��X���W������
								Step[i].AfterY = R_S0[Beforejignum + Jig4].ly - Step[Step[j + 1].Assy2].Y_Dif;//�^�񂾌��Y���W������
								Step[i].AfterZ = R_S0[Beforejignum + Jig4].lz - Step[Step[j + 1].Assy2].Z_Dif + R_S0[Beforejignum + Jig4].Bz + temp_Buhin;//�^�񂾌��Z���W�����聚
								//temp_Buhin = 0.0;
								//Step[i].RIchiX2 = Step[i].RIchiX1;
								//Step[i].RIchiY2 = Step[i].RIchiX1;
								break;
							}
						}
					}
					Step[j].BeforeX = Step[j].AfterX = Step[i].AfterX;//"�g���ړ�����"��APP,OFFSET�̌���
					Step[j].BeforeY = Step[j].AfterY = Step[i].AfterY;
					Step[j].BeforeZ = Step[i].AfterZ;
					Step[j].AfterZ = Step[j].BeforeZ - Step[j].OFFSET;

					break;
				}
				else if (strcmp(Step[j].Motion, "�L�΂�") == 0)//���ɏo�Ă��铮�삪"�L�΂�"�̂Ƃ��i�H��p�^�[��2�p�j
				{
					for (l = j; l <= STEP; l++)
					{
						if (strcmp(Step[l].Motion, "�g���Ă�") == 0)//���ȍ~�ɏo�Ă���"�g���Ă�"��T��
						{
							for (int Jig4 = 1; Jig4 <= R_H.J; Jig4++)
							{
								if (R_S0[Beforejignum + Jig4].N == Step[l].Case)//"�g���Ă�"�����T���ɂ���
								{
									Step[i].AfterX = R_S0[Beforejignum + Jig4].lx;//�^�񂾌��X���W������
									Step[i].AfterY = R_S0[Beforejignum + Jig4].ly;//�^�񂾌��Y���W������
									Step[i].AfterZ = R_S0[Beforejignum + Jig4].lz + 50;//�^�񂾌��Z���W������
									break;
								}
							}
							break;
						}
					}
					break;
				}

			}
		}
		else if (strcmp(Step[i].Motion, "�g���ړ�����") == 0)//���삪"�g���ړ�����"�̂Ƃ�
		{
			for (l = i; l <= STEP; l++)
			{
				if (strcmp(Step[l].Motion, "�L�΂�") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//���̓��삪"�L�΂�"���p����肪��v������
				{
					Step[l].BeforeX = Step[i].AfterX;//�L�΂��O��X���W������
					Step[l].BeforeY = Step[i].AfterY;//�L�΂��O��Y���W������
					Step[l].BeforeZ = Step[i].AfterZ;//�L�΂��O��Z���W������
					break;
				}
			}
		}
	}
}

//���싗���ύX�p�֐�
void Kyori_Change(int STEP)
{
	for (int i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "�L�΂�") == 0 || strcmp(Step[i].Motion, "�^��") == 0 || strcmp(Step[i].Motion, "�g���ړ�����") == 0)//���삪"�L�΂�"�܂���"�^��"�܂���"�g���ړ�����"�̂Ƃ�
		{
			Step[i].Dousa_Kyori = sqrt((fabs(Step[i].AfterX - Step[i].BeforeX) * fabs(Step[i].AfterX - Step[i].BeforeX)) + (fabs(Step[i].AfterY - Step[i].BeforeY) * fabs(Step[i].AfterY - Step[i].BeforeY)) + (fabs(Step[i].AfterZ - Step[i].BeforeZ) * fabs(Step[i].AfterZ - Step[i].BeforeZ)));		//A,B�̍��W��񂩂狗���𓱏o
		}
	}
}

void Write_VFDL(int STEP, int* n, int vfdlnum)
{
	FILE* fp;
	char m16a4[STR_MAX];

	int F_Count = 0;

	//sprintf(a, "VFDL%d", VFDL_No);

	//�o�͐�̃t���p�X��shima�ɑ������

	strcpy(m16a4, OutPath);
	strcat(m16a4, Conv_Char(vfdlnum));
	strcat(m16a4, ".csv");

	/***�o�͂���t�@�C�����J��***/

	//  �����ŁA�t�@�C���|�C���^���擾���� 

	if ((fp = fopen(m16a4, "w")) == NULL)				// �G���[�̏ꍇ�͒ʏ�A�ُ�I������ 
	{
		exit(EXIT_FAILURE);
	}
	//�w�b�_�̏o��
	fprintf(fp, "%d,%d,%d\n", R_H.W, R_H.L, R_H.P);							//��ƎҔԍ�,���C���ԍ�,�H���ԍ�
	fprintf(fp, "%d,%d,%d,%d,%d\n", R_H.H, R_H.Ht, R_H.V, R_H.C, R_H.S);		//�X�e�b�v��,�L���X�e�b�v��,���x�w��,���`�F�b�N,��Ԕԍ�
	fprintf(fp, "%d,%d,%d\n", R_H.PA, R_H.T, R_H.J);							//���i��,�H�,���
	fprintf(fp, "%d\n", R_H.MI);												//���ԕ��i��
	fprintf(fp, "%d,%d,%d\n", R_H.MO, R_H.MO1, R_H.MO2);												//�������ԕ��i��
	fprintf(fp, "%d,%d,%d\n\n", R_H.TA, R_H.TR, R_H.WD);					//���̐�,�g���C�̐�,�ʉߓ_��

	//�����ʒu�̏o��
	for (int i = 1; i <= G + 1; i++)
	{
		if (R_S0[i].N >= -299 && R_S0[i].N < -100)
		{
			fprintf(fp, "%d,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%d,%d\n", R_S0[i].N, R_S0[i].lx, R_S0[i].ly, R_S0[i].lz, R_S0[i].Hx, R_S0[i].Hy, R_S0[i].Hz, R_S0[i].Bx, R_S0[i].By, R_S0[i].Bz, R_S0[i].C, R_S0[i].S);
		}
		else if (R_S0[i].N >= 8000 && R_S0[i].N < 9000)
		{
			fprintf(fp, "%d,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf\n", R_S0[i].N, R_S0[i].lx, R_S0[i].ly, R_S0[i].lz, R_S0[i].Hx, R_S0[i].Hy, R_S0[i].Hz, R_S0[i].Bx, R_S0[i].By, R_S0[i].Bz);
		}
		else if (R_S0[i].N >= 9000 && R_S0[i].N < 10000)
		{
			fprintf(fp, "%d,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%d,%4.2lf,%4.2lf,%4.2lf,%d,0,180\n", R_S0[i].N, R_S0[i].lx, R_S0[i].ly, R_S0[i].lz, R_S0[i].Hx, R_S0[i].Hy, R_S0[i].Hz, R_S0[i].S, R_S0[i].Bx, R_S0[i].By, R_S0[i].Bz, R_S0[i].C);
		}
		else
		{
			fprintf(fp, "%d,%4.2lf,%4.2lf,%4.2lf,%d,%d,%d\n", R_S0[i].N, R_S0[i].lx, R_S0[i].ly, R_S0[i].lz, R_S0[i].C, R_S0[i].X, R_S0[i].Y);
		}
	}


	for (int j = 1; j <= STEP; j++)
	{
		if (strcmp(Step[j].M, "F") == 0)
		{
			F_Count++;

			fprintf(fp, "\n");

			fprintf(fp, "F,%d,%d,%d,%d,%.3lf,%d,%d\n", F_Count, R_C[j].NF, R_C[j].HL, R_C[j].Flag, R_C[j].T, R_C[j].AC, R_C[j].AS);

			//**�Y���X�e�b�v�܂ł̍��v���Ԃ̌v�Z**//
			if (j == 1)
			{
				R_C[j].STime = 0.0;

				R_C[j].STime = R_C[j].T;
			}
			else
			{
				R_C[j].STime = R_C[j].T + R_C[j - 1].STime;
			}
			//**�Y���X�e�b�v�܂ł̍��v�����̌v�Z**//
			if (strcmp(Step[j].Motion, "�L�΂�") == 0 || strcmp(Step[j].Motion, "�^��") == 0 || strcmp(Step[j].Motion, "�g���ړ�����") == 0)//���삪"�L�΂�"�A"�^��"�A"�g���ړ�����"�̂Ƃ�
			{
				if (j == 1)
				{
					R_C[j].SKyori = 0.0;

					R_C[j].SKyori = Step[j].Dousa_Kyori;

					if (strcmp(Step[j].Hand, "����") == 0)
					{
						L_Kyori = Step[j].Dousa_Kyori;
					}
					else if (strcmp(Step[j].Hand, "�E��") == 0)
					{
						R_Kyori = Step[j].Dousa_Kyori;
					}
				}
				else
				{
					R_C[j].SKyori = R_C[j - 1].SKyori + Step[j].Dousa_Kyori;

					if (strcmp(Step[j].Hand, "����") == 0)
					{
						L_Kyori = L_Kyori + Step[j].Dousa_Kyori;
					}
					else if (strcmp(Step[j].Hand, "�E��") == 0)
					{
						R_Kyori = R_Kyori + Step[j].Dousa_Kyori;
					}
				}
			}
			else
			{
				R_C[j].SKyori = R_C[j - 1].SKyori;
			}



			fprintf(fp, "%.3lf,%.3lf,%d,%.3lf\n", R_C[j].SKyori, R_C[j].SKakudo, R_C[j].TurnNum, R_C[j].STime);

			if (strcmp(Step[j].Motion, "�L�΂�") == 0)
			{
				Reach_count++;
				fprintf(fp, "�L�΂� %s �� A ���� B ��\n", Step[j].Hand);

				fprintf(fp, "A,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].Motion, "�^��") == 0)
			{
				Bring_count++;
				fprintf(fp, "�^�� %s �� A ���� B ��\n", Step[j].Hand);

				fprintf(fp, "A,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].Motion, "�g���ړ�����") == 0)
			{
				fprintf(fp, "�g���ړ����� %s �� APP ���� OFFSET ��\n", Step[j].Hand);

				fprintf(fp, "APP,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "OFFSET,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].Motion, "��������") == 0)
			{
				Turn_count++;
				fprintf(fp, "�������� %d �� %s �� X ���� Y ��\n", Step[j].KoP, Step[j].Hand);

				fprintf(fp, "X,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "Y,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);

			}
			else if (strcmp(Step[j].Motion, "�͂�") == 0)
			{
				Grab_count++;
				fprintf(fp, "�͂� %d �� %s �� %d �ɂ��\n", Step[j].KoP, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].Motion, "�͂ݎ��") == 0)
			{
				Grab_count++;
				fprintf(fp, "�͂ݎ�� %d �� %s ���� %s �� %d �ɂ��\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].Motion, "����") == 0)
			{
				Release_count++;
				fprintf(fp, "���� %d �� %s �� %d �ɂ��\n", Step[j].KoP, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].Motion, "�u������") == 0)
			{
				Leave_count++;
				fprintf(fp, "�u������ %d �� %s �� %s �� %d �ɂ��\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].Motion, "�t����") == 0)
			{
				Attach_count++;
				fprintf(fp, "�t���� %d �� %s �� %s ��\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand);
			}
			else if (strcmp(Step[j].Motion, "�O��") == 0)
			{
				Remove_count++;
				fprintf(fp, "�O�� %d �� %s ���� %s ��\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand);
			}
			else if (strcmp(Step[j].Motion, "��[��ύX����") == 0)
			{
				fprintf(fp, "��[��ύX���� %s �̈ʒu�� %s ��\n", Step[j].Hand, Step[j].Sentan_Num);
			}
			else if (strcmp(Step[j].Motion, "�g���Ă�") == 0)
			{
				if (Step[j].flag == 1)//�H��̂Ƃ�
				{
					fprintf(fp, "�g���Ă� %s,%s �� %d �� %s �� %d �� %d,%d,%s �� %s,%d,%s �� %d �ɂ���\n", Step[j].Sup1, Step[j].Sup1_2, Step[j].Assy1, Step[j].Sup2, Step[j].Assy2, Step[j].Jig, Step[j].angle_num, Step[j].Hand, Step[j].Sup3, Step[j].Case, Step[j].Sup4, Step[j].Assy12);
				}
				else
				{
					fprintf(fp, "�g���Ă� %s �� %d �� %s �� %d �� ", Step[j].Sup1, Step[j].Assy1, Step[j].Sup2, Step[j].Assy2);

					if (strcmp(Step[j].Hand, "�E��Q����") != 0)
					{

						fprintf(fp, "%d,%s ", Step[j].Jig, Step[j].Hand);

						fprintf(fp, "�� %s,%s �� %d �ɂ���\n", Step[j].Sup3, Step[j].Sup4, Step[j].Assy12);
					}
					else
					{
						fprintf(fp, "%s �� %s,%s �� %d �ɂ���\n", Step[j].Hand, Step[j].Sup3, Step[j].Sup4, Step[j].Assy12);
					}
				}
			}
			else if (strcmp(Step[j].Motion, "���_��ύX����") == 0)
			{
				fprintf(fp, "���_��ύX���� %d �� ���_ �ɂ���\n", Step[j].KoP);

				fprintf(fp, "���_,%d,%d,%d\n", Step[j].Genten0X, Step[j].Genten0Y, Step[j].Genten0Z);
			}
			else if (strcmp(Step[j].Motion, "��������") == 0)
			{
				fprintf(fp, "�������� %d �� %d ��\n", Step[j].KoP, Step[j].Color);
			}
			else if (strcmp(Step[j].Motion, "�L������") == 0)
			{
				fprintf(fp, "�L������ %s �� %d �� %d ��\n", Step[j].Kensa_Cont, Step[j].Kensa_Table, Step[j].Pen);
			}
			else if (strcmp(Step[j].Motion, "����") == 0)
			{
				fprintf(fp, "���� %s ���� %s �� %d ���� %d �� ���� %s �� %s ������ ", Step[j].Place1, Step[j].Place2, Step[j].angle1, Step[j].angle2, Step[j].S_M_L, Step[j].F_B_S);
				//�ʉߓ_�����݂��邩�ǂ���
				if (Step[j].Tukaten[0] != '\0')
				{
					for (int m = 0; m <= n[j]; m++)
					{
						if (m == n[j])
						{
							fprintf(fp, "%d ��ʂ���\n", Step[j].Tukaten[m]);
						}
						else
						{
							fprintf(fp, "%d,", Step[j].Tukaten[m]);
						}
					}

				}
				else
				{
					fprintf(fp, "\n");
				}

				fprintf(fp, "%s,%4.2lf,%4.2lf,%4.2lf\n", Step[j].Place1, Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "%s,%4.2lf,%4.2lf,%4.2lf\n", Step[j].Place2, Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].Motion, "�K�i") == 0)
			{
				fprintf(fp, "�K�i �� %d �i %s ���� A ���s B �� %s ������\n", Step[j].Dansuu, Step[j].updown, Step[j].frontback);

				fprintf(fp, "A,%4.2lf,%4.2lf,%4.2lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%4.2lf,%4.2lf,%4.2lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].Motion, "���") == 0)
			{
				fprintf(fp, "��� %d ���� %d ��\n", Step[j].angle1, Step[j].angle2);
			}
			else if (strcmp(Step[j].Motion, "����") == 0)
			{
				fprintf(fp, "���� %d ��\n", Step[j].Zamen_height);
			}
			else if (strcmp(Step[j].Motion, "����") == 0)
			{
				fprintf(fp, "����\n");
			}
			else if (strcmp(Step[j].Motion, "��]����") == 0)
			{
				fprintf(fp, "��]���� %d �� X ���� Y ��\n", Step[j].KoP);

				fprintf(fp, "X,%d,%d,%d\n", Step[j].angle1_X, Step[j].angle2_X, Step[j].angle3_X);

				fprintf(fp, "Y,%d,%d,%d\n", Step[j].angle1_Y, Step[j].angle2_Y, Step[j].angle3_Y);

			}
			else if (strcmp(Step[j].Motion, "���ɖ߂�") == 0)
			{
				fprintf(fp, "���ɖ߂� %s ��\n", Step[j].sp_KoP);

				fprintf(fp, "%s,%d,%d,%d\n", Step[j].sp_KoP, Step[j].KoP1, Step[j].KoP2, Step[j].KoP3);
			}
			else if (strcmp(Step[j].Motion, "����") == 0)
			{
				fprintf(fp, "���� �Ώە� ��\n");

				fprintf(fp, "�Ώە�,%d,%d,%d,%d,%d\n", Step[j].KoP1, Step[j].KoP2, Step[j].KoP3, Step[j].KoP4, Step[j].KoP5);
			}
			else if (strcmp(Step[j].Motion, "��ױݸ��") == 0)
			{
				fprintf(fp, "��ױݸ�� %d ���� %4.1lf %s �ؑւ���\n", Step[j].angle_num, Step[j].Kyori, Step[j].Syunzi_Keika);
			}
			else if (strcmp(Step[j].Motion, "���߂���") == 0)
			{
				if ((strcmp(Step[j].Hand, "����") != 0) || ((strcmp(Step[j].Hand, "�E��") != 0)) || ((strcmp(Step[j].Hand, "����Q�E��") != 0)))
				{
					fprintf(fp, "���߂��� %d �� ���ߗ� %d ��\n", Step[j].Jig, Step[j].Tokaritsu);

				}
				else
				{
					fprintf(fp, "���߂��� %s �� ���ߗ� %d ��\n", Step[j].Hand, Step[j].Tokaritsu);

				}
			}
			else if (strcmp(Step[j].Motion, "ϰ��") == 0)
			{
				fprintf(fp, "ϰ�� %d �� �\������\n", Step[j].marker_num);
			}
			else if (strcmp(Step[j].Motion, "�~��") == 0)
			{
				fprintf(fp, "�~�� %d �� %s\n", Step[j].entyu_num, Step[j].Hyouzi_Kesu);
			}
		}
		else
		{
			if (strcmp(Step[j].M, "�L�΂�") == 0)
			{
				fprintf(fp, "�L�΂� %s �� A ���� B ��\n", Step[j].Hand);

				fprintf(fp, "A,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].M, "�^��") == 0)
			{
				fprintf(fp, "�^�� %s �� A ���� B ��\n", Step[j].Hand);

				fprintf(fp, "A,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].M, "�g���ړ�����") == 0)
			{
				fprintf(fp, "�g���ړ����� %s �� APP ���� OFFSET ��\n", Step[j].Hand);

				fprintf(fp, "APP,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "OFFSET,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].M, "��������") == 0)
			{
				fprintf(fp, "�������� %d �� %s �� X ���� Y ��\n", Step[j].KoP, Step[j].Hand);

				fprintf(fp, "X,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "Y,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);

			}
			else if (strcmp(Step[j].M, "�͂�") == 0)
			{
				fprintf(fp, "�͂� %d �� %s �� %d �ɂ��\n", Step[j].KoP, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].M, "�͂ݎ��") == 0)
			{
				fprintf(fp, "�͂ݎ�� %d �� %s ���� %s �� %d �ɂ��\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].M, "����") == 0)
			{
				fprintf(fp, "���� %d �� %s �� %d �ɂ��\n", Step[j].KoP, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].M, "�u������") == 0)
			{
				fprintf(fp, "�u������ %d �� %s �� %s �� %d �ɂ��\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].M, "�t����") == 0)
			{
				fprintf(fp, "�t���� %d �� %s �� %s ��\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand);
			}
			else if (strcmp(Step[j].M, "�O��") == 0)
			{
				fprintf(fp, "�O�� %d �� %s ���� %s ��\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand);
			}
			else if (strcmp(Step[j].M, "��[��ύX����") == 0)
			{
				fprintf(fp, "��[��ύX���� %s �̈ʒu�� %s ��\n", Step[j].Hand, Step[j].Sentan_Num);
			}
			else if (strcmp(Step[j].M, "�g���Ă�") == 0)
			{
				fprintf(fp, "�g���Ă� %s �� %d �� %s �� %d �� ", Step[j].Sup1, Step[j].Assy1, Step[j].Sup2, Step[j].Assy2);

				if (strcmp(Step[j].Hand, "�E��Q����") != 0)
				{

					fprintf(fp, "%d,%s ", Step[j].Jig, Step[j].Hand);

					fprintf(fp, "�� %s,%s �� %d �ɂ���\n", Step[j].Sup3, Step[j].Sup4, Step[j].Assy12);
				}
				else
				{
					fprintf(fp, "%s �� %s,%s �� %d �ɂ���", Step[j].Hand, Step[j].Sup3, Step[j].Sup4, Step[j].Assy12);
				}
			}
			else if (strcmp(Step[j].M, "���_��ύX����") == 0)
			{
				fprintf(fp, "���_��ύX���� %d �� ���_ �ɂ���\n", Step[j].KoP);

				fprintf(fp, "���_,%d,%d,%d\n", Step[j].Genten0X, Step[j].Genten0Y, Step[j].Genten0Z);
			}
			else if (strcmp(Step[j].M, "��������") == 0)
			{
				fprintf(fp, "�������� %d �� %d ��\n", Step[j].KoP, Step[j].Color);
			}
			else if (strcmp(Step[j].M, "�L������") == 0)
			{
				fprintf(fp, "�L������ %s �� %d �� %d ��\n", Step[j].Kensa_Cont, Step[j].Kensa_Table, Step[j].Pen);
			}
			else if (strcmp(Step[j].M, "�K�i") == 0)
			{
				fprintf(fp, "�K�i �� %d �i %s ���� A ���s B �� %s ������\n", Step[j].Dansuu, Step[j].updown, Step[j].frontback);

				fprintf(fp, "A,%4.2lf,%4.2lf,%4.2lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%4.2lf,%4.2lf,%4.2lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].M, "���") == 0)
			{
				fprintf(fp, "��� %d ���� %d ��\n", Step[j].angle1, Step[j].angle2);
			}
			else if (strcmp(Step[j].M, "����") == 0)
			{
				fprintf(fp, "���� %d ��\n", Step[j].Zamen_height);
			}
			else if (strcmp(Step[j].M, "����") == 0)
			{
				fprintf(fp, "����\n");
			}
			else if (strcmp(Step[j].M, "��]����") == 0)
			{
				fprintf(fp, "��]���� %d �� X ���� Y ��\n", Step[j].KoP);

				fprintf(fp, "X,%d,%d,%d\n", Step[j].angle1_X, Step[j].angle2_X, Step[j].angle3_X);

				fprintf(fp, "Y,%d,%d,%d\n", Step[j].angle1_Y, Step[j].angle2_Y, Step[j].angle3_Y);

			}
			else if (strcmp(Step[j].M, "���ɖ߂�") == 0)
			{
				fprintf(fp, "���ɖ߂� %s ��\n", Step[j].sp_KoP);

				fprintf(fp, "%s,%d,%d,%d\n", Step[j].sp_KoP, Step[j].KoP1, Step[j].KoP2, Step[j].KoP3);
			}
			else if (strcmp(Step[j].M, "����") == 0)
			{
				fprintf(fp, "���� �Ώە� ��\n");

				fprintf(fp, "�Ώە�,%d,%d,%d,%d,%d\n", Step[j].KoP1, Step[j].KoP2, Step[j].KoP3, Step[j].KoP4, Step[j].KoP5);
			}
			else if (strcmp(Step[j].M, "��ױݸ��") == 0)
			{
				fprintf(fp, "��ױݸ�� %d ���� %4.1lf %s �ؑւ���\n", Step[j].angle_num, Step[j].Kyori, Step[j].Syunzi_Keika);
			}
			else if (strcmp(Step[j].M, "���߂���") == 0)
			{
				if ((strcmp(Step[j].Hand, "����") != 0) || ((strcmp(Step[j].Hand, "�E��") != 0)) || ((strcmp(Step[j].Hand, "����Q�E��") != 0)))
				{
					fprintf(fp, "���߂��� %d �� ���ߗ� %d ��\n", Step[j].Jig, Step[j].Tokaritsu);
				}
				else
				{
					fprintf(fp, "���߂��� %s �� ���ߗ� %d ��\n", Step[j].Hand, Step[j].Tokaritsu);

				}
			}
			else if (strcmp(Step[j].M, "ϰ��") == 0)
			{
				fprintf(fp, "ϰ�� %d �� �\������\n", Step[j].marker_num);
			}
			else if (strcmp(Step[j].M, "�~��") == 0)
			{
				fprintf(fp, "�~�� %d �� %s\n", Step[j].entyu_num, Step[j].Hyouzi_Kesu);
			}

			R_C[j].STime = R_C[j - 1].STime;
		}

	}

	fclose(fp);


}

void Write_TimeHyouka(int STEP, FILE* fp2)
{

	c++;

	if (c == 1)
	{
		fprintf(fp2, "�����ԍ�,�X�e�b�v��,���Ԓl,�L�΂�,�^��,�͂ݎ��/�͂�,����,�u������,�t����,�O��,��������,�L�΂�,�^��,�͂ݎ��/�͂�,����,�u������,�t����,�O��,��������,�ړ�����,����ړ�����,�E��ړ�����,����]�p�x");
		for (int i = 1; i <= R_H.J; i++)
		{
			if (i == R_H.J)
			{
				fprintf(fp2, ",%d\n", i);
			}
			else
			{
				fprintf(fp2, ",%d", i);
			}
		}
	}

	fprintf(fp2, "%d,%d,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%d,%d,%d,%d,%d,%d,%d,%d,%4.2lf,%4.2lf,%4.2lf,%.1lf", c, R_H.H, R_C[STEP - 1].STime, STime_Reach, STime_Bring, STime_Grab, STime_Release, STime_Leave, STime_Attach, STime_Remove, STime_Turn, Reach_count, Bring_count, Grab_count, Release_count, Leave_count, Attach_count, Remove_count, Turn_count, R_C[STEP - 1].SKyori, L_Kyori, R_Kyori, All_Angle);//������,���Ԓl

	for (int j = 1; j <= R_H.J; j++)
	{
		if (j == countJ)//�p���鎡��ƈ�v������
		{
			fprintf(fp2, ",%4.2lf", R_C[STEP].STime);
		}
		else
		{
			fprintf(fp2, ",");
		}
		if (j == R_H.J)
		{
			fprintf(fp2, "\n");
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

//int�^�̕ϐ���char�^�̕�����ɕϊ�����֐�
char* Conv_Char(int no) {
	char buf[10];
	sprintf(buf, "%d", no);
	return buf;
}

