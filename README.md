# Codificação Código Hamming

## Enunciado:
Codificar um arquivo texto usando código Hamming (31, 26)
- O arquivo codificado deve ser gravado com o nome "nome_original.hamming"

Arquivo intermediário será alterado na apresentação. Decodificação deste arquivo deve corrigir os erros inseridos.

Arquivo intermediário deve ter um espaço entre cada conjunto codificado

Nome do arquivo de saída:  "nome_original.dec"

## ⛓︎⌨︎ Codificação Hamming (31, 26)
### Definição:
A Codificação Hamming é um corretor de erros para garantir a precisão dos dados durante a transmissão ou armazenamento. Consiste em adicionar bits extras aos dados originais, permitindo que o sistema detecte e corrija erros de bit único.

### Importante:
- `Bits Redundantes`: São bits binários extras que são gerados e adicionados aos bits portadores de informação da transferência de dados para garantir que nenhum bit seja perdido durante a operação. O número de bits redundantes pode ser calculado com:
`2^r >= m + r + 1` onde `r = número de bits redundantes` e `m = número de bits dos dados de entrada`.
- `Tipos de Bits de Paridade`: Um bit de paridade é um bit anexado a um conjunto de bits binários para garantir que o número total de 1's nos dados seja par ou ímpar.
	1. Bit de Paridade `par`: Em caso de paridade `par`, para um determinado conjunto de bits, o número de 1's é contado. Se essa contagem for ímpar, então o valor do bit de paridade é definido como 1, tornando a contagem final de bits 1's par. Se a contagem dos bits 1's nos dados já for par, então o bit de paridade assume valor 0.
	2. Bit de Paridade `impar`: Mesma ideia, porém, a contagem final dos bits 1's deve ser ímpar. Caso a soma dos bits 1's seja par, o pit de paridade assum valor 1, para que a contagem final tenha valor ímpar. Caso contrário, assume 0, pois a contagem dos bits já é ímpar.

### Algoritmo do Código de Hamming
1. Escreva as posições dos bits começado em 1, no formato binário;
2. Todas as posições dos bits que são uma potência de 2 devem ser marcadas como bits de paridade;
3. Todas as outras posições de bits são marcadas como bits de dados;
4. Cada bit de dados é incluído em um conjunto de bits de paridade, conforme determinado por sua posição do bit no formato binário;
> OBSERVAÇÃO: O bit de paridade 1 cobre todas as posições de bits cuja representação binária tem um 1 na posição menos significatica. O bit de paridade 2 cobre todas as posições de bits cuja representação binária tem um 1 na segunda posição menos significativa. A mesma lógica se repete até o quinto bit de paridade, em outras palavras, cada bit de paridade abrange todos os bits em que o AND bit a bit da posição de paridade e a posição do bit são diferentes de zero.
5. Defina um bit de paridade como 1 se o número total de unidades nas posições que ele verifica for ímpar e 0 em caso contrário.

### Detecção e Correção dos Erros
A detecção de erros considera os bits de paridade **juntamente** com os bits de dados. Caso todos os novos bits devolvidos sejam zeros, então não há erro. Quando houver erro, a verificação da paridade irá retornar o índice da posição que contém o erro, basta, então, inverter o bit indicado por ela.

### Funções
- `marcaPosicoes`: Deixa vazia as posições dos bits de redundância e preenche com os dados as demais posições
- `calculaParidade`: Conta quantos 1's existem no conjunto de bits estipulado pelo X-ésimo bit menos significativo. Retorna um arquivo .hamming
- `decodificaHamming`: Recebe um arquivo codificado .hamming e calcula a paridade levando em conta os bits de redundância. Em caso de 1 erro, corrije. Para dois ou mais erros, devolve uma mensagem: `Não foi possível detectar e corrigir os erros`. Devolve o arquivo decodificado em caso de sucesso.

### Arquivos:
- `hamming31_26.h`: Contém os protótipos das funções usadas para codificar e decodificar os arquivos.
- `hamming31_26.c`: Implementação dos protótipos de `hamming31_26.h`.
- `hamming.c`: Código principal que codifica e decodifica o arquivo.

### Posições dos Bits de Paridade

BP1: `1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31`

BP2: `2, 3, 6, 7, 10, 11, 14, 15, 18, 19, 22, 23, 26, 27, 30, 31`

BP4: `4, 5, 6, 7, 12, 13, 14, 15, 20, 21, 22, 23, 28, 29, 30, 31`

BP8: `8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 26, 27, 28, 29, 30, 31`

BP16: `16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31`
