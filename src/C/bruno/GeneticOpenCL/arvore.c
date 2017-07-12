#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <float.h>
#include "arvore.h"

///N�o est� sendo utilizado no programa principal
void inicializaArvore(Arvore* arv){
    arv->numNos = 0;
}

int calculaTamanhoSubArvore(Arvore* arv, int indice){
    int soma = arv->numeroFilhos[indice];
    int tam = 1;

    while(soma != tam-1){
        indice++;
        soma += arv->numeroFilhos[indice];
        tam++;
    }
    return tam;
}

void geradorArvore(Arvore* arv, int maxTam, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N, int* seed,
                    int maxDados, int minDados){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int soma = 1;

    int aux[MAX_NOS];
    int sorteio, j, num, tipo;

    //usar 'num' o inves de j pra nao ter variavel sobrando...
    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    arv->numNos = 0;
    empilha(&pilha, indice);

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == 0){

            int rdmType = randomType(NUM_OPBIN, NUM_OPUN, N, seed);
            //printf("RDMtipo = %d\n", rdmType);
//
            sorteio = conjuntoOpTerm[rdmType];
            tipo = unpackTipo(sorteio);
            //printf("sorteio = %d\n", sorteio);
            //printf("tipo = %d\n", tipo);

            if(tipo == VAR || tipo == CTE){
                num = 0;
            } else if (tipo == FUN){
                num = 1;
            } else if (tipo == FBIN){
                num = 2;
            }

            arv->numeroFilhos[indice] = num;
            arv->numNos++;
            soma += num;

        if(soma > maxTam){
            soma-=arv->numeroFilhos[indice];
            arv->numeroFilhos[indice] = 0;
            sorteio = conjuntoOpTerm[randomLeafType(NUM_OPBIN, NUM_OPUN, N,seed)];
            num = 0;
            tipo = unpackTipo(sorteio);
        }

        if(tipo == CTE){
            /**AQUI*/
            //printf("tipooooooo = %d\n", tipo);

            //printf("RandomConst = %f\n", constante);
            sorteio = packFloat(CTE, randomConst(seed, maxDados, minDados));
            //printf("tipo = %d\n", unpackTipo(sorteio));
            //printf("RandomConstUnpacked = %f\n", unpackFloat(sorteio));

            //char c;
            //scanf("%c", &c);
        }
        arv->informacao[indice] = sorteio;

    }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }
}


void criaCheia(Arvore* arv, int maxDepth, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N, int* seed,
               int maxDados, int minDados){

    //conferir de alguma forma que cabe uma �rvore cheia no vetor (no caso confere para uma arvore binaria)
    //nao funciona no caso se sortear '1' filho v�rias vezes, pois acaba cabendo uma arvore mais profunda e ainda 'cheia'
    //if(pow(2, maxDepth+1)-1 > MAX_NOS)
    //printf("Espaco insuficiente para arvore cheia nesta profundidade.");
    //return;

    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int j, num, tipo, sorteio;
    //se tirar 'soma' tem que garantir que a profundidade maxima garanta uma arvore nao-degenerada
    //int soma = 1;

    int aux[MAX_NOS];

    //usar 'num' o inves de j pra nao ter variavel sobrando...
    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }
    arv->numNos = 0;
    empilha(&pilha, indice);

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == 0){
            if(pilha.topo+1 != maxDepth){
                sorteio = conjuntoOpTerm[randomNodeType(NUM_OPBIN, NUM_OPUN, N,seed)];
                tipo = unpackTipo(sorteio);
                if (tipo == FUN){
                    num = 1;
                } else if (tipo == FBIN){
                    num = 2;
                }

            } else {
                sorteio = conjuntoOpTerm[randomLeafType(NUM_OPBIN, NUM_OPUN, N,seed)];
                tipo = unpackTipo(sorteio);
                if(tipo == CTE){
                        /**AQUI*/
                    //printf("tipooooooo = %d\n", tipo);
                    //float constante = randomConst(seed);
                   // printf("RandomConst = %f\n", constante);
                    sorteio = packFloat(CTE, randomConst(seed,maxDados, minDados));
                    //printf("tipo = %d\n", unpackTipo(sorteio));
                    //printf("RandomConstUnpacked = %f\n", unpackFloat(sorteio));

            //char c;
            //scanf("%c", &c);
                }
                num = 0;
            }
            arv->numeroFilhos[indice] = num;
            arv->numNos++;
            arv->informacao[indice] = sorteio;
        }

        /*
        if(soma > MAX_NOS){
            soma-=arv->numeroFilhos[indice];
            arv->numeroFilhos[indice] = 0;
        }
        */

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }
}

//pos ordem -avaliar / conversor
void imprimeArvoreNivel(Arvore* arv){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int aux[MAX_NOS];
    int j;

    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    if(arv->numNos != 0)
        empilha(&pilha, indice);

    while(pilha.topo != -1){

        indice = desempilha(&pilha);

        if(aux[indice] == 0){
            printf("(%d)", pilha.topo+1);
            for(j = 0; j <= pilha.topo+1; j++)
                printf("-");

            int tipo = unpackTipo(arv->informacao[indice]);
            if(tipo == CTE)
                printf("%f | %d\n", unpackFloat(arv->informacao[indice]), arv->numeroFilhos[indice]);
            else
                printf("%d | %d\n", unpackInt(arv->informacao[indice]), arv->numeroFilhos[indice]);
                //printf("%d | %d\n", arv->informacao[indice], arv->numeroFilhos[indice]);
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }
}

void imprimeArvorePre(Arvore* arv, char** LABELS){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int aux[MAX_NOS];
    int j;

    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    if(arv->numNos != 0){
        empilha(&pilha, indice);
    }

    while(pilha.topo != -1){
        indice = desempilha(&pilha);
        if(aux[indice] == 0){
            printf("("/*%d", arv->informacao[indice]*/);

            //imprimeSinxate(arv->informacao[indice], LABELS); //arv->informacao[indice], arv->numeroFilhos[indice]);
            imprimeSinxate(arv, indice, LABELS);
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        } else {
            printf(")");
        }
    }
}

void imprimeArvorePos(Arvore* arv, char** LABELS){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int aux[MAX_NOS];
    int j;

    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    if(arv->numNos != 0){
        empilha(&pilha, indice);
        printf("(");
    }

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == arv->numeroFilhos[indice]){
            //imprimeSinxate(arv->informacao[indice], LABELS);
            imprimeSinxate(arv, indice, LABELS);
            printf(")");
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            printf("(");
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }

    }
}

void shift(Arvore* arv, int tam, int indice){//indice a partir de onde come�a o shift, tam = tamanho do shift
    int i;
    if(tam > 0){
        for(i = arv->numNos-1; i >= indice; i--){
            arv->numeroFilhos[i+tam] = arv->numeroFilhos[i];
            arv->informacao[i+tam] = arv->informacao[i];
        }
    } else {
        for(i = indice; i < arv->numNos; i++){
            arv->numeroFilhos[i+tam] = arv->numeroFilhos[i];
            arv->informacao[i+tam] = arv->informacao[i];
        }
    }
    arv->numNos += tam;
}

void mutacao(Arvore* arvore, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N, int* seed,
             int maxDados, int minDados){
    int i;
    //sorteia uma subarvore da arvore inicial
        //pega o tamanho dessa subarvore
    int indiceSub = rand2(seed) % (arvore->numNos);
    int tamanhoSub = calculaTamanhoSubArvore(arvore, indiceSub);

    //confere o tamanho max para criar uma nova arvore
        //tamanho disponivel+tamanho da subarvore sorteada
    int espacoLivre = MAX_NOS-(arvore->numNos)+tamanhoSub;

    //cria uma nova arvore aleatoria de at� o tamanho maximo calculado
    //TODO: criar uma nova arvore ou nao?
    Arvore novaArvore;
    //inicializaArvore(&novaArvore);
    geradorArvore(&novaArvore, espacoLivre, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N,seed, maxDados, minDados);


    //determina o tamanho do deslocamento da arvore

//TODO: colocar if para raiz?
//if(indiceSub != 0 && indiceSub != arvore->numNos-1){
        int tamShift = novaArvore.numNos - tamanhoSub;
        shift(arvore, tamShift, indiceSub+tamanhoSub);


    //troca a informa��o
    for(i = 0; i < novaArvore.numNos; i++){
        arvore->numeroFilhos[i+indiceSub] = novaArvore.numeroFilhos[i];
        arvore->informacao[i+indiceSub] = novaArvore.informacao[i];
    }
}

void crossOver(Arvore* arvore1, Arvore* arvore2, int* seed){

    int espacoLivre1, espacoLivre2, indiceSub1, indiceSub2, tamanhoSub1, tamanhoSub2;
    int cont = 0;
    do{
        indiceSub1 = rand2(seed) % (arvore1->numNos);
        tamanhoSub1 = calculaTamanhoSubArvore(arvore1, indiceSub1);

        indiceSub2 = rand2(seed) % (arvore2->numNos);
        tamanhoSub2 = calculaTamanhoSubArvore(arvore2, indiceSub2);

        espacoLivre1 = MAX_NOS-(arvore1->numNos)+tamanhoSub1;
        espacoLivre2 = MAX_NOS-(arvore2->numNos)+tamanhoSub2;
        if(cont++ == 5) return;
    }while(espacoLivre1-tamanhoSub2 < 0 || espacoLivre2-tamanhoSub1 < 0);

//    Arvore arvAux;
//    for(i = 0; i < tamanhoSub1; i++){
//        arvAux.informacao[i] = arvore1->informacao[indiceSub1+i];
//        arvAux.numeroFilhos[i] = arvore1->numeroFilhos[indiceSub1+i];
//    }

    int tamShift1 = tamanhoSub2 - tamanhoSub1;
    //shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);

    int tamShift2 = tamanhoSub1 - tamanhoSub2;


    if(tamanhoSub1 >= tamanhoSub2){
        shift(arvore2, tamShift2, indiceSub2+tamanhoSub2);
        trocaSubArv(arvore1, arvore2, indiceSub1, indiceSub2, tamanhoSub1, tamanhoSub2);
        shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);
    } else {
        shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);
        trocaSubArv(arvore2, arvore1, indiceSub2, indiceSub1, tamanhoSub2, tamanhoSub1);
        shift(arvore2, tamShift2, indiceSub2+tamanhoSub2);
    }

//    for(i = 0; i < tamanhoSub2; i++){
//        arvore1->numeroFilhos[i+indiceSub1] = arvore2->numeroFilhos[i+indiceSub2];
//        arvore1->informacao[i+indiceSub1] = arvore2->informacao[i+indiceSub2];
//    }
//
//    for(i = 0; i < tamanhoSub1; i++){
//        arvore2->numeroFilhos[i+indiceSub2] = arvAux.numeroFilhos[i];
//        arvore2->informacao[i+indiceSub2] = arvAux.informacao[i];
//    }
}

void  trocaSubArv(Arvore* arvMaior,Arvore* arvMenor,int ind1,int ind2,int tamanhoSubMenor, int tamanhoSubMaior){
    int i=0, aux;
    while(i<tamanhoSubMenor){
        aux = arvMaior->informacao[ind1+i];
        arvMaior->informacao[ind1+i] = arvMenor->informacao[ind2+i];
        arvMenor->informacao[ind2+i] = aux;
        aux = arvMaior->numeroFilhos[ind1+i];
        arvMaior->numeroFilhos[ind1+i] = arvMenor->numeroFilhos[ind2+i];
        arvMenor->numeroFilhos[ind2+i] = aux;
        i++;
    }
    while(i<tamanhoSubMaior){
        arvMenor->informacao[ind2+i] = arvMaior->informacao[ind1+i];
        arvMenor->numeroFilhos[ind2+i] = arvMenor->numeroFilhos[ind2+i];
    }
}

void imprimeSinxate(Arvore* arv, int j, char* LABELS[]){ //int id, int tipo){
    int tipo = retornaTipo(arv, j);
    int info = arv->informacao[j];
    //unpackTipo(info);
    switch(tipo){
        case PLUS:
            printf("+");
            break;
        case MIN:
            printf("-");
            break;
        case MULT:
            printf("*");
            break;
        case DIV:
            printf("/");
            break;
        case SIN:
            printf("sin");
            break;
        case COS:
            printf("cos");
            break;
        case SQR:
            printf("sqrt");
            break;
        case EXP:
            printf("exp");
            break;
        case CTE:;//This is an empty statement.
            float valorF = unpackFloat(info);
            printf("%.5f", valorF);
            break;
        case VAR:;
            int valor2 = unpackInt(info);
            printf("%s", LABELS[valor2]);
            break;
        default:
            break;

    }
}

int retornaTipo(Arvore* arv, int j){
    if(arv->numeroFilhos[j] == 0){
        return unpackTipo(arv->informacao[j]);
    } else {
        return unpackInt(arv->informacao[j]);
    }
}


float executa(Arvore* arv, float dados[], int N){

    PilhaEx pilhaEx;
    pilhaEx.topo = -1;

    int j;
    int tipo;

    for(j = arv->numNos -1; j>=0; j= j-1){
        tipo = retornaTipo(arv, j);
        //unpackTipo(arv->informacao[j]);
        //printf("tipo = %d\n", tipo);
        switch(tipo){
            case PLUS:
                empilha2(&pilhaEx,desempilha2(&pilhaEx) + desempilha2(&pilhaEx));
                break;
            case MIN:
                empilha2(&pilhaEx,desempilha2(&pilhaEx) - desempilha2(&pilhaEx));
                break;
            case MULT:
                empilha2(&pilhaEx,desempilha2(&pilhaEx) * desempilha2(&pilhaEx));
                break;
            case DIV:
                empilha2(&pilhaEx,proDiv (desempilha2(&pilhaEx), desempilha2(&pilhaEx)));
                break;
            case SIN:
                empilha2(&pilhaEx,sin(desempilha2(&pilhaEx)));
                break;
            case COS:
                empilha2(&pilhaEx,cos(desempilha2(&pilhaEx)));
                break;
            case SQR:
               empilha2(&pilhaEx,proSqrt(desempilha2(&pilhaEx)));
                break;
            case EXP:
                empilha2(&pilhaEx,expf(desempilha2(&pilhaEx)));
                break;

            case CTE:;//This is an empty statement.
                //int c; scanf("%d", c);
                float valorF = unpackFloat(arv->informacao[j]);
                //printf("valorCte = %f", valorF);
                empilha2(&pilhaEx, valorF);
                break;
            case VAR:;
                int valor2 = unpackInt(arv->informacao[j]);
                empilha2(&pilhaEx, dados[valor2]);
                break;
        }
    }

    float erro = desempilha2(&pilhaEx)- dados[N-1];
    erro = ( isinf( erro ) || isnan( erro ) ) ? /*FLT_MAX*/ INFINITY : erro;
    //printf("erro = %f\n", erro*erro);
    return erro*erro;
}

float opBinaria(int operador, float valor1, float valor2){
    switch(operador){
        case PLUS:
            return (valor1 + valor2);
        case MIN:
            return (valor1 - valor2);
        case MULT:
            return (valor1 * valor2);
        case DIV:
            return proDiv(valor1, valor2);
    }
    return -1;
}

float opUnaria(int operador, float valor){
    switch(operador){
        case SIN:
            return (float)sin(valor);
        case COS:
            return (float)cos(valor);
        case SQR:
            return proSqrt(valor);
        default:
            return -1;
    }
    return -1;
}

void testaExecuta(float* dados[]){
//    Arvore arvore1;
//    criaArvTeste(&arvore1);
//    imprimeArvorePre(&arvore1);

//    int i, j;
//    for(i = 0; i < M; i++){
//        for(j = 0; j < N; j++){
//            printf("%.2f ", dados[i][j]);
//        }
//        printf("\n");
//    }

//    printf("%f", executa(&arvore1, dados[1]));
}

void generate(Arvore* arv, int min, int max){
    int j;
    int tamanho = min + randomSuperior(max-min+1);
    arv->numNos = tamanho;
    //printf("p=%d\n", tamanho);
    int d[max];
    arv->numeroFilhos[0] = randomSuperior(tamanho-1)%3;
    //printf("numF=%d", arv->numeroFilhos[0]);
    d[0] = tamanho-1;

    for(j = 1; j < tamanho-1; j++){
        //printf("aaaaaaa\n");
        d[j] = d[j-1]-arv->numeroFilhos[j-1];
        if(d[j] == (tamanho-j-1)){
            //printf("bbbbbbb\n");
            arv->numeroFilhos[j] = 1 + randomSuperior(d[j])%2;

        } else {
            //printf("ccccccc\n");
            //printf("%d\n", d[j]);
            arv->numeroFilhos[j] = randomSuperior(d[j])%3;

        }
    }
    arv->numeroFilhos[tamanho-1] = 0;
    return;
}

void testaPrint(Arvore *arvore){
//    printf("POR NIVEL: \n");
//    imprimeArvoreNivel(arvore);
//    printf("\nPOS ORDEM: \n");
//    imprimeArvorePos(arvore);
//    printf("\nPRE ORDEM: \n");
//    imprimeArvorePre(arvore);
}

void criaArvTeste(Arvore *arvore1){
    arvore1->numeroFilhos[0] = 2;
    arvore1->numeroFilhos[1] = 0;
    arvore1->numeroFilhos[2] = 2;
    arvore1->numeroFilhos[3] = 2;
    arvore1->numeroFilhos[4] = 1;
    arvore1->numeroFilhos[5] = 1;
    arvore1->numeroFilhos[6] = 0;
    arvore1->numeroFilhos[7] = 0;
    arvore1->numeroFilhos[8] = 0;


    arvore1->informacao[0] = packInt(FBIN, 1);
    arvore1->informacao[1] = packInt(VAR, 0);
    arvore1->informacao[2] = packInt(FBIN, 2);
    arvore1->informacao[3] = packInt(FBIN, 1);
    arvore1->informacao[4] = packInt(FUN, 5);
    arvore1->informacao[5] = packInt(FUN, 6);
    arvore1->informacao[6] = packInt(VAR, 0);
    arvore1->informacao[7] = packInt(VAR, 0);
    arvore1->informacao[8] = packInt(VAR, 0);

    arvore1->numNos = 9;
}

void realizaTestes(){

    int i;
    Arvore arvore1, arvore2;


    inicializaArvore(&arvore1);
    inicializaArvore(&arvore2);
    for(i = 0; i < MAX_NOS ; i++){
        arvore1.numeroFilhos[i]=-99;
    }
    //teste(&arvore1);
    generate(&arvore1,5, 10);
    //geradorArvore(&arvore1, MAX_NOS);
//    criaCheia(&arvore2, 2);
//
//    imprimeArvoreNivel(&arvore1);
//    printf("\n\n");
//    imprimeArvoreNivel(&arvore2);
//    printf("\n\n");

    for(i = 0; i < MAX_NOS ; i++){
        printf("%d ", arvore1.numeroFilhos[i]);
    }

    printf("\n");



//    for(i = 0; i < arvore2.numNos ; i++){
//        printf("%d ", arvore2.numeroFilhos[i]);
//    }
//
//    printf("\n\n");
//
//    printf("MUTA��O ARVORE 1: \n");
//    mutacao(&arvore1);
//    imprimeArvoreNivel(&arvore1);
//    printf("\n\n");
//
//    printf("CROSS OVER ARVORES 1 E 2: \n");
//    crossOver(&arvore1, &arvore2);
//
//    imprimeArvoreNivel(&arvore1);
//    printf("\n");
//    imprimeArvoreNivel(&arvore2);
//    printf("\n");
}

//TODO:fazer receber os dados tamb�m+labels.
void leIndividuo(char *fileName, Arvore* individuo, char** LABELS, float** dados, int M, int N) {
    //printf("%s", fileName);
    FILE* arquivo = fopen(fileName, "r");
    char cp[100000];
    int posicao = 0;
    int informacao;

//    fseek(arquivo, 0, SEEK_END);
//    long fsize = ftell(arquivo);
//    fseek(arquivo, 0, SEEK_SET);


    if (arquivo == NULL) {
        fprintf(stderr, "Error opening data file.\n");
        return;
    }
    const char delimiters[] = " ()";

    while (fgets(cp, 100000, arquivo)) {

        char *token;

        token = strtok (cp, delimiters);      /* token => "words" */
          //  printf("%s\n", token);

        while(token != NULL){
            //printf("%s\n", token);
            if(strcmp(token, "+") == 0){
                //printf("aaa\n");
                informacao = packInt(FBIN, PLUS);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "-") == 0){
                informacao = packInt(FBIN, MIN);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "*") == 0){
                informacao = packInt(FBIN, MULT);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "/") == 0){
                informacao = packInt(FBIN, DIV);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "sen") == 0){
                informacao = packInt(FUN, SIN);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(strcmp(token, "cos") == 0){
                informacao = packInt(FUN, COS);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(strcmp(token, "sqrt") == 0){
                informacao = packInt(FUN, SQR);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(token[0] == 'x'){
                token++;
                informacao = packInt(VAR, atoll(token)-1);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 0;
            } else {
                informacao = packFloat(CTE, atof(token));
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 0;
            }
            posicao++;
            token = strtok (NULL, delimiters);
        }
        individuo->numNos = posicao;
//        int i;
//        for(i= 0; i < posicao-1; i++){
//            printf("%d ", individuo->informacao[posicao]);
//        }
        imprimeArvorePre(individuo, LABELS);
        printf("\nnos = %d\n", individuo->numNos);
        //executa(individuo, dados[0], N);
        int j;
        float erro = 0;
        for(j = 0; j < M; j++){
            //printf("%d ", j);
            erro = erro + executa(individuo, dados[j], N);
        }
        printf("erro = %.20f", erro);

//        token = strtok (NULL, delimiters);    /* token => "separated" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "by" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "spaces" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "and" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "punctuation" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => NULL */
//        printf("%s", token);
    }

}