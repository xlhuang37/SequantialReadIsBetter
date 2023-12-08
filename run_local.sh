./write   -n log_hhd_normal_write.txt
./write   -n log_hhd_stride_write.txt -s 
./write   -n log_hhd_random_write.txt -b -l 0 -u 25600
./write   -n log_hhd_normal_read.txt -c
./write   -n log_hhd_stride_read.txt -s -c
./write  -n log_hhd_random_read.txt -b -l 0 -u 25600 -c
./write   -n log_hhd_normal_write.txt
./write   -n log_hhd_stride_write.txt -s
./write   -n log_hhd_random_write.txt -b -l 0 -u 25600
./write   -n log_hhd_normal_read.txt -c
./write    -n log_hhd_stride_read.txt -s -c
./write   -n log_hhd_random_read.txt -b -l 0 -u 25600 -c
