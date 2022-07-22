#include "Tarjeta.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LEN_MSG 5
#define LEN_NUM 3
#define LEN_MONTO 13
#define LEN_SEC 4

struct RequestMsg {
	char tipoMensaje[LEN_MSG];
	char  lenNumTarjeta[LEN_NUM];
	char* numeroTarjeta;
	char  monto[LEN_MONTO];
	char  codSeguridad[LEN_SEC];
};	

struct
RequestMsg* CrearRequest(char* buffer);
bool BuscarNumeroTarjeta(range_t* rt);
bool EsNumeroTarjetaABuscar(range_t* rt, char* numeroTarjeta);
char* BuscarNombreTarjeta(int* id);

bool CheckRequestMessage(char* buffer) {
	
	struct RequestMsg* requestMsg = CrearRequest(buffer);
	range_t rt;
	card_t ct;

	rt.len = atoi(requestMsg->lenNumTarjeta);

	strncpy(rt.rangeLow, requestMsg->numeroTarjeta, 8);
	rt.rangeLow[8] = '\0';

	int offset = (rt.len - (LEN_RANGE - 1));
	strncpy(rt.rangeHigh, requestMsg->numeroTarjeta+offset, 8);
	rt.rangeHigh[8] = '\0';

	bool encontrado = BuscarNumeroTarjeta(&rt);
	if (encontrado) {
		char* nombreTarjeta;
		nombreTarjeta = BuscarNombreTarjeta(&rt.id);
	}
	return encontrado;
}

struct
RequestMsg* CrearRequest(char* buffer) {
	struct RequestMsg *rm = malloc(sizeof(struct RequestMsg));
	
	strncpy(rm->tipoMensaje, buffer, 4);
	printf("tipo: %s\n", rm->tipoMensaje);

	strncpy(rm->lenNumTarjeta, buffer+=4, 2);
	int len = atoi(rm->lenNumTarjeta);
	printf("len:  %s\n", rm->lenNumTarjeta);

	rm->numeroTarjeta = malloc(len+1);
	strncpy(rm->numeroTarjeta, buffer+=2, len);
	rm->numeroTarjeta[len] = '\0';
	printf("num:  %s\n", rm->numeroTarjeta);

	strncpy(rm->monto, buffer+=atoi(rm->lenNumTarjeta), (LEN_MONTO-1));
	printf("monto: %s\n", rm->monto);

	strncpy(rm->codSeguridad, buffer+=(LEN_MONTO-1), 3);
	printf("sec:  %s\n", rm->codSeguridad);

	return rm;
}

bool BuscarNumeroTarjeta(range_t* rt) {

	FILE *fp = fopen("./test/server/ranges.dat", "r");

	if (!fp) {
		printf("Error al abrir ./test/server/ranges.dat\n");
		return false;
	}

	size_t sizeLine = 32;
	char*line = malloc(sizeof(char)*32);
	int len;
	bool encontrado = false;
	do {
		len = getline(&line, &sizeLine, fp);

		// get id
		char idAux[3] = {0};
		strncpy(idAux,line,2);
		rt->id = (atoi(idAux));

		char* aux = line;
		int lenCard = strlen(aux+=2);

		char numeroTarjeta[lenCard];
		strncpy(numeroTarjeta, aux, lenCard-1);
		
		encontrado = EsNumeroTarjetaABuscar(rt, numeroTarjeta);	
		
		if (len==-1) break;
		if (encontrado) {
			printf("Encontrado: %s\n", numeroTarjeta);
			break;
		}
	} while (len >= 0);
	free(line);
	fclose(fp);
	return encontrado;
}


bool EsNumeroTarjetaABuscar(range_t* rt, char* numeroTarjeta) {
	char range_l[LEN_RANGE];
	char range_h[LEN_RANGE];
	int len = strlen(numeroTarjeta);
	int offset_h = len - (LEN_RANGE - 1);
	if (rt->len != len) return false; // len from file ranges.dat != len from request msg client

	char *aux = numeroTarjeta;
	strncpy(range_l, numeroTarjeta, LEN_RANGE-1);
	strncpy(range_h, aux+=(offset_h), LEN_RANGE-1);

	return (!strncmp(range_l, rt->rangeLow, (LEN_RANGE-1)) || 
			!strncmp(range_h, rt->rangeHigh, (LEN_RANGE-1)));
}

char* BuscarNombreTarjeta(int* id) {
	char c_id[3];
	snprintf(c_id,3,"%02d",(*id));
	printf("Id a buscar: %s\n", c_id);
	FILE *fp = fopen("./test/server/cards.dat", "r");

	if (!fp) {
		printf("Error al abrir ./test/server/cards.dat\n");
		return false;
	}

	size_t sizeLine = 32;
	char*line = malloc(sizeof(char)*32);
	int len;
	bool encontrado = false;
	char* nombreTarjeta = NULL;
	do {
		len = getline(&line, &sizeLine, fp);

		// get id
		char idAux[3] = {0};
		strncpy(idAux,line,2);

		if (!strncmp(idAux,c_id,2)) { //id encontrado
			char* aux = line;
			int lenCard = strlen(aux+=2);

			nombreTarjeta = malloc(lenCard);
			strncpy(nombreTarjeta, aux, lenCard-1);
			nombreTarjeta[lenCard-1] = '\0';
			
			printf("Encontrado: %s\n", nombreTarjeta);
		}
		if (len==-1) break;
	} while (len >= 0);
	free(line);
	fclose(fp);
	return nombreTarjeta;
}
