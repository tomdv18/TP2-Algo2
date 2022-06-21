#define _POSIX_C_SOURCE 200809L
#include "funciones_tp2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include "mensajes.h"


#define COMANDO_VACIO ""


/* ----- CREACION TDA CLINICA ----- */

struct clinica{

	abb_t *doctores;
	hash_t *pacientes;
	hash_t *especialidades;
};

struct doctor
{
	char *nombre;
	char *especialidad;
	size_t atendidos;
}; 

struct paciente
{
	char *nombre;
	char* antiguedad;
};


struct especialidad
{
	char *nombre_especialidad;
	cola_t *cola_urgente;
	heap_t *cola_normal;
	size_t cantidad;
};


//=================================================================================//

/*
 *Funcion que recibe dos paciente_t y devuelve:
 *  un entero que serà
 *   menor a 0  si  la antiguedad de a < antiguedad de b
 *       0      si  antiguedad de a == antiguedad de b
 *   mayor a 0  si  antiguedad de a  > antiguedad de b
 */
int funcion_comparacion_(const paciente_t * a, const paciente_t *b){
/*
	if ((size_t)&a->antiguedad > (size_t)&b->antiguedad){
		return 1;
	}
	else if((size_t)&a->antiguedad < (size_t)&b->antiguedad){
		return -1;
	} */
	return (-1) * strcmp(a->antiguedad, b->antiguedad);
}

/*Pre: Debe recibir dos punteros genericos
 *Funcion de comparacion generica, que llamara a un wrapper de la misma
 *Post: Devuelve un numero entre -1 y 1, segun corresponda
 */ 
int funcion_comparacion(const void *a, const void *b){
	return funcion_comparacion_((paciente_t*)a, (paciente_t*) b);
}


/*
 *Pre: Recibe un doctor almacenado en memoria
 *Post: Libera toda la memoria almacenada correspondiente al doctor
 */
void _funcion_destrucc_doctor(doctor_t * doctor){
	free(doctor->nombre);
	free(doctor->especialidad);
	free(doctor);
}

void funcion_destrucc_doctor(void * doctor){
	_funcion_destrucc_doctor((doctor_t*) doctor);
}

/*
 *Pre: Recibe un paciente almacenado en memoria
 *Post: Libera toda la memoria almacenada correspondiente al paciente
 */
void _funcion_destrucc_paciente(paciente_t * paciente){
	free(paciente->nombre);
	free((void*)paciente->antiguedad);
	free(paciente);
}
void funcion_destrucc_paciente(void * paciente){
	_funcion_destrucc_paciente((paciente_t*) paciente);
}
/*
 *Pre: Recibe una especialidad almacenada en memoria
 *Post: Libera toda la memoria almacenada correspondiente a toda la especialidad
 */
void _funcion_destrucc_especialidad(especialidad_t * especialidad){
	cola_destruir(especialidad->cola_urgente, NULL);
	heap_destruir(especialidad->cola_normal, NULL);
	free(especialidad->nombre_especialidad);
	free(especialidad);
}
void funcion_destrucc_especialidad(void * especialidad){
	_funcion_destrucc_especialidad((especialidad_t*)(especialidad));
}


// Ya esta en el .h
clinica_t *clinica_crear(){


	clinica_t *clinica = malloc(sizeof(clinica_t));
	if (!clinica)
	{
		return NULL;
	}

	clinica->doctores = abb_crear(strcmp, funcion_destrucc_doctor);
	if (!clinica->doctores)
	{
		free(clinica);
		return NULL;
	}

	clinica->pacientes = hash_crear(funcion_destrucc_paciente);
	if (!clinica->pacientes)
	{
		abb_destruir(clinica->doctores);
		free(clinica);
		return NULL;
	}
	clinica->especialidades = hash_crear(funcion_destrucc_especialidad);
	if (!clinica->especialidades)
	{
		abb_destruir(clinica->doctores);
		hash_destruir(clinica->pacientes);
		free(clinica);
		return NULL;
	}
	return clinica;
}
// Ya esta en el .h

void clinica_destruir(clinica_t *clinica)
{

	abb_destruir(clinica->doctores);
	hash_destruir(clinica->pacientes);
	hash_destruir(clinica->especialidades);
	free(clinica);
}
// Ya esta .H
bool clinica_pertenece(clinica_t * clinica, char * persona, char * ambito){
	if (strcmp(ambito, "ESPECIALIDAD") == 0){
		return hash_pertenece(clinica->especialidades, persona);
	}
	if (strcmp(ambito, "PACIENTE") == 0){
		return hash_pertenece(clinica->pacientes, persona);
	}	
	if (strcmp(ambito, "DOCTOR") == 0){
		return abb_pertenece(clinica->doctores, persona);
	}
	return false;
}

//=================================================================================//
// Ya esta en el .h
void *creador_doctor(char **campo, void *extra)
{
	doctor_t *doctor = malloc(sizeof(doctor_t));
	if (!doctor)
	{
		return NULL;
	}
	doctor->nombre = strdup(campo[0]);
	if (!doctor->nombre)
	{
		free(doctor);
		return NULL;
	}
	doctor->especialidad = strdup(campo[1]);
	if (!doctor->especialidad)
	{
		free(doctor->nombre);
		free(doctor);
		return NULL;
	}
	doctor->atendidos = 0;
	return doctor;
}

// Ya esta en el .h
void *creador_paciente(char **campo, void *extra){

	paciente_t *paciente = malloc(sizeof(paciente_t));
	if (!paciente)
	{
		return NULL;
	}
	paciente->nombre = strdup(campo[0]);

	if (!paciente->nombre)
	{
		free(paciente);
		return NULL;
	} 
	if (!isdigit(*campo[1])) 
	{
		
		printf(ENOENT_ANIO, campo[1]);
		free(paciente->nombre);
		free(paciente);
		return NULL;
	} 
	paciente->antiguedad = strdup(campo[1]);
	return paciente;
}
// Ya esta en el .h
bool parsear_pacientes(clinica_t *clinica, lista_t* lista_pacientes){

	lista_iter_t* lista_pacientes_iter = lista_iter_crear(lista_pacientes);

	if(!lista_pacientes_iter){
		return false;
	}

	while(!lista_iter_al_final(lista_pacientes_iter)){
		
		paciente_t* actual = lista_iter_ver_actual(lista_pacientes_iter);
		if(!actual){
			return false;
		}
		if (!hash_guardar(clinica->pacientes, actual->nombre, actual)){
			lista_iter_destruir(lista_pacientes_iter);
			return false;
		}

		lista_iter_avanzar(lista_pacientes_iter);
	}
	lista_iter_destruir(lista_pacientes_iter);
	return true;
}
// Ya esta en el .h
bool parsear_doctores(clinica_t * clinica, lista_t* lista_doctores){
	lista_iter_t* lista_doctores_iter = lista_iter_crear(lista_doctores);

	if(!lista_doctores_iter){
		return false;
	}

	while(!lista_iter_al_final(lista_doctores_iter)){
		
		doctor_t* actual = lista_iter_ver_actual(lista_doctores_iter);
		if(!actual){
			return false;
		}
		if (!abb_guardar(clinica->doctores, actual->nombre, actual)){
			lista_iter_destruir(lista_doctores_iter);
			return false;
		}

		lista_iter_avanzar(lista_doctores_iter);
	}
	lista_iter_destruir(lista_doctores_iter);
	return true;

}
/*
 *Pre: Recibe el nombre de la especialidad a crear. NO debe existir una especialidad con el mismo nombre
 *Post: Devuelve una especialidad, con sus respectivas colas asignadas en memoria.
 *Si falla en algo, devuelve NULL
 */
especialidad_t * especialidad_crear(char * especialidad_nombre){
	especialidad_t* nueva_especialidad = malloc(sizeof(especialidad_t));
	if (!nueva_especialidad){
		return NULL;
	}	
	nueva_especialidad->cola_urgente = cola_crear();
	if (!nueva_especialidad->cola_urgente){
		free(nueva_especialidad);
		return NULL;
	}
	nueva_especialidad->cola_normal = heap_crear(funcion_comparacion);
	if (!nueva_especialidad->cola_normal){
		cola_destruir(nueva_especialidad->cola_urgente, NULL);
		free(nueva_especialidad);
		return NULL;
	}
	nueva_especialidad->nombre_especialidad = strdup(especialidad_nombre);
	nueva_especialidad->cantidad = 0;
	return nueva_especialidad;
}

bool parsear_especialidades(clinica_t * clinica, lista_t* lista_doctores){

	lista_iter_t* lista_doctores_iter = lista_iter_crear(lista_doctores);

	if(!lista_doctores_iter){
		return false;
	}

	while(!lista_iter_al_final(lista_doctores_iter)){
		
		doctor_t* actual = lista_iter_ver_actual(lista_doctores_iter);
		if(!actual){
			lista_iter_destruir(lista_doctores_iter);
			return false;
		}
		if(!hash_pertenece(clinica->especialidades,actual->especialidad)){
			especialidad_t* especialidad = especialidad_crear(actual->especialidad);
			if (!hash_guardar(clinica->especialidades, actual->especialidad, especialidad)){

				lista_iter_destruir(lista_doctores_iter);
				return false;
			}
		}

		lista_iter_avanzar(lista_doctores_iter);
	}
	lista_iter_destruir(lista_doctores_iter);
	return true;

}


//=================================================================================//
//Ya esta en el .h
void pedir_turno (clinica_t * clinica, char * nom_paciente, char*especialidad, bool urgente){
	especialidad_t * especialidad_struct = hash_obtener(clinica->especialidades,especialidad);
	paciente_t * paciente = hash_obtener(clinica->pacientes, nom_paciente);
	if (urgente){
		cola_encolar(especialidad_struct->cola_urgente,paciente);
		especialidad_struct->cantidad++;
	}
	else{
		heap_encolar(especialidad_struct->cola_normal,paciente);
		especialidad_struct->cantidad++;

	}
	printf(CANT_PACIENTES_ENCOLADOS,especialidad_struct->cantidad,especialidad);

}

//=================================================================================//

/*
 *Pre: Recibe una especialidad. Previamente inicializada
 *Post: Devuelve el siguiente paciente que debe ser atendido (priorizando a los urgentes por sobre los regulares).
 * 
 */
paciente_t * paciente_a_atender(especialidad_t* especialidad){
	if(!cola_esta_vacia(especialidad->cola_urgente)){
		paciente_t* paciente_urgente = cola_desencolar(especialidad->cola_urgente);
		
		return paciente_urgente;
	}
	paciente_t* paciente_normal = heap_desencolar(especialidad->cola_normal);
	return paciente_normal;

}
//Ya esta en el .h
void atender(clinica_t * clinica, char* doctor_nombre){
	doctor_t *doctor = abb_obtener(clinica->doctores, doctor_nombre);
	especialidad_t* especialidad = hash_obtener(clinica->especialidades,doctor->especialidad);
	
	if(especialidad->cantidad == 0){
		printf(SIN_PACIENTES);
		return;
	}
	paciente_t * paciente = paciente_a_atender(especialidad);
	especialidad->cantidad--;

	doctor->atendidos++;
	printf(PACIENTE_ATENDIDO, paciente->nombre);
    printf(CANT_PACIENTES_ENCOLADOS, especialidad->cantidad ,doctor->especialidad);

}


//=================================================================================//
typedef struct salida{
	char * nombre;
	char* especialidad;
	size_t atendidos;
}salida_t;

typedef struct extra{
	char** parametros;
	size_t contador;
	salida_t * salida;
	size_t tope;
}extra_t;

/*
 *Pre: Recibe una clave, un doctor y un dato del tipo extra_t. Previamente inicializados
 *Post: Imprime los doctores que se encuentran dentro del rango especificado
 * (Menor al parametro 2 de extra)
 */
bool visitar_imprimiendo_desde_comienzo_(const char* clave, doctor_t * dato, extra_t * extra){
	if (strcmp(clave, (char*)extra->parametros[1]) <= 0){
			extra->salida[extra->tope].nombre = dato->nombre;
			extra->salida[extra->tope].especialidad = dato->especialidad;
			extra->salida[extra->tope].atendidos = dato->atendidos;
			extra->tope ++;
			(size_t)extra->contador++;
		}
		return true;
}
/*
 *Pre: Recibe una clave, un doctor y un dato del tipo extra_t. Previamente inicializados
 */
bool visitar_imprimiendo_desde_comienzo(const char* clave, void * dato, void * extra){
	return visitar_imprimiendo_desde_comienzo_(clave, (doctor_t*)dato, (extra_t*)extra);
}
/*
 *Pre: Recibe una clave, un doctor y un dato del tipo extra_t. Previamente inicializados
 *Post: Imprime los doctores que se encuentran dentro del rango especificado
 * (Mayor al parametro 1 de extra)
 */
bool visitar_imprimiendo_desde_un_punto_(const char* clave, doctor_t * dato, extra_t * extra){

	if (strcmp(clave, (char*)extra->parametros[0]) >= 0 ){
			extra->salida[extra->tope].nombre = dato->nombre;
			extra->salida[extra->tope].especialidad = dato->especialidad;
			extra->salida[extra->tope].atendidos = dato->atendidos;			
				extra->tope ++;
			(size_t)extra->contador++;
		}
		return true;
}
/*
 *Pre: Recibe una clave, un doctor y un dato del tipo extra_t. Previamente inicializados
 */
bool visitar_imprimiendo_desde_un_punto(const char* clave, void * dato, void * extra){
	return visitar_imprimiendo_desde_un_punto_(clave, (doctor_t*)dato, (extra_t*)extra );
}

/*
 *Pre: Recibe una clave, un doctor y un dato del tipo extra_t. Previamente inicializados
 *Post: Imprime los doctores que se encuentran dentro del rango especificado
 * (Mayor al parametro 1, menor al parametro 2 de extra)
 */
bool visitar_imprimiendo_rango_delimitado_(const char* clave, doctor_t * dato, extra_t * extra){
	if (strcmp(clave, (char*)extra->parametros[0]) >= 0 ){
		if (strcmp(clave, (char*)extra->parametros[1]) <= 0){
			extra->salida[extra->tope].nombre = dato->nombre;
			extra->salida[extra->tope].especialidad = dato->especialidad;
			extra->salida[extra->tope].atendidos = dato->atendidos;
			extra->tope ++;
			(size_t)extra->contador++;
		}
	}
	return true;
}
/*
 *Pre: Recibe una clave, un doctor y un dato del tipo extra_t. Previamente inicializados
 */
bool visitar_imprimiendo_rango_delimitado(const char* clave, void * dato, void * extra){
	return visitar_imprimiendo_rango_delimitado_(clave, (doctor_t*)dato, (extra_t*) extra);
}

//Ya esta en el .h
void crear_informe(clinica_t * clinica, bool entero, char **parametros){
	if (entero == true){
		printf(DOCTORES_SISTEMA, abb_cantidad(clinica->doctores));
		if ( abb_cantidad(clinica->doctores) != 0){
			abb_iter_t * iterador = abb_iter_in_crear(clinica->doctores);
			if (!iterador){
			printf("Ocurrio un error a la hora de crear el informe, intentelo de nuevo\n");
			return;
			}
			size_t i = 1;
			while (!abb_iter_in_al_final(iterador)){
				const char * doctor_nombre = abb_iter_in_ver_actual(iterador);
				doctor_t * doctor = abb_obtener(clinica->doctores,doctor_nombre);
				printf(INFORME_DOCTOR, i, doctor->nombre, doctor->especialidad, doctor->atendidos);
				i++;
				abb_iter_in_avanzar(iterador);
			}
			abb_iter_in_destruir(iterador);
		}
			return;
	}
		
	extra_t * extra = malloc(sizeof(extra_t));
	extra->parametros = parametros;
	extra->contador = 1;
	extra->tope = 0;
	extra->salida = calloc(1 + abb_cantidad(clinica->doctores),sizeof(salida_t));

	if (strcmp(parametros[0], COMANDO_VACIO) == 0){
		abb_in_order(clinica->doctores, visitar_imprimiendo_desde_comienzo, extra);
	}
	else if (strcmp(parametros[1], COMANDO_VACIO) == 0){
		abb_in_order(clinica->doctores, visitar_imprimiendo_desde_un_punto, extra);
	}
	else{
		abb_in_order(clinica->doctores, visitar_imprimiendo_rango_delimitado, extra);
	}
	printf(DOCTORES_SISTEMA, extra->tope);
	for (int i = 0; i < extra->tope; i++){
		printf(INFORME_DOCTOR,(size_t) (i + 1), extra->salida[i].nombre, extra->salida[i].especialidad, extra->salida[i].atendidos);
	}
	free(extra->salida);
	free(extra);
	return;
	}
//=================================================================================//


