#include "myann.h"

void feed_forward(
const double *input,
const double * weight,
const double *bias,
double *output,
const int INPUT_NODES,
const int OUTPUT_NODES,
activation_t activation) {
	for(int j=0;j<OUTPUT_NODES;j++) {
		double sum = 0.0;
		for(int i=0;i<INPUT_NODES;i++) {
			sum += input[i]*weight[i*OUTPUT_NODES+j];
		}
		sum += 1*bias[j];
		if(activation==LINEAR) {
			output[j] = sum;
		} else if(activation==SIGMOID) {
			output[j] = 1.0/(1.0+exp(-sum));
		} else if(activation==RELU) {
			output[j] = sum>0?sum:0;
		}
//		myPrint.printf("output[%d] = %f\n", j, output[j]);
	}
}

void back_propagation(
const double *output_b,
const double * weight,
const double * input_f,
double *input_b,
const int OUTPUT_NODES,
const int INPUT_NODES,
activation_t activation) {
	for(int j=0;j<INPUT_NODES;j++) {
		double sum = 0.0;
		for(int i=0;i<OUTPUT_NODES;i++) {
			sum += output_b[i]*weight[j*OUTPUT_NODES+i];
		}    
		if(activation==LINEAR) {
			input_b[j] = sum;
		} else if(activation==SIGMOID) {
			input_b[j] = input_f[j]*(1-input_f[j])*sum;
		} else if(activation==RELU) {
			input_b[j] = (input_f[j]>0?1:0)*sum;
		}
//		myPrint.printf("input_b[%d] = %f\n", j, input_b[j]);
	}
}

double get_error(
const double *target, 
const double *output,
const int OUTPUT_NODES,
loss_t loss) {
	double Error = 0.0;
	double error[OUTPUT_NODES];
	double sum_error = 0.0;
	for(int i=0;i<OUTPUT_NODES;i++) {
		if(loss == MSE) {    
			error[i] = 
			0.5*(output[i]-target[i])*(output[i]-target[i]);
		} else if(loss == CEE) {
			error[i] = -target[i]*log(output[i]);
		}
		// myPrint.printf("error[%d] = %f\n", i, error[i]);
		sum_error += error[i];    
	}
	Error = sum_error;
	// myPrint.printf("Error = %f\n", Error);
	return Error;
}

void get_DoutputE(
const double *target, 
const double *output,
double *DoutputE,
const int OUTPUT_NODES) {
	for(int i=0;i<OUTPUT_NODES;i++) {
		DoutputE[i] = (output[i]-target[i])*1;
//		myPrint.printf("DoutputE[%d] = %f\n", i, DoutputE[i]);   
	} 
}

void prepare_back_propagation(
const double *DoutputE, 
const double *output,
double *output_b,
const int OUTPUT_NODES,
activation_t activation) {
	for(int i=0;i<OUTPUT_NODES;i++) {        
		if(activation==LINEAR) {
			output_b[i] = DoutputE[i];
		} else if(activation==SIGMOID) {
			output_b[i] = output[i]*(1-output[i])*DoutputE[i];
		} else if(activation==RELU) {
			output_b[i] = (output[i]>0?1:0)*DoutputE[i];
		} 
//		myPrint.printf("output_b[%d] = %f\n", i, output_b[i]); 
	} 
}

void get_gradients(
double *DweightE,
double *DbiasE,
const double *output_b,
const double *input_f,  
const int F_INPUT_NODES,
const int OUTPUT_NODES) { 
	for(int j=0;j<OUTPUT_NODES;j++) {   
		for(int i=0;i<F_INPUT_NODES;i++) {
			DweightE[i*OUTPUT_NODES+j] = 
			input_f[i]*output_b[j];
//			myPrint.printf("DweightE[%d][%d] = %f\n",
//			i, j, DweightE[i*OUTPUT_NODES+j]);
		}
		DbiasE[j]= 1*output_b[j];   
//		myPrint.printf("DbiasE[%d] = %f\n", j, DbiasE[j]);
	}
}

void apply_gradients(
const double *DweightE,
const double *DbiasE,
double learning_rate,
double *weight,
double *bias, 
const int INPUT_NODES,
const int OUTPUT_NODES) { 
	for(int j=0;j<OUTPUT_NODES;j++) {   
		for(int i=0;i<INPUT_NODES;i++) {
			weight[i*OUTPUT_NODES+j] -= 
			learning_rate*DweightE[i*OUTPUT_NODES+j];
//			myPrint.printf("weight[%d][%d] = %f\n",
//			i, j, weight[i*OUTPUT_NODES+j]);
		}
		bias[j] -= learning_rate*DbiasE[j];       
//		myPrint.printf("bias[%d] = %f\n", j, bias[j]);
	}
}

const double INITIAL_WEIGHT_MAX = 0.5;

void initialize_weight(  
double *weight,
double *bias,
int INPUT_NODES,
int OUTPUT_NODES) {  
	double rand_num; 
	for(int j=0;j<OUTPUT_NODES;j++) {   
		for(int i=0;i<INPUT_NODES;i++) {
			rand_num = double(rand()%1000)/1000;
			weight[i*OUTPUT_NODES+j] = 
			2.0*(rand_num - 0.5)*INITIAL_WEIGHT_MAX;
//			myPrint.printf("%6.3f ", weight[i*OUTPUT_NODES+j]);
		}
		rand_num = double(rand()%1000)/1000;
		bias[j] = 2.0*(rand_num - 0.5)*INITIAL_WEIGHT_MAX;
//		myPrint.printf("%6.3f ", bias[j]); 
//		myPrint.printf("\n");
	}
//	myPrint.printf("\n");
}
