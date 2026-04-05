/* ============================================================================
 * funcoes.c -> contém as funções auxiliares ao programa
 * 
 * manipulação das strings, leitura de linhas, alocação dinâmica da memória
 * ============================================================================ */

#include <stdio.h>     
#include <stdlib.h>    
#include <string.h>    
#include <ctype.h>     
#include "funcoes.h"   // declarações das funções auxiliares

#define MAX_LENGTH 200

//Função para converter uma string para maiúsculas
void maiuscula(char *str) {             
    for (int i= 0; str[i]; i++)
    {          
      str[i] = toupper((unsigned char)str[i]);
      }
 }

//Função para eliminar todos os caracteres inválidos (que não sejam letras, espaços, ou dígitos)
void limpar_input(char *str) {
    int lida=0;
    int limpa=0;

    while(str[lida] != '\0') 
    {      
        //convertemos para unsigned char para evitar erros
        if (isalpha((unsigned char)str[lida]) || isdigit((unsigned char)str[lida]) || str[lida] == ' ' || str[lida] == '\'') { 
        str[limpa] = str[lida];
        limpa++;
        } lida++;
    }
    str[limpa]= '\0'; //'\0' para marcamos o fim da string limpa
}

//Função para remover os espaços no ínicio e no fim da string, e ignorar linhas se estas estiverem em branco(vazias)
char *sem_espacos(char *str) {     
    while (*str && isspace((unsigned char)*str))
    { //enq o caractere for um espaço, avançamoso ponteiro str, para marcar o ínicio
    str++;
    }
    if (*str =='\0'){
        return str;                         
    }
    //Remoção dos espaços no fim
    char *fim= str+ strlen(str)-1;
    while(fim>str && isspace((unsigned char)*fim))
    { fim--;
      *(fim+1)='\0';
    } return str;  //str modificada, já sem os espaços no fim e no ínicio
}                             

//Função para copiar uma string, mantendo todas as suas características
char *copiar_string(const char *str) {
    int tamanho= strlen(str)+1;       
    char *copia = malloc(tamanho*sizeof(char));
    if (copia == NULL) 
    {
    return NULL;
    }
    for (int i= 0; i< tamanho; i++) 
    {
    copia[i] = str[i];
    }
return copia;
}

/*Função para ler uma linha de um ficheiro, usando a alocação dinâmica de memória, 
fazendo recurso a um buffer que começa com a capacidade inicial, MAX_LENGHT e duplica de tamanho quando necessário
*/
char *leitura_linha(FILE *ficheiro){
size_t capacidade= MAX_LENGTH;               
size_t length= 0;                             //número de bytes ja lidos
char *linha= malloc(capacidade);
    
if (linha==NULL){                             
    return NULL; 
    }
    
int c; //character lido
    while ( (c=fgetc(ficheiro) ) !=EOF && c !='\n') 
    { 
    if (length +1 >= capacidade) {
    capacidade*= 2;}
    
char *buffer= realloc(linha, capacidade);      
        if (buffer==NULL) {                            
           free(linha);                               
           return NULL;
           }
        linha= buffer;                      //substituímos o buffer se correr bem a realocação
        }
        linha[length++]=(char)c;  //anteriormente definido como int para a utilzação do fgetc
}
//se a função não tiver lido nada e chegou ao EOF, então sabemos que não há mais linhas
if (length== 0 && c== EOF) 
{
free(linha);                        
return NULL;
}
linha[length]= '\0';                       
   return linha;                              
}
