#include "kernel/types.h"
#include "user/user.h"


// 创建新管道供未来的子进程使用，将数据搬入后返回创建的管道
void porter(int lpipe[2], int rpipe[2], int first)
{   
    int buffer;
    while (read(lpipe[0], &buffer, sizeof(int)))
    {   
        if (buffer % first != 0)
        {
            write(rpipe[1], &buffer, sizeof(int));
        }
        
    }
}


void primes(int lpipe[2])
{   
    // 上来先关闭左侧写管道
    close(lpipe[1]);

    // 打印开头的第一个素数
    int first;
    if (read(lpipe[0], &first, sizeof(int)) == sizeof(int))
    {
        fprintf(1, "%d\n", first); 
    }else{
        exit(0);
    }
    

    // 创建新管道供未来的子进程使用，将数据搬入后返回创建的管道
    int rpipe[2];
    pipe(rpipe);
    porter(lpipe, rpipe, first);
    
    // 搬运完数据后就关闭左侧进程的读管道和右进程的写管道
    close(lpipe[0]);

    // 创建进程
    if (fork() == 0){
        primes(rpipe);
    }else{
        close(rpipe[0]);
        close(rpipe[1]);
        wait(0);
        exit(0);
    }
}


int main(int argc){
    int fd[2];
    pipe(fd);
    for (int i = 2; i < 36; i++)
    {
        write(fd[1], &i, sizeof(int));
    }
    
    primes(fd);
    exit(0);
}

