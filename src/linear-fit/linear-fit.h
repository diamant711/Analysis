#include<fstream>

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

int data_in_parser(linear_fit_parameters* fit_data, char* path);

