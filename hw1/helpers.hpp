#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <filesystem>
#include <string>
#include <optional>
using namespace std;
#include "helpers.cpp"

struct Methods {
	bool help = false;
	bool tcp = false;
	bool udp = false;
};

void render_res(Connection* connection, string type, string filter);
void render_res_v6(ConnectionV6* connection, string type, string filter);