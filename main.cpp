#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#pragma warning(disable:4996)

char input_control[256];  //定义的文字数组，用于储存用户输入的命令
char keywords[10], input_var[244];  //分别储存用户的命令关键字与命令输入值
float function_input[244];  //浮点型数组，用于储存用户输入的函数
int gcx;

int diff1(float *array_i, float *array_o, int igcx);  //求一阶导函数，返回整数型的导函数阶数
float beFunction(float *i_function, int igcx, float i_var);  //计算输入函数的特定值，返回函数的解
int zeros_f(float *i_function, float *out_value, int igcx, float m, float n);  //求零点函数，返回零点个数
float dichotomy(float low, float high, float er);  //二分法求解函数，返回求得的解
int mono(float *i_dfunction, int idgcx, int low, int high);  //求单调区间函数
int maxp(float *orgin_fun, int igcx, float *o_maxp, int low, int high);  //求极大值函数，返回极大值个数
int minp(float *orgin_fun, int igcx, float *o_minp, int low, int high);  //求极小值函数，返回极小值个数
int runALL(float *function_input, int igcx, float low, float high);  //将输入的函数进行详细分析
int reporter(float *function_input, int igcx, float low, float high);  //生成函数的分析文件并保存为TXT
int mono_r(float *i_dfunction, int idgcx, int low, int high, FILE *o_char);  //将函数的单调区间写入TXT
void reportFunction(float *array_s, int n, FILE *pFile);  //将函数的数学表达式写入TXT文件中
void showFunction(float *array_s, int n);  //将函数的数学表达式打印在屏幕上
void inputA(char *array_a);  //通过输入函数的系数进行函数创建，以输入的值得个数确定函数的阶数
int syntaxPD(char *ipt, int len);  //语法判断函数，确定用户输入的语法是否正确
void inistr(int sa);  //初始化函数，用于初始化创建的数组，清除垃圾存储值
void dropFunction();  //删除用户已输入的函数值
int plot(float *input_fun, int igcx, float low, float high, float err);  //画图函数，画函数的图像

int main()  //主程序  
{
	int jumpout = 0, strcd = 0,k=0,v=0;
	int i,d2gcx=-1,jsc;
	float d2fun[243],x,outZ[20];
	gcx = -1;
	inistr(0);
	dropFunction();
	printf("欢迎使用阉割版MATLAB\n此程序仅用于高阶函数的分析。\n");
	printf(">>请输入命令来执行\n");
	printf("--------------------------------------\n");
	while (jumpout==0)
	{
		printf(">>");
		gets_s(input_control);
		strcd = strlen(input_control);
		if (syntaxPD(input_control, strcd) == 0)
			printf("反馈：输入的语法错误。\n");
		else
		{
			while (input_control[k++] != '(');
			//printf("Keyword len:%d\n", k);
			v = k;
			while (input_control[v++] != ')');
			v = v - k;
			//printf("Input Var len:%d\n", v);
			for (i = 0; i < k-1; i++)
				keywords[i] = input_control[i];
			for (i = 0; i <  v-1; i++)
				input_var[i] = input_control[i+k];
			//printf("%s\n", keywords);
			//printf("%s\n", input_var);
			if (strcmp(keywords, "exit") == 0)  //finished
				jumpout = 1;
			else if (strcmp(keywords, "fs=") == 0)  //none
			{
				//if(input_var==NULL)
				showFunction(function_input, gcx);
			}
			else if (strcmp(keywords, "inputA") == 0)  //finished
			{
				if (input_var[0] == 0)
					printf("反馈：尚未输入变量。\n");
				else
				{
					dropFunction();
					printf("反馈: 已成功清除原有的高阶函数。\n");
					inputA(input_var);
				}
			}
			else if (strcmp(keywords, "plot") == 0)  //none
			{
				if (gcx >= 1)
				{
					if (input_var[0] == '\0' && input_var[1] == '\0'&&input_var[2] == '\0')
					{
						printf("反馈：请输入两个参数来选择区间范围。\n");
					}
					else
					{
						char *token;
						float set[3];
						token = strtok(input_var, ",");
						set[0] = atof(token);
						token = strtok(NULL, ",");
						set[1] = atof(token);
						token = strtok(NULL, ",");
						set[2] = atof(token);
						plot(function_input, gcx, set[0], set[1], set[2]);
					}	
				}
				else
				{
					printf("反馈：尚未输入任何函数。\n");
				}
			}
			else if (strcmp(keywords, "zeros") == 0)  //finished
			{
				if (gcx >= 1)
				{
					if (input_var[0] == '\0' || input_var[1] == '\0')
					{
						printf("反馈：请输入两个参数来选择区间范围。\n");
					}
					else
					{
						char *token;
						float set[2];
						token = strtok(input_var, ",");
						set[0] = atof(token);
						token = strtok(NULL, ",");
						set[1] = atof(token);
						jsc = zeros_f(function_input, outZ, gcx, set[0], set[1]);
						if (jsc == 0)
						{
							printf("此函数无零点。\n");
						}
						else
						{
							for (i = 0; i < jsc; i++)
							{
								printf("零点为：(%.2f,0)\n", outZ[i]);
							}
						}
					}
				}
				else
				{
					printf("反馈：尚未输入任何函数。\n");
				}
			}
			else if (strcmp(keywords, "diff2") == 0)  //finished
			{
				if (gcx > 1)
				{
					d2gcx = diff1(function_input, d2fun, gcx);
					if (d2gcx>2)
					{
						d2gcx = diff1(d2fun, d2fun, d2gcx);
						//printf("二阶导函数的阶数：%d\n", d2gcx);
						//printf("二阶导函数F''(x)为：");
						//showFunction(d2fun, d2gcx);
							if (input_var[0] == '\0' || input_var[1] == '\0')
							{
								printf("反馈：请输入两个参数来选择区间范围。\n");
							}
							else
							{
								char *token;
								float set[2];
								token = strtok(input_var, ",");
								set[0] = atof(token);
								token = strtok(NULL, ",");
								jsc = zeros_f(d2fun, outZ, d2gcx, set[0], set[1]);
								if (jsc==0)
								{
									printf("此函数不存在拐点。\n");
								}
								else
								{
									for (i = 0; i < jsc; i++)
									{
										printf("函数拐点为：%.2f\n", outZ[i]);
									}
								}
							}
					}
					else
					{
						printf("此函数不存在拐点。\n");
					}
				}
				else if(gcx==0)
				{
					printf("反馈：此函数不存在拐点。\n");
				}
				else
				{
					printf("反馈：尚未输入任何函数。\n");
				}
			}
			else if (strcmp(keywords, "mono") == 0)  //finished
			{
				if (input_var[0] == '\0' || input_var[1] == '\0')
				{
					printf("反馈：请输入两个参数来选择区间范围。\n");
				}
				else
				{
					char *token;
					float set[2];
					token = strtok(input_var, ",");
					set[0] = atof(token);
					token = strtok(NULL, ",");
					set[1] = atof(token);
					if (gcx >= 1)
					{
						char o_mono[50];
						d2gcx = diff1(function_input, d2fun, gcx);
						//printf("函数的阶数：%d  ", d2gcx);
						//showFunction(d2fun,d2gcx);
						mono(d2fun, d2gcx, set[0], set[1]);
					}
					else
					{
						printf("反馈：尚未输入任何函数。\n");
					}
				}
			}
			else if (strcmp(keywords, "maxv") == 0)  //finished
			{
			if (input_var[0] == '\0' || input_var[1] == '\0')
			{
				printf("反馈：请输入两个参数来选择区间范围。\n");
			}
			else
			{
				char *token;
				float set[2];
				token = strtok(input_var, ",");
				set[0] = atof(token);
				token = strtok(NULL, ",");
				set[1] = atof(token);
				jsc = maxp(function_input, gcx,outZ , set[0], set[1]);
				if (jsc <= 0)
				{
					printf("本函数不存在最大值。\n");
				}
				else
				{
					printf("本函数的最大值为：");
					for (i = 0; i < jsc; i++)
					{
						printf("(%.2f,%.2f)  ", outZ[i], beFunction(function_input, gcx, outZ[i]));
					}
					printf("\n最大值分别为：");
					for (i = 0; i < jsc; i++)
					{
						printf("%.2f  ", beFunction(function_input, gcx, outZ[i]));
					}
					printf("\n");
				}
			}
			}
			else if (strcmp(keywords, "minv") == 0)  //finished
			{
			if (input_var[0] == '\0' || input_var[1] == '\0')
			{
				printf("反馈：请输入两个参数来选择区间范围。\n");
			}
			else
			{
				char *token;
				float set[2];
				token = strtok(input_var, ",");
				set[0] = atof(token);
				token = strtok(NULL, ",");
				set[1] = atof(token);
				jsc = minp(function_input, gcx, outZ, set[0], set[1]);
				if (jsc <= 0)
				{
					printf("本函数不存在最小值。\n");
				}
				else
				{
					printf("本函数的最小值点为：");
					for (i = 0; i < jsc; i++)
					{
						printf("(%.2f,%.2f)  ", outZ[i], beFunction(function_input, gcx, outZ[i]));
					}
					printf("\n最小值分别为：");
					for (i = 0; i < jsc; i++)
					{
						printf("%.2f  ", beFunction(function_input, gcx, outZ[i]));
					}
					printf("\n");
				}
			}
			}
			else if (strcmp(keywords, "runALL") == 0)  //finished
			{
			if (input_var[0] == '\0' || input_var[1] == '\0')
			{
				printf("反馈：请输入两个参数来选择区间范围。\n");
			}
			else
			{
				char *token;
				float set[2];
				token = strtok(input_var, ",");
				set[0] = atof(token);
				token = strtok(NULL, ",");
				set[1] = atof(token);
				runALL(function_input, gcx, set[0], set[1]);
			}
			}
			else if (strcmp(keywords, "report") == 0)  //finished
			{
			if (input_var[0] == '\0' || input_var[1] == '\0')
			{
				printf("反馈：请输入两个参数来选择区间范围。\n");
			}
			else
			{
				char *token;
				float set[2];
				token = strtok(input_var, ",");
				set[0] = atof(token);
				token = strtok(NULL, ",");
				set[1] = atof(token);
				reporter(function_input, gcx, set[0], set[1]);
			}
			}
			else if (strcmp(keywords, "dropF") == 0)  //finished
			{
				if (gcx>=0)
				{
					dropFunction();
					printf("反馈: 已成功清除原有的高阶函数。\n");
				}
				else
				{
					printf("反馈：尚未输入任何函数。\n");
				}
			}
			else if (strcmp(keywords, "help") == 0)  //finished
			{
				printf("程序帮助：\n基本语法为命令关键词带括号，括号里写输入变量或空\n");
				printf("fs=()    显示已输入的函数\n");
				printf("inputA()    通过输入高阶函数的系数构建高阶函数，在括号内以英文逗号进行分隔，从高次项到低次项顺序输入\n");
				printf("plot()    将已经输入的函数进行画图操作\n");
				printf("zeros()    计算已输入函数的零点并显示出来\n");
				printf("diff2()    计算已输入函数的拐点并显示出来\n");
				printf("mono()    判断已输入函数的单调性与单调区间\n");
				printf("maxv()    计算已输入函数的极大值并显示出来\n");
				printf("minv()    计算已输入函数的极小值并显示出来\n");
				printf("runALL()    将已输入的函数进行完整的分析并且显示出来\n");
				printf("report()    生成已输入函数的报告并保存为TXT文本文档\n");
				printf("dropF()    删除已输入的函数\n");
				printf("help()    显示本程序的帮助页面\n");
				printf("info()    显示软件信息以及开发者名单\n");
				printf("exit()    退出程序\n");
			}
			else if (strcmp(keywords, "info") == 0)  //finished
			{
				printf("程序信息：\n制作人员：梁哲铭 朱翔宇 周霆威 陈牧言 王一迪\n");
				printf("院校：南京工程学院\n");
				printf("班级组别：机器人172第9组\n");
			}
			else
			{
				printf("输入的命令：%s\n未找到，请检查你输入的命令。\n",keywords);
			}
			v = 0; k = 0;
			inistr(1);
			d2gcx = -1;
		}
	}
	printf("Good Bye!\n");
	system("pause");
}

int diff1(float *array_i, float *array_o,int igcx)
{
	int i;
	int ogcx = -1;
	for (i = 0; i < igcx; i++)
	{
		array_o[i] = array_i[i] * (igcx - 1 - i);
	}
	if (igcx - 1 <= 0)
	{
		ogcx = 0;
	}
	else
	{
		ogcx = igcx - 1;
	}
	//showFunction(array_o,ogcx);
	return ogcx;
}

float beFunction(float *i_function, int igcx, float i_var)
{
	int i;
	float ds = 0;
		for (i = 0; i < igcx; i++)
		{
			ds = i_function[i] * pow(i_var, igcx-1 - i) + ds;
			//printf("pow:%f", pow(i_var, igcx - 1 - i));
			//printf("ds:%f\n", ds);
		}
	//printf("ds:%f\n", ds);
	return ds;
}

int zeros_f (float *i_function, float *out_value,int igcx,float m,float n)
{
float f1, f2, h,i;
int s=0;
//printf("函数定义区间为:");
//scanf("%f %f", &m, &n);
for (i = m; i <= n; i=i+0.01)
	{
	    f1 = beFunction(i_function, igcx, i);
		h = i + 0.01;
		f2 = beFunction(i_function, igcx, h);
		if (f1 == 0)
		{
			//printf("f1：%f\n", f1);
			//printf("f2：%f\n", f2);
			//x = i;
			//printf("零1点：%f\n", x);
			out_value[s] = i;
			s++;
		}
		else if (f1 < 0 && f2>0)
		{
			//printf("f1：%f\n", f1);
			//printf("f2：%f\n", f2);
			//printf("零点：%f\n", x);
			out_value[s] = dichotomy(i, h, 0.00005);
			s++;
		}
		else if (f2 < 0 && f1>0)
		{
			//printf("f1：%f\n", f1);
			//printf("f2：%f\n", f2);
			//printf("零点：%f\n", x);
			out_value[s] = dichotomy(i, h, 0.00005);
			s++;
		}
	}
	return s;
}

float dichotomy(float low, float high, float er)
{
	float ans;
	while ((high-low)>er)
	{
		ans = (low + high) / 2.0;
		if (beFunction(function_input, gcx, low)*beFunction(function_input, gcx, ans) < 0)
		{
			high = ans;
		}
		else
		{
			low = ans;
		}
		//printf("根变化c=%f\n", ans);
	}
	//printf("方程最终根c=%.3f\n", ans);
	return ans;
}

int mono(float *i_dfunction, int idgcx,  int low, int high)
{
	int gs_0 = 0,i;
	float dpoint_0[50], zd, yd,d0;
	gs_0 = zeros_f(i_dfunction, dpoint_0, idgcx, low, high);
	if (gs_0 <= 0)
	{
		d0 = beFunction(i_dfunction, idgcx, (low + high) / 2.0);
		if (d0 > 0)
		{
			printf("函数为单调增函数。");
		}
		else if (d0 < 0)
		{
			printf("函数为单调减函数。");
		}
	}
	else
	{
		for (i = 0; i < gs_0; i++)
		{
			zd = beFunction(i_dfunction, idgcx, (dpoint_0[i] - 0.01));
			yd = beFunction(i_dfunction, idgcx, (dpoint_0[i] + 0.01));
			if (zd<0 && yd >0)
			{
				printf("|-|%.2f|+|", dpoint_0[i]);
			}
			else if (zd > 0 && yd < 0)
			{
				printf("|+|%.2f|-|", dpoint_0[i]);
			}
		}
	}
	printf("\n");
	return 0;
}

int maxp(float *orgin_fun,int igcx, float *o_maxp,int low,int high)
{
	int i, djs = 0,zjs=0,maxp=0,dzero=0;
	float zd, yd, dfun[50], dfun_z[50];
	djs = diff1(orgin_fun, dfun, igcx);
	zjs = zeros_f(dfun, dfun_z, djs, low, high);
	if (zjs <= 0)
	{
		//printf("此1函数不存在极大值。\n");
		return 0;
	}
	else
	{
		for (i = 0; i < zjs; i++)
		{
			zd = beFunction(dfun, djs, (dfun_z[i] - 0.01));
			yd = beFunction(dfun, djs, (dfun_z[i] + 0.01));
			if (zd>0 && yd<0)
			{
				//printf("函数的极大值点为：%.2f\n", dfun_z[i]);
				o_maxp[dzero] = dfun_z[i];
				dzero++;
			}
		}
		if (dzero==0)
		{
			//printf("此2函数不存在极大值。\n");
			return 0;
		}
		else if (dzero > 0)
		{
			return dzero;
		}
	}
}

int minp(float *orgin_fun, int igcx,float *o_minp, int low, int high)
{
	int i, djs = 0, zjs = 0, maxp = 0, dzero = 0;
	float zd, yd, dfun[50], dfun_z[50];
	djs = diff1(orgin_fun, dfun, igcx);
	zjs = zeros_f(dfun, dfun_z, djs, low, high);
	if (zjs <= 0)
	{
		//printf("此1函数不存在极小值。\n");
		return 0;
	}
	else
	{
		for (i = 0; i < zjs; i++)
		{
			zd = beFunction(dfun, djs, (dfun_z[i] - 0.01));
			yd = beFunction(dfun, djs, (dfun_z[i] + 0.01));
			//printf("zd:%f  yd:%f\n", zd, yd);
			if (zd < 0 && yd > 0)
			{
				//printf("函数的极小值点为：%.2f\n", dfun_z[i]);
				o_minp[dzero] = dfun_z[i];
				dzero++;
			}
		}
		if (dzero == 0)
		{
			//printf("此2函数不存在极小值。\n");
			return 0;
		}
		else if (dzero > 0)
		{
			return dzero;
		}
	}
}

int runALL(float *function_input, int igcx, float low, float high)
{
	float storage[50],storage1[50];
	int js1 = 0,i;
	if (igcx <= 0)
	{
		printf("尚未输入任何高次方程。\n");
	}
	else
	{
		printf("高次方程为：\n");
		showFunction(function_input, igcx);
		js1 = zeros_f(function_input, storage, igcx, low, high);
		printf("函数零点为：");
		for (i = 0; i < js1; i++)
		{
			printf("(%.4f,0)  ", storage[i]);
		}
		printf("\n");

		printf("函数拐点为：");
		if (igcx > 3)
		{
			js1 = diff1(function_input, storage, igcx);
			js1 = diff1(storage, storage1, js1);
			js1 = zeros_f(storage1, storage, js1, low, high);
			for ( i = 0; i < js1; i++)
			{
				printf("(%f,%f)  ", storage[i], beFunction(function_input, igcx, storage[i]));
			}
			printf("\n");
		}
		else
		{
			printf("不存在。\n");
		}

		js1 = maxp(function_input, gcx, storage, low, high);
		if (js1 <= 0)
		{
			printf("函数的极大值为：本函数不存在极大值。\n");
		}
		else
		{
			printf("本函数的极大值为：");
			for (i = 0; i < js1; i++)
			{
				printf("(%f,%f)  ", storage[i], beFunction(function_input, gcx, storage[i]));
			}
			printf("\n极大值分别为：");
			for (i = 0; i < js1; i++)
			{
				printf("%f  ", beFunction(function_input, gcx, storage[i]));
			}
			printf("\n");
		}

		js1 = minp(function_input, gcx, storage, low, high);
		if (js1 <= 0)
		{
			printf("函数的极小值：本函数不存在极小值。\n");
		}
		else
		{
			printf("函数的极小值点为：");
			for (i = 0; i < js1; i++)
			{
				printf("(%f,%f)  ", storage[i], beFunction(function_input, gcx, storage[i]));
			}
			printf("\n极小值分别为：");
			for (i = 0; i < js1; i++)
			{
				printf("%f  ", beFunction(function_input, gcx, storage[i]));
			}
			printf("\n");
		}

		printf("函数的单调区间为：");
		js1 = diff1(function_input, storage, gcx);
		mono(storage, js1, low, high);
	}
	return 1;
}

int reporter(float *function_input, int igcx, float low, float high)
{
	float storage[50], storage1[50];
	int js1 = 0, i;
	const char *FileName = "Result.txt";
	FILE *pFile;
	pFile = fopen(FileName, "a");
	if (pFile == NULL)
	{
		printf("反馈：无法输出TXT文件。\n");
		return 0;
	}
	if (igcx <= 0)
	{
		fprintf(pFile,"尚未输入任何高次方程。\n");
	}
	else
	{
		fprintf(pFile, "高次方程为：\n");
		reportFunction(function_input, igcx,pFile);
		js1 = zeros_f(function_input, storage, igcx, low, high);
		fprintf(pFile, "函数零点为：");
		for (i = 0; i < js1; i++)
		{
			fprintf(pFile, "(%.4f,0)  ", storage[i]);
		}
		fprintf(pFile, "\n");

		fprintf(pFile, "函数拐点为：");
		if (igcx > 3)
		{
			js1 = diff1(function_input, storage, igcx);
			js1 = diff1(storage, storage1, js1);
			js1 = zeros_f(storage1, storage, js1, low, high);
			for (i = 0; i < js1; i++)
			{
				fprintf(pFile, "(%f,%f)  ", storage[i], beFunction(function_input, igcx, storage[i]));
			}
			fprintf(pFile, "\n");
		}
		else
		{
			fprintf(pFile, "不存在。\n");
		}

		js1 = maxp(function_input, gcx, storage, low, high);
		if (js1 <= 0)
		{
			fprintf(pFile, "函数的极大值：本函数不存在极大值。\n");
		}
		else
		{
			fprintf(pFile, "函数的最大值点为：");
			for (i = 0; i < js1; i++)
			{
				fprintf(pFile, "(%f,%f)  ", storage[i], beFunction(function_input, gcx, storage[i]));
			}
			fprintf(pFile, "\n最大值分别为：");
			for (i = 0; i < js1; i++)
			{
				fprintf(pFile, "%f  ", beFunction(function_input, gcx, storage[i]));
			}
			fprintf(pFile, "\n");
		}

		js1 = minp(function_input, gcx, storage, low, high);
		if (js1 <= 0)
		{
			fprintf(pFile, "函数的极小值：本函数不存在极小值。\n");
		}
		else
		{
			fprintf(pFile, "函数的极小值点为：");
			for (i = 0; i < js1; i++)
			{
				fprintf(pFile, "(%f,%f)  ", storage[i], beFunction(function_input, gcx, storage[i]));
			}
			fprintf(pFile, "\n极小值分别为：");
			for (i = 0; i < js1; i++)
			{
				fprintf(pFile, "%f  ", beFunction(function_input, gcx, storage[i]));
			}
			fprintf(pFile, "\n");
		}

		fprintf(pFile, "函数的单调区间为：");
		js1 = diff1(function_input, storage, gcx);
		mono_r(storage, js1, low, high,pFile);
		fprintf(pFile, "\n");
		printf("反馈：分析文件已成功保存，保存为根目录下的Result.txt文件中。\n");
	}
	fclose(pFile);
	return 1;
}

int mono_r(float *i_dfunction, int idgcx, int low, int high,FILE *o_char)
{
	int gs_0 = 0, i;
	float dpoint_0[50], zd, yd, d0;
	gs_0 = zeros_f(i_dfunction, dpoint_0, idgcx, low, high);
	if (gs_0 <= 0)
	{
		d0 = beFunction(i_dfunction, idgcx, (low + high) / 2.0);
		if (d0 > 0)
		{
			fprintf(o_char,"函数为单调增函数。");
		}
		else if (d0 < 0)
		{
			fprintf(o_char, "函数为单调减函数。");
		}
	}
	else
	{
		for (i = 0; i < gs_0; i++)
		{
			zd = beFunction(i_dfunction, idgcx, (dpoint_0[i] - 0.01));
			yd = beFunction(i_dfunction, idgcx, (dpoint_0[i] + 0.01));
			if (zd < 0 && yd >0)
			{
				fprintf(o_char, "|-|%.2f|+|", dpoint_0[i]);
			}
			else if (zd > 0 && yd < 0)
			{
				fprintf(o_char, "|+|%.2f|-|", dpoint_0[i]);
			}
		}
	}
	fprintf(o_char, "\n");
	return 1;
}

void reportFunction(float *array_s, int n,FILE *pFile)
{
	int i = 0;
	n--;
	fprintf(pFile, "F(x)=");
	for (i = 0; i <= n; i++)
	{
		if (array_s[i] != 0)
		{
			if (n - i == 0)
			{
				fprintf(pFile, "%.2f", array_s[i]);
			}
			else
			{
				fprintf(pFile, "%.2f", array_s[i]);
				fprintf(pFile, "*x^");
				fprintf(pFile, "%d", n - i);
			}
			if (i != n)
				if (array_s[i + 1] >= 0)
					fprintf(pFile, "+");
				else
				{

				}
		}
	}
	fprintf(pFile, "\n");
}

void showFunction(float *array_s, int n)
{
	int i = 0;
	n--;
	printf("F(x)=");
	for (i = 0; i <= n; i++)
	{
		if (array_s[i] != 0)
		{
			if (n - i == 0)
			{
				printf("%.2f", array_s[i]);
			}
			else
			{
				printf("%.2f", array_s[i]);
				printf("*x^");
				printf("%d", n - i);
			}
			if (i != n)
				if (array_s[i + 1] >= 0)
					printf("+");
				else
				{

				}
		}
	}
	printf("\n");
}

void inputA(char *array_a)
{
	int i = 0;
	char *token;
	token = strtok(array_a, ",");
	while (token != NULL)
	{
		function_input[i] = atof(token);
		token = strtok(NULL, ",");
		i++;
	}
	gcx = i;
	printf("函数的项数：%d\n", i);
	showFunction(function_input, i);
}

int syntaxPD(char *ipt, int len)
{
	int i, z = 0, y = 0, q = 0, b = 0;
	for (i = 0; i <= len; i++)
	{
		if (ipt[i] == '(')
			z++, q = i;
		else if (ipt[i] == ')')
			y++, b = i;
	}
	if (z == 1 && y == 1 && q != 0 && b - q >= 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void inistr(int sa)
{
	int i;
	if (sa == 0)
	{
		for (i = 0; i < 256; i++)
			input_control[i] = '\0';
	}
	for (i = 0; i < 10; i++)
		keywords[i] = '\0';
	for (i = 0; i < 244; i++)
		input_var[i] = '\0';
}

void dropFunction()
{
	int i;
	gcx = -1;
	for (i = 0; i < 244; i++)
		function_input[i] = 0;
}

int plot(float *input_fun, int igcx,float low,float high,float err)
{
	const char *FileName = "plot.dat";
	FILE *pFile;
	pFile = fopen(FileName,"wt");
	if (pFile == NULL)
	{
		printf("反馈：无法输出dat文件。\n");
		return 0;
	}
	for (float i = -50; i < 50; i++)
	{
		fprintf(pFile,"%.0f %.0f\n", i, beFunction(input_fun, igcx, i));
	}
	fclose(pFile);
	printf("请复制  plot \"plot.dat\" with linespoin 进行画图操作\n");
	printf("请注意，必须安装gnuplot图像软件才可运行！否则系统将报错。\n");
	printf("软件可输入exit退出！\n");
	system("gnuplot");
	return 0;
}