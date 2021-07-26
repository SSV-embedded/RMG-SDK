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

5) Install the Python Libraries for Machine Learning in the RMG/941C
```
root@emblinux:~# apt-get update
<..>
Reading package lists... Done

root@emblinux:~# apt-get -y install python3-tflite-runtime
Reading package lists... Done
<..>

root@emblinux:~# apt-get -y install python3-pandas
Reading package lists... Done
<..>
Setting up python3-pandas (0.23.3+dfsg-3) ...

root@emblinux:~# apt-get -y install python3-matplotlib
Reading package lists... Done
<..>
Setting up python3-matplotlib (3.0.2-2) ...

root@emblinux:~# apt-get -y install python3-scipy
Reading package lists... Done
<..>
Setting up python3-scipy (1.1.0-7) ...

root@emblinux:~# apt-get -y install python3-sklearn
Reading package lists... Done
<..>
Setting up python3-sklearn (0.20.2+dfsg-6) ...
```


## How do you transfer and run a program for a test

1) Start the Visual Studio Code Editor

2) Copy all files into a new directory and use Start "Open Folder ..." to open it
```
mha@mhaw10 MINGW64 /d/work/devel/RMG-SDK/Python_Machine_Learning (main)
$ ls
example1/  example2/  README.md

mha@mhaw10 MINGW64 /d/work/devel/RMG-SDK/Python_Machine_Learning (main)
$ ls example1/
tfl-version.py

mha@mhaw10 MINGW64 /d/work/devel/RMG-SDK/Python_Machine_Learning (main)
$ ls example2/
C-my_model.tflite  R-my_model.tflite  tfl-test.py
```

3) Open a new Git-Bash via "Terminal" -> "New" and start a SSH connection. Then change to the directory "/tmp".
```
mha@mhaw10 MINGW64 /d/work/devel/RMG-SDK/Python_Machine_Learning/example1 (main)
$ ssh root@192.168.0.126
root@192.168.0.126's password: 
root@emblinux:~# cd /tmp
root@emblinux:/tmp#
```


4) Upload the file "tfl-version.py" with Visual Studio Code Extension "ftp-simple"
Mouse Right click on file -> use "ftp-simple:save" -> select FTP-Server "RMG/941C" -> current directory "/tmp".
```
root@emblinux:/tmp# ls -la t*
-rw-r--r-- 1 root root  568 Jul 26 17:34 tfl-version.py
```


5) Run the program with the Python interpreter in the RMG/941C
```
root@emblinux:/tmp# python3 tfl-version.py
Python Version: 3.7.3 (default, Jan 22 2021, 20:04:44) 
[GCC 8.3.0]
TensorFlow Lite interpreter version: 2.6.0-rc1-2
NumPy Version: 1.16.2
root@emblinux:/tmp#
```


6) Upload the files "C-my_model.tflite", "R-my_model.tflite" and "tfl-test.py" with Visual Studio Code Extension "ftp-simple"
Mouse Right click on file -> use "ftp-simple:save" -> select FTP-Server "RMG/941C" -> current directory "/tmp".
```
root@emblinux:/tmp# ls -la tfl-test.py C-my_model.tflite R-my_model.tflite
-rw-r--r-- 1 root root 3492 Jul 26 17:34 C-my_model.tflite
-rw-r--r-- 1 root root  768 Jul 26 17:34 R-my_model.tflite
-rw-r--r-- 1 root root 1050 Jul 26 17:34 tfl-test.py
```


7) Run the program with the Python interpreter in the RMG/941C
```
root@emblinux:/tmp# python3 tfl-test.py
[[-15.78]]
root@emblinux:/tmp#
```
