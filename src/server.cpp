#include "ThreadPool.h"
#include <iostream>
#include "nethead.h"
#include "UdpServer.h"
#include <unistd.h>
#include <fstream>
#include <stdexcept>
using namespace std;

int main(int argc, char const *argv[])
{
    //把当前进程变为守护进程
	if(daemon(1,1)){
		throw runtime_error("daemon");
	}
    string ip;
    short port;

    ifstream ifs;
    ifs.open("../conf/config.dat");

    if(!(ifs>>ip>>port)){
    	throw runtime_error("read config");
    }
    cout << ip << port;
    //prepair
    Address server_addr(htonl(INADDR_ANY),htons(port),AF_INET);
    Socket socket("udp");
    //bind
    socket.bindToAddess(server_addr);
    //start server;
    ThreadPool pool(10);
    UdpServer server(socket,server_addr,pool);
    server.start();
}
