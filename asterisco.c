/*Lógica programa:

Como em inglês: "YOU" se torna "I" no meio da frase, mas "ME" se for a ultima palavra da linha,
temos de saber quantas palavras e quais são para fazer a substituição correta.

Para isso vamos precisar de uma função que divida o texto em palavras e conte quantas palavras há, 
para depois outra função fazer as substituições corretas. */

//NOTA: static-> só visível dentro deste ficheiro, const-> não pode ser modificada, static const-> privada e imutável

#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     
#include "asterisco.h"  
#include "funcoes.h"    

//struct para a substituição dos pronomes
typedef struct {
    const char *pron_original;      
    const char *pron_resp;    
} Substituicao;

// Tabela de regras da substituição dos pronomes no funcionamento em inglês (dada no enunciado do projeto)
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

//Tabela de regras da substituição dos pronomes no funcionamento em português (dada no enunciado do projeto)
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

static int contador_palavras(const char *s) 
{int contador= 0;                
int fora=0;                     
    for (int i=0; s[i]; i++) {      
        if (s[i]== ' ')            
        {fora= 1;                //else=dentro, fora=1
        }else if (fora){            //se o character não for um espaço e estávamos fora, então entramos numa nova palavra
            fora=0;            
            contador++;
        }
    }return contador;
}

/* função responsável por dividir a string em palavras (separadas por espaços) e devolver um array de strings,
 guarda o número de palavras escrito para sabermos quantas separações temos de fazer e que memória alocar para o array de strings,
 para fazermos essa separação recorremos à função strtok() (para separar por espacos) */
static char **definir_palavras(const char *s, int *num_palavras) {
    *num_palavras= contador_palavras(s);
    
if (*num_palavras == 0){              
return NULL;}
    
    char **palavras= malloc(sizeof(char *) * (*num_palavras));      
    char *copia= copiar_string(s);      //copiamos a str porque strtok modifica a str original
        int indice= 0;
    //strtok vai dividir a string nos espacos, retornando cada uma variavél auxiliar (token com cada palavra e NULL quando não houver mais palavras)
    char *token= strtok(copia, " ");
    while (token && indice < *num_palavras) 
    {
    palavras[indice++]= copiar_string(token);       //incrementamos o indice, para sabermos em que palavra vamos
    token= strtok(NULL, " ");        
    }free(copia);
    
    return palavras;
}

/* funcao principal do modulo:
 recebe o texto depois da keyword, e visa aplicar as regras de substituição dos pronomes
 criamos um array "replaced" que marca as palavras ja substituídas para evitar substituições duplas (ex: "I" -> "YOU" -> "I" de volta)
 garantimos que no funcionamento em ingles: "YOU" passa a "I" no meio da frase, mas "ME" na ultima palavra da linha. */
 char *pronomes(const char *restante, int ingles) {     //inglês como principal (1=inglês, 0=português)
    int num_palavras;         
    char **palavras= definir_palavras(restante, &num_palavras);     //guarda as palavras restantes do input(depois da keyword)
    if (palavras== NULL || num_palavras == 0){            
        return copiar_string("");}
    //escolhemos as substituições conforme o idioma
    const Substituicao *pronomes_substituir;       //ponteiro para a tabela de substituição a usar
    if (ingles) {
        pronomes_substituir= en_version;
    }else 
    {
        pronomes_substituir= pt_version;
    }

    //Alocamos memória para o array, que marca as palavras já substituídas-> se replaced[i]=1 a palavra i já foi substituída
    int *replaced= calloc(num_palavras, sizeof(int));  
    for (int i = 0; i < num_palavras; i++) {        
        if (replaced[i]) continue;
        //exceção do "YOU"
        if (ingles && strcmp(palavras[i], "YOU") == 0) {
            free(palavras[i]);                 
            //se YOU for a última palavra, substituimos por "ME"-> se não substituímos por I
            if (i== num_palavras - 1) {     
            palavras[i]= copiar_string("ME");       
            }else 
            { palavras[i]= copiar_string("I");   //YOU em qq outra posição I
            }
            replaced[i]= 1;               //marcação da palavra como já substituida
            continue;                     
        }
        //percorre a tabela dos pronomes até encontrar match ou fim da tabela
        for (int r= 0; pronomes_substituir[r].pron_original != NULL; r++) 
        {
        if (strcmp(palavras[i], pronomes_substituir[r].pron_original) == 0) {
        char *novo = copiar_string(pronomes_substituir[r].pron_resp);
        free(palavras[i]);
        palavras[i] = novo;
        replaced[i] = 1;
        // marcar posições seguintes com o mesmo valor para não serem reprocessadas
        for (int j = i + 1; j < num_palavras; j++) {
            if (strcmp(palavras[j], novo) == 0){
                    replaced[j] = 1;}
            } break;
        }
    }
    }
    //Função responsável por reconstruir a str final (após substituição) juntando todas as palavras com espaços 
    size_t total= 0;            //variável para guardar o tamanho total da str final (resposta), para alocarmos a memória necessária para essa str e para juntarmos as palavras todas
    for (int i= 0; i < num_palavras; i++)
        total+= strlen(palavras[i]) + 1;      

    char *str_final= malloc(total +1);             //str final-> resposta do eliza dps da substituição e da junção das palavras, alocamos a memória necessária para essa str +1 , para o char terminal
    str_final[0]= '\0';                        
    for (int i= 0; i < num_palavras; i++) {
        if (i> 0){
            strcat(str_final, " ");         //adicionamos um espaço entre as palavras, mas não antes da primeira palavra, daí i>0            
        }
        strcat(str_final, palavras[i]);        
        free(palavras[i]);                     
    }
    free(palavras);                         
    free(replaced);                       
    return str_final;
}

