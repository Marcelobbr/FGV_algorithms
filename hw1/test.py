#!/usr/bin/env python
# coding: utf-8

import numpy as np
from datetime import datetime
import matplotlib.pyplot as plt
import csv
import random

# # SORTING ALGORITHMS
def insert_sort(a):
    for i in range(len(a)):
        cur_value = a[i]
        j = i-1
        while j >= 0 and a[j] > cur_value:
            a[j+1] = a[j]
            j = j-1
        a[j+1] = cur_value
    return a

def merge_sort(a):
    a = list(a)
    lenA= len(a)
    half_len = int(lenA/2)
    if len(a) <= 1:
        return a
    left = a[:half_len]
    right = a[half_len:]
    return merge(merge_sort(left), merge_sort(right))
def merge(a, b):
    result = []
    while a and b != []:
        if a[0] < b[0]:
            result.append(a[0])
            a.pop(0)
        else: 
            result.append(b[0])
            b.pop(0)
    result.extend(a)
    result.extend(b)
    return result

def quicksort(a):
    less = []
    equal = []
    greater = []
    if len(a) > 1:
        #pivot = a[0] #o algoritmo trava se não for randomico
        pivot = random.choice(a)
        for x in a:
            if x < pivot:
                less.append(x)
            elif x == pivot:
                equal.append(x)
            elif x > pivot:
                greater.append(x)
        return quicksort(less)+equal+quicksort(greater)
    else:
        return a

def is_sorted(a):
    if all([a[i] <= a[i+1] for i in range(len(a)-1)]):
        return True
    return False

# # FUNCOES AUXILIARES
def timer(t_init):
    t = datetime.now() - t_init
    t = t.seconds + t.microseconds*10**-6
    return t

def fill_list(mode, n):
    if mode == "random":
        result = np.random.randint(n, size=n)
    elif mode == "ascending":
        result = np.linspace(0,n,n)
    elif mode == "descending":
        result = np.linspace(n,0,n)
    result = [int(i) for i in result]
    return result

#caso queira salvar diretamente as imagens em vez de apenas criar csv
def save_figure(data, sizes, order):
    N = len(sizes)
    legend = list(data.keys())
    
    fig, ax = plt.subplots(figsize=(10,10))
    
    ind = np.arange(N)
    width = 0.1
    
    values = list(data.values())
    bars_dict = {}
    for i in range(len(data)):
        p = "p"+str(i)
        bars_dict[p] = ax.bar(ind+width*i, values[i], width)
    
    title = 'Runtime and algorithms ({} order)'.format(order)
    ax.set_title(title)
    ax.set_xticks(ind + width/2)
    ax.set_xticklabels(list(sizes))
    
    ax.legend(bars_dict.values(), legend)
    ax.set_xlabel('List sizes')
    ax.set_ylabel('Runtime')
    ax.autoscale_view()
    
    fig_name = "runtime_comparison_{}.png".format(order)
    plt.savefig(fig_name)

def csv_writer(file, data, sizes):
    writer = csv.writer(open(file, 'w'))
    headers = ["list_size"]
    for i in list(data.keys()):
        headers.append(i)
    values = list(data.values())
    values = list(map(list, zip(*values)))
    writer.writerow(headers)
    count = 0
    print(sizes)
    for i in values:
        row = []
        row.append(sizes[count])
        row.extend(i)
        writer.writerow(row)
        count+=1
        
def get_cpp_data(order):
    file = 'c++outputs_'+order+'.csv'
    csv_reader = csv.reader(open(file), delimiter=',')
    line_count = 0
    headers = next(csv_reader, None)[:3]
    cpp_dict = {h:[] for h in headers}
    for row in csv_reader:
        for h, v in list(zip(headers, row)):
            cpp_dict[h].append(float(v))
    return cpp_dict

# # 4.1
# Fiz com 10 sets indo de 270 a 2.700, pois o python limitava a quantidade maxima de resursoes,
# principalmente no quicksort. No C++ consegui trabalhar com ate 100.000 valores, mas eu nao teria
# como comparar com python.

def main():
    ns = list(np.linspace(10000, 100000, 10))
    ns = [int(i) for i in ns]
    
    #itera no ordenamento original das listas
    for order_type in ["random","ascending","descending"]:
        mergesort_rt, insertsort_rt, quicksort_rt = ([] for i in range(3))
        print("\norder_type:", order_type)
        
        #itera nos tamanhos das listas
        for n in ns:
            n = int(n)
            # create and fill an array with random numbers
            a = fill_list(order_type, n)
            
            # call insert_sort and  calculate the time
            time = datetime.now()
            sorted_a = insert_sort(a)
            t1 = round(timer(time),9)
            if not is_sorted(sorted_a):
                print("ERROR")
    
            # call merge_sort and  calculate the time
            time = datetime.now()
            sorted_a = merge_sort(a)
            t2 = round(timer(time),9)
            if not is_sorted(sorted_a):
                print("ERROR")
    
            # call quicksort and  calculate the time
            time = datetime.now()
            sorted_a = quicksort(a)
            t3 = round(timer(time),9)
            if not is_sorted(sorted_a):
                print("ERROR")
            
            insertsort_rt.append(t1)
            mergesort_rt.append(t2)
            quicksort_rt.append(t3)
            print("%d %.2f %.2f %.2f" % (n, t1, t2, t3))
        algorithms_dict = {}
        algorithms_dict['python_insert_sort'] = insertsort_rt
        algorithms_dict['python_merge_sort'] = mergesort_rt
        algorithms_dict['python_quick_sort'] = quicksort_rt
        cpp_algorithms_dict = get_cpp_data(order_type)
        algorithms_dict = {**cpp_algorithms_dict, **algorithms_dict}
        file = 'ALLoutputs_'+order_type+'.csv'
        csv_writer(file, algorithms_dict, ns)
        
        #save_figure(algorithms_dict, ns, order_type)
        if order_type == "random": return algorithms_dict

if __name__ == "__main__":
    main()

# # 4.2
# A função fill_list cria os arrays nas ordens indicadas a seguir: "random", "ascending", "descending". No 'main' eu gero as listas e salvo os gráficos com essas ordens.

# # 4.3
# A função fill_list cria os arrays nas ordens indicadas a seguir: "random", "ascending", "descending". No 'main' eu gero as listas e salvo os gráficos com essas ordens.
