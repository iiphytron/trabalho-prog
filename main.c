#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>//$para usar getopt, ler linha de commando
#include "asterisco.h" // $ substituir por pornomes quando encontra *
#include "base_dados.h" //$ le o ficheiro  e liberta memoria 
#include "funcoes.h" //$elimina espaços, tudo em maisculas, copia o string

#define MAX_LENGTH 256 //tamanho max do input


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
    while ((opt = getopt (argc, argv, "h:o:l:f:p") )!=-1)
    {
        switch (opt)
        {
        case 'h':
        printf("nigga");
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
        printf(output_global, "%s", output_inicio);
        if (log_file)
        printf(log_file,"%s, output_inicio");//$ se log_file=1 escreve no ficheiro log

    }
    char buffer[MAX_LENGTH]; //$ spot para guardar a linha lida
    char *ultima_linha= NULL;// guarda o input anterior 
    
    while(1)
    {
        printf(output_global, "input>");
        if (fgets(buffer,MAX_LENGTH, input_global)==NULL) // fgets returns NULL se chegarmos a End Of FIle
        {break;}
        
        if (log_file) printf(log_file, %s, buffer);//guarda input no log 
        //$ remover char invalidos espacos e mete em maisculas
        limpar_input(buffer);
        char *linha_limpa =sem_espacos(buffer);
        maiscula(linha_limpa);

        if(linha_limpa[0] =='\0')
        {continue;}// se linha nao tiver keywords pede nova linha
        
        const char *palavra_fim;
        if (modo_portugues =1)
        {
            palavra_fim = ("ADIOS");
        }
        else
        {
            palavra_fim = ("BYE");
        }
        if (strcmp (linha_limpa, palavra_fim) == 0)//$ compara input do utilizador com palavra fim se corresponder a 0 termina cycle
        if (output_fim != NULL)
        {
            printf(output_global, "%s", output_fim);//$ imprime mensagem de despedida
        if (log_file) printf (log_file, "%s",output_fim);//$guarda no log se log esta ativo
        }
        break;

    }
    if (ultima_linha!= NULL &&(strcmp (linha_limpa, ultima_linha)==0 ))
    {
        if(output_repetiçao != NULL)
        {
            printf(output_global, "%s",output_repetiçao );
            if (log_file) printf(log_file,"%s", output_repetiçao);
        }
        continue;//nao atuualiza nada pq é repetido
    }
    Key_Resposta *conjunto = encontrar_keyw (linha_limpa);// procura keyw no DB que aparece no input
    if(conjunto!= NULL)
    {
        char *resp = gerar_nova_resposta(conjunto);//gerar resposta e alterar de forma circular
        
        if(resp !=NULL)
        {
            char *asterisco = strchr (resp,'*');
            if (asterisco != NULL)
        }

    }

}