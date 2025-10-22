/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"

/* this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend){
	int i = leftstart; // index for left subarray
	int j = rightstart; // index for right subarray
	int k = leftstart; // index for temp array B

	// merge the two subarrays into B[]
	while(i <= leftend && j <= rightend){
		if(A[i] <= A[j]){
			B[k++] = A[i++];
		}else{
			B[k++] = A[j++];
		}
	}

	// copy the remaining elements of the left subarray, if any
	while(i <= leftend){
		B[k++] = A[i++];
	}

	// copy the remaining elements of the right subarray, if any
	while(j <= rightend){
		B[k++] = A[j++];
	}

	// copy the merged elements back into the original array A[]
	memcpy(&A[leftstart], &B[leftstart], (rightend - leftstart + 1) * sizeof(int));
}

/* this function will be called by parallel_mergesort() as its base case. */
void my_mergesort(int left, int right){
	if(left >= right){
		return;
	}
	int mid = (left + right) / 2;
	my_mergesort(left, mid);
	my_mergesort(mid + 1, right);
	merge(left, mid, mid + 1, right);
}

/* this function will be called by the testing program. */
void *parallel_mergesort(void *arg){
		struct argument *args = (struct argument *) arg; // cast void* back to a pointer
		int left = args->left;
		int right = args->right;
		int level = args->level;
		if(level < cutoff){
		   int mid = (left + right) / 2;
		   pthread_t left_t, right_t;
		   
		   struct argument *leftArg = buildArgs(left, mid, level + 1);	
		   struct argument *rightArg = buildArgs(mid + 1, right, level + 1);

		   pthread_create(&left_t,NULL,parallel_mergesort,leftArg);
		   pthread_create(&right_t,NULL,parallel_mergesort,rightArg);

		   pthread_join(left_t,NULL);
		   pthread_join(right_t,NULL);
		   merge(left, mid, mid + 1, right);
		}else
		{
			my_mergesort(left, right);
		}
		//free(args); // free the argument struct
		return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
		struct argument *a = malloc(sizeof(struct argument));
		a->left = left;
		a->right = right;
		a->level = level;
		return a;
}

