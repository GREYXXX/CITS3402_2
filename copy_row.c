#include "variables.h"

void copy_row(int local_mat[], int n, intp, int k_row[], int k){
        int job_k = k % (n/p);
        for(int j = 0; j < n; j++){
                k_row[j] = local_mat[job_k*n + j];
        }
}
