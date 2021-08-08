#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <cstdlib>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <wait.h>
#define LER                             0
#define ESCREVER                        1

// essas variaveis servem para escolher o sinal
bool SIGUSR1_BOOL = false;
bool SIGUSR2_BOOL = false;
bool SIGTERM_BOOL = false;

using namespace std;

// tarefa 1
int sig_handler_1 (int comandoParaExecutar){
	time_t t;
	int aleatorio;
	pid_t pid_filho_tf1;

	srand((unsigned) time(&t)); //tempo como argumento do srand

	int pipe1[2]; 
	int pipe2[2];
	// 0 - leitura
	// 1 - escrita

	//pai cria pipe
	if (pipe(pipe1) == -1)
		cout << "Falha no Pipe " << endl;

	if (pipe(pipe2) == -1)
		cout << "Falha no Pipe " << endl; 

	//pai cria processo filho
	pid_filho_tf1 = fork();

	if (pid_filho_tf1 < 0){ //se ocorrer erro ao criar processo filho
		cout << "Falha no fork " << endl;
		exit(EXIT_FAILURE);
	}

	//pai
	if (pid_filho_tf1 > 0){
		//pai espera finalizacao do filho
		wait(NULL);

		//pai le o numero aleatorio sorteado pelo filho e coloca em uma variavel comandoParaExecutar 
		read(pipe2[LER], &comandoParaExecutar, sizeof(comandoParaExecutar));
		//pai fecha qualquer ponta aberta do pipe
		close(pipe2[LER]);
	}
	
	//filho
	else{
		//sorteio de numero aleatorio entre 1 e 100
		aleatorio = rand() % 100 + 1;

		//imprime numero aleatorio
		cout << aleatorio << endl;

		//envia para o pai o numero aleatorio
		write(pipe2[ESCREVER], &aleatorio, sizeof(aleatorio));
		//filho fecha qualquer ponta aberta do pipe
		close(pipe2[ESCREVER]);

		//forca finalizacao do filho para ir ao pai
		exit(EXIT_SUCCESS);
	}	
	return comandoParaExecutar;

// Referências usadas:
// https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm
// https://www.youtube.com/watch?v=D88bdcJH6Jw&ab_channel=RodrigodeSouzaCouto
// https://linux.die.net/man/2/write
// https://linux.die.net/man/3/read
// https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/
// https://man7.org/linux/man-pages/man2/pipe.2.html
// https://linuxhint.com/pipe_system_call_c/
// Livro: Fundamentos de Sistemas Operacionais - Nona Edição - Autores: Abraham Silberschatz, Peter Baer Galvin e Greg Gagne
// https://www.cplusplus.com/reference/cstdlib/rand/
}

// tarefa 2
void sig_handler_2 (int comandoParaExecutar){
	pid_t pid_filho_tf2;
	//pai cria processo filho
	pid_filho_tf2 = fork();

	if (pid_filho_tf2 < 0){ //se ocorrer erro ao criar processo filho
		cout << "Falha no fork " << endl;
		exit(EXIT_FAILURE);
	}

	//pai
	if (pid_filho_tf2 > 0){
		//pai espera finalizacao do filho
		wait(NULL);
	}

	//filho
	else{
		//se for 0 imprime mensagem na tela e finaliza a execucao
		if (comandoParaExecutar == 0)
			cout << "Nao ha comando a executar " << endl;
		
		//filho chama o exec para executar o ping, faz o caso 2: se for diferente de zero e numero par
		if ((comandoParaExecutar != 0) and (comandoParaExecutar % 2 == 0))
			execlp("/bin/ping","ping","8.8.8.8","-c","5",NULL);

		//filho chama o exec para executar o ping, faz o caso 3: se for numero impar
		else if ((comandoParaExecutar !=0) and (comandoParaExecutar % 2 != 0))
			execlp("/bin/ping","ping","paris.testdebit.info","-c","5","-i","2",NULL);

		exit(EXIT_SUCCESS);
	}

	// Referências usadas:
	// pingExample.c fornecido pelo professor
	// https://www.geeksforgeeks.org/exec-family-of-functions-in-c/
	// https://www.hostinger.com.br/tutoriais/comando-ping-linux
}

void sig_handler_term (){
	cout << "Finalizando o disparador... " << endl;
}

void sig_handler (int sig){

	switch (sig){
	case SIGUSR1:
		SIGUSR1_BOOL = true;
		break;

	case SIGUSR2:
		SIGUSR2_BOOL = true;
		break;
	
	case SIGTERM:
		SIGTERM_BOOL = true;
		break;
	default:
		break;
	}
}

int main (){

	pid_t pid; 
	int comandoParaExecutar = 0; //inicia com o valor zero (default) 
	
	//pega o valor do pid
	pid = getpid ();
	
	//imprime o pid
	cout << "PID: " << pid << endl;
	
	//disparador de sinais
	signal (SIGUSR1, sig_handler);
	signal (SIGUSR2, sig_handler);
	signal (SIGTERM, sig_handler);

	// apos a execucao de cada tarefa, o programa devera voltar a esperar sinais, por isso fica em um loop infinito
        while (true){
            cout << "Esperar sinais" << endl;
			sleep(15);

			if (SIGUSR1_BOOL){
				comandoParaExecutar = sig_handler_1 (comandoParaExecutar);
				SIGUSR1_BOOL = false;
			}
			
			if (SIGUSR2_BOOL){
				sig_handler_2 (comandoParaExecutar);
				SIGUSR2_BOOL = false;
			}

			if (SIGTERM_BOOL){
				sig_handler_term ();
				return 0;
			}
	}

	// Referências usadas: 
	// http://linguagemc.com.br/loop-infinito-em-c/
	// https://www.softwaretestinghelp.com/cpp-sleep/
	// https://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/ - Fonte fornecida pelo professor
	// Livro: Fundamentos de Sistemas Operacionais - Nona Edição - Autores: Abraham Silberschatz, Peter Baer Galvin e Greg Gagne
}