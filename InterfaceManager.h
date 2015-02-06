#ifndef InterfaceManagerH
#define InterfaceManagerH

#include <ifaddrs.h>
#include <string>
#include <vector>

using namespace std;

struct IfaceInfo
{
    string name;
    int type;
    bool addrexist;
    unsigned char mac[6];
    
    bool operator == (const IfaceInfo &r) const {
		return this->name == r.name;
	}

	bool operator != (const IfaceInfo &r) const {
		return this->name != r.name;
	}
};



typedef vector<IfaceInfo> Ifaces;

class InterfaceManager{

public:
    InterfaceManager();
    ~InterfaceManager();
    void printIface(IfaceInfo);
    Ifaces getIfaces();
	void update();
	Ifaces newvec;
	Ifaces gonevec;
	
private:
    ifaddrs *ifaddr;
    void getDiff(Ifaces,Ifaces);

};


#endif