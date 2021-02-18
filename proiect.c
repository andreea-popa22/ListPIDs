#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void listPID(pid_t p_curent)
{

    ///printf("%d\n", p_curent);

    int pipefd[2];
    int pipefd2[2];
    
    pipe(pipefd);
    pipe(pipefd2);

    int j;
    for(j=1;j<=2;j++){
        if(fork()==0){
            if(j==1){
                close(pipefd[0]);

                dup2(pipefd[1],1);

                close(pipefd[1]);

                char *kbuf = calloc(100, 100 * sizeof(char));
                sprintf(kbuf, "%d", p_curent);

                char *argv[] ={"pgrep","-P",kbuf,"-d:",NULL};
                execve("/usr/bin/pgrep",argv,NULL);
            }
            else if(j==2){
                close(pipefd2[0]);

                dup2(pipefd2[1],1);

                close(pipefd2[1]);

                char *kbuf2 = calloc(100, 100 * sizeof(char));
                sprintf(kbuf2, "%d", p_curent);
                char *argv[] ={"ps","-o","pid,user,%mem,stat","-p",kbuf2,NULL};
                execve("/bin/ps",argv,NULL);

            }

        }
    }
    int nr2;
    char buffer2[1000];

    close(pipefd2[1]);

    nr2=read(pipefd2[0],buffer2,sizeof(buffer2));
    buffer2[nr2]=0;
    printf("%s",buffer2);

    int nr;
    int v[1000];
    char buffer[1000];

    close(pipefd[1]);

    nr=read(pipefd[0],buffer,sizeof(buffer));
    buffer[nr]=0;

    char *p = strtok(buffer, ":");
    int i = 0;
    while (p != NULL)
    {
        v[i] = atoi(p);
        i++;
        p = strtok(NULL, ":");
    }
    for (int j = 0; j < i; j++)
        listPID(v[j]);
    

}

int main(int argc, char *argv[])
{
    if(argc==2){
        listPID(atoi(argv[1]));
    }
    else{
        printf("Numar incorect de argumente.\n");
        return 1;
    }
}