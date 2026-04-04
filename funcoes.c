/* ============================================================================
 * funcoes.c - Ficheiro que contém as funções auxiliares
 * 
 * manipulação de strings, leitura de linhas, alocação dinâmica de memória
 * ============================================================================ */

 /*============================================================================ 
NOTAS IMPORTANTES: 

*WHY unsinged char-> Se usarmos char, os caracteres fora do ASCII range (0–127), por exemplo,
letras acentuadas á, à, etc, são representadas em números negativos (-128-(-1))
o unsinged char garante que o valor fica entre 0 e 255. 

mantemos char para ser compatível com as funções em C, mas usamos unsigned char 
temporariamente quando precisamos de usar funções de como o isalpha, o toupper, etc. 
neste caso temos de fazer cast (conversão forçada) nas funções em que é necessário.

*o buffer é um espaço de memória usado para armazenar dados temporariamente durante a execução do programa.
============================================================================ */

#include <stdio.h>     // printf, fprintf, fgetc, FILE, EOF 
#include <stdlib.h>    // malloc, realloc, free
#include <string.h>    // strlen, strcmp
#include <ctype.h>     // toupper, isalpha, isdigit, isspace
#include "funcoes.h"   // declarações das funções auxiliares

#define MAX_LENGTH 200

//Função para converter uma string para maiúsculas
void maiuscula(char *str) {             //void-> porque a função não retorna nada
    for (int i= 0; str[i]; i++){          // Percorre a string até encontrar '\0' que é quando a str acaba
        str[i] = toupper((unsigned char)str[i]);
      }
    }

//Função para remover todos os caracteres inválidos (que não sejam letras, espaços, ou dígitos)
void limpar_input(char *str) {
    int lida=0;
    int limpa=0;    //Índices auxiliares

    while(str[lida] != '\0') {      //enquanto não chegarmos ao final da string
        //conversão para unsigned char para evitar erros
        if (isalpha((unsigned char)str[lida]) || isdigit((unsigned char)str[lida]) || str[lida] == ' ' || str[lida] == '\'') { //se o caractere da string for uma letra, um dígito, ou um espaço, ou um apóstrofo, guardamos esse caractere na str[limpa]
            str[limpa] = str[lida];
            limpa++;
        }
        lida++;
    }
    str[limpa] = '\0'; //'\0' para marcamos o fim da string limpa
}

//Função para remover os espaços no ínicio e no fim da string / e ignorar a linha se ela estiver em branco
    //Remover espaços no ínicio:
char *sem_espacos(char *str) {     //char, porque no fim a função devolve um ponteiro para a char sem espaços
    while (*str && isspace((unsigned char)*str)){        //enq o caractere for um espaço, avançamoso ponteiro str, para marcar o ínicio /e unsigned char para evitar erros da função isspace
        str++;
    }
    //Função isspace reconhece como "espaço" os caracteres que geram espaços em branco no ecrã ou movam o cursor, ex.: ' '(Espaço comum-ASCII 32); '\t' (Tabulação horizontal-ASCII 9); '\n' (Nova linha-ASCII 10)
    
    if (*str =='\0'){                       // Se, depois de avaçarmos os espaços, chegarmos ao fim da str \0, significa que a string só tinha espaços, devolvemos a str vazia (sem ponteiro avançado)
        return str;                         
    }

    //Remover espaços no fim:
    /*começamos no caracter 0 da string e avançamos até ao final (+strlen), 
    depois recuamos um para ficar no último caracter, pois strlen conta o número de caracteres antes do '\0', ou seja, 
    o índice do último caractere é strlen-1.
    ex.: tamanho string =6, 0+6=6, mas na realidade o índice do último caractere é 5, pq a contagem começa no 0, logo 6-1=5 */
    
    char *fim= str + strlen(str)-1;              
    
    while (fim>str && isspace((unsigned char)*fim)){          //enq o caractere for um espaço, andamos com o ponteiro fim ,do fim para o início, para marcar o fim (último caractere que não é um espaço), ex.: OLÁ    -> OLÁ\0       
        fim--;
    *(fim+1)='\0';        //escrevemos o \0 no endereço de memória específico fim+1
    }                   
        return str;
}                             
//só return str, pq o end já modificou a string colocando um terminador \0 no final do último caracter, qnd a lermos, só se lê até ao \0

//Função para duplicar uma string
char *copiar_string(char *str) {
    int tamanho= strlen(str) + 1;       //Calcular tamanho da string / +1 para incluirmos o caracter de terminação \0 (tb temos de copiar esse pois ele faz parte da str e ocupa espaço na memória)

    char *copia = malloc(tamanho*sizeof(char));         /*a função malloc serve para pedir ao computador uma memória específica enquanto o programa está a correr, garante que a cópia sobrevive para ser usada no resto do programa
                                                        malloc(Quantidade de elementos=tamanho * Tamanho de cada elemento=sizeof(char)), pq queremos guardar char*/
    if (copia == NULL) {        //Verificamos se o computador conseguir alocar a memória, se não conseguiu, retorna NULL para indicar que houve um erro
        return NULL;
    }
    for (int i= 0; i< tamanho; i++) {       //copiamos caracter a caracter (como um array normal)
        copia[i] = str[i];
    }
    return copia;
}

//Função para ler uma linha de um ficheiro
/* Lê uma linha completa de um ficheiro usando alocação dinâmica.
 * O buffer começa com uma capacidade inicial e duplica de tamanho quando
 * necessário */

 //size_t é um unsigned int usado para representar tamanhos em C
 
 char *leitura_linha(FILE *ficheiro){
    size_t capacidade = MAX_LENGTH;               //Capacidade atual do buffer, inicializada a 200
    size_t length= 0;                             //Numero de bytes ja lidos
    char *linha= malloc(capacidade);              //Alocacao inicial
    
    if (linha==NULL){                              //falha na alocação de memória
        return NULL;
    }

    int c; // character lido (int para EOF)
    /*fgetc explicação: 
    lê um único character (unsigned char) da posição atual de ficheiro ou stream de entrada (stdin) e avança o ponteiro para o próximo character. 
    retorna o character lido como um inteiro (int)(0-255) ou EOF (End of File)(geralmente -1) ao encontrar newline ou EOF */
    
    while ((c=fgetc(ficheiro))!=EOF && c!='\n') {
        //Se o buffer estiver quase cheio, duplicamos a capacidade
        if (length +1 >= capacidade) {
            capacidade*= 2;                       // Duplicamos a capacidade
            
            /*buffer serve como um ponteiro temporário para proteger o buffer original , linha, em caso de falha do realloc
            se realloc falhar, devolve NULL e não liberta a memória original (linha), ao fazermos linha= NULL, perdemos o ponteiro para a memória antiga —memory leak */
            
            /*como usar realloc: void* realloc(void* ptr, size_t new_size);
             o primeiro argumento é o ponteiro antigo, e o segundo é o novo tamanho em bytes*/

            char *buffer= realloc(linha, capacidade);      // tenta realocar, size linha para capacidade nova
            if (buffer==NULL) {                            // se falhar
                free(linha);                               // ainda temos o ponteiro original, podemos libertar
                return NULL;
            }
            linha= buffer;                      //só substituimos o buffer se correr bem a realocação
        }
        linha[length++] = (char)c;              //armazena o character
    }
    
    /*Explicação linha acima e abaixo:
    c = 'O', 'l', 'a'  (lidos um a um pelo fgetc)

    length=0 → linha[0] = 'O' → length passa a 1
    length=1 → linha[1] = 'l' → length passa a 2
    length=2 → linha[2] = 'a' → length passa a 3

    a incrementação ++ depois da variável, significa que usamos primeiro o valor atual e só dps incrementamos

    depois do while:
    linha[3]= '\0'
    resultado: linha = "Ola"

    Pq o cast forçado (char)c?
    c é int (para poder guardar EOF que é -1), 
    mas queremos guardar um char na string, o (char)c converte o inteiro para character — sem isso o compilador pode dar um warning. 
    */

    //se não leu nada e chegou ao EOF, nao ha mais linhas
    if (length== 0 && c== EOF) {
        free(linha);                        //liberta a memória alocada para a linha, pq não vamos usar
        return NULL;
    }

    linha[length]= '\0';                       //termina a string
    return linha;                              //retorna a linha lida
}
