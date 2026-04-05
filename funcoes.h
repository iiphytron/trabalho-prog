/* ============================================================================
 * funcoes.h
 *
 * este ficheiro declara as funcoes auxiliares usadas em todo o programa:
 * conversao para maiusculas, limpeza de caracteres, duplicacao de strings, etc.
 * ============================================================================ */

#ifndef FUNCOES_H   //"if not defined FUNCOES_H"
#define FUNCOES_H   //define it now, so next time this check fails

#include <stdio.h>   //Necessario para o tipo FILE* usado em read_line

//maiuscula: converte todos os caracteres da 'str' para maiusculas
void maiuscula(char *str);

/* limpar_input: remove da string 'str' todos os caracteres que não são
 * letras (a-z, A-Z), espaços, dígitos (0-9) ou apóstrofo (')*/
void limpar_input(char *str);

// sem_espacos: remove espaços em branco no início e no fim da string.
char *sem_espacos(char *str);

// copiar_string: duplica a string 'str', alocando memoria dinamicamente, 
 char *copiar_string(const char *str);

/* leitura_linha: le uma linha completa de um ficheiro, sem limite de tamanho.
 * A memoria e alocada dinamicamente e cresce conforme necessario.
 * Retorna NULL quando atinge o fim do ficheiro (EOF).
 * Devolve a linha lida como uma string. */
char *leitura_linha(FILE *ficheiro);

#endif  //Fim da inclusão guardada// teste
