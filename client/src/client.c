#include "client.h"
#include "utils.h"
#include <commons/log.h>
#include <readline/readline.h>
#include <string.h>

int main(void)
{
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	// Instanciar un logger.
	logger = iniciar_logger();
	log_info(logger, "Logger instanciado exitosamente.");

	// Instanciar la config del cliente
	config = iniciar_config(logger);
	log_info(logger, "Configuracion del cliente instanciada exitosamente.");


	// Obtener los valores de la instancia de configuracion
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Leer la consola, esperar la respuesta del usuario
	leer_consola(logger);

	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
  enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("top0_logger.log", "LOGGER_TP0", 1, LOG_LEVEL_INFO);
	
	if(nuevo_logger == NULL){
		perror("[ERROR]: No se pudo instanciar el nuevo_logger. (Return NULL)");
		exit(EXIT_FAILURE);
	}
	
	return nuevo_logger;
}

t_config* iniciar_config(t_log* logger)
{
	t_config* nuevo_config = config_create("cliente.config");

	if(nuevo_config == NULL){
		log_error(logger, "No se pudo instanciar la configuracion del cliente");
		exit(EXIT_FAILURE);
	}
	
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	// Declaro un espacio de memoria para guardar el input del usuario
	char* leido;

	// Mientras leido no sea NULL, es decir, el usuario no ejecute un comando vacio...
	do
	{
		// Solicito un "valor"
		leido = readline("[Console] ~> ");

		// Lo muestro por pantalla
		if (strcmp(leido, "\0") != 0)
		{
			log_info(logger, "leido ~> %s", leido);
		}
	} while (strcmp(leido, "\0") != 0);
	

	// Libero la memoria
	free(leido);
	log_info(logger,"Liberado el espacio de memoria de 'leido'");
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	do {
		leido = readline("[Package] ~>");
		
		if(strcmp(leido, "\0") != 0)
		{
			agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		}

	} while (strcmp(leido, "\0") != 0);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);

	enviar_paquete(paquete, conexion);
	free(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
}
