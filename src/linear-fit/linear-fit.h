#include <fstream>
#include <cmath>
#include "TCanvas.h"
#include "TApplication.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TF1.h"
#include <cstring>
#include <cstdio>

using namespace std;

typedef struct {
	bool weights;
	char *input_file_path;
	double **data_in;
	int dots;
	double m;
	double q;
	double sigma_m;
	double sigma_q;
	double test_x2;
	double test_x2_r;
} linear_fit_parameters;

static const char* ConvertDoubleToString(double value);

int linear_fit_data_in_parser(linear_fit_parameters* fit_data, char* path);

int linear_fit_calculus(linear_fit_parameters* fit_data);

int linear_fit_output(linear_fit_parameters* fit_data, char *x_title, char *y_title);
