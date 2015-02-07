#include "InterfaceManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netdb.h>
#include <net/if_arp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>



using namespace std;

InterfaceManager::InterfaceManager(){
	update();
	
	
}


InterfaceManager::~InterfaceManager(){
	if(ifaddr)
		freeifaddrs(ifaddr);
	
}



Ifaces InterfaceManager::getIfaces(){
    
    ifaddrs *ifa;
    
    if(getifaddrs(&ifaddr) == -1)
		exit(EXIT_FAILURE);
    
    
    Ifaces ifvec; //result

    for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
		IfaceInfo iface_info;
		if(ifa->ifa_addr->sa_family == AF_PACKET){	
	    
			//iface name
		    iface_info.name = ifa->ifa_name;
		    
			//iface type
			iface_info.type = -1;
			string line;
			string path = "//sys//class//net//";
			path += ifa->ifa_name;
			path += "//type";
			ifstream file (path.c_str());
			if(file.is_open()){
			    getline(file, line);
			    iface_info.type = atoi(line.c_str());
			}
			
		    //get mac addr
		    iface_info.addrexist = false;
		    sockaddr_ll *s = (sockaddr_ll*)ifa->ifa_addr;
		    if(!(ifa->ifa_flags & IFF_NOARP) &&
			    	!(ifa->ifa_flags & IFF_LOOPBACK)){
				iface_info.addrexist = true;
			    for(int i = 0; i < 6; i++)
				    iface_info.mac[i] = s->sll_addr[i];

			}
		    
		    ifvec.push_back(iface_info);
		}
    }
    
    
    freeifaddrs(ifaddr);
    ifaddr = NULL;	
    
    return ifvec;

}


void InterfaceManager::printIface(IfaceInfo iface_info){
	printf("%s ", iface_info.name.c_str());

	switch(iface_info.type){
	    case ARPHRD_ETHER:
	    printf("Ethernet ");
	    break;
			    
	    case ARPHRD_LOOPBACK:
	    printf("Loopback ");
	    break;
	
	    case ARPHRD_TUNNEL:
	    printf("Tunnel ");
	    break;
	
	    case ARPHRD_TUNNEL6:
	    printf("Tunnel ");
	    break;
			    
	    case ARPHRD_IPDDP:
	    printf("Tunnel ");
	    break;
	
	    case -1:
	    printf("Error ");
	    break;
	
	    default:
	    printf("Other (%d) ", iface_info.type);
	    break;
	}
	
	if(iface_info.addrexist)
		for(int i = 0; i < 6; i++)
			printf(i < 5 ? "%x:" : "%x", iface_info.mac[i]);
	
	printf("\n");
	
}
Ifaces ifvec;
void InterfaceManager::update(){
	Ifaces ifvec_curr = getIfaces();
	newvec.clear();
	gonevec.clear();
	getDiff(ifvec, ifvec_curr);

	ifvec = ifvec_curr;
	
	
	
}

void InterfaceManager::getDiff(Ifaces lhs, Ifaces rhs){
	for(Ifaces::iterator itr = rhs.begin(); itr != rhs.end(); ++itr){
		bool isFined = false;
		for(Ifaces::iterator itl = lhs.begin(); itl != lhs.end(); ++itl){
			if(*itl == *itr){
				isFined = true;
				break;
			}
		}
		if(!isFined)
			newvec.push_back(*itr);
	}

	for(Ifaces::iterator itl = lhs.begin(); itl != lhs.end(); ++itl){
		bool isFined = false;
		for(Ifaces::iterator itr = rhs.begin(); itr != rhs.end(); ++itr){
			if(*itl == *itr){
				isFined = true;
				break;	
			}
		}
		if(!isFined)
			gonevec.push_back(*itl);
	}

	
}
