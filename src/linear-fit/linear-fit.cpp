#include "linear-fit.h"

static void compatibility_notation(linear_fit_parameters *fit_data){
	char **file_ram;
	int line = 0;
	string tmp;
	ifstream file_in;
	ofstream file_out;
	file_in.open(fit_data->input_file_path);
	if(!file_in.good())
		return;
	getline(file_in, tmp);
	while(!file_in.eof()){
		line++;
		getline(file_in, tmp);
	}
  file_in.clear();
  file_in.seekg(0, ios::beg);	
	file_ram = new char*[line];
	for(int i = 0; i < line; i++){
		getline(file_in, tmp);
		file_ram[i] = new char[strlen(tmp.c_str()) + 1];
		memcpy(file_ram[i], tmp.c_str(), strlen(tmp.c_str()) + 1);
	}
	for(int i = 0; i < line; i++){
		for(int j = 0; file_ram[i][j] != '\0'; j++){
			if(file_ram[i][j] == ',')
				file_ram[i][j] = '.';
		}
	}
	file_in.close();
	file_out.open(fit_data->input_file_path);
	if(!file_out.good())
		return;
	for(int i = 0; i < line; i++){
		file_out << file_ram[i];
		delete[] file_ram[i];
		file_out << endl;
	}
	file_out.close();
	delete[] file_ram;
}

int linear_fit_data_in_parser(linear_fit_parameters *fit_data, char *path){
	int len;
	fit_data->input_file_path = path;
  compatibility_notation(fit_data);
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
	double m_test, s_w = 0, s_x = 0, s_xx = 0, s_y = 0, s_xy = 0, delta;
	fit_data->s_tot = new double[fit_data->dots];
	m_test = (fit_data->data_in[1][fit_data->dots - 1] - fit_data->data_in[1][0]) /
					 (fit_data->data_in[0][fit_data->dots - 1] - fit_data->data_in[0][0]);
	for(int i = 0; i < fit_data->dots; i++){
		fit_data->s_tot[i] = sqrt(pow(fit_data->data_in[3][i], 2) + pow(m_test * fit_data->data_in[2][i], 2));
		s_w += 1/pow(fit_data->s_tot[i], 2);
		s_x += (fit_data->data_in[0][i]) / pow(fit_data->s_tot[i], 2);
		s_xx += pow(fit_data->data_in[0][i], 2) / pow(fit_data->s_tot[i], 2);
		s_y += (fit_data->data_in[1][i]) / pow(fit_data->s_tot[i], 2);
		s_xy += (fit_data->data_in[1][i] * fit_data->data_in[0][i]) / pow(fit_data->s_tot[i], 2);
	}
	delta = (s_xx * s_w) - (s_x * s_x);
	fit_data->q = (s_xx * s_y - s_xy * s_x) / delta;
	fit_data->m = (s_xy * s_w - s_x * s_y) / delta;
	fit_data->sigma_q = sqrt(s_xx/delta);
	fit_data->sigma_m = sqrt(s_w/delta);
	fit_data->test_x2 = 0;
	for(int i = 0; i < fit_data->dots; i++){
		fit_data->test_x2 += pow( (fit_data->data_in[1][i] - (fit_data->data_in[0][i] * fit_data->m + fit_data->q)) / fit_data->s_tot[i], 2);
	}
	fit_data->test_x2_r = fit_data->test_x2 / (fit_data->dots - 2);
	return 0;
}

int linear_fit_output(linear_fit_parameters *fit_data, char *x_title, char *y_title){
 	TApplication *app = new TApplication("Grafici",0,0); 
	
	TCanvas *c1 = new TCanvas("c1","Linear Fit Graph",200,10,700,500);

  c1->SetGrid();
 
	TGraphErrors *gr_xy_err = new TGraphErrors(fit_data->dots, fit_data->data_in[0], fit_data->data_in[1],
																														 NULL, fit_data->s_tot);
  gr_xy_err->SetMarkerColor(4);
  gr_xy_err->SetMarkerStyle(20);
	char title[100];
	sprintf(title, "m = %lf +- %lf // q = %lf +- %lf // ~x2 = %lf // GDL = %d", fit_data->m, 
																																							 fit_data->sigma_m, 
																																							 fit_data->q, 
																																							 fit_data->sigma_q, 
																																							 fit_data->test_x2_r,
																																							 fit_data->dots - 2);
	gr_xy_err->SetTitle(title);
  gr_xy_err->GetXaxis()->SetTitle(x_title);
  gr_xy_err->GetYaxis()->SetTitle(y_title);
	gr_xy_err->Draw("AP");

	TF1 *func_fit = new TF1("linear_fit", "[0]*x+[1]");

	func_fit->SetParameter(0, fit_data->m);
	func_fit->SetParameter(1, fit_data->q);
  func_fit->SetLineColor(2);
	func_fit->SetLineWidth(1);
	func_fit->Draw("SAME");
	
	gr_xy_err->Draw("P SAME");
	
	//c1->Update();

	app->Run();

	return 0;
}
