#include<stdio.h>
#include<signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "task1.h"

int main()
{
	int p1, p2;
   if((p1 = fork()) == -1) {
      printf("Child Process 1 create error!"); 
   }
   if(p1 == 0) {
         islock = 1;
         signal(SIGINT, SIG_IGN); //忽略Ctrl+C信号
         signal(16, unlock); //设置信号，当接收到16信号时执行解锁函数
         lock(); //锁定，接收到父进程发来的16信号时，解锁
         srand((unsigned) time(NULL)); //随机数种子函数设置
         int random_1 = rand() % 10; //产生一个随机数
         printf("Child Process 1's random num is %d\n", random_1);
         printf("Child Process 1 is Killed by Parent!\n");
         exit(random_1); //将产生的随机数返回给父进程
   }
   else {
      if((p2 = fork()) == -1) {
         printf("Child Process 2 create error!");
      }
      if(p2 == 0) {
         islock = 1;
         signal(SIGINT, SIG_IGN); //忽略Ctrl+C信号
         signal(16, unlock); //设置信号，当接收到16信号时执行解锁函数
         lock(); //锁定，接收到父进程发来的16信号时，解锁
         
         srand((unsigned) time(NULL)); //随机数种子函数设置
         int random_2 = rand() % 10; //产生一个随机数
         printf("Child Process 2's random num is %d\n", random_2);
         printf("Child Process 2 is Killed by Parent!\n");
         exit(random_2); //将随机数返回给父进程
      }
      else {
         // 此处为父进程
         islock = 1; //父进程锁定
         signal(SIGINT, unlock);//注册信号，当接收到Ctrl+C信号时解锁
         lock(); //锁定夫程序
         int p1_num, p2_num;
         kill(p1, 16);//给子程序p1提供16信号，使子程序1解锁
        
    
	      int child_ret1, child_ret2; //用于接受子程序的返回值
         int pid1=wait(&child_ret1); //获取子程序1的返回值
         int num1 = WEXITSTATUS(child_ret1);
         printf("Parent process get Child Process 1's random num: %d\n", WEXITSTATUS(child_ret1)); //输出子程序1的返回值
	        
         /*
         printf("wait process's pid=%d,status=0x%X,exit value=%d(0x%X)\n",pid1,child_ret1,
				WEXITSTATUS(child_ret1),WEXITSTATUS(child_ret1)); //子程序所有返回信息
         */
         sleep(3); //由于子程序1和子程序2时间太接近会使得产生的随机数相同，因此先暂停3s
         kill(p2, 16); //给子程序2提供16信号，使子程序2解锁
         int pid2=wait(&child_ret2); //获取子程序2的返回值
         int num2 = WEXITSTATUS(child_ret2);
         printf("Parent process get Child Process 2's random num: %d\n", WEXITSTATUS(child_ret2));//输出子程序2的返回值

         //对比子程序1和子程序2的随机数值，并根据大小情况执行不同分支
         if(num1 <= num2) {
                      printf("Child Process 2's random number is bigger\n");
                      if(execl("ls","ls","-l",NULL) == -1) {
                           perror("error execl"); //此execl不会执行成功，因为无法找到对应程序执行，程序会输出错误信息
                      }
         }
         else {
                      printf("Child Process 1's random number is bigger\n");
                      if(execl("/bin/ls","ls","-l",NULL)==-1){ 
			                  perror("erro execl");//此execl会执行成功，并输出对应信息
                      }
         }
         printf("Parent Process is Killed!\n");
         exit(0);
      }
   }
}
