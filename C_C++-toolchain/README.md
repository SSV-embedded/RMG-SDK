## Prepare the development environment under Windows 10

1) Install Git for windows with the default settings
Download from the site: https://gitforwindows.org/


2) Install Docker Desktop for Windows
Download from the site: https://docs.docker.com/docker-for-windows/install/


3) Install Visual Studio Code for Windows 10
Download from the site: https://code.visualstudio.com/Download


## Start the development environment

1) Start Docker Desktop from the Windows Start menu (look at https://docs.docker.com/docker-for-windows/install/)


2) Start Visual Studio Code from the Windows Start menu (more info at https://code.visualstudio.com/docs)


3) Install Visual Studio Code Extension "ftp-simple"
Use the menu extensions on the left. Search for "ftp-simple" and install it.


4) Setup Visual Studio Code Extension "ftp-simple" via F1 –> ftp-simple:Config – FTP Connection Setting
```json
    {
        "name": "RMG/941C",
        "host": "192.168.0.126",
        "port": 22,
        "type": "sftp",
        "username": "root",
        "password": "root",
        "path": "/tmp/",
        "autosave": false,
        "confirm": true
    }
```

5) Now the development environment is ready to use.


## How do you compile a simple C program

1) Start the Visual Studio Code Editor


2) Create a new directory and use Start "Open Folder ..." to open it


3) Create a new file "hello.c" with Symbol "New File"


4) Enter the following source code in the editor window:
```c
    #include <stdio.h>
    int main(void)
    {
        printf("Hello world!\n\n");
        return 0;
    }
```

5) Save the file "hello.c" with "File"->"Save"


6) Open a new Git-Bash via "Terminal" -> "New"
```
    mha@mhaw10 MINGW64 /d/work/docker/example1
    $
```

7) Start the Docker with the compiler in the bash terminal
```
    $ docker run --rm -v /$(pwd):/workdir ssvembeddedde/crossbuild gcc hello.c -o hello
```

8) The hello program was created
```
    $ ls -al
    total 13
    drwxr-xr-x 1 mha 197121    0 Jun 21 14:20 ./     
    drwxr-xr-x 1 mha 197121    0 Jun 21 14:04 ../    
    -rw-r--r-- 1 mha 197121 8176 Jun 21 14:20 hello  
    -rw-r--r-- 1 mha 197121   88 Jun 21 14:10 hello.c
```

9) Now you can transfer the program hello to the target with "ftp-simple" and run it.


## How do you transfer and run a program

1) Open a new Git-Bash via "Terminal" -> "New" and start a SSH connection. Then change to the directory "/tmp".
During the first connection you will receive the following message:
```
mha@mhaw10 MINGW64 /d/work/docker/example1
$ssh root@192.168.0.126 
The authenticity of host '192.168.0.126 (192.168.0.126)' can't be esatbilshed. 
ED25519 key fingerprint is SHA256:+1VycWd8HOJjCIjB+x0oVg9ou3Sy5qjAEX74JcHo29w.
The key is not know by any other names 
Are you sure you want to continue connecting (yes/no/[fingerprint])?
Enter yes to continue 

mha@mhaw10 MINGW64 /d/work/docker/example1
$ ssh root@192.168.0.126
root@192.168.0.126's password: 
root@emblinux:~# cd /tmp
root@emblinux:/tmp#
```

2) Upload the file "hello" with Visual Studio Code Extension "ftp-simple"
Mouse Right click on file -> use "ftp-simple:save" -> select FTP-Server "RMG/941C" -> current directory "/tmp".
```
root@emblinux:/tmp# ls -la h*
-rw-r--r-- 1 root root 8176 Jun  2 12:19 hello
```

3) Make the file executable and run the program
```
root@emblinux:/tmp# chmod +x hello
root@emblinux:/tmp# ./hello
Hello world!        

root@emblinux:/tmp#
```



## How do you compile a C program with external libraries

I will show you how to do this using the CAN helloworld project

1) First copy the project files into a new directory
```
$ ls -la
total 18
drwxr-xr-x 1 mha 197121    0 Jun 21 12:21 ./
drwxr-xr-x 1 mha 197121    0 Jun 21 14:04 ../
-rw-r--r-- 1 mha 197121 5895 Jun 18 15:32 can_helloworld.c
-rw-r--r-- 1 mha 197121  778 Jun 16 12:00 log.c
-rw-r--r-- 1 mha 197121  250 Jun 16 12:00 log.h
-rw-r--r-- 1 mha 197121  242 Jun 16 14:20 Makefile
```

2) Start the Visual Studio Code Editor and open the directory with "Open Folder ..."
The editor automatically opens the Makefile and shows it.
```makefile
CC=gcc
STRIP=strip
CFLAGS=
OPTS=-Wall
# -Werror

DEPS = log.c
PROJ1 = can_helloworld

all: clean $(PROJ1)

$(PROJ1): $(PROJ1).c $(DEPS) Makefile
	$(CC) $(CFLAGS) $(OPTS) $(DEPS) -o $@ $< -lmosquitto
	$(STRIP) $(PROJ1)

clean:
	rm -f $(PROJ1)
```

3) First, we must prepare the toolchain. Open a new Git-Bash via "Terminal" -> "New"
```
mha@mhaw10 MINGW64 /d/work/docker/example2
$
```

4) Create a new Docker container named "toolchain".
Test the Unix command "ls -la". You need to see the project files.
Finally exit the container.
```
$ docker run -it --name toolchain -v /$(pwd):/workdir ssvembeddedde/crossbuild bash
root@234e66b26f3f:/workdir# ls -la                      
total 16
drwxrwxrwx 1 root root  512 Jun 21 10:21 .
drwxr-xr-x 1 root root 4096 Jun 22 09:56 ..
-rwxrwxrwx 1 root root  242 Jun 16 12:20 Makefile        
-rwxrwxrwx 1 root root 5895 Jun 18 13:32 can_helloworld.c
-rwxrwxrwx 1 root root  778 Jun 16 10:00 log.c
-rwxrwxrwx 1 root root  250 Jun 16 10:00 log.h
root@234e66b26f3f:/workdir# exit
exit
```

5) Install an additional library for MQTT "libmosquitto" in a Docker container. First start the named container.
```
mha@mhaw10 MINGW64 /d/work/docker/example2
$ docker start -a -i toolchain
root@234e66b26f3f:/workdir#
```

6) Download the package list from debian
```
root@234e66b26f3f:/workdir# apt-get update
Get:1 http://security.debian.org/debian-security buster/updates InRelease [65.4 kB]
Get:2 http://deb.debian.org/debian buster InRelease [122 kB]     
Get:3 http://deb.debian.org/debian buster-updates InRelease [51.9 kB]
Get:4 http://security.debian.org/debian-security buster/updates/main armhf Packages [286 kB]
Get:5 http://security.debian.org/debian-security buster/updates/main amd64 Packages [293 kB]
Get:6 http://deb.debian.org/debian buster/main amd64 Packages [7907 kB]
Get:7 http://deb.debian.org/debian buster/main armhf Packages [7698 kB]
Get:8 http://deb.debian.org/debian buster-updates/main amd64 Packages.diff/Index [7132 B]
Get:9 http://deb.debian.org/debian buster-updates/main armhf Packages.diff/Index [7132 B]
Get:10 http://deb.debian.org/debian buster-updates/main amd64 Packages 2021-06-21-1401.46.pdiff [656 B]
Get:10 http://deb.debian.org/debian buster-updates/main amd64 Packages 2021-06-21-1401.46.pdiff [656 B]
Get:11 http://deb.debian.org/debian buster-updates/main armhf Packages 2021-06-21-1401.46.pdiff [448 B]
Get:11 http://deb.debian.org/debian buster-updates/main armhf Packages 2021-06-21-1401.46.pdiff [448 B]
Fetched 16.4 MB in 5s (3421 kB/s)
Reading package lists... Done
```

7) Install "libmosquitto" in the toolchain.
```
root@234e66b26f3f:/workdir# apt-get install -y libmosquitto-dev:armhf
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following additional packages will be installed:
  gcc-8-base:armhf libc6:armhf libgcc1:armhf libidn2-0:armhf libmosquitto1:armhf libssl1.1:armhf libunistring2:armhf
Suggested packages:
  glibc-doc:armhf libc-l10n:armhf locales:armhf
The following NEW packages will be installed:
  gcc-8-base:armhf libc6:armhf libgcc1:armhf libidn2-0:armhf libmosquitto-dev:armhf libmosquitto1:armhf libssl1.1:armhf libunistring2:armhf
0 upgraded, 8 newly installed, 0 to remove and 15 not upgraded.
<..>
```

8) You can now compile the program with make.
```
root@234e66b26f3f:/workdir# make
rm -f can_helloworld
gcc  -Wall log.c -o can_helloworld can_helloworld.c -lmosquitto
strip can_helloworld
root@234e66b26f3f:/workdir# ls -la
total 28
drwxrwxrwx 1 root root  512 Jun 22 10:24 .
drwxr-xr-x 1 root root 4096 Jun 22 09:56 ..
-rwxrwxrwx 1 root root  242 Jun 16 12:20 Makefile
-rwxr-xr-x 1 root root 9804 Jun 22 10:24 can_helloworld
-rwxrwxrwx 1 root root 5895 Jun 18 13:32 can_helloworld.c
-rwxrwxrwx 1 root root  778 Jun 16 10:00 log.c
-rwxrwxrwx 1 root root  250 Jun 16 10:00 log.h
```

9) Finally you can stop the container with "exit".
```
root@234e66b26f3f:/workdir# exit
exit
```

10) A restart of the container is possible at any time. The additional library remains in the Docker container.
```
mha@mhaw10 MINGW64 /d/work/docker/example2
$ docker start -a -i toolchain
root@234e66b26f3f:/workdir# make
rm -f can_helloworld
gcc  -Wall log.c -o can_helloworld can_helloworld.c -lmosquitto
strip can_helloworld
```

11) Now you can transfer the program can_helloworld to the target with "ftp-simple" and run it.
