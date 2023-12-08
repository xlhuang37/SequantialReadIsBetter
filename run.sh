./write -m /dev/sdb1 -n log_hhd_normal_write.txt
./write -m /dev/sdb1 -n log_hhd_stride_write.txt -s 1
./write -m /dev/sdb1 -n log_hhd_random_write.txt -b 1 -l 0 -u 25600
./write -m /dev/sdb1 -n log_hhd_normal_read.txt -r 1
./write -m /dev/sdb1 -n log_hhd_stride_read.txt -s 1 -r 1
./write -m /dev/sdb1 -n log_hhd_random_read.txt -b 1 -l 0 -u 25600 -r 1
./write -m /dev/sda2 -n log_hhd_normal_write.txt
./write -m /dev/sda2 -n log_hhd_stride_write.txt -s 1
./write -m /dev/sda2 -n log_hhd_random_write.txt -b 1 -l 0 -u 25600
./write -m /dev/sda2 -n log_hhd_normal_read.txt -r 1
./write -m /dev/sda2 -n log_hhd_stride_read.txt -s 1 -r 1
./write -m /dev/sda2 -n log_hhd_random_read.txt -b 1 -l 0 -u 25600 -r 1