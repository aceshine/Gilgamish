#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/event.h>


static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


static void server_init()
{
	
}


static void* server(void* argv)
{
	pthread_mutex_lock(&mtx);
	sleep(1);
	printf("server start\n");



	pthread_mutex_unlock(&mtx);
	return NULL;
}

static void* client(void* argv)
{
	pthread_mutex_lock(&mtx);
	printf("client start\n");
	pthread_mutex_unlock(&mtx);
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t server_id;
	pthread_t client_id;
	pthread_create(&server_id, NULL, server, NULL);
	pthread_create(&client_id, NULL, client, NULL);

	void* result;
	pthread_join(server_id, &result);
	return 0;
}