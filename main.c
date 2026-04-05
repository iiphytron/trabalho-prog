#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>//$para usar getopt, ler linha de commando
#include "asterisco.h" // $ substituir por pornomes quando encontra *
#include "base_dados.h" //$ le o ficheiro  e liberta memoria 
#include "funcoes.h" //$elimina espaços, tudo em maisculas, copia o string
#include "getopt.h"

int modo_portugues = 0;//$ default= ingles, fica "1" com -p e muda para base de dados portugues
FILE *input_global = NULL;
FILE *output_global = NULL;
FILE *log_file = NULL; // ficheiro log que aparece com -l

int main(int argc, char *argv[])//$ c= num argumentos linha de commando, v= array com args 
{
    const char *ficheiro = "eliza.dat"; //$ base de dados default
    input_global = stdin;
    output_global = stdout;

    int opt;//$ a opçao que getopt guardou
    while ((opt = getopt (argc, argv, "ho:l:f:pi:") )!=-1)//$p e h nao precisa de argumentos
    {
        switch (opt)
        {
        case 'h':
         printf("Uso: ./eliza [OPTIONS]\n");
                printf("  -h  ajuda\n");
                printf("  -i nome ficheiro de entrada\n");
                printf("  -o nome ficheiro de saida\n");
                printf("  -l  criar ficheiro de log\n");
                printf("  -f  base de dados alternativa\n");
                printf("  -p  modo portugues\n");
        return EXIT_SUCCESS;
            //help depois fazer os prints
//-----------------------------------------------------------------------------------------------------------------------------------------//
        case 'i' ://$ abre ficheiro de entrada 
        input_global = fopen (optarg, "r");
        if(input_global== NULL)

        {printf("erro a abrir ficheiro %s ", optarg );
        return EXIT_FAILURE;//se o ficheiro nao existe 
        }
        break;
//-----------------------------------------------------------------------------------------------------------------------------------------//
        case 'o'://$escreve no ficheiro de saidda 
        output_global = fopen(optarg, "w");
        
        if (output_global== NULL)
        {
            printf("erro a abrir ficheiro %s", optarg );
            return EXIT_FAILURE;
        }
        break;
//----------------------------------------------------------------------------------------------------------------------------------------//
        case 'l': //$ cria ficheiro log
         log_file = fopen(optarg,"w");
         if (log_file== NULL)
        {
            printf("erro a abrir ficheiro %s", optarg );
            return EXIT_FAILURE;
        }
        break;
//---------------------------------------------------------------------------------------------------------------------------------------//
        case 'f' ://$ -f guarda o nome do ficheiro na variavel "ficheiro" quando chamamos ler_fich(ficheiro) carrega aquele que o utilizador indica
        ficheiro= optarg;
        break;         
//--------------------------------------------------------------------------------------------------------------------------------------//
        case 'p':
        modo_portugues = 1;
        break;
//-------------------------------------------------------------------------------------------------------------------------------------//
        default:
        printf ("opcao invalida -h para ajuda");
        return EXIT_FAILURE;

        }
    }
    ler_ficheiro(ficheiro);//$ le o ficheiro data, guarda keywords,asteriscos,  e mensagens
    if (output_inicio !=NULL )
    {
        fprintf(output_global, "%s\n", output_inicio);
        if (log_file)
        fprintf(log_file,"%s\n", output_inicio);//$ se log_file=1 escreve no ficheiro log

    }
    char *ultima_linha= NULL;// guarda o input anterior 
    
    while(1)
    {
        char *linha_lida = leitura_linha(input_global);//$chama leitura linha le o input; aloca memoria e guarda um ponteiro linha_lida 
            if (linha_lida == NULL)
        {break;}
        
        if (log_file) fprintf(log_file, "%s\n", linha_lida);//guarda input no log 
        //$ remover char invalidos espacos e mete em maisculas
        limpar_input(linha_lida);
        char *linha_limpa =sem_espacos(linha_lida);
        maiuscula(linha_limpa);

        if(linha_limpa[0] =='\0')
        {free(linha_lida);
        continue;}// se linha nao tiver keywords pede nova linha
        
        const char *palavra_fim;
        if (modo_portugues ==1)
        {
            palavra_fim = ("ADEUS");
        }
        else
        {
            palavra_fim = ("BYE");
        }
        if (strcmp (linha_limpa, palavra_fim) == 0)//$ compara input do utilizador com palavra fim se corresponder a 0 termina cycle
        {
            if (output_fim != NULL)
            {
                fprintf(output_global, "%s\n", output_fim);//$ imprime mensagem de despedida
        if (log_file) fprintf (log_file, "%s\n",output_fim);//$guarda no log se log esta ativo
            }
        free(linha_lida);
        break;
        }                    

    
    if (ultima_linha!= NULL &&(strcmp (linha_limpa, ultima_linha)==0 ))
    {
        if(output_repeticao != NULL)
        {
            fprintf(output_global, "%s\n",output_repeticao );
            if (log_file) fprintf(log_file,"%s\n", output_repeticao);
        }
        free(linha_lida);
        continue;//$nao atuualiza nada pq é repetido
    }
    if (ultima_linha != NULL) free(ultima_linha);//a primeira iteraçao é null nao ha nada para libertar
    ultima_linha = copiar_string(linha_limpa);//Guardamos uma cópia da linha atual para na próxima iteração podermos comparar com o novo input e detetar repetições.
    
    Key_Resposta *conjunto = encontrar_keyw (linha_limpa);//$ procura keyw no DB que aparece no input
    if(conjunto!= NULL)
    {
        char *resp = gerar_nova_resposta(conjunto);//$gerar resposta e alterar de forma circular
        
        if(resp !=NULL)
        {    // $procura asterisco na resposta
            char *asterisco = strchr (resp,'*');
            if (asterisco != NULL)
            {
                char *resto= NULL;
                for (int i= 0; i < conjunto->num_keywords; i++) {
                    resto= procura_keywords(linha_limpa, (*conjunto).keywords[i]);
                    if (resto) break;
                }
                char *conjugado;
                if (resto != NULL) {
                    conjugado= pronomes(resto, !modo_portugues);
                } else {
                    conjugado= pronomes("", !modo_portugues);
                }

                //$resposta final: oarte antes do * texto com conjugaçao e resto depois do *
                int tamanho_antes =( asterisco -resp);//$ numerom de elementos antes do *
                char *resposta_final =malloc(tamanho_antes +strlen(conjugado)+strlen(asterisco+1)+ 2);  //o espaço necessario para o print final, tamanho antes do asterisco +"conjugado + tamanho do texto depois do asterisco +1 porque str acaba com \0"
                                                                                                        //$asterisco+1 porque precisamos o caharacter depois do *, +1 para o espaço antes do conjugado
                    strncpy(resposta_final, resp, tamanho_antes);//$copia tudo antes do asterisc
                    resposta_final[tamanho_antes] = '\0';//$ termina a string para strcat saber onde colar
                    if (strlen(conjugado) > 0 && tamanho_antes > 0 && resp[tamanho_antes-1] != ' ')
                        strcat(resposta_final, " ");
                    strcat(resposta_final, conjugado);
                    strcat(resposta_final, asterisco+1);//$copy paste
                

                    fprintf(output_global, "%s\n",resposta_final );
                    if (log_file) fprintf(log_file, "%s\n",resposta_final );
                    free(conjugado);
                    free(resposta_final);
                    }
                    else 
                    {
                    fprintf(output_global, "%s\n", resp);
                    if (log_file) fprintf(log_file, "%s\n",resp);
                    }
        
        }
    }
        free (linha_lida);
    }
                libertar_memoria();
                if (log_file !=NULL) fclose(log_file);
                if(input_global!= stdin ) fclose(input_global);
                 if(output_global != stdout) fclose(output_global);
                if (ultima_linha != NULL) free(ultima_linha);
                 return EXIT_SUCCESS;
    
    }
