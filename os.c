#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#define TRUE 1
#define CANTIDAD_PROCESOS  10

int listaProcesos[CANTIDAD_PROCESOS];

void procesoHijo(){
    kill(getpid(), SIGSTOP);
    
    for(;;){
        sleep(1);
        printf("\n[%i]\n",getpid());
        fflush(stdout);
    }
}

void sighandler(int signum){
    for(int i = 0; i < CANTIDAD_PROCESOS; i++){
        if(listaProcesos[i] == -1){
            pid_t nuevoProceso = fork();
            listaProcesos[i] = nuevoProceso;

            if(nuevoProceso == 0)
                procesoHijo();

            break;
        }
    }
}

int main()
{
    printf("Proceso padre %i\n",getpid());

    for(int i = 0; i < CANTIDAD_PROCESOS; i++)
        listaProcesos[i] = -1;

    signal(SIGUSR1,sighandler);

    printf("Iniciando ciclo infinito\n");
    while(TRUE){
        for(int i = 0;i < CANTIDAD_PROCESOS; i++){
            if(listaProcesos[i] != -1){
                printf("\nDespertando a [%i]...\n",listaProcesos[i]);
                kill(listaProcesos[i],SIGCONT);
                sleep(3);
                printf("\nAhora lo mando a dormir\n");
                kill(listaProcesos[i],SIGSTOP);
                sleep(1);
            }
        }
    }
}