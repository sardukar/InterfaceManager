#include <stdio.h>
#include <stdlib.h>
#include <csignal>
#include <unistd.h>
#include <sys/time.h>
#include "InterfaceManager.h"


InterfaceManager *manager;

void sigtermHandler(int signum);
void sigintHandler(int signum);

unsigned long GetTickCount(){
    timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);

}


int main(){
	signal(SIGTERM, sigtermHandler);
	signal(SIGINT, sigintHandler);
	
    manager = new InterfaceManager();
	
    
    int cnt = 0;
    printf("-----------------------------------------------\n");
    
    unsigned long _start = GetTickCount() - 5000;

    while(1){
    	
    	manager->update();
		
		
		for(Ifaces::iterator iter = manager->newvec.begin(); iter != manager->newvec.end(); ++iter){
		  	printf("NEW ");
		   	manager->printIface(*iter);
		   	printf("-----------------------------------------------\n");
	    }
	    for(Ifaces::iterator iter = manager->gonevec.begin(); iter != manager->gonevec.end(); ++iter){
		  	printf("GONE %s \n", (*iter).name.c_str());
		  	printf("-----------------------------------------------\n");
		   	
	    }
    	
    	if(GetTickCount() - _start > 5000){
        	Ifaces ifvec = manager->getIfaces();
		for(Ifaces::iterator iter = ifvec.begin(); iter != ifvec.end(); ++iter){
	        	printf("IFACE ");
			manager->printIface(*iter);
        	}
        	printf("-----------------------------------------------\n");
		//sleep(5);
	    _start = GetTickCount();
	}
    	usleep(10000);
    	
    }

    return 0;
}

void _terminate(){
	delete manager;
	
}

void sigtermHandler(int signum){
	_terminate();
	exit(signum);
}

void sigintHandler(int signum){
	_terminate();
	exit(signum);
}
