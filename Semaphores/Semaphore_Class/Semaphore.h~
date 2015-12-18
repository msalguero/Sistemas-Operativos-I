

class Semaphore {
	int S;
	pthread_mutex_t mutex;

public:
	Semaphore(int s);
	void wait();
	void signal();
};

Semaphore::Semaphore(int s){
	this->S = s;
	pthread_mutex_init(&mutex, NULL);
}

void Semaphore::wait(){
	while (S <= 0);
	//pthread_mutex_lock(&mutex);
    S--;
    //pthread_mutex_unlock(&mutex);

}

void Semaphore::signal(){
    //pthread_mutex_lock(&mutex);
    S++;
    //pthread_mutex_unlock(&mutex);
}