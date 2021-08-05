#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <cstdlib>
#include <sys/types.h>


// essas variaveis servem para escolher o sinal
bool SIGUSR1_BOOL = false;
bool SIGUSR2_BOOL = false;
bool SIGTERM_BOOL = false;


using namespace std;

void sig_handler_1 (){


}

void sig_handler_2 (){


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
			sig_handler_1 ();

		if (SIGUSR2_BOOL)
			sig_handler_2 ();

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
