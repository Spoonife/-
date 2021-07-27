#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include"regex.h"
#include"dirent.h"
#define N 1024
#define M 100

//申请动态内存
//char* SpaceGet(char* str) {
//	str = (char*)malloc(sizeof(char) * N);
//	if (str) {
//		memset(str, 0, sizeof(str));
//	}
//	else {
//		printf("动态内存分配失败！\n");
//	}
//	return str;
//}

//处理文件名后缀
char* FileNameClean(char* str,int i) {
	int len = 0;
	char fileName[100] = { 0 };
	len = strlen(str);
	//去掉文件名的最后i个字符
	strncpy(fileName, str, len - i);
	return fileName;
}

//处理目录名
char* ListNameClaen(char* str) {
	char dirName[N] = { 0 };
	char fileName[N] = { 0 };
	int len = 0;
	int i = 0;
	len = strlen(str);

	//当用户输入目录为磁盘下时
	if ('\\' == str[len - 1] &&  '\\' == str[len - 2]) {
		strcpy(dirName, str);
		strncat(dirName, str, 1);
	}
	else {
		//去掉参数中最后的\反斜杠
		strncpy(dirName, str, len - 1);
		dirName[len] = { 0 };
		len = strlen(dirName);
		//获取最后一级目录名的长度
		while ('\\' != dirName[len - 1]) {
			len--;
			i++;
		}
		len = strlen(dirName);
		//暂存最后一级目录名
		strncpy(fileName, dirName + len - i - 1, len);
		fileName[i + 1] = { 0 };
		//拼接最后一集目录名
		strcat(dirName, fileName);
	}
	return dirName;
}

//检查用户命令
int UserCmdCheck(char* str) {
	int res = 0;
	char* userCmd = nullptr;//存用户命令
	userCmd = (char*)malloc(sizeof(char) * N);

	if (userCmd) {
		memset(userCmd, 0, sizeof(userCmd));
		strcpy(userCmd, str);
		//判断用户命令为-file还是-dir
		if (strcmp(userCmd, "-file") == 0) {
			res = 1;
		}
		else if (strcmp(userCmd, "-dir") == 0) {
			res = 2;
		}
		else if (strcmp(userCmd, "-help") == 0) {
			res = 3;
		}
		else {
			res = 0;
		}
		free(userCmd);
		userCmd = nullptr;
	}
	else {
		printf("内存空间分配失败！\n");
	}
	return res;
}

//检查用户参数是否适用-file
int UserCmdCheckFile(char* str) {
	int res = 0;
	int len = 0;
	char* userParameter = nullptr;//存用户参数
	userParameter = (char*)malloc(sizeof(char) * N);

	if (userParameter) {
		memset(userParameter, '0', sizeof(userParameter));
		strcpy(userParameter, str);
		len = strlen(userParameter);//获取文件名长度
		FILE* fp = fopen(userParameter, "r");
		//比较文件名的最后两个字符是否为.c，文件存在且是.c文件时，返回1
		if ((fp) && ('.' == userParameter[len - 2]) && ('c' == userParameter[len - 1])) {
			res = 1;
			fclose(fp);
		}
		else {
			if (fp) {
				fclose(fp);
			}
			res = 0;
		}
		free(userParameter);
		userParameter = nullptr;
	}
	else {
		printf("内存空间分配失败！\n");
	}
	return res;
}

//检查用户参数是否适用-dir
int UserCmdCheckLsit(char* str) {
	int res = 0;
	int len = 0;
	char* filePath = nullptr;
	filePath = (char*)malloc(sizeof(char) * N);

	if (filePath) {
		memset(filePath, 0, sizeof(filePath));
		strcpy(filePath, str);
		DIR* dp = opendir(filePath);
		//目录是否存在
		if (dp) {
			res = 1;
		}
		else {
			res = 0;
		}
		free(filePath);
		filePath = nullptr;
	}
	else {
		printf("内存空间分配失败！\n");
	}
	return res;
}

//函数名判断
int FunNameCheck(char* str) {
	int res = 0;
	int status = 0;
	int cflags = REG_EXTENDED;
	regmatch_t pmatch[1];
	regex_t reg;
	const size_t nmatch = 1;
	const char* pattern = "^(\\w+)[\*,&]*\\s+(\\w*)\\s*[\*,&]*\\s*[\*,&]*(\\w+)\\s*\\(\.*$";
	char* buf = str;

	//编译正则模式
	regcomp(&reg, pattern, cflags);
	//执行正则表达式和缓存的比较
	status = regexec(&reg, buf, nmatch, pmatch, 0);
	if (status == REG_NOMATCH) {
		//printf("No match\n");
		res = 0;
	}
	else {
		//printf("比较成功\n");
		res = 1;
	}
	regfree(&reg);
	return res;
}

//从文件中提取行内容
int FunGetFromFile(char* str, FILE* fp2) {
	int res = 0;
	int len = 0;
	char* funName = nullptr;//存函数名的数组
	char* fileName = nullptr;
	funName = (char*)malloc(sizeof(char) * N);
	fileName = (char*)malloc(sizeof(char) * N);
	memset(funName, 0, sizeof(funName));
	memset(fileName, 0, sizeof(funName));
	strcpy(fileName, str);
	extern int g_funNum;//记录函数名个数
	FILE* fp1 = fopen(fileName, "r");

	if (fp1) {
		//读取文件中每一行的内容
		while ((fgets(funName, M, fp1)) != NULL) {
			len = strlen(funName);
			//printf("%c\n", funName[len - 1]);
			//函数名判断
			if (';' != funName[len - 2] && FunNameCheck(funName)) {
				g_funNum++;
				//格式化输出到文件中
				fprintf(fp2, "%5d-", g_funNum);
				fputs(funName, fp2);
			}
		}
		fclose(fp1);
		free(funName);
		funName = nullptr;
		res = 1;
	}
	else {
		res = 0;
	}
	free(fileName);
	fileName = nullptr;
	return res;
}

//从目录下的文件中提取行内容
int FunGetFromList(char* filePath) {

	int res = 0;
	char* userParameter = nullptr;
	char* newFileName = nullptr;
	char* fileName = nullptr;
	userParameter = (char*)malloc(sizeof(char) * N);
	newFileName = (char*)malloc(sizeof(char) * N);
	fileName = (char*)malloc(sizeof(char) * N);

	if (newFileName && fileName && userParameter) {
		memset(userParameter, 0, sizeof(userParameter));
		memset(newFileName, 0, sizeof(newFileName));
		memset(fileName, 0, sizeof(fileName));
		//拼凑新文件名
		strcpy(userParameter, filePath);
		strcpy(newFileName, userParameter);
		//strcpy(newFileName, FileNameClean(newFileName, 1));
		//printf("%s\n", newFileName);
		strcpy(newFileName, ListNameClaen(newFileName));
		strcat(newFileName, "_FunName.txt");
		DIR* dp = opendir(userParameter);
		struct dirent* entry;
		FILE* fp = fopen(newFileName, "w");
		if (dp && fp) {
			//读取目录下文件
			while ((entry = readdir(dp)) != nullptr) {	
				strcpy(fileName, userParameter);
				//读取文件名
				strcat(fileName, entry->d_name);
				//printf("%s\n", fileName);
				//.c文件判断
				if (UserCmdCheckFile(fileName)) {
					//输出文件名
					fprintf(fp, entry->d_name);
					fprintf(fp, "\n");
					//从文件中提取函数名
					FunGetFromFile(fileName, fp);
				}
			}
			fclose(fp);
			closedir(dp);
			free(fileName);
			fileName = nullptr;
			res = 1;
		}
		else {
			res = 0;
		}
		free(newFileName);
		free(userParameter);
		newFileName = nullptr;
		userParameter = nullptr;
	}
	else {
		printf("内存空间分配失败！\n");
	}
	return res;
}

int g_funNum = 0;//记录函数个数

int main(int argc, char* argv[]) {

	char* userParameter = nullptr;
	char* userCmd = nullptr;
	char newFileName[M] = { 0 };
	int num = 0;
	int len = 0;
	extern int g_funNum;
	userCmd = (char*)malloc(sizeof(char) * N);
	userParameter = (char*)malloc(sizeof(char) * N);
	
	if (userCmd && userParameter) {
		memset(userCmd, 0, sizeof(userCmd));
		memset(userParameter, 0, sizeof(userParameter));
		strcpy(userCmd, argv[1]);
		//记录命令判断的结果
		num = UserCmdCheck(userCmd);
		switch (num) {
		//执行-file命令
		case 1: {
			strcpy(userParameter, argv[2]);
			for (int i = 3; i < argc; i++) {
				strcat(userParameter, " ");
				strcat(userParameter, argv[i]);
			}
			if (UserCmdCheckFile(userParameter)) {
				//拼接新文件名
				strcpy(newFileName, userParameter);
				strcpy(newFileName, FileNameClean(newFileName, 2));
				strcat(newFileName, "_FunName.txt");
				FILE* fp = fopen(newFileName, "w");
				//从文件中提取函数名
				FunGetFromFile(userParameter, fp);
				//重置函数名个数
				g_funNum = 0;
				fclose(fp);
			}
			else {
				printf("请输入正确的.c文件名！\n");
			}
			free(userParameter);
			break;
		}
		//执行-dir命令
		case 2: {
			strcpy(userParameter, argv[2]);
			for (int i = 3; i < argc; i++) {
				strcat(userParameter, " ");
				strcat(userParameter, argv[i]);
			}
			if (UserCmdCheckLsit(userParameter)) {
				len = strlen(userParameter);
				if ('\\' != userParameter[len - 1] || ':' == userParameter[len - 2]) {
					strcat(userParameter, "\\");
				}
				FunGetFromList(userParameter);
			}
			else {
				printf("请输入正确的目录名！\n");
			}
			free(userParameter);
			userParameter = nullptr;
			break;
		}
		//执行-help命令
		case 3: {
			printf("-file    提取.c文件中的函数名\n");
			printf("         -file <filename1>\n");
			printf("-dir	 提取目录下.c文件中的函数名\n");
			printf("         -dir  <filelist1>\n");
			printf("-help	 获取指令帮助\n");
			break;
		}
		default: {
			printf("无效指令！");
			printf("请输入-help获取帮助！\n");
		}
		free(userCmd);
		userCmd = nullptr;
		}
	}
	else {
		printf("内存空间分配失败！\n");
	}
	return 0;
}
