
#include "Header.h"

void Produttore(Struct*,int);
void Consumatore(Struct*,int);
void StampaInfo();

int main() {
	
	StampaInfo();
		
	pid_t pid;
	key_t Chiave_SHM = ftok ("./",'C'); // chiave della shared memory
	key_t Chiave_SEM = ftok ("./",'C'); // chiave del semaforo
	
	//Inizializzazione Shared Memory Struct
	
	int ds_shm = shmget(Chiave_SHM, sizeof(Struct), IPC_CREAT|0664);
	if(ds_shm<0) { perror("SHM errore"); exit(1); }
	Struct * c;
	c = (Struct *) shmat(ds_shm, NULL, 0); 
	
	//FINE

	//Inizializzazione Semaforo, 4 semafori
	
	int ds_sem = semget(Chiave_SEM, 4, IPC_CREAT|0664);
	if(ds_sem<0) { perror("SEM errore"); exit(1); }

	//FINE
	
	Set_Dati(c,ds_sem);
	
	for(int i=0;i<Num_Produttori;i++){
			pid = fork();
			if(pid<0){perror("Error Fork"); exit(1);}
			else if (pid==0){
			Produttore(c, ds_sem);
			exit(1);
			}
		}
	
	for(int i=0;i<Num_Consumatori;i++){
			pid = fork();
			if(pid<0){perror("Error Fork"); exit(1);}
			else if (pid==0){
			Consumatore(c, ds_sem);
			exit(1);
			}
		}


	for(int i=0; i<Num_Processi; i++) {
		wait(NULL);
	}
	
	Remove_Dati(c,ds_sem,ds_shm);
	
    return 0;
}

void Produttore(Struct* c ,int ds_sem){
	
	srand(getpid()*time(NULL));
	for(int i=0;i<Num_Produzioni;i++){
		sleep(2);
		Produzione(c,ds_sem);
	}
	
}

void Consumatore(Struct* c,int ds_sem){
	
	for(int i=0;i<Num_Consumazioni;i++){
		sleep(1);
		Consumazione(c,ds_sem);
	}	

}

void StampaInfo(){
		
	printf("\n\t\tINFO\n");
	printf("Esercizio Produttore Consumatore CodaCircolare\nScambio con SHM e controllo con Semafori\n");
	printf("<%d>Produttori  con <%d>Produzioni  <TOT=%d>\n",Num_Produttori,Num_Produzioni,Tot_Produttori);
	printf("<%d>Consumatori con <%d>Consumazioni<TOT=%d>\n",Num_Consumatori,Num_Consumazioni,Tot_Consumatori);
	printf("A ogni Consumazione, consuma <2> elementi\n");
	printf("<%d>Processi Totali\n",Tot_Produttori+Tot_Consumatori);
	printf("\n\t<AVVIO COMPILAZIONE>\n\n");
	printf("\t\t\t Array Stringa\n");
	
}
