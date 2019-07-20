#include <bits/stdc++.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std; 

/* FUNCOES AUXILIARES */
void linspace(int array[], int size){
    for (int i = 0; i < size; i++){
        array[i]=270+270*i;
}}

void print(int A[], int len_A) { 
    for (int i=0; i < len_A; i++) 
        cout << A[i] << " ";
    cout << endl;
} 

void build_array (int A[], int n, string order_type){
    //string order_type {"random"};
    if (order_type == "random")
    for (int count = 0; count < n; count++){
        A[count]=1+rand()% n;
    }
    else if (order_type == "ascending")
    for (int count = 0; count < n; count++){
        A[count]=count;
    }
    else if (order_type == "descending")
    for (int count = n; count > 0; count--){
        //cout << "testing order" << count << " \n";
        A[n-count]=count;
    }
    else 
        cout << "\nPLEASE CHOOSE A CORRECT ORDER\n";
}

/* ALGORITMOS DE ORDENAMENTO */
/* INSERTIONSORT */
void insertion_sort(int A[], int n){  
    int i, cur_value, j;  
    for (i = 1; i < n; i++){  
        cur_value = A[i];  
        j = i - 1;  
        while (j >= 0 && A[j] > cur_value){  
            A[j + 1] = A[j];  
            j = j - 1;  
        }  
        A[j + 1] = cur_value;  
    }  
}

/* MERGESORT */
int merge(int A[],int start, int mid,int end) {
    int len_left,len_right;
    int i,j,k; // i,j,k are iterators
    len_left=mid-start+1;
    len_right=end-mid;             
    int left[len_left],right[len_right];
    
    //cria parte da esquerda
    for(i=0;i<len_left;i++) {
        left[i]=A[start+i];
    }
    //cria parte da direita
    for(j=0;j<len_right;j++) {
        right[j]=A[mid+j+1];
    }
    //i=0,j=0;

    for(k=start,i=0,j=0;i<len_left&&j<len_right;k++){
        if(left[i]<right[j]) {
            A[k]=left[i];i++;
        }
        else {
            A[k]=right[j];j++;
        }
    }
    while(i<len_left) {
        A[k]=left[i];k++;i++;
    }
    while(j<len_right) {
        A[k]=right[j];k++;j++;
    }
}

int merge_sort(int A[],int start,int end) {
    //int q;                                
    if(start<end) {
        int mid=(start+end)/2;
        merge_sort(A,start,mid);
        merge_sort(A,mid+1,end);
        merge(A,start,mid,end);
    }
}

/* QUICKSORT */
int partition(int *a,int start,int end){
    int pivot=a[end];
    
    int pivot_iter=start;
    int i,temp; 
    
    for(i=start;i<end;i++) {
    	if(a[i]<=pivot) {
            temp=a[i]; //variavel 'temp' vai trocar os elementos de lugar
            a[i]=a[pivot_iter];
            a[pivot_iter++]=temp;
        }
     }
      temp=a[end]; //'temp' vai trocar os elementos de lugar
      a[end]=a[pivot_iter];
      a[pivot_iter]=temp;
     return pivot_iter;
 }

void quick_sort(int A[], int start, int end) { 
	if (start < end)  { 
		int separator = partition(A, start, end); 
		quick_sort(A, start, separator - 1); 
		quick_sort(A, separator + 1, end); 
	} 
} 

/* CHECA SE ARRAY ESTA ORDENADO EM ORDEM CRESCENTE */
bool is_sorted(int A[],int n){
	bool result=true;
    cout << "CHECK IF SORTED. IF NOT, SEND WARNING\n";
    for(int i=0;i<n;i++){
        if(A[i]>A[i+1])	{
            cout << "WARNING: ARRAY IS NOT SORTED!!!\n";
            result=false;
            break;
        }
    }
	return result;
}
    
int main() { 
    string test;
    
    //monta tamanhos dos arrays de 1.000 a 10.000
    int sets_size = 10;
    int sets[sets_size] = {};
    linspace(sets, sets_size);
    cout << "LIST OF SETS SIZES: \n";
    print(sets, sets_size);
    
    //stringstream serve para salvar csv/txt com resultados
    stringstream ss;
    string order_type,file_name;
    
    //1o loop escolhe entre 3 ordenamentos do array original: random, ascending ou descending
    for (int h =0; h<3; h++){
        ss.str("");
        ss.clear();
        if (h == 0){order_type = {"random"};}
        if (h == 1){order_type = {"ascending"};}
        if (h == 2){order_type = {"descending"};}
        ss << "c++outputs_" << order_type << ".csv";
        file_name = ss.str();
        ofstream out (file_name);
        out << "cpp_insert_sort,cpp_merge_sort,cpp_quick_sort,\n";
        
        //2o loop escolhe entre 10 tamanhos de array: 10.000 a 100.000
        for (int i = 0;i<sets_size;i++){
            int arr_len = sets[i];
            
            //3o loop escolhe entre 3 algoritmos de ordenamento: insertionsort, mergesort, quicksort
            for (int j =0; j<3; j++){
                int A[arr_len]={};
                build_array(A, arr_len, order_type);
                
                //inicia o cronometro
                clock_t tStart = clock();
                //cout << "TESTING ALGORITHM TYPE" << j << endl;
                if (j == 0){insertion_sort(A, arr_len)   ;}
                if (j == 1){merge_sort(A, 0, arr_len - 1);}
                if (j == 2){quick_sort(A, 0, arr_len - 1);}
                
                double timerun =  (double)(clock() - tStart)/CLOCKS_PER_SEC;
                bool is_sorting = is_sorted(A,arr_len);
                out << timerun << ","; // salva o timerun do algoritmo
            }
            out << endl; // termina a linha com 3 timerun para cada algoritmo
        }
        out.close(); // fecha o arquivo no loop
    }
	return 0; 
} 

