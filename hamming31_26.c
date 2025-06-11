/*Implementações*/

#include "hamming31_26.h"

//Alocar memória para a struct no código principal
//linha[27] == 26 bits + \0
void marcaPosicoes(unsigned char linha[27], struct Hamming *hamming)
{
	int j = 1;

	for (int i = 0; i < 26; )
	{
		//Se a posição for potência de 2, avança para a próxima
		if ((j & (j - 1)) == 0)
		{
			hamming->codigo[j] = 0;
			j++;
			continue;	//não avança i
		}
		
		hamming->codigo[j] = (linha[i] == '1') ? 1:0;
		j++;
		i++;	//avança i somente depois de inserir dados
	}
}

//Calcula a paridade
void codificaHamming(FILE *arquivo, struct Hamming *hamming)
{
	hamming->codigo[1] = 0;
	hamming->codigo[2] = 0;
	hamming->codigo[4] = 0;
	hamming->codigo[8] = 0;
	hamming->codigo[16] = 0;

	for (int i = 1; i < 32; i++)
	{
		if (i == 1 || i == 2 || i == 4 || i == 8 || i == 16)
			continue;

		//Bit de paridade 1
		if (i & 1)
			hamming->codigo[1] ^= hamming->codigo[i];

		//Bit de paridade 2
		if (i & 2)
			hamming->codigo[2] ^= hamming->codigo[i];

		//Bit de paridade 4
		if (i & 4)
			hamming->codigo[4] ^= hamming->codigo[i];

		//Bit de paridade 8
		if (i & 8)
			hamming->codigo[8] ^= hamming->codigo[i];
		
		//Bit de paridade 16
		if (i & 16)
			hamming->codigo[16] ^= hamming->codigo[i];
	}

	//Escreve no arquivo intermediário
	for (int i = 1; i < 32; i++)
	{
		printf("%c", hamming->codigo[i]);
		fputc(hamming->codigo[i] ? '1' : '0', arquivo);;
	}

	//Coloca o \n entre os conjuntos
	fputc('\n', arquivo);
}

void traduz(FILE *arquivo, FILE *destino, unsigned int total_bits) {
    unsigned char byte = 0;
    int count = 0;
    int bit_char;
	int bit_count = 0;

	printf("função de traduzir o arquivo\n");
    rewind(arquivo); // Garante que estamos no início do arquivo

    while (((bit_char = fgetc(arquivo)) != EOF) && (bit_count <= total_bits))
	{
        if (bit_char != '0' && bit_char != '1') 
		{
            continue; // Ignora caracteres inválidos
        }

        byte <<= 1;
        byte |= (bit_char - '0');
        count++;
		bit_count++;

        if (count == 8) {
            fputc(byte, destino);
            byte = 0;
            count = 0;
        }
    }

    // Se sobraram bits no final
    if (count > 0) {
        byte <<= (8 - count);
        fputc(byte, destino);
    }
}

void decodificaHamming(FILE *fonte, FILE *temporario, unsigned char linha_codificada[32], struct Hamming *h)
{
    unsigned char linha[32] = {0};

    // Remove o '\n' do final da linha recebida
    linha_codificada[strcspn(linha_codificada, "\n")] = '\0';

    // Passa a linha codificada para uma linha auxiliar indexada em 1
    for (int i = 0; i < 31; i++) {
        linha[i + 1] = (linha_codificada[i] == '1') ? 1 : 0;
    }

    // Recalcula os bits de paridade
    h->codigo[1] = 0;
    h->codigo[2] = 0;
    h->codigo[4] = 0;
    h->codigo[8] = 0;
    h->codigo[16] = 0;

    for (int i = 1; i < 32; i++) {
        if (i & 1) h->codigo[1] ^= linha[i];
        if (i & 2) h->codigo[2] ^= linha[i];
        if (i & 4) h->codigo[4] ^= linha[i];
        if (i & 8) h->codigo[8] ^= linha[i];
        if (i & 16) h->codigo[16] ^= linha[i];
    }

    // Verifica e corrige erros
    if (!(linha[1] == h->codigo[1] && linha[2] == h->codigo[2] &&
          linha[4] == h->codigo[4] && linha[8] == h->codigo[8] && linha[16] == h->codigo[16])) {
        int erro = h->codigo[1] + (h->codigo[2] << 1) + (h->codigo[4] << 2) + (h->codigo[8] << 3) + (h->codigo[16] << 4);

        if (erro > 0 && erro < 32) {
            printf("Corrigindo erro na linha [%d]...\n", erro);
            linha[erro] ^= 1;
        }
    }

    // Escreve apenas os bits de dados no arquivo temporário (ignorando bits de paridade)
    for (int i = 1; i < 32; i++) {
        if (i != 1 && i != 2 && i != 4 && i != 8 && i != 16) {
            fputc(linha[i] ? '1' : '0', temporario);
        }
    }
}
