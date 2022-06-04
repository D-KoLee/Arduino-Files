#ifndef _MY_ACTIVATION_H_
#define _MY_ACTIVATION_H_

typedef enum _activation {
  LINEAR=0,
  SIGMOID=1,
  RELU=2,
} activation_t;

typedef enum _loss {
  MSE=0,
  CEE=1,
} loss_t;

#endif//_MY_ACTIVATION_H_
