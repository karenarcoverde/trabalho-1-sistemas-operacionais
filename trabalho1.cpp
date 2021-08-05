#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <cstdlib>
#include <sys/types.h>

using namespace std;

void sig_handler_1 (int sig){


}

void sig_handler_2 (int sig){




}

void sig_handler_term (int sig){
	cout << "Finalizando o disparador... " << endl;
        exit(0); // zero indica que obteve sucesso na execucao, assim como o return 0

// Referência usada:
// https://www.cmmprogressivo.net/2019/12/Funcao-exit-sair-terminar-Cpp.html
}

int main (){

	pid_t pid; 
	int comandoParaExecutar = 0; //inicia com o valor zero (default) 
	
	//pega o valor do pid
	pid = getpid ();
	
	//imprime o pid
	cout << "PID: " << pid << endl;
	
	//disparador de sinais
	signal (SIGUSR1, sig_handler_1);
	signal (SIGUSR2, sig_handler_2);
	signal (SIGTERM, sig_handler_term);

	// apos a execucao de cada tarefa, o programa devera voltar a esperar sinais, por isso fica em um loop infinito
	while (true){
		cout << "Esperar sinais" << endl;
		sleep (5);
	};

	return 0;

	// Referências usadas: 
	// http://linguagemc.com.br/loop-infinito-em-c/
	// https://www.softwaretestinghelp.com/cpp-sleep/
	// https://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/ - Fonte fornecida pelo professor
	// Livro: Fundamentos de Sistemas Operacionais - Nona Edição - Autores: Abraham Silberschatz, Peter Baer Galvin e Greg Gagne
}
