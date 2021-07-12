#include<stdio.h>
#include<string.h>
#include<iostream>
#include"choose.cpp"
#include"regex.h"
#include"dirent.h"

int FunNameCheck(char str[]) {
	int flag = 0;
	int status, i;
	int cflags = REG_EXTENDED;
	regmatch_t pmatch[1];
	const size_t nmatch = 1;
	regex_t reg;
	const char* pattern = "^(\\w+)\\s+[\\*,&]*\\s*(\\w+)\\s*\\(\.*$";
	char* buf = str;
	//编译正则模式
	regcomp(&reg, pattern, cflags);
	//执行正则表达式和缓存的比较
	status = regexec(&reg, buf, nmatch, pmatch, 0);
	if (status == REG_NOMATCH) {
		//printf("No match\n");
		flag = 0;
	}
	else {
		//printf("比较成功\n");
		flag = 1;
	}
	regfree(&reg);
	return flag;
}

void FunGetFromFile(char FileName[]) {

	FILE* fp1 = fopen(FileName, "r");
	FILE* fp2 = fopen(strcat(FileName, "_FunName.txt"), "w");
	char FunName[100] = { 0 };
	extern int g_FunNum;

	if (fp1) {
		while ((fgets(FunName, 100, fp1)) != NULL) {
			//printf("行内容获取成功！\n");
			//printf("%s\n", FunName);
			if (FunNameCheck(FunName)) {
				//printf("%s\n", FunName);
				g_FunNum++;
				fprintf(fp2, "%5d-", g_FunNum);
				fputs(FunName, fp2);
			}
		}
		fclose(fp1);
		fclose(fp2);
	}
	else {
		printf("Can't open this file!");
	}
}

void FunGetFromFile_2(char FileName[],FILE* fp) {

	FILE* fp1 = fopen(FileName, "r");
	char FunName[100] = { 0 };
	extern int g_FunNum;

	if (fp1) {
		while ((fgets(FunName, 100, fp1)) != NULL) {
			//printf("行内容获取成功！\n");
			//printf("%s\n", FunName);
			if (FunNameCheck(FunName)) {
				//printf("%s\n", FunName);
				g_FunNum++;
				fprintf(fp, "%5d-", g_FunNum);
				//printf("%5d-", g_FunNum);
				fputs(FunName, fp);
				//printf("%s\n", FunName);
			}
		}
		fclose(fp1);
	}
	else {
		printf("Can't open this file!");
	}
}

void FunGetFromList(char FilePath[]) {

	DIR* dp = opendir(FilePath);
	struct dirent* entry;
	char *FileName;
	int len = 0;
	FILE* fp = fopen(strcat(FilePath, "_FunName.txt"), "w");

	if (dp) {
		while ((entry = readdir(dp)) != nullptr) {
			//printf("%s\n", entry->d_name);
			FileName = entry->d_name;
			//printf("%s\n", FileName);
			len = strlen(FileName);
			//printf("%d\n", len);
			if (FileName[len - 1] == 'c') {
				//printf("这是C文件！\n");
				fprintf(fp, FileName);
				fprintf(fp, "\n");
				FunGetFromFile_2(FileName,fp);
			}
		}
		fclose(fp);
		closedir(dp);
	}
	else {
		printf("打开目录失败！\n");
	}


}

int g_FunNum = 0;

int main() {

	char UserCmd[20] = { 0 };
	char UserParameter[100] = { 0 };
	char* ExistCmd[] = { "-file", "-dir", "-tree" };
	//for (int i = 0; i < 3; i++) {
	//	printf("%s\n", ExistCmd[i]);
	//}

	scanf("%s", UserCmd);
	scanf("%s", UserParameter);
	//printf("%s\n", UserCmd);
	//printf("%s\n", UserParameter);

	switch (hash_str_to_uint32(UserCmd)) {
	case hash_str_to_uint32("-file"):
		FunGetFromFile(UserParameter);
		printf("函数名获取成功！\n");
		break;
	case hash_str_to_uint32("-dir"):
		FunGetFromList(UserParameter);
		printf("函数名获取成功！\n");
		break;
	case hash_str_to_uint32("-tree"):
		break;
	default:
		printf("Un!");
	}

	return 0;
}
