#include<stdio.h>
#include<string.h>
#include<iostream>
#include"choose.cpp"
#include"regex.h"

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
	if (status == REG_NOMATCH){
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

	if (fp1 ) {
		while ((fgets(FunName, 100, fp1)) != NULL) {
			//printf("行内容获取成功！\n");
			//printf("%s\n", FunName);
			if (FunNameCheck(FunName)) {
				//printf("%s\n", FunName);
				fputs(FunName, fp2);
			}
		}
	}
	else {
		printf("Can't open this file!");
	}
}

void FunGetFromList(char Dir[]) {

}

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
		case hash_str_to_uint32("-dir"):
			FunGetFromList(UserParameter);
		case hash_str_to_uint32("-tree"):
			break;
		default:
			printf("Un!");
	}

	return 0;
}
