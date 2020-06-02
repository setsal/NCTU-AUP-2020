/**
 * 
 * @file sandbox.hpp
 *
 * @brief generate sandbox.so to inject libiary
 *
 * @author setsal Lan
 * Contact: setsal.cs08g@nctu.edu.tw
 *
 */
#include <limits.h>
#include <stdlib.h>

using namespace std;

FILE *stream;
char buf[1024];
char buf2[1024];
char tmpBuffer[1024];

#define gen_reject_func(ret, sym, ...)         \
    ret sym(__VA_ARGS__) {                     \
        reject_info(path, #sym, 1);            \
        return -1;                             \
    }                                           

#define gen_single_hook(ret, default, sym, exec_parameters, ...)                    \
    ret sym(__VA_ARGS__) {                                                          \
        char *workdir = getenv("WORKDIR");                                          \
        realpath(path, buf);                                                        \
        ret (*func)(__VA_ARGS__);                                                   \
        if ( strncmp(workdir, buf, strlen(workdir)) == 0 ) {                        \
            func = (ret (*)(__VA_ARGS__)) dlsym(RTLD_NEXT, #sym);                   \
            ret ret_res = func exec_parameters;                                     \
            return ret_res;                                                         \
        }                                                                           \
        reject_info(path, #sym, 0);                                                 \
        return default;                                                             \
    }                                           
