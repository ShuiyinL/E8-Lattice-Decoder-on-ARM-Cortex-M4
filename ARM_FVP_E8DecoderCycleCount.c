/*
 ============================================================================
 Name        : E8DecoderCycleCount.c
 Debugger    : Arm FVP (installed with Arm DS)/MPS2_Cortex_M4
 Author      : 
 Version     : 1
 Copyright   : Free
 Description : Implementation of E8 and Dn lattice decoders in
 Conway, J., Sloane, N.: Fast quantizing and decoding and algorithms for lattice quantizers and codes.
 IEEE Transactions on Information Theory 28(2), 227–232 (1982).
 https://doi.org/10.1109/TIT.1982.1056484
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // Required for memcpy()



volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)0xE0001004; //address of the register
volatile unsigned int *DWT_CONTROL  = (volatile unsigned int *)0xE0001000; //address of the register
volatile unsigned int *DWT_LAR      = (volatile unsigned int *)0xE0001FB0; //address of the register
volatile unsigned int *SCB_DEMCR    = (volatile unsigned int *)0xE000EDFC; //address of the register



float* subtract_arrays(int* intArray, float* doubleArray, int size) {
    // Allocate memory for the result array (of type double)
	float* resultArray = (float*)malloc(size * sizeof(float));

	/*
    // Check if memory allocation was successful
    if (resultArray == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL; // Return NULL if allocation fails
    }
    */

    // Perform element-wise subtraction
    for (int i = 0; i < size; i++) {
        resultArray[i] = (float)intArray[i] - doubleArray[i]; // Cast int to double for subtraction
    }

    return resultArray;
}

float* subtract_arrays_double(float* doubleArray0, float* doubleArray, int size) {
    // Allocate memory for the result array (of type double)
	float* resultArray = (float*)malloc(size * sizeof(float));

    /*
	// Check if memory allocation was successful
    if (resultArray == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL; // Return NULL if allocation fails
    }
    */

    // Perform element-wise subtraction
    for (int i = 0; i < size; i++) {
        resultArray[i] = doubleArray0[i] - doubleArray[i]; // Cast int to double for subtraction
    }

    return resultArray;
}


float calculateArrayNorm(float arr[], int size) {
	float sum_of_squares = 0.0;
    for (int i = 0; i < size; i++) {
        sum_of_squares += (float)arr[i] * arr[i]; // Cast to double to avoid overflow and ensure floating-point arithmetic
    }
    return (sum_of_squares);

}

int sumArrayElements(int arr[], int size) {
    int sum = 0; // Initialize sum to zero
    // Loop through each element of the array
    for (int i = 0; i < size; i++) {
        sum += arr[i]; // Add the current element to the sum
    }
    return sum; // Return the total sum
}



void printFloatArray(float arr[], int size) {
    printf("Array elements: ");
    for (int i = 0; i < size; i++) {
        printf("%.2lf ", arr[i]);
    }
    printf("\n");
}

void printIntArray(int arr[], int size) {
    printf("Array elements: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void calculateAbsoluteValues(float arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = fabs(arr[i]); // Apply abs() to each element
    }
}

int* roundArrayToNearestInteger(float arr[], int size) {
	int* newArray = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
    	newArray[i] = (int)round(arr[i]);
    }
    return newArray;
}


int findIndexOfLargest(float arr[], int size) {
    if (size <= 0) {
        // Handle empty or invalid array size
        return -1; // Or some other error indicator
    }

    int largest_val = arr[0];
    int largest_index = 0;

    for (int i = 1; i < size; i++) {
        if (arr[i] > largest_val) {
            largest_val = arr[i];
            largest_index = i;
        }
    }

    return largest_index;
}



int findLargestAbsoluteDifferenceIndex(int *intArray, float *floatArray, int size) {

	/*
	if (size <= 0 || intArray == NULL || floatArray == NULL) {
        return -1; // Handle invalid input
    }
    */

    float maxAbsoluteDiff = -1;
    int maxIndex = -1;

    for (int i = 0; i < size; i++) {
        float diff = (float)intArray[i] - floatArray[i];
        float absoluteDiff = fabs(diff); // Use fabs for float absolute value

        if (absoluteDiff > maxAbsoluteDiff) {
            maxAbsoluteDiff = absoluteDiff;
            maxIndex = i;
        }
    }

    return maxIndex;
}


int E8Decision(int *intLP1, float *floatLP2, float *floatArray, int size) {

	/*
	if (size <= 0 || intArray == NULL || floatArray == NULL) {
        return -1; // Handle invalid input
    }
    */

    float tempt_sum = 0;
    int Index = 0;

    for (int i = 0; i < size; i++) {
        tempt_sum = tempt_sum+ ((float)intLP1[i] - floatLP2[i])*((float)intLP1[i] + floatLP2[i]-floatArray[i]-floatArray[i]);
        //float absoluteDiff = fabs(diff); // Use fabs for float absolute value
    }

    if (tempt_sum < 0) {
        return Index=1;
    } else {
        return Index=2;
    }
}


int* DnDecoder(float arr[],int size) {
    // Allocate memory for the new array dynamically
    int* f = (int*)malloc(size * sizeof(int));
    int* g = (int*)malloc(size * sizeof(int));
    int i = 0;
    int totalSum=0;


    /*
    // Check if memory allocation was successful
    if (f == NULL) {
        printf("Memory allocation failed!\n");
        return NULL; // Return NULL to indicate an error
    }
    */

    //f =roundArrayToNearestInteger(arr, size);
    //g =roundArrayToNearestInteger(arr, size);
    //memcpy(g, f, size * sizeof(int));

    for (i = 0; i < size; i++) {
    	f[i] = (int)roundf(arr[i]);
    	g[i]=f[i];
    	totalSum=totalSum+f[i];
    }


    //int index =findLargestAbsoluteDifferenceIndex(f, arr, size);
    int index = -1;
    float maxAbsoluteDiff = -1;

        for (i = 0; i < size; i++) {
            float diff = (float)f[i] - arr[i];
            float absoluteDiff = fabs(diff); // Use fabs for float absolute value

            if (absoluteDiff > maxAbsoluteDiff) {
                maxAbsoluteDiff = absoluteDiff;
                index = i;
            }
        }

    /*
    float* result = subtract_arrays(f, arr, size);
    calculateAbsoluteValues(result, size); // Call the function
    int index = findIndexOfLargest(result, size);
    */


    if (arr[index]>f[index]) {
        g[index]=g[index]+1;
    } else {
    	g[index]=g[index]-1;
    }

    //int totalSum = sumArrayElements(f, size);
    if ((totalSum & 1) == 0) {
    //if (totalSum % 2 == 0) {
        return f;
    } else {
        return g;
    }

}


int main() {
    /* Initialization and definition*/
	//Setup cycle count
	*SCB_DEMCR |= 0x01000000;
	*DWT_LAR = 0xC5ACCE55; // enable access
	*DWT_CYCCNT = 0; // reset the counter

	//Example of input vector
	float y[] = { 1, 0.5000, 0.5000, 0.5000, 0.5000, 0.5000, 1.5000, 0.5000 };


	//Input to E8 lattice decoding
	float shift[] = { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5 };
	float shift_n[] = { -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5 };
	int arraySize = 8, flag = 0;
	int *LP1;
	float *LP2;

	//Other variables
	unsigned int t1, t2, numCycle1, numCycles;
	float *numbers_shift;
	int *tempt;

	printf("Input vector y: \n");
	printFloatArray(y, arraySize); // Pass the array and its size


    /*E8 lattice decoder:
     Step 1: call D8 decoding twice, obtain two estimated E8 lattice points LP1 and LP2
     Step 2: choose the closest E8 point to y, by comparing ||LP1-y|| and ||LP2-y||
    */


	//First call of D8 decoder, output the first estimated E8 point LP1
	*DWT_CONTROL |= 1; // enable the counter
	t1 = *DWT_CYCCNT;

	LP1 = DnDecoder(y, arraySize);

	t2 = *DWT_CYCCNT;
	*DWT_CONTROL |= 0; // close the counter
	numCycle1 = t2 - t1; //clock cycles for the first call of D8 lattice decoder

	//printf("Decoded the first estimated E8 lattice point: \n");
	//printIntArray(LP1, arraySize); // Pass the array and its size

	printf("clock cycles for the first call of D8 lattice decoder: %u \n",
			numCycle1);                 // Display the difference


	//Second call of D8 decoder, output the second estimated E8 point LP2
	*DWT_CONTROL |= 1; // enable the counter
	t1 = *DWT_CYCCNT;

	numbers_shift = subtract_arrays_double(y, shift, arraySize);
	tempt = DnDecoder(numbers_shift, arraySize);
	LP2 = subtract_arrays(tempt, shift_n, arraySize);

	//choose the closest E8 point to the input vector y, by comparing ||LP1-y|| and ||LP2-y||
	flag = E8Decision(LP1, LP2, y, arraySize);

	t2 = *DWT_CYCCNT;
	*DWT_CONTROL |= 0; // close the counter
	numCycles = numCycle1 + t2 - t1; // The total clock cycles for E8 Lattice CVP decoder

	//printf("Decoded the second estimated E8 lattice point: \n");
	//printFloatArray(LP2, arraySize); // Pass the array and its size

	printf("clock cycles for the second call of D8 lattice decoder: %u \n",
			t2 - t1);

	printf("The closest E8 lattice point to the input vector y: \n");
	if (flag == 1) {
		printIntArray(LP1, arraySize); //LP1 is the closest E8 point to y
	} else {
		printFloatArray(LP2, arraySize); //LP2 is the closest E8 point to y
	}

	printf("\n");
	printf("clock cycles for E8 lattice decoding: %u", numCycles);


	return EXIT_SUCCESS;
}
