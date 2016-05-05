#ifndef INDIVIDUO_H
#define INDIVIDUO_H
#include "../No.h"
#include <cstdlib>
#include <iostream>
#include <math.h>
using namespace std;
class Individuo
{
    public:
        Individuo(int lin, int col, int num_entradas, int num_saidas);
        virtual ~Individuo();
        No ***matrizNo;
        No **entradas, **saidas;
        int pontuacao;
        int qtdLigantes;
        int num_linhas, num_colunas, num_saidas, num_entradas;
        void imprime();
        void avalia(bool **tabela_entrada, bool **tabela_saida);

    protected:
    private:
};

#endif // INDIVIDUO_H
