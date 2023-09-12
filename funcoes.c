#include "header.h"

// função que cria o cabeçalho da lista de despesas
expenses_ptr expenses_header() {
    expenses_ptr aux = NULL;
    aux = (expenses_ptr) malloc (sizeof(struct expenses_node));
    if (aux)
        aux->next = NULL;
    return aux;
}

// função que cria o cabeçalho da lista de excedentes
overFlow overFlow_header() {
    overFlow aux = NULL;
    aux = (overFlow) malloc (sizeof(struct expense_overflow));
    if (aux) {
        aux->data = (struct overflow *) malloc(sizeof(struct overflow));
        aux->data->overflow = 0;
        aux->next = NULL;
    }
    return aux;
}

// função que cria um novo nó da lista de despesas
expenses_ptr new_month_node(expenses_ptr prev_node, char type[]) {
    expenses_ptr no, prev;
    prev = prev_node;

    int i;
    no = (expenses_ptr) malloc (sizeof(struct expenses_node));
    if (no) {
        no->data = (struct expenses *) malloc (sizeof(struct expenses));
        strcpy(no->data->type,type);
        no->data->budget = 0;
        no->data->anual_expense = 0;
        for(i = 0; i < 12; ++i) {
            no->data->month_expense[i] = 0;
            strcpy(no->data->descriptions[i],"");
        }
        no->next = prev->next;
        prev->next = no;
    }
    return no;
}

// função que cria um novo nó da lista de excedentes
overFlow new_overFlow_node(overFlow prev_node, char type[], int budget, int budget_sum, int value, int mes) {
    overFlow no, prev;
    prev = prev_node;

    no = (overFlow) malloc (sizeof(struct expense_overflow));
    if (no) {
        no->data = (struct overflow *) malloc (sizeof(struct overflow));
        strcpy(no->data->type,type);
        no->data->budget = budget;
        no->data->budget_sum = budget_sum;
        no->data->overflow = value;
        no->data->mes = mes;
        no->next = prev->next;
        prev->next = no;
    }
    return no;
}

// ponteiro para o cabeçalho da lista de despesas
static expenses_ptr expenses_list_header;

// ponteiro para o cabeçalho da lista de escedentes
static overFlow overFlow_list_header;

void delete_lists() {

    // inicializar ponteiros auxiliares para fazer a eliminação das listas
    expenses_ptr expense_aux = NULL;
    overFlow overFlow_aux = NULL;

    while (expenses_list_header->next) {
        expenses_list_header->data = NULL;
        expense_aux = expenses_list_header->next;
        expenses_list_header->next = expense_aux->next;
        free(expense_aux);
    }

    while (overFlow_list_header->next){
        overFlow_list_header->data = NULL;
        overFlow_aux = overFlow_list_header->next;
        overFlow_list_header->next = overFlow_aux->next;
        free(overFlow_aux);
    }

    expenses_list_header = NULL;
    overFlow_list_header = NULL;
}

void save_to_bin() {

    // inicializar ponteiros auxiliares
    expenses_ptr expense_data = expenses_list_header;
    overFlow overFlow_data = overFlow_list_header;

    // ir ao ficheiro config.txt buscar o nome para o ficheiro binário onde se guardarão as listas
    FILE * configFile = fopen("config.txt", "r");
    char expenseList[STRINGSIZE] = "", overFlowList[STRINGSIZE] = "";
    int i;
    fgets(expenseList, STRINGSIZE, configFile);
    fgets(overFlowList, STRINGSIZE, configFile);
    fclose(configFile);
    expenseList[ strlen(expenseList)-2 ] = '\0';
    overFlowList[ strlen(overFlowList)-2 ] = '\0';

    // percorrer as lista com os dados das despesas gravando a informação no ficheiro binário
    FILE * output_Bin = fopen (expenseList, "wb");
    expense_data = expenses_list_header->next;
    while (expense_data) {

        fwrite( &(expense_data->data->type), sizeof(expense_data->data->type), 1, output_Bin);
        fwrite( &(expense_data->data->budget), sizeof(expense_data->data->budget), 1, output_Bin);
        fwrite( &(expense_data->data->anual_expense), sizeof(expense_data->data->anual_expense), 1, output_Bin);
        for(i = 0; i < 12; ++i)
            fwrite( &(expense_data->data->month_expense[i]), sizeof(expense_data->data->month_expense[i]), 1, output_Bin);
        for(i = 0; i < 12; ++i)
            fwrite( &(expense_data->data->descriptions[i]), sizeof(expense_data->data->descriptions[i]), 1, output_Bin);

        expense_data = expense_data->next;
    }
    fclose(output_Bin);

    // percorrer as lista com os dados dos execedentes gravando a informação no ficheiro binário
    output_Bin = fopen (overFlowList, "wb");
    overFlow_data = overFlow_list_header->next;
    while (overFlow_data){

        fwrite(&overFlow_data->data->type, sizeof(overFlow_data->data->type), 1, output_Bin);
        fwrite(&overFlow_data->data->budget, sizeof(overFlow_data->data->budget), 1, output_Bin);
        fwrite(&overFlow_data->data->budget_sum, sizeof(overFlow_data->data->budget_sum), 1, output_Bin);
        fwrite(&overFlow_data->data->overflow, sizeof(overFlow_data->data->overflow), 1, output_Bin);
        fwrite(&overFlow_data->data->mes, sizeof(overFlow_data->data->mes), 1, output_Bin);

        overFlow_data = overFlow_data->next;
    }
    fclose(output_Bin);

    // chamar função que elimina as listas da memória
    delete_lists();
}

void create_linked_lists(char data[]) {

    expenses_list_header = expenses_header();
    expenses_ptr expense_data = expenses_list_header;

    overFlow_list_header = overFlow_header();
    overFlow overFlow_data = overFlow_list_header;

    char *temp_ptr = NULL, *description = NULL, *value = NULL, type[STRINGSIZE/2] = "";
    int month_count = 0, counter = 0;
    temp_ptr = strtok(data, " \n");
    ++counter; // variável auxiliar para controlar o conteúdo do temp_ptr

    // percorrer a string data partindo-a em tokens e guardando os dados na lista de despesas e na lista de excedentes
    while (month_count <= 12) {

        // guardar o tipo de despesa e o orçamento mensal
        if (month_count == 0) {
            if (counter % 2 != 0) expense_data = new_month_node(expense_data, temp_ptr);
            else expense_data->data->budget = (int) strtol(temp_ptr, NULL, 10);
            temp_ptr = strtok(NULL, " \n");
            ++counter;
        }

        // guardar os dados de cada mês
        else {
            expense_data = expenses_list_header->next;

            // obter os dados
            if (counter == 1) description = temp_ptr;
            else if (counter == 2) value = temp_ptr;
            else if (counter == 3) {
                strcpy(type,temp_ptr);
                type[strlen(type)-1] = '\0';

                // ir preenchendo a lista de despesas à medida que percorre a informação de cada mês
                while (expense_data) {

                    if (strcasecmp(type, expense_data->data->type) == 0) {
                        expense_data->data->anual_expense += (int) strtol(value, NULL, 10);
                        expense_data->data->month_expense[month_count-1] += (int) strtol(value, NULL, 10);
                        strcat(expense_data->data->descriptions[month_count-1], description);
                        strcat(expense_data->data->descriptions[month_count-1], ": ");
                        strcat(expense_data->data->descriptions[month_count-1], value);
                        strcat(expense_data->data->descriptions[month_count-1], "\n");
                        break;
                    }
                    expense_data = expense_data->next;
                }

                counter = 0;
                description = value = NULL;
                type[0] = '\0';
            }

            temp_ptr = strtok(NULL, "-\n");
            ++counter;
        }

        // quando apanhar o separador de meses incrementa o contador de meses e avança para o mês seguinte
        if (temp_ptr[0] == '+') {

            // após preencher os dados relativos a cada mês -> percorrer novamente a lista de despesas para:
            // varificar se a despesa anual total no mês atual, já está a ultrapassar o orçamento total até ao mês atual
            // em mais de 10%
            // se sim criar um novo nó na lista de orçamentos
            expense_data = expenses_list_header->next;
            while(expense_data) {
                if (expense_data->data->anual_expense > (expense_data->data->budget*month_count) + ((expense_data->data->budget*month_count)*0.1) ) {

                    // verificar se o excedente aumentou face ao mês anterior caso já tenha ultrapassado o orçamento anteriormente (para não haver nós repetidos)
                    if (overFlow_data->data->overflow < expense_data->data->anual_expense - expense_data->data->budget*month_count) {
                        overFlow_data = new_overFlow_node(overFlow_data, expense_data->data->type, expense_data->data->budget, expense_data->data->budget*month_count, expense_data->data->anual_expense - expense_data->data->budget*month_count, month_count);
                    }
                }
                expense_data = expense_data->next;
            }

            temp_ptr = strtok(NULL, "-\n");
            ++month_count;
            counter = 1;
        }
    }

    // por fim chamar a função que grava os dados noutro ficheiro binário
    save_to_bin();
}

char * get_bin_data(char *inputFile) {

    // abrir o ficheiro
    strcat(inputFile, ".txt");
    FILE * data = fopen(inputFile, "r");

    // se o ficheiro nao existir
    if (data == NULL) {
        fclose(data);
        return "1";
    }

    else {

        char line[STRINGSIZE] = "";
        long size = 0;
        fseek (data, 0, SEEK_END);
        size = ftell(data);

        // se o ficheiro estiver vazio
        if ( size == 0 ) {
            fclose(data);
            return "2";
        }

        // se existir e tiver dados lê-os gerar o ficheiro binário e copiar para o array output_bin os dados
        else {

            // gerar o ficheiro binário
            fseek (data, 0, SEEK_SET);
            FILE * data_bin = fopen("data", "wb");
            while ( fgets(line, STRINGSIZE, data) ) {
                fwrite(line, sizeof(line), 1, data_bin);
            }
            fclose(data);
            fclose(data_bin);

            // ler e verificar a integridade do ficheiro binário
            data_bin = fopen(inputFile, "rb");
            unsigned long i = 0;
            unsigned char u;
            fseek(data_bin, 0, SEEK_END);
            long end = ftell(data_bin);
            char output_bin[end];
            fseek(data_bin, 0 , SEEK_SET);
            fread(output_bin, end, 1, data_bin);
            end = 0;
            int control = 0;

            // verificar se há caracteres especiais, se houver devolver mensagem de erro
            while (control < 13) {

                u = output_bin[i];
                if (u == '+') ++control;  // contador de meses
                if (u >= 0xC0) {
                    bzero(output_bin,end);
                    fclose(data_bin);
                    return "3";
                }
                i++;
            }

            // se não tiverem sido contados 12 meses devolve mensagem de erro
            if (control < 12) {
                fclose(data_bin);
                return "3";
            }

            // percorrer a string output para verificar se os dados estão corretos
            for (i = 0; i < strlen(output_bin); ++i) {
                if (output_bin[i] == '-' && (output_bin[i-1] == ' ' || output_bin[i+1] == ' ')) {
                    fclose(data_bin);
                    return "3";
                }
            }

            // criar as duas listagens e voltar para o main depois
            create_linked_lists(output_bin);
        }
    }
    return "0";
}

char * print_lists(char mode) {

    // ir ao ficheiro config.txt buscar os nomes dos ficheiros binários com os nós das listas
    FILE * configFile = fopen("config.txt", "r");
    char expenseList[STRINGSIZE] = "", overFlowList[STRINGSIZE] = "";
    fgets(expenseList, STRINGSIZE, configFile);
    fgets(overFlowList, STRINGSIZE, configFile);
    fclose(configFile);

    // eliminar o \n e \r das strings
    expenseList[ strlen(expenseList)-2 ] = '\0';
    overFlowList[ strlen(overFlowList)-2 ] = '\0';

    struct expenses expense_struct;
    struct overflow overFlow_struct;

    // se o utilizador apenas quiser ver as despesas ou ambos
    if (mode == '1' || mode == '3') {
        // percorrer o ficheiro com os dados das despesas
        FILE *output_Bin = fopen(expenseList, "rb");
        if (output_Bin == NULL) return "-1";
        int j;
        printf("\n********************LISTA DE DESPESAS********************");
        while (fread(&expense_struct, sizeof(struct expenses), 1, output_Bin)) {

            printf("\nTIPO DE DESPESA: %s\n- orcamento mensal: %d\n- gasto anual: %d\n", expense_struct.type,expense_struct.budget, expense_struct.anual_expense);

            for (j = 0; j < 12; ++j)
                printf("- mes: %d\n- gasto mensal: %d\nDescrições:\n%s---------\n", j + 1,expense_struct.month_expense[j], expense_struct.descriptions[j]);
        }
        fclose(output_Bin);
    }

    // se apenas quiser ver os excedentes ou ambos
    if (mode == '2' || mode == '3') {

        // percorrer o ficheiro com os dados dos excedentes
        FILE * output_Bin = fopen(overFlowList, "rb");
        if (output_Bin == NULL) return "-1";
        printf("\n********************LISTA DE ORÇAMENTOS EXCEDENTES********************\n");
        while (fread(&overFlow_struct, sizeof(struct overflow), 1, output_Bin))
            printf("\nTipo de despesa: %s\n- orçamento mensal: %d\n- orçamento total até ao mês em causa: %d\n- o excedente total é %d no mês %d\n", overFlow_struct.type, overFlow_struct.budget, overFlow_struct.budget_sum, overFlow_struct.overflow, overFlow_struct.mes);

        fclose(output_Bin);
    }

    if (mode == '4') {
        // percorrer o ficheiro com os dados das despesas
        FILE *output_Bin = fopen(expenseList, "rb");
        if (output_Bin == NULL) return "-1";
        printf("\n********************TOTAL DA DESPESA EM CADA TIPO********************\n");
        while (fread(&expense_struct, sizeof(struct expenses), 1, output_Bin))
            printf("TIPO: %s -> gasto anual: %d\n", expense_struct.type, expense_struct.anual_expense);
        fclose(output_Bin);
    }

    if (mode != '1' && mode != '2' && mode != '3' && mode != '4') return "-2";
    return "0";
}