#include "linear-fit.h"
#include <cmath>

int data_in_parser(linear_fit_parameters *fit_data, char *path){
	int len;
	fit_data->input_file_path = path;
  string tmp;
	ifstream file_in;
  file_in.open(fit_data->input_file_path);
	if(!file_in.good()){
  	return -1;
  }
  fit_data->dots = 0;
  getline(file_in, tmp);
  while(!file_in.eof()){
  	(fit_data->dots)++;
    getline(file_in, tmp);
  }
  if(fit_data->dots == 0){
    file_in.close();
    return -1;
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
	return 0;
}

int linear_fit_calculus(linear_fit_parameters* fit_data){
	double s_tot[fit_data->dots], m_test, s_w = 0, s_x = 0, s_xx = 0, s_y = 0, s_xy = 0, delta;
	m_test = (fit_data->data_in[1][fit_data->dots] - fit_data->data_in[1][0]) /
					 (fit_data->data_in[0][fit_data->dots] - fit_data->data_in[0][0]);
	for(int i = 0; i < fit_data->dots; i++){
		s_tot[i] = sqrt(pow(fit_data->data_in[3][i], 2) + pow(m_test * fit_data->data_in[2][i], 2));
		s_w += 1/pow(s_tot[i], 2);
		s_x += (fit_data->data_in[0][i]) / pow(s_tot[i], 2);
		s_xx += pow(fit_data->data_in[0][i], 2) / pow(s_tot[i], 2);
		s_y += (fit_data->data_in[1][i]) / pow(s_tot[i], 2);
		s_xy += (fit_data->data_in[1][i] * fit_data->data_in[0][i]) / pow(s_tot[i], 2);	
	}
	delta = s_xx * s_w + s_x * s_x;
	fit_data->q = (s_xx * s_y + s_xy * s_x) / delta;
	fit_data->m = (s_xy * s_w + s_x * s_y) / delta;
	fit_data->sigma_m = sqrt(s_xx/delta);
	fit_data->sigma_q = sqrt(s_w/delta);
	return 0;
}
