/*Alunas:
 *Giovanna Fioravante Dalledone
 *Nadia Luana Lobkov
*/

#ifndef HAMMING31_26_H
#define HAMMING31_26_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct Hamming {
	uint32_t codigo[32];	//vetor de dados + bits de paridade (0 até 31 - vamos usar do 1 ao 31)
};

//Deixa vazia as posições dos bits de redundância
//Preenche com dados do arquivo as demais posições
//Manipula a struct Hamming
void marcaPosicoes(unsigned char linha[28], struct Hamming *hamming);

//Recebe o vetor com os dados do arquivo
//Calcula os bits de paridade
//Devolve um arquivo codificado
void codificaHamming(FILE *arquivo, struct Hamming *hamming);

void traduz(FILE *arquivo, FILE *destino, unsigned int total_bits);

//Recebe o vetor codificado
//Verifica a paridade
//Em caso de sucesso retorna o arquivo decodificado
//Em caso de falha retorna mensagem de erro
void decodificaHamming(FILE *arquivo, FILE *temporario, unsigned char linha[32], struct Hamming *h);

#endif
