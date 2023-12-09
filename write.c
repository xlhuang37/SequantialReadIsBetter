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

    // Creating Aligned Buffer
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
            if(UPPER_BOUND - LOWER_BOUND - block_num <= 0){perror("The Bound you set is smaller than block size??");}
            random_cursor = (random_cursor % (UPPER_BOUND - LOWER_BOUND - block_num)) * BLOCK_SIZE;
            
            lseek(file, random_cursor, SEEK_SET);
        }

       if(READ){
          if(write(file, buffer, block_size)==-1){perror("write error, wtf?!"); return 1;}
       }
       else{
          if(write(file, buffer, block_size)==-1){perror("write error, wtf?!"); return 1;}
       }
       
       if(stride){lseek(file, stride_val, SEEK_CUR);}
    }
    end_cpu = clock();
    gettimeofday(&end, NULL);

    cpu_time_used = ((double) (end_cpu - start_cpu)) / CLOCKS_PER_SEC;
    total_time_used =  ((end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec)) / (1000000 *1.0);
    throughput = (ONE_GB) / total_time_used /(1024*1024*1.0); // Throughput in MB/s

    // Document the data
    printf("CPU_TIME_USED = %f\n", cpu_time_used);
    printf("TOTAL_TIME_USED = %f\n", total_time_used);
    printf("Block Size: %d bytes, Throughput: %f MB/s\n", block_size, throughput);
    fprintf(log, "%d\n%f\n%f\n%f\n", block_num * BLOCK_SIZE, cpu_time_used, total_time_used, throughput);

    close(file);
    fclose(log);
    free(buffer);


    return 0;
}

// Debug Function to test that stuff is really written to the disk.
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
        // Set directory to write to or read from
        case 'm':
            length = strlen(optarg);
            device = malloc(length + 1);
            strcat(device, optarg);
            break;
        // Directory that contains recorded CPU_time, corresponding granularity, etc. 
        case 'n':
            length = strlen(optarg);
            log_directory = malloc(length + 1);
            strcat(log_directory, optarg);
            break;
        // Enable Stride Mode
        case 's':
            stride = 1;
            break;
        // Enable Random Access Mode
        case 'b':
            bounded = 1;
            break;
        // Set up upper bound Logic Block Addressing (LBA) for random access 
        case 'u':
            upper_bound = atoi(optarg);
            break;
        // Set up lower bound LBA
        case 'l':
            lower_bound = atoi(optarg);
            break;
        // Enable Read Mode
        case 'c':
            read_bool = 1;
            break;
        // Should never be here
        default:
            printf("wtf Wrong Argument");
            return 1;
        }
    }
    if(stride){
        for(int i = 1; i < 25600; i*=10){
            stride_val = i * BLOCK_SIZE;
            // j is set to 11 because the HHD only has 46 gbs. With j being max 10j, we guarantee that we write at most 11 gbs.
            for(int j = 1; j < i*11; j*=10){
                for(int k = 0; k < 5; k++){
                    sequential_write(j, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                }
            }
        }
    }
    else{
            for(int j = 0; j < 5; j++){
                sequential_write(1, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
            for(int j = 0; j < 5; j++){
                sequential_write(2, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
            for(int j = 0; j < 5; j++){
                sequential_write(5, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }

            for(int j = 0; j < 5; j++){
                sequential_write(10, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
            for(int j = 0; j < 5; j++){
                sequential_write(20, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
            for(int j = 0; j < 5; j++){
                sequential_write(100, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
                        }
            for(int j = 0; j < 5; j++){
                sequential_write(1000, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
            for(int j = 0; j < 5; j++){
                sequential_write(2000, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
            for(int j = 0; j < 5; j++){
                sequential_write(5000, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
            for(int j = 0; j < 5; j++){
                sequential_write(10000, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
            for(int j = 0; j < 5; j++){
                sequential_write(15000, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }
            for(int j = 0; j < 5; j++){
                sequential_write(20000, device, log_directory, stride, stride_val, bounded,  lower_bound, upper_bound, read_bool);
            }


    }

    // read_file(device, 0, 1000);
    // read_file(device, 4096, 1000);
    return 0;
}
