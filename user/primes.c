#include "kernel/types.h"
#include "user/user.h"

// 文件描述重定向
void mapping(int fd, int pip[])
{
    close(fd);

    dup(pip[fd]);
    close(pip[0]);
    close(pip[1]);
}

void primes()
{
    int pp2[2];
    pipe(pp2);

    int fst, next;

    if (read(0, &fst, sizeof(int)))
    {
        printf("prime %d\n", fst);

        int pid2 = fork();
        if (pid2 == 0)
        {
            mapping(1, pp2);
            while (read(0, &next, sizeof(int)))
            {
                if (next % fst != 0)
                {
                    write(1, &next, sizeof(int));
                }
            }
        }
        else
        {
            wait(0);
            mapping(0, pp2);
            primes();
        }
    }
}

int main(int argc, char *argv[])
{
    int pp1[2];
    pipe(pp1);
    int pid = fork();
    if (pid == 0)
    {
        mapping(1, pp1);

        for (int i = 2; i <= 35; ++i)
        {
            write(1, &i, sizeof(int));
        }
    }
    else
    {
        wait(0);
        mapping(0, pp1);
        primes();
    }

    exit(0);
}