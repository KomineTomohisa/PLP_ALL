#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<conio.h>

//定数の宣言 --------------------------------------------------
#define SysVer "Ver.1.10 2023/01/21"
#define PW "plp0831"
#define PWNum 7
#define M_PI 3.14159;
#define DEF_PAR_FNAME "PLP_ALL_S0作成システムパラメータ.txt"					//使用するパラメータファイルの名称
#define STR_MAX 500														//文字の最大の部品数を500とする
#define FGETS_MAX_LENGTH 20000											//文字の最大文字数を20000とする
#define MaxMM 50000														//取り扱う最大の組付けや動作の数を50000とする
#define MaxM 50000				//取り扱う最大の中間製品数を50000とする
#define Max_Junjo 200000
#define MaxN 8000				//取り扱う最大の部品数を8000とする
#define MaxKN 50000 
#define MaxKM 200


//パスワード関係
#define PWNum 8
#define SFTN 3	 //文字をシフトする数

char SEIHIN_Name[STR_MAX], outDir[STR_MAX];
char inPath_Input[STR_MAX];
char inPath_Input_Junjo[STR_MAX];
char inPath_Input_Buhin[STR_MAX];
char NowPath[STR_MAX];
char OutPath[STR_MAX];
int s_File;		//ファイルの最大番号
int D_model;    //導出形式
int N;						//読み込んだ製品の部品数
int K;			//工具数
int J;			//治具数


//プロトタイプ宣言 --------------------------------------------------

//必ず必要なもの

void GetNowPath(char* NP, char* S);		//現在のファイルを特定する関数
void ShowFinMsg(char*);					//文字を出力？する関数
void ReadPara(char* S);					//使用するパラメータファイルを読み込む関数
void Del_Com(char*);
char* Conv_Char(int);//int型の変数をchar型の文字列に変換する関数

//自作の関数
void Read_BZ();							//部品属性を読み込む関数
void Read_Junjo();					//組立順序を読み込む関数

int Jig_Count(int);						//治具の個数をカウント

void Write_S0();						//S0を書き出す関数

//パスワード//
int PWCheck(int, char*);
void PWG(char*);

//***構造体***//

//部品属性用の構造体
struct Buhin
{
	int Bu_num;//部番
	char Buhin_Name[STR_MAX];//部品名
	char CAD_file[STR_MAX];//CADファイル
	int Buhin_num;//部品番号
	double Center_X;//中心X
	double Center_Y;//中心Y
	double Center_Z;//中心Z
	double Length_X;//部品の長さ（X方向）
	double Length_Y;//部品の長さ（Y方向）
	double Length_Z;//部品の長さ（Z方向）
	double Xmin;
	double Xmax;
	double Ymin;
	double Ymax;
	double Zmin;
	double Zmax;
	int Color_num;//色番号
	double weight;//重量
	int kogu_num;//工具番号
	int kogu_name;//工具名
	int situkan;//質感
	int tokutyo;//特徴(1:ねじ、2:O、Cﾘﾝｸﾞ、3:Eﾘﾝｸﾞ、4:ばね、5:ﾅｯﾄ)
	int OK;
	int Size_Small;//小さい
	int Thin;//薄い
	int Size_Big;//大きい
	int Weight;//重い
	int Handling_Careful;//取扱注意部品
	int Shape_Change;//ハーネス
	int Misassembly_dir;//誤組付け注意（方向）
	int Misassembly_sim;//誤組付け注意（類似）
};
struct Buhin R_B[MaxN];

struct KJUNJO {
	long long kumij;			//組立順序番号
	int kuminum[MaxKN];	//組付け番号
	int p1[MaxKN];	//部品1
	int p2[MaxKN];	//部品2
	int p3[MaxKN];	//組立て部品
	char Oya_Pos1[MaxKN][3];
	char Oya_Pos2[MaxKN][3];
	int Jig[MaxKN];
	int KouguNum[MaxKN];
	int DousaNum[MaxKN];
};
struct KJUNJO R_KJUNJO;

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
	// 	2行目：組立順序ファイルの導出形式
	//  3行目：組立順序ファイルの最大番号
	//  4行目：入力するフォルダのパス
	//  5行目：出力するフォルダのパス


	fp1 = fopen(NowPath, "r");
	if (fp1 == NULL) ShowFinMsg("パラメータファイルがありません");				 // ファイルを開けなければ終了
	if (fscanf(fp1, "%s", SEIHIN_Name) == EOF) ShowFinMsg("製品名が不正です");
	if (fscanf(fp1, "%d", &D_model) == EOF) ShowFinMsg("導出形式を入力してください");
	if (fscanf(fp1, "%d", &s_File) == EOF) ShowFinMsg("ファイルの最大番号を入力してください");
	if (fscanf(fp1, "%s", inPath_Input) == EOF) ShowFinMsg("入力先フォルダのパスが不正です");
	if (fscanf(fp1, "%s", outDir) == EOF) ShowFinMsg("出力先フォルダのパスが不正です");
	fclose(fp1);

	//"部品属性"読み込み用パス作成
	strcpy(inPath_Input_Buhin, inPath_Input);
	strcat(inPath_Input_Buhin, "\\");
	strcat(inPath_Input_Buhin, SEIHIN_Name);
	strcat(inPath_Input_Buhin, "_部品属性.csv");

	//Output用パス作成
	strcpy(OutPath, outDir);
	strcat(OutPath, "\\");
	strcat(OutPath, SEIHIN_Name);

	//"組立順序"読み込み用パス作成
	strcpy(inPath_Input_Junjo, inPath_Input);
	strcat(inPath_Input_Junjo, "\\");
	strcat(inPath_Input_Junjo, SEIHIN_Name);

	if (D_model == 1)//網羅
	{
		strcat(inPath_Input_Junjo, "_網羅_動作付組立順序");
		strcat(OutPath, "_網羅_レイアウト.csv");
	}
	else if (D_model == 2)//部品群
	{
		strcat(inPath_Input_Junjo, "_部品群_動作付組立順序");
		strcat(OutPath, "_部品群_レイアウト.csv");
	}
	else//分解
	{
		strcat(inPath_Input_Junjo, "_分解_動作付組立順序");
		strcat(OutPath, "_分解_レイアウト.csv");
	}

	printf("製品名：%s\n", SEIHIN_Name);
	for (int j = 1; j <= 3; j++)
	{
		if (j == D_model)
		{
			if (j == 1)//網羅
			{
				printf("導出形式：網羅\n");
			}
			else if (j == 2)//部品群
			{
				printf("導出形式：部品群\n");
			}
			else//分解
			{
				printf("導出形式：分解\n");
			}
		}
	}
	printf("ファイルの最大番号：%d\n", s_File);
	printf("Input(組立順序)情報：%s\n", inPath_Input_Junjo);
	printf("Input(部品属性)情報：%s\n", inPath_Input_Buhin);
	printf("Output情報：%s\n\n", OutPath);

	//部品属性の読み込み
	Read_BZ();
	printf("部品属性読み込み完了\n");

	//順序評価ファイルの読み込み
	Read_Junjo();
	printf("組立順序読み込み完了\n");

	//S0作成関数
	Write_S0();
	printf("\nS0の作成が完了しました。\n");

	//終了の処理
	printf("(Enterキーを押してください)\n");
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

	int First_Time = 0;//始めは飛ばす
	int flg;			//フラグ
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

		fp1 = fopen(inPath_Input_Junjo, "r");						//NowPathのファイルを開いて、読み込む
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

					Jig = Jig_Count(Knumber);

					if (Jig >= temp_Jig)
					{
						temp_Jig = Jig;
					}

				}
				else //最初のKのとき
				{
					First_Time++;
				}

				j = 1;
				sscanf(temp, "%c%lld%d", &K, &R_KJUNJO.kumij, &flg);//K,組立順序番号,存在フラグの読み込み

			}
			else
			{
				sscanf(temp, "%d%d%d%d%s%s%d%d%d", &R_KJUNJO.kuminum[j], &R_KJUNJO.p1[j], &R_KJUNJO.p2[j], &R_KJUNJO.p3[j], R_KJUNJO.Oya_Pos1[j], R_KJUNJO.Oya_Pos2[j], &R_KJUNJO.Jig[j], &R_KJUNJO.KouguNum[j], &R_KJUNJO.DousaNum[j]);

				j++;
			}

		}

		fclose(fp1);

	}

	J = temp_Jig;//格納

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

	fp1 = fopen(inPath_Input_Buhin, "r");						//NowPathのファイルを開いて、読み込む
	if (fp1 == NULL)ShowFinMsg("部品属性がありません");		//ファイルを開けなければ終了

	//以降は、開けたファイルが部品属性である前提で処理をする

	//1行目の読み込み
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	//2行目の読み込み
	fgets(temp, FGETS_MAX_LENGTH, fp1);

	sscanf(temp, "%d", &N);

	for (int i = 1; i <= N; i++)
	{
		flag = 0;

		fgets(temp, FGETS_MAX_LENGTH, fp1);

		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		s = temp;

		sscanf(s, "%d%s%s%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%d%d%d%d%d%d", &R_B[i].Bu_num, &R_B[i].Buhin_Name, &R_B[i].CAD_file, &R_B[i].Buhin_num, &R_B[i].Center_X, &R_B[i].Center_Y, &R_B[i].Center_Z, &R_B[i].Length_X, &R_B[i].Length_Y, &R_B[i].Length_Z, &R_B[i].Xmin, &R_B[i].Xmax, &R_B[i].Ymin, &R_B[i].Ymax, &R_B[i].Zmin, &R_B[i].Zmax, &R_B[i].Color_num, &R_B[i].weight, &R_B[i].kogu_num, &R_B[i].kogu_name, &R_B[i].situkan, &R_B[i].tokutyo, &R_B[i].OK);

		//工具数カウント用の処理
		if (R_B[i].kogu_num != 0)
		{
			for (int j = i - 1; j >= 1; j--)
			{
				if (R_B[j].kogu_num == R_B[i].kogu_num)//今までにその工具が用いられているとき
				{
					flag = 1;
					break;
				}
			}
			if (flag == 0)//今までにその工具を一度も使用しないとき
			{
				temp_Kougu = R_B[i].kogu_num;
				count++;
			}
		}

	}
	K = count;//格納

	free(temp);

	fclose(fp1);
}

int Jig_Count(int Knumber)
{
	int temp;
	int count = 0;//治具数カウント用
	for (int i = 1; i <= Knumber; i++)
	{
		if (i == 1)
		{
			count++;
			temp = R_KJUNJO.Jig[i];
		}
		else
		{
			if (temp != R_KJUNJO.Jig[i])//治具が切り替わったとき
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

	int Jig = -100;//治具の基準変数
	int Tray = 9000;//トレイの基準変数
	int Kougu = -10;//工具の基準変数

	fp1 = fopen(OutPath, "w");						//Outputのファイルを開いて、書き込む

	//ヘッダの出力
	fprintf(fp1, "1\n");										
	fprintf(fp1, "1,1,1\n");										//作業者番号,ライン番号,工程番号
	fprintf(fp1, "100,100,2,0,0\n");								//ステップ数,有効ステップ数,速度指定,干渉チェック,状態番号
	fprintf(fp1, "%d,%d,%d\n", N, K, J);							//部品数,工具数,治具数
	fprintf(fp1, "0,0,0\n");										//中間部品数
	fprintf(fp1, "0,0,0\n");										//生成中間部品数
	fprintf(fp1, "1,%d,0\n\n", N + 1);								//机の数,トレイの数(部品数＋仮置き場),通過点数

	for (int i = 1; i <= 2 + N; i++)//部品の詳細の出力
	{
		if (i == 1)//右手
		{
			fprintf(fp1, "0,200,0,845,0,0,0\n");
		}
		else if (i == 2)//左手
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

	for (int i = 1; i <= J; i++)//治具の詳細の出力
	{
		fprintf(fp1, "%d,0,0,0,0,0,0,0,0,0,23,1\n",Jig-i);
	}
	
	fprintf(fp1, "8001,480,480,350,850,21,0,50,0,0\n");//机詳細の出力

	for (int i = 1; i <= N + 2; i++)//トレイ&仮置き場&完成品箱
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

//読み込んだ文字列の「 」（半角スペース）を「*」（アスタリスク）に置き換える関数
void Change_Space(char* s) {
	int	i;

	i = 0;
	while (s[i] != '\0') {
		if (s[i] == ' ') s[i] = '*';
		i++;
	}
}

//int型の変数をchar型の文字列に変換する関数
char* Conv_Char(int no) {
	char buf[10];
	sprintf(buf, "%d", no);
	return buf;
}