#include "abb.h"
#include "lista.h"
#include "hash.h"
#include "cola.h"
#include "heap.h"
#include <stdbool.h>

typedef struct clinica clinica_t;

typedef struct doctor doctor_t;

typedef struct paciente paciente_t;

typedef struct especialidad especialidad_t;

//=================================================================================//

/*
 *Post: Crea una clinica asignando su memoria, devuelve un puntero a ella
 * Si falla, devuelve NULL
 */
clinica_t *clinica_crear();

 /*
 *Pre: Debe recibir una clinica previamente inicializada y un doctor que se encuentre en el sistema
 *Atiende al siguiente paciente del doctor, si lo hay, segun su especialidad, 
 */
void atender(clinica_t * clinica,char* doctor);

  /*
 *Pre: Recibe una clinica previamente inicializada, un paciente y una especialidad validos y un booleano 
 * que representa el valor de la urgencia
 * Pide turno para el paciente recibido, encolandolo en la especialidad 
 */
void pedir_turno (clinica_t * clinica, char * nom_paciente, char*especialidad, bool urgente);

/*
 *Pre: Recibe una clinica previamente inicializada, un booleano y dos strings
 *  Imprime un listado de los doctores, dependiendo del estado del booleano
 * Imprimira todo el listado o una parte de este
 */
void crear_informe(clinica_t * clinica, bool entero, char **parametros);	


/*
 *Pre: Recibe una clinica previamente inicializada, un nombre y un parametro que tiene que 
 * ser cualquiera de estos tres : "ESPECIALIDAD","PACIENTE", "DOCTOR"
 *Post: Devuelve verdadero si el personaje existe en el ambito deseado, si el ambito es incorrecto
 * o el personaje no existe, devuelve false
 */
bool clinica_pertenece(clinica_t * clinica, char * persona, char * ambito);


/*
 *Pre: Recibe una clinica previamente inicializada
 *Post: Devuelve toda la memoria asignada a la clinica y sus pertenecientes
 */
void clinica_destruir(clinica_t *clinica);
//=================================================================================//

/*
 *Pre: Recibe una linea de archivo y un puntero extra
 *Post: Asigna memoria para el doctor, lo inicializa y lo devuelve. Si falla devuelve NULL.
 */
void *creador_doctor(char**linea, void* extra);

/*
 *Pre: Recibe una linea de archivo y un puntero extra
 *Post: Asigna memoria para el paciente, lo inicializa y lo devuelve. Si falla devuelve NULL.
 */
void *creador_paciente(char**linea, void* extra);

/*
 *Pre: Recibe un hash vacio de pacientes y la lista de estos.
 *Post: Devuelve true si todos los pacientrs fueron transferidos al hash con exito, de lo contrario devuelve false.
 *la claves del hash es los nombres de los pacientes.
 */
bool parsear_pacientes(clinica_t *, lista_t* lista_pacientes);

/*
 *Pre: Recibe un abb vacio de doctores y la lista de estos.
 *Post: Devuelve true si todos los doctores fueron transferidos al abb con exito, de lo contrario devuelve false.
 *las claves del abb son los nombres de los doctores.
 */
bool parsear_doctores(clinica_t *, lista_t* lista_doctores);

/*
 *Pre: Recibe un hash vacio de especialidades y la lista de doctores.
 *Post: Devuelve true si todas las especialidades fueron transferidos al hash con exito, de lo contrario devuelve false.
 *las claves del hash son los nombres de las especialides. No hay claves repetidas.
 */
bool parsear_especialidades(clinica_t * clinica, lista_t* lista_doctores);
