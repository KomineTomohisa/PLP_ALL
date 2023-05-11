#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<conio.h>



//定数の宣言 --------------------------------------------------
#define SysVer "Ver.1.10 2021/01/26"
#define PW "plp0831"
#define PWNum 7
#define M_PI 3.14159;
#define DEF_PAR_FNAME "レイアウト変更VFDL導出システムパラメータ.txt"					//使用するパラメータファイルの名称
#define STR_MAX 500														//文字の最大の部品数を500とする
#define FGETS_MAX_LENGTH 20000											//文字の最大文字数を20000とする
#define MaxMM 50000														//取り扱う最大の組付けや動作の数を50000とする
#define Max_VFDL 200000		
#define MaxKN 50000 
#define MaxKM 200
#define MaxExcel 1048576;												//Excelの最大行数

//パスワード関係
#define PWNum 8
#define SFTN 3	 //文字をシフトする数

//グローバル変数の宣言 --------------------------------------------------

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

char NowPath[STR_MAX];								//最終的にパラメータファイルのフルパスを入れる
char SEIHIN_Name[STR_MAX];							//ハンドリング評価シート内の製品名を入れる


//int,double--------------------------------------------------------------

int s_File;		//ファイルの最大番号
int D_model;    //導出形式
int N;				//全部品の数
int T;				//全中間製品の数
int A;				//接触線の数
int K;				//組付けの数
//int VFDL_Count;		//VFDLの数
int G;				//初期位置の行数
int STEPNUM = 0;	//ステップ数
int Jignum = 0;	    //治具数
int Beforejignum;//S0において治具の詳細に入る前の数
int Beforetraynum;//S0においてトレーの詳細に入る前の数
int Beforefinishboxnum;//S0において完成品箱の詳細に入る前の数
int Beforekougunum;//S0において工具の詳細に入る前の数
int c = 0;
int countJ = 0;//順序ごとの治具数のカウント用
int countF = 0;//前列に配置する部品数をカウント
int Reach_count = 0;//"伸ばす"カウント用
int Bring_count = 0;//"運ぶ"カウント用
int Turn_count = 0;//"持ち直す"カウント用
int Grab_count = 0;//"掴み取る","掴む"カウント用
int Release_count = 0;//"放す"カウント用
int Leave_count = 0;//"置き放す"カウント用
int Attach_count = 0;//"付ける"カウント用
int Remove_count = 0;//"外す"カウント用

double STime_Reach;//"伸ばす"時間値総合評価用
double STime_Bring;//"運ぶ"時間値総合評価用
double STime_Turn;//"持ち直す"時間値総合評価用
double STime_Grab;//"掴み取る","掴む"時間値総合評価用
double STime_Release;//"放す"時間値総合評価用
double STime_Leave;//"置き放す"時間値総合評価用
double STime_Attach;//"付ける"時間値総合評価用
double STime_Remove;//"外す"時間値総合評価用

double All_Angle = 0;//総回転角度

double R_Kyori;//右手移動距離
double L_Kyori;//左手移動距離




//プロトタイプ宣言 --------------------------------------------------

//必ず必要なもの

void GetNowPath(char* NP, char* S);		//現在のファイルを特定する関数
void ShowFinMsg(char*);					//文字を出力？する関数
void ReadPara(char* S);					//使用するパラメータファイルを読み込む関数
void Del_Com(char*);
char* Conv_Char(int);//int型の変数をchar型の文字列に変換する関数


//***自作の関数***//

//Input情報読み込み用
void Read_VFDL();						//VFDLを読み込む関数

void Read_Buhin_Zokusei();				//部品属性を読み込む関数

void Read_Tyukan();						//中間製品属性を読み込む関数

void Read_Junjo(int);						//組立順序を読み込む関数

//Output情報読み込み用
void Write_VFDL(int, int*, int);             //VFDLを書き出す関数

void Write_TimeHyouka(int, FILE*);				//時間値評価を書き出す関数


//時間値計算用
void Time_Reach(int);						//「伸ばす」の時間値導出関数

void Time_Bring(int);						//「運ぶ」の時間値導出関数

void Time_grab(int);						//「掴み取る」の時間値導出関数

void Time_Assemble(int);					//「組立てる」の時間値導出関数

void Time_Turn(int);						//「持ち直す」の時間値導出関数


//レイアウト用関数
void BoxJig_Hyouka();						//部品箱・治具評価用関数

void S0_Change(int);						//S0変更用関数

void C_Change(int);							//変化内のレイアウト変更用関数

void Kyori_Change(int);						//動作距離の変更用関数

//並べ替え用
void swap(int);

//組付け位置変更関数
void Kumituke_Position(int);				//組付け位置を格納する関数

//パスワード//
int PWCheck(int, char*);
void PWG(char*);

//***構造体***//

//ヘッダに関する構造体
struct H
{
	int W;		//作業者番号
	int L;		//ライン番号
	int P;		//工程番号
	int H;		//ステップ数
	int Ht;		//有効ステップ数
	int V;		//速度指定
	int C;		//干渉チェック
	int S;		//状態番号
	int PA;		//部品数
	int T;		//工具数
	int J;		//治具数
	int MI;		//中間部品数
	int MO;		//生成中間部品数
	int MO1;
	int MO2;
	int TA;		//机の数
	int TR;		//トレイの数
	int WD;		//通過点数
	int RA;		//棚の数
};
struct H R_H;

//初期位置に関する構造体
struct S0
{
	//int型
	int N;//識別番号
	int C;//色番号
	int S;//初期状態
	int X;
	int Y;
	//double型
	double lx;//位置x
	double ly;//位置y
	double lz;//位置z
	double Hx;//方向x
	double Hy;//方向y
	double Hz;//方向z
	double Bx;//ブロック形状のサイズx
	double By;//ブロック形状のサイズy
	double Bz;//ブロック形状のサイズz

};
struct S0 R_S0[100];

//変化に関する構造体
struct C
{
	//int型
	int SEQ;	//変化の連番
	int NF;		//この後に続く命令の数
	int HL;		//ハイライト表示させる部品番号
	int Flag;	//本来のステップは1,VFDL作成に伴い便宜上生じたステップは0
	int AC;		//注記の色
	int AS;		//注記の文字
	int TurnNum;
	//double型
	double T;		//変化の時間
	double SKyori;
	double SKakudo;
	double STime;

};
struct C R_C[1000];

//VFDLに関する構造体
struct VFDL_Info              //VFDLの情報を格納する構造体
{
	char M[STR_MAX];
	char Motion[STR_MAX];	//動作
	char Hand[STR_MAX];				//使用している手（左手か右手か）
	char Place1[STR_MAX];   //場所1
	char Place2[STR_MAX];   //場所2
	double BeforeX;//動作前の状態X
	double BeforeY;//動作前の状態Y
	double BeforeZ;//動作前の状態Z
	double AfterX;//動作後の状態X
	double AfterY;//動作後の状態Y
	double AfterZ;//動作後の状態Z
	int OyaP;//親部品
	int KoP;//子部品
	int HandShape;			//手の形
	char BeforeJig[STR_MAX];		//支え具
	char Sentan_Num[STR_MAX];		//先端番
	int Genten0X;			//原点のx座標
	int Genten0Y;			//原点のy座標
	int Genten0Z;			//原点のz座標
	int Color;				//色番号
	char Kensa_Cont[STR_MAX];		//検査内容
	int Kensa_Table;		//検査表
	int Pen;				//ペン番号
	char Sup1[STR_MAX];				//支え1
	char Sup1_2[STR_MAX];				//支え1
	char Sup2[STR_MAX];				//支え2
	char Sup3[STR_MAX];				//支え3
	char Sup4[STR_MAX];				//支え4
	int Assy1;				//組立品1
	int Assy2;				//組立品2
	int Assy12;				//組立品12
	int Jig;				//治工具
	int Dansuu;				//段数
	char updown[STR_MAX];			//上るか下るか
	char frontback[STR_MAX];			//前か後か
	int angle1;				//回転前の角度
	int angle2;				//回転後の角度
	int Zamen_height;		//座面の高さ
	int angle1_X;			//「回転する」の回転前の情報1
	int angle2_X;			//「回転する」の回転前の情報2
	int angle3_X;			//「回転する」の回転前の情報3
	int angle1_Y;			//「回転する」の回転後の情報1
	int angle2_Y;			//「回転する」の回転後の情報2
	int angle3_Y;			//「回転する」の回転後の情報3
	char sp_KoP[STR_MAX];			//文字列の対象物
	int KoP1;				//対象物の情報1
	int KoP2;				//対象物の情報2
	int KoP3;				//対象物の情報3
	int KoP4;
	int KoP5;
	int angle_num;			//アングル番号
	double Kyori;				//「カメラアングル」における距離情報
	char Syunzi_Keika[STR_MAX];		//瞬時または経過(カメラアングル)
	int Tokaritsu;			//透過率
	int marker_num;			//マーカー番号
	int entyu_num;			//円柱番号
	char Hyouzi_Kesu[STR_MAX];		//表示または消す(円柱)
	double Dousa_Kyori;
	//歩行用
	char S_M_L[STR_MAX];//歩幅の大・中・小
	char F_B_S[STR_MAX];//歩く向き
	int Case;
	char Case2[STR_MAX];
	int Tukaten[10];
	int flag;
	double X_Dif;//X座標の差
	double Y_Dif;//Y座標の差
	double Z_Dif;//Z座標の差
	double OFFSET;//APP→OFFSETの距離
};
struct VFDL_Info Step[FGETS_MAX_LENGTH];

//部品箱・治具に関する構造体
struct Box_Jig
{
	int Box_hyouka;//部品箱の評価(0:やりづらさ要因の"小さい",2:やりづらさ要因の"大きい",1:それ以外)
	int Box_hand;//部品をどちらの手で扱うか(0:左手,1:右手)
	int Number;//部品が出現する順番を示す
	double Box_x;//部品箱のxサイズ
	double Box_y;//部品箱のyサイズ
	double Box_z;//部品箱のzサイズ
	double Jig_x;//治具のxサイズ
	double Jig_y;//治具のyサイズ
	double Jig_z;//治具のzサイズ
	double Volume;//部品の体積
	int num;//部品番号
};
struct Box_Jig BJ[STR_MAX];

struct KJUNJO {
	long long kumij;			//組立順序番号
	int kuminum[MaxKN];	//組付け番号
	int p1[MaxKN];	//部品1
	int p2[MaxKN];	//部品2
	int p3[MaxKN];	//組立て部品
	char Kumituke_Pos[MaxKN][3];//組付け方向
	char Oya_Pos1[MaxKN][3];//親部品の姿勢（X方向）
	char Oya_Pos2[MaxKN][3];//親部品の姿勢（Z方向）
	int Jig[MaxKN];
	int KouguNum[MaxKN];
	int DousaNum[MaxKN];

	int Dir[MaxKN];//治具に付く高さ用の方向(1:X,2:Y,3:Z)
};
struct KJUNJO R_KJUNJO;

//レイアウトに関する構造体
struct Rayout
{
	double Box_x[STR_MAX];//部品箱のxサイズ
	double Box_y[STR_MAX];//部品箱のyサイズ
	double Box_z[STR_MAX];//部品箱のzサイズ
	double Box_xCo[STR_MAX];//治具のx座標
	double Box_yCo[STR_MAX];//治具のy座標
	double Box_zCo[STR_MAX];//治具のz座標
	double Jig_x[STR_MAX];//治具のxサイズ
	double Jig_y[STR_MAX];//治具のyサイズ
	double Jig_z[STR_MAX];//治具のzサイズ
	double Jig_xCo[STR_MAX];//治具のx座標
	double Jig_yCo[STR_MAX];//治具のy座標
	double Jig_zCo[STR_MAX];//治具のz座標
	double Parts_xCo[STR_MAX];//部品のx座標
	double Parts_yCo[STR_MAX];//部品のy座標
	double Parts_zCo[STR_MAX];//部品のy座標
	double finish_centerX;//完成品箱の中心点のX座標
	double finish_centerY;//完成品箱の中心点のY座標
	double finish_centerZ;//完成品箱の中心点のZ座標
	double karioki_centerX;//仮置き場の中心点のX座標
	double karioki_centerY;//仮置き場の中心点のY座標
	double karioki_centerZ;//仮置き場の中心点のZ座標
	int num[STR_MAX];
	int Box_inclination[STR_MAX];//部品箱の傾き
};
struct Rayout Ray;
//部品属性に関する構造体
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
	int Box_type;//部品箱の種類
	int Buhinsu;//部品箱に入れる部品の数
};
struct Buhin R_B[100];

struct Tyukan
{
	int Tyunum;//中間製品番号
	double lx;//長さX
	double ly;//長さY
	double lz;//長さZ
	double Xkotei;//長さX固定（並べ替え前）
	double Ykotei;//長さY固定（並べ替え前）
	double Zkotei;//長さZ固定（並べ替え前）
};
struct Tyukan R_T[STR_MAX];

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// プログラム部分
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void main(int argc, char* argv[])
{
	printf("argv[0]:%s\n", argv[0]);		//.exeを出力
	printf("NowPath:%s\n", NowPath);		//NowPathを出力
	GetNowPath(NowPath, argv[0]);			//NowPathを特定する

	strcat(NowPath, "Input\\");				//パラメータファイルの置き場は.exeがあるフォルダ下のInputとする
	strcat(NowPath, DEF_PAR_FNAME);			//パラメータファイルを探す
	printf("\n");							//改行
	printf("NowPath:%s\n", NowPath);		//NowPathを出力
	printf("\n");							//改行

	/************************************************************************************************/
	/*パスワードの認証*/
	/************************************************************************************************/
	//if (PWCheck(argc, argv[1]) == 0) return 0; //パスワードが一致しなかったら0が返ってくる → プログラムの終了 (= return 0)


	ReadPara(NowPath);						//パラメータファイルを読み込む

}


void ReadPara(char* NowPath)
{
	FILE* fp1;//ファイルポインタの宣言

	/************************************************************************************************/
	/*パラメータファイルを開く*/
	/************************************************************************************************/
	// パラメータファイルの仕様
	//  1行目：製品名
	//	2行目：VFDLファイルの導出形式（１：網羅、２：部品群、３：分解）
	//	3行目：VFDLファイルの最大番号
	//  4行目：INPUTするフォルダのパス(VFDL用)
	//  5行目：INPUTするフォルダのパス(Data用)
	//  6行目：OUTPUTするフォルダのパス(VFDL用)
	//	7行目：OUTPUTするフォルダのパス(時間値総合評価用)

	fp1 = fopen(NowPath, "r");
	if (fp1 == NULL) ShowFinMsg("パラメータファイルがありません"); // ファイルを開けなければ終了
	if (fscanf(fp1, "%s", SEIHIN_Name) == EOF) ShowFinMsg("製品名が不正です");
	if (fscanf(fp1, "%d", &D_model) == EOF) ShowFinMsg("導出形式を入力してください");
	if (fscanf(fp1, "%d", &s_File) == EOF) ShowFinMsg("VFDLファイルの最大番号を入力してください");
	if (fscanf(fp1, "%s", inPath_Input1) == EOF) ShowFinMsg("入力先フォルダのパス(VFDL)が不正です");
	if (fscanf(fp1, "%s", inPath_Input2) == EOF) ShowFinMsg("入力先フォルダのパス(Data)が不正です");
	if (fscanf(fp1, "%s", outDir1) == EOF) ShowFinMsg("出力先フォルダのパス①が不正です");
	if (fscanf(fp1, "%s", outDir2) == EOF) ShowFinMsg("出力先フォルダのパス②が不正です");
	fclose(fp1);

	//"部品属性"読み込み用パス作成
	strcpy(inPath_Input_Buhin_Zokusei, inPath_Input2);
	strcat(inPath_Input_Buhin_Zokusei, "\\");
	strcat(inPath_Input_Buhin_Zokusei, SEIHIN_Name);
	strcat(inPath_Input_Buhin_Zokusei, "_部品属性.csv");
	
	//"中間製品属性"読み込み用パス作成
	strcpy(inPath_Input_Tyukan, inPath_Input2);
	strcat(inPath_Input_Tyukan, "\\");
	strcat(inPath_Input_Tyukan, SEIHIN_Name);

	//VFDL読み込み用パス作成
	strcpy(inPath_Input_VFDL, inPath_Input1);
	strcat(inPath_Input_VFDL, "\\");
	strcat(inPath_Input_VFDL, SEIHIN_Name);

	//組立順序読み込み用パス作成
	strcpy(inPath_Input_Junjo, inPath_Input2);
	strcat(inPath_Input_Junjo, "\\");
	strcat(inPath_Input_Junjo, SEIHIN_Name);

	//Output用パス(VFDL用)作成
	strcpy(OutPath, outDir1);
	strcat(OutPath, "\\");
	strcat(OutPath, SEIHIN_Name);


	if (D_model == 1)//網羅
	{
		strcat(inPath_Input_Tyukan, "_網羅_中間製品属性.csv");
		strcat(inPath_Input_VFDL, "_網羅_VFDL");
		strcat(inPath_Input_Junjo, "_網羅_組立順序");
		strcat(OutPath, "網羅_レイアウト変更VFDL");
	}
	else if (D_model == 2)//部品群
	{
		strcat(inPath_Input_Tyukan, "_部品群_中間製品属性.csv");
		strcat(inPath_Input_VFDL, "_部品群_VFDL");
		strcat(inPath_Input_Junjo, "_部品群_組立順序");
		strcat(OutPath, "部品群_レイアウト変更VFDL");
	}
	else//分解
	{
		strcat(inPath_Input_Tyukan, "_分解_中間製品属性.csv");
		strcat(inPath_Input_VFDL, "_分解_VFDL");
		strcat(inPath_Input_Junjo, "_分解_組立順序");
		strcat(OutPath, "分解_レイアウト変更VFDL");
	}

	//Output用パス(総合評価ファイル用)作成
	strcpy(outPath_TimeHyouka, outDir2);
	strcat(outPath_TimeHyouka, "\\");
	strcat(outPath_TimeHyouka, SEIHIN_Name);
	strcat(outPath_TimeHyouka, "_時間値総合評価.csv");

	printf("製品名：%s\n", SEIHIN_Name);
	printf("VFDLファイルの最大番号：%d\n", s_File);
	printf("VFDLファイルの導出形式（1：網羅,2：部品群,3：分解）：%d\n", D_model);
	printf("Input(VFDL)情報：%s\n", inPath_Input1);
	printf("Input(Data)情報：%s\n", inPath_Input2);
	printf("Input(部品属性)情報：%s\n", inPath_Input_Buhin_Zokusei);
	printf("Output(VFDL)情報：%s\n\n", OutPath);
	printf("Output(総合評価)情報：%s\n\n", OutPath);

	//部品属性の読み込み
	Read_Buhin_Zokusei();
	printf("部品属性読み込み完了\n");

	//中間製品属性の読み込み
	Read_Tyukan();
	printf("中間製品属性読み込み完了\n");

	//動作付組立順序の読み込み
	//Read_Junjo();
	//printf("動作付組立順序読み込み完了\n");

	//VFDLの読み込み
	printf("評価を作成中です\n");
	Read_VFDL();
	printf("\n評価の作成が完了しました。\n");

	//終了の処理
	printf("(Enterキーを押してください)\n");
	unsigned char key;
	while (1) {
		key = getch();
		if (key == 0x0d) break;
	}
}

void Read_VFDL()           //VFDLを読み込む関数
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

	for (int vfdlnum = 1; vfdlnum <= s_File; vfdlnum++)//パラメータで指定されたVFDLの数だけ回す
	{
		//初期化
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


		//入力先のフルパスをInPathに代入する
		/*strcpy(InPath, inPath_Input_VFDL);
		strcat(InPath, Conv_Char(i));
		strcat(InPath, "_価値作業");
		strcat(InPath, ".csv");*/

		fp1 = fopen(InPath, "r");						//NowPathのファイルを開いて、読み込む
		if (fp1 == NULL)ShowFinMsg("VFDLがありません");		//ファイルを開けなければ終了

		printf("現在開いているVFDL %d\r", vfdlnum);

		//以降は、開けたファイルがVFDLである前提で処理をする

		//1行目の読み込み
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		s = temp;

		sscanf(s, "%d%d%d", &R_H.W, &R_H.L, &R_H.P);

		//2行目の読み込み
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		s = temp;

		sscanf(s, "%d%d%d%d%d", &R_H.H, &R_H.Ht, &R_H.V, &R_H.C, &R_H.S);	//ステップ数,有効ステップ数,速度指定,干渉チェック,状態番号

		//3行目の読み込み
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		s = temp;

		sscanf(s, "%d%d%d", &R_H.PA, &R_H.T, &R_H.J);

		//4行目の読み込み
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		s = temp;

		sscanf(s, "%d", &R_H.MI);

		//5行目の読み込み
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		s = temp;

		sscanf(s, "%d%d%d", &R_H.MO, &R_H.MO1, &R_H.MO2);

		//6行目の読み込み
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		s = temp;

		sscanf(s, "%d%d%d", &R_H.TA, &R_H.TR, &R_H.WD);

		//7行目の読み込み
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		G = 2 + R_H.PA + R_H.T + R_H.J + R_H.MI + R_H.TA + R_H.TR + R_H.WD + R_H.RA;

		//8行目からFから1行前まで
		for (i = 1; i <= G; i++)
		{

			fgets(temp, FGETS_MAX_LENGTH, fp1);

			Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
			s = temp;

			sscanf(s, "%d", &R_S0[i].N);
			s = strchr(&s[1], ' ');		//空白の文字を探して、そこを文字列の開始点にする

			//簡易治具の情報
			if (R_S0[i].N >= -299 && R_S0[i].N < -100)
			{
				sscanf(s, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%d", &R_S0[i].lx, &R_S0[i].ly, &R_S0[i].lz, &R_S0[i].Hx, &R_S0[i].Hy, &R_S0[i].Hz, &R_S0[i].Bx, &R_S0[i].By, &R_S0[i].Bz, &R_S0[i].C, &R_S0[i].S);
			}
			//机の情報
			else if (R_S0[i].N >= 8000 && R_S0[i].N < 9000)
			{
				sscanf(s, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%d", &R_S0[i].lx, &R_S0[i].ly, &R_S0[i].lz, &R_S0[i].Hx, &R_S0[i].Hy, &R_S0[i].Hz, &R_S0[i].Bx, &R_S0[i].By, &R_S0[i].Bz, &R_S0[i].C);
			}
			//トレイの情報
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

		//Fから1行前の読み込み

		fgets(temp, FGETS_MAX_LENGTH, fp1);

		while (fgets(temp, FGETS_MAX_LENGTH, fp1) != NULL)	//残りの全ての行を読み込む
		{
			Del_Com(temp);
			s = temp;
			sscanf(s, "%s", &Step[STEPNUM].M);		//文字情報を読み込む

			s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする


			if (strcmp(Step[STEPNUM].M, "F") == 0)	//文字列の先頭がFのとき
			{

				sscanf(s, "%d%d%d%d%lf%d", &R_C[STEPNUM].SEQ, &R_C[STEPNUM].NF, &R_C[STEPNUM].HL, &R_C[STEPNUM].Flag, &R_C[STEPNUM].T, &R_C[STEPNUM].AC);	//変化の1行目を読み込む

				fgets(temp, FGETS_MAX_LENGTH, fp1);

				Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
				s = temp;

				sscanf(s, "%lf%lf%d%lf", &R_C[STEPNUM].SKyori, &R_C[STEPNUM].SKakudo, &R_C[STEPNUM].TurnNum, &R_C[STEPNUM].STime);//変化の2行目を読み込む

				fgets(temp, FGETS_MAX_LENGTH, fp1);

				Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
				s = temp;

				sscanf(s, "%s", &Step[STEPNUM].Motion);	//動作を読み込む

				if (strcmp(Step[STEPNUM].Motion, "伸ばす") == 0)	//動作が'伸ばす'のとき
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//Aに関する情報を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//Bに関する情報を読み込む

				}
				else if (strcmp(Step[STEPNUM].Motion, "運ぶ") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//Aに関する情報を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//Bに関する情報を読み込む


				}
				else if (strcmp(Step[STEPNUM].Motion, "組立移動する") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%s%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//Aに関する情報を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%s%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//Bに関する情報を読み込む

				}
				else if (strcmp(Step[STEPNUM].Motion, "持ち直す") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//対象物を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//Xに関する情報を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//Yに関する情報を読み込む

					//printf("%d%d%d%d%d%d\n", Step[STEPNUM].RTurn0X, Step[STEPNUM].RTurn0Y, Step[STEPNUM].RTurn0Z, Step[STEPNUM].RTurn1X, Step[STEPNUM].RTurn1Y, Step[STEPNUM].RTurn1Z);


				}
				else if (strcmp(Step[STEPNUM].Motion, "掴む") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//対象物を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'で'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].HandShape);		//手の形を読み込む

					R_C[STEPNUM].T = 2.0 * 0.036;			//治具上の部品を掴む=ケース1A

					STime_Grab += R_C[STEPNUM].T;//合計時間用


				}
				else if (strcmp(Step[STEPNUM].Motion, "掴み取る") == 0)
				{

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//対象物を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].BeforeJig);		//支え具の番号を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);		    //'から'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].HandShape);		//手の形を読み込む


				}
				else if (strcmp(Step[STEPNUM].Motion, "放す") == 0)
				{

					R_C[STEPNUM].T = 0.072;

					STime_Release += R_C[STEPNUM].T;//合計時間用

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//対象物を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'で'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].HandShape);		//手の形を読み込む


				}
				else if (strcmp(Step[STEPNUM].Motion, "置き放す") == 0)
				{

					R_C[STEPNUM].T = 0.2736;

					STime_Leave += R_C[STEPNUM].T;//合計時間用

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//対象物を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].BeforeJig);		//支え具の番号を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'に'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'で'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].HandShape);		//手の形を読み込む


				}
				else if (strcmp(Step[STEPNUM].Motion, "付ける") == 0)
				{
					//網羅用
					R_C[STEPNUM].T = 0.2016;

					STime_Attach += R_C[STEPNUM].T;//合計時間用

					//printf("付ける%dの時間値を入力してください:", j);			//その動作番号の「付ける」の時間値入力を要求する

					//sscanf("%lf", &R_C[j].T);

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//対象物を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].BeforeJig);		//支え具の番号を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'に'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

					Jignum++;//治具数をカウント

				}
				else if (strcmp(Step[STEPNUM].Motion, "外す") == 0)
				{
					//printf("外す%dの時間値を入力してください:", j);			//その動作番号の「外す」の時間値入力を要求する
					//sscanf("%lf", &R_C[j].T);
					//網羅用
					R_C[STEPNUM].T = 0.144;

					STime_Remove += R_C[STEPNUM].T;//合計時間用

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//対象物を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].BeforeJig);		//支え具の番号を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);		    //'から'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

				}
				else if (strcmp(Step[STEPNUM].Motion, "先端を変更する") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);		    //'の位置を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Sentan_Num);		//先端番を読み込む

				}
				else if (strcmp(Step[STEPNUM].Motion, "組立てる") == 0)
				{
					Step[STEPNUM].flag = 0;
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Sup1);		//支え1を読み込む

					if (strcmp(Step[STEPNUM].Sup1, "左手") == 0 || strcmp(Step[STEPNUM].Sup1, "右手") == 0 || strcmp(Step[STEPNUM].Sup1, "右手＿左手") == 0)//工具用
					{
						Step[STEPNUM].flag = 1;
						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%s", &Step[STEPNUM].Sup1_2);		//支え1を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'の'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%d", &Step[STEPNUM].Assy1);		//組立品1を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'と'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%s", &Step[STEPNUM].Sup2);		//支え2を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'の'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%d", &Step[STEPNUM].Assy2);		//組立品2を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%d", &Step[STEPNUM].Jig);		//使用している冶具の情報を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%d", &Step[STEPNUM].angle_num);		//使用している工具の情報を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'で'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%s", &Step[STEPNUM].Sup3);		//支え3を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%d", &Step[STEPNUM].Case);		//使用している冶具の情報を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%s", &Step[STEPNUM].Sup4);		//支え4を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'の'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%d", &Step[STEPNUM].Assy12);		//組立品12を読み込む
					}
					else
					{
						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'の'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%d", &Step[STEPNUM].Assy1);		//組立品1を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'と'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%s", &Step[STEPNUM].Sup2);		//支え2を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'の'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%d", &Step[STEPNUM].Assy2);		//組立品2を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

						if (strcmp(Step[STEPNUM].Hand, "右手＿左手") != 0)    //手の情報が右手＿左手ではないとき
						{
							Step[STEPNUM].Jig = atoi(Step[STEPNUM].Hand);

							s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
							sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む
						}

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'で'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%s", &Step[STEPNUM].Sup3);		//支え3を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%s", &Step[STEPNUM].Sup4);		//支え4を読み込む

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%c", trash);		    //'の'を読み込んで捨てる

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%d", &Step[STEPNUM].Assy12);		//組立品12を読み込む
					}

				}
				else if (strcmp(Step[STEPNUM].Motion, "原点を変更する") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//対象物を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%s%d%d%d", trash, &Step[STEPNUM].Genten0X, &Step[STEPNUM].Genten0Y, &Step[STEPNUM].Genten0Z);//原点の座標情報を読み込む

				}
				else if (strcmp(Step[STEPNUM].Motion, "検査する") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//対象物を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].Color);		//色番号を読み込む

				}
				else if (strcmp(Step[STEPNUM].Motion, "記入する") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Kensa_Cont);		//検査内容を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].Kensa_Table);		//検査表を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'に'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].Pen);		//ペン番号を読み込む	

				}
				else if (strcmp(Step[STEPNUM].Motion, "歩く") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Place1);		    //場所1を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);				//'から'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Place2);		    //場所2を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);				//'へ'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].angle1);		    //角度1を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);				//'から'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].angle2);		    //角度2を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);				//'へ'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);				//'歩幅'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].S_M_L);		    //歩幅の大きさを読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);				//'で'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].F_B_S);		    //歩行の向きを読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);				//'向きに'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);				//これがないと上手くいかない

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].Case);

					//通過点があるかどうかの場合分け
					if (Step[STEPNUM].Case != '\0')
					{
						Step[STEPNUM].Tukaten[n[STEPNUM]] = Step[STEPNUM].Case;

						s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
						sscanf(s, "%s", &Step[STEPNUM].Case2);

						//通過点が2つ以上あるかどうかの場合分け
						while (strcmp(Step[STEPNUM].Case2, "を通って") != 0)
						{
							n[STEPNUM]++;
							Step[STEPNUM].Tukaten[n[STEPNUM]] = atoi(Step[STEPNUM].Case2);

							s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
							sscanf(s, "%s", &Step[STEPNUM].Case2);

						}

					}
					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%s%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//A地点に関する情報を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%s%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//B地点に関する情報を読み込む

				}
				else if (strcmp(Step[STEPNUM].Motion, "階段") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].Dansuu);		//階段の段数を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'段'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].updown);		//上るのか下がるのかを読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);		    //'高さ'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'A'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);		    //'奥行'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'B'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'で'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].frontback);		//前か後ろかを読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].BeforeX, &Step[STEPNUM].BeforeY, &Step[STEPNUM].BeforeZ);//Aに関する情報を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%c%lf%lf%lf", trash, &Step[STEPNUM].AfterX, &Step[STEPNUM].AfterY, &Step[STEPNUM].AfterZ);//Bに関する情報を読み込む


				}
				else if (strcmp(Step[STEPNUM].Motion, "回る") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].angle1);		//元の角度を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);		    //'から'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].angle2);		//回転後の角度を読み込む


				}
				else if (strcmp(Step[STEPNUM].Motion, "座る") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].Zamen_height);		//座面の高さを読み込む

				}
				else if (strcmp(Step[STEPNUM].Motion, "立つ") == 0)
				{

				}
				else if (strcmp(Step[STEPNUM].Motion, "回転する") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].KoP);		//対象物を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%s%d%d%d", trash, &Step[STEPNUM].angle1_X, &Step[STEPNUM].angle2_X, &Step[STEPNUM].angle3_X);//Xの座標情報を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%s%d%d%d", trash, &Step[STEPNUM].angle1_Y, &Step[STEPNUM].angle2_Y, &Step[STEPNUM].angle3_Y);//Yの座標情報を読み込む

				}
				else if (strcmp(Step[STEPNUM].Motion, "元に戻す") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].sp_KoP);		//対象物を読み込む

					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%s%d%d%d", trash, &Step[STEPNUM].KoP1, &Step[STEPNUM].KoP2, &Step[STEPNUM].KoP3);//対象物の情報を読み込む


				}
				else if (strcmp(Step[STEPNUM].Motion, "消す") == 0)
				{
					fgets(temp, FGETS_MAX_LENGTH, fp1);

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					s = temp;

					sscanf(s, "%s%d%d%d%d%d", trash, &Step[STEPNUM].KoP1, &Step[STEPNUM].KoP2, &Step[STEPNUM].KoP3, &Step[STEPNUM].KoP4, &Step[STEPNUM].KoP5);//対象物の情報を読み込む


				}
				else if (strcmp(Step[STEPNUM].Motion, "ｶﾒﾗｱﾝｸﾞﾙ") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].angle_num);		//アングル番号を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);		    //'距離'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%lf", &Step[STEPNUM].Kyori);		//距離を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Syunzi_Keika);		//'瞬時''経過'を読み込む


				}
				else if (strcmp(Step[STEPNUM].Motion, "透過する") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

					if ((strcmp(Step[STEPNUM].Hand, "左手") != 0) || ((strcmp(Step[STEPNUM].Hand, "右手") != 0)) || ((strcmp(Step[STEPNUM].Hand, "左手＿右手") != 0)))
					{
						Step[STEPNUM].Jig = atoi(Step[STEPNUM].Hand);
					}

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", trash);		    //'透過率'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].Tokaritsu);		//透過率を読み込む


				}
				else if (strcmp(Step[STEPNUM].Motion, "ﾏｰｶｰ") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].marker_num);		//マーカー番号を読み込む

				}
				else if (strcmp(Step[STEPNUM].Motion, "円柱") == 0)
				{
					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%d", &Step[STEPNUM].entyu_num);		//円柱番号を読み込む

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

					s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
					sscanf(s, "%s", &Step[STEPNUM].Hyouzi_Kesu);		//'表示する'または'消す'を読み込む


				}

				STEPNUM++;

			}
			else if (strcmp(Step[STEPNUM].M, "放す") == 0)
			{
				Step[STEPNUM].Motion[0] = '\0';

				R_C[STEPNUM].T = 0.072;

				sscanf(s, "%d", &Step[STEPNUM].KoP);		//対象物を読み込む

				s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
				sscanf(s, "%c", trash);		    //'を'を読み込んで捨てる

				s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
				sscanf(s, "%s", &Step[STEPNUM].Hand);		//使用している手の情報を読み込む

				s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
				sscanf(s, "%c", trash);		    //'で'を読み込んで捨てる

				s = strchr(&s[1], ' ');				//空白の文字を探して、そこを文字列の開始点にする
				sscanf(s, "%d", &Step[STEPNUM].HandShape);		//手の形を読み込む

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

		//Fを読み込む変数の初期化　※これをしないと網羅で回らない
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
	char* temp, * s;     //テキストの内容を読み込む
	char* trash;
	int i = 0;
	int j = 0;

	temp = (char*)malloc(20000);
	trash = (char*)malloc(20000);


	fp1 = fopen(inPath_Input_Buhin_Zokusei, "r");						//NowPathのファイルを開いて、読み込む
	if (fp1 == NULL)ShowFinMsg("部品属性がありません");		//ファイルを開けなければ終了

	//以降は、開けたファイルが部品属性である前提で処理をする

	//1行目の読み込み
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	//2行目の読み込み
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	sscanf(temp, "%d", &N);

	for (i = 1; i <= N; i++)
	{
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		s = temp;

		sscanf(s, "%d%s%s%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%d%d%d%d%d%d%d%d", &R_B[i].Bu_num, &R_B[i].Buhin_Name, &R_B[i].CAD_file, &R_B[i].Buhin_num, &R_B[i].Center_X, &R_B[i].Center_Y, &R_B[i].Center_Z, &R_B[i].Long_X, &R_B[i].Long_Y, &R_B[i].Long_Z, &R_B[i].Xmin, &R_B[i].Xmax, &R_B[i].Ymin, &R_B[i].Ymax, &R_B[i].Zmin, &R_B[i].Zmax, &R_B[i].Color_num, &R_B[i].weight, &R_B[i].kogu_num, &R_B[i].kogu_name, &R_B[i].situkan, &R_B[i].tokutyo, &R_B[i].OK, &R_B[i].Box_type, &R_B[i].Buhinsu);
		R_B[i].Xkotei = R_B[i].Long_X;//並べ替え前に格納
		R_B[i].Ykotei = R_B[i].Long_Y;//並べ替え前に格納
		R_B[i].Zkotei = R_B[i].Long_Z;//並べ替え前に格納
		if (R_B[i].Box_type == 1)
		{
			printf("%dは整列の部品箱です(個数：%d)\n", i, R_B[i].Buhinsu);
		}
		else
		{
			printf("%dは集合の部品箱です(個数：%d)\n", i, R_B[i].Buhinsu);
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
	char* temp, * s;     //テキストの内容を読み込む
	int trashi;
	char* trash;
	double temps;

	temp = (char*)malloc(20000);
	trash = (char*)malloc(20000);
	printf("%s\n", inPath_Input_Tyukan);
	fp1 = fopen(inPath_Input_Tyukan, "r");						//NowPathのファイルを開いて、読み込む
	if (fp1 == NULL)ShowFinMsg("中間製品属性がありません");		//ファイルを開けなければ終了

	//1行目の読み込み
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
	s = temp;

	sscanf(s, "%s%d", trash, &T);

	//2行目の読み込み
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	for (int i = 1; i <= T; i++)
	{
		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		s = temp;

		sscanf(s, "%d%d%lf%lf%lf", &R_T[i].Tyunum, &trashi, &R_T[i].lx, &R_T[i].ly, &R_T[i].lz);

		//並べ替え前に格納
		R_T[i].Xkotei = R_T[i].lx;
		R_T[i].Ykotei = R_T[i].ly;
		R_T[i].Zkotei = R_T[i].lz;
	}

	//並び替え
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
	
	int First_Time = 0;//始めは飛ばす
	int File_num;//ファイル数
	int Knumber;
	int flg;
	int flag = 0;
	int j = 0;
	int count = 0;

	char InPath[STR_MAX];

	//ファイル数の計算
	File_num = (N * s_File) / MaxExcel;

	temp = (char*)malloc(20000);

	for (int file = 0; file <= File_num; file++)
	{

		sprintf(InPath, "%s%d_J.csv", inPath_Input_Junjo, file);

		fp1 = fopen(InPath, "r");						//NowPathのファイルを開いて、読み込む
		if (fp1 == NULL)ShowFinMsg("組立順序がありません");		//ファイルを開けなければ終了

		First_Time = 0;

		while (fgets(temp, FGETS_MAX_LENGTH, fp1) != NULL)//組立順序のファイルの最後まで繰り返す
		{
			Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
			if (strstr(temp, "K") != NULL) //読み込んだ行にKの文字があるとき
			{
				if (First_Time != 0)
				{
					Knumber = j - 1;
				}
				else //最初のKのとき
				{
					First_Time++;
				}

				count++;

				if (J == count)//該当する組立順序
				{
					sscanf(temp, "%c%lld%d", &K, &R_KJUNJO.kumij, &flg);//K,組立順序番号,存在フラグの読み込み
				}

				j = 1;
				
			}
			else
			{

				if (J == count)//該当する組立順序
				{
					sscanf(temp, "%d%d%d%d%s%s%s%d%d%d", &R_KJUNJO.kuminum[j], &R_KJUNJO.p1[j], &R_KJUNJO.p2[j], &R_KJUNJO.p3[j], R_KJUNJO.Kumituke_Pos[j], R_KJUNJO.Oya_Pos1[j], R_KJUNJO.Oya_Pos2[j], &R_KJUNJO.Jig[j], &R_KJUNJO.KouguNum[j], &R_KJUNJO.DousaNum[j]);

					//親部品の方向を格納する処理
					if ((strcmp(R_KJUNJO.Oya_Pos1[j], "上") == 0 || strcmp(R_KJUNJO.Oya_Pos1[j], "下") == 0))//親部品の姿勢（X軸方向）が上または下
					{
						R_KJUNJO.Dir[j] = 1;//X軸が高さ
					}
					else if ((strcmp(R_KJUNJO.Oya_Pos2[j], "上") == 0 || strcmp(R_KJUNJO.Oya_Pos2[j], "下") == 0))//親部品の姿勢（Z軸方向）が上または下
					{
						R_KJUNJO.Dir[j] = 3;//Z軸が高さ
					}
					else
					{
						R_KJUNJO.Dir[j] = 2;//Y軸が高さ
					}

					if (j == N - 1)//最終行の時
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

//組付け位置変更用関数
void Kumituke_Position(int STEP)
{
	double temp = 0.0;
	int tempJig;

	Beforejignum = 2 + R_H.PA + R_H.T;

	for (int i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "組立移動する") == 0)//"組立移動する"の時
		{
			for (int Jig = 1; Jig <= R_H.J; Jig++)
			{
				tempJig = atoi(Step[i + 1].Sup4);
				if (Step[i + 1].flag == 1)//工具組付けのとき
				{
					for (int j = i; j >= 1; j--)
					{
						if (strcmp(Step[j].Motion, "運ぶ") == 0 && strcmp(Step[j].Hand, "左手") == 0)//左手で運ぶ作業を探す
						{
							temp = Step[j].AfterZ;//格納
							break;
						}
					}
					if (R_S0[Beforejignum + Jig].N == Step[i + 1].Case)//"組立てる"治具を探しにいく
					{
						Step[Step[i + 1].Assy2].X_Dif = R_S0[Beforejignum + Jig].lx - Step[i].BeforeX;//治具の中心点からXの差を求める
						Step[Step[i + 1].Assy2].Y_Dif = R_S0[Beforejignum + Jig].ly - Step[i].BeforeY;//治具の中心点からYの差を求める
						Step[Step[i + 1].Assy2].Z_Dif = R_S0[Beforejignum + Jig].lz - Step[i].BeforeZ;//治具の中心点からZの差を求める
						break;
					}
				}
				else
				{
					for (int j = i; j >= 1; j--)
					{
						if (strcmp(Step[j].Motion, "運ぶ") == 0 && strcmp(Step[j].Hand, "左手") == 0)//左手で運ぶ作業を探す
						{
							temp = Step[j].AfterZ;//格納
							break;
						}
					}
					if (R_S0[Beforejignum + Jig].N == tempJig)//"組立てる"治具を探しにいく
					{
						Step[Step[i + 1].Assy2].X_Dif = R_S0[Beforejignum + Jig].lx - Step[i].BeforeX;//治具の中心点からXの差を求める
						Step[Step[i + 1].Assy2].Y_Dif = R_S0[Beforejignum + Jig].ly - Step[i].BeforeY;//治具の中心点からYの差を求める
						Step[Step[i + 1].Assy2].Z_Dif = temp - Step[i].BeforeZ;//治具の中心点からZの差を求める
						break;
					}
				}
				
			}
			Step[i].OFFSET = Step[i].BeforeZ - Step[i].AfterZ;

		}
	}
}

//部品箱・治具評価関数
void BoxJig_Hyouka()	//部品箱・治具評価用関数
{
	int temp;
	double temp2;
	int Box[3];//部品箱（集合）の体積
	double Box_capa[3];

	Box[0] = 95 * 100 * 50;//部品箱（集合）小の体積を求める
	Box[1] = 96 * 160 * 72;//部品箱（集合）中の体積を求める
	Box[2] = 145 * 200 * 90;//部品箱（集合）大の体積を求める

	for (int i = 1; i <= N; i++)
	{
		BJ[i].Volume = R_B[i].Long_X * R_B[i].Long_Y * R_B[i].Long_Z;//部品の体積を求める

		BJ[i].num = i;

		//**治具のサイズ決定**//
		BJ[i].Jig_x = R_B[i].Long_X + 20; //治具のxサイズの決定
		BJ[i].Jig_y = R_B[i].Long_Y + 20; //治具のyサイズの決定
		BJ[i].Jig_z = 20; //治具のzサイズの決定（固定値）

		swap(i);//大きい順に並び替え

		//**部品箱のサイズを決める処理**//
		if (R_B[i].Box_type == 1)//整列させる部品箱の時
		{
			if (R_B[i].Buhinsu % 2 == 0)//想定する部品数が偶数の時
			{
				for (int k = 1; k <= 10; k++)
				{
					if ((R_B[i].Buhinsu > k * k + k) && (R_B[i].Buhinsu <= ((k + 1) * (k + 1))))
					{
						BJ[i].Box_x = (R_B[i].Xkotei + 20) * k; //部品箱のxサイズの決定=正方形サイズ
						BJ[i].Box_y = (R_B[i].Ykotei + 25) * k; //部品箱のyサイズの決定=正方形サイズ
						BJ[i].Box_z = 50; //部品箱のzサイズの決定
						break;
					}
					else if (R_B[i].Buhinsu > k * k && R_B[i].Buhinsu <= k * k + k)
					{
						BJ[i].Box_x = (R_B[i].Xkotei + 20) * k; //部品箱のxサイズの決定
						BJ[i].Box_y = (R_B[i].Ykotei + 25) * (k + 1); //部品箱のyサイズの決定
						BJ[i].Box_z = 50; //部品箱のzサイズの決定
						break;
					}
				}
			}
			else//想定する部品数が奇数の時
			{
				for (int k = 1; k <= 10; k++)
				{
					if ((R_B[i].Buhinsu > k * k + k) && (R_B[i].Buhinsu <= ((k + 1) * (k + 1))))
					{
						BJ[i].Box_x = (R_B[i].Xkotei + 20) * k; //部品箱のxサイズの決定=正方形サイズ
						BJ[i].Box_y = (R_B[i].Ykotei + 25) * k; //部品箱のyサイズの決定=正方形サイズ
						BJ[i].Box_z = 50; //部品箱のzサイズの決定
						break;
					}
					else if (R_B[i].Buhinsu > k * k && R_B[i].Buhinsu <= k * k + k)
					{
						BJ[i].Box_x = (R_B[i].Xkotei + 20) * k; //部品箱のxサイズの決定
						BJ[i].Box_y = (R_B[i].Ykotei + 25) * (k + 1); //部品箱のyサイズの決定
						BJ[i].Box_z = 50; //部品箱のzサイズの決定
						break;
					}
					else if (R_B[i].Buhinsu == 1)
					{
						BJ[i].Box_x = (R_B[i].Xkotei + 30) * 1; //部品箱のxサイズの決定=正方形サイズ
						BJ[i].Box_y = (R_B[i].Ykotei + 30) * 1; //部品箱のyサイズの決定=正方形サイズ
						BJ[i].Box_z = 50; //部品箱のzサイズの決定
						break;
					}
				}
			}
		}
		else//集合の部品箱の時
		{
			for (int B = 0; B <= 2; B++)
			{
				Box_capa[B] = Box[B] / BJ[i].Volume;//それぞれの部品箱における許容個数を算出
			}
			for (int B2 = 0; B2 <= 2; B2++)
			{
				if (Box_capa[B2] > R_B[i].Buhinsu)//許容個数>想定個数の時
				{
					if (B2 == 0)//最も小さい部品箱の時
					{
						BJ[i].Box_x = 95; //部品箱のxサイズの決定
						BJ[i].Box_y = 100; //部品箱のyサイズの決定
						BJ[i].Box_z = 50; //部品箱のzサイズの決定
					}
					else if (B2 == 1)//2番目に小さい部品箱の時
					{
						BJ[i].Box_x = 96; //部品箱のxサイズの決定
						BJ[i].Box_y = 160; //部品箱のyサイズの決定
						BJ[i].Box_z = 72; //部品箱のzサイズの決定
					}
					else//最も大きい部品箱の時
					{
						BJ[i].Box_x = 145; //部品箱のxサイズの決定
						BJ[i].Box_y = 200; //部品箱のyサイズの決定
						BJ[i].Box_z = 90; //部品箱のzサイズの決定
					}
					break;
				}
				else//どの部品箱にも該当しないとき
				{
					printf("部品%dの部品箱がエラーです\n", i);
				}
			}

		}

		//**部品が１段目か２段目かを決める処理**//
		if (R_B[i].Long_X <= 21.2 && R_B[i].Long_Y <= 14.9)//やりづらさ要因の小さいに該当する部品のとき
		{
			BJ[i].Box_hyouka = 0; //最前列に配置（評価番号0）
		}
		else //それ以外の部品のとき
		{
			BJ[i].Box_hyouka = 1; //2列目に配置（評価番号1）
		}

	}

	//2段目から1段目に配置する(2段目が長くなった時)ための前準備
	for (int j = 1; j <= N; j++)
	{
		for (int k = j + 1; k <= N; k++)
		{
			if (BJ[j].Volume > BJ[k].Volume)
			{
				//体積順に並び替える
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

//「伸ばす」時間値導出用関数
void Time_Reach(int STEP)
{
	int flag = 0;

	for (int i = 1; i <= STEP; i++)
	{
		flag = 0;
		if (strcmp(Step[i].Motion, "伸ばす") == 0 || strcmp(Step[i].M, "伸ばす") == 0)//動作が「伸ばす」のとき
		{
			if (Step[i].BeforeX == Step[i].AfterX && Step[i].BeforeY == Step[i].AfterY && Step[i].BeforeZ == Step[i].AfterZ)//見かけの"伸ばす"のとき
			{
				R_C[i].T = 0.0;
			}
			else
			{
				for (int j = i + 1; j <= STEP; j++)
				{

					//**「伸ばす」⇒「掴み取る」の時の「伸ばす」の時間値導出**//
					if (strcmp(Step[j].Motion, "掴み取る") == 0)			//次の動作が「掴み取る」の時
					{
						//**部品箱の部品に手を伸ばす=ケースC,Dの時間値を適用**//
						if (strncmp(Step[j].BeforeJig, "部品置き場", 10) == 0) //5文字目までが「部品置き場」で一致するか
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
						//工具に手を伸ばす = ケースA
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
					//**「伸ばす」⇒「掴む」の時の「伸ばす」の時間値導出(ケースA)**//
					else if (strcmp(Step[j].Motion, "掴む") == 0)
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
					//**「伸ばす」⇒「放す」の時の「伸ばす」の時間値導出(ケースA)**//
					else if (strcmp(Step[j].Motion, "放す") == 0)
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
					//**「伸ばす」⇒「伸ばす」の時の「伸ばす」の時間値導出(ケースA)**//
					else if (strcmp(Step[j].Motion, "伸ばす") == 0)
					{
						flag = 1;

						//**前の動作の「伸ばす」の時間値をケースAに**//
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


						//この動作番号の「伸ばす」の時間値をケースAに//
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
				STime_Reach += R_C[i].T;//合計時間用
				STime_Reach += R_C[i + 1].T;//合計時間用
			}
			else
			{
				STime_Reach += R_C[i].T;//合計時間用
			}

		}
	}
}

//「持ち直す」時間値導出関数
void Time_Turn(int STEP)
{
	for (int i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "持ち直す") == 0)//動作が「持ち直す」のとき
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

			STime_Turn += R_C[i].T;//合計時間用

			All_Angle = All_Angle + Step[i].BeforeX + Step[i].BeforeY + Step[i].BeforeZ + Step[i].AfterX + Step[i].AfterY + Step[i].AfterZ;//総回転角度用
		}
	}
}

//「運ぶ」時間値導出用関数
void Time_Bring(int STEP)
{
	for (int i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "運ぶ") == 0 || strcmp(Step[i].M, "運ぶ") == 0)//動作が「運ぶ」のとき
		{
			if (Step[i].BeforeX == Step[i].AfterX && Step[i].BeforeY == Step[i].AfterY && Step[i].BeforeZ == Step[i].AfterZ)//見かけの"運ぶ"のとき
			{
				R_C[i].T = 0.0;
			}
			else
			{
				for (int j = i + 1; j <= STEP; j++)
				{
					//「運ぶ」⇒「組立てる」の時の「運ぶ」の時間値導出（ケースC）
					if (strcmp(Step[j].Motion, "組立てる") == 0)
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
					//「運ぶ」⇒「付ける」の時の「運ぶ」の時間値導出（ケースC）
					else if (strcmp(Step[j].Motion, "付ける") == 0)
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
					//「運ぶ」⇒「組立移動する」の時の「運ぶ」の時間値導出（ケースC）
					else if (strcmp(Step[j].Motion, "組立移動する") == 0)
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
					//「運ぶ」⇒「置き放す」の時の「運ぶ」の時間値導出（ケースB）
					else if (strcmp(Step[j].Motion, "置き放す") == 0)
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
					//「運ぶ」⇒「放す」の時の「運ぶ」の時間値導出（ケースB）
					else if (strcmp(Step[j].Motion, "放す") == 0)
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
					//「運ぶ」⇒「運ぶ」の時の1つ目の「運ぶ」の時間値導出（ケースB）
					else if (strcmp(Step[j].Motion, "運ぶ") == 0)
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
			STime_Bring += R_C[i].T;//合計時間用
		}
	}
}

//「掴み取る」時間値導出用関数
void Time_grab(int STEP)
{

	for (int i = 1; i <= STEP; i++)
	{

		if (strcmp(Step[i].Motion, "掴み取る") == 0 || strcmp(Step[i].M, "掴み取る") == 0)//動作が「掴み取る」のとき
		{
			R_C[i].T = 0.126;

			STime_Grab += R_C[i].T;//合計時間用

			//	if (strncmp(Step[i].BeforeJig, "部品置き場", 5) == 0)		//部品箱の部品を掴むとき
			//		//if (strcmp(Step[j].BeforeJig, "部品置き場") == 0)		//部品箱の部品を掴むとき
			//	{
			//		printf("部品%dは他のものと入り混じってますか（Yes:1,No:2）:", Step[i].KoP);
			//		scanf("%d", &Step[i].Case4_1);

			//		if (Step[i].Case4_1 == 1)//ケース4系に該当するもの
			//		{

			//			if (R_B[Step[i].KoP].Long_X < 6 && R_B[Step[i].KoP].Long_Y < 6 && R_B[Step[i].KoP].Long_Z < 3)	//ケース4C
			//			{
			//				R_C[i].T = 12.9 * 0.036;
			//			}
			//			else if (R_B[Step[i].KoP].Long_X < 25 && R_B[Step[i].KoP].Long_Y < 25 && R_B[Step[i].KoP].Long_Z < 25)		//ケース4B
			//			{
			//				R_C[i].T = 9.1 * 0.036;
			//			}
			//			else//ケース4A
			//			{
			//				R_C[i].T = 7.3 * 0.036;
			//			}

			//		}
			//		else if (Step[i].Case4_1 == 2)//ケース1系に該当するもの
			//		{
			//			printf("部品%dの円筒形の底面と1側面に障害がありますか（Yes:1,No:2）:", Step[i].KoP);
			//			scanf("%d", &Step[i].Case4_2);

			//			if (Step[i].Case4_2 == 1)//ケース1C系に該当するもの
			//			{
			//				printf("部品%dの直径(mm)を入力してください(整数)：", Step[i].KoP);
			//				scanf("%d", &Step[i].Case1C);

			//				if (Step[i].Case1C < 6)		//直径が6㎜よりも小さいとき
			//				{
			//					R_C[i].T = 10.8 * 0.036;
			//				}
			//				else if (Step[i].Case1C >= 6 && Step[i].Case1C < 12)		//直径が6㎜から12㎜までのもの
			//				{
			//					R_C[i].T = 8.7 * 0.036;
			//				}
			//				else if (Step[i].Case1C >= 12)
			//				{
			//					R_C[i].T = 7.3 * 0.036;
			//				}

			//			}
			//			else if (Step[i].Case4_2 == 2)//ケース1Aまたは1Bに該当するもの
			//			{
			//				//swap(i);
			//				if (R_B[Step[i].KoP].Long_X * R_B[Step[i].KoP].Long_Y < 316)		//やりづらさ要因の「小さい」に該当するもの
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

			//	else//工具を掴むとき
			//	{
			//		R_C[i].T = 2.0 * 0.036;			//工具を掴む=ケース1A
			//	}
		}
	}
}

//「組立てる」、「組立移動する」時間値導出用関数
void Time_Assemble(int STEP)
{
	for (int i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "組立てる") == 0 || strcmp(Step[i].M, "組立てる") == 0)//動作が「組立てる」のとき
		{
			R_C[i].T = 0.5;//「組立てる」の時間値＝0.5[sec]

			R_C[i - 1].T = 0.5;//「組立移動する」の時間値＝0.5[sec]

			//for (int k = 1; k <= K; k++)
			//{
			//	if ((Step[i].Assy1 == R_K.p1[k] && Step[i].Assy2 == R_K.p2[k]) || (Step[i].Assy2 == R_K.p1[k] && Step[i].Assy1 == R_K.p2[k]))//組み付ける部品の情報から接触番号を特定する
			//	{

			//		for (int l = 1; l <= A; l++)
			//		{
			//			//どの接触線を満たすのか
			//			if (R_K.Sessyoku[k][l] == 1)
			//			{
			//				if (R_Se[l].Time > R_C[i].T)//複数の接触線を満たす場合、大きい方の時間値を用いる
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

//並び替え関数
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

//S0変更用関数
void S0_Change(int STEP)
{
	int countB = 0;//部品箱カウント用
	int count = 0;
	int countFR = 0;
	int countFR2 = 0;
	int countR = 0;//右手扱う部品カウント
	int countR1 = 0;//左手1列目カウント
	int countR2 = 0;//左手2列目カウント
	int countL1 = 0;//右手1列目カウント
	int countL2 = 0;//右手2列目カウント
	int Jigflag = 0;
	int center;//中心治具
	int centerflow;//中心治具から何個離れるか
	int BeforeJig = 0;
	int flag = 0;
	double tempX;
	double tempR1;
	double tempR2;
	double tempL1;
	double tempL2;
	double tempMax = 0.0;//完成品箱の位置用
	double tempMax2 = 0.0;
	double tempBoxsizeL1;
	double tempBoxsizeL2;

	countJ = 0;

	R_H.TR = R_H.TR + 1;//トレイの数を1つ増やす（仮置き場用）

	for (int i = 1; i <= STEP; i++)
	{
		//if (strcmp(Step[i].Motion, "掴み取る") == 0 || strcmp(Step[i].Motion, "掴む") == 0)//動作が「掴み取る」又は「掴む」のとき
		if (strcmp(Step[i].Motion, "掴み取る") == 0)//動作が「掴み取る]のとき
		{
			for (int j = 1; j <= N; j++)
			{
				if (Step[i].KoP == j)//つかむ対象が単部品だった時
				{
					countB++;
					BJ[countB].Number = j;//部品の出現順を記録

					if (strcmp(Step[i].Hand, "左手") == 0)
					{
						BJ[j].Box_hand = 0;
					}
					else if (strcmp(Step[i].Hand, "右手") == 0)
					{
						if (BJ[j].Box_hyouka == 0)//その部品を最前列に配置する時
						{
							countF++;//カウント
						}
						BJ[j].Box_hand = 1;
						countR++;//右手で扱う部品をカウント
					}
				}
			}
		}
		else if (strcmp(Step[i].Motion, "付ける") == 0 && strcmp(Step[i].BeforeJig, "-10") == 0)//動作が「付ける」かつ対象が工具のとき（工具パターン1用）
		{
			for (int j = 1; j <= N; j++)
			{

				if (Step[i].KoP == j)//つかむ対象が単部品だった時
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
						BJ[countB].Number = j;//部品の出現順を記録

						if (strcmp(Step[i].Hand, "左手") == 0)
						{
							BJ[j].Box_hand = 0;
						}
						else if (strcmp(Step[i].Hand, "右手") == 0)
						{
							if (BJ[j].Box_hyouka == 0)//その部品を最前列に配置する時
							{
								countF++;//カウント
							}
							BJ[j].Box_hand = 1;
							countR++;//右手で扱う部品をカウント
						}
					}
				}

			}
			flag = 0;
		}

		if (strcmp(Step[i].Motion, "付ける") == 0 && atoi(Step[i].BeforeJig) != BeforeJig && atoi(Step[i].BeforeJig) != -10)
		{
			Jigflag = 0;
			for (int k = 1; k <= N; k++)
			{
				if (Step[i].KoP == k)//治具に付ける部品が単部品だったとき→ベース部品が変化したとき
				{
					Jigflag = 1;
					break;
				}
				else if (Step[i].KoP != k)//治具に付ける部品が中間製品だったとき→姿勢が変化したとき
				{
					Jigflag = 0;
				}
			}
			if (Jigflag == 1)
			{
				//ベース部品に合わせた治具に変更
				countJ++;
				Ray.Jig_x[countJ] = BJ[Step[i].KoP].Jig_x;
				Ray.Jig_y[countJ] = BJ[Step[i].KoP].Jig_y;
				Ray.Jig_z[countJ] = BJ[Step[i].KoP].Jig_z;
			}
			else if (Jigflag == 0)
			{
				for (int Tyukan = 1; Tyukan <= T; Tyukan++)
				{
					if (Step[i].KoP == R_T[Tyukan].Tyunum)//該当する中間製品を探す
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

	//**右2列目の部品多い場合1列目に配置する処理**//
	if (countR / 2 != countF)//(右手で扱う部品数)/2が右前列の個数と一致してないとき=1列目と2列目のバランスが悪いとき
	{
		for (int j = 1; j <= N; j++)
		{
			if (BJ[j].Box_hand == 1 && BJ[j].Box_hyouka == 1)//2列目且つ右側に配置する部品のとき
			{
				countFR++;
				Ray.num[countFR] = j;//2列目右側の部品を格納				
			}
		}
		//(右手で扱う部品数)/2が右前列の個数と一致するまで前列に持ってくる
	}

	for (int k = 1; k <= N; k++)
	{
		for (int l = 1; l <= countFR; l++)
		{
			if (Ray.num[k] == BJ[l].num)//2列目右側の部品の中から部品が小さい順に1列目に配置する
			{
				countFR2++;
				BJ[Ray.num[k]].Box_hyouka = 0;//1列目に配置する
				break;
			}
		}
		if (countFR2 == (countR / 2 - countF))//1列目と2列目のバランスが良くなったら終了
		{
			break;
		}
	}

	//****治具座標の決定****//
	if (countJ % 2 == 1)//治具の数が"奇数"のとき
	{
		center = (countJ + 1) / 2;
		centerflow = countJ / 2;
		//中央に配置したい治具を(0,125,850)にする
		Ray.Jig_xCo[center] = tempX = 0;
		Ray.Jig_yCo[center] = 125;
		Ray.Jig_zCo[center] = 850;

		if (centerflow != 0)//治具が1個以上
		{
			//**作業者から見て右側の治具について**//
			for (int Rodd = 0; Rodd < centerflow; Rodd++)
			{
				//printf("%4.2lf,%4.2lf\n", Ray.Jig_x[center - x] / 2, Ray.Jig_x[center - x - 1] / 2);
				Ray.Jig_xCo[center - Rodd - 1] = tempX + (Ray.Jig_x[center - Rodd] / 2) + 64 + (Ray.Jig_x[center - Rodd - 1] / 2);
				tempX = Ray.Jig_xCo[center - Rodd - 1];
				Ray.Jig_yCo[center - Rodd - 1] = 125;
				Ray.Jig_zCo[center - Rodd - 1] = 850;
			}

			//**作業者から見て左側の治具について**//
			tempX = 0;//基準を中心に戻す
			for (int Lodd = 0; Lodd < centerflow; Lodd++)
			{
				Ray.Jig_xCo[center + Lodd + 1] = tempX - (Ray.Jig_x[center + Lodd] / 2) - 64 - (Ray.Jig_x[center + Lodd + 1] / 2);
				tempX = Ray.Jig_xCo[center + Lodd + 1];
				Ray.Jig_yCo[center + Lodd + 1] = 125;
				Ray.Jig_zCo[center + Lodd + 1] = 850;
			}
		}
	}
	else//治具の数が"偶数"のとき
	{
		centerflow = countJ / 2;
		tempX = 0;
		//**作業者から見て右側の治具について**//
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
		//**作業者から見て左側の治具について**//
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

	//***治具の座標・大きさを代入***//
	Beforejignum = 2 + R_H.PA + R_H.T;
	count++;
	for (int x = 1; x <= R_H.J; x++)
	{
		if (count <= countJ)//使用する治具の座標決定
		{
			R_S0[Beforejignum + x].lx = Ray.Jig_xCo[count];
			R_S0[Beforejignum + x].ly = Ray.Jig_yCo[count];
			R_S0[Beforejignum + x].lz = Ray.Jig_zCo[count];
			R_S0[Beforejignum + x].Bx = Ray.Jig_x[count];
			R_S0[Beforejignum + x].By = Ray.Jig_y[count];
			R_S0[Beforejignum + x].Bz = Ray.Jig_z[count];
			count++;
		}
		else//使用しない治具は全て0
		{
			R_S0[Beforejignum + x].lx = 0.0;
			R_S0[Beforejignum + x].ly = 0.0;
			R_S0[Beforejignum + x].lz = 0.0;
			R_S0[Beforejignum + x].Bx = 0.0;
			R_S0[Beforejignum + x].By = 0.0;
			R_S0[Beforejignum + x].Bz = 0.0;
		}
	}

	//***机の大きさを決定***//
	R_S0[Beforejignum + R_H.J + 1].lx = 1000;//x座標

	tempR1 = 0.0;//初期化
	tempR2 = 0.0;

	//****部品箱・部品の座標決定****//
	for (int y = 1; y <= N; y++)//出現する順番
	{
		if (BJ[BJ[y].Number].Box_hand == 0)//部品箱を左手で扱うとき
		{
			if (BJ[BJ[y].Number].Box_hyouka == 0)//部品が"小さい"とき=1列目
			{
				//printf("%4.2lfaiueo\n", BJ[BJ[y].Number].Box_x);
				Ray.Box_xCo[BJ[y].Number] = tempR1 - BJ[BJ[y].Number].Box_x;
				tempR1 = Ray.Box_xCo[BJ[y].Number];
				Ray.Box_yCo[BJ[y].Number] = 300;
				Ray.Box_zCo[BJ[y].Number] = 850;
				Ray.Parts_xCo[BJ[y].Number] = tempR1 + (BJ[BJ[y].Number].Box_x / 2);
				Ray.Parts_yCo[BJ[y].Number] = Ray.Box_yCo[BJ[y].Number] + (BJ[BJ[y].Number].Box_y) / 2;
				Ray.Parts_zCo[BJ[y].Number] = 860;
				Ray.Box_inclination[BJ[y].Number] = 0;//部品箱の傾きを指定
				tempMax = Ray.Box_xCo[BJ[y].Number];//完成品箱用の格納
			}
			else if (BJ[BJ[y].Number].Box_hyouka == 1)//それ以外=2列目
			{
				Ray.Box_xCo[BJ[y].Number] = tempR2 - BJ[BJ[y].Number].Box_x;
				tempR2 = Ray.Box_xCo[BJ[y].Number];
				Ray.Box_yCo[BJ[y].Number] = 300;
				Ray.Box_zCo[BJ[y].Number] = 1050;
				Ray.Parts_xCo[BJ[y].Number] = tempR2 + (BJ[BJ[y].Number].Box_x / 2);
				Ray.Parts_yCo[BJ[y].Number] = Ray.Box_yCo[BJ[y].Number] - 10 + (BJ[BJ[y].Number].Box_y) / 2;
				//Ray.Parts_zCo[BJ[y].Number] = Ray.Box_zCo[BJ[y].Number] + (BJ[BJ[y].Number].Box_y * sin(20 * 3.141592 / 180) / 2) + (R_B[y].Ykotei / 2) * tan(20 * 3.141592 / 180) + R_B[y].Zkotei / 2;
				Ray.Parts_zCo[BJ[y].Number] = 1100;
				if (sqrt(((Ray.Parts_xCo[BJ[y].Number] + 200) * (Ray.Parts_xCo[BJ[y].Number] + 200) + (Ray.Parts_yCo[BJ[y].Number] * Ray.Parts_yCo[BJ[y].Number]))) > 600)//部品の位置が最大作業域を超えた場合
				{

				}
				Ray.Box_inclination[BJ[y].Number] = -20;//部品箱の傾きを指定

			}
		}
		else//部品箱を右手で扱うとき
		{
			if (BJ[BJ[y].Number].Box_hyouka == 0)//部品が"小さい"とき=1列目
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
				Ray.Box_inclination[BJ[y].Number] = 0;//部品箱の傾きを指定

			}
			else if (BJ[BJ[y].Number].Box_hyouka == 1)//それ以外=2列目
			{
				if (countL2 == 0)//中心線から1つ目の部品箱
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
				if (sqrt(((Ray.Parts_xCo[BJ[y].Number] - 200) * (Ray.Parts_xCo[BJ[y].Number] - 200) + (Ray.Parts_yCo[BJ[y].Number] * Ray.Parts_yCo[BJ[y].Number]))) > 600)//部品の位置が最大作業域を超えた場合
				{

				}
				Ray.Box_inclination[BJ[y].Number] = -20;//部品箱の傾きを指定

			}
		}
		if (y == N)
		{
			tempMax2 = tempMax;//最も左の部品箱の座標を格納
		}
	}

	//***部品箱の座標・大きさを代入***//
	Beforetraynum = 2 + R_H.PA + R_H.T + R_H.J + R_H.MI + R_H.TA;
	for (int tray = 1; tray <= N; tray++)//出現する部品
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

	//***部品の座標を代入***//
	for (int Parts = 1; Parts <= N; Parts++)
	{
		R_S0[2 + Parts].lx = Ray.Parts_xCo[Parts];
		R_S0[2 + Parts].ly = Ray.Parts_yCo[Parts];
		R_S0[2 + Parts].lz = Ray.Parts_zCo[Parts];
	}

	//***完成品箱の座標・大きさ決定***//
	Beforefinishboxnum = 2 + R_H.PA + R_H.T + R_H.J + R_H.MI + R_H.TA + N;
	R_S0[1 + Beforefinishboxnum].Bx = tempMax2 - 350;//X座標(最も左の部品箱の真横に配置する)
	R_S0[1 + Beforefinishboxnum].By = 200;//Y座標
	R_S0[1 + Beforefinishboxnum].Bz = 850;//Z座標
	R_S0[1 + Beforefinishboxnum].Hx = R_T[1].lx + 10;//Xの長さ(完成品の大きさ+10mm)
	R_S0[1 + Beforefinishboxnum].Hy = R_T[1].ly + 10;//Yの長さ(完成品の大きさ+10mm)
	R_S0[1 + Beforefinishboxnum].Hz = 20;//Zの長さ
	R_S0[1 + Beforefinishboxnum].C = 0;//傾き

	//***完成品箱の中心点の決定***//
	Ray.finish_centerX = R_S0[1 + Beforefinishboxnum].Bx + (R_S0[1 + Beforefinishboxnum].Hx / 2);
	Ray.finish_centerY = R_S0[1 + Beforefinishboxnum].By + (R_S0[1 + Beforefinishboxnum].Hy / 2);
	Ray.finish_centerZ = 855;

	//***仮置き場の座標・大きさ決定***//
	R_S0[2 + Beforefinishboxnum].Bx = R_S0[Beforejignum + countJ].lx - R_S0[Beforejignum + countJ].Bx - 100;//X座標
	R_S0[2 + Beforefinishboxnum].By = R_S0[Beforejignum + countJ].ly - 25;//Y座標
	R_S0[2 + Beforefinishboxnum].Bz = R_S0[Beforejignum + countJ].lz;//Z座標
	R_S0[2 + Beforefinishboxnum].Hx = 100;//Xの長さ
	R_S0[2 + Beforefinishboxnum].Hy = 80;//Yの長さ
	R_S0[2 + Beforefinishboxnum].Hz = 25;//Zの長さ
	R_S0[2 + Beforefinishboxnum].C = 0;//傾き
	R_S0[2 + Beforefinishboxnum].S = 5;//厚さ
	R_S0[2 + Beforefinishboxnum].N = R_S0[1 + Beforefinishboxnum].N + 1;//番号
	R_S0[2 + Beforefinishboxnum].lx = 1;
	R_S0[2 + Beforefinishboxnum].ly = 1;
	R_S0[2 + Beforefinishboxnum].lz = 4;//色

	//***仮置き場の中心点の決定***//
	Ray.karioki_centerX = R_S0[2 + Beforefinishboxnum].Bx + (R_S0[2 + Beforefinishboxnum].Hx / 2);
	Ray.karioki_centerY = R_S0[2 + Beforefinishboxnum].By + (R_S0[2 + Beforefinishboxnum].Hy / 2);
	Ray.karioki_centerZ = 855;

	Beforekougunum = 2 + R_H.PA;

	//***工具の座標を決定
	for (int Kougu = 1; Kougu <= R_H.T; Kougu++)
	{
		R_S0[Beforekougunum + Kougu].lx = 200;//右手の初期位置
		R_S0[Beforekougunum + Kougu].ly = 0;
		R_S0[Beforekougunum + Kougu].lz = 1240;//机から390mmの位置
	}

}

//動作座標変更関数
void C_Change(int STEP)
{
	int i;
	int j;
	int k;
	int l;
	int tempJig;//int変換用
	int flag = 0;
	double temp;
	double temp_Buhin = 0.0;//部品の長さを格納しておく変数

	for (i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "伸ばす") == 0)//動作が"伸ばす"のとき
		{
			if (strcmp(Step[i + 1].Motion, "掴み取る") == 0)//次の動作が"掴み取る"のとき
			{
				for (k = 1; k <= N; k++)
				{
					if (Step[i + 1].KoP == k)//掴み取るものが単部品だったら
					{
						Step[i].AfterX = R_S0[2 + k].lx;//伸ばす先のX座標を決定
						Step[i].AfterY = R_S0[2 + k].ly;//伸ばす先のY座標を決定
						Step[i].AfterZ = R_S0[2 + k].lz;//伸ばす先のZ座標を決定

						for (l = i; l <= STEP; l++)
						{
							if (strcmp(Step[l].Motion, "運ぶ") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//次の動作が"運ぶ"且つ用いる手が一致した時
							{
								Step[l].BeforeX = Step[i].AfterX;//運ぶ前のX座標を決定
								Step[l].BeforeY = Step[i].AfterY;//運ぶ前のY座標を決定
								Step[l].BeforeZ = Step[i].AfterZ;//運ぶ前のZ座標を決定
								break;
							}
						}
					}
					else//単部品ではないとき＝治具上の部品を掴み取るor仮置き場(複数治具or単独治具)の部品を掴み取る
					{
						for (int Jig = 1; Jig <= R_H.J; Jig++)
						{
							tempJig = atoi(Step[i + 1].BeforeJig);
							if (R_S0[Beforejignum + Jig].N == tempJig)
							{
								Step[i].AfterX = R_S0[Beforejignum + Jig].lx;//伸ばす先のX座標を決定
								Step[i].AfterY = R_S0[Beforejignum + Jig].ly;//伸ばす先のY座標を決定
								Step[i].AfterZ = R_S0[Beforejignum + Jig].lz;//伸ばす先のY座標を決定
								flag = 1;
								break;
							}
						}
						if (flag == 0)//仮置き場にある部品を掴み取るとき
						{
							for (int beforedousa = i; beforedousa >= 1; beforedousa--)
							{
								if ((strcmp(Step[beforedousa].Motion, "置き放す") == 0 && Step[i + 1].KoP == Step[beforedousa].KoP)
									|| (strcmp(Step[beforedousa].Motion, "組立てる") == 0 && Step[i + 1].KoP == Step[beforedousa].Assy12))//以前の動作から("置き放す"かつ対象物が同一のもの)||("組立てる"かつ対象物が同一のもの)を探す
								{
									Step[i].AfterX = Step[beforedousa - 1].AfterX;//置き放す前の運ぶのBと一致
									Step[i].AfterY = Step[beforedousa - 1].AfterY;
									Step[i].AfterZ = Step[beforedousa - 1].AfterZ;
									break;
								}
							}
						}
						flag = 0;
						for (l = i; l <= STEP; l++)
						{
							if (strcmp(Step[l].Motion, "運ぶ") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//次の動作が"運ぶ"且つ用いる手が一致した時
							{

								Step[l].BeforeX = Step[i].AfterX;//運ぶ前のX座標を決定
								Step[l].BeforeY = Step[i].AfterY;//運ぶ前のY座標を決定
								Step[l].BeforeZ = Step[i].AfterZ;//運ぶ前のY座標を決定
								break;
							}
						}
					}
				}
			}
			else if (strcmp(Step[i + 1].Motion, "掴む") == 0)//次の動作が"掴む"のとき
			{
				if (strcmp(Step[i + 2].Motion, "外す") == 0)//次の次の動作が"外す"のとき
				{
					for (int Jig2 = 1; Jig2 <= R_H.J; Jig2++)
					{
						tempJig = atoi(Step[i + 2].BeforeJig);
						if (R_S0[Beforejignum + Jig2].N == tempJig)
						{
							Step[i].AfterX = R_S0[Beforejignum + Jig2].lx;//伸ばす先のX座標を決定
							Step[i].AfterY = R_S0[Beforejignum + Jig2].ly;//伸ばす先のY座標を決定
							Step[i].AfterZ = R_S0[Beforejignum + Jig2].lz;//伸ばす先のZ座標を決定
							break;
						}
					}
					for (l = i; l <= STEP; l++)
					{
						if (strcmp(Step[l].Motion, "運ぶ") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//次の動作が"運ぶ"且つ用いる手が一致した時
						{
							Step[l].BeforeX = Step[i].AfterX;//運ぶ前のX座標を決定
							Step[l].BeforeY = Step[i].AfterY;//運ぶ前のY座標を決定
							Step[l].BeforeZ = Step[i].AfterZ;//運ぶ前のZ座標を決定
							break;
						}
					}
				}
				else//工具を掴むまたは工具組付けで部品を支えるとき
				{
					for (int Kougu = 1; Kougu <= R_H.T; Kougu++)
					{
						if (Step[i + 1].KoP == R_S0[Beforekougunum + Kougu].N)//工具を掴むとき
						{
							flag = 1;
							Step[i].AfterX = R_S0[Beforekougunum + Kougu].lx;//伸ばす先のX座標を決定
							Step[i].AfterY = R_S0[Beforekougunum + Kougu].ly;//伸ばす先のY座標を決定
							Step[i].AfterZ = R_S0[Beforekougunum + Kougu].lz;//伸ばす先のZ座標を決定
							break;
						}
					}
					if (flag == 0)//工具組付けで部品を支えるとき
					{
						for (int beforedousa = i; beforedousa >= 1; beforedousa--)
						{
							if (strcmp(Step[beforedousa].Motion, "掴み取る") == 0 && Step[beforedousa].KoP == Step[i + 1].KoP)//対象としている掴み取るを探す
							{
								Step[i].AfterX = Step[beforedousa + 1].AfterX;
								Step[i].AfterY = Step[beforedousa + 1].AfterY;
								Step[i].AfterZ = Step[beforedousa + 1].AfterZ;
							}
						}
					}
					for (l = i + 1; l <= STEP; l++)
					{
						if ((strcmp(Step[l].Motion, "運ぶ") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0) || (strcmp(Step[l].Motion, "伸ばす") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0))//次の動作が"運ぶ"且つ用いる手が一致した時
						{
							Step[l].BeforeX = Step[i].AfterX;//運ぶ前のX座標を決定
							Step[l].BeforeY = Step[i].AfterY;//運ぶ前のY座標を決定
							Step[l].BeforeZ = Step[i].AfterZ;//運ぶ前のZ座標を決定
							break;
						}
					}
				}
			}
			else if (strcmp(Step[i + 1].Motion, "付ける") == 0)//次の動作が"付ける"のとき
			{
				for (k = 1; k <= N; k++)
				{
					if (Step[i + 1].KoP == k)//掴み取るものが単部品だったら
					{
						Step[i].AfterX = R_S0[2 + k].lx;//伸ばす先のX座標を決定
						Step[i].AfterY = R_S0[2 + k].ly;//伸ばす先のY座標を決定
						Step[i].AfterZ = R_S0[2 + k].lz;//伸ばす先のZ座標を決定

						for (l = i; l <= STEP; l++)
						{

							if (strcmp(Step[l].Motion, "運ぶ") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//次の動作が"運ぶ"且つ用いる手が一致した時
							{
								Step[l].BeforeX = Step[i].AfterX;//運ぶ前のX座標を決定
								Step[l].BeforeY = Step[i].AfterY;//運ぶ前のY座標を決定
								Step[l].BeforeZ = Step[i].AfterZ;//運ぶ前のZ座標を決定
								break;
							}
						}
					}
				}
			}
			else if (strcmp(Step[i + 1].Motion, "伸ばす") == 0)//次の動作が"伸ばす"のとき
			{
				//iとi+1の伸ばすのBを初期位置にする

				break;
			}
		}
		else if (strcmp(Step[i].Motion, "運ぶ") == 0)//動作が"運ぶ"のとき
		{
			for (j = i; j <= STEP; j++)//"持ち直す"用
			{
				if (strcmp(Step[j].Motion, "置き放す") == 0)//次に出てくる動作が"置き放す"のとき
				{
					if (Step[j].KoP == -10)
					{
						for (int Kougu = 1; Kougu <= R_H.T; Kougu++)
						{
							if (Step[j].KoP == R_S0[Beforekougunum + Kougu].N)
							{
								Step[i].AfterX = R_S0[Beforekougunum + Kougu].lx;//伸ばす先のX座標を決定
								Step[i].AfterY = R_S0[Beforekougunum + Kougu].ly;//伸ばす先のY座標を決定
								Step[i].AfterZ = R_S0[Beforekougunum + Kougu].lz;//伸ばす先のY座標を決定
								break;
							}
						}
					}
					else
					{
						if (strcmp(Step[j + 1].Motion, "伸ばす") == 0 && strcmp(Step[j + 2].Motion, "伸ばす") == 0)//完成品箱に置き放す
						{
							Step[i].AfterX = Ray.finish_centerX;//運んだ後のX座標を決定
							Step[i].AfterY = Ray.finish_centerY;//運んだ後のY座標を決定
							Step[i].AfterZ = Ray.finish_centerZ;//運んだ後のZ座標を決定
						}
						else//仮置き場に置き放すとき
						{
							Step[i].AfterX = Ray.karioki_centerX;//運んだ後のX座標=一番左の治具の左側に配置
							Step[i].AfterY = Ray.karioki_centerY;//運んだ後のY座標を決定
							Step[i].AfterZ = Ray.karioki_centerZ;//運んだ後のZ座標を決定
						}

					}
					for (l = i; l <= STEP; l++)
					{
						if (strcmp(Step[l].Motion, "伸ばす") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//次の動作が"伸ばす"且つ用いる手が一致した時
						{
							Step[l].BeforeX = Step[i].AfterX;//運ぶ前のX座標を決定
							Step[l].BeforeY = Step[i].AfterY;//運ぶ前のY座標を決定
							Step[l].BeforeZ = Step[i].AfterZ;//運ぶ前のY座標を決定
							break;
						}
					}
					break;
				}
				else if (strcmp(Step[j].Motion, "付ける") == 0)//次に出てくる動作が"付ける"のとき
				{
					
					for (int k = 1; k <= N - 1; k++)
					{
						if (atoi(Step[j].BeforeJig) == R_KJUNJO.Jig[k])//組立順序から治具を検索する
						{

							if (R_KJUNJO.Dir[k] == 1)//治具に付く部品の方向がX方向の時
							{
								if (Step[j].KoP <= N)//治具に付く部品が単部品の時
								{
									for (int B1 = 1; B1 <= N; B1++)
									{
										if (Step[j].KoP == R_B[B1].Bu_num)//治具に付く部品を探す
										{
											temp_Buhin = R_B[B1].Xkotei / 2;

											break;
										}
									}
								}
								else//治具に付く部品が中間製品の時
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
							else if (R_KJUNJO.Dir[k] == 2)//治具に付く部品の方向がY方向の時
							{
								if (Step[j].KoP <= N)//治具に付く部品が単部品の時
								{
									for (int B1 = 1; B1 <= N; B1++)
									{
										if (Step[j].KoP == R_B[B1].Bu_num)//治具に付く部品を探す
										{
											temp_Buhin = R_B[B1].Ykotei / 2;

											break;
										}
									}
								}
								else//治具に付く部品が中間製品の時
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
							else//治具に付く部品の方向がZ方向の時
							{
								if (Step[j].KoP <= N)//治具に付く部品が単部品の時
								{
									for (int B1 = 1; B1 <= N; B1++)
									{
										if (Step[j].KoP == R_B[B1].Bu_num)//治具に付く部品を探す
										{
											temp_Buhin = R_B[B1].Zkotei / 2;

											break;
										}
									}
								}
								else//治具に付く部品が中間製品の時
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
							Step[i].AfterX = R_S0[Beforejignum + Jig3].lx;//運んだ後のX座標を決定
							Step[i].AfterY = R_S0[Beforejignum + Jig3].ly;//運んだ後のY座標を決定
							Step[i].AfterZ = R_S0[Beforejignum + Jig3].lz + R_S0[Beforejignum + Jig3].Bz + temp_Buhin;//運んだ後のZ座標を決定★
							break;
						}
					}
					if (flag == 0)//工具に部品を付けるとき
					{
						for (int m = i - 1; m >= 1; m--)
						{
							if (strcmp(Step[m].Motion, "運ぶ") == 0)
							{
								temp = Step[m].AfterZ;
								break;
							}
						}
						for (k = j; k <= STEP; k++)
						{
							if (strcmp(Step[k].Motion, "組立てる") == 0)
							{
								for (int Jig4 = 1; Jig4 <= R_H.J; Jig4++)
								{
									if (R_S0[Beforejignum + Jig4].N == Step[k].Case)//"組立てる"治具を探しにいく
									{
										Step[i].AfterX = R_S0[Beforejignum + Jig4].lx;//運んだ後のX座標を決定
										Step[i].AfterY = R_S0[Beforejignum + Jig4].ly;//運んだ後のY座標を決定
										Step[i].AfterZ = temp;//運んだ後のZ座標を決定
										break;
									}
								}
								break;
							}
						}
					}
					for (l = i; l <= STEP; l++)
					{
						if (strcmp(Step[l].Motion, "伸ばす") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//次の動作が"伸ばす"且つ用いる手が一致した時
						{
							Step[l].BeforeX = Step[i].AfterX;//伸ばす前のX座標を決定
							Step[l].BeforeY = Step[i].AfterY;//伸ばす前のY座標を決定
							Step[l].BeforeZ = Step[i].AfterZ;//伸ばす前のZ座標を決定
							break;
						}
					}
					flag = 0;
					break;
				}
				else if (strcmp(Step[j].Motion, "組立移動する") == 0)//次に出てくる動作が"組立移動する"のとき
				{
					for (int Jig4 = 1; Jig4 <= R_H.J; Jig4++)
					{
						tempJig = atoi(Step[j + 1].Sup4);
						if (Step[j + 1].flag == 1)//工具組付けのとき
						{
							if (R_S0[Beforejignum + Jig4].N == Step[j + 1].Case)//"組立てる"治具を探しにいく
							{
								Step[i].AfterX = R_S0[Beforejignum + Jig4].lx - Step[Step[j + 1].Assy2].X_Dif;//運んだ後のX座標を決定
								Step[i].AfterY = R_S0[Beforejignum + Jig4].ly - Step[Step[j + 1].Assy2].Y_Dif;//運んだ後のY座標を決定
								Step[i].AfterZ = R_S0[Beforejignum + Jig4].lz - Step[Step[j + 1].Assy2].Z_Dif + R_S0[Beforejignum + Jig4].Bz + temp_Buhin;//運んだ後のZ座標を決定★

								//temp_Buhin = 0.0;
								//Step[i].RIchiX2 = Step[i].RIchiX1;
								//Step[i].RIchiY2 = Step[i].RIchiX1;
								break;
							}
						}
						else
						{
							if (R_S0[Beforejignum + Jig4].N == tempJig)//"組立てる"治具を探しにいく
							{
								Step[i].AfterX = R_S0[Beforejignum + Jig4].lx - Step[Step[j + 1].Assy2].X_Dif;//運んだ後のX座標を決定
								Step[i].AfterY = R_S0[Beforejignum + Jig4].ly - Step[Step[j + 1].Assy2].Y_Dif;//運んだ後のY座標を決定
								Step[i].AfterZ = R_S0[Beforejignum + Jig4].lz - Step[Step[j + 1].Assy2].Z_Dif + R_S0[Beforejignum + Jig4].Bz + temp_Buhin;//運んだ後のZ座標を決定★
								//temp_Buhin = 0.0;
								//Step[i].RIchiX2 = Step[i].RIchiX1;
								//Step[i].RIchiY2 = Step[i].RIchiX1;
								break;
							}
						}
					}
					Step[j].BeforeX = Step[j].AfterX = Step[i].AfterX;//"組立移動する"のAPP,OFFSETの決定
					Step[j].BeforeY = Step[j].AfterY = Step[i].AfterY;
					Step[j].BeforeZ = Step[i].AfterZ;
					Step[j].AfterZ = Step[j].BeforeZ - Step[j].OFFSET;

					break;
				}
				else if (strcmp(Step[j].Motion, "伸ばす") == 0)//次に出てくる動作が"伸ばす"のとき（工具パターン2用）
				{
					for (l = j; l <= STEP; l++)
					{
						if (strcmp(Step[l].Motion, "組立てる") == 0)//次以降に出てくる"組立てる"を探す
						{
							for (int Jig4 = 1; Jig4 <= R_H.J; Jig4++)
							{
								if (R_S0[Beforejignum + Jig4].N == Step[l].Case)//"組立てる"治具を探しにいく
								{
									Step[i].AfterX = R_S0[Beforejignum + Jig4].lx;//運んだ後のX座標を決定
									Step[i].AfterY = R_S0[Beforejignum + Jig4].ly;//運んだ後のY座標を決定
									Step[i].AfterZ = R_S0[Beforejignum + Jig4].lz + 50;//運んだ後のZ座標を決定
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
		else if (strcmp(Step[i].Motion, "組立移動する") == 0)//動作が"組立移動する"のとき
		{
			for (l = i; l <= STEP; l++)
			{
				if (strcmp(Step[l].Motion, "伸ばす") == 0 && strcmp(Step[i].Hand, Step[l].Hand) == 0)//次の動作が"伸ばす"且つ用いる手が一致した時
				{
					Step[l].BeforeX = Step[i].AfterX;//伸ばす前のX座標を決定
					Step[l].BeforeY = Step[i].AfterY;//伸ばす前のY座標を決定
					Step[l].BeforeZ = Step[i].AfterZ;//伸ばす前のZ座標を決定
					break;
				}
			}
		}
	}
}

//動作距離変更用関数
void Kyori_Change(int STEP)
{
	for (int i = 1; i <= STEP; i++)
	{
		if (strcmp(Step[i].Motion, "伸ばす") == 0 || strcmp(Step[i].Motion, "運ぶ") == 0 || strcmp(Step[i].Motion, "組立移動する") == 0)//動作が"伸ばす"または"運ぶ"または"組立移動する"のとき
		{
			Step[i].Dousa_Kyori = sqrt((fabs(Step[i].AfterX - Step[i].BeforeX) * fabs(Step[i].AfterX - Step[i].BeforeX)) + (fabs(Step[i].AfterY - Step[i].BeforeY) * fabs(Step[i].AfterY - Step[i].BeforeY)) + (fabs(Step[i].AfterZ - Step[i].BeforeZ) * fabs(Step[i].AfterZ - Step[i].BeforeZ)));		//A,Bの座標情報から距離を導出
		}
	}
}

void Write_VFDL(int STEP, int* n, int vfdlnum)
{
	FILE* fp;
	char m16a4[STR_MAX];

	int F_Count = 0;

	//sprintf(a, "VFDL%d", VFDL_No);

	//出力先のフルパスをshimaに代入する

	strcpy(m16a4, OutPath);
	strcat(m16a4, Conv_Char(vfdlnum));
	strcat(m16a4, ".csv");

	/***出力するファイルを開く***/

	//  ここで、ファイルポインタを取得する 

	if ((fp = fopen(m16a4, "w")) == NULL)				// エラーの場合は通常、異常終了する 
	{
		exit(EXIT_FAILURE);
	}
	//ヘッダの出力
	fprintf(fp, "%d,%d,%d\n", R_H.W, R_H.L, R_H.P);							//作業者番号,ライン番号,工程番号
	fprintf(fp, "%d,%d,%d,%d,%d\n", R_H.H, R_H.Ht, R_H.V, R_H.C, R_H.S);		//ステップ数,有効ステップ数,速度指定,干渉チェック,状態番号
	fprintf(fp, "%d,%d,%d\n", R_H.PA, R_H.T, R_H.J);							//部品数,工具数,治具数
	fprintf(fp, "%d\n", R_H.MI);												//中間部品数
	fprintf(fp, "%d,%d,%d\n", R_H.MO, R_H.MO1, R_H.MO2);												//生成中間部品数
	fprintf(fp, "%d,%d,%d\n\n", R_H.TA, R_H.TR, R_H.WD);					//机の数,トレイの数,通過点数

	//初期位置の出力
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

			//**該当ステップまでの合計時間の計算**//
			if (j == 1)
			{
				R_C[j].STime = 0.0;

				R_C[j].STime = R_C[j].T;
			}
			else
			{
				R_C[j].STime = R_C[j].T + R_C[j - 1].STime;
			}
			//**該当ステップまでの合計距離の計算**//
			if (strcmp(Step[j].Motion, "伸ばす") == 0 || strcmp(Step[j].Motion, "運ぶ") == 0 || strcmp(Step[j].Motion, "組立移動する") == 0)//動作が"伸ばす"、"運ぶ"、"組立移動する"のとき
			{
				if (j == 1)
				{
					R_C[j].SKyori = 0.0;

					R_C[j].SKyori = Step[j].Dousa_Kyori;

					if (strcmp(Step[j].Hand, "左手") == 0)
					{
						L_Kyori = Step[j].Dousa_Kyori;
					}
					else if (strcmp(Step[j].Hand, "右手") == 0)
					{
						R_Kyori = Step[j].Dousa_Kyori;
					}
				}
				else
				{
					R_C[j].SKyori = R_C[j - 1].SKyori + Step[j].Dousa_Kyori;

					if (strcmp(Step[j].Hand, "左手") == 0)
					{
						L_Kyori = L_Kyori + Step[j].Dousa_Kyori;
					}
					else if (strcmp(Step[j].Hand, "右手") == 0)
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

			if (strcmp(Step[j].Motion, "伸ばす") == 0)
			{
				Reach_count++;
				fprintf(fp, "伸ばす %s を A から B へ\n", Step[j].Hand);

				fprintf(fp, "A,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].Motion, "運ぶ") == 0)
			{
				Bring_count++;
				fprintf(fp, "運ぶ %s で A から B へ\n", Step[j].Hand);

				fprintf(fp, "A,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].Motion, "組立移動する") == 0)
			{
				fprintf(fp, "組立移動する %s で APP から OFFSET へ\n", Step[j].Hand);

				fprintf(fp, "APP,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "OFFSET,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].Motion, "持ち直す") == 0)
			{
				Turn_count++;
				fprintf(fp, "持ち直す %d を %s で X から Y へ\n", Step[j].KoP, Step[j].Hand);

				fprintf(fp, "X,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "Y,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);

			}
			else if (strcmp(Step[j].Motion, "掴む") == 0)
			{
				Grab_count++;
				fprintf(fp, "掴む %d を %s で %d により\n", Step[j].KoP, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].Motion, "掴み取る") == 0)
			{
				Grab_count++;
				fprintf(fp, "掴み取る %d を %s から %s で %d により\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].Motion, "放す") == 0)
			{
				Release_count++;
				fprintf(fp, "放す %d を %s で %d により\n", Step[j].KoP, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].Motion, "置き放す") == 0)
			{
				Leave_count++;
				fprintf(fp, "置き放す %d を %s に %s で %d により\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].Motion, "付ける") == 0)
			{
				Attach_count++;
				fprintf(fp, "付ける %d を %s に %s で\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand);
			}
			else if (strcmp(Step[j].Motion, "外す") == 0)
			{
				Remove_count++;
				fprintf(fp, "外す %d を %s から %s で\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand);
			}
			else if (strcmp(Step[j].Motion, "先端を変更する") == 0)
			{
				fprintf(fp, "先端を変更する %s の位置を %s に\n", Step[j].Hand, Step[j].Sentan_Num);
			}
			else if (strcmp(Step[j].Motion, "組立てる") == 0)
			{
				if (Step[j].flag == 1)//工具のとき
				{
					fprintf(fp, "組立てる %s,%s の %d と %s の %d を %d,%d,%s で %s,%d,%s の %d にする\n", Step[j].Sup1, Step[j].Sup1_2, Step[j].Assy1, Step[j].Sup2, Step[j].Assy2, Step[j].Jig, Step[j].angle_num, Step[j].Hand, Step[j].Sup3, Step[j].Case, Step[j].Sup4, Step[j].Assy12);
				}
				else
				{
					fprintf(fp, "組立てる %s の %d と %s の %d を ", Step[j].Sup1, Step[j].Assy1, Step[j].Sup2, Step[j].Assy2);

					if (strcmp(Step[j].Hand, "右手＿左手") != 0)
					{

						fprintf(fp, "%d,%s ", Step[j].Jig, Step[j].Hand);

						fprintf(fp, "で %s,%s の %d にする\n", Step[j].Sup3, Step[j].Sup4, Step[j].Assy12);
					}
					else
					{
						fprintf(fp, "%s で %s,%s の %d にする\n", Step[j].Hand, Step[j].Sup3, Step[j].Sup4, Step[j].Assy12);
					}
				}
			}
			else if (strcmp(Step[j].Motion, "原点を変更する") == 0)
			{
				fprintf(fp, "原点を変更する %d を 原点 にする\n", Step[j].KoP);

				fprintf(fp, "原点,%d,%d,%d\n", Step[j].Genten0X, Step[j].Genten0Y, Step[j].Genten0Z);
			}
			else if (strcmp(Step[j].Motion, "検査する") == 0)
			{
				fprintf(fp, "検査する %d を %d で\n", Step[j].KoP, Step[j].Color);
			}
			else if (strcmp(Step[j].Motion, "記入する") == 0)
			{
				fprintf(fp, "記入する %s を %d に %d で\n", Step[j].Kensa_Cont, Step[j].Kensa_Table, Step[j].Pen);
			}
			else if (strcmp(Step[j].Motion, "歩く") == 0)
			{
				fprintf(fp, "歩く %s から %s へ %d から %d へ 歩幅 %s で %s 向きに ", Step[j].Place1, Step[j].Place2, Step[j].angle1, Step[j].angle2, Step[j].S_M_L, Step[j].F_B_S);
				//通過点が存在するかどうか
				if (Step[j].Tukaten[0] != '\0')
				{
					for (int m = 0; m <= n[j]; m++)
					{
						if (m == n[j])
						{
							fprintf(fp, "%d を通って\n", Step[j].Tukaten[m]);
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
			else if (strcmp(Step[j].Motion, "階段") == 0)
			{
				fprintf(fp, "階段 を %d 段 %s 高さ A 奥行 B で %s 向きに\n", Step[j].Dansuu, Step[j].updown, Step[j].frontback);

				fprintf(fp, "A,%4.2lf,%4.2lf,%4.2lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%4.2lf,%4.2lf,%4.2lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].Motion, "回る") == 0)
			{
				fprintf(fp, "回る %d から %d へ\n", Step[j].angle1, Step[j].angle2);
			}
			else if (strcmp(Step[j].Motion, "座る") == 0)
			{
				fprintf(fp, "座る %d へ\n", Step[j].Zamen_height);
			}
			else if (strcmp(Step[j].Motion, "立つ") == 0)
			{
				fprintf(fp, "立つ\n");
			}
			else if (strcmp(Step[j].Motion, "回転する") == 0)
			{
				fprintf(fp, "回転する %d を X から Y へ\n", Step[j].KoP);

				fprintf(fp, "X,%d,%d,%d\n", Step[j].angle1_X, Step[j].angle2_X, Step[j].angle3_X);

				fprintf(fp, "Y,%d,%d,%d\n", Step[j].angle1_Y, Step[j].angle2_Y, Step[j].angle3_Y);

			}
			else if (strcmp(Step[j].Motion, "元に戻す") == 0)
			{
				fprintf(fp, "元に戻す %s を\n", Step[j].sp_KoP);

				fprintf(fp, "%s,%d,%d,%d\n", Step[j].sp_KoP, Step[j].KoP1, Step[j].KoP2, Step[j].KoP3);
			}
			else if (strcmp(Step[j].Motion, "消す") == 0)
			{
				fprintf(fp, "消す 対象物 を\n");

				fprintf(fp, "対象物,%d,%d,%d,%d,%d\n", Step[j].KoP1, Step[j].KoP2, Step[j].KoP3, Step[j].KoP4, Step[j].KoP5);
			}
			else if (strcmp(Step[j].Motion, "ｶﾒﾗｱﾝｸﾞﾙ") == 0)
			{
				fprintf(fp, "ｶﾒﾗｱﾝｸﾞﾙ %d 距離 %4.1lf %s 切替える\n", Step[j].angle_num, Step[j].Kyori, Step[j].Syunzi_Keika);
			}
			else if (strcmp(Step[j].Motion, "透過する") == 0)
			{
				if ((strcmp(Step[j].Hand, "左手") != 0) || ((strcmp(Step[j].Hand, "右手") != 0)) || ((strcmp(Step[j].Hand, "左手＿右手") != 0)))
				{
					fprintf(fp, "透過する %d を 透過率 %d で\n", Step[j].Jig, Step[j].Tokaritsu);

				}
				else
				{
					fprintf(fp, "透過する %s を 透過率 %d で\n", Step[j].Hand, Step[j].Tokaritsu);

				}
			}
			else if (strcmp(Step[j].Motion, "ﾏｰｶｰ") == 0)
			{
				fprintf(fp, "ﾏｰｶｰ %d を 表示する\n", Step[j].marker_num);
			}
			else if (strcmp(Step[j].Motion, "円柱") == 0)
			{
				fprintf(fp, "円柱 %d を %s\n", Step[j].entyu_num, Step[j].Hyouzi_Kesu);
			}
		}
		else
		{
			if (strcmp(Step[j].M, "伸ばす") == 0)
			{
				fprintf(fp, "伸ばす %s を A から B へ\n", Step[j].Hand);

				fprintf(fp, "A,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].M, "運ぶ") == 0)
			{
				fprintf(fp, "運ぶ %s で A から B へ\n", Step[j].Hand);

				fprintf(fp, "A,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].M, "組立移動する") == 0)
			{
				fprintf(fp, "組立移動する %s で APP から OFFSET へ\n", Step[j].Hand);

				fprintf(fp, "APP,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "OFFSET,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].M, "持ち直す") == 0)
			{
				fprintf(fp, "持ち直す %d を %s で X から Y へ\n", Step[j].KoP, Step[j].Hand);

				fprintf(fp, "X,%.1lf,%.1lf,%.1lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "Y,%.1lf,%.1lf,%.1lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);

			}
			else if (strcmp(Step[j].M, "掴む") == 0)
			{
				fprintf(fp, "掴む %d を %s で %d により\n", Step[j].KoP, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].M, "掴み取る") == 0)
			{
				fprintf(fp, "掴み取る %d を %s から %s で %d により\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].M, "放す") == 0)
			{
				fprintf(fp, "放す %d を %s で %d により\n", Step[j].KoP, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].M, "置き放す") == 0)
			{
				fprintf(fp, "置き放す %d を %s に %s で %d により\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand, Step[j].HandShape);
			}
			else if (strcmp(Step[j].M, "付ける") == 0)
			{
				fprintf(fp, "付ける %d を %s に %s で\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand);
			}
			else if (strcmp(Step[j].M, "外す") == 0)
			{
				fprintf(fp, "外す %d を %s から %s で\n", Step[j].KoP, Step[j].BeforeJig, Step[j].Hand);
			}
			else if (strcmp(Step[j].M, "先端を変更する") == 0)
			{
				fprintf(fp, "先端を変更する %s の位置を %s に\n", Step[j].Hand, Step[j].Sentan_Num);
			}
			else if (strcmp(Step[j].M, "組立てる") == 0)
			{
				fprintf(fp, "組立てる %s の %d と %s の %d を ", Step[j].Sup1, Step[j].Assy1, Step[j].Sup2, Step[j].Assy2);

				if (strcmp(Step[j].Hand, "右手＿左手") != 0)
				{

					fprintf(fp, "%d,%s ", Step[j].Jig, Step[j].Hand);

					fprintf(fp, "で %s,%s の %d にする\n", Step[j].Sup3, Step[j].Sup4, Step[j].Assy12);
				}
				else
				{
					fprintf(fp, "%s で %s,%s の %d にする", Step[j].Hand, Step[j].Sup3, Step[j].Sup4, Step[j].Assy12);
				}
			}
			else if (strcmp(Step[j].M, "原点を変更する") == 0)
			{
				fprintf(fp, "原点を変更する %d を 原点 にする\n", Step[j].KoP);

				fprintf(fp, "原点,%d,%d,%d\n", Step[j].Genten0X, Step[j].Genten0Y, Step[j].Genten0Z);
			}
			else if (strcmp(Step[j].M, "検査する") == 0)
			{
				fprintf(fp, "検査する %d を %d で\n", Step[j].KoP, Step[j].Color);
			}
			else if (strcmp(Step[j].M, "記入する") == 0)
			{
				fprintf(fp, "記入する %s を %d に %d で\n", Step[j].Kensa_Cont, Step[j].Kensa_Table, Step[j].Pen);
			}
			else if (strcmp(Step[j].M, "階段") == 0)
			{
				fprintf(fp, "階段 を %d 段 %s 高さ A 奥行 B で %s 向きに\n", Step[j].Dansuu, Step[j].updown, Step[j].frontback);

				fprintf(fp, "A,%4.2lf,%4.2lf,%4.2lf\n", Step[j].BeforeX, Step[j].BeforeY, Step[j].BeforeZ);

				fprintf(fp, "B,%4.2lf,%4.2lf,%4.2lf\n", Step[j].AfterX, Step[j].AfterY, Step[j].AfterZ);
			}
			else if (strcmp(Step[j].M, "回る") == 0)
			{
				fprintf(fp, "回る %d から %d へ\n", Step[j].angle1, Step[j].angle2);
			}
			else if (strcmp(Step[j].M, "座る") == 0)
			{
				fprintf(fp, "座る %d へ\n", Step[j].Zamen_height);
			}
			else if (strcmp(Step[j].M, "立つ") == 0)
			{
				fprintf(fp, "立つ\n");
			}
			else if (strcmp(Step[j].M, "回転する") == 0)
			{
				fprintf(fp, "回転する %d を X から Y へ\n", Step[j].KoP);

				fprintf(fp, "X,%d,%d,%d\n", Step[j].angle1_X, Step[j].angle2_X, Step[j].angle3_X);

				fprintf(fp, "Y,%d,%d,%d\n", Step[j].angle1_Y, Step[j].angle2_Y, Step[j].angle3_Y);

			}
			else if (strcmp(Step[j].M, "元に戻す") == 0)
			{
				fprintf(fp, "元に戻す %s を\n", Step[j].sp_KoP);

				fprintf(fp, "%s,%d,%d,%d\n", Step[j].sp_KoP, Step[j].KoP1, Step[j].KoP2, Step[j].KoP3);
			}
			else if (strcmp(Step[j].M, "消す") == 0)
			{
				fprintf(fp, "消す 対象物 を\n");

				fprintf(fp, "対象物,%d,%d,%d,%d,%d\n", Step[j].KoP1, Step[j].KoP2, Step[j].KoP3, Step[j].KoP4, Step[j].KoP5);
			}
			else if (strcmp(Step[j].M, "ｶﾒﾗｱﾝｸﾞﾙ") == 0)
			{
				fprintf(fp, "ｶﾒﾗｱﾝｸﾞﾙ %d 距離 %4.1lf %s 切替える\n", Step[j].angle_num, Step[j].Kyori, Step[j].Syunzi_Keika);
			}
			else if (strcmp(Step[j].M, "透過する") == 0)
			{
				if ((strcmp(Step[j].Hand, "左手") != 0) || ((strcmp(Step[j].Hand, "右手") != 0)) || ((strcmp(Step[j].Hand, "左手＿右手") != 0)))
				{
					fprintf(fp, "透過する %d を 透過率 %d で\n", Step[j].Jig, Step[j].Tokaritsu);
				}
				else
				{
					fprintf(fp, "透過する %s を 透過率 %d で\n", Step[j].Hand, Step[j].Tokaritsu);

				}
			}
			else if (strcmp(Step[j].M, "ﾏｰｶｰ") == 0)
			{
				fprintf(fp, "ﾏｰｶｰ %d を 表示する\n", Step[j].marker_num);
			}
			else if (strcmp(Step[j].M, "円柱") == 0)
			{
				fprintf(fp, "円柱 %d を %s\n", Step[j].entyu_num, Step[j].Hyouzi_Kesu);
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
		fprintf(fp2, "順序番号,ステップ数,時間値,伸ばす,運ぶ,掴み取る/掴む,放す,置き放す,付ける,外す,持ち直す,伸ばす,運ぶ,掴み取る/掴む,放す,置き放す,付ける,外す,持ち直す,移動距離,左手移動距離,右手移動距離,総回転角度");
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

	fprintf(fp2, "%d,%d,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%4.2lf,%d,%d,%d,%d,%d,%d,%d,%d,%4.2lf,%4.2lf,%4.2lf,%.1lf", c, R_H.H, R_C[STEP - 1].STime, STime_Reach, STime_Bring, STime_Grab, STime_Release, STime_Leave, STime_Attach, STime_Remove, STime_Turn, Reach_count, Bring_count, Grab_count, Release_count, Leave_count, Attach_count, Remove_count, Turn_count, R_C[STEP - 1].SKyori, L_Kyori, R_Kyori, All_Angle);//順序数,時間値

	for (int j = 1; j <= R_H.J; j++)
	{
		if (j == countJ)//用いる治具数と一致したら
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
	// 文字列の最後から文字を検索する
	// 【引数】
	// const char *s　：　検索対象文字列
	// int c　：　検索文字
	// 【戻り値】
	// 見つかったとき　：　一致文字のアドレス
	// 見つからなかったとき　：　NULL

	for (i = 0; i <= (p - S); i++) {
		NP[i] = S[i];
	}
	NP[i] = '\0'; // 最後にNULLを付加
}

//起動時のパスワードを確認するための関数
int PWCheck(int N, char* S)
{
	char pw[PWNum + 1];
	PWG(pw); //パスワードを生成してpwに代入する

	if (N >= 2)
	{           /*引数が2未満ならパスワードはないので起動しない (= return 0) */
		if (strncmp(S, pw, PWNum) == 0)
		{	/*パスワードが一致したら起動する (= return 1) */
			return 1;
		}
	}
	return 0; /*起動しない (= return 0) */
}

/******************************************************************************
'パスワード生成ルール
'実行時のyyyymmdd → ddyyyymm
'dd を3で割った余りを r として
'ddyyyymmのn文字目を、
'ASCIIコード表で
'10 + r + 3 * (n - 1)
'だけずらした文字に変換する
'******************************************************************************/
void PWG(char* s)
{
	time_t timer;
	struct tm* date;
	char str[PWNum + 1]; //8文字 + null

	int i, r;

	timer = time(NULL);          /* 経過時間を取得 */
	date = localtime(&timer);    /* 経過時間を時間を表す構造体 date に変換 */

	//rの計算 : 日付 → 3で割ったあまり
	strftime(str, PWNum + 1, "%d", date);
	r = atoi(str) % SFTN;

	//PW生成 : 日付 → パスワード
	strftime(str, PWNum + 1, "%d%Y%m", date);
	for (i = 0; i < PWNum; i++)
	{
		s[i] = str[i] + 10 + r + SFTN * i;
	}
	s[i] = '\0';
}

void ShowFinMsg(char* msg) {
	printf("%s\n", msg);
	printf("(Enterキーを押してください)\n");
	unsigned char key;
	while (1) {
		key = getch();
		if (key == 0x0d) break;
	}
}

//読み込んだ文字列の「,」（カンマ）を「 」（半角スペース）に置き換える関数
void Del_Com(char* s) {
	int	i;

	i = 0;
	while (s[i] != '\0') {
		if (s[i] == ',') s[i] = ' ';
		i++;
	}
}

//int型の変数をchar型の文字列に変換する関数
char* Conv_Char(int no) {
	char buf[10];
	sprintf(buf, "%d", no);
	return buf;
}

