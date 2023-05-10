#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h>

//プロトタイプ宣言 --------------------------------------------------
void ShowFinMsg(char *);
void Del_Com(char *);
void GetNowPath(char *, char *);
void Del_Com(char *);
void Write_kumijun();
void Shuffle_oyako(FILE*);
void Read_sametime();
void Count_oyako_shuffle();

//定数の宣言 --------------------------------------------------
#define DEF_PAR_FNAME "システムパラメータ.txt" 
#define STR_MAX 1000
#define FGETS_MAX_LENGTH 20000
#define MaxN 1000 //取り扱う最大の部品数を64とする
#define MaxMM 1000000


//構造体の宣言 --------------------------------------------------
struct kumijun//組立順序一行ずつを入れておく構造体
{
	int kuminum;//組付け番号
	int oya;//親部品
	int ko;//子部品
	int tyukan;//できる中間製品
	char dir[10];//組付ける方向
	char XLineDir[3];
	char ZLineDir[3];
	int jig;//治具番号
	int base;//ベース部品
	int bothhands_type;//同時に組付ける組付けかどうか(１なら親部品が両手、２なら子部品が両手)
	int kougu;
}kumijun[MaxN];

struct kumihead//組立順序の組立順序番号などが書いてある行を入れておく構造体
{
	char K[2];//最初のK
	int KJnum;//組立順序番号
	int flag;//存在フラグ
	int x;
}kumihead;

struct same_time_parts//同時制約に書かれている部品番号を入れておく構造体
{
	int flag[2];//存在フラグ
	int sessyoku[2];
	int Num[2];//組付け番号
	int parts1[2];//同時に組付けたい部品1
	int parts2[2];//同時に組付けたい部品2
	int bothhands_type[2];//親子指定の情報
	int kougu;
}sametime;

/*struct checked_kumijun//連続した組立順序の2行の中に同時に組付ける部品が4つある組立順序で出力したものを保存しておくもの
{
	int sum_kuminum;//組付け番号の合計値
	int kuminum[MaxN];//各行の組付け番号
}check_kumij[100000];*/

struct sametime_kumituke_Pair
{
	int Pair_num1;//組付けのペアの数
	int Pair_num2;//
	//int sessyoku[MaxN][2];
	int Num[MaxN][2];//組付け番号
	int parts1[MaxN][2];//部品1
	int parts2[MaxN][2];//部品2
	int bothhands_type[MaxN][2];//親子関係
}kumitsuke_Pair[MaxN];

struct BigProducts
{
	//int sessyoku;
	int Num;//組付け番号
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

//グローバル変数の宣言 --------------------------------------------------
char SEIHIN_Name[STR_MAX], outDir[STR_MAX], inPath_kumijun[STR_MAX], inPath_douji[STR_MAX], inPath[STR_MAX],outPath[STR_MAX];
int douji[MaxN][3], seiyakusu = 0/*制約の数を記録しておく変数*/, gyousu = 0, change_oyako = 0, variation = 0, rearrange[MaxN/2]/*親子指定なしの同時に組付ける部品が64部品32組*/;
int Pair = 0, BPcount = 0, Supportcount = 0, Tegumicount = 0, Kougucount = 0;
int NewNOS0 = 0;
int tegumijig = 0;
int max_kumijun = 0;
int Filename = 0;
char dir_default= 'D';

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// プログラム部分
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int main(int argc, char *argv[])
{
	FILE *fp1;
	int parts1, parts2;
	char NowPath[STR_MAX]; //最終的にパラメータファイルのフルパスを入れる
	char MsgS[STR_MAX]; //メッセージ用、文字型配列
	double mode = 0;

	//このプログラムのあるパスを取得して、NowPathに入れる
	GetNowPath(NowPath, argv[0]);
	strcat(NowPath, "Input\\");	//パラメータファイルの置き場は.exeがあるフォルダ下のInputとする
	strcat(NowPath, DEF_PAR_FNAME); //NowPath に パラメータファイル名を結合して、フルパスにする

	/************************************************************************************************/
	/*パラメータファイルを開く*/
	/************************************************************************************************/
	// パラメータファイルの仕様
	//  1行目：製品名
	//  2行目：組付けファイル名を含めたフルパス
	//  3行目：組立順序のファイル名を含めたフルパス
	//  4行目：検索結果を出力するフォルダのパス（ファイル名は「検索結果.csv」とする）

	fp1 = fopen(NowPath, "r");
	if (fp1 == NULL) ShowFinMsg("パラメータファイルがありません"); // ファイルを開けなければ終了
	if (fscanf(fp1, "%s", SEIHIN_Name) == EOF) ShowFinMsg("製品名が不正です");
	//if (fscanf(fp1, "%d", &tegumijig) == EOF) ShowFinMsg("手組みを行う位置の情報が足りません");
	if (fscanf(fp1, "%d", &Filename) == EOF) ShowFinMsg("読み取るファイル名を記述してください");
	if (fscanf(fp1, "%d", &max_kumijun) == EOF) ShowFinMsg("読み取る組立順序ファイルの最大数を記述してください");
	if (fscanf(fp1, "%s", inPath) == EOF) ShowFinMsg("INPUTのパスが不正です");
	if (fscanf(fp1, "%s", outPath) == EOF) ShowFinMsg("OUTPATHのパスが不正です");
	fclose(fp1);

	//printf("読み取る最大のファイル番号は%dです\n", max_kumijun);

	//puts("部品数を入力してください");
	//scanf("%d", &N);

	Read_sametime();

	Write_kumijun();


	
	puts("出力完了");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// ユーティリティ
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void Write_kumijun()//組立順序を読み込む関数
{
	//puts("組立順序を検索");

	FILE*fp1;
	FILE*fp2;
	FILE*fp3;
	int i = 0, j = 0, k = 0, a = 0, b = 0, c = 0, d = 0, hantei = 0, totalhantei = 0, kanousu = 0, hanbetsu = 0, sametime[MaxN][3], tmp = 0;
	int karioki = 0, totalkarioki = 0, foregoing = 0/*過去に出力した組立順序の数*/, sum_samekuminum = 0/*組付け番号の合計値*/, already = 0/*既に出力した組立順序と同じときに1にする*/, same = 0/*既に同じ組付けが出てきているのかの判定*/;
	int tegumihantei = 0, o = 0, t = 0, s = 0, BPflag = 0, Tflag = 0, Sflag = 0, BH2flag = 0;/*大物、要援助、手組みに当てはまったかどうかの判定*/;
	char WkD[STR_MAX], WkD2[STR_MAX];
	char temp[FGETS_MAX_LENGTH];
	int kumijuncount = 0;
	int kougu_buhin;
	
	//出力先のフルパスをWkDに代入する

	for (kumijuncount = 0;kumijuncount <= max_kumijun;kumijuncount++)
	{
		//strcpy(WkD, outDir);
		//strcat(WkD, "\\");
		//strcat(WkD, SEIHIN_Name);
		if (Filename == 1)
		{
			sprintf(WkD, "%s\\%s_網羅_動作付組立順序%d.csv", outPath, SEIHIN_Name, kumijuncount);

			sprintf(WkD2, "%s\\%s_網羅_組立順序%d_J.csv", inPath, SEIHIN_Name, kumijuncount);
		}
		else if(Filename == 2)
		{
			sprintf(WkD, "%s\\%s_部品群_動作付組立順序%d.csv", outPath, SEIHIN_Name, kumijuncount);

			sprintf(WkD2, "%s\\%s_部品群_組立順序%d_J.csv", inPath, SEIHIN_Name, kumijuncount);
		}
		else if (Filename == 3)
		{
			sprintf(WkD, "%s\\%s_分解_動作付組立順序%d.csv", outPath, SEIHIN_Name, kumijuncount);
			
			sprintf(WkD2, "%s\\%s_分解_組立順序%d_J.csv", inPath, SEIHIN_Name, kumijuncount);
		}
		char* fname1 = WkD;
		char* fname2 = WkD2;
		fp1 = fopen(fname2, "r");//組立順序ファイルを読み込みモードで開く
		printf("インプットファイル　%s\n", fname2);
		if (fp1 != NULL)
		{
			fp2 = fopen(fname1, "w");//検索結果のファイルを書き込みモードで開く
			printf("アウトプットファイル　%s\n", fname1);
			//fp3 = fopen(fname2, "w");//検索結果のファイルを書き込みモードで開く

			fgets(temp, FGETS_MAX_LENGTH, fp1);
			Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
			sscanf(temp, "%s%d%d%d", &kumihead.K, &kumihead.KJnum, &kumihead.flag, &kumihead.x);

			while (fgets(temp, FGETS_MAX_LENGTH, fp1) != NULL)//ファイルの最後まで
			{
				if (strstr(temp, "K") != NULL)//読み込んだ行にKの文字があるとき
				{
				onemore:
					//工具を用いた組立かどうか
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
					for (i = 0; i < gyousu; i++)//2行ずつなので1つの組立順序の行数-1
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
											if (kumitsuke_Pair[j].bothhands_type[a][0] == 1 && kumitsuke_Pair[j].bothhands_type[b][1] == 1)//組付けが親部品、子部品を入れ替えても両手2部品組付けになる時
											{//前の作業が両手作業で、できた中間製品が次の組付けの子部品になるとき
												/*if ((kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[a][0]) || ((kumijun[i - 2].tyukan == kumitsuke_Pair[j].parts2[a][0] && kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[b][1]) && kumijun[i - 1].bothhands_type > 0))*/
												if ((kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[a][0] || (kumijun[i - 2].tyukan == kumitsuke_Pair[j].parts2[a][0] && kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[b][1])) && kumijun[i - 1].bothhands_type > 0)
												{
													kumijun[i].bothhands_type = 2;//子部品が両手
													kumijun[i + 1].bothhands_type = 2;
													//親子関係の入れ替え
													kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts2[a][0];
													kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts2[b][1];

													kumijun[i].ko = kumitsuke_Pair[j].parts1[a][0];
													kumijun[i + 1].ko = kumitsuke_Pair[j].parts1[b][1];

													//両手で組立てることのできる2行で最初の行でできた中間製品が子部品になっているとき(親2子1)
													if (kumijun[i].tyukan == kumijun[i + 1].ko)
													{
														kumijun[i].bothhands_type = 1;//親部品が両手
														kumijun[i + 1].bothhands_type = 1;

													}

												}
												else
												{
													kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts1[a][0];
													kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts1[b][1];

													kumijun[i].ko = kumitsuke_Pair[j].parts2[a][0];
													kumijun[i + 1].ko = kumitsuke_Pair[j].parts2[b][1];

													kumijun[i].bothhands_type = 2;//子部品が両手
													kumijun[i + 1].bothhands_type = 2;


												}

												////両手で組立てることのできる2行で最初の行でできた中間製品が子部品になっているとき(親2子1)
												//if (kumijun[i].tyukan == kumijun[i + 1].ko)
												//{
												//	kumijun[i].bothhands_type = 1;//親部品が両手
												//	kumijun[i + 1].bothhands_type = 1;
												//}
											}
											else//親部品にやりづらさがある時
											{

												kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts1[a][0];//組付けリストでは両手で組付けることのできる部品があらかじめ子部品になっているのでOK
												kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts1[b][1];

												kumijun[i].ko = kumitsuke_Pair[j].parts2[a][0];
												kumijun[i + 1].ko = kumitsuke_Pair[j].parts2[b][1];
												//両手で組立てることのできる2行で最初の行でできた中間製品が子部品になっているとき(親2子1)
												if (kumijun[i].tyukan == kumijun[i + 1].ko)
												{
													kumijun[i].bothhands_type = 1;//親部品が両手
													kumijun[i + 1].bothhands_type = 1;

												}
												else
												{
													//親子関係の情報を加える
													kumijun[i].bothhands_type = 2;//子部品が両手
													kumijun[i + 1].bothhands_type = 2;
												}
											}
											i += 2;//outに行くのでi++されないから+2する
											BH2flag = 1;//両手2部品のフラグを付けておく(いらないかも)
											goto out;
										}
										else if (kumijun[i].kuminum == kumitsuke_Pair[j].Num[b][1] && kumijun[i + 1].kuminum == kumitsuke_Pair[j].Num[a][0])//要検討 これでは1，5と5，1の組合せの接触線が選ばれるため、両手で2部品組立てる際に同じ組立順序になる可能性あり。しかし順序に制約がかかっている場合このようにしないと順序が選ばれないことがある
										{
											if (kumitsuke_Pair[j].bothhands_type[a][0] == 1 && kumitsuke_Pair[j].bothhands_type[b][1] == 1)//組付けが親部品、子部品を入れ替えても両手2部品組付けになる時
											{//前の作業が両手作業で、できた中間製品が次の組付けの子部品になるとき
												/*if ((kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[b][1]) || ((kumijun[i - 2].tyukan == kumitsuke_Pair[j].parts2[b][1] && kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[a][0]) && kumijun[i - 1].bothhands_type > 0))*/
												if ((kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[b][1] || (kumijun[i - 2].tyukan == kumitsuke_Pair[j].parts2[b][1] && kumijun[i - 1].tyukan == kumitsuke_Pair[j].parts2[a][0])) && kumijun[i - 1].bothhands_type > 0)
												{
													kumijun[i].bothhands_type = 2;//子部品が両手
													kumijun[i + 1].bothhands_type = 2;
													//親子関係の入れ替え
													kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts2[b][1];
													kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts2[a][0];

													kumijun[i].ko = kumitsuke_Pair[j].parts1[b][1];
													kumijun[i + 1].ko = kumitsuke_Pair[j].parts1[a][0];

													//両手で組立てることのできる2行で最初の行でできた中間製品が子部品になっているとき(親2子1)

													if (kumijun[i].tyukan == kumijun[i + 1].ko)
													{
														kumijun[i].bothhands_type = 1;//親部品が両手
														kumijun[i + 1].bothhands_type = 1;
													}
												}

												else
												{
													kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts1[b][1];
													kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts1[a][0];

													kumijun[i].ko = kumitsuke_Pair[j].parts2[b][1];
													kumijun[i + 1].ko = kumitsuke_Pair[j].parts2[a][0];
													//両手で組立てることのできる2行で最初の行でできた中間製品が子部品になっているとき(親2子1)
													if (kumijun[i].tyukan == kumijun[i + 1].ko)
													{
														kumijun[i].bothhands_type = 1;//親部品が両手
														kumijun[i + 1].bothhands_type = 1;

													}
													//親子関係の情報を加える
													else
													{
														kumijun[i].bothhands_type = 2;//子部品が両手
														kumijun[i + 1].bothhands_type = 2;
													}


												}
												////両手で組立てることのできる2行で最初の行でできた中間製品が子部品になっているとき(親2子1)
												//if (kumijun[i].tyukan == kumijun[i + 1].ko)
												//{
												//	kumijun[i].bothhands_type = 1;//親部品が両手
												//	kumijun[i + 1].bothhands_type = 1;
												//}
											}
											else//親部品にやりづらさがある時
											{

												kumijun[i].oya = kumijun[i].base = kumitsuke_Pair[j].parts1[b][1];//組付けリストでは両手で組付けることのできる部品があらかじめ子部品になっているのでOK
												kumijun[i + 1].oya = kumijun[i + 1].base = kumitsuke_Pair[j].parts1[a][0];

												kumijun[i].ko = kumitsuke_Pair[j].parts2[b][1];
												kumijun[i + 1].ko = kumitsuke_Pair[j].parts2[a][0];
												if (kumijun[i].tyukan == kumijun[i + 1].ko)
												{
													kumijun[i].bothhands_type = 1;//親部品が両手
													kumijun[i + 1].bothhands_type = 1;

												}
												else
												{
													//親子関係の情報を加える
													kumijun[i].bothhands_type = 2;//子部品が両手
													kumijun[i + 1].bothhands_type = 2;
												}

											}
											i += 2;//outに行くのでi++されないから+2する
											BH2flag = 1;//両手2部品のフラグを付けておく(いらないかも)
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
									if (BPkumituke[o].oyakoflug == 1)//同時組付けリストの親部品だけが重いとき
									{
										if (kumijun[i].oya == BPkumituke[o].parts1) kumijun[i].bothhands_type = 6;//親部品が重い
										else kumijun[i].bothhands_type = 6;//子部品が重い
									}
									else if (BPkumituke[o].oyakoflug == 2)//同時組付けリストの子部品だけが重いとき
									{
										if (kumijun[i].ko == BPkumituke[o].parts2) kumijun[i].bothhands_type = 5;//子部品が重い
										else kumijun[i].bothhands_type = 7;//親部品が重い
									}
									else if (BPkumituke[o].oyakoflug == 3) kumijun[i].bothhands_type = 7;//親部品子部品共に重いとき
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
						//printf("手組み\n");
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
					
					
					
						fprintf(fp2, "%s,%d,%d,%d,親部品の姿勢(X軸方向),親部品の姿勢(Z軸方向),治具番号,工具番号,動作番号\n", kumihead.K, kumihead.KJnum, kumihead.flag, kumihead.x);
						for (i = 0;i < gyousu;i++)
						{
							fprintf(fp2, "%d,%d,%d,%d,%c,%c,%d,%d,%d\n", kumijun[i].kuminum, kumijun[i].oya, kumijun[i].ko, kumijun[i].tyukan, dir_default, dir_default, kumijun[i].jig, kumijun[i].kougu, kumijun[i].bothhands_type);
						}

					//fprintf(fp3, "K,%d,%d\n", kumihead.KJnum, hantei);


				overlap://goto文のラベル

					Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
					sscanf(temp, "%s%d%d%d", &kumihead.K, &kumihead.KJnum, &kumihead.flag, &kumihead.x);
					//printf("K %d\n",kumihead.KJnum);
					gyousu = 0;
					hantei = 0;
				}
				else//読み込んだ行の中にKが入っていないとき(組立順序の行の途中の時)
				{
					//ファイルの読み込み
					Del_Com(temp);
					
					
					sscanf(temp, "%d%d%d%d%s%s%s%d%d%d", &kumijun[gyousu].kuminum, &kumijun[gyousu].oya, &kumijun[gyousu].ko, &kumijun[gyousu].tyukan,&kumijun[gyousu].dir,&kumijun[gyousu].XLineDir, &kumijun[gyousu].ZLineDir, &kumijun[gyousu].jig, &kumijun[gyousu].kougu, &kumijun[gyousu].bothhands_type); //組付け番号、部品1、部品2、中間製品の読み込み
					//printf("1列目%d 2列目%d 3列目%d 4列目%d 5列目%s 6列目%s 7列目%s 8列目%d 9列目%d 10列目%d", kumijun[gyousu].kuminum, kumijun[gyousu].oya, kumijun[gyousu].ko, kumijun[gyousu].tyukan,kumijun[gyousu].dir, kumijun[gyousu].XLineDir, kumijun[gyousu].ZLineDir, kumijun[gyousu].jig, kumijun[gyousu].kougu, kumijun[gyousu].bothhands_type);
					kumijun[gyousu].bothhands_type = -1;																																															 //fprintf(fp2, "%d,%d,%d,%d,%s,%d,%d\n", kumijun[gyousu].kuminum, kumijun[gyousu].oya, kumijun[gyousu].ko, kumijun[gyousu].tyukan, kumijun[gyousu].dir, kumijun[gyousu].jig, kumijun[gyousu].base);
					gyousu++;//組立順序の行数を増やす
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

void Read_sametime()//同時制約ファイルを読み込む
{
	FILE*fp;
	char temp[FGETS_MAX_LENGTH];
	int a, i = 0, P1 = 0, P2 = 0;//sametime.flag[0], sametime.sessyoku[0], sametime.Num[0], sametime.parts1[0], sametime.parts2[0];
	int BP = 0/*大物組付け*/, BH2 = 1/*両手2部品組付け*/, Supportflag = 0/*要援助組付け*/, tegumi = 0/*手組み*/, kougu = 0/*工具*/;
	int sessyokusenNum = 0;
	char WkD[STR_MAX];

	sprintf(WkD, "%s\\%s_動作組付けリスト.csv", inPath, SEIHIN_Name);
	//printf("%s", WkD);
	char* fname = WkD;
	fp = fopen(fname, "r");
	printf("インプットファイル　%s\n", fname);
	//fp = fopen(inPath_douji, "r");//同時制約ファイルを読み込みモードで開く
	//fgets(temp, FGETS_MAX_LENGTH, fp);//ファイルの一行目を飛ばす           
	//fgets(temp, FGETS_MAX_LENGTH, fp);
	//fgets(temp, FGETS_MAX_LENGTH, fp);                                                 古屋修正4月13日
	while (fgets(temp, FGETS_MAX_LENGTH, fp) != NULL)//ファイルの最後まで読み取る
	{
		//printf("%s\n", temp);
		Del_Com(temp);//csvファイルはデータごとにカンマがついた形で保存されているので、カンマを消す
		//printf("a%d,b%d,c%d,d%d,e%d,f%d ", sametime.flag[0], sametime.sessyoku[0], sametime.Num[0], sametime.parts1[0], sametime.parts2[0], sametime.bothhands_type[0]);//構造体にファイルの一行を収納
		//printf("g%d, h%d, i%d, j%d, k%d, l%d\n", sametime.flag[1], sametime.sessyoku[1], sametime.Num[1], sametime.parts1[1], sametime.parts2[1], sametime.bothhands_type[1]);
		if (strstr(temp, "大物") != NULL)
		{
			//両手2部品の最後のペアを保存
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
		else if (strstr(temp, "要援助") != NULL)
		{
			fgets(temp, FGETS_MAX_LENGTH, fp);
			Del_Com(temp);
			BH2 = 0;
			BP = 0;
			Supportflag = 1;
			//fgets(temp, FGETS_MAX_LENGTH, fp);
		}
		else if (strstr(temp, "手組み") != NULL)
		{
			fgets(temp, FGETS_MAX_LENGTH, fp);
			Del_Com(temp);
			BH2 = 0;
			BP = 0;
			Supportflag = 0;
			tegumi = 1;
			//fgets(temp, FGETS_MAX_LENGTH, fp);
		}
		else if (strstr(temp, "工具") != NULL)
		{
			//printf("工具使用\n");
			fgets(temp, FGETS_MAX_LENGTH, fp);
			Del_Com(temp);
			BH2 = 0;
			BP = 0;
			Supportflag = 0;
			tegumi = 0;
			kougu = 1;
			//fgets(temp, FGETS_MAX_LENGTH, fp);
		}
		else if (BH2 == 1)//両手2部品組付けの読み込み
		{
			sscanf(temp, "%d%d%d%d%d%d%d%d%d%d%d%d", &sametime.flag[0], &sametime.sessyoku[0], &sametime.Num[0], &sametime.parts1[0], &sametime.parts2[0], &sametime.bothhands_type[0], &sametime.flag[1], &sametime.sessyoku[1], &sametime.Num[1], &sametime.parts1[1], &sametime.parts2[1], &sametime.bothhands_type[1]);//構造体にファイルの一行を収納
			if (sametime.flag[0] == 1 && sametime.flag[1] == 1)
			{
				for (i = 0;i < 2;i++)
				{
					//kumitsuke_Pair[Pair].sessyoku[P1][i] = sametime.sessyoku[i];//ペア1つ目の接触線[0],2つ目の接触線[1]
					sessyokusenNum = sametime.sessyoku[i];
					kumitsuke_Pair[Pair].Num[P1][i] = sametime.Num[i];//ペア1つ目の組付け番号[0],2つ目の組付け番号[1]
					kumitsuke_Pair[Pair].parts1[P1][i] = sametime.parts1[i];//ペア1つ目の親部品,2つ目の親部品
					kumitsuke_Pair[Pair].parts2[P1][i] = sametime.parts2[i];//ペア1つ目の子部品,2つ目の子部品
					kumitsuke_Pair[Pair].bothhands_type[P1][i] = sametime.bothhands_type[i];//ペアの親子指定の情報
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

				printf("　　　　　　%d,%d,%d,%d,%d\n", sessyokusenNum, kumitsuke_Pair[Pair].Num[P2][1], kumitsuke_Pair[Pair].parts1[P2][1], kumitsuke_Pair[Pair].parts2[P2][1], kumitsuke_Pair[Pair].bothhands_type[P2][1]);

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
			sscanf(temp, "%d%d%d%d%d%d", &sametime.flag[0], &sametime.sessyoku[0], &sametime.Num[0], &sametime.parts1[0], &sametime.parts2[0], &sametime.bothhands_type[0]);//構造体にファイルの一行を収納
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
			sscanf(temp, "%d%d%d%d%d%d", &sametime.flag[0], &sametime.sessyoku[0], &sametime.Num[0], &sametime.parts1[0], &sametime.parts2[0], &sametime.bothhands_type[0]);//構造体にファイルの一行を収納
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
			sscanf(temp, "%d%d%d%d%d%d", &sametime.flag[0], &sametime.sessyoku[0], &sametime.Num[0], &sametime.parts1[0], &sametime.parts2[0], &sametime.bothhands_type[0]);//構造体にファイルの一行を収納
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
			sscanf(temp, "%d%d%d%d%d%d%d", &sametime.flag[0], &sametime.sessyoku[0], &sametime.Num[0], &sametime.parts1[0], &sametime.parts2[0], &sametime.bothhands_type[0],&sametime.kougu);//構造体にファイルの一行を収納
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

void Shuffle_oyako(FILE*fp1)//親子関係の指定がない場合に親子関係を入れ替えた組立順序を出力するための関数
{
	int i = 0, j = 0, k = 0, shuffle = 0, a = 0;

	for (i = 0;i < variation;i++)
	{
		fprintf(fp1, "%s,%d%03d,%d,%d,組付け方向,治具番号,ベース部品\n", kumihead.K, kumihead.KJnum, i + 1, kumihead.flag, kumihead.x);
		for (k = 0;k < gyousu;k++)
		{
			shuffle = (i >> j) & 1;//変数iの2進数のjの位が1かどうか(1なら１を返し、0なら0を返す)
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


//メッセージを表示して、プログラムを終了するための関数
void ShowFinMsg(char *msg) {
	int c;
	printf("%s\n", msg);
	printf("(Enterキーを押してください)");
	c = getchar();
	exit(EXIT_FAILURE);
}

//読み込んだ文字列の「,」（カンマ）を「 」（半角スペース）に置き換える関数
void Del_Com(char *s) {
	int	i;

	i = 0;
	while (s[i] != '\0') {
		if (s[i] == ',') s[i] = ' ';
		i++;
	}
}

//このプログラムの置いてあるパスを返すための関数
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
