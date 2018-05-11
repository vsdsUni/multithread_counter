/*Vitor Saldanha da Silveira
 *Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>

//declaração de variáveis globais
int soma = 0;
pthread_mutex_t trava_distribui_numeros;
pthread_mutex_t trava_soma;
int indice = 0;
int tam = 0;
unsigned long int entrada[64]; 

#define n_threads 3

int primo(int64_t n);
void* working_thread(void *arg);
int64_t distribui_numeros();

// ---------------- funçao main()-------------------
int main(){
//declaração das variáveis locais da main()
    pthread_t threads[n_threads];   
	int i = 0, j = 0; 	  

	// Lendo a entrada da String e vendo o tamanho dela 
    while( scanf("%lu", &entrada[i]) == 1 ){
    	i++;
    	if((entrada[i]!=' ')||(entrada[i]!= '\n')){
            tam++;
        }
    }
// Threads
    for(i = 0;(i<tam);i++){
        pthread_create(&(threads[i]), NULL, working_thread, NULL);

    }
//Esperando threads morrerem
	for (i = 0; i < n_threads; i++) {
		pthread_join(threads[i], NULL);
	}
	printf ("%d\n", soma);

	return 0;
}

//Esta funcao verifica se um dado numero inteiro positivo n é primo 
int primo(int64_t n){
	int64_t d,Primo;			
	if (n <= 1 || (n != 2 && n % 2 == 0)){
		Primo = 0;    /* nenhum numero inteiro <= 1 ou par > 2 e' primo */
	}
    else
        Primo = 1;		/* o numero e' primo ate que se prove o contrario */

    d = 3;
    while (Primo  && d <= n / 2) {
        if (n % d == 0)
            Primo = 0;

    d = d + 2;		/* testamos so' os  impares: 3, 5, 7... */
    }
    if (Primo)
        return 1;
    else 
        return 0;
}

// Thread trabalhando
void* working_thread(void *arg) {
    int i = 0, eprimo;
    int64_t numero;

    while(1){
        pthread_mutex_lock(&trava_distribui_numeros);
        numero = distribui_numeros();
        pthread_mutex_unlock(&trava_distribui_numeros);
        if(numero == -1){
            break;
        }
        eprimo = primo(numero);
        
        pthread_mutex_lock(&trava_soma);
        soma = soma + eprimo;
        pthread_mutex_unlock(&trava_soma);
        
    }
}

//  Da um numero diferente para cada threads usando mutexes e lock
int64_t distribui_numeros(){
    int64_t n;
    if(indice < tam){
        n = entrada[indice++];
        return n;
    }
    return -1;
    
}

