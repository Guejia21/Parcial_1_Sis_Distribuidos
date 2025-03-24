/*Definicion de las operaciones que se pueden realizar*/
program gestionar_cocineros{
    version gestionar_cocineros_version{
        bool seleccionarIDCocinero(int idCocinero)=1;
        bool terminarPrepararPedido(int idCocinero)=2;
    }=1;
}=0x20000003;