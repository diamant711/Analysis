#include "best.h"
#include <cmath>

void best_in_results_parser(best_data *data){
	cout << "How much data will be entered?";
	cin >> data->num_data_in;
	data->in_results = new double*[data->num_data_in];
	for(int i = 0; i < data->num_data_in; i++){
		data->in_results[i] = new double[2];
		cout << i + 1 << "° Result = ";
		cin >> data->in_results[i][0];
		cout << "Uncertainty" << i << " = ";
		cin >> data->in_results[i][1];
	}
}

static void compatibility_check(best_data *data){
	for(int i = 0; i < data->num_data_in; i++){
		for(int j = 0; j < data->num_data_in; j++){
			if(i != j){
				if(data->in_results[i][0] < data->in_results[j][0]){
					if((data->in_results[i][0] + data->in_results[i][1]) < (data->in_results[j][0] - data->in_results[j][1])){
						data->compatibility = false;
						return;
					}
				} else {
					if((data->in_results[i][0] - data->in_results[i][1]) > (data->in_results[j][0] + data->in_results[j][1])){
						data->compatibility = false;
						return;
					}
				}
			}
		}
	}
	data->compatibility = true;
}

void avarage_calculus(best_data *data){
	double partial = 0;
	double err_partial = 0;
	compatibility_check(data);
	if(data->compatibility){
		for(int i = 0; i < data->num_data_in; i++){
			partial += data->in_results[i][0]*(1/pow(data->in_results[i][1], 2));
			err_partial += 1/pow(data->in_results[i][1], 2);
		}
		data->best_approximation[0] = partial/err_partial;
		data->best_approximation[1] = sqrt(1/err_partial);
	} else {
		return;
	}
}

void output_compatibility_graph_best_approximation(best_data *data){
	if(data->compatibility){
		cout << "The results are compatible! The best aproximation is " << data->best_approximation[0] << " +- " << data->best_approximation[1] << endl;
	} else {
		cout << "The results are NOT compatible!" << endl;
	}

	/*
	TApplication *app = new TApplication("Grafici",0,0); 
	
	TCanvas *c1 = new TCanvas("c1","Linear Fit Graph",200,10,700,500);

  c1->SetGrid();

	double n = 1;
	double v[data->num_data_in];
	for(int i = 0; i < data->num_data_in; i++)
		v[i] = n * i + 1;

	TGraphErrors *gr_x_err = new TGraphErrors(data->num_data_in, data->in_results[0], v,
																														 data->in_results[1], NULL);
  gr_x_err->SetMarkerColor(4);
  gr_x_err->SetMarkerStyle(20);
	char title[100];
	sprintf(title, "best = %lf +- %lf ", data->best_approximation[0], data->best_approximation[1]);
	
	gr_x_err->SetTitle(title);
  gr_x_err->GetXaxis()->SetTitle("");
	gr_x_err->GetXaxis()->SetTitle("");
  int max = 0, min;
	for(int i = 0; i < data->num_data_in; i++){
		if(abs(data->in_results[0][i]) + data->in_results[0][i] > max)
			max = data->in_results[0][i];
	}
	min = max;
	for(int i = 0; i < data->num_data_in; i++){
		if(data->in_results[0][i] - data->in_results[1][i] < min)
			min = data->in_results[0][i] - data->in_results[1][i];
	}
	TAxis *axis = gr_x_err->GetXaxis();

  axis->SetLimits(0,data->num_data_in + 1);
  
	gr_x_err->Draw("AP");
	
	//c1->Update();

	app->Run();
	*/
}
