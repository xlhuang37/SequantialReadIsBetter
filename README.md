# lab-5-alien

## Code Structure
All of the code for this lab is in write.c. My code supports both reading and writing in three modes:
1. Sequential mode. The code will sequentially write blocks of data until writing a total of 1GB.
2. Stride mode. After each write/read, the code will move forward by a offset. Other than that, the same as sequential mode.
3. Random mode. Upper and lower bounds will be defined, only to which the code can perform read/write. A random number is used to offset where we are reading inside the boundary.
Arguments are used to control the modes, boundaries, and read/write:
1. -m arg1: directory to write to or read from.
2. -n arg1: directory of the log that records throughput.
3. -s: enters stride mode. 
4. -b: enters random mode. (If random mode and stride mode are both selected, the current behavior of code is that it will enter random mode.)
5. -u arg1: sets upper bound. arg1's unit is "block." That is, an upper bound of 25600 means that the upper bound in bytes is 25600*4096. 
6. -l arg1: sets lower bound.
7. -c: enters read mode. If not set, enters write mode.

## Known Issues (and I don't think it is my issue):
Code works perfectly on my docker, but it has the following issue on gwion:

It seems that the system blocks os calls such as write does not work on gwion, but higher-level functions such as fopen/fwrite does their job. Open will open the remote file, and from ls -l I could see that the disk was modified, but the runtime indicates that the write to disk was actually not done.

