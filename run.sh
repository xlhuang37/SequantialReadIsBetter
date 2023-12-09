
./write  -m /dev/sdb1 -n log_hhd_normal_write.txt
./write  -m /dev/sdb1 -n log_hhd_stride_write.txt -s
./write  -m /dev/sdb1 -n log_hhd_random_write.txt -b -l 0 -u 25600
./write   -m /dev/sdb1 -n log_hhd_normal_read.txt -c
./write   -m /dev/sdb1 -n log_hhd_stride_read.txt -s -c
./write  -m /dev/sdb1  -n log_hhd_random_read.txt -b -l 0 -u 25600 -c
# The experiments for SSD sda2 is commented because sda2 only has 1kb of space??
# ./write  -m /dev/sdb1 -n log_hhd_normal_write.txt
# ./write  -m /dev/sdb1 -n log_hhd_stride_write.txt -s
# ./write  -m /dev/sdb1 -n log_hhd_random_write.txt -b -l 0 -u 25600
# ./write   -m /dev/sdb1 -n log_hhd_normal_read.txt -c
# ./write   -m /dev/sdb1 -n log_hhd_stride_read.txt -s -c
# ./write  -m /dev/sdb1  -n log_hhd_random_read.txt -b -l 0 -u 25600 -c
