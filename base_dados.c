/* ============================================================================
 * base_dados.c - Objetivos:
 * Carregar e gerir a base de dados das respostas;
 *
 * 1. Ler o ficheiro e guardar as palavras-chave (separadas por '!') e respostas (separadas por '.') numa variável global (dados_guardados);
 * 2. Definir as mensagens especiais (boas-vindas, repetição, despedida) e guardá-las em variáveis globais;
 * 3. Libertar toda a memoria alocada quando o programa termina;
 * ============================================================================ */

#include <stdio.h>        
#include <stdlib.h>       
#include <string.h>       
#include "base_dados.h"   
#include "funcoes.h"      
// *conjunto é um ponteiro para a struct que guarda as keywords e respostas lidas do ficheiro

//Armazena as palavras-chave e respostas lidas do ficheiro, bem como e o número de conjuntos destes lidos
Key_Resposta *dados_guardados= NULL;     //ponteiro para array dinâmico de keywords e respostas, NULL porque ainda não alocamos memória para ele
int tamanho_dg=0;        
size_t capacidade_dg=8;

//Função para aumentar a capacidade do array dados_guardados se necessário
void aumentar_capacidade_dg() {
    if ((size_t)tamanho_dg >= capacidade_dg) 
    {capacidade_dg *= 2;       
    Key_Resposta *novo_array= realloc(dados_guardados, capacidade_dg * sizeof(Key_Resposta));          // tenta realocar, size dados_guardados para capacidade nova
    if (novo_array== NULL) 
    {                            
        free(dados_guardados);                          //ainda temos o ponteiro original, podemos libertá-lo para evitar vazamento de memória
        fprintf(stderr, "Ocorreu um erro a alocar memória para dados_guardados! \n");
        exit(1); 
    } dados_guardados=novo_array;                      //só substituimos o array se correr bem a realocação
    }   
}

//Definir respostas especiais
// =NULL, pois ainda não guardámos nenhuma str, só depois de ler o ficheiro 
char *output_inicio = NULL;     //mostrada no início do programa
char *output_repeticao = NULL;  //mostrada quando o input do utilizador for igual ao input anterior
char *output_fim = NULL;        //mostrada quando o utilizador escrever "BYE"

//Função para procurar as keywords no input
    /*para garantir que a keyword aparece como palavra completa e não como parte de outra palavra, temos de verificar as fronteiras destas:
    usamos a função strstr() para encontrar uma string(keyyword), dentro de outra string(input), 
    e depois verificamos se o character antes e depois da keyword é um espaço ou o ínicio/fim da string */
char *procura_keywords(const char *linha, const char *keyword) {     //linha= input já em maiúsculas
    int tamanho_keyword= strlen(keyword);
    const char *procura= linha;
    while ((procura= strstr(procura,keyword)) != NULL) 
    {                                                    //Procura keywords na linha atravéss da função strstr, devolve um ponteiro para a primeira keyword encontradana linha, ou NULL se não encontrar nenhuma         
        int fronteira_esq= (procura == linha) || (*(procura - 1) == ' ');       
        
        char depois= *(procura + tamanho_keyword);                                                      
        
        int fronteira_dir= (depois == '\0') || (depois == ' ');
        if (fronteira_esq && fronteira_dir)                // Se ambas as fronteiras são zero ou ínicio/fim, encontrámos a keyword completa
            return (char *)(procura + tamanho_keyword);     
        procura++;                                                      
    }
return NULL;                       
}

//Função para ler o ficheiro e guardar as palavras-chave e respostas numa variável
void ler_ficheiro(const char *nome_ficheiro) {      // const char *, significa que dentro da função não se altera o conteúdo da str
    FILE *ficheiro= fopen(nome_ficheiro, "r");
    if (ficheiro==NULL) 
    { printf("Erro a abrir ficheiro.\n");
        exit(1); 
    }
        
    dados_guardados = malloc(capacidade_dg * sizeof(Key_Resposta));
    if (dados_guardados == NULL) {
        fprintf(stderr, "Ocorreu um erro a alocar memória para dados_guardados! \n");
        exit(1);
    }
    
    char *linha;
    //Objetivo: ler os conjuntos de keywords e respostas
    while ((linha=leitura_linha(ficheiro)) != NULL) {      
        if (linha[0] == '\0') {free(linha); continue;}
        
        aumentar_capacidade_dg();       //aumentamos a capacidade antes de ler o ficheiro, para garantir que temos espaço para guardar os dados lidos do ficheiro
        
        Key_Resposta *conjunto= &dados_guardados[tamanho_dg];
        (*conjunto).num_keywords = 0;
        (*conjunto).num_respostas = 0;
        (*conjunto).prox_resposta = 0;          //inicializamos tudo a zeros, porque ainda não lemos nada
        (*conjunto).keywords = NULL;            //inicializamos os ponteiros a NULL, porque ainda não alocamos memória para eles   
        (*conjunto).respostas = NULL; 
        
        //Ler keywords(ou seja ler ficheiro até a "!")
        while(linha && strcmp(linha,"!") != 0) {
            int n=(*conjunto).num_keywords;
            
            //aloca/cresce o array das keywords
            char **auxiliar= realloc((*conjunto).keywords, (n+1) * sizeof(char *));         //n=número de keywords já lidas, alocamos espaço para mais uma keyword, daí n+1
            if (auxiliar== NULL) {      //se não conseguirmos alocar mais memória, então temos de libertar a memória já alocada para as keywords, para evitar vazamento de memória
                free((*conjunto).keywords);
                fprintf(stderr, "Ocorreu um erro a alocar memória para keywords!\n");
                exit(1);
            }
            (*conjunto).keywords= auxiliar;
            (*conjunto).keywords[n] =copiar_string(linha);  //guarda a keyword
            maiuscula((*conjunto).keywords[n]);
            (*conjunto).num_keywords++;     //incrementamos o número de keywords do conjunto
            
            free(linha);
            linha=leitura_linha(ficheiro);
        }
        free(linha);
        linha= leitura_linha(ficheiro);
        while (linha && strcmp(linha, ".") != 0) {      
            int n=(*conjunto).num_respostas;
            
            // aloca/cresce o array de respostas
            char **auxiliar = realloc((*conjunto).respostas, (n + 1) * sizeof(char *));         
            if (auxiliar== NULL) {      
                free((*conjunto).respostas);
                fprintf(stderr, "Ocorreu um erro a alocar memória para respostas!\n");
                exit(1);
            } (*conjunto).respostas = auxiliar;
            (*conjunto).respostas[n] = copiar_string(linha);  //guardamos a resposta
            (*conjunto).num_respostas++;
            free(linha);
            linha=leitura_linha(ficheiro);
        }free(linha);
        linha= NULL;

        // Verificar se é uma das mensagens especiais
        if ((*conjunto).num_keywords == 1) {        //verifica se este conjunto tem apenas 1 keyword, pq as msgs especiais têm apenas 1 keyword  
            if (strcmp((*conjunto).keywords[0], "START") == 0) {
                output_inicio = copiar_string((*conjunto).respostas[0]);
                for (int k = 0; k < (*conjunto).num_keywords; k++) free((*conjunto).keywords[k]);
                free((*conjunto).keywords);
                for (int r = 0; r < (*conjunto).num_respostas; r++) free((*conjunto).respostas[r]);
                free((*conjunto).respostas);
                continue;
            }
            if (strcmp((*conjunto).keywords[0], "REPEAT") == 0)
            {
                output_repeticao = copiar_string((*conjunto).respostas[0]);
                for (int k = 0; k < (*conjunto).num_keywords; k++) free((*conjunto).keywords[k]);
                free((*conjunto).keywords);
                for (int r = 0; r < (*conjunto).num_respostas; r++) free((*conjunto).respostas[r]);
                free((*conjunto).respostas);
                continue;
            }

            if (strcmp((*conjunto).keywords[0], "BYE") == 0 || strcmp((*conjunto).keywords[0], "ADEUS") == 0) {        //pode ser BYE ou ADEUS, dependendo do idioma
                output_fim= copiar_string((*conjunto).respostas[0]);
                for (int k = 0; k < (*conjunto).num_keywords; k++) free((*conjunto).keywords[k]);
                free((*conjunto).keywords);
                for (int r = 0; r < (*conjunto).num_respostas; r++) free((*conjunto).respostas[r]);
                free((*conjunto).respostas);
                continue;
            }
        } tamanho_dg++;           //apenas os conjuntos que não são especiais vão para os dados_guardados, por isso só incrementamos o tamanho_dg aqui
    } fclose(ficheiro);
}

//Função para fazer o round-robin (escolhe a próxima resposta a dar, e volta ao início se chegar ao fim)
char *gerar_nova_resposta(Key_Resposta *conjunto) {   
    if ((*conjunto).num_respostas == 0) {
    return NULL;                                       
    }
    char *respos= (*conjunto).respostas[(*conjunto).prox_resposta];     
    
    (*conjunto).prox_resposta=((*conjunto).prox_resposta + 1) % (*conjunto).num_respostas; /* acede ao membro prox_resposta e avança o índice de resposta, 
                                                                                            voltando ao início se chegar ao fim, ex.: respos=0, 4+1=5 , num_respostas=5, 5%5= 0, 
                                                                                            se demos a resposta nº4, (0-4), a proxima resposta a dar vai ser a 0, volta ao ínicio*/
    return respos;
}

//Função para procurar a primeira keyword do input
Key_Resposta *encontrar_keyw(char *input) {
    for (int i=0; i<tamanho_dg; i++) {            //percorremos os dados_guardados, que é onde guardamos as keywords e respostas lidas do ficheiro
        Key_Resposta *conjunto= &dados_guardados[i];           //retorna o primeiro conjunto que tiver uma keyword encontrada
        
        for (int j=0; j<(*conjunto).num_keywords; j++) {           //percorremos as keywords de cada conjunto, para procurar a keyword do input
            if (procura_keywords(input, (*conjunto).keywords[j])) 
            {
                return &dados_guardados[i];        //se encontrar a keyword, retorna um ponteiro para o conjunto de keywords e respostas correspondente
            }
    }
    }
    if (tamanho_dg > 0) {       //se o ciclo for acabar e não tivermos encontrado keywords compativeis, o código devolve o último grupo da base de dados,que contém a resposta por omissão, que é a resposta dada quando nenhuma keyword é encontrada no input
        return &dados_guardados[tamanho_dg - 1]; // Devolve o último
    }else{
    return NULL; // Base de dados vazia
}
}

// Função para libertar memória
void libertar_memoria() {
    free(output_inicio);                //liberta a memória das mensagens especiais
    free(output_repeticao);
    free(output_fim);

    for (int i = 0; i < tamanho_dg; i++) 
    {
        
        Key_Resposta *ptr= &dados_guardados[i];     //ptr= variavel auxiliar para evitar escrever dados_guardados[i] várias vezes
        
        for (int k = 0; k < ptr->num_keywords; k++)
            free(ptr->keywords[k]);         //libertamos cada keyword
        free(ptr->keywords);                //libertamos o array de keywords
        for (int r = 0; r < ptr->num_respostas; r++)
            free(ptr->respostas[r]);        //libertamos cada resposta
        free(ptr->respostas);               //libertamos o array de respostas
    }
    free(dados_guardados); //liberta a memória do array de dados guardados
}
//se libertassemos as keywords primeiro, perdiamos o acesso a keywords[k] -> memory leak!
