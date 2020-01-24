#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#pragma warning(disable:4996)

char input_control[256];  //������������飬���ڴ����û����������
char keywords[10], input_var[244];  //�ֱ𴢴��û�������ؼ�������������ֵ
float function_input[244];  //���������飬���ڴ����û�����ĺ���
int gcx;

int diff1(float *array_i, float *array_o, int igcx);  //��һ�׵����������������͵ĵ���������
float beFunction(float *i_function, int igcx, float i_var);  //�������뺯�����ض�ֵ�����غ����Ľ�
int zeros_f(float *i_function, float *out_value, int igcx, float m, float n);  //����㺯��������������
float dichotomy(float low, float high, float er);  //���ַ���⺯����������õĽ�
int mono(float *i_dfunction, int idgcx, int low, int high);  //�󵥵����亯��
int maxp(float *orgin_fun, int igcx, float *o_maxp, int low, int high);  //�󼫴�ֵ���������ؼ���ֵ����
int minp(float *orgin_fun, int igcx, float *o_minp, int low, int high);  //��Сֵ���������ؼ�Сֵ����
int runALL(float *function_input, int igcx, float low, float high);  //������ĺ���������ϸ����
int reporter(float *function_input, int igcx, float low, float high);  //���ɺ����ķ����ļ�������ΪTXT
int mono_r(float *i_dfunction, int idgcx, int low, int high, FILE *o_char);  //�������ĵ�������д��TXT
void reportFunction(float *array_s, int n, FILE *pFile);  //����������ѧ���ʽд��TXT�ļ���
void showFunction(float *array_s, int n);  //����������ѧ���ʽ��ӡ����Ļ��
void inputA(char *array_a);  //ͨ�����뺯����ϵ�����к����������������ֵ�ø���ȷ�������Ľ���
int syntaxPD(char *ipt, int len);  //�﷨�жϺ�����ȷ���û�������﷨�Ƿ���ȷ
void inistr(int sa);  //��ʼ�����������ڳ�ʼ�����������飬��������洢ֵ
void dropFunction();  //ɾ���û�������ĺ���ֵ
int plot(float *input_fun, int igcx, float low, float high, float err);  //��ͼ��������������ͼ��

int main()  //������  
{
	int jumpout = 0, strcd = 0,k=0,v=0;
	int i,d2gcx=-1,jsc;
	float d2fun[243],x,outZ[20];
	gcx = -1;
	inistr(0);
	dropFunction();
	printf("��ӭʹ���˸��MATLAB\n�˳�������ڸ߽׺����ķ�����\n");
	printf(">>������������ִ��\n");
	printf("--------------------------------------\n");
	while (jumpout==0)
	{
		printf(">>");
		gets_s(input_control);
		strcd = strlen(input_control);
		if (syntaxPD(input_control, strcd) == 0)
			printf("������������﷨����\n");
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
					printf("��������δ���������\n");
				else
				{
					dropFunction();
					printf("����: �ѳɹ����ԭ�еĸ߽׺�����\n");
					inputA(input_var);
				}
			}
			else if (strcmp(keywords, "plot") == 0)  //none
			{
				if (gcx >= 1)
				{
					if (input_var[0] == '\0' && input_var[1] == '\0'&&input_var[2] == '\0')
					{
						printf("����������������������ѡ�����䷶Χ��\n");
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
					printf("��������δ�����κκ�����\n");
				}
			}
			else if (strcmp(keywords, "zeros") == 0)  //finished
			{
				if (gcx >= 1)
				{
					if (input_var[0] == '\0' || input_var[1] == '\0')
					{
						printf("����������������������ѡ�����䷶Χ��\n");
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
							printf("�˺�������㡣\n");
						}
						else
						{
							for (i = 0; i < jsc; i++)
							{
								printf("���Ϊ��(%.2f,0)\n", outZ[i]);
							}
						}
					}
				}
				else
				{
					printf("��������δ�����κκ�����\n");
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
						//printf("���׵������Ľ�����%d\n", d2gcx);
						//printf("���׵�����F''(x)Ϊ��");
						//showFunction(d2fun, d2gcx);
							if (input_var[0] == '\0' || input_var[1] == '\0')
							{
								printf("����������������������ѡ�����䷶Χ��\n");
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
									printf("�˺��������ڹյ㡣\n");
								}
								else
								{
									for (i = 0; i < jsc; i++)
									{
										printf("�����յ�Ϊ��%.2f\n", outZ[i]);
									}
								}
							}
					}
					else
					{
						printf("�˺��������ڹյ㡣\n");
					}
				}
				else if(gcx==0)
				{
					printf("�������˺��������ڹյ㡣\n");
				}
				else
				{
					printf("��������δ�����κκ�����\n");
				}
			}
			else if (strcmp(keywords, "mono") == 0)  //finished
			{
				if (input_var[0] == '\0' || input_var[1] == '\0')
				{
					printf("����������������������ѡ�����䷶Χ��\n");
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
						//printf("�����Ľ�����%d  ", d2gcx);
						//showFunction(d2fun,d2gcx);
						mono(d2fun, d2gcx, set[0], set[1]);
					}
					else
					{
						printf("��������δ�����κκ�����\n");
					}
				}
			}
			else if (strcmp(keywords, "maxv") == 0)  //finished
			{
			if (input_var[0] == '\0' || input_var[1] == '\0')
			{
				printf("����������������������ѡ�����䷶Χ��\n");
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
					printf("���������������ֵ��\n");
				}
				else
				{
					printf("�����������ֵΪ��");
					for (i = 0; i < jsc; i++)
					{
						printf("(%.2f,%.2f)  ", outZ[i], beFunction(function_input, gcx, outZ[i]));
					}
					printf("\n���ֵ�ֱ�Ϊ��");
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
				printf("����������������������ѡ�����䷶Χ��\n");
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
					printf("��������������Сֵ��\n");
				}
				else
				{
					printf("����������Сֵ��Ϊ��");
					for (i = 0; i < jsc; i++)
					{
						printf("(%.2f,%.2f)  ", outZ[i], beFunction(function_input, gcx, outZ[i]));
					}
					printf("\n��Сֵ�ֱ�Ϊ��");
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
				printf("����������������������ѡ�����䷶Χ��\n");
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
				printf("����������������������ѡ�����䷶Χ��\n");
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
					printf("����: �ѳɹ����ԭ�еĸ߽׺�����\n");
				}
				else
				{
					printf("��������δ�����κκ�����\n");
				}
			}
			else if (strcmp(keywords, "help") == 0)  //finished
			{
				printf("���������\n�����﷨Ϊ����ؼ��ʴ����ţ�������д����������\n");
				printf("fs=()    ��ʾ������ĺ���\n");
				printf("inputA()    ͨ������߽׺�����ϵ�������߽׺���������������Ӣ�Ķ��Ž��зָ����Ӹߴ���ʹ���˳������\n");
				printf("plot()    ���Ѿ�����ĺ������л�ͼ����\n");
				printf("zeros()    ���������뺯������㲢��ʾ����\n");
				printf("diff2()    ���������뺯���Ĺյ㲢��ʾ����\n");
				printf("mono()    �ж������뺯���ĵ������뵥������\n");
				printf("maxv()    ���������뺯���ļ���ֵ����ʾ����\n");
				printf("minv()    ���������뺯���ļ�Сֵ����ʾ����\n");
				printf("runALL()    ��������ĺ������������ķ���������ʾ����\n");
				printf("report()    ���������뺯���ı��沢����ΪTXT�ı��ĵ�\n");
				printf("dropF()    ɾ��������ĺ���\n");
				printf("help()    ��ʾ������İ���ҳ��\n");
				printf("info()    ��ʾ�����Ϣ�Լ�����������\n");
				printf("exit()    �˳�����\n");
			}
			else if (strcmp(keywords, "info") == 0)  //finished
			{
				printf("������Ϣ��\n������Ա�������� ������ ������ ������ ��һ��\n");
				printf("ԺУ���Ͼ�����ѧԺ\n");
				printf("�༶��𣺻�����172��9��\n");
			}
			else
			{
				printf("��������%s\nδ�ҵ�����������������\n",keywords);
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
//printf("������������Ϊ:");
//scanf("%f %f", &m, &n);
for (i = m; i <= n; i=i+0.01)
	{
	    f1 = beFunction(i_function, igcx, i);
		h = i + 0.01;
		f2 = beFunction(i_function, igcx, h);
		if (f1 == 0)
		{
			//printf("f1��%f\n", f1);
			//printf("f2��%f\n", f2);
			//x = i;
			//printf("��1�㣺%f\n", x);
			out_value[s] = i;
			s++;
		}
		else if (f1 < 0 && f2>0)
		{
			//printf("f1��%f\n", f1);
			//printf("f2��%f\n", f2);
			//printf("��㣺%f\n", x);
			out_value[s] = dichotomy(i, h, 0.00005);
			s++;
		}
		else if (f2 < 0 && f1>0)
		{
			//printf("f1��%f\n", f1);
			//printf("f2��%f\n", f2);
			//printf("��㣺%f\n", x);
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
		//printf("���仯c=%f\n", ans);
	}
	//printf("�������ո�c=%.3f\n", ans);
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
			printf("����Ϊ������������");
		}
		else if (d0 < 0)
		{
			printf("����Ϊ������������");
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
		//printf("��1���������ڼ���ֵ��\n");
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
				//printf("�����ļ���ֵ��Ϊ��%.2f\n", dfun_z[i]);
				o_maxp[dzero] = dfun_z[i];
				dzero++;
			}
		}
		if (dzero==0)
		{
			//printf("��2���������ڼ���ֵ��\n");
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
		//printf("��1���������ڼ�Сֵ��\n");
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
				//printf("�����ļ�Сֵ��Ϊ��%.2f\n", dfun_z[i]);
				o_minp[dzero] = dfun_z[i];
				dzero++;
			}
		}
		if (dzero == 0)
		{
			//printf("��2���������ڼ�Сֵ��\n");
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
		printf("��δ�����κθߴη��̡�\n");
	}
	else
	{
		printf("�ߴη���Ϊ��\n");
		showFunction(function_input, igcx);
		js1 = zeros_f(function_input, storage, igcx, low, high);
		printf("�������Ϊ��");
		for (i = 0; i < js1; i++)
		{
			printf("(%.4f,0)  ", storage[i]);
		}
		printf("\n");

		printf("�����յ�Ϊ��");
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
			printf("�����ڡ�\n");
		}

		js1 = maxp(function_input, gcx, storage, low, high);
		if (js1 <= 0)
		{
			printf("�����ļ���ֵΪ�������������ڼ���ֵ��\n");
		}
		else
		{
			printf("�������ļ���ֵΪ��");
			for (i = 0; i < js1; i++)
			{
				printf("(%f,%f)  ", storage[i], beFunction(function_input, gcx, storage[i]));
			}
			printf("\n����ֵ�ֱ�Ϊ��");
			for (i = 0; i < js1; i++)
			{
				printf("%f  ", beFunction(function_input, gcx, storage[i]));
			}
			printf("\n");
		}

		js1 = minp(function_input, gcx, storage, low, high);
		if (js1 <= 0)
		{
			printf("�����ļ�Сֵ�������������ڼ�Сֵ��\n");
		}
		else
		{
			printf("�����ļ�Сֵ��Ϊ��");
			for (i = 0; i < js1; i++)
			{
				printf("(%f,%f)  ", storage[i], beFunction(function_input, gcx, storage[i]));
			}
			printf("\n��Сֵ�ֱ�Ϊ��");
			for (i = 0; i < js1; i++)
			{
				printf("%f  ", beFunction(function_input, gcx, storage[i]));
			}
			printf("\n");
		}

		printf("�����ĵ�������Ϊ��");
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
		printf("�������޷����TXT�ļ���\n");
		return 0;
	}
	if (igcx <= 0)
	{
		fprintf(pFile,"��δ�����κθߴη��̡�\n");
	}
	else
	{
		fprintf(pFile, "�ߴη���Ϊ��\n");
		reportFunction(function_input, igcx,pFile);
		js1 = zeros_f(function_input, storage, igcx, low, high);
		fprintf(pFile, "�������Ϊ��");
		for (i = 0; i < js1; i++)
		{
			fprintf(pFile, "(%.4f,0)  ", storage[i]);
		}
		fprintf(pFile, "\n");

		fprintf(pFile, "�����յ�Ϊ��");
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
			fprintf(pFile, "�����ڡ�\n");
		}

		js1 = maxp(function_input, gcx, storage, low, high);
		if (js1 <= 0)
		{
			fprintf(pFile, "�����ļ���ֵ�������������ڼ���ֵ��\n");
		}
		else
		{
			fprintf(pFile, "���������ֵ��Ϊ��");
			for (i = 0; i < js1; i++)
			{
				fprintf(pFile, "(%f,%f)  ", storage[i], beFunction(function_input, gcx, storage[i]));
			}
			fprintf(pFile, "\n���ֵ�ֱ�Ϊ��");
			for (i = 0; i < js1; i++)
			{
				fprintf(pFile, "%f  ", beFunction(function_input, gcx, storage[i]));
			}
			fprintf(pFile, "\n");
		}

		js1 = minp(function_input, gcx, storage, low, high);
		if (js1 <= 0)
		{
			fprintf(pFile, "�����ļ�Сֵ�������������ڼ�Сֵ��\n");
		}
		else
		{
			fprintf(pFile, "�����ļ�Сֵ��Ϊ��");
			for (i = 0; i < js1; i++)
			{
				fprintf(pFile, "(%f,%f)  ", storage[i], beFunction(function_input, gcx, storage[i]));
			}
			fprintf(pFile, "\n��Сֵ�ֱ�Ϊ��");
			for (i = 0; i < js1; i++)
			{
				fprintf(pFile, "%f  ", beFunction(function_input, gcx, storage[i]));
			}
			fprintf(pFile, "\n");
		}

		fprintf(pFile, "�����ĵ�������Ϊ��");
		js1 = diff1(function_input, storage, gcx);
		mono_r(storage, js1, low, high,pFile);
		fprintf(pFile, "\n");
		printf("�����������ļ��ѳɹ����棬����Ϊ��Ŀ¼�µ�Result.txt�ļ��С�\n");
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
			fprintf(o_char,"����Ϊ������������");
		}
		else if (d0 < 0)
		{
			fprintf(o_char, "����Ϊ������������");
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
	printf("������������%d\n", i);
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
		printf("�������޷����dat�ļ���\n");
		return 0;
	}
	for (float i = -50; i < 50; i++)
	{
		fprintf(pFile,"%.0f %.0f\n", i, beFunction(input_fun, igcx, i));
	}
	fclose(pFile);
	printf("�븴��  plot \"plot.dat\" with linespoin ���л�ͼ����\n");
	printf("��ע�⣬���밲װgnuplotͼ������ſ����У�����ϵͳ������\n");
	printf("���������exit�˳���\n");
	system("gnuplot");
	return 0;
}