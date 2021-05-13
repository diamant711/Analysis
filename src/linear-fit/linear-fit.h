#include <fstream>
#include <cmath>
#include "TCanvas.h"
#include "TApplication.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TF1.h"

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
} linear_fit_parameters;

int linear_fit_data_in_parser(linear_fit_parameters* fit_data, char* path);

int linear_fit_calculus(linear_fit_parameters* fit_data);

int linear_fit_output(linear_fit_parameters* fit_data);
