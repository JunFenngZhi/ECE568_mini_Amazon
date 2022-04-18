#include"Server.h"

int main(){
    Ptr server = Server::get_instance();
    server->run();
    return 0;
}