/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */
#include <stdlib.h> /* exit() */
#include <sys/types.h> /* define pid_t */
#include <unistd.h> /* fork() */
#include <stdio.h> /*printf() e scanf()*/
#include <sys/mman.h> /*mmap()*/

#define TRUE 1
#define FALSE 0
#define N_THREADS 4/*Numero de threads permitidas*/
#define TAM_MAX 200 /*Numero maximo de entradas suportadas*/

typedef unsigned long long ll;

/*Contador de primos compartilhado*/
ll contador = 0;

/*Tamanho do vetor de entrada*/
int tam_entrada = 0;

/*Trava para regiao critica*/
pthread_mutex_t trava;

/*Indica se o numero na posicao correspondente do vetor de entradas ja foi tratado*/
int resolvido[TAM_MAX]; 

/*Vetor de entrada entrada*/
ll entrada[TAM_MAX];

/*Verifica se um numero eh primo*/
/*Retorna 1 caso seja primo, e 0 caso nao seja*/
int primo(ll num){
	ll i;

	if(num == 2)
		return TRUE;

	if(num == 1 || num%2 == 0)
		return FALSE;

	for(i = 3; i*i <= num; i+=2){
		if(num%i == 0)
			return FALSE;
	}

	return TRUE;
}

void worker(void *args){
	int i;
	
	while(1){
		/*Encontra um caso para resolver (regiao critica)*/
		pthread_mutex_lock(&trava);
		for(i = 0; i < tam_entrada; i++)
			if(resolvido[i] == 0)
				break;
		/*Se acabaram os casos nao resolvidos, termina*/
		if(i == tam_entrada){
			pthread_mutex_unlock(&trava);
			return;
		}
		
		/*Se encontrou caso nao resolvido, marca que vai resolver*/
		resolvido[i] = 1;
		pthread_mutex_unlock(&trava);
		
		/*Resolve caso*/
		if(primo(entrada[i]))
			contador++;
	}
	
}

int main() {
	int i = 0;
	
	/*Vetor para guardar indentificadores das threads*/
	pthread_t threads[N_THREADS];
	void *args;
	args = NULL ;/*Soh para evitar warning*/

	/*Le numeros*/
	while(scanf("%llu", &entrada[tam_entrada]) == 1) 
		tam_entrada++;
	
	/*Inicia vetor*/
	for(i = 0; i < tam_entrada; i++)
		resolvido[i] = 0;
	
	/*Cria threads*/
	for(i = 0; i < N_THREADS; i++)
	        pthread_create(&(threads[i]), NULL, worker, args);

	/*Espera todas as threads terminarem*/
	for(i = 0; i < N_THREADS; i++)
		pthread_join(threads[i], NULL);

	printf("%llu\n", contador);

 	return 0;
}
