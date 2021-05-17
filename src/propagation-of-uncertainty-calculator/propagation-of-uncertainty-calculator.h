#include <iostream>
#include "TF1.h"

using namespace std;

typedef struct propagation_data{
	char *formula;
	double **parameters;
	int num_par = 0;
	char **parameters_name;
	double result;
} propagation_data;

void propagation_data_in_parser(propagation_data *data);

void propagation_data_calculus(propagation_data *data);
