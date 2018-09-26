#include <iostream>
#include <unistd.h>
#include "Server/ChatServer.h"
#include "Client/ChatClient.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Please start the program in --client or --server mode." << endl;
        return 1;
    } else if (strcmp(argv[1], "--client") == 0) {
        return ChatClient().start();
    } else if (strcmp(argv[1], "--server") == 0) {
        return ChatServer().start();
    } else {
        cerr << "Invalid option." << endl;
        return 2;
    }
    return 0;
}
