//Arthur Yan Merkle
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILE_PATH "./redesocial2.txt"

typedef struct Pessoa Pessoa;

struct Pessoa{
    int id;
    int idade;
    int sexo;
};

//Função de criação da rede social

void criar_rede_social(unsigned int ***rede_social, unsigned int n){
    unsigned int x, y;
    (*rede_social) = (unsigned int **)malloc(sizeof(unsigned int *) * n);
    for (x = 0; x < n; x++){
        *((*rede_social) + x) = (unsigned int *)malloc(sizeof(unsigned int) * n);
        for ( y = 0; y < n; y++){
            *(*((*rede_social) + x) + y) = 0;
        }
    }
}

// Geração de números aleatórios
float num_random(){
    return (float)((double)rand()) / RAND_MAX;
}

//Printa rede social formada
void printa_rede_social(unsigned int **rede_social, unsigned int n){
    unsigned int x, y;
    for ( x = 0; x < n; x++){
        for ( y = 0; y < n; y++){
            printf("%d", *(*(rede_social + x) + y));
        }
        printf("\n");
    }
}

//Printar Pessoa
void printa_pessoa(Pessoa pessoa){
    printf("Id: (%d) | idade: %d | sexo %d: ", pessoa.id, pessoa.idade, pessoa.sexo);
}

void printa_pessoas( Pessoa *pessoas, unsigned int n){
    unsigned int x;
    for (x = 0; x < n; x++){
        printa_pessoa(*(pessoas + x));
    }
}

//Printar pessoas com 30 anos ou mais
void printa_pessoas_com_30_ou_mais(Pessoa *pessoas, unsigned int n){
    unsigned int x;

    for ( x = 0; x < n; x++){
        if ((pessoas + x) -> idade > 30){
            printa_pessoa ( * (pessoas + x) );
        }
    }
}

//Conta o número de pessoas do sexo masculino e feminino
void printa_count_sexo_pessoas (Pessoa *pessoas, unsigned int n){
    unsigned int x, f, m;
    f = 0;
    m = 0;
    for (x = 0; x < n; x++){
        if ((pessoas + x) -> sexo == 1){
            f++;
        } else{
            m++;
        }
    }
    printf("\tExistem %d mulheres e %d homens na lista de amigos\n ", f, m);
}

//Printa a rede social com todas as pessoas
void printa_rede_social_com_pessoas (unsigned int **rede_social, Pessoa *pessoas, unsigned int n){
    printa_rede_social (rede_social, n);
    printa_pessoas (pessoas, n);
}

//Envio de solicitação de amizade
void add_amizade (unsigned int **rede_social, unsigned int pos_x, unsigned int pos_y){
    *(*(rede_social + pos_x) + pos_y) = 1;
    *(*(rede_social + pos_y) + pos_x) = 1;
}

// Contador de amizades
void count_amizades ( unsigned int **rede_social, unsigned int n, unsigned int pessoa, unsigned *out){
    if ( pessoa >= n){
        return;
    }
    unsigned int y;
    *out = 0;
    for (y = 0; y < n; y++){
        if (*(*(rede_social + pessoa) + y) && y != pessoa){
            (*out) ++;
        }
    }
}

//Criação da rede social através de um arquivo
void criar_rede_social_de_um_arquivo (unsigned int ***rede_social, unsigned int n, const char *path){
    unsigned int x, y;
    int aux_c;
    FILE *arq_read;
    arq_read = fopen(path, "r");
    criar_rede_social (rede_social, n);
    x = 0;
    y = 0;
    unsigned int flags [] = {0, 0};
    while (!feof (arq_read) ){
        aux_c = fgetc (arq_read);
        if ( aux_c == EOF){
            continue;
        } else if ( aux_c == '\n'){
            flags[0] = 0;
        } else if (!flags[0] && (char)aux_c == '|'){
            flags[0] = 1;
        } else if (flags [1] && (char)aux_c == ' '){
            flags[1] = 0;
            add_amizade (*rede_social, x, y);
            y = 0;
        } else if (flags[0] && (char)aux_c != '|' && (char)aux_c != ' '){
            flags[1] = 1;
            y *= 10;
            y += aux_c - 48;
        }
    }
    fclose(arq_read);
}

// Amizades em comum da rede social criada através do arquivo
void count_amizades_comum_do_arquivo (unsigned int pessoa_u,
                                      unsigned int pessoa_v,
                                      unsigned n, const char *path,
                                      unsigned *out){
    unsigned int y;
    *out = 0;
    unsigned int **rede_social;
    criar_rede_social_de_um_arquivo (&rede_social, n, path);
    for ( y = 0; y < n; y++){
        if (*(*(rede_social + y) + pessoa_u) && *(*(rede_social + y) + pessoa_v) && 
            y != pessoa_u && y != pessoa_v){
          (*out) ++;  
        }
    }                                      
}

//Salvar a rede social em um arquivo
void salvar_rede_social_no_arquivo ( unsigned int **rede_social, unsigned int n, const char *path){
    unsigned int x, y;
    FILE *save_arq;
    save_arq = fopen (path, "w");
    if ( !save_arq ){
        return;
    }
    for (x = 0; x < n; x ++){
        fprintf(save_arq, "%u | ", x);
        for ( y = 0; y < n; y++){
            if (*(*(rede_social + x) + y)){
                fprintf(save_arq, "%u | ", y);
            }
        }
        fprintf(save_arq, "\n");
    } 
    fclose(save_arq);  
}

//Popular o vetor de pessoas
void populate_pessoas_vetor ( Pessoa *pessoas, unsigned int n){
    unsigned int i;
    for (i = 0; i < n; i++){
        (pessoas + i) -> id = (int)i;
        (pessoas + i) -> idade = (int)(num_random() * 80);
        (pessoas + i) -> sexo = (int)(num_random() > 0.5 ? 1 : 0);
    }
}

//Criação do vetor de pessoas

void criar_vetor_pessoas (Pessoa **pessoas, unsigned int n){
    (*pessoas) = (Pessoa *)malloc(sizeof(Pessoa) * n);
}

//Popular a rede social
void populate_rede_social (unsigned int **rede_social, unsigned int n, float fator_p){
    unsigned int i, j;
    float random_num;
    for (i = 0; i < n; i ++){
        for ( j = 0; j < n; j++){
            random_num = num_random();
            if (random_num < fator_p){
                add_amizade (rede_social, i, j);
            }
        }
    }
}

//Coeficiente de aglomeração
float coeficiente_aglomero (unsigned int **rede_social, unsigned int n, unsigned int i){
    float coefc;
    unsigned int counter, N, u, aux;
    counter = 0;
    count_amizades(rede_social, n, i, &n);
    for ( u = 0; u < n; u ++){
        if (*(*(rede_social + i) + u)){
            count_amizades_comum_do_arquivo (i, u, n, FILE_PATH, &aux);
            counter += aux;
        }
    }
    coefc = (float)counter / ((float)N * ((float)N - 1) / 2);
    return coefc;
}

void desalocar_rede_social (unsigned int **rede_social, unsigned int n){
    unsigned int x;

    for ( x = 0; x < n; x++){
        free (*(rede_social + x));
    }
    free (rede_social);
}

void desalocar_pessoas( Pessoa *pessoas, unsigned int n){
    free(pessoas);
}

int main(int argc, char const *argv[]){
    
    clock_t  start_time, end_time;

    
    setlocale(LC_ALL, "Portuguese");
    srand( time (NULL));

    unsigned int qtd_pessoas, cont_amizades;
     
    // Fator de probabilidade de u ser amigo de v
    float fat_p;

    //Rede Social
    unsigned int **rede_social;

    //Vetor de pessoas
    Pessoa *pessoas;

    rede_social = NULL;
    pessoas = NULL;

    //Recebe quantidade de pessoas que terão na rede social e no vetor 
    do{
        printf("Digite a quantidade de pessoas que estarão na rede social: ");
        scanf("%d", &qtd_pessoas);
    }while (qtd_pessoas <= 0);

    //Recebe o fator p
    do{
        printf("Digite o fator p que populará a rede social: ");
        scanf("%f", &fat_p);
    }while (fat_p < 0 || fat_p > 1);
    
    start_time = clock();
    //Alocar a memória
    criar_rede_social (&rede_social, qtd_pessoas);
    criar_vetor_pessoas (&pessoas, qtd_pessoas);

    //Popular a memória com dados
    populate_rede_social (rede_social, qtd_pessoas, fat_p);
    populate_pessoas_vetor (pessoas, qtd_pessoas);
    //Chamar a função salvar rede social no arquivo
    salvar_rede_social_no_arquivo(rede_social, qtd_pessoas, FILE_PATH);
    count_amizades_comum_do_arquivo (5, 6, qtd_pessoas, FILE_PATH, &cont_amizades);
    printf("%u e %u tem %u amigos em comum \n ", 5, 6, cont_amizades);
    //
    count_amizades (rede_social, qtd_pessoas, 5, &cont_amizades);
    printf("%u tem %u amigos \n", 5, cont_amizades);
    printa_rede_social_com_pessoas (rede_social, pessoas, qtd_pessoas);
    printa_pessoas_com_30_ou_mais ( pessoas, qtd_pessoas);
    printa_count_sexo_pessoas (pessoas, qtd_pessoas);

    //Desalocar a rede social
    desalocar_rede_social( rede_social, qtd_pessoas);
    desalocar_pessoas   ( pessoas, qtd_pessoas);

    end_time = clock();
    printf("O programa levou %lu segundos para executar. \n", (end_time - start_time) / CLOCKS_PER_SEC);
    return 0;
}