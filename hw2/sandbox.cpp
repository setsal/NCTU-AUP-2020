/**
 * 
 * @file sandbox.cpp
 *
 * @brief generate sandbox.so to inject libiary
 *
 * @author setsal Lan
 * Contact: setsal.cs08g@nctu.edu.tw
 *
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include "sandbox.hpp"
using namespace std;


void reject_info(const char *arg, const char *func, int isRestrict) {
    
    // define original func type
    FILE* (*old_fopen)(const char *filename, const char *mode);

    // check if path valid in sandbox limit directory
    old_fopen = (FILE* (*)(const char *filename, const char *mode)) dlsym(RTLD_NEXT, "fopen");
    
    // use old fopen
    if (isRestrict)
        sprintf(tmpBuffer, "[sandbox] %s(%s): not allowed\n", func, arg);
    else 
        sprintf(tmpBuffer, "[sandbox] %s: access to %s is not allowed\n", func, arg);   
    stream = old_fopen("/dev/tty", "a+");
    if ( stream == NULL ) {
        cout << "/dev/tty open failed" << endl;
    }
    fprintf(stderr, tmpBuffer);
}

void debug_info(const char *str) {
    
    // define original func type
    FILE* (*old_fopen)(const char *filename, const char *mode);

    // check if path valid in sandbox limit directory
    old_fopen = (FILE* (*)(const char *filename, const char *mode)) dlsym(RTLD_NEXT, "fopen");
    
    // use old fopen
    sprintf(tmpBuffer, "[DEBUG] %s\n", str);
    stream = old_fopen("/dev/tty", "a+");
    if ( stream == NULL ) {
        cout << "/dev/tty open failed" << endl;
    }
    fprintf(stderr, tmpBuffer);
}


/* Reject function */
gen_reject_func(int, system, const char *path);
gen_reject_func(int, execl, const char *path, const char *arg, ...);
gen_reject_func(int, execle, const char *path, const char *arg, ...);
gen_reject_func(int, execlp, const char *path, const char *arg, ...);
gen_reject_func(int, execv, const char *path, char *const argv[]);
gen_reject_func(int, execve, const char *path, char *const argv[],char *const envp[]);
gen_reject_func(int, execvp, const char *path, char *const argv[]);


/* Normal single diff function inject with pre define macro */
gen_single_hook(int, -1, chdir, (path), const char *path);
gen_single_hook(int, -1, chmod, (path, mode), const char *path, mode_t mode);
gen_single_hook(int, -1, chown, (path, owner, group), const char *path, uid_t owner, gid_t group);
gen_single_hook(int, -1, creat, (path, mode), const char *path, mode_t mode);
gen_single_hook(FILE*, NULL, fopen, (path, mode), const char *path, const char *mode);
gen_single_hook(int, -1, mkdir, (path, mode), const char *path, mode_t mode);
gen_single_hook(DIR*, NULL, opendir, (path), const char *path);
gen_single_hook(ssize_t, -1, readlink, (path, buf, bufsiz), const char *path, char *buf, size_t bufsiz);
gen_single_hook(int, -1, remove, (path), const char *path);
gen_single_hook(int, -1, rmdir, (path), const char *path);
gen_single_hook(int, -1, stat, (path, statbuf), const char *path, struct stat *statbuf);
gen_single_hook(int, -1, __xstat, (ver, path, statbuf), int ver, const char * path, struct stat *statbuf);
gen_single_hook(int, -1, __lxstat, (ver, path, statbuf), int ver, const char * path, struct stat *statbuf);
gen_single_hook(int, -1, unlink, (path), const char *path);


/* Special case for deal with Variadic parameters */
// open
int open(const char *path, int flags, ...) {
    char *workdir = getenv("WORKDIR");

    // get parmeters
    va_list ap;
    va_start(ap, flags);
    mode_t mode = va_arg(ap, mode_t);

    // format dirname to absolute path
    realpath(path, buf);

    // check if path valid in sandbox limit directory
    if ( strncmp(workdir, buf, strlen(workdir)) == 0 ) {
        if (mode) {
            int (*func)(const char *path, int flags, ...);
            func = (int (*)(const char *path, int flags, ...)) dlsym(RTLD_NEXT, "open");
            int ret = func(path, flags, mode);
            return ret;
            
        } else {
            int (*func)(const char *path, int flags);
            func = (int (*)(const char *path, int flags)) dlsym(RTLD_NEXT, "open");    
            int ret = func(path, flags);
            return ret;
        }
    }
    reject_info(path, "open", 0);    
    return -1;  
}

int openat(int dirfd, const char *path, int flags, ...) {
    char *workdir = getenv("WORKDIR");

    // get parmeters
    va_list ap;
    va_start(ap, flags);
    mode_t mode = va_arg(ap, mode_t);

    // format dirname to absolute path
    realpath(path, buf);


    // check if path valid in sandbox limit directory
    if ( strncmp(workdir, buf, strlen(workdir)) == 0 ) {
        if (mode) {
            int (*func)(int dirfd, const char *path, int flags, ...);
            func = (int (*)(int dirfd, const char *path, int flags, ...)) dlsym(RTLD_NEXT, "openat");
            int ret = func(dirfd, path, flags, mode);
            return ret;
            
        } else {
            int (*func)(int dirfd, const char *path, int flags);
            func = (int (*)(int dirfd, const char *path, int flags)) dlsym(RTLD_NEXT, "openat");    
            int ret = func(dirfd, path, flags);
            return ret;
        }
    }
    reject_info(path, "openat", 0);    
    return -1;  
}

/* Two argument inject function */
int link(const char *oldpath, const char *newpath) {
    char *workdir = getenv("WORKDIR");
    
    // format dirname to absolute path
    realpath(oldpath, buf);
    realpath(newpath, buf2);

    // define original func type
    int (*func)(const char *oldpath, const char *newpath);

    // check if path valid in sandbox limit directory
    if ( strncmp(workdir, buf, strlen(workdir)) == 0 && strncmp(workdir, buf2, strlen(workdir)) == 0 ) {
        func = (int (*)(const char *oldpath, const char *newpath)) dlsym(RTLD_NEXT, "link");
        int ret = func(oldpath, newpath);
        return ret;
    }
    reject_info(oldpath, "link", 0);    
    reject_info(newpath, "link", 0);
    return -1;
}

// rename
int rename(const char *oldpath, const char *newpath) {
    char *workdir = getenv("WORKDIR");
    
    // format dirname to absolute path
    realpath(oldpath, buf);
    realpath(newpath, buf2);

    // define original func type
    int (*func)(const char *oldpath, const char *newpath);

    // check if path valid in sandbox limit directory
    if ( strncmp(workdir, buf, strlen(workdir)) == 0 && strncmp(workdir, buf2, strlen(workdir)) == 0 ) {
        func = (int (*)(const char *oldpath, const char *newpath)) dlsym(RTLD_NEXT, "rename");
        int ret = func(oldpath, newpath);
        return ret;
    }
    reject_info(oldpath, "rename", 0);    
    reject_info(newpath, "rename", 0);
    return -1;
}


// symlink
int symlink(const char *target, const char *linkpath) {
    char *workdir = getenv("WORKDIR");
    
    // format dirname to absolute path
    realpath(target, buf);
    realpath(linkpath, buf2);

    // define original func type
    int (*func)(const char *target, const char *linkpath);

    // check if path valid in sandbox limit directory
    if ( strncmp(workdir, buf, strlen(workdir)) == 0 && strncmp(workdir, buf2, strlen(workdir)) == 0 ) {
        func = (int (*)(const char *target, const char *linkpath)) dlsym(RTLD_NEXT, "symlink");
        int ret = func(target, linkpath);
        return ret;
    }
    reject_info(target, "symlink", 0);    
    reject_info(linkpath, "symlink", 0);
    return -1;
}


/* For large file support function */
gen_single_hook(FILE*, NULL, fopen64, (path, mode), const char *path, const char *mode);
gen_single_hook(int, -1, creat64, (path, mode), const char *path, mode_t mode);
gen_single_hook(int, -1, stat64, (path, statbuf), const char *path, struct stat *statbuf);
gen_single_hook(int, -1, __xstat64, (ver, path, statbuf), int ver, const char * path, struct stat *statbuf);

// open
int open64(const char *path, int flags, ...) {
    char *workdir = getenv("WORKDIR");

    // get parmeters
    va_list ap;
    va_start(ap, flags);
    mode_t mode = va_arg(ap, mode_t);

    // format dirname to absolute path
    realpath(path, buf);

    // check if path valid in sandbox limit directory
    if ( strncmp(workdir, buf, strlen(workdir)) == 0 ) {
        if (mode) {
            int (*func)(const char *path, int flags, ...);
            func = (int (*)(const char *path, int flags, ...)) dlsym(RTLD_NEXT, "open64");
            int ret = func(path, flags, mode);
            return ret;
            
        } else {
            int (*func)(const char *path, int flags);
            func = (int (*)(const char *path, int flags)) dlsym(RTLD_NEXT, "open64");    
            int ret = func(path, flags);
            return ret;
        }
    }
    reject_info(path, "open64", 0);    
    return -1;  
}

int openat64(int dirfd, const char *path, int flags, ...) {
    char *workdir = getenv("WORKDIR");

    // get parmeters
    va_list ap;
    va_start(ap, flags);
    mode_t mode = va_arg(ap, mode_t);

    // format dirname to absolute path
    realpath(path, buf);

    // check if path valid in sandbox limit directory
    if ( strncmp(workdir, buf, strlen(workdir)) == 0 ) {
        if (mode) {
            int (*func)(int dirfd, const char *path, int flags, ...);
            func = (int (*)(int dirfd, const char *path, int flags, ...)) dlsym(RTLD_NEXT, "openat64");
            int ret = func(dirfd, path, flags, mode);
            return ret;
            
        } else {
            int (*func)(int dirfd, const char *path, int flags);
            func = (int (*)(int dirfd, const char *path, int flags)) dlsym(RTLD_NEXT, "openat64");    
            int ret = func(dirfd, path, flags);
            return ret;
        }
    }
    reject_info(path, "openat64", 0);    
    return -1;  
}