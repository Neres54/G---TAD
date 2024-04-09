#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Banco.h"


void getCurrentDate(char* buffer) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", tm_info);
}

Conta* abrirConta() {
    Conta* nova_conta = (Conta*)malloc(sizeof(Conta));

    printf("Digite o número da conta: ");
    scanf("%d", &nova_conta->numero);
    printf("Digite o nome do titular: ");
    scanf("%s", nova_conta->nome_titular);
    printf("Digite o CPF do titular: ");
    scanf("%s", nova_conta->cpf_titular);
    nova_conta->saldo = 0.0;
    nova_conta->extrato = NULL; 
    nova_conta->num_transacoes = 0;

    printf("\nConta aberta com sucesso!\n");

    return nova_conta;
}

void depositar(Conta* conta) {
    float valor;
    printf("Digite o valor a ser depositado: ");
    scanf("%f", &valor);
    conta->saldo += valor;

    // Atualiza o extrato
    Transacao nova_transacao;
    getCurrentDate(nova_transacao.data);
    nova_transacao.tipo = 'C';
    nova_transacao.valor = valor;
    conta->extrato = (Transacao*)realloc(conta->extrato, (conta->num_transacoes + 1) * sizeof(Transacao));
    conta->extrato[conta->num_transacoes++] = nova_transacao;

    printf("Depósito de %.2f realizado com sucesso!\n", valor);
}

void sacar(Conta* conta) {
    float valor;
    printf("Digite o valor a ser sacado: ");
    scanf("%f", &valor);
    if (valor > conta->saldo) {
        printf("Saldo insuficiente para realizar o saque.\n");
    } else {
        conta->saldo -= valor;

        // Atualiza o extrato
        Transacao nova_transacao;
        getCurrentDate(nova_transacao.data);
        nova_transacao.tipo = 'D';
        nova_transacao.valor = valor;
        conta->extrato = (Transacao*)realloc(conta->extrato, (conta->num_transacoes + 1) * sizeof(Transacao));
        conta->extrato[conta->num_transacoes++] = nova_transacao;

        printf("Saque de %.2f realizado com sucesso!\n", valor);
    }
}

void realizarPix(Conta* conta) {
    float valor;
    char cpf_destino[15];
    printf("Digite o CPF do destinatário: ");
    scanf("%s", cpf_destino);
    printf("Digite o valor a ser transferido: ");
    scanf("%f", &valor);
    if (valor > conta->saldo) {
        printf("Saldo insuficiente para realizar o PIX.\n");
    } else {
        conta->saldo -= valor;

        
        Transacao nova_transacao;
        getCurrentDate(nova_transacao.data);
        nova_transacao.tipo = 'P'; // PIX
        nova_transacao.valor = valor;
        conta->extrato = (Transacao*)realloc(conta->extrato, (conta->num_transacoes + 1) * sizeof(Transacao));
        conta->extrato[conta->num_transacoes++] = nova_transacao;

        printf("PIX de %.2f para CPF %s realizado com sucesso!\n", valor, cpf_destino);
    }
}

void mostrarDados(Conta* conta) {
    printf("\nDados da conta:\n");
    printf("Número da conta: %d\n", conta->numero);
    printf("Nome do titular: %s\n", conta->nome_titular);
    printf("CPF do titular: %s\n", conta->cpf_titular);

   
    printf("\nExtrato Bancário:\n");
    printf("Data\t\tTipo\tValor\tSaldo\n");
    float saldo_atual = conta->saldo;
    for (int i = 0; i < conta->num_transacoes; ++i) {
        saldo_atual += (conta->extrato[i].tipo == 'C' ? conta->extrato[i].valor : -conta->extrato[i].valor);
        printf("%s\t%c\t%.2f\t%.2f\n", conta->extrato[i].data, conta->extrato[i].tipo, conta->extrato[i].valor, saldo_atual);
    }
}

int main() {
    Banco banco;
    banco.contas = NULL;
    banco.num_contas = 0;

    int opcao = -1;
    while (opcao != 0) {
        printf("\nMenu:\n");
        printf("1. Abrir Conta\n");
        printf("2. Depositar\n");
        printf("3. Sacar\n");
        printf("4. Realizar PIX\n");
        printf("5. Mostrar Dados de Todas as Contas\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Conta* nova_conta = abrirConta();
            banco.contas = (Conta**)realloc(banco.contas, (banco.num_contas + 1) * sizeof(Conta*));
            banco.contas[banco.num_contas++] = nova_conta;
        } else if (opcao == 2 || opcao == 3 || opcao == 4) {
            if (banco.num_contas == 0) {
                printf("Não há contas bancárias para realizar a operação.\n");
            } else {
                int num_conta;
                printf("Digite o número da conta: ");
                scanf("%d", &num_conta);
                int encontrada = 0;
                for (int i = 0; i < banco.num_contas; i++) {
                    if (banco.contas[i]->numero == num_conta) {
                        encontrada = 1;
                        if (opcao == 2)
                            depositar(banco.contas[i]);
                        else if (opcao == 3)
                            sacar(banco.contas[i]);
                        else if (opcao == 4)
                            realizarPix(banco.contas[i]);
                        break;
                    }
                }
                if (!encontrada)
                    printf("Conta não encontrada.\n");
            }
        } else if (opcao == 5) {
            if (banco.num_contas == 0) {
                printf("Não há contas bancárias para mostrar.\n");
            } else {
                printf("\n=== Dados de Todas as Contas ===\n");
                for (int i = 0; i < banco.num_contas; i++) {
                    printf("\nConta %d:\n", i + 1);
                    mostrarDados(banco.contas[i]);
                }
            }
        } else if (opcao != 0) {
            printf("Opção inválida!\n");
        }
    }

    for (int i = 0; i < banco.num_contas; i++) {
        free(banco.contas[i]->extrato);
        free(banco.contas[i]);
    }
    free(banco.contas);

    return 0;
}


