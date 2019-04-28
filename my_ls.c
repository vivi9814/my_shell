#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int fetch_input(char *a) {
	char p;
	int num  = 0;

	//getchar를 이용해서 사용자가 엔터를 하기 전까지 입력을 받는다.
	//만약 사용자의 입력이 입력 최대길이를 넘어간다면 더이상 입력을 받지 않는다.
	while (((p = getchar()) != '\n') && (num < MAX_LEN)) {
		a[num++] = p;
	}
	//입력최대길이를 넘어설 경우 경고문을 발생시킨다.
	if (num == MAX_LEN && p != '\n') {
		perror("length error");
		num = -1;
	} else {
		a[num] = 0;
	}
	//엔터를 하기 전까지 계속 입력을 받는다.
	while (p != '\n') p = getchar();
	return num;
}


int parse(char *buffer, int length, char* args[]) {
	int args_num = 0, last = -1, i;
	args[0] = NULL;
	//입력받은 문자열을 파싱한다.
	for (i = 0; i <= length; ++i) {
		//뛰어쓰기나 탭 처리
		if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i]) {
			continue;
		} else {
			if (last != i-1) {
				//입력받은 인자의 메모리 공간을 동적으로 할당 받는다.
				args[args_num] = (char*)malloc(i-last);
				if (args[args_num] == NULL) {
					perror("error");
					return 1;
				}
				memcpy(args[args_num], &buffer[last+1], i-last-1);
				args[args_num][i-last] = 0;
				args[++args_num] = NULL;
			}
			last = i;
		}
	}
	return args_num;
}



int main(void) {

	char *args[MAX_LEN/2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */

	char buffer[MAX_LEN + 1];
	memset(buffer, 0, sizeof(buffer));

	int length, args_num;

	//쉘 시작 하기 전 안내 문구
	printf("\n\n\n");
	printf("[CSE4009 SYSTEM PROGRAMMING] TERM PROJECT\n");
	printf("나만의 쉘 만들기\n");
	printf("소프트웨어학부 2017011985 박예진\n");
	printf("이 프로그램은 ls 기능만 별도로 구현하였습니다.\n");
	printf("다른 기능은 my_shell프로그램을 사용해주세요.\n");
	printf("자세한 사용법은 위키를 참고해주세요.\n");
	printf("https://github.com/vivi9814/my_shell/wiki/My-Own-Linux-Shell-in-C\n");
	printf("주의사항1 : 입력할 수 있는 최대 문자열 길이는 100자입니다.\n");
	printf("주의사항2 : 현재 위치를 출력할 때 위치의 문자열 길이가 1024를 초과하면 에러가 발생합니다.\n");
	printf("\n\n\n");
	while (should_run) {
		printf("my_shell>");
		//flush는 버퍼가 차지 않았어도 강제로 출력을 해준다.
		fflush(stdout);

		length = fetch_input(buffer);
		if (length == -1) continue;
	
		args_num = parse(buffer, length, args);

		//입력된 인자의 갯수가 0개이면 입력이 없다.
		//빈입력을 했을 경우, no_input전까지의 코드를 건너뛴다.
		if (args_num == 0)
			goto no_input;

		// 		5. 쪼개진 문자열이 ls와 같다면 현재 위치의 파일 및 폴더를 출력한다.
		// ls의 옵션은 아직 구현되지 않았다.
		// ls기능은 정상 작동하나 ls를 사용하고 나면 그 다음 명령어까지만 실행이 되고 이후 랜덤한 시간동안 쉘에 명령어 기능이 되지않음.
		int myls(char *path){
				DIR *dp;
				struct dirent *dirt;
				dp = opendir(".");
				if (!dp) {
					perror("myls error\n");
					exit(1);
				}
				while( dirt = readdir(dp)){
					if(dirt->d_name != "." || ".."){
						printf("%s\t", dirt->d_name);
					}
				}
				printf("\n");
				closedir(dp);			
		}

		if (strcmp(args[0], "myls") == 0) {
			if (args_num >= 2) {
				printf("myls에는 아직 별도의 옵션이 구현되지 않았습니다. ls명령어를 사용해주세요.\n");
			}
			else {
				// if ((dp = opendir(".")) == NULL){
				// 	printf("open current dir error\n");
				// }
				// else{
					char *pwd = getenv("PWD");
					myls(pwd);
			}
			exit(0);
			// goto no_input;
			// continue;
		}

	}
}