#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <conio.h>

/*プロトタイプ宣言--------------------------------------------------------------------------------------------------------------------------------------------------*/
/*input情報を読み込む関数*/
void Read_kumitsuke_Dir();
void Read_buhin_zokusei();
void Read_tyukanseihin_zokusei();
void Read_kumituke();
void Read_Sessyoku();

void Make_jigmat();

/*両手用組付けリストを作成するメインの関数*/
void Make_BothHands_Kumitukelist(fp2);

/*両手用組付けリストに必要な関数*/
void Found_BH2Parts();
void Make_BH2Pair();
void Check_BH2Pair_Dir(int, int, char *);
void Check_BHPair_Kumituke();
void Write_BH_Kumilist_BH2(FILE *);
void Write_BH_Kumilist_BP(FILE *);
void Write_BH_Kumilist_BH1(FILE *);
void Write_BH_Kumilist_Tegumi(FILE *);
void Write_KouguParts(FILE *);

/*システムを動かす上で必要となる関数*/
void ShowFinMsg(char *);
void Del_Com(char *);//読み込んだ文字列の「,」（カンマ）を「 」（半角スペース）に置き換える関数
void GetNowPath(char *, char *);

/*定数の宣言--------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define STR_MAX 1000
#define DEF_PAR_FNAME "システムパラメータ.txt" 
#define FGETS_MAX_LENGTH 20000
#define MaxN 8000 //取り扱う最大の部品数を8000とする
#define MaxMM 1000000 //取り扱う最大の中間製品を1,000,000とする
#define MaxDN 300 //CSVファイル一行に記入される最大の文字数(部品数は64部品なので200用意する)
#define MaxKN 1000				//最大の組付数を1,000とする
#define Kumitsuke 0
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679
#define Flag_num 1
#define gosa 2

/*構造体の宣言------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
	int OyaKo_Flag;//0:親子変更不可、1:親子変更可
}kumitoku1[2000][MaxN];

struct kumituke2_tokusei
{
	int Num;
	int Oya_Parts;
	int Ko_Parts;
	int OyaKo_Flag;//0:親子変更不可、1:親子変更可
}kumitoku2[2000][MaxN];

struct BigProducts
{
	int Num;
	int Oya;
	int Ko;
	int sagyou_num;//親部品子部品どちらが大物なのか

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
	int k_num;				//接触線番号
	int p1;					//部品1
	int p2;					//部品2
	int p3;					//組立部品
	int c1[MaxN];			//部品1の構成部品
	int c2[MaxN];			//部品2の構成部品
	int c3[MaxN];			//部品2の構成部品
	int flug[MaxN];			//接触線のフラグ
	int Sessyoku;			//使用されている接触線番号の最小値
	char Dir[3];			//各組付けの組付け方向
	int CS_L[MaxN];			//組付けごとの接触有効空間の評価(幅)
	int CS_H[MaxN];			//組付けごとの接触有効空間の評価(高さ)
	int Contact_Space_L;	//組付けごとの接触有効空間の評価(幅)
	int Contact_Space_H;	//組付けごとの接触有効空間の評価(高さ)
	int Contact_Space;		//接触有効空間の評価
	int Special_Assembly;	//異種組付けの評価
	int Assembly_Power;		//組付け力の評価
	int Asymmetric;			//特定姿勢位置決めの評価
	int Contact_Point;		//接触点

}A_Kumituke[MaxKN];

//グローバル変数の宣言
char SEIHIN_Name[STR_MAX], inPath_kumitsuke_Dir[STR_MAX], inPath_jigmat[STR_MAX], outDir[STR_MAX], inPath_buhin_zokusei[STR_MAX], inPath_buhin_tokusei[STR_MAX], inPath_sessyokusen_tokusei[STR_MAX], inPath_kumitsuke_list[STR_MAX], inPath_tyukan_zokusei[STR_MAX], inPath_input[STR_MAX];
char inPath_kumitsukeyouin[STR_MAX], inPath_buhinyouin[STR_MAX], inPath_kumituke[STR_MAX],inPath_Sessyoku[STR_MAX];
int buhinsu = 0, sessyokusensu = 0, kumitsukesu = 0, tyukansu = 0;//部品数、接触線の総数、組付けの総数、中間製品の総数
int BP_count = 0, BH1_count = 0, Tegumi_count = 0, BH2_count = 0;
int /*BP[MaxMM], */BH1[MaxMM], Tegumi[MaxMM], BH2[MaxMM];//各両手作業に分類された組付けの格納庫
int BH2_Parts[MaxN]/*やりづらさがない両手2部品候補の部品群*/, BH2P_count = 0/*やりづらさがない両手2部品候補の部品のカウント*/;
int SPair_count/*同時に付ける接触線のペアをカウント*/, SPair_count_sum = 0/*接触線のペアの総数*/;
int kouguparts_num = 0;//工具を用いる部品の数
int B_youinsu = 0;//部品特性要因評価の数
int mode = 0;
int kumi = 0;//組付けの数
int N;//部品数
int K;//組付け数
int S;//接触線数
int File_Name;			//導出形式（網羅：1、部品群：2、分解法：3）


int main(int argc, char *argv[])
{
	FILE *fp1;//メモ帳
	char NowPath[STR_MAX]; //最終的にパラメータファイルのフルパスを入れる
	int i = 0;

	//このプログラムのあるパスを取得して、NowPathに入れる
	GetNowPath(NowPath, argv[0]);
	strcat(NowPath, "Input\\");	//パラメータファイルの置き場は.exeがあるフォルダ下のInputとする
	strcat(NowPath, DEF_PAR_FNAME); //NowPath に パラメータファイル名を結合して、フルパスにする

/************************************************************************************************/
/*パラメータファイルを開く*/
/************************************************************************************************/
	// パラメータファイルの仕様
	//  1行目：製品名
	//  2行目：ファイルの導出形式
	//  3行目：入力するフォルダのパス
	//  4行目：出力するフォルダのパス

	fp1 = fopen(NowPath, "r");
	if (fp1 == NULL) ShowFinMsg("パラメータファイルがありません"); // ファイルを開けなければ終了
	if (fscanf(fp1, "%s", SEIHIN_Name) == EOF) ShowFinMsg("製品名が不正です");
	if (fscanf(fp1, "%d", &File_Name) == EOF) ShowFinMsg("導出形式を入力してください");
	if (fscanf(fp1, "%s", inPath_input) == EOF) ShowFinMsg("inputのアドレスが不正です");
	sprintf(inPath_buhin_zokusei, "%s\\%s_部品属性.csv", inPath_input, SEIHIN_Name);
	//sprintf(inPath_kumitsuke_Dir, "%s\\%s_接触関係_組立位置情報.csv", inPath_input, SEIHIN_Name);
	//sprintf(inPath_kumitsuke_list, "%s\\%s_組付表.csv", inPath_input, SEIHIN_Name);

	sprintf(inPath_Sessyoku, "%s\\%s_接触関係_組立位置情報.csv", inPath_input, SEIHIN_Name);

	strcpy(inPath_kumituke, inPath_input);
	strcat(inPath_kumituke, "\\");
	strcat(inPath_kumituke, SEIHIN_Name);

	if (File_Name == 1)//網羅のとき
	{
		strcat(inPath_kumituke, "_網羅_組付け.csv");
	}
	else if (File_Name == 2)//部品群のとき
	{
		strcat(inPath_kumituke, "_部品群_組付け.csv");
	}
	else if (File_Name == 3)//分解のとき
	{
		strcat(inPath_kumituke, "_分解_組付け.csv");
	}


	
	if (fscanf(fp1, "%s", outDir) == EOF) ShowFinMsg("出力先フォルダのパスが不正です");
	fclose(fp1);
	//各ファイルを読み込む
	printf("インプットファイル%s\n", inPath_Sessyoku);
	//puts("インプットファイル　接触関係_組立位置情報");
	Read_Sessyoku();

	printf("インプットファイル%s\n", inPath_buhin_zokusei);
	//puts("インプットファイル　部品属性");
	Read_buhin_zokusei();
	//puts("組付け方向");
	//Read_kumitsuke_Dir();

	printf("インプットファイル%s\n", inPath_kumituke);
	//puts("インプットファイル　組付け");
	Read_kumituke();
	

	//両手同時組付けリストを作成する
	Make_BothHands_Kumitukelist();
	//puts("アウトプットファイル　動作組付リストを作成しました");

}

/*各ファイルを読み込む関数------------------------------------------------------------------------------------------------------------------------------------------------------*/

void Read_Sessyoku()
{
	FILE* fp1;

	char* temp;
	
	temp = (char*)malloc(20000);

	int i = 0;

	fp1 = fopen(inPath_Sessyoku, "r");//接触関係組立位置情報のファイルを読み込みモードで開く
	if (fp1 == NULL) {
		if (i != 0) { printf("\n"); }
		ShowFinMsg("接触関係_組立位置情報のファイルを開けません。\nシステムを終了します。\n");	// ファイルを開けなければ終了
	}

	fgets(temp, FGETS_MAX_LENGTH, fp1);

	fgets(temp, FGETS_MAX_LENGTH, fp1);
	sscanf(temp, "%d", &S);//接触線数を読み込む


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
		/*部品番号、中心点、部品の大きさの読み取り*/
		sscanf(temp, "%d%s%s%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%lf%d%s%d%d%d%d%d", &c, &a, &b, &buhinzokusei[buhinsu].Parts, &buhinzokusei[buhinsu].centerX, &buhinzokusei[buhinsu].centerY, &buhinzokusei[buhinsu].centerZ, &buhinzokusei[buhinsu].lengthX, &buhinzokusei[buhinsu].lengthY, &buhinzokusei[buhinsu].lengthZ,
			&d, &e, &f, &g, &h, &i, &j, &buhinzokusei[buhinsu].Weight, &buhinzokusei[buhinsu].Tool,&a,&c,&buhinzokusei[buhinsu].KouguType,&c,&buhinzokusei[buhinsu].Okikata,&buhinzokusei[buhinsu].Atsukai);

		//printf("%d,%s,%s,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%d,%lf,%d\n", c, a, b, buhinzokusei[buhinsu].Parts, buhinzokusei[buhinsu].centerX, buhinzokusei[buhinsu].centerY, buhinzokusei[buhinsu].centerZ, buhinzokusei[buhinsu].lengthX, buhinzokusei[buhinsu].lengthY, buhinzokusei[buhinsu].lengthZ,
			//d, e, f, g, h, i, j, buhinzokusei[buhinsu].Weight, buhinzokusei[buhinsu].Tool);

		if (buhinzokusei[buhinsu].Tool != 0)//工具を用いる部品の時
		{
			KouguParts[kouguparts_num].parts = buhinzokusei[buhinsu].Parts;
			KouguParts[kouguparts_num].tool = buhinzokusei[buhinsu].Tool;
			KouguParts[kouguparts_num].okikata = buhinzokusei[buhinsu].Okikata;
			KouguParts[kouguparts_num].atsukai = buhinzokusei[buhinsu].Atsukai;
			KouguParts[kouguparts_num].kougutype = buhinzokusei[buhinsu].KouguType;
			kouguparts_num++;
		}

		buhinsu++;//部品数を記録
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


	fp1 = fopen(inPath_kumituke, "r");//接触関係組立位置情報のファイルを読み込みモードで開く
	if (fp1 == NULL) {
		if (i != 0) { printf("\n"); }
		ShowFinMsg("組付けのファイルを開けません。\nシステムを終了します。\n");	// ファイルを開けなければ終了
	}

	fgets(temp, FGETS_MAX_LENGTH, fp1);//1行読み込み

	int j = 1;

	while (fgets(temp, FGETS_MAX_LENGTH, fp1) != NULL)//組付けのファイルの最後まで繰り返す
	{
		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		s = temp;

		sscanf(s, "%d", &A_Kumituke[j].k_num); s = strchr(&s[1], ' ');	//組付け番号を読み込む	//空白の文字を探して、そこを文字列の開始点にする
		sscanf(s, "%d", &trash); s = strchr(&s[1], ' ');				//不要な情報を読み込む	//空白の文字を探して、そこを文字列の開始点にする
		sscanf(s, "%d", &A_Kumituke[j].p1); s = strchr(&s[1], ' ');	//部品1の部品番号を読み込む	//空白の文字を探して、そこを文字列の開始点にする
		sscanf(s, "%d", &A_Kumituke[j].p2); s = strchr(&s[1], ' ');	//部品2の部品番号を読み込む	//空白の文字を探して、そこを文字列の開始点にする
		sscanf(s, "%d", &A_Kumituke[j].p3); s = strchr(&s[1], ' ');	//組立部品の部品番号を読み込む	//空白の文字を探して、そこを文字列の開始点にする

		//部品1の構成部品の読み込み
		for (int k = 0; k < N; k++)	//部品数繰り返す
		{
			sscanf(s, "%d", &A_Kumituke[j].c1[k]); s = strchr(&s[1], ' ');		//構成部品の部品番号を読み込む	//空白の文字を探して、そこを文字列の開始点にする
		}

		//部品2の構成部品の読み込み
		for (int k = 0; k < N; k++)	//部品数繰り返す
		{
			sscanf(s, "%d", &A_Kumituke[j].c2[k]); s = strchr(&s[1], ' ');		//構成部品の部品番号を読み込む	//空白の文字を探して、そこを文字列の開始点にする
		}

		//組立部品の構成部品の読み込み
		for (int k = 0; k < N; k++)	//部品数繰り返す
		{
			sscanf(s, "%d", &A_Kumituke[j].c3[k]); s = strchr(&s[1], ' ');		//構成部品の部品番号を読み込む	//空白の文字を探して、そこを文字列の開始点にする
		}

		//発生する接触線番号の読み込み

		for (int k = 1; k <= S; k++)	//接触線数繰り返す
		{
			sscanf(s, "%d", &A_Kumituke[j].flug[k]); s = strchr(&s[1], ' ');		//接触線の存在フラグをを読み込む	//空白の文字を探して、そこを文字列の開始点にす

			if (A_Kumituke[j].flug[k] == 1)	//存在フラグが「1」のとき
			{
				A_Kumituke[j].Sessyoku = k;	//接触線番号を代入

				break;	//for文を抜ける=最初に立っているフラグを格納
			}
		}

		j++;	//配列の値の追加
	}

	K = j;//組付け数の格納

	free(temp);
	fclose(fp1);
}

/*両手用組付けリストを作成する関数--------------------------------------------------------------------------------------------------------------------------------------------------*/
void Make_BothHands_Kumitukelist()
{
	FILE *fp;//出力ファイル
	char WkD[STR_MAX], *fname = WkD;
	int kouguflag = 0;

	//出力先のフルパスをWkDに代入する
	strcpy(WkD, outDir);
	strcat(WkD, "\\");
	strcat(WkD, SEIHIN_Name);
	strcat(WkD, "_動作組付けリスト.csv");

	fp = fopen(fname, "w");
	printf("アウトプットファイル　%s\n", fname);
	Write_KouguParts(fp);

	fclose(fp);

}

/*工具組付けを付与する関数--------------------------------------------------------------------------------------------------------------------------------------------------*/
void Write_KouguParts(fp)
{
	int i, j;
	//printf("手組組付けの候補を出力する（開始）\n");

	fprintf(fp, "工具\n");
	fprintf(fp, "存在フラグ,接触線番号,組付け番号,親部品,子部番号,工具名,工具パターン\n");
	//printf("組付けの数　%d　\n\n", K);
	for (i = 0; i < K; i++)
	{
		for (j = 0;j < kouguparts_num;j++)
		{
			if (A_Kumituke[i].p1 == KouguParts[j].parts)
			{
				fprintf(fp, "%d,%d,%d,%d,%d,%d,%d\n", Flag_num, A_Kumituke[i].Sessyoku, A_Kumituke[i].k_num, A_Kumituke[i].p2, A_Kumituke[i].p1, KouguParts[j].tool, -2);//工具パターンを2にする
			}
			else if (A_Kumituke[i].p2 == KouguParts[j].parts)
			{
				fprintf(fp, "%d,%d,%d,%d,%d,%d,%d\n", Flag_num, A_Kumituke[i].Sessyoku, A_Kumituke[i].k_num, A_Kumituke[i].p1, A_Kumituke[i].p2, KouguParts[j].tool, -2);//工具パターンを2にする
			}
		}
	}
}

//システムを動かす上で必要となる関数------------------------------------------------------------------------------------------------------------
void ShowFinMsg(char *msg) 
{
	int c;
	printf("%s\n", msg);
	printf("(Enterキーを押してください。)");
	c = getchar();
	exit(1);
}

void Del_Com(char *s)//読み込んだ文字列の「,」（カンマ）を「 」（半角スペース）に置き換える関数
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