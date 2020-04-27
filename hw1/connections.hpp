#include <string>
#include <vector>
#include <optional>
#include <bits/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct Connection {
	struct ::in_addr tmp_local_addr;
	std::string local_addr;
	int local_port;
	struct ::in_addr tmp_remote_addr;
	std::string remote_addr;
	int remote_port;
	int pid;
	std::string program;
};


struct ConnectionV6 {
	struct ::in6_addr tmp_local_addr;
	std::string local_addr;
	int local_port;
	struct ::in6_addr tmp_remote_addr;
	std::string remote_addr;
	int remote_port;
	int pid;
	std::string program;
};

