#include "ThreadPool.h"
#include "nethead.h"
#include "UdpServer.h"
#include "Configure.h"
#include "Log.h"
#include "Index.h"
#include "Diction.h"

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char const *argv[])
{

    // 把当前进程变为守护进程
    if (daemon(1, 1))
    {
        WRITE_STR(string(" turn main to deamon error"));
        throw runtime_error("daemon");
    }

#ifdef BUILD_INDEX
    Index *p_index = Index::getInstance();
    p_index->buildIndexFromDiction();
#endif
    Diction::getInstance();
    // config
    Configure *conf = Configure::getInstance();
    string mtn = conf->getConfigByName("max_thread_num");
    int max_thread_num = atoi(mtn.c_str());

    ThreadPool pool(max_thread_num);

    string sport = conf->getConfigByName("port");
    istringstream ss(sport);
    short port = 0;
    if (!(ss >> port))
    {
        WRITE_STR(string("trans str-port to int-port error"));
        throw runtime_error("sstream");
    }

    // prepair
    Address server_addr(htonl(INADDR_ANY), htons(port), AF_INET);
    Socket socket("udp");
    //bind
    socket.bindToAddess(server_addr);
    //start server;
    UdpServer server(socket, server_addr, pool);
    server.start();
}
