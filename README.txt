Trabalho 1 de Sistemas Operacionais (EEL770) do período de 2021.1
Aluna: Karen dos Anjos Arcoverde

Sistema operacional usado: Linux
Distribuição: Ubuntu 20.04.2 LTS

O pid do processo é impresso na tela assim que o programa executa.

Para compilar o programa, digite no terminal:
make

Para executar o programa, digite no terminal:
./trabalho1

Para limpar os objetos e o executável, digite no terminal:
make clean
 
Para usar o programa, digite em outro terminal para que sinais sejam disparados: 
1) KILL -SIGUSR1 <pid> -> Executa a Tarefa 1
2) KILL -SIGUSR2 <pid> -> Executa a Tarefa 2
3) KILL -SIGTERM <pid> -> Finaliza o programa

Explicação resumida de cada tarefa:
Tarefa 1 -> Gera número aleatório de 1 a 100 e imprime na tela
Tarefa 2 -> Se o número for par e diferente de zero: Executa "ping 8.8.8.8 -c 5"
	    Se o número for ímpar e diferente de zero: Executa "ping paris.testdebit.info -c 5 -i 2"
	    Se o número for zero: Imprime "Não há comando a executar" e finaliza
