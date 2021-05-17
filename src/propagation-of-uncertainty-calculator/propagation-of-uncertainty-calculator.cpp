#include "propagation-of-uncertainty-calculator.h"
#include <cstring>

static char* replace(propagation_data *data, int par, char *mod_formula){
	mod_formula = new char[strlen(data->formula) - 2];
	for(int i = 0; data->formula[i] != '\0'; i++){
		if(data->formula[i] == '['){
			if(data->formula[i + 2] == ']'){
				for(int j = 0; j < i; j++)
					mod_formula[j] = data->formula[j];
				mod_formula[i] = 'x';
				int k = 1;
				for(int j = i + 3; mod_formula[j] != '\0'; j++){
					mod_formula[j - 2] = mod_formula[j];
					k++;
				}
				mod_formula[i + k] = '\0';
			}
		}
	}
	return mod_formula;
}

void propagation_data_in_parser(propagation_data *data){
	cout << "Inserire la relazione: ";
	data->formula = new char[100];
	cin >> data->formula;
	for(int i = 0; data->formula[i] != '\0'; i++){
		if(data->formula[i] == '['){
			if (data->formula[i + 2] == ']') {
				data->num_par = (data->formula[i + 1] - 48 + 1);
			}
		}
	}
	data->parameters = new double*[data->num_par];
	data->parameters_name = new char*[data->num_par];
	for(int i = 0; i < data->num_par; i++){
		cout << "Nome parametro [" << i << "] : ";
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
	char *mod_formula;
	cout << "formula = " << data->formula << " ; mod_formula = " << *(replace(data, 0, mod_formula)) << endl;
	delete[] mod_formula;
}
