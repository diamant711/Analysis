#include "TGraphErrors.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TAxis.h"
#include <iostream>
#include <cmath>

using namespace std;

typedef struct {
	double **in_results;
	bool compatibility;
	double best_approximation[2];
	int num_data_in;
} best_data;

void best_in_results_parser(best_data *data);

static void compatibility_check(best_data *data);

void avarage_calculus(best_data *data);

void output_compatibility_graph_best_approximation(best_data *data);
