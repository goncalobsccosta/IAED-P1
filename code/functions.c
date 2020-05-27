/*
 * File:  functions.c
 * Author:  Goncalo Costa
 * Description: Ficheiro que contem todas as funcoes do projeto.
*/

#include "functions.h"

void strtoken(char line[INPUT][LINEMAX], int input){
	/*Funcao auxiliar de separacao da instrucao lida*/
	const char s[2] = ":";
	char *token, aux[LINEMAX];
	int i = 1;

	
	strcpy(aux, line[0]);
	*line[0] = '\0';

	token = strtok(aux, s);
	strcpy(line[0], token);
	
	/*Sabendo o numero da instrucoes que a linha tem, e possivel separar a linha*/
	while(i < input){
		token = strtok(NULL, s);
		*line[i] = '\0';
		strcpy(line[i], token);
		i++;
	}
}


int a(struct Product prodList[PRODUCTS], char line[INPUT][LINEMAX], int idp){
	/*Funcao adiciona produto
	* Separar a linha em instrucoes*/
	strtoken(line, 4);

	/*Atribuir cada valor a sua variavel*/
	strcpy(prodList[idp].desc, line[0]);
	prodList[idp].price = atoi(line[1]);
	prodList[idp].weight = atoi(line[2]);
	prodList[idp].qtd = atoi(line[3]);
	prodList[idp].idp = idp;

	printf("Novo produto %i.\n", idp);
	/*Incrementa o indice do produto*/
	return ++idp;
}


void q(struct Product prodList[PRODUCTS], char line[INPUT][LINEMAX], int idp){
	/*Funcao adiciona stock ao produto*/
	int auxIdp, auxQtd; 

	/*Separa a linha e atribui os valores as variaveis*/
	strtoken(line, 2);
	auxIdp = atoi(line[0]);
	auxQtd = atoi(line[1]);

	/*Condicao de verificacao da exequibilidade da instrucao*/
	auxIdp < idp ? prodList[auxIdp].qtd += auxQtd : printf("Impossivel adicionar produto %i ao stock. Produto inexistente.\n", auxIdp); 
}


void A(struct Product prodList[PRODUCTS], char line[INPUT][LINEMAX], int idp, int ide, struct Encomenda encomendas[ORDERS]){
	/*Funcao adiciona produto a encomenda*/
	int auxIde, auxIdp, auxQtd, pesoTotal, i = 0;

	/*Separa a linha e atribui os valores as variaveis*/
	strtoken(line, 3);
	auxIde = atoi(line[0]);
	auxIdp = atoi(line[1]);
	auxQtd = atoi(line[2]);

	/*Condicoes de verificacao da exequibilidade da instrucao*/
	if (auxIde > ide){printf("Impossivel adicionar produto %i a encomenda %i. Encomenda inexistente.\n", auxIdp, auxIde); return;}

	if (auxIdp >= idp){printf("Impossivel adicionar produto %i a encomenda %i. Produto inexistente.\n", auxIdp, auxIde); return;}

	if (auxQtd > prodList[auxIdp].qtd){printf("Impossivel adicionar produto %i a encomenda %i. Quantidade em stock insuficiente.\n", auxIdp, auxIde); return;}

	pesoTotal = pesoEncomenda(prodList, encomendas[auxIde]);
	
	if ((pesoTotal + (prodList[auxIdp].weight * auxQtd)) > 200){
		printf("Impossivel adicionar produto %i a encomenda %i. Peso da encomenda excede o maximo de 200.\n", auxIdp, auxIde); return;
	}

	/*Ciclo de procura de produto, caso ja exista esse produto numa encomenda, nao adiciona novo produto, mas sim 
	adiciona ao stock*/
	for(i= 0 ; i < encomendas[auxIde].prodTotal; i++){
		if (encomendas[auxIde].prodIdp[i] == auxIdp){
			encomendas[auxIde].prodQtd[i] += auxQtd;
			prodList[auxIdp].qtd -= auxQtd;
			return;
		}
	}
	/*Caso nao encontre o produto na encomenda, adiciona novo porduto na encomenda*/
	encomendas[auxIde].prodIdp[encomendas[auxIde].prodTotal] = auxIdp;
	encomendas[auxIde].prodQtd[encomendas[auxIde].prodTotal] = auxQtd;
	prodList[auxIdp].qtd -= auxQtd;
	encomendas[auxIde].prodTotal++;
}


int pesoEncomenda(struct Product prodList[PRODUCTS], struct Encomenda order){
	/*Funcao auxiliar de calculo do peso de uma encomenda*/
	int i, peso = 0;

	/*Ciclo que incrementa o peso da encomenda*/
	for(i = 0; i <= order.prodTotal; i++){
		peso += (order.prodQtd[i]) * (prodList[order.prodIdp[i]].weight);
	}
	return peso;
}


void r(struct Product prodList[PRODUCTS], char line[INPUT][LINEMAX], int idp){
	/*Funcao remove stock de produto*/
	int auxIdp, auxQtd;

	/*Separa a linha e atribui os valores as variaveis*/
	strtoken(line, 2);
	auxIdp = atoi(line[0]);
	auxQtd = atoi(line[1]);

	/*Condicoes de verificacao da exequibilidade da instrucao*/
	if (auxIdp >= idp){
		printf("Impossivel remover stock do produto %i. Produto inexistente.\n", auxIdp);
		return;
	}
	if (prodList[auxIdp].qtd < auxQtd){
		printf("Impossivel remover %i unidades do produto %i do stock. Quantidade insuficiente.\n", auxQtd, auxIdp);
		return;
	}
	/*Caso exista o produuto remove o stock*/
	prodList[auxIdp].qtd -= auxQtd;
}


void R(char line[INPUT][LINEMAX], int idp, int ide, struct Encomenda encomendas[ORDERS], struct Product prodList[PRODUCTS]){
	/*Funcao remove produto de encomenda*/
	int auxIdp, auxIde, pos;

	/*Separa a linha e atribui os valores as variaveis*/
	strtoken(line, 2);
	auxIde = atoi(line[0]);
	auxIdp = atoi(line[1]);

	/*Condicoes de verificacao da exequibilidade da instrucao*/
	if (auxIde > ide){printf("Impossivel remover produto %i a encomenda %i. Encomenda inexistente.\n", auxIdp, auxIde); return;}

	if (auxIdp >= idp){printf("Impossivel remover produto %i a encomenda %i. Produto inexistente.\n", auxIdp, auxIde); return;}

	/*Algoritmo de procura do produto na encomenda*/
	for(pos = 0; pos < encomendas[auxIde].prodTotal; pos++){
		if (encomendas[auxIde].prodIdp[pos] == auxIdp){
			/*Caso encontre adiciona o stock ao sistema e retira o produto da encomenda*/
			prodList[auxIdp].qtd += encomendas[auxIde].prodQtd[pos];
			encomendas[auxIde].prodQtd[pos] = 0;
			return;
		}
	}
}


void C(int ide, struct Encomenda encomendas[ORDERS], char line[INPUT][LINEMAX], struct Product prodList[PRODUCTS]){
	/*Calculo do custo de uma encomenda*/
	int auxIde, i, price = 0;
	auxIde = atoi(line[0]);

	/*Condicao de verificacao da exequibilidade da instrucao*/
	if (auxIde > ide){printf("Impossivel calcular custo da encomenda %i. Encomenda inexistente.\n", auxIde); return;}

	/*Algoritmo que calcula o custo de uma encomenda*/
	for(i = 0; i <= encomendas[auxIde].prodTotal; i++){
		price += prodList[encomendas[auxIde].prodIdp[i]].price * encomendas[auxIde].prodQtd[i];
	}
	printf("Custo da encomenda %i %i.\n", auxIde, price);
}


void p(struct Product prodList[PRODUCTS], char line[INPUT][LINEMAX], int idp){
	/*Muda o preco de um produto existente no sistema*/
	int auxIdp, price;

	/*Separa a linha e atribui os valores as variaveis*/
	strtoken(line, 2);
	auxIdp = atoi(line[0]);
	price = atoi(line[1]);

	/*Muda o preco caso seja possivel*/
	if (auxIdp >= idp){printf("Impossivel alterar preco do produto %i. Produto inexistente.\n", auxIdp); return;}
	prodList[auxIdp].price = price;
}


void E(int ide, struct Encomenda encomendas[ORDERS], char line[INPUT][LINEMAX], struct Product prodList[PRODUCTS], int idp){
	/*Mostra a descricao e a quantidade de um produto numa encomenda*/
	int auxIdp, auxIde, pos;

	/*Separa a linha e atribui os valores as variaveis*/
	strtoken(line, 2);
	auxIde = atoi(line[0]);
	auxIdp = atoi(line[1]);

	/*Condicoes de verificacao da exequibilidade da instrucao*/
	if (auxIde > ide){printf("Impossivel listar encomenda %i. Encomenda inexistente.\n", auxIde); return;}
	
	if (auxIdp >= idp){printf("Impossivel listar produto %i. Produto inexistente.\n", auxIdp); return;}

	/*Algoritmo que encontra um produto numa encomenda e retorna a sua descricao e quantidade*/
	for(pos = 0; pos <= encomendas[auxIde].prodTotal; pos++){
		if (encomendas[auxIde].prodIdp[pos] == auxIdp){
			printf("%s %i.\n", prodList[auxIdp].desc, encomendas[auxIde].prodQtd[pos]);
			return;
		}
	}
	printf("%s 0.\n", prodList[auxIdp].desc);
}


void m(int ide, struct Encomenda encomendas[ORDERS], char line[INPUT][LINEMAX], int idp){
	/*Dado um produto retorna a encomenda que tem mais quantidade do prorpio*/
	int auxIdp, qtd = 0, i = 0, p = 0, auxIde;
	auxIdp = atoi(line[0]);

	/*Condicao de verificacao da exequibilidade da instrucao*/
	if (auxIdp >= idp){printf("Impossivel listar maximo do produto %i. Produto inexistente.\n", auxIdp); return;}

	/*Algoritmo de procura do produto com mais ocorrencias em todas as encomendas*/
	for (i = 0; i <= ide; i++){
		/*Algoritmo de procura do produto introcuzido no comando*/
		for (p = 0; p <= encomendas[i].prodTotal; p++){
			if (encomendas[i].prodIdp[p] == auxIdp){
				if (encomendas[i].prodQtd[p] > qtd){
					/*Guarda a quantidade caso ela seja maior que a guardada anteriormente*/
					qtd = encomendas[i].prodQtd[p];
					auxIde = i;
				}
			}
		}
	}

	/*Caso exista esse produto numa encomenda, retorna-o*/
	if (qtd != 0){
		printf("Maximo produto %i %i %i.\n", auxIdp, auxIde, qtd);
	}
}


void quicksort1(struct Product prodListSorted[PRODUCTS], int l, int r){
	/*Primeira funcao de ordenacao*/
	int i;
	if (r <= l) return;
	i = partition(prodListSorted, l, r);
	quicksort1(prodListSorted, l, i - 1);
	quicksort1(prodListSorted, i + 1, r);
}


int partition(struct Product prodListSorted[PRODUCTS], int l, int r){
	int i = l - 1;
	int j = r;
	while (i < j){
		/*Verificacao de ordenacao dos produtos tendo em conta os precos, e estabilizacao do algoritmo com base nos indices de produto*/
		while (prodListSorted[++i].price < prodListSorted[r].price || ((prodListSorted[i].price == prodListSorted[r].price) && prodListSorted[i].idp < prodListSorted[r].idp));
		while (prodListSorted[r].price < prodListSorted[--j].price || ((prodListSorted[j].price == prodListSorted[r].price) && prodListSorted[r].idp < prodListSorted[j].idp))
			if (j == l) break;
		if (i < j)
			/*Macro utilizada para trocar dos produtos de posicao*/
			exchg(prodListSorted[i], prodListSorted[j]);
	}
	exchg(prodListSorted[i], prodListSorted[r]);
	return i;
}


void l(struct Product prodList[PRODUCTS], int idp){
	int i = 0, p;
	struct Product prodListSorted[PRODUCTS];
	
	/*Copia os produtos do sistema para poderem ser organizados*/
	for(p = 0; p < idp; p++){
		strcpy(prodListSorted[p].desc, prodList[p].desc);
		prodListSorted[p].price = prodList[p].price;
		prodListSorted[p].weight = prodList[p].weight;
		prodListSorted[p].qtd = prodList[p].qtd;
		prodListSorted[p].idp = prodList[p].idp;
	}

	/*Organiza todos os produtos do menor preco para o maior*/
	quicksort1(prodListSorted, 0, idp - 1);
	
	printf("Produtos\n");
	/*Retorna a lista dos produtos organizados*/
	for(i = 0; i < idp; i++){
		printf("* %s %i %i\n", prodListSorted[i].desc, prodListSorted[i].price, prodListSorted[i].qtd);
	}
}


void quicksort2(struct Product produ[MAXORDER], int l, int r){
	/*Segunda funcao de ordenacao*/
	int i;
	if (r <= l) return;
	i = partition1(produ, l, r);
	quicksort2(produ, l, i - 1);
	quicksort2(produ, i + 1, r);
}


int partition1(struct Product produ[MAXORDER], int l, int r){
	int i = l - 1;
	int j = r;
	struct Product pAux = produ[r];
	while (i < j){
		/*Compara as descricoes de cada produto para organizar por ordem alfabetica*/
		while (strcmp(produ[++i].desc, pAux.desc) < 0);
		while (strcmp(pAux.desc, produ[--j].desc) < 0)
			if (j == l) break;
		if (i < j)
			/*Macro utilizada para trocar dos produtos de posicao*/
			exchg(produ[i], produ[j]);
	}
	exchg(produ[i], produ[r]);
	return i;
}


void L(int ide, struct Encomenda encomendas[ORDERS], char line[INPUT][LINEMAX], struct Product prodList[PRODUCTS]){
	int auxIde, p = 0, i = 0;
	struct EncomendaUnica singleEncomenda;
	auxIde = atoi(line[0]);

	/*Condicao de verificacao da exequibilidade da instrucao*/
	if (auxIde > ide){printf("Impossivel listar encomenda %i. Encomenda inexistente.\n", auxIde); return;}

	/*Copia uma encomenda especifica para ser organizada*/
	for(p = 0; p < encomendas[auxIde].prodTotal; p++){
		singleEncomenda.prods[p] = prodList[encomendas[auxIde].prodIdp[p]];
		singleEncomenda.prods[p].qtd = encomendas[auxIde].prodQtd[p];
	}
	singleEncomenda.prodTotal = encomendas[auxIde].prodTotal;

	/*Organiza por ordem alfabetica*/
	quicksort2(singleEncomenda.prods, 0, singleEncomenda.prodTotal - 1);

	printf("Encomenda %i\n", auxIde);
	/*Retorna uma lista de todos os produtos numa encomenda por ordem alfabetica*/
	for(i = 0; i < encomendas[auxIde].prodTotal; i++){
		if (singleEncomenda.prods[i].qtd != 0){
			printf("* %s %i %i\n", singleEncomenda.prods[i].desc, singleEncomenda.prods[i].price, singleEncomenda.prods[i].qtd);
		}
	}
}

void V(int ide, struct Encomenda encomendas[ORDERS], char line[INPUT][LINEMAX]){
	int auxIde = atoi(line[0]);

	/*Condicao de verificacao da exequibilidade da instrucao*/
	if (auxIde > ide){printf("Impossivel listar encomenda %i. Encomenda inexistente.\n", auxIde); return;}

	printf("%i %s.\n", auxIde, encomendas[auxIde].cliente);
}

void quicksort3(struct EncomendaCusto encomendas[ORDERS], int l, int r){
	/*Primeira funcao de ordenacao*/
	int i;
	if (r <= l) return;
	i = partition3(encomendas, l, r);
	quicksort3(encomendas, l, i - 1);
	quicksort3(encomendas, i + 1, r);
}


int partition3(struct EncomendaCusto encomendas[ORDERS], int l, int r){
	int i = l - 1;
	int j = r;
	while (i < j){
		/*Verificacao de ordenacao dos produtos tendo em conta os precos, e estabilizacao do algoritmo com base nos indices de produto*/
		while (encomendas[++i].custo < encomendas[r].custo || ((encomendas[i].custo == encomendas[r].custo) && encomendas[i].ide < encomendas[r].ide));
		while (encomendas[r].custo < encomendas[--j].custo || ((encomendas[j].custo == encomendas[r].custo) && encomendas[r].ide < encomendas[j].ide))
			if (j == l) break;
		if (i < j)
			/*Macro utilizada para trocar dos produtos de posicao*/
			exchg2(encomendas[i], encomendas[j]);
	}
	exchg2(encomendas[i], encomendas[r]);
	return i;
}

void Y(int ide, struct Encomenda encomendas[ORDERS], struct Product prodList[PRODUCTS]){
	int p = 0, i = 0, price = 0;
	struct EncomendaCusto encomendasc[ORDERS];
	for (p = 0; p <= ide; p++){
		price = 0;
		for(i = 0; i <= encomendas[p].prodTotal; i++){
			price += prodList[encomendas[p].prodIdp[i]].price * encomendas[p].prodQtd[i];
		}
		encomendasc[p].custo = price;
		encomendasc[p].ide = p;
	}

	/*Organiza por ordem alfabetica*/
	quicksort3(encomendasc, 0, ide);

	printf("Encomendas\n");
	/*Retorna uma lista de todos os produtos numa encomenda por ordem alfabetica*/
	for(i = 0; i <= ide; i++){
			printf("* %i %i\n", encomendasc[i].ide, encomendasc[i].custo);
	}
}