/*Código Principal*/

#include "hamming31_26.h"

int main()
{
	char nome_arq_para_codificar[2048];
	char nome_arq_decodificar[2048];
	unsigned char linha_codificada[32];	//26 bits + \n + \0
	unsigned char linha[27];		//26 bits + '\0'


	FILE *mapaBits;
	FILE *decodificado;

	printf("Digite o nome do arquivo que deseja codificar:\n");
	scanf("%s", nome_arq_para_codificar);

	FILE *arquivo = fopen(nome_arq_para_codificar, "rb");
	if(!arquivo)
	{
		printf("Não foi possível abrir o arquivo.\n");
		return -1;
	}

	rewind(arquivo);	//vai para o início do arquivo

	struct Hamming *hamming = malloc(sizeof(struct Hamming));
	if (!hamming)
	{
		printf("Não foi possível alocar memória para o hammin.\n");
	}

	unsigned int total_bits = 0;		//total de bits escritos 

	/*CRIA O ARQUIVO INTERMEDIARIO E CALCULA A PARIDADE ------------------------------*/
	mapaBits = fopen("intermediario.txt", "a");
	if (!mapaBits)
	{
		printf("Erro ao abrir arquivo intermediário.\n");
		fclose(arquivo);
		free(hamming);
		return -1;
	}

	unsigned char byte;
    int bit_index = 0;

	unsigned int indice1 = 1;
    while(fread(&byte, 1, 1, arquivo) == 1)
    {
        for (int i = 7; i >= 0; i--)
        {
            linha[bit_index] = (byte & (1 << i)) ? '1': '0';
            bit_index++;

            if(bit_index == 26)
            {
                linha[26] = '\0';
				marcaPosicoes(linha, hamming);
				codificaHamming(mapaBits, hamming);
				bit_index = 0;
				total_bits += 26;
				indice1 += 31;
            }
        }

    }

	//Se sobraram bits no final
	if (bit_index > 0)
	{
		// Marca o final dos dados reais
		for (int i = bit_index + 1; i < 26; i++)
			linha[i] = '0';
	    linha[26] = '\0';
		marcaPosicoes(linha, hamming);
	    codificaHamming(mapaBits, hamming);
	}

	indice1 += bit_index;
	total_bits += bit_index;
	fclose(mapaBits);
	/*--------------------------------------------------------------------------------*/

	/*DECODIFICA O ARQUIVO RECEBIDO --------------------------------------------------*/
	printf("Digite o nome do arquivo que deseja decodificar:\n");
	scanf("%s", nome_arq_decodificar);

	FILE *temporario = fopen("temporario.txt", "a");
	if (!temporario)
	{
		printf("Não foi possível abrir arquivo temporário.\n");
		return -1;
	}


	//é o arquivo intermediário
	FILE *decodificar = fopen(nome_arq_decodificar, "rb");
	if (!decodificar)
	{
		printf("Erro ao tentar abrir o arquivo para decodificação.\n");
		return -1;
	}

	rewind(decodificar);		//vai para o início do arquivo
	
	decodificado = fopen("monologo.dec", "ab");
	if(!decodificado)
	{
		printf("Não foi possível abrir o arquivo decodificado.\n");
		fclose(arquivo);
		fclose(mapaBits);
		free(hamming);
		return -1;
	}

	printf("\n");

	unsigned char i = 1;
	unsigned int indice = 1;

	printf("INDICE 1 = %d\n", indice1);
	printf("TOTAL BITS = %d\n", total_bits);

	while (fgets(linha_codificada, sizeof(linha_codificada), decodificar) != NULL)
{
    // Remove o '\n' do final, se existir
    linha_codificada[strcspn(linha_codificada, "\n")] = '\0';

	//printf("strlen(linha_codificada) = %d\n", strlen(linha_codificada));

    // Verifica se a linha tem exatamente 31 caracteres (bits)
	if (strlen(linha_codificada) != 31) {
	   if (strlen(linha_codificada) == 0) {
		    continue; // Linha vazia, pula
		} else {
			printf("Erro: Linha com tamanho inválido (%ld)\n", strlen(linha_codificada));
			continue;
		}
	}
    // Processa a linha codificada
    decodificaHamming(decodificado, temporario, linha_codificada, hamming);

    // Atualiza os índices
    indice += 26;
    i += 32;
}

//	printf("indice do teste decodificado = %d\n", indice);

	unsigned int bits_uteis = ((total_bits / 26) * 26); // Apenas os bits de dados completos
	if (bits_uteis > indice - 1) {
		bits_uteis = indice - 1;
	}

	fclose(temporario);

	temporario = fopen("temporario.txt", "r");

	traduz(temporario, decodificado, bits_uteis);

	/*--------------------------------------------------------------------------------*/

	fclose(decodificado);
	fclose(decodificar);
	fclose(temporario);
	fclose(arquivo);
	free(hamming);

	return 0;
}
