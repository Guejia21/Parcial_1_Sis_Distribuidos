/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "IntClienteSerPedidos.h"
#include "IntSerPedidosSerDisplay.h"
#include "IntCocineroSerPedidos.h"
#include <stdbool.h>
#include <string.h>


int cantidadUsuariosFila = 0;
cocinero vectorCocineros[3];
nodo_hamburguesa filaVirtual[10];

int consultarNumeroCocineroDispobible()
{
	int i;
	for(i = 0; i < 3; i++)
	{
		if(vectorCocineros[i].ocupado == false)
		{
			return i;
		}
	}
	return -1;
}
void notificar_cocineros_1()
{
	CLIENT *datosConexion;
	void  *result_1;
	char ipServidor[10];
	notificacion  objNotificacion;
	strcpy(ipServidor,"localhost");

#ifndef	DEBUG
	datosConexion = clnt_create (ipServidor, notificar_cocineros, notificar_cocineros_version, "udp");
	if (datosConexion == NULL) {
		clnt_pcreateerror (ipServidor);
		exit (1);
	}
#endif	/* DEBUG */
	for(int i =0;i < 3;i++){
		objNotificacion.cocineros[i].noCocinero = vectorCocineros[i].noCocinero;
		objNotificacion.cocineros[i].ocupado = vectorCocineros[i].ocupado;
		objNotificacion.cocineros[i].objHamburguesaAPreparar.cantidadIngredientesExtra = vectorCocineros[i].objHamburguesaAPreparar.cantidadIngredientesExtra;
		strcpy(objNotificacion.cocineros[i].objHamburguesaAPreparar.nombreHamburguesa, vectorCocineros[i].objHamburguesaAPreparar.nombreHamburguesa);
		objNotificacion.cocineros[i].objHamburguesaAPreparar.tipoHamburguesa = vectorCocineros[i].objHamburguesaAPreparar.tipoHamburguesa;
	}
	objNotificacion.cantidadUsuariosFilaVirtual = cantidadUsuariosFila;
	//Enviar la notificación
	result_1 = enviarnotificacion_1(&objNotificacion, datosConexion);
	if (result_1 == (void *) NULL) {
		clnt_perror (datosConexion, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (datosConexion);
#endif	 /* DEBUG */
}

int * generarturno_1_svc(nodo_hamburguesa *argp, struct svc_req *rqstp)
{
	static int  result;
	int posicion = consultarNumeroCocineroDispobible();
	printf("\n\n Hamburguesa a preparar");
	printf("\n Nombre hamburguesa: %s", (*argp).nombreHamburguesa);
	printf("\n Tipo de hamburguesa: %d", (*argp).tipoHamburguesa);
	printf("\n Cantidad de ingredientes extra: %d", (*argp).cantidadIngredientesExtra);
	if(posicion==-1){
		printf("\n No hay cocineros disponibles");
		//Se asgina la hamburguesa pedida a la fila virtual
		filaVirtual[cantidadUsuariosFila] = *argp;
		cantidadUsuariosFila++;
		printf("\n Hamburguesa agregada a la fila virtual");
	}
	else{
		printf("\n El cocinero en la posicion %d está libre y se asiganara a la hamburguesa con nombre %s", (posicion+1), (*argp).nombreHamburguesa);
		//Se asgina la hamburguesa pedida al cocinero y se marca como ocupado
		vectorCocineros[posicion].ocupado = true;
		vectorCocineros[posicion].objHamburguesaAPreparar = *argp;
		printf("\n Hamburguesa asignada al cocinero");
	}
	result = cantidadUsuariosFila;
	notificar_cocineros_1();
	printf("\n");
	return &result;
}
int * seleccionaridcocinero_1_svc(int *argp, struct svc_req *rqstp)
{
	static int  result;
	int indexCoc= (*argp)-1;
	printf("Cocinero intentando conectarse con ID: %d\n", indexCoc+1);

	//Validamos que sea un codigo valido
	if(indexCoc<0 || indexCoc>3){
		printf("El cocinero intento conectarse con un ID invalido\n");
		result = 0;
		return &result;
	}
	//Validamos que no haya otro cocinero con el mismo ID
	if(vectorCocineros[indexCoc].enLinea){
		printf("El cocinero intento conectarse con un ID ya en uso\n");
		result = 0;
		return &result;
	}
	//El cocinero se conecta
	vectorCocineros[indexCoc].enLinea=true;
	printf("El cocinero se conecto con exito\n");
	result = 1;
	return &result;
}

int * terminarprepararpedido_1_svc(int *argp, struct svc_req *rqstp)
{
	static int  result;
	int indexCoc = (*argp)-1;
	printf("El cocinero %d esta intentando terminar su pedido.\n", indexCoc+1);


	//Validamos si el cocinero tiene un pedido 
	if(!vectorCocineros[indexCoc].ocupado){
		printf("El cocinero no tiene pedidos pendientes");
		result = 0;
		return &result;
	}
	printf("El pedido ha sido preparado");	
	//El cocinero termina su pedido
	//Se le asigna otro pedido al cocinero si hay pedidos en la fila		
	
	//Si no hay más pedidos, el cocinero queda libre
	if(cantidadUsuariosFila==0){
		vectorCocineros[indexCoc].ocupado=false;
		notificar_cocineros_1();
		result = 0;
		printf("No hay más pedidos pendientes");
		return &result;
	}	
	//Asignamos al cocinero el siguiente pedido
	vectorCocineros[indexCoc].objHamburguesaAPreparar=filaVirtual[0];
	printf("Se asigno un nuevo pedido al cocinero");
	//Movemos la fila
	for(int i=0;i<cantidadUsuariosFila-1;i++)
		filaVirtual[i]=filaVirtual[i+1];
	//Se elimina un pedido de la fila virtual
	cantidadUsuariosFila--;
	
	//Notificamos el cambio al servidor display
	notificar_cocineros_1();
	result = 1;
	return &result;
}