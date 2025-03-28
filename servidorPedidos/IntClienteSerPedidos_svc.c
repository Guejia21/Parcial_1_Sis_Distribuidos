/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "IntClienteSerPedidos.h"
#include "IntCocineroSerPedidos.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
autorizar_usuarios_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		nodo_hamburguesa generarturno_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case generarTurno:
		_xdr_argument = (xdrproc_t) xdr_nodo_hamburguesa;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) generarturno_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}
static void
gestionar_cocineros_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		int seleccionaridcocinero_1_arg;
		int terminarprepararpedido_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case seleccionarIDCocinero:
		_xdr_argument = (xdrproc_t) xdr_int;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) seleccionaridcocinero_1_svc;
		break;

	case terminarPrepararPedido:
		_xdr_argument = (xdrproc_t) xdr_int;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) terminarprepararpedido_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

void cargarVectorCocineros(){
	for (int i = 0; i < 3; i++)
	{
		vectorCocineros[i].noCocinero = (i+1);
		vectorCocineros[i].ocupado = false;
		vectorCocineros[i].enLinea = false;
	}
	
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (autorizar_usuarios, autorizar_usuarios_version);
	pmap_unset (gestionar_cocineros, gestionar_cocineros_version);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, autorizar_usuarios, autorizar_usuarios_version, autorizar_usuarios_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (autorizar_usuarios, autorizar_usuarios_version, udp).");
		exit(1);
	}
	if (!svc_register(transp, gestionar_cocineros, gestionar_cocineros_version, gestionar_cocineros_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (gestionar_cocineros, gestionar_cocineros_version, udp).");
		exit(1);
	}
	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, autorizar_usuarios, autorizar_usuarios_version, autorizar_usuarios_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (autorizar_usuarios, autorizar_usuarios_version, tcp).");
		exit(1);
	}
	if (!svc_register(transp, gestionar_cocineros, gestionar_cocineros_version, gestionar_cocineros_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (gestionar_cocineros, gestionar_cocineros_version, tcp).");
		exit(1);
	}
	cargarVectorCocineros();
	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
