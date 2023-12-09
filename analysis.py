import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import scipy.stats as stats

granularity_list = []
data_lists = []
all_data_list = []
new_list = []
with open('normal_write', 'r') as file:
    counter = 0
    while True:
        data = file.readline()
        if not data:
            break
        data = data.rstrip()
        data = float(data)
        if counter % 20 == 0:
            data_lists.append(new_list)
            new_list = []
            granularity_list.append(data)
        if counter % 4 == 3:
            new_list.append(data)
            all_data_list.append(data)
        counter += 1
data_lists.append(new_list)
data_lists = data_lists[1:]

mean_list = []
counter = 0
while(counter < len(all_data_list)):
    sum = 0.0
    for i in range(5):
        sum += all_data_list[counter + i]
    mean_list.append(sum / 5)
    counter += 5



std = np.array([np.std(m) for m in data_lists])
sems = std / np.sqrt(5)
t_score = stats.t.ppf(0.975, 4)
confidence_intervals = t_score * sems

plt.errorbar(granularity_list, mean_list, yerr=confidence_intervals, fmt='o', capsize=5, label='Mean with Error Bars')
plt.plot(granularity_list, mean_list)
plt.scatter(granularity_list, mean_list, label='Points', color='blue')
plt.title('HHD_Read_Random')
plt.xscale('log')  
plt.xlabel('Granularity')
plt.ylabel('Throughput')
plt.show()

# stridelen_list = []
# stridelen = 2048
# data_lists = []
# all_data_list = []
# new_list = []
# with open('stride_read_409600', 'r') as file:
#     counter = 0
#     while True:
#         data = file.readline()
#         if not data:
#             break
#         data = data.rstrip()
#         data = float(data)
#         if counter % 20 == 0:
#             stridelen *= 2
#             data_lists.append(new_list)
#             new_list = []
#             stridelen_list.append(stridelen)
#         if counter % 4 == 3:
#             new_list.append(data)
#             all_data_list.append(data)
#         counter += 1

# mean_list = []
# counter = 0
# while(counter < len(all_data_list)):
#     sum = 0.0
#     for i in range(5):
#         sum += all_data_list[counter + i]
#     mean_list.append(sum / 5)
#     counter += 5



# std = np.array([np.std(m) for m in data_lists])
# sems = std / np.sqrt(5)
# t_score = stats.t.ppf(0.975, 4)
# confidence_intervals = t_score * sems

# plt.errorbar(stridelen_list, mean_list, yerr=confidence_intervals, fmt='o', capsize=5, label='Mean with Error Bars')
# plt.plot(stridelen_list, mean_list)
# plt.scatter(stridelen_list, mean_list, label='Points', color='blue')
# plt.title('HHD_read_Stride_Granularity40960000')
# plt.xscale('log')  
# plt.xlabel('StrideLength')
# plt.ylabel('Throughput')
# plt.show()
