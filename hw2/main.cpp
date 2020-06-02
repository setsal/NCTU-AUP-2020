/**
 * 
 * @file main.cpp
 *
 * @brief Main interface for libiary inject
 *
 * @author setsal Lan
 * Contact: setsal.cs08g@nctu.edu.tw
 *
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <dlfcn.h>
using namespace std;

void usage();

int main (int argc, char* argv[]) {

    char buf[1024];

    // Default libiary path
    string libiary_path = "LD_PRELOAD=./sandbox.so";
    string base_dir = "WORKDIR=" + string(realpath("./",buf));

    
    const char* const optstring = "p:d:-";
    
    
    const option longopts[] = {
        { "path", 1, nullptr, 'p' },
		{ "basedir", 1, nullptr, 'd' },
		{ nullptr, 0, nullptr, 0 }
    };

    while (true) {
        
        const int opt = getopt_long(argc, argv, optstring, longopts, NULL);
        
        if ( opt == -1 ) {
            break;
        }
        switch (opt) {
            case 'p':
                // set shared libiary path
                libiary_path = "LD_PRELOAD=" + string(realpath(optarg,buf));
                cout << "[Info] custom set libiary path to " << libiary_path << endl;
                break;
            case 'd':
                // base dir allow to access
                base_dir = "WORKDIR=" + string(realpath(optarg,buf));
                cout << "[Info] custom set work dir to " << base_dir << endl;
                break;
            case '-':
                break;
            case '?':
            default:
                usage();
                return 1;
        }
    }    

    cout << endl << "[INFO]...Running in sandbox" << endl << endl;

    char *envp[] = {(char*)base_dir.c_str(), (char*)libiary_path.c_str(), NULL};

    if ( argc > optind ) {
        //do something      
        execvpe(argv[optind], argv+optind, envp);
    } else {
        cout << "no command given." << endl;
        return -1;
    }
    
    return 0;
}  

void usage() {
    cout << "usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...]" << endl;
    cout << "\t-p: set the path to sandbox.so, default = ./sandbox.so" << endl;
    cout << "\t-d: the base directory that is allowed to access, default = ." << endl;
    cout << "\t--: separate the arguments for sandbox and for the executed command" << endl;
}