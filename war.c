#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TERRITORIOS 5

// ====================== STRUCT ======================
typedef struct {
    char nome[50];
    char cor[20];
    int tropas;
} Territorio;

// ====================== PROTÓTIPOS ======================
void inicializarTerritorios(Territorio *mapa, int n);
void exibirMapa(const Territorio *mapa, int n);
void simularBatalha(Territorio *mapa, int n);
void verificarMissao(const Territorio *mapa, int n, const char *missao, int *jogoAtivo);
void menuPrincipal(Territorio *mapa, int n, const char *missao);
void limparBuffer();

// ====================== MAIN ======================
int main() {
    srand(time(NULL));

    int n = MAX_TERRITORIOS;
    Territorio *mapa = (Territorio *)calloc(n, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    printf("=== WAR ESTRUTURADO - NÍVEL MESTRE ===\n\n");

    inicializarTerritorios(mapa, n);

    // Missões disponíveis
    const char *missoes[] = {
        "Destruir o exército Verde",
        "Conquistar 3 territórios"
    };
    const char *missao = missoes[rand() % 2];

    printf("\nMissão atribuída: %s\n", missao);
    printf("\nPressione ENTER para iniciar o jogo...\n");
    getchar();

    menuPrincipal(mapa, n, missao);

    free(mapa);
    return 0;
}

// ====================== FUNÇÕES ======================

// Inicializa territórios com valores automáticos
void inicializarTerritorios(Territorio *mapa, int n) {
    const char *nomes[] = {"Brasil", "Argentina", "Chile", "Peru", "México"};
    const char *cores[] = {"Azul", "Vermelho", "Verde", "Amarelo", "Roxo"};

    for (int i = 0; i < n; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[i]);
        mapa[i].tropas = rand() % 6 + 3; // Tropas entre 3 e 8
    }

    printf("=== Territórios Iniciais ===\n");
    exibirMapa(mapa, n);
}

// Exibe o estado atual do mapa
void exibirMapa(const Territorio *mapa, int n) {
    printf("\n--- ESTADO DO MAPA ---\n");
    for (int i = 0; i < n; i++) {
        printf("%d - %-10s | Cor: %-10s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula uma batalha entre dois territórios
void simularBatalha(Territorio *mapa, int n) {
    int atacante, defensor;

    printf("\n=== Fase de Ataque ===\n");
    exibirMapa(mapa, n);

    printf("\nEscolha o território atacante (1-%d): ", n);
    scanf("%d", &atacante);
    printf("Escolha o território defensor (1-%d): ", n);
    scanf("%d", &defensor);
    limparBuffer();

    atacante--;
    defensor--;

    if (atacante < 0 || atacante >= n || defensor < 0 || defensor >= n || atacante == defensor) {
        printf("Escolha inválida!\n");
        return;
    }

    if (mapa[atacante].tropas <= 1) {
        printf("O território atacante não possui tropas suficientes!\n");
        return;
    }

    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    printf("\n%s (Atacante) rolou %d\n", mapa[atacante].nome, dadoAtaque);
    printf("%s (Defensor) rolou %d\n", mapa[defensor].nome, dadoDefesa);

    if (dadoAtaque >= dadoDefesa) {
        mapa[defensor].tropas--;
        printf("Atacante venceu! %s perde 1 tropa.\n", mapa[defensor].nome);
        if (mapa[defensor].tropas <= 0) {
            printf("Território %s foi conquistado por %s!\n",
                   mapa[defensor].nome, mapa[atacante].cor);
            strcpy(mapa[defensor].cor, mapa[atacante].cor);
            mapa[defensor].tropas = 1;
            mapa[atacante].tropas--;
        }
    } else {
        printf("Defensor resistiu! Nenhuma mudança de território.\n");
    }

    exibirMapa(mapa, n);
}

// Verifica se a missão foi cumprida
void verificarMissao(const Territorio *mapa, int n, const char *missao, int *jogoAtivo) {
    int territoriosConquistados = 0;
    int verdeExiste = 0;

    for (int i = 0; i < n; i++) {
        if (strcmp(mapa[i].cor, "Verde") == 0) verdeExiste = 1;
        if (strcmp(mapa[i].cor, mapa[0].cor) == 0) territoriosConquistados++;
    }

    printf("\nMissão atual: %s\n", missao);

    if (strcmp(missao, "Destruir o exército Verde") == 0) {
        if (!verdeExiste) {
            printf("✅ Missão cumprida! Você destruiu o exército Verde!\n");
            *jogoAtivo = 0;
        } else {
            printf("❌ O exército Verde ainda resiste!\n");
        }
    } else if (strcmp(missao, "Conquistar 3 territórios") == 0) {
        if (territoriosConquistados >= 3) {
            printf("✅ Missão cumprida! Você conquistou 3 ou mais territórios!\n");
            *jogoAtivo = 0;
        } else {
            printf("❌ Ainda não conquistou territórios suficientes.\n");
        }
    }
}

// Menu principal e controle do jogo
void menuPrincipal(Territorio *mapa, int n, const char *missao) {
    int opcao;
    int jogoAtivo = 1;

    while (jogoAtivo) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missão\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                simularBatalha(mapa, n);
                break;
            case 2:
                verificarMissao(mapa, n, missao, &jogoAtivo);
                break;
            case 0:
                printf("Saindo do jogo...\n");
                jogoAtivo = 0;
                break;
            default:
                printf("Opção inválida!\n");
        }
    }
}

// Limpa buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
