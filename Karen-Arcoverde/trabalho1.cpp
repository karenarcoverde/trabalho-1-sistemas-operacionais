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
	pid_t pid_filho;

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
	pid_filho = fork();

	if (pid_filho < 0){ //se ocorrer erro 
		cout << "Falha no fork " << endl;
		exit(EXIT_FAILURE);
	}

	//pai
	if (pid_filho > 0){
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
		aleatorio = rand() % 100;

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
}

// tarefa 2
void sig_handler_2 (int comandoParaExecutar){


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

		if (SIGUSR1_BOOL)
			comandoParaExecutar = sig_handler_1 (comandoParaExecutar);
			
		if (SIGUSR2_BOOL)
			sig_handler_2 (comandoParaExecutar);

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