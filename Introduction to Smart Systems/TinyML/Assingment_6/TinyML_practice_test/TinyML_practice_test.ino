#include "myann.h"
#include <stdio.h>
#include <stdarg.h>
#include <PrintEx.h>
PrintEx myPrint = Serial;

const int PATTERN_COUNT = 10;
const int INPUT_NODES = 7;
const int HIDDEN_NODES = 4;
const int OUTPUT_NODES = 4;

double sumError = 0;

double input[PATTERN_COUNT][INPUT_NODES] = {
  { 1, 1, 1, 1, 1, 1, 0 }, // 0
  { 0, 1, 1, 0, 0, 0, 0 }, // 1
  { 1, 1, 0, 1, 1, 0, 1 }, // 2
  { 1, 1, 1, 1, 0, 0, 1 }, // 3
  { 0, 1, 1, 0, 0, 1, 1 },
  { 1, 0, 1, 1, 0, 1, 1 },
  { 0, 0, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 0, 0, 1, 1 }
};
double target[PATTERN_COUNT][OUTPUT_NODES] = {
  { 0, 0, 0, 0 },
  { 0, 0, 0, 1 },
  { 0, 0, 1, 0 },
  { 0, 0, 1, 1 },
  { 0, 1, 0, 0 },
  { 0, 1, 0, 1 },
  { 0, 1, 1, 0 },
  { 0, 1, 1, 1 },
  { 1, 0, 0, 0 },
  { 1, 0, 0, 1 }
};
double hidden[HIDDEN_NODES];
double output[PATTERN_COUNT][OUTPUT_NODES];

//sum error : 0.01025
//weightH[INPUT_NODES][HIDDEN_NODES]
//-0.98500 -2.40861 -2.44881 -2.66809 -6.38802 1.70272 -2.34095 -1.01652 -0.14528 0.58013
//-1.39443 0.67178 -0.02019 0.57071 -6.40436 -3.94004 -0.64208 -1.68291 -1.16441 -2.38147
//biasH[HIDDEN_NODES]
//0.97580
//-0.43677
//0.54038
//-0.09797
//2.44142
//-0.57624
//1.38470
//1.33798
//-0.78977
//-0.36035
//weightO[HIDDEN_NODES][OUTPUT_NODES]
//1.86710
//1.94931
//2.30209
//2.23881
//-9.67021
//3.77213
//2.57097
//2.18267
//0.73973
//1.90458
//biasO[OUTPUT_NODES]
//-4.75264


double weightH[INPUT_NODES][HIDDEN_NODES] = {
  { -2.91703, 5.41097, -5.26981, -4.69735 },
  { 0.90888, -1.66985, -2.51382, -1.72516 },
  { 0.09756, 0.92623, 4.96570, -2.96621 },
  { 4.05811, -1.53781, 2.23749, 1.29215 },
  { 0.99279, -4.71496, -3.62960, 2.94658 },
  { -4.92638, -4.20094, -0.15394, -0.82885 },
  { -2.10893, - 0.74264, - 4.51816, 3.71595 }
};
double biasH[HIDDEN_NODES] = {
  1.53899,
  -0.04174,
  4.89612,
  0.39245
};
double weightO[HIDDEN_NODES][OUTPUT_NODES] = {
  { -5.43054, -8.08971, 2.50232, 1.39708 },
  { -0.46309, 4.22896, 9.48748, 14.40990 },
  { -8.30426, 8.43746, -1.36867, 0.23920 },
  { -3.25865, 4.17170, 10.13540, -3.03200 }
};
double biasO[OUTPUT_NODES] = {
  4.15727,
  -5.35873,
  -6.67808,
  -3.24029
};


void setup() {
  Serial.begin(9600);
  delay(1000);

  for (int pc = 0; pc < PATTERN_COUNT; pc++) {

    feed_forward(input[pc],
                 (const double *)weightH,
                 biasH,
                 hidden,
                 INPUT_NODES,
                 HIDDEN_NODES,
                 SIGMOID);

    feed_forward(hidden,
                 (const double *)weightO,
                 biasO,
                 output[pc],
                 HIDDEN_NODES,
                 OUTPUT_NODES,
                 SIGMOID);

    double Error =
      get_error(target[pc], output[pc],
                OUTPUT_NODES, MSE);

    sumError += Error;
  }

  myPrint.printf("sum error : %.5f\n", sumError);
  myPrint.printf("weightH[INPUT_NODES][HIDDEN_NODES]\n");
  for (int i = 0; i < INPUT_NODES; i++) {
    for (int j = 0; j < HIDDEN_NODES; j++) {
      myPrint.printf("%7.3f ", weightH[i][j]);
    }
    myPrint.printf("\n");
  }
  myPrint.printf("biasH[HIDDEN_NODES]\n");
  for (int i = 0; i < HIDDEN_NODES; i++) {
    myPrint.printf("%7.3f ", biasH[i]);
    myPrint.printf("\n");
  }
  myPrint.printf("weightO[HIDDEN_NODES][OUTPUT_NODES]\n");
  for (int i = 0; i < HIDDEN_NODES; i++) {
    for (int j = 0; j < OUTPUT_NODES; j++) {
      myPrint.printf("%7.3f ", weightO[i][j]);
    }
    myPrint.printf("\n");
  }
  myPrint.printf("biasO[OUTPUT_NODES]\n");
  for (int i = 0; i < OUTPUT_NODES; i++) {
    myPrint.printf("%7.3f ", biasO[i]);
    myPrint.printf("\n");
  }


  myPrint.printf("\n");

  for (int pc = 0; pc < PATTERN_COUNT; pc++) {
    myPrint.printf("target %d : ", pc);
    for (int on = 0; on < OUTPUT_NODES; on++) {
      myPrint.printf("%.0f ", target[pc][on]);
    }
    myPrint.printf("pattern %d : ", pc);
    for (int on = 0; on < OUTPUT_NODES; on++) {
      myPrint.printf("%.2f ", output[pc][on]);
    }
    myPrint.printf("\n");
  }
}

void loop() {

}
