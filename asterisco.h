/* ============================================================================
 * asterisco.h - funcoes da substituição dos pronomes aquando um asterisco
 *
 * quando uma resposta da ELIZA tem *, 
 * este é substituído pelo texto que aparece depois da keyword no stdin do utilizador
 * No entanto, os pronomes devem ser substituidos para que a frase faça sentido, de acordo com a tabela do enunciado do projeto.
 * ============================================================================ */

#ifndef ASTERISCO_H
#define ASTERISCO_H

/* pronomes: aplica as regras de substituicao de pronomes ao texto dado.
 * Parametros:
 *   restante - texto a conjugar, dps da keyword (já em maiúsculas)
 *   ingles=1 se o texto tiver em EN, 0 se em PT, para sabermos que pronomes usar
 *   Devolve a nova string, str final, alocada dinamicamente
 */

 char *pronomes(const char *restante, int ingles);

#endif 
