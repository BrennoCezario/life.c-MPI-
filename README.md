# Jogo da Vida Paralelizado

Esta é uma implementação em C do Jogo da Vida, um autômato celular, paralelizado usando MPI. O Jogo da Vida é uma simulação onde células vivas e mortas evoluem baseadas em regras simples de vizinhança. Nesta implementação, é possível configurar o tamanho do tabuleiro e o número de passos através de um arquivo de entrada.

## Como usar

1. Compile o código utilizando um compilador C compatível com MPI.
2. Execute o programa fornecendo um arquivo de configuração com o tamanho do tabuleiro e o número de passos.
3. O programa exibirá a evolução do tabuleiro em cada passo, mostrando células vivas como 'x' e células mortas como espaços.

# Detalhes do projeto
O projeto consiste em uma implementação do Jogo da Vida em paralelo utilizando MPI. O Jogo da Vida é um autômato celular criado pelo matemático britânico John Horton Conway em 1970. Ele é um exemplo clássico de um sistema dinâmico que exibe comportamentos complexos a partir de regras simples.

O código é capaz de ler um arquivo de entrada que representa o estado inicial de um tabuleiro para o Jogo da Vida. Existem dois arquivos de entrada fornecidos: life.in e judge.in.

Após a leitura dos arquivos de entrada e a configuração inicial do tabuleiro, o programa executa o Jogo da Vida conforme as regras estabelecidas. A cada passo, o estado atual do tabuleiro é exibido na tela, mostrando as células vivas como 'x' e as células mortas como espaços em branco.

Ao final da execução, o programa libera os recursos alocados e encerra sua execução de forma ordenada.

Este projeto demonstra o uso de programação paralela com MPI para otimizar a execução de algoritmos intensivos em computação, como o Jogo da Vida, em sistemas distribuídos ou com múltiplos núcleos de processamento.

