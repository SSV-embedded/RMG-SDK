## Prepare the development environment under Windows 10

1) Install Git for windows with the default settings
Download from the site: https://gitforwindows.org/


2) Install Visual Studio Code for Windows 10
Download from the site: https://code.visualstudio.com/Download


## Start the development environment

1) Start Visual Studio Code from the Windows Start menu (more info at https://code.visualstudio.com/docs)


2) Install Visual Studio Code Extension "ftp-simple"
Use the menu extensions on the left. Search for "ftp-simple" and install it.


3) Setup Visual Studio Code Extension "ftp-simple" via F1 –> ftp-simple:Config – FTP Connection Setting
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

4) Now the development environment is ready to use.


## Prepare the Python runtime environment on the RMG/941C

1) First start and configure the RMG. Don't forget to set the date and time!


2) Start the Visual Studio Code Editor


3) Open a new Git-Bash via "Terminal" -> "New" and start a SSH connection.
During the first connection you will receive the following message:
```
mha@mhaw10 MINGW64 /d/work/docker/RMG-SDK/Python_CAN_samples (main)
$ssh root@192.168.0.126 
The authenticity of host '192.168.0.126 (192.168.0.126)' can't be esatbilshed. 
ED25519 key fingerprint is SHA256:+1VycWd8HOJjCIjB+x0oVg9ou3Sy5qjAEX74JcHo29w.
The key is not know by any other names 
Are you sure you want to continue connecting (yes/no/[fingerprint])?
Enter yes to continue 

mha@mhaw10 MINGW64 /d/work/docker/RMG-SDK/Python_CAN_samples (main)
$ ssh root@192.168.0.126
root@192.168.0.126's password: 
root@emblinux:~#
```


4) Install the Python interpreter in the RMG/941C
```
root@emblinux:~# apt-get update
<..>
Reading package lists... Done

root@emblinux:~# apt-get install python3
<..>
After this operation, 20.5 MB of additional disk space will be used.
Do you want to continue? [Y/n] y
<..>

root@emblinux:~# python3 --version
Python 3.7.3
```


## How to test the Python interpreter with a simple program

1) Start the Visual Studio Code Editor


2) Create a new directory and use Start "Open Folder ..." to open it


3) Create a new file "hello.py" with Symbol "New File"


4) Enter the following source code in the editor window:
```python
# Hello world program
print('Hello world!\n')
```

5) Save the file "hello.py" with "File"->"Save"

6) Now you can transfer the program "hello.py" to the target with "ftp-simple" and run it.


## How do you transfer and run a program

1) Open a new Git-Bash via "Terminal" -> "New" and start a SSH connection. Then change to the directory "/tmp".
```
mha@mhaw10 MINGW64 /d/work/docker/RMG-SDK/Python_CAN_samples/example1 (main)
$ ssh root@192.168.0.126
root@192.168.0.126's password: 
root@emblinux:~# cd /tmp
root@emblinux:/tmp#
```


2) Upload the file "hello.py" with Visual Studio Code Extension "ftp-simple"
Mouse Right click on file -> use "ftp-simple:save" -> select FTP-Server "RMG/941C" -> current directory "/tmp".
```
root@emblinux:/tmp# ls -la h*
-rw-r--r-- 1 root root 46 Jul 13 17:50 hello.py
```


3) Run the program with the Python interpreter in the RMG/941C
```
root@emblinux:/tmp# python3 hello.py 
Hello world!

root@emblinux:/tmp#
```


## Setup and run program CAN Helloworld

1) First start and configure the RMG. You must enable the CAN interface (Network -> CAN).


2) Start the Visual Studio Code Editor and open the folder with the file "can_helloworld.py"


3) Open a new Git-Bash via "Terminal" -> "New" and start a SSH connection.
```
mha@mhaw10 MINGW64 /d/work/docker/RMG-SDK/Python_CAN_samples/example2 (main)
$ ssh root@192.168.2.56
root@192.168.2.56's password: 
root@emblinux:~#
```


4) Install Python packages for Socket-CAN and MQTT
```
root@emblinux:~# apt-get update
<..>
Reading package lists... Done

root@emblinux:~# apt-get install python3-paho-mqtt python3-can
<..>
Do you want to continue? [Y/n] y
<..>
```


5) Upload the file "can_helloworld.py" with Visual Studio Code Extension "ftp-simple"
Mouse Right click on file -> use "ftp-simple:save" -> select FTP-Server "RMG/941C" -> current directory "/tmp".
```
root@emblinux:~# cd /tmp

root@emblinux:/tmp# ls -la c*
-rw-r--r-- 1 root root 1470 Jul 13 18:30 can_helloworld.py
root@emblinux:/tmp#
```


6) Run the program with the Python interpreter in the RMG/941C
```
root@emblinux:/tmp# python3 can_helloworld.py 
Start CAN Helloworld...
Waiting for CAN message...
MQTT topic: can_helloworld/data
MQTT payload: { "id":"0x321", "type":"SFF", "len":3, "data":[1, 2, 3] }
```
