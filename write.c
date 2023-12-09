#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define ONE_GB (1024*1024*1024)
#define MAX_BLOCK_SIZE (100 * 1024 * 1024)
#define MAX_BLOCK_NUM 25600
#define BLOCK_SIZE 4096


int sequential_write(int block_num, char* device, char* log_directory, int stride, int stride_val, int BOUNDED, int LOWER_BOUND, int UPPER_BOUND, int READ) {
    // Initializing block sizes
    int block_size = block_num * BLOCK_SIZE;
    printf("Writing using granularity %d\n", block_size);
    if(READ){printf("Read Mode On\n");}
    else{printf("Write Mode On\n");}
    if(stride){printf("Stride mode on. Stride offset is %d\n", stride_val);}
    if(BOUNDED){printf("Random mode on. Random boundary logical block %d to logical block %d\n", LOWER_BOUND, UPPER_BOUND);}


    void *buffer;
    int file, random_cursor;
    clock_t start_cpu, end_cpu;
    struct timeval start, end;
    double cpu_time_used, throughput, total_time_used;

    // buffer with two different patterns.
    posix_memalign(&buffer, 4096, block_size);
    memset(buffer, '3', block_size);

    file = open(device, O_DIRECT | O_RDWR);
    if(file == -1){perror("Error opening file"); return 1;}
    FILE* log = fopen(log_directory, "a");

    gettimeofday(&start, NULL);
    start_cpu = clock();
    for (long long total_written = 0; total_written + block_size < ONE_GB; total_written += block_size) {
       if(BOUNDED){
            random_cursor = random();
            random_cursor = (random_cursor % (UPPER_BOUND - LOWER_BOUND - block_num)) * BLOCK_SIZE;
            lseek(file, random_cursor, SEEK_SET);
        }

       if(READ){
          if(read(file, buffer, block_size)==-1){perror("write error, wtf?!");}
       }
       else{
          if(write(file, buffer, block_size)==-1){perror("write error, wtf?!");}
       }
       
       if(stride){lseek(file, stride_val, SEEK_CUR);}
    }
    end_cpu = clock();
    gettimeofday(&end, NULL);

    cpu_time_used = ((double) (end_cpu - start_cpu)) / CLOCKS_PER_SEC;
    total_time_used =  ((end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec)) / (1000000 *1.0);
    throughput = (ONE_GB) / total_time_used /(1024*1024*1.0); // Throughput in MB/s

    printf("CPU_TIME_USED = %f\n", cpu_time_used);
    printf("TOTAL_TIME_USED = %f\n", total_time_used);
    printf("Block Size: %d bytes, Throughput: %f MB/s\n", block_size, throughput);
    fprintf(log, "%d\n%f\n%f\n%f\n", block_num, cpu_time_used, total_time_used, throughput);

    close(file);
    fclose(log);
    free(buffer);


    return 0;
}

int read_file(char* directory, int offset, int read_size){
    char* buffer = calloc(read_size + 1, sizeof(char));
    int file = open(directory, O_RDONLY);
    if(file == -1){perror("Error opening file"); return 1;}
    lseek(file, offset, SEEK_SET);
    read(file, buffer, read_size);
    printf("%s\n",buffer);
    free(buffer);
    return 0;
}


int main(int argc, char ** argv){
    printf("we are inside main function\n");
    int stride, stride_val, bounded, upper_bound, lower_bound, c, read_bool, length;
    char* device = "test_file.txt";
    char* log_directory = "log.txt";
    stride = 0;
    stride_val = -1;
    bounded = 0;
    read_bool = 0;
    upper_bound = 1;
    lower_bound = 0;
    while ((c = getopt (argc, argv, "sbcm:n:u:l:")) != -1) {
        switch (c) {
        case 'm':
            length = strlen(optarg);
            device = malloc(length + 1);
            strcat(device, optarg);
            break;
        case 'n':
            length = strlen(optarg);
            log_directory = malloc(length + 1);
            strcat(log_directory, optarg);
            break;
        case 's':
            stride = 1;
            break;
        case 'b':
            bounded = 1;
            break;
        case 'u':
            upper_bound = atoi(optarg);
            break;
        case 'l':
            lower_bound = atoi(optarg);
            break;
        case 'c':
            read_bool = 1;
            break;
        default:
            printf("wtf");
            return 1;
        }
    }
    if(stride){
        for(int i = 1; i <= 6; i+=1){
            stride_val = i * BLOCK_SIZE;
            for(int j = 1; j < 6; j+=1){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 10; j < 60; j+=10){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 100; j < 600; j+=100){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 1000; j < 2560; j+=500){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
        }
        for(int i = 10; i <= 60; i+=10){
            stride_val = i * BLOCK_SIZE;
            for(int j = 1; j < 6; j+=1){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 10; j < 60; j+=10){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 100; j < 600; j+=100){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 1000; j < 2560; j+=500){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
        }
        for(int i = 100; i <= 600; i+=100){
            stride_val = i * BLOCK_SIZE;
            for(int j = 1; j < 6; j+=1){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 10; j < 60; j+=10){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 100; j < 600; j+=100){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 1000; j < 2560; j+=500){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
        }
        for(int i = 1000; i <= 6000; i+=1000){
            stride_val = i * BLOCK_SIZE;
            for(int j = 1; j < 6; j+=1){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 10; j < 60; j+=10){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 100; j < 600; j+=100){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 1000; j < 2560; j+=500){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
        }
        for(int i = 10000; i <= MAX_BLOCK_NUM; i+=5000){
            stride_val = i * BLOCK_SIZE;
            for(int j = 1; j < 6; j+=1){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 10; j < 60; j+=10){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 100; j < 600; j+=100){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
            for(int j = 1000; j < 2560; j+=500){
                for(int k = 0; k < 16; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
        }

    }
    else{
        for(int i = 1; i < 6; i+=1){
            for(int j = 0; j < 16; j++){
                sequential_write(0 + i, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
        }
        for(int i = 10; i < 60; i+=10){
            for(int j = 0; j < 16; j++){
                sequential_write(100 + i, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
        }
        for(int i = 100; i < 600; i+=100){
            for(int j = 0; j < 16; j++){
                sequential_write(1000 + i, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
        }
        for(int i = 1000; i < 6000; i+=1000){
            for(int j = 0; j < 16; j++){
                sequential_write(5000 + i, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
        }
        for(int i = 10000; i < 25600; i+=5000){
            for(int j = 0; j < 16; j++){
                sequential_write(10000 + i, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
        }
        for(int j = 0; j < 16; j++){
            sequential_write(25600, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
        }
    }

    // read_file(device, 0, 1000);
    // read_file(device, 4096, 1000);
    return 0;
}
