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
    {"TEU",    "MEU"},      // ordem da tabela original
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

/* função responsável por contar o número de palavras da str, 
para dps sabermos se estamos na última palavra ou não, no caso da substituição do "YOU" pelo "I"
palavras são conjuntos de char separados por espaços */

static int qnts_palavras(const char *s) {
    int contador= 0;                // Contador de palavras
    int in_word = 0;                /* Flag: estamos dentro de uma palavra? */
    for (int i = 0; s[i]; i++) {
        if (s[i] == ' ')            /* Espaco: saimos da palavra */
            in_word = 0;
        else if (!in_word) {        /* Caracter nao-espaco e estavamos fora */
            in_word = 1;            /* Entramos numa nova palavra */
            contador++;                /* Incrementa contador */
        }
    }
    return contador;
}

/* split_words: divide uma string em palavras individuais.
 * Retorna um array de strings (cada uma alocada dinamicamente).
 * O numero de palavras e escrito em *nwords.
 * Usa strtok() para separar por espacos. */
static char **split_words(const char *s, int *nwords) {
    *nwords = qnts_palavras(s);       /* Conta quantas palavras ha */
    if (*nwords == 0)               /* String vazia */
        return NULL;
    /* Aloca array de ponteiros para as palavras */
    char **words = malloc(sizeof(char *) * (*nwords));
    char *copy = my_strdup(s);      /* Copia porque strtok modifica a string */
    int idx = 0;
    /* strtok divide a string nos espacos, retornando cada token */
    char *tok = strtok(copy, " ");
    while (tok && idx < *nwords) {
        words[idx++] = my_strdup(tok);  /* Duplica cada palavra */
        tok = strtok(NULL, " ");        /* Proxima palavra */
    }
    free(copy);                     /* Liberta a copia temporaria */
    return words;
}

/* conjugate: funcao principal deste modulo.
 * Recebe o texto remanescente (depois da keyword) e aplica as regras
 * de substituicao de pronomes.
 *
 * O array 'replaced' marca as palavras ja substituidas para evitar
 * substituicoes em cadeia (ex: "I" -> "YOU" -> "I" de volta).
 *
 * Caso especial em ingles: "YOU" torna-se "I" no meio da frase,
 * mas "ME" se for a ultima palavra da linha. */
char *conjugate(const char *remainder, int portuguese) {
    int nwords;
    char **words = split_words(remainder, &nwords);
    if (!words || nwords == 0)              /* Texto vazio */
        return my_strdup("");               /* Retorna string vazia */

    /* Seleciona a tabela de regras conforme o idioma */
    const Substituicao *rules = portuguese ? pt_version : en_version;
    /* Array de flags: replaced[i]=1 se a palavra i ja foi substituida */
    int *replaced = calloc(nwords, sizeof(int));

    /* Percorre cada palavra e aplica regras de substituicao */
    for (int i = 0; i < nwords; i++) {
        if (replaced[i])                    /* Ja foi substituida, salta */
            continue;

        /* Caso especial: "YOU" em ingles */
        if (!portuguese && strcmp(words[i], "YOU") == 0) {
            free(words[i]);                 /* Liberta a palavra original */
            /* Se e a ultima palavra, substitui por "ME"; senao, por "I" */
            words[i] = my_strdup(i == nwords - 1 ? "ME" : "I");
            replaced[i] = 1;               /* Marca como substituida */
            continue;                       /* Passa a proxima palavra */
        }

        /* Percorre a tabela de regras ate encontrar match ou fim da tabela */
        for (int r = 0; rules[r].pron_original != NULL; r++) {
            if (strcmp(words[i], rules[r].pron_original) == 0) {
                free(words[i]);             /* Liberta a original */
                words[i] = my_strdup(rules[r].pron_resp);  /* Substitui */
                replaced[i] = 1;           /* Marca como substituida */
                break;                      /* Sai do ciclo de regras */
            }
        }
    }

    /* Reconstroi a string final juntando todas as palavras com espacos */
    size_t total = 0;
    for (int i = 0; i < nwords; i++)
        total += strlen(words[i]) + 1;      /* +1 para espaco ou '\0' */

    char *result = malloc(total + 1);       /* +1 de seguranca */
    result[0] = '\0';                       /* Inicia string vazia */
    for (int i = 0; i < nwords; i++) {
        if (i > 0)
            strcat(result, " ");            /* Espaco entre palavras */
        strcat(result, words[i]);           /* Anexa a palavra */
        free(words[i]);                     /* Liberta a palavra */
    }
    free(words);                            /* Liberta array de ponteiros */
    free(replaced);                         /* Liberta array de flags */
    return result;                          /* Retorna texto conjugado */
}
