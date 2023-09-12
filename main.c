/* Escolhas feitas:
 *
 * O programa está dividido em 3 ficheiros: main.c, funcoes.c e header.h .
 * No main.c ocorre toda a interação com o utilizador, onde são imprimidos no ecrã os comandos disponíveis, no ficheiro
 * funcoes.c ocorre o processamento e análise dos dados de input e geração do output, no header.h estão apenas as estruturas
 * das listas ligadas e as estruturas necessárias para armazenar os dados.
 *
 * Para criar o ficheiro binário de input, o utilizador deve introduzir o nome dum ficheiro de texto, depois o programa
 * converte-o para binário, fazendo depois a verificação da integridade dos dados nele contidos. Fanzendo também as
 * devidas verificações de existência do ficheiro de input e se está vazio ou não
 *
 * A estrutura dos ficheiro inicial é a seguinte:
 *
 * alimentação 200
 * transportes 100
 * vestuário 50
 * treinos 50
 * passeios 50
 * hobies 50
 * +
 * (descrição-valor-tipo de despesa)
 * (descrição-valor-tipo de despesa)
 * (descrição-valor-tipo de despesa)
 * +
 * (descrição-valor-tipo de despesa)
 * (descrição-valor-tipo de despesa)
 * (descrição-valor-tipo de despesa)
 * +
 * .
 * .
 * +
 *
 * Como se vê no inicio o ficheiro tem todos os tipos de despesa com o respectivo orçamento mensal à frente, criei 6 tipos
 * de despesas diferentes.
 * Depois uso o (+) para separar os dados de cada mês, bem como os tipos de despesa, do resto dos dados. Ou seja no
 * processamento, uso o (+) como contador de meses, conseguindo assim obter a informação sobre cada mês. O ficheiro
 * termina por isso com o caracter + .
 *
 * No processamento (comando 1), é chamda a função (get_bin_data) que recebe como parâmetro o nome do ficheiro de input
 * verifica se o ficheiro existe e não está vazio, se sim lê o ficheiro desde o inicio até ao fim guardando na string
 * (output_bin) esses dados.
 *
 * Depois ainda nesta função essa string é percorrida, fazendo a verifiação de que não há caracteres especiais, e se os
 * dados estão com a mesma estrutura que mencionei acima, caso contrário também não faz o processamento, devolvendo uma
 * mensagem de erro distinta para cada um dos casos, dizendo qual o problema detectado.
 *
 * No entanto se não detetar erros, é chamada, dentro da função (get_bin_data), a função (create_linked_lists), que recebe
 * a string (output_bin) e vai percorrê-la partindo em tokens, de forma a criar as duas listagens pedidas no enunciado.
 *
 * 1ª listagem tem todos os dados relativos a cada tipo de despesa, com o seguinte esqueleto, tem apenas (6 nós):
 *
 * cabeçalho ->  nó    ->     nó   ->    nó   ->    nó   ->    nó   ->    nó
 *               |            |          |          |          |          |
 *            expenses    expenses   expenses   expenses   expenses   expenses
 *
 * A estrutura expenses contém:
 *  - o tipo de despesa;
 *  - o gasto anual;
 *  - o orçamento mensal;
 *  - um array de inteiros em que cada posição corresponde a um mês do ano, e o respetivo valor é o gasto total nesse mês;
 *  - uma matriz com todas as descrições onde as linhas da matriz correspondem ao mês, e o conteúdo da linha contém
 *    as descrições de cada despesa nesse mês;
 *
 * 2ª listagem tem os dados relativos aos orçamentos cujo valor ultrapassou o orçamento total em mais de 10%, o esqueleto é:
 *
 * cabeçalho ->  nó    ->    nó   ->    nó   ->    nó   ->    nó   ->    nó   ->    nó   ->  ...
                 |           |          |          |          |          |          |
 *            overflow   overflow   overflow   overflow   overflow   overflow   overflow
 *
 * A estrutura overflow contém:
 *  - o tipo da despesa;
 *  - o orçamento mensal;
 *  - o orçamento total até ao mês em causa;
 *  - o valor do excedente;
 *  - o mês;
 *
 *  Como é perceptível, as estrututras dos nós de ambas as listas apenas têm um ponteiro para o nó seguinte, e outro
 *  ponteiro para a respectiva estrutura com os dados em causa, sendo construidas dinâmicamente.
 *
 *  Após criadas ambas as listas, dentro da função (create_linked_lists), é chamada a função (save_to_bin), que não
 *  recebe parametro nenhum, porque os ponteiros para os cabeçalhos de ambas as listas estão declarados estáticamente
 *  no ficheiro (funcoes.c). Esta função vai guardar a informação das listas em 2 ficheiros binários separados.
 *
 *  Primeiro a função vai ao ficheiro de texto (config.txt) buscar os nomes dos ficheiro binários para o qual irá grava os
 *  dados, esses nomes são:
 *  expenseList
 *  overFlowList
 *
 *  De seguida começa então a percorrer a lista de dados, gravando no ficheiro binário com nome (expenseList), préviamente
 *  criado através do comando fopen(), os elementos da estrutura (expenses), quando terminar faz o mesmo para a segunda lista
 *  mas agora, no ficheiro binário com o nome (overFlowList).
 *
 *  Quando terminar, a função (save_to_bin) chama a função (delete_lists), que tal como o nome indica, serve para eliminar
 *  as listas ligadas, libertando essa memória, pois os seus dados já estão gravados noutro sítio.
 *
 *  Com isto o programa volta para o main, imprimindo uma mensagem dizendo que os dados foram processados e gravados com sucesso.
 *  Ficando de seguida à espera do próximo comando.
 *
 *  O utilizador tem então mais 2 comandos disponíveis, o comando (3) que serve para sair do programa, e o comando (2),
 *  que serve para imprimir os dados gravados.
 *
 *  Após selecionar este comando, é mostrado na consola um sub menu, com mais 4 comandos possíveis:
 *  - (1) - imprime apenas a lista de despesas com todos os dados já mencinados; (extra)
 *  - (2) - imprime apenas lista de orçamentos excedentes; (tal como é pedido no enunciado)
 *  - (3) - imprime ambas as listas;
 *  - (4) - imprime apenas o valor total da despesa em cada tipo (tal como é pedido no enunciado);
 *
 *  Para fazer estas impressões, é chamada função (print_lists) que recebe como parâmetro o comando, acede depois ao ficheiro
 *  config.txt para saber qual o ficheiro binário a abrir, e depois disso, percorre então o respectivo ficheiro, imprimindo
 *  as informações pedidas.
 *
 *  Se o utilizador tentar imprimir quaisquer dados sem ter primeiro, inserido um input, o programa avisa que tal não
 *  foi feito préviamente.
 *
 *  Todos os erros que ocorrem devolve um valor específico ao main, que identifca o erro, imprimindo uma mensagem específica
 *  na consola, dizendo qual o erro, a possível causa, e como corrigir.
*/

#include "funcoes.c"

int main() {
    printf("------------------------------INÍCIO------------------------------\n\n");
    char comando, inputFile[STRINGSIZE], *message;
    int c;

    // interação com o utilizador
    printf("Comandos:\n1 - Analisar despesas;\n2 - Imprimir resultados da análise;\n3 - Sair;\n\nComando: ");
    while (1) {

        scanf("%1c", &comando);

        // fazer o processmaneto dos dados
        if (comando == '1') {

            // limpar o input e receber o nome do ficheiro binário
            printf("Inserir nome do ficheiro a analisar: ");
            while ((c = getchar()) != '\n' && c != EOF);
            scanf("%20s", inputFile);
            message = get_bin_data(inputFile);

            if(strcmp(message,"1") == 0)
                printf("Erro: O ficheiro introduzido não existe!\n");

            else if (strcmp(message,"2") == 0)
                printf("Aviso: ficheiro está vazio!\n");

            else if(strcmp(message,"3") == 0)
                printf("Erro ao carregar o ficheiro, dados incorretos!\nAviso - dados têm que estar no seguinte formato:\n\n\t*Tipo de despesa* *orçamento*\n\t...\n\t*Tipo de despesa* *orçamento*\n\t+\n\t*Descrição da despesa-valor-tipo de despesa*\n\t+\n\t...\n\t+\n\t*Descrição da despesa-valor-tipo de despesa*\n\t+\n\nO (+) é o separador de meses, sendo que têm de haver 13 (+), e os dados no ficheiro terminam com (+).\nNão pode conter caracteres especiais.\nAs linhas têm de ter no máximo 100 caracteres.\n");

            else
                printf("Dados carregados e processados com sucesso!\n");

            while ((c = getchar()) != '\n' && c != EOF);
        }

        // para imprimir as listas ligadas
        else if (comando == '2') {

            while ((c = getchar()) != '\n' && c != EOF);
            printf("\n\tComandos:\n\t1 - Ver lista de despesas completa;\n\t2 - Ver lista de orçamentos excedentes;\n\t3 - Ver ambas as listagens;\n\t4 - Listar o valor total da despesa em cada tipo;\n\n\tComando: ");
            scanf("%1c", &comando);
            message = print_lists(comando);

            if(strcmp(message,"-1") == 0) printf("\tErro: nenhum ficheiro introduzido!\n");

            else if(strcmp(message,"-2") == 0) printf("\tErro: comando inválido!\n");

            while ((c = getchar()) != '\n' && c != EOF);
        }

        // para sair do programa
        else if (comando == '3') break;

        // caso o comando seja inválido limpar o stdin
        else {
            printf("Comando inválido!\n");
            while ((c = getchar()) != '\n' && c != EOF);
        }

        printf("\nPróximo comando: ");
    }
    printf("-------------------------------FIM-------------------------------");
    return 0;
}