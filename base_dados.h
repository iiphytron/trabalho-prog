/* ============================================================================
 * base_dados.h- estruturas e funções para as respostas do chatbot ELIZA
 *
 * Define as struct que vão guardar as informacões do ficheiro, tais como as palavras-chave, respostas e as mensagens especiais
 * ============================================================================ */

#ifndef BASE_DADOS_H   
#define BASE_DADOS_H

#include <stdio.h>   //necessário para FILE* usado em leitura_linha

// Struct para cada conjunto de palavras-chave->resposta
typedef struct {
    int num_respostas;          
    int prox_resposta;     //indice que indica qual resposta dar na próxima vez que uma dada keyword for encontrada
    char **keywords;       //ponteiro para array dinâmico (uma vez que desta vez já não temos um max de keywords) de str com as keywords
    int num_keywords;
    char **respostas;     // ponteiro para array dinâmico de str com as respostas associadas às keywords
} Key_Resposta;

//variáveis globais (extern-> externas a este ficheiro, não as criamos aqui, já existem noutro fich)
extern Key_Resposta *dados_guardados;
extern int tamanho_dg;
extern char *output_inicio;
extern char *output_repeticao;
extern char *output_fim;

//Função para ler o ficheiro e guardar as palavras-chave e respostas numa variável
void ler_ficheiro(const char *nome_ficheiro);

////Função para fazer o round-robin (escolhe a próxima resposta a dar, e volta ao início se chegar ao fim)
char *gerar_nova_resposta(Key_Resposta *conjunto);

//Função para procurar a primeira keyword do input
Key_Resposta *encontrar_keyw(char *input);

//Função para libertar a memória
void libertar_memoria();

/*NOTA O QUE NÃO INCLUIR NO HEADER:
funções internas que não são usadas fora do ficheiro.c, neste caso:
procura_keywords();         // só é usada dentro do ficheiro
aumentar_capacidade_dg();   // especifica para os dados_guardados
*/

#endif // BASE_DADOS_H