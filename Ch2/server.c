#include "task2.h"

int main() {
    int shmid;
    int key = 1022;
    pid_t p;
    char c;
    char *shmptr, *s;
    int cnt;
    setvbuf(stdout, NULL, _IONBF, 0); // 关闭输出缓存

    // 调用函数创建共享内存空间
    shmid = create_shmid(shmid, key);
    shmptr = create_shmptr(shmptr, shmid);

    printf("This is the server of drink shop, you can send your message to guests, please enter the message and press Enter to send\n");


    if ((p = fork()) < 0) {
        printf("Fork failed\n");
        exit(1);
    }

    while (1) {
        // 子进程接收数据
        if (p == 0) {
            while (*shmptr != 'c') {
                sleep(1);
            }
            printf("\n\033[43;37mReceive From Client:\33[0m ");
            cnt = 0;
            for (s = shmptr + 1; *s != '\0'; s++) {
                cnt++;
                putchar(*s);
            }
            printf("\nReceive Total %d letter", cnt);
            printf("\n>> ");
            *shmptr = '#';
        }
        // 父进程发送数据
        else{
            printf(">> ");
            scanf("%[^\n]", shmptr + 1);
            getchar(); // 吸收最后回车
            *shmptr = 's';
            while (*shmptr != '#') {
                sleep(1);
            }
            printf("\033[46;37mSuccessfully Sent\33[0m\n");
        }
    }
    return 0;
}
