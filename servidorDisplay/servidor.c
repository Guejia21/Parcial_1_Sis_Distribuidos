/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "IntSerPedidosSerDisplay.h"
#include <stdbool.h>
#include <time.h>

/**
 * @brief Funcion que permite mostrar la hora de la ultima actualizacion 
 * 
 */
void mostrarHoraUltimaActualizacion();

/**
 * @brief Funcion que permite enviar una notificacion al servidor
 * @param argp
 * @param rqstp 
 * @return void* 
 */
void * enviarnotificacion_1_svc(notificacion *argp, struct svc_req *rqstp)
{
	static char * result;
	printf("\n								Pedidos asignados						");
	printf("\n Mesa			Cocinero			Tipo Hamburguesa		Cantidad Ingredientes		Nombre");
	for(int i=0; i<3; i++){
		if(((*argp).cocineros[i].ocupado==true)){
			printf("\n   %d			  %d				      %d			           %d				 %s",
			(i+1),
			(*argp).cocineros[i].noCocinero,
			(*argp).cocineros[i].objHamburguesaAPreparar.tipoHamburguesa,
			(*argp).cocineros[i].objHamburguesaAPreparar.cantidadIngredientesExtra,
			(*argp).cocineros[i].objHamburguesaAPreparar.nombreHamburguesa);
		}
	}
	int cantidadUsuariosFilaVirtual = (*argp).cantidadUsuariosFilaVirtual;
	if (cantidadUsuariosFilaVirtual > 0)
	{
		printf("\n Cantidad de pedidos en la fila: %d", (*argp).cantidadUsuariosFilaVirtual);
	}	
	mostrarHoraUltimaActualizacion();
	printf("\n");
	return (void *) &result;
}

void mostrarHoraUltimaActualizacion(){
	time_t tiempo_actual;
	struct tm *info_tiempo;
	//Obtener el tiempo actual
	time(&tiempo_actual);
 
	//Convertir el tiempo a una estructura local
	info_tiempo = localtime(&tiempo_actual);
	printf("\n Ultima actualizacion: %02d:%02d\n",
		info_tiempo->tm_hour,
		info_tiempo->tm_min
	);
}