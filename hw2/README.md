# NCTU Unix Programing HW2

Play in a Sandbox!?

##  Complie
+ Please ensure your `g++ gcc` use the standard c99 up
    ```
    make
    ```
+ 資料夾亦有附上老師提供的 sample.c 會一併編譯成 `sample`, `sample2` 提供使用

## Function monitor
基本上照著老師提供的 requirement 做 monitor
```c
// Reject function
int execl(const char *path, const char *arg, ...);
int execle(const char *path, const char *arg, ...);
int execlp(const char *path, const char *arg, ...);
int execv(const char *path, char *const argv[]);
int execve(const char *path, char *const argv[],char *const envp[]);
int execvp(const char *path, char *const argv[]);
int system(const char *path);


// Normal function ( just follow teacher's requirements )
int chdir(const char *path);
int chmod(const char *path, mode_t mode);
int chown(const char *path, uid_t owner, gid_t group);
int creat(const char *path, mode_t mode);
FILE *fopen(const char *path, const char *mode);
int link(const char *oldpath, const char *newpath);
int mkdir(const char *path, mode_t mode);
int open(const char *path, int flags, ...);
int openat(int dirfd, const char *path, int flags, ...);
DIR *opendir(const char *path);
ssize_t readlink(const char *path, char *buf, size_t bufsiz);
int remove(const char *path);
int rename(const char *oldpath, const char *newpath);
int rmdir(const char *path);
int stat(const char *path, struct stat *statbuf);
int __lxstat (int ver, const char * path, struct stat *statbuf);
int __xstat(int ver, const char * path, struct stat *statbuf);
int symlink(const char *target, const char *linkpath);
int unlink(const char *path);
```

對於 large file support, 透過對老師提供的 sample 編譯分析
`readelf --syms ./sample2 | grep 64`
提供額外如下種 function 做 large file 支援

```
FILE *fopen64(const char *path, const char *mode);
int creat64(const char *path, mode_t mode);
int stat64(const char *path, struct stat *statbuf);
int __xstat64(int ver, const char * path, struct stat *statbuf);
int open64(const char *path, int flags, ...);
int openat64(int dirfd, const char *path, int flags, ...);
```
![Imgur](https://i.imgur.com/2QqKa0e.png)

## ScreenShots

### test env
+ debian 4.19, libc version 2.29

### res

+ sample.c

    ![Imgur](https://i.imgur.com/DvP5gOJ.png)

    ![Imgur](https://i.imgur.com/4mrHEHI.png)

+ sample2.c

    ![Imgur](https://i.imgur.com/HEMbmmC.png)

## Others
希望環境沒有太大差異  有自己在 ubuntu VM 上面跑過

若有問題歡迎跟我聯繫 :)

