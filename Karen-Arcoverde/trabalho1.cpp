#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <cstdlib>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <wait.h>

// essas variaveis servem para escolher o sinal
bool SIGUSR1_BOOL = false;
bool SIGUSR2_BOOL = false;
bool SIGTERM_BOOL = false;

using namespace std;

// tarefa 1
int sig_handler_1 (int comandoParaExecutar){
	time_t t;
	int num_aleat;

	srand((unsigned) time(&t));

	int pipe1[2], pipe2[2];

	//pai cria pipe
	pipe(pipe1);
	pipe(pipe2);

	//pai cria processo filho
	pid_t pid_filho = fork();

	
	if(pid_filho >= 0){ //sucesso em criar filho
		//filho
		if (pid_filho == 0){
			//sorteio de numero aleatorio entre 1 e 100
			num_aleat = rand() % 100;

			//imprime numero aleatorio
			cout << num_aleat << endl;

			//envia para o pai o numero aleatorio
			write(pipe2[1], &num_aleat, sizeof(num_aleat));
			//filho fecha qualquer ponta aberta do pipe
			close(pipe2[1]);

			//forca finalizacao do filho para ir ao pai
			exit(EXIT_SUCCESS);
		}	
		//pai
		else{
			//pai espera finalizacao do filho
			int status = 0;
			pid_t aux_pid;
			while((aux_pid = wait(&status)) > 0);

			//pai le o numero aleatorio sorteado pelo filho e coloca em uma variavel comandoParaExecutar 
			read(pipe2[0], &comandoParaExecutar, sizeof(comandoParaExecutar));
			//pai fecha qualquer ponta aberta do pipe
			close(pipe2[0]);
		}
	}
	return comandoParaExecutar;
// Referências usadas:
// https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm
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