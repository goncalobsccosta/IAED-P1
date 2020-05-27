/*
 * File:  main.c
 * Author:  Goncalo Costa
 * Description: Loop principal do porgrama.
*/

#include "functions.h"

int main(){
	char command, line[INPUT][LINEMAX];
	struct Product prodList[PRODUCTS];
	struct Encomenda encomendas[ORDERS];
	int idp = 0, ide = -1;

	while(1){
		/*Leitura do comando*/
		scanf("%c", &command);
		switch(command){
			/*Dependendo do comando entra em determinada funcao*/
			case 'a':
				scanf("%s", line[0]);
				idp = a(prodList, line, idp);
				break;
			case 'q':
				scanf("%s", line[0]);
				q(prodList, line, idp);
				break;
			case 'N':
				scanf("%s", line[0]);
				printf("Nova encomenda %i %s.\n", ++ide, line[0]);
				encomendas[ide].prodTotal = 0;
				strcpy(encomendas[ide].cliente, line[0]);
				break;
			case 'A':
				scanf("%s", line[0]);
				A(prodList, line, idp, ide, encomendas);
				break;
			case 'r':
				scanf("%s", line[0]);
				r(prodList, line, idp);
				break;
			case 'R':
				scanf("%s", line[0]);
				R(line, idp, ide, encomendas, prodList);
				break;
			case 'C':
				scanf("%s", line[0]);
				C(ide, encomendas, line, prodList);
				break;
			case 'p':
				scanf("%s", line[0]);
				p(prodList, line, idp);
				break;
			case 'E':
				scanf("%s", line[0]);
				E(ide, encomendas, line, prodList, idp);
				break;
			case 'm':
				scanf("%s", line[0]);
				m(ide, encomendas, line, idp);
				break;
			case 'l':
				l(prodList, idp);
				break;
			case 'L':
				scanf("%s", line[0]);
				L(ide, encomendas, line, prodList);
				break;
			case 'V':
				scanf("%s", line[0]);
				V(ide, encomendas, line);
				break;
			case 'Y':
				Y(ide, encomendas, prodList);
				break;
			case 'x':
				return 0;
		}
	}
}