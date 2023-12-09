#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

int main(){
pid_t pid;
int ret = 1;
int status;
pid = fork();

if (pid == -1){

	// pid == -1 表示发生了错误
	printf("can't fork, error occured\n");
	exit(EXIT_FAILURE);
}
else if (pid == 0){

	// pid == 0 表示创建了子进程
	// getpid() 返回调用进程的进程id
	// 返回子进程的进程id
	printf("child process, pid = %u\n",getpid());
	// 返回子进程的父进程，即父进程本身
	printf("parent of child process, pid = %u\n",getppid());

	// argv list第一个参数应该指向
	// 与正在执行的文件关联的文件名
	// 数组指针必须以NULL结尾
	// 指针
	char * argv_list[] = {"ls","-lart","/home",NULL};

	// execv（）仅在发生错误时返回。
	// 返回值为-1
	execv("ls",argv_list);
	exit(0);
}
else{
	// 为pid返回一个正数
	// 父进程
	// getppid（）返回进程的父进程id
	// 调用进程
        // 返回父进程ID的父进程
	printf("Parent Of parent process, pid = %u\n",getppid());
	printf("parent process, pid = %u\n",getpid());


		// 父进程对子进程调用waitpid（）
		// waitpid（）系统调用暂停
		// 调用进程直到pid指定的子进程
		// 状态改变
		// 有关所有标志或选项，请参见wait（）手册
		if (waitpid(pid, &status, 0) > 0) {
			
			if (WIFEXITED(status) && !WEXITSTATUS(status))
			printf("program execution successful\n");
			
			else if (WIFEXITED(status) && WEXITSTATUS(status)) {
				if (WEXITSTATUS(status) == 127) {

					// execv failed
					printf("execv failed\n");
				}
				else
					printf("program terminated normally,"
					" but returned a non-zero status\n");			
			}
			else
			printf("program didn't terminate normally\n");		
		}
		else {
		// waitpid() failed
		printf("waitpid() failed\n");
		}
	exit(0);
}
return 0;
}