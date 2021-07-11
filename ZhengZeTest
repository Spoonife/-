#include <stdio.h>
#include <sys/types.h>
#include <regex.h>

int main(int argc, char** argv)
{
	int status, i;
	int cflags = REG_EXTENDED;
	regmatch_t pmatch[1];
	const size_t nmatch = 1;
	regex_t reg;
	const char* pattern = "^(\\w+)\\s+[\\*,&]*\\s*(\\w+)\\s*\\(\.*$";
	char* buf = "int main(){";
	regcomp(&reg, pattern, cflags);//编译正则模式
	status = regexec(&reg, buf, nmatch, pmatch, 0);//执行正则表达式和缓存的比较
	if (status == REG_NOMATCH)
		printf("No match\n");
	else if (0 == status)
	{
		printf("比较成功\n");
	}
	regfree(&reg);
	return 0;
}
