#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <getopt.h>
#include <iomanip>
#include <regex>
#include <vector>
#include <arpa/inet.h>
#include <filesystem>
#include "connections.hpp"
#include "helpers.hpp"

using namespace std;
namespace fs = std::filesystem;


void list_tcp();
void list_udp();
void read_proc(const char* path, const char* type);
void read_proc_v6(const char* path, const char* type);
void build_pid_node();
void parse_pid(int inode);
string parse_program(string path);

map<int, int> inode_to_pid;
map<int, string> pid_to_cmd;
string filter_string;


int main(int argc, char* argv[]) {
    const char* const optstring = "tu";
    struct Methods methods;

    const option longopts[] = {
        { "tcp", 0, nullptr, 't' },
		{ "udp", 0, nullptr, 'u' },
		{ nullptr, 0, nullptr, 0 }
    };

    while (true) {
        
        const int opt = getopt_long(argc, argv, optstring, longopts, NULL);
        
        if (opt == -1) {
            break;
        }
            
        switch (opt) {
            case 't':
                methods.tcp = true;
                break;
            case 'u':
                methods.udp = true;
                break;
            default:
                cout << "Usage: ./hw1 [-t|--tcp] [-u|--udp] [filter-string]" << endl;
                return 1;
        }
    }
    
    // Get filter string
    if ( optind != argc ) {
        // combine filter string in once variable with space separate
        for( int i=optind; i<argc; i++ ) {
            if( i == argc-1 ) {
                filter_string += string(argv[i]);    
            }
            else {
                filter_string += string(argv[i]) + ' ';
            }
        }
    }
    
    // build map in pid, node, program
    build_pid_node();

    if ( !methods.tcp && !methods.udp ) {
        list_tcp();
        list_udp();    
    }
    else {
        if ( methods.tcp ) list_tcp();
        if ( methods.udp )  list_udp();
    }

    return 0;
}


void list_tcp() {
	cout << "List of tcp connections:" << endl; \
	cout << setw(7) << left << "Proto" \
		 << setw(22) << left << "Local Address" \
		 << setw(22) << left << "Foreign Address" \
		 << "PID/Program name" \
		 << endl;
    read_proc("/proc/net/tcp", "tcp");
    read_proc_v6("/proc/net/tcp6", "tcp");
    cout << endl;
    return;
}


void list_udp() {
	cout << "List of udp connections:" << endl; \
	cout << setw(7) << left << "Proto" \
		 << setw(22) << left << "Local Address" \
		 << setw(22) << left << "Foreign Address" \
		 << "PID/Program name" \
		 << endl;
    read_proc("/proc/net/udp", "udp");
    read_proc_v6("/proc/net/udp6", "udp");
    cout << endl;
    return;
}


void read_proc(const char* path, const char* type) {
    ifstream fin(path);
    if(!fin) {
        cout << "Error in reading file " << path << endl;
        exit(EXIT_FAILURE);
    }
    string tmp;
    char buffer[4096];
    char tmp_addr[INET_ADDRSTRLEN];
    struct Connection* connection = new Connection();
    int inode;

    // escape the first header line
    fin.getline(buffer, sizeof(buffer));
    
    while(fin.getline(buffer, sizeof(buffer))) {
        sscanf(buffer, "%*s%x:%x%x:%x%*s%*s%*s%*s%*s%*s%d",
				&connection->tmp_local_addr.s_addr, &connection->local_port,
				&connection->tmp_remote_addr.s_addr, &connection->remote_port,
				&inode);

        inet_ntop(AF_INET, &(connection->tmp_local_addr.s_addr), tmp_addr, INET_ADDRSTRLEN);
        connection->local_addr = string(tmp_addr);
        inet_ntop(AF_INET, &(connection->tmp_remote_addr.s_addr), tmp_addr, INET_ADDRSTRLEN);
        connection->remote_addr = string(tmp_addr);
        
        connection->pid = inode_to_pid[inode];        
        connection->program = pid_to_cmd[connection->pid];
        
        if ( strcmp(type, "tcp") == 0 ) {
            render_res(connection, "tcp", filter_string);
        }
        else {
            render_res(connection, "udp", filter_string);
        }
    }
    fin.close();
}


void read_proc_v6(const char* path, const char* type) {
    ifstream fin(path);
    if(!fin) {
        cout << "Error in reading file " << path << endl;
        exit(EXIT_FAILURE);
    }
    string tmp;
    char buffer[4096];
    char tmp_addr[INET6_ADDRSTRLEN];
    struct ConnectionV6* connection = new ConnectionV6();
    int inode;

    // escape the first header line
    fin.getline(buffer, sizeof(buffer));
    
    while(fin.getline(buffer, sizeof(buffer))) {
        sscanf(buffer, "%*s%08X%08X%08X%08X:%x%08X%08X%08X%08X:%x%*s%*s%*s%*s%*s%*s%d",
				&connection->tmp_local_addr.s6_addr32[0],
                &connection->tmp_local_addr.s6_addr32[1],
                &connection->tmp_local_addr.s6_addr32[2],
                &connection->tmp_local_addr.s6_addr32[3],
                &connection->local_port,
				&connection->tmp_remote_addr.s6_addr32[0],
                &connection->tmp_remote_addr.s6_addr32[1],
                &connection->tmp_remote_addr.s6_addr32[2],
                &connection->tmp_remote_addr.s6_addr32[3],
                &connection->remote_port,
				&inode);

        inet_ntop(AF_INET6, &(connection->tmp_local_addr), tmp_addr, sizeof(INET6_ADDRSTRLEN));
        connection->local_addr = string(tmp_addr);
        inet_ntop(AF_INET6, &(connection->tmp_remote_addr), tmp_addr, sizeof(INET6_ADDRSTRLEN));
        connection->remote_addr = string(tmp_addr);
        
        connection->pid = inode_to_pid[inode];        
        connection->program = pid_to_cmd[connection->pid];

        if ( strcmp(type, "tcp") == 0 ) {
            render_res_v6(connection, "tcp6", filter_string);
        }
        else {
            render_res_v6(connection, "udp6", filter_string);
        }
    }
    fin.close();
}


void build_pid_node() {
    const filesystem::directory_options options = (
        filesystem::directory_options::follow_directory_symlink |
        filesystem::directory_options::skip_permission_denied
    );

    string fb_proc_path;
    int pid;
    regex rgx("[0-9]");
    for (auto &proc : filesystem::directory_iterator("/proc")) {
        if ( !proc.is_directory() || !regex_search(proc.path().filename().string(), rgx) ) continue;
        
        fb_proc_path = proc.path().string() + "/fd";
        pid = stoi(proc.path().filename().string()); //program pid

        // parse program
        string program_name = parse_program(proc.path().string() + "/comm");
        pid_to_cmd[pid] = program_name;

        for (auto &fd_proc : filesystem::directory_iterator(fb_proc_path, std::filesystem::directory_options(options))) {
            if ( !fd_proc.is_symlink() ) {
                continue;
            }
            // parse inode & pid
            string symlink_name = fs::read_symlink(fd_proc.path()).string();
            if ( symlink_name.compare(0, 8, "socket:[") == 0 ) {
                int inode = stoi(symlink_name.substr(8, symlink_name.size()-9));
                inode_to_pid[inode] = pid;
            }
            if ( symlink_name.compare(0, 7, "[0000]:") == 0 ) {
                int inode = stoi(symlink_name.substr(7, symlink_name.size()));
                inode_to_pid[inode] = pid;
            }
        }
    }
    return;
}


string parse_program(string path) {
    ifstream fin(path);
    string program;
    if(!fin) {
        cout << "Error in reading file " << path << endl;
        exit(EXIT_FAILURE);
    }
    getline(fin, program);
    //remove null byte
    for (auto &i : program) {
        if (i == '\0') 
            i = ' ';
    }    
    return program;
}
