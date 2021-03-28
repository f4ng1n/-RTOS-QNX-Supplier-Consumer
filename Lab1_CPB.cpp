/*
 * LAB 1- СРВ_Фан Тхи Хай Ха_6362
 * Задание 1: Задача «Поставщик-Потребитель»
 *
 * Два процесса (две нитки) работают с некоторым Буфером.
 * Процесс «Поставщик» в случайные моменты времени записывает в Буфер
 * «пакеты информации», процесс «Потребитель» считывает информацию из Буфера.
 *
 * Построить модель работы «Поставщика» и «Потребителя» при следующих условиях:
 * 	1. «Поставщик» не может записывать информацию и блокируется, если Буфер полон.
 * 	2. «Потребитель» Блокируется, если Буфер пуст.
 * 	3. Не допускается одновременная запись и чтение информации.
 *
 * ----------------------------------------------------------------------
 *
 * 	Задание следует реализовать двумя способами:
 * 		а) с использованием трех семафоров
 * 		б) с использованием условной переменной
 *
 * ----------------------------------------------------------------------
*/

#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <map>
#include "Buffer.h"
#include <sys/time.h>
using namespace std;

enum speedMode { READER_FASTER, WRITER_FASTER };
enum workMode { SEMAPHORES, CONDITION };
map<string,workMode>workModes;

void WorkModes(){
	workModes["SEMAPHORES"]=SEMAPHORES;
	workModes["CONDITION"]=CONDITION;
}

//Semaphores
sem_t _sem_lock;
sem_t _sem_empty;
sem_t _sem_full;

//Condition variables
pthread_cond_t _condition;
//Mutex
pthread_mutex_t _mutex;

//Speed mode
speedMode mode;

//Writer and Reader threads
pthread_t writerThread, readerThread;

//Buffer
Buffer* buff;
void* ConVar_WriterFunc(void* arg);
void* ConVar_ReaderFunc(void* arg);
int Buffer=0;

//Semaphore functions
void* Sem_WriterFunc(void* arg);
void* Sem_ReaderFunc(void* arg);

void threadSleep(){
	if ((pthread_self() == readerThread && mode == READER_FASTER) || (pthread_self() == writerThread && mode == WRITER_FASTER)) {
		sleep (1);
	} else {
		sleep(3);
	}
}

struct timeval currentTime_1,currentTime_2,currentTime_11,currentTime_22;
time_t oldTime_Consumer, newTime_Supplier,oldTime_Supplier,newTime_Consumer;

void Menu(){
	printf("LAB 1. SUPPLIER-CONSUMER\n");
	printf("1.Using SEMAPHORES-Thread Functions\n (SEMAPHORES)\n");
	printf("2.Using CONDITIONAL VARIABLES Thread-Functions (CONDITION) \n");
}

void* Consumer (void* args)
{
	for (int i = 0;i < 15;i++)
	{
		sem_wait(&_sem_empty);
		sem_wait(&_sem_lock);
		gettimeofday(&currentTime_1, NULL);
		oldTime_Consumer = time(NULL);
		Buffer--;
		gettimeofday(&currentTime_2, NULL);
		newTime_Supplier = time(NULL);
		printf("\nBegin %s %ld mc\n",ctime(&oldTime_Consumer),currentTime_1.tv_usec);
		printf("-> Consumer: -1 = %d\n",Buffer);
		printf("End %s %ld mc\n",ctime(&newTime_Supplier),currentTime_2.tv_usec);
		fflush(stdout);
		sem_post(&_sem_lock);
		sem_post(&_sem_full);
		usleep (300);
	}
	return EXIT_SUCCESS;
}

void* Supplier (void* args)
{
	for (int i = 0;i < 25;i++)
	{
		sem_wait(&_sem_full);
		sem_wait(&_sem_lock);
		//gettimeofday(&currentTime_11, NULL);
		oldTime_Supplier = time(NULL);
		Buffer++;
		gettimeofday(&currentTime_22, NULL);
		newTime_Supplier = time(NULL);
		printf("\nBegin %s %ld mc\n",ctime(&oldTime_Supplier),currentTime_11.tv_usec);
		printf(">Supplier: +1 = %d\n",Buffer);
		printf("End %s %ld mc\n",ctime(&newTime_Supplier),currentTime_22.tv_usec);
		fflush(stdout);
		sem_post(&_sem_lock);
		sem_post(&_sem_empty);
		usleep (300);
	}
	return EXIT_SUCCESS;
}
int main(void) {
	Menu();
	WorkModes();
	printf("\nChoose work mode -> ");
	std::string input;
	scanf ("%d",input);
//	//Buffer size
//	buff = new Buffer(6);
	//Initializing
	switch (workModes[input]) {
//	case SEMAPHORES:
//		pthread_t thread_Cons,thread_Sup;
//
//		sem_init(&_sem_lock,NULL,1);
//		sem_init(&_sem_empty,NULL,0);
//		sem_init(&_sem_full,NULL,15);
//
//		pthread_create(&thread_Cons,NULL,&Consumer,NULL);
//		pthread_create(&thread_Sup,NULL,&Supplier,NULL);
//		sem_destroy(&_sem_empty);
//		sem_destroy(&_sem_full);
//		sem_destroy(&_sem_lock);
//		pthread_join(thread_Cons,NULL);
//		pthread_join(thread_Sup,NULL);
//		return EXIT_SUCCESS;
//		break;
	case CONDITION:
//		work = CONDITION;
		pthread_cond_init(&_condition,NULL);
		pthread_mutex_init(&_mutex,NULL);

		pthread_create(&writerThread, NULL,&ConVar_WriterFunc,NULL);
		pthread_create(&readerThread, NULL,&ConVar_ReaderFunc,NULL);
		break;
	default:
		throw invalid_argument("Invalid Work Mode!");
	}
	//Joining threads and waiting them to end
	pthread_join(writerThread,NULL);
	pthread_join(readerThread,NULL);
	return 0;
}

//ZADANIE 1 - Semaphores thread_functions
//void* Sem_WriterFunc(void* arg){
//	int counter = 0;
//	while(true){
//		sem_wait(&_sem_lock);
//	if (buff->IsFull()){
//		printf("Buffer full! Supplier is waiting...!\n");
//		sem_post(&_sem_lock);//lock-semaphore
//		sem_wait(&_sem_full); //waiting full-semaphore to resolve
//		sem_wait(&_sem_lock);//after resolving, unlock
//	printf("Supplier is unlocked and active! \n");
//	}
//	if (buff->IsEmpty() && counter > 0){
//		sem_post(&_sem_empty);
//	}
//	printf("Start writing to Buffer: '%d' \n",counter);
//	buff->Write(counter++);
//	buff->Print();
//	sem_post(&_sem_lock);
//	threadSleep();
//	}
//	return 0;
//}

//
//void* Sem_ReaderFunc(void* arg){
//	while(true){
//		sem_wait(&_sem_lock);
//	if (buff->IsEmpty()){
//		printf("Buffer is empty.Consumer is waiting...! \n");
//		sem_post(&_sem_lock);
//		sem_wait(&_sem_empty);
//		sem_wait(&_sem_lock);
//		printf("Consumer is unlocked and active! \n");
//	}
//	if (buff->IsFull()){
//		sem_post(&_sem_full);
//	}
//	printf("Start reading from Buffer: '%d' \n",buff->Read());
//	buff->Print();
//	sem_post(&_sem_lock);
//	threadSleep();
//	}
//	return 0;
//}


//ZADANIE 2 - Conditional variables Functions

void* ConVar_WriterFunc(void* arg){
	int counter = 0;
	while(true){
		pthread_mutex_lock(&_mutex);
	if (buff->IsFull()){
		printf("Buffer full! Supplier is waiting...!\n");
		pthread_cond_wait(&_condition,&_mutex);
		printf("Supplier is active! \n");
	}
	if (buff->IsEmpty()){
		pthread_cond_signal(&_condition);
	}
	printf("Start writing to Buffer: '%d' \n",counter);
	buff->Write(counter++);
	buff->Print();
	pthread_mutex_unlock(&_mutex);
	threadSleep();
	}
	return 0;

}

void* ConVar_ReaderFunc(void* arg){
	while (true){
		pthread_mutex_lock(&_mutex);
	if (buff->IsEmpty()){
		printf("Buffer is empty! Consumer is waiting...! \n");
		pthread_cond_wait(&_condition,&_mutex);
		printf("Consumer is unlocked and active! \n");
	}
	if (buff->IsFull()){
		pthread_cond_signal(&_condition);
	}
	printf("Start reading from Buffer: '%d' \n",buff->Read());
	buff->Print();
	pthread_mutex_unlock(&_mutex);
	threadSleep();
	}
	return 0;
}
