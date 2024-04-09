typedef struct Transacao {
    char data[20];
    char tipo; // 'C' para crédito, 'D' para débito, 'P' para PIX
    float valor;
} Transacao;

typedef struct Conta {
    int numero;
    char nome_titular[50];
    char cpf_titular[15];
    float saldo;
    Transacao* extrato;
    int num_transacoes; 
} Conta;

typedef struct Banco {
    Conta** contas;
    int num_contas;
} Banco;
 

void getCurrentDate(char* buffer);
void depositar(Conta* conta);
void sacar(Conta* conta);
void realizarPix(Conta* conta);
void mostrarDados(Conta* conta);
Conta* abrirConta();


