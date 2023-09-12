#ifndef PROJECTO_HEADER_H
#define PROJECTO_HEADER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define STRINGSIZE 100

// estrutura do nó da lista de despesas
typedef struct expenses_node * expenses_ptr;
struct expenses_node {
    struct expenses * data;
    struct expenses_node * next;
};

// estrutura dos dados
struct expenses {
    char type[STRINGSIZE];               // tipo de despesa
    int budget;                          // orçamento mensal
    int anual_expense;                   // gasto anual
    int month_expense[12];               // array com o gasto total de cada mes
    char descriptions[12][STRINGSIZE*10];// matriz com as descriçoes das despesas de cada mes
};

// estrutura do nó da lista de despesas que ultrapassam em 10% o orçamento
typedef struct expense_overflow * overFlow;
struct expense_overflow {
    struct overflow * data;
    struct expense_overflow * next;
};

// estrutura com os dados de despesas que ultrapassam em 10% o orçamento
struct overflow {
    char type[STRINGSIZE];               // tipo de despesa
    int budget;                          // orçamento mensal
    int budget_sum;                      // orçamento total até ao mês em causa
    int overflow;                        // desvio global
    int mes;                             // mês
};

#endif //PROJECTO_HEADER_H