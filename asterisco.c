/*Lógica programa:

Como em inglês: "YOU" se torna "I" no meio da frase, mas "ME" se for a ultima palavra da linha,
temos de saber quantas palavras e quais são para fazer a substituição correta.

Para isso vamos precisar de uma função que divida o texto em palavras e conte quantas palavras há, 
para depois outra função fazer as substituições corretas. */

//NOTA: static-> só visível dentro deste ficheiro, const-> não pode ser modificada, static const-> privada e imutável

#include <stdio.h>      // funções básicas de C 
#include <stdlib.h>     // funções de alocação de memória
#include <string.h>     // funções da str
#include "asterisco.h"  // declaracao da funcao asterisco
#include "funcoes.h"    // copiar_string

//struct para a substituição dos pronomes
//por ex.: {"ARE", "AM"}, vai significar que o "ARE" é substituido por "AM".
typedef struct {
    const char *pron_original;      // Pronome a procurar no input do utilizador
    const char *pron_resp;          // Pronome que vai substituir o original na resposta da ELIZA
} Substituicao;

// Tabela de regras da substituição dos pronomes no funcionamento em INGLES (dada no enunciado do projeto)
 static const Substituicao en_version[] = {
    {"ARE",   "AM"},
    {"AM",    "ARE"},
    {"WERE",  "WAS"},
    {"WAS",   "WERE"},
    {"YOU",   "I"},       // tratado como caso especial no código (ME/I)
    {"I",     "YOU"},
    {"YOUR",  "MY"},
    {"MY",    "YOUR"},
    {"IVE",   "YOUVE"},
    {"YOUVE", "IVE"},
    {"IM",    "YOURE"},
    {"YOURE", "IM"},
    {"ME",    "YOU"},
    {"US",    "YOU"},
    {"WE",    "YOU"},
    {NULL, NULL}
};

// Tabela de regras da substituição dos pronomes no funcionamento em PORTUGUÊS (dada no enunciado do projeto)
static const Substituicao pt_version[] = {
    {"EU",     "TU"},
    {"TU",     "EU"},
    {"COMIGO", "CONTIGO"},
    {"CONTIGO","COMIGO"},
    {"TEU",    "MEU"},  
    {"SEU",    "MEU"},
    {"TEUS",   "MEUS"},
    {"SEUS",   "MEUS"},
    {"TUA",    "MINHA"},
    {"SUA",    "MINHA"},
    {"TUAS",   "MINHAS"},
    {"SUAS",   "MINHAS"},
    {"MEU",    "TEU"},
    {"MEUS",   "TEUS"},
    {"MINHA",  "TUA"},
    {"MINHAS", "TUAS"},
    {"MIM",    "TI"},
    {"TI",     "MIM"},
    {NULL, NULL}
};

/*função responsável por contar o número de palavras da str, 
para dps sabermos se estamos na última palavra ou não, no caso da substituição do "YOU" pelo "I"
palavras são conjuntos de char separados por espaços*/
//variável de controlo "fora" para saber se estamos dentro ou fora de uma palavra: 0-> dentro de uma palavra(não), 1-> fora(sim)

static int contador_palavras(const char *s) {
    int contador= 0;                // Contador de palavras
    int fora=0;                     
    for (int i=0; s[i]; i++) {      //percorre cada character da string até chegar ao '\0'
        if (s[i]== ' ')             //se encontrarmos um espaço, saímos da palavra
            fora= 1;                //else=dentro, fora=0
        else if (fora) {            //se o character não for um espaço e estávamos fora, então entramos numa nova palavra
            fora=0;            
            contador++;             //incrementamos o contador de palavras
        }
    }
    return contador;
}

/* função responsável por dividir a string em palavras (separadas por espaços) e devolver um array de strings,
 guarda o número de palavras escrito para sabermos quantas separações temos de fazer e que memória alocar para o array de strings,
 para fazermos essa separação recorremos à função strtok() (para separar por espacos) */

static char **definir_palavras(const char *s, int *num_palavras) {
    *num_palavras= contador_palavras(s);       //conta quantas palavras há na str
    if (*num_palavras == 0)               
        return NULL;
    
    char **palavras= malloc(sizeof(char *) * (*num_palavras));      //alocamos array de ponteiros para guardar as palavras
    char *copia= copiar_string(s);      //copiamos a str porque strtok modifica a str original
    int indice= 0;
    //strtok vai dividir a string nos espacos, retornando cada uma variavél auxiliar (token com cada palavra e NULL quando não houver mais palavras)
    char *token= strtok(copia, " ");
    while (token && indice < *num_palavras) {
        palavras[indice++]= copiar_string(token);       //copiamos a palavra do token para o array de palavras, e incrementamos o indice, para sabermos em que palavra vamos
        token= strtok(NULL, " ");        //próxima palavra
    }
    free(copia);                     //libertamos a copia da str, dps de guardarmos as palavras no array de palavras
    return palavras;
}

/* funcao principal do modulo:
 recebe o texto depois da keyword, e visa aplicar as regras de substituição dos pronomes
 criamos um array "replaced" que marca as palavras ja substituídas para evitar substituições duplas (ex: "I" -> "YOU" -> "I" de volta)
 garantimos que no funcionamento em ingles: "YOU" passa a "I" no meio da frase, mas "ME" na ultima palavra da linha. */
 char *pronomes(const char *restante, int ingles) {     //inglês como principal (1=inglês, 0=português)
    int num_palavras;         
    char **palavras= definir_palavras(restante, &num_palavras);     //guarda as palavras restantes do input(depois da keyword)
    
    if (palavras== NULL || num_palavras == 0)              
        return copiar_string("");               //devolve umaa str vazia

    //escolhemos as substituições conforme o idioma
    const Substituicao *pronomes_substituir;       //ponteiro para a tabela de substituição a usar
    if (ingles) {
        pronomes_substituir= en_version;
    } else {
        pronomes_substituir= pt_version;
    }

    //Alocamos memória para o array, que marca as palavras já substituídas-> se replaced[i]=1 a palavra i já foi substituída
    int *replaced= calloc(num_palavras, sizeof(int));       /*void *calloc(size_t num_elementos, size_t tamanho_elemento);
                                                            o calloc zera todos os bits da memória alocada, evitando lixo*/

    //percorre as palavra 1 a 1, e faz a substituição de acordo com as regras
    for (int i = 0; i < num_palavras; i++) {        
        if (replaced[i])                    //se a palavra i já foi substituída, saltamos para a próxima palavra, avançamos o i
            continue;

        //exceção do "YOU"
        if (ingles && strcmp(palavras[i], "YOU") == 0) {
            free(palavras[i]);                 //libertamos o pronome original
            
            //se YOU for a última palavra, substituimos por "ME"-> se não substituímos por I
            if (i== num_palavras - 1) {     //-1, pois os indices começam no 0, logo o índice da última palavra é num_palavras-1, ex.: num_palavras=4, indice=3
            palavras[i]= copiar_string("ME");       //copiamos a str ME, o array das palavras, para na reconstrução da resposta substituirmos o YOU pelo pronome ME
            } else {
            palavras[i]= copiar_string("I");   //YOU em qq outra posição I
            }
            replaced[i]= 1;               //marcamos a palavra como substituida
            continue;                     //passamos para a próxima palavra
        }

        //percorre a const dos pronomes até encontrar match ou fim da tabela
        for (int r= 0; pronomes_substituir[r].pron_original != NULL; r++) {
            
            if (strcmp(palavras[i], pronomes_substituir[r].pron_original) == 0) {       //comparamos a palavra i com o pronome original da tabela de substituição, se for igual, fazemos a substituição
                free(palavras[i]);             //libertamos a palavra original
                palavras[i]= copiar_string(pronomes_substituir[r].pron_resp);  //substituímos pela resposta correspondente
                replaced[i] = 1;           //marcamos como substituída
                break;                      //saímos do ciclo de regras
            }
        }
    }

    //Função responsável por reconstruir a str final (após substituição) juntando todas as palavras com espaços 
    size_t total= 0;            //variável para guardar o tamanho total da str final (resposta), para alocarmos a memória necessária para essa str e para juntarmos as palavras todas
    for (int i= 0; i < num_palavras; i++)
        total+= strlen(palavras[i]) + 1;      //+1 para espaco ou para o character terminal '\0'

    char *str_final= malloc(total +1);             //str final-> resposta do eliza dps da substituição e da junção das palavras, alocamos a memória necessária para essa str +1 , para o char terminal
    str_final[0]= '\0';                        //inicialização da str como vazia 
    for (int i= 0; i < num_palavras; i++) {
        
        /*strcat-> concatena as palavras, 
        char *strcat(char *destino, const char *origem), 
        a função copia a str origem e a cola a nova palavra a partir do /0 da string destino, substituindo-o.*/
        
        if (i> 0){
            strcat(str_final, " ");         //adicionamos um espaço entre as palavras, mas não antes da primeira palavra, daí i>0            
        }
        strcat(str_final, palavras[i]);        //concatena a palavra atual à str final
        free(palavras[i]);                     //liberta a palavra, após esta já estar na str final, para não haver memory leak
    }
    free(palavras);                         
    free(replaced);                    //libertamos as variaveis usadas                        
    return str_final;
}
