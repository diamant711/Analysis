#include <iostream>
#include <TF1.h>
#include <cstring>
#include <cmath>
#include <fstream>
#include "symbolicc++.h"

#define FILE_LOG ".analysis-history.log"

using namespace std;

typedef struct propagation_data{
	char *formula;
	double **parameters;
	int num_par = 0;
	char **parameters_name;
	double result;
} propagation_data;

static void save_propagation_data(propagation_data *data);

static bool restore_propagation_data(propagation_data *data);

static char* replace_param(propagation_data *data, int par, char *mod_formula);

static void compatibility_notation(propagation_data *data);

void propagation_data_in_parser(propagation_data *data);

void propagation_data_calculus(propagation_data *data);

void propagation_data_output(propagation_data *data);
