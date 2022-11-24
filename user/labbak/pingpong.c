#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pp1[2]; // 父进程向子进程发送消息管道
    int pp2[2]; // 子进程向父进程发送消息管道

    pipe(pp1);
    pipe(pp2);

    char buf[] = {'a'};
    int len = sizeof(buf);

    int pid = fork();
    if (pid == 0)
    {
        close(pp1[1]);
        close(pp2[0]);

        if (read(pp1[0], buf, len) != len)
        {
            exit(1);
        }

        printf("%d: received ping\n", getpid());

        if (write(pp2[1], buf, len) != len)
        {
            exit(1);
        }

        exit(0);
    }

    close(pp1[0]);
    close(pp2[1]);

    if (write(pp1[1], buf, len) != len)
    {
        exit(1);
    }

    if (read(pp2[0], buf, len) != len)
    {
        exit(1);
    }
    printf("%d: received pong\n", getpid());

    wait(0);
    exit(0);
}