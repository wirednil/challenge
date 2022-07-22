#include <t_financiera.h>
#include <Socket.h>
#include <stdio.h>
#include <string.h>

#define LenTipoMensaje 5
#define LenNumTarjeta  16
#define LenMonto       13

#define LOGERR(fmt,...) { fprintf(stderr,fmt "\n",##__VA_ARGS__);}
const char* const OK = "00";
typedef int sock;

int EnviarAHost(unsigned char* data);
void ResponseMessage(unsigned char* data);

int main(int argc, char*argv[]) {
	Transaccion* t;
	if (!strncmp(argv[1],"1", 1))
		t = CreaTransaccion(25.25, "1243098732458765", "223");
	else 
		t = CreaTransaccion(3704.25, "1241098732458755", "199");

	Mostrar(ValidarDatos(t)); 
	
	char auxMonto[LenMonto];
	snprintf(auxMonto, LenMonto, "%012.0f", t->monto*100);

	char* buffer = CrearRequestMessage(t);
	return EnviarAHost((unsigned char*)buffer);
}

int EnviarAHost(unsigned char* data) {
	sock sockfd = socketCreate();
	if (sockfd == -1) {
		LOGERR("No se pudo crear el socket");
		return -1;
	}

	sock connfd = socketConnect(sockfd, "127.0.0.1", 9000);
	if (connfd == -1) {
		LOGERR("Fallo en la conexión");
		return -1;
	}

//	printf("Se pudo conectar con el servidor\n");

	sock ret = socketWrite(sockfd, data);
	memset(data,0,strlen(data));
	sock resm = socketRead(sockfd, data, 5);

	ResponseMessage(data);

	socketClose(sockfd);
	return 0;
}

void ResponseMessage(unsigned char* data) {
	char tipoMensaje[LenTipoMensaje];
	char cod_respuesta[3];
	unsigned char* aux = data;
	strncpy(tipoMensaje,data,4);
	strncpy(cod_respuesta, data+=4,2);
	
	if (!strncmp(OK,cod_respuesta,2)) {
		printf("APROBADA.\n"); 
	} else {
		printf("RECHAZADA.\n");
	}
}




