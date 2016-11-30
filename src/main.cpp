#include <iostream>
#include <ctime>
#include "Database.h"
#include "Grammar.h"
#include "No.h"
#include "Configures.h"
#include "Search.h"
#include "Configures.h"
#include "SimpleParser.h"
#include "../inc/ParserDerivadas.h"
#include "../inc/LeastSquareParser.h"
#include "../inc/LSDerivadasParser.h"

using namespace std;

//#define parserLeastSquare
#define parserLSDerivadas
int main(){
#ifdef MAKEDATA
    double a, e = 0;
    int size = 200;
    for(int i = 0; i < size; i++){
        a = (double)i/100;

        cout << a << " ";

//        cout << a + b + c - d - e << endl;
        cout << cos(a) << endl;
    }

    for(int i = 0; i < size; i++)
        cout << i << " ";

    cin.get();
#endif


    srand(time(NULL));
    cout << "Hello GP-ufjf!" << endl;
    conf = new Configures();

    //set parametros
    conf->MAXDEEP = 6;

    conf->generations = 2000;
    conf->popSize = 500;
    conf->elitism = 0.1;
    conf->crossoverRate = 0.9;
    conf->mutationRate = 0.1;

    conf->NUM_THREADS = 4;

    /// Loading database and grammar

//    data = new Database("input/data/edo.dat");
//    data->loadGroup("input/data/edo1.dat");
//    grammar = new Grammar("input/grammar/edogram.txt");

//    data = new Database("input/data/rk4.dat");
//    data->loadGroup("input/data/rk4.grp");
//    grammar = new Grammar("input/grammar/rk4.dat");

    data = new Database("input/data/ita.dat");
    data->loadGroup("input/data/ita.grp");
    grammar = new Grammar("input/grammar/ita.dat");

//    data = new Database("input/data/teste.dat");
//    data->loadGroup("input/data/teste.grp");
//    grammar = new Grammar("input/grammar/teste.dat");

//    data = new Database("input/data/ita.dat");
//    data->loadGroup("input/data/ita.grp");
//    grammar = new Grammar("input/grammar/ita.dat");

//    data = new Database("input/data/cos.dat");
//    data->loadGroup("input/data/cos.grp");
//    grammar = new Grammar("input/grammar/senx.dat");

//    data = new Database("input/data/ls.dat");
//    data->loadGroup("input/data/ls.grp");
//    grammar = new Grammar("input/grammar/ls.dat");

    conf->numTree = data->prediction; // seta o numero de variaveis a serem preditas. dependente do problema a ser tratado

    grammar->print();
    cin.get();
   // data->print();

    double** dados_treino = data->values;
    cout << "total training " << data->totalTraining << " total test " << data->totalTest << " total validation "<< data->totalValidation << endl;

    /// Setting parser

    #ifdef simpleParser
      SimpleParser * parser = new SimpleParser();
    #endif
    #ifdef parserGenericParser
        parser = new GenericParser();
    #endif
    #ifdef parserLeastSquare
        parser = new LeastSquareParser();
    #endif
    #ifdef parserDerivadas
        parser = new ParserDerivadas();
    #endif
    #ifdef parserLSDerivadas
        parser = new LSDerivadasParser();
    #endif
    #ifdef parserRungeKutta
         parser = new RK4Parser();
    #endif

    parser->setDataSet(dados_treino,data->totalTraining);

    Search* s = new Search(parser,NULL,NULL);
    s->stepByStep = true;
    s->evolve();


    return 0;
}