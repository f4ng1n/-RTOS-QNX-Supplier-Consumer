/*
 * LAB 1- СРВ_Фан Тхи Хай Ха_6362
 * Задание 1: Задача «Поставщик-Потребитель»
 *
 * Два процесса (две нитки) работают с некоторым Буфером.
 * Процесс «Поставщик» в случайные моменты времени записывает в Буфер
 * «пакеты информации», процесс «Потребитель» считывает информацию из Буфера.
 *
 * Построить модель работы «Поставщика» и «Потребителя» при следующих условиях:
 * 	a. «Поставщик» не может записывать информацию и блокируется, если Буфер полон.
 * 	b. «Потребитель» Блокируется, если Буфер пуст.
 * 	c. Не допускается одновременная запись и чтение информации.
 *
 * ----------------------------------------------------------------------
 *
 * 	Задание следует реализовать  способом: с использованием трех семафоров. Фиксируя значения текущего времени при начале и окончании работы в «критической секции» поставщика и Потребителя, показать выполнение условия 1с.
 *
 *  Задание 2: С использованием условной переменной
 * ----------------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/time.h>



//3 Semaphores
sem_t sem_lock; //для захватывания общего ресурса только одним потоком
sem_t sem_empty; //buffer=0 (min)
sem_t sem_full;// buffer=15 (max)

//Time
struct timeval CurrTime1, CurrTime2, CurrTime11, CurrTime22;
time_t oldTime_Consumer, oldTime_Supplier, newTime_Consumer, newTime_Supplier;

void* Consumer(void* arg);
void* Supplier(void* arg);
void* ConVar_Consumer(void* arg);
void* ConVar_Supplier(void* arg);
//---ZADAIE 1---------
int Buffer=0;

void* Consumer(void* arg){
	for (int i=0;i<25;i++){
		sem_wait(&sem_empty); // «Потребитель» Блокируется, если Буфер пуст.
		sem_wait(&sem_lock);

		gettimeofday(&CurrTime1,NULL);
		oldTime_Consumer = time(NULL);

		Buffer--;

		gettimeofday(&CurrTime2,NULL);
		newTime_Consumer = time(NULL);

		printf("\n-->Run: %s %ld (ms)\n",ctime(&oldTime_Consumer),CurrTime1.tv_usec);
		printf("Consumer: -1 -> Buffer = %d\n",Buffer);
		printf("-->Stop: %s %ld (ms)\n",ctime(&newTime_Consumer),CurrTime2.tv_usec);
		fflush(stdout);

		sem_post(&sem_lock);
		sem_post(&sem_full);
		usleep(300);

	}
	return EXIT_SUCCESS;
}

void* Supplier(void* arg){
	for (int i=0;i<45;i++) {
		sem_wait(&sem_full);
		sem_wait(&sem_lock);

		gettimeofday(&CurrTime11,NULL);
		oldTime_Supplier = time(NULL);

		Buffer++;

		gettimeofday(&CurrTime22,NULL);
		newTime_Supplier = time(NULL);

		printf("\n-->Run: %s %ld (ms)\n",ctime(&oldTime_Supplier),CurrTime11.tv_usec);
		printf("Supplier: +1 -> Buffer = %d\n",Buffer);
		printf("-->Stop: %s %ld (ms)\n",ctime(&newTime_Supplier),CurrTime22.tv_usec);
		fflush(stdout);

		sem_post(&sem_lock);
		sem_post(&sem_empty);
		usleep(300);
	}
	return EXIT_SUCCESS;
}

//------ZADANIE 2-------------
int BuffEmpty=1, BuffFull=0;

pthread_cond_t CondFull,CondEmpty;
pthread_mutex_t mutex;

struct timeval CurrTimeCV1, CurrTimeCV2, CurrTimeCV11, CurrTimeCV22;
time_t oldTimeCV_Consumer, oldTimeCV_Supplier, newTimeCV_Consumer, newTimeCV_Supplier;

void* ConVar_Consumer(void* arg){
	while(1){
		pthread_mutex_lock(&mutex);

		gettimeofday(&CurrTimeCV1,NULL);
		oldTimeCV_Consumer = time(NULL);

		while(BuffEmpty){
			pthread_cond_wait(&CondEmpty,&mutex);
		}
		Buffer--;

		if (Buffer == 0){
			BuffEmpty=1;
		}

		gettimeofday(&CurrTimeCV2,NULL);
		newTimeCV_Consumer = time(NULL);

		printf("-->Run:%s %ld (ms)\n",ctime(&oldTimeCV_Consumer),CurrTimeCV1.tv_usec);
		printf("Consumer: -1 -> Buffer = %d\n",Buffer);
		printf("-->Stop:%s %ld (ms)\n",ctime(&newTimeCV_Consumer),CurrTimeCV2.tv_usec);

		fflush(stdout);

		BuffFull = 0;

		pthread_cond_signal(&CondFull);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

void* ConVar_Supplier(void* arg){
	while(1){
		pthread_mutex_lock(&mutex);

		gettimeofday(&CurrTimeCV11,NULL);
		oldTimeCV_Supplier = time(NULL);
		while(BuffFull){
			pthread_cond_wait(&CondFull,&mutex);
		}

		Buffer++;
		if(Buffer == 15){
			BuffFull = 1;
		}

		gettimeofday(&CurrTimeCV22,NULL);
		newTimeCV_Supplier = time(NULL);

		printf("\n-->Run:%s %ld (ms)\n",ctime(&oldTimeCV_Supplier),CurrTimeCV11.tv_usec);
		printf("Supplier: +1 -> Buffer = %d\n",Buffer);
		printf("-->Stop%s %ld (ms)\n",ctime(&newTimeCV_Supplier),CurrTimeCV22.tv_usec);
		fflush(stdout);

		BuffEmpty = 0;

		pthread_cond_signal(&CondEmpty);
		pthread_mutex_unlock(&mutex);
	}
}

void Menu(){
	printf("LAB 1. SUPPLIER-CONSUMER\n");
	printf("1. Using SEMAPHOREs - Thread functions \n");
	printf("2. Using CONDITIONAL VARIABLES Thread-Functions\n");
}

int main(int argc, char *argv[]){
	Menu();
	int input;
	printf("\nChoose work mode -> "); scanf("%d",&input);
	switch(input){
	case 1:
		fflush(stdout);
		pthread_t thread_Consumer, thread_Supplier;
		printf("Semaphores:\n");
		sem_init(&sem_empty,'\0',0);
		sem_init(&sem_full,'\0',15);
		sem_init(&sem_lock,'\0',1);

		pthread_create(&thread_Consumer,NULL,&Consumer,NULL);
		pthread_create(&thread_Supplier,NULL,&Supplier,NULL);

		sem_destroy(&sem_empty);
		sem_destroy(&sem_full);
		sem_destroy(&sem_lock);

		pthread_join(thread_Consumer,NULL);
		pthread_join(thread_Supplier,NULL);
		return EXIT_SUCCESS;
		break;

	case 2:
		fflush(stdout);
		pthread_t thread_Cond_Consumer, thread_Cond_Supplier;
		printf("\nConditional Variables:\n");

		pthread_cond_init(&CondFull, NULL);
		pthread_cond_init(&CondEmpty,NULL);

		pthread_mutex_init(&mutex,NULL);

		pthread_create(&thread_Cond_Consumer,NULL,&ConVar_Consumer,NULL);
		pthread_create(&thread_Cond_Supplier,NULL,&ConVar_Supplier,NULL);

		pthread_join(thread_Cond_Consumer,NULL);
		pthread_join(thread_Cond_Supplier,NULL);

		pthread_cond_destroy(&CondEmpty);
		pthread_cond_destroy(&CondFull);

		pthread_mutex_destroy(&mutex);
		return EXIT_SUCCESS;
		break;
	default:
		printf("\nWrong option! Again!\n");
		printf("\nChoose work mode -> "); scanf("%d",&input);
			Menu();
	}



}
