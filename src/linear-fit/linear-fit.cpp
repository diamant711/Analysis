#include "linear-fit.h"

linear_fit_parameters* data_in_parser(linear_fit_parameters *fit_data, char *path){
	int len;
	fit_data->input_file_path = path;
  string tmp;
	ifstream file_in;
  file_in.open(fit_data->input_file_path);
	if(!file_in.good()){
  	return NULL;
  }
  fit_data->dots = 0;
  getline(file_in, tmp);
  while(!file_in.eof()){
  	(fit_data->dots)++;
    getline(file_in, tmp);
  }
  if(fit_data->dots == 0){
    file_in.close();
    return NULL;
  }
  file_in.clear();
  file_in.seekg(0, ios::beg);
	fit_data->data_in = new double*[4];
	fit_data->data_in[0] = new double[fit_data->dots];
	fit_data->data_in[1] = new double[fit_data->dots];
	fit_data->data_in[2] = new double[fit_data->dots];
	fit_data->data_in[3] = new double[fit_data->dots];
	for(int i = 0; i < fit_data->dots; i++)
		file_in >> fit_data->data_in[0][i] >> fit_data->data_in[1][i]
						>> fit_data->data_in[2][i] >> fit_data->data_in[3][i];
	file_in.close();
	return fit_data;
}
