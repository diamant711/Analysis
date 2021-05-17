#include "propagation-of-uncertainty-calculator.h"

void propagation_data_in_parser(propagation_data *data){
	cout << "Inserire la relazione: ";
	data->formula = new char[100];
	cin >> data->formula;
	cout << "Quanti parametri ha la relazione? ";
	cin >> data->num_par;
	data->parameters = new double*[data->num_par];
	data->parameters_name = new char*[data->num_par];
	for(int i = 0; i < data->num_par; i++){
		cout << "Nome " << i+1 << "° parametro :";
		data->parameters_name[i] = new char[6];
		cin >> data->parameters_name[i];
		cout << data->parameters_name[i] << " = ";
		data->parameters[i] = new double[2];
		cin >> data->parameters[i][0];
		cout << "Sigma " << data->parameters_name[i] << " = ";
		cin >> data->parameters[i][1];
	}
}

void propagation_data_calculus(propagation_data *data){
	double num_diff[data->num_par];
	for(int i = 0; i < data->num_par; i++){
		
	}
}
