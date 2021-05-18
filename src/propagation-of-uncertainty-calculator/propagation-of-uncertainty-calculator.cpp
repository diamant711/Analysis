#include "propagation-of-uncertainty-calculator.h"
#include <cstdlib>

static void save_propagation_data(propagation_data *data){
	ofstream file_out;
	file_out.open(FILE_LOG);
	if(!file_out.good()){
		cout << "Error on opening file" << endl;
	}
	file_out << data->formula << endl;
	file_out << data->num_par << endl;
	for(int i = 0; i < data->num_par; i++)
		file_out << data->parameters[i][0] << ' ' << data->parameters[i][1] << endl;
	for(int i = 0; i < data->num_par; i++)
		file_out << data->parameters_name[i] << endl;
}

static bool restore_propagation_data(propagation_data *data){
	char sel;
	ifstream file_in;
	file_in.open(FILE_LOG);
	if(!file_in.good()){
		return 0;
	}
	data->formula = new char[100];
	file_in >> data->formula;
	file_in >> data->num_par;
	data->parameters = new double*[data->num_par];
	for(int i = 0; i < data->num_par; i++){
		data->parameters[i] = new double[2];
		file_in >> data->parameters[i][0] >> data->parameters[i][1];
	}
	data->parameters_name = new char*[data->num_par];
	for(int i = 0; i < data->num_par; i++){
		data->parameters_name[i] = new char[6];
		file_in >> data->parameters_name[i];
	}
	cout << "Formula = " << data->formula << endl;
	for(int i = 0; i < data->num_par; i++)
		cout << "[" << i << "] ~ " << data->parameters_name[i] << " = " 
				 << data->parameters[i][0] << " +- " << data->parameters[i][1] << endl;
	while(1){	
		cout << "Usa questi dati? [s/m/n] ";
		cin >> sel;
		switch (sel) {
			case 's':
				return 1;
			break;
		
			case 'm':
				int sel2;
				cout << "Cosa vuoi modificare? Relazione (-1), parametri (0:" << data->num_par - 1 << "): ";
				cin >> sel2;
				if(sel2 <= -1) {
					cout << "Nuova relazione = ";
					cin >> data->formula;
					save_propagation_data(data);
					return 1;
				} else if(sel2 >= 0) {
					cout << data->parameters_name[sel2] << " = ";
					cin >> data->parameters[sel2][0];
					cout << "Sigma " << data->parameters_name[sel2] << " = ";
					cin >> data->parameters[sel2][1];
					save_propagation_data(data);
					return 1;
				} else {
					return 0;
				}
			break;

			case 'n':
				for(int i = 0; i < data->num_par; i++){	
        	delete[] data->parameters_name[i];
        	delete[] data->parameters[i];
        }
        delete[] data->parameters;
        delete[] data->parameters_name;
		  	delete[] data->formula;
        return 0;
			break;

			default:
				cout << "Selezione errata, riprova" << endl;
			break;
		}
	}
}

static char* replace_param(propagation_data *data, int par, char *mod_formula){
	mod_formula = new char[strlen(data->formula) - 2];
	for(int i = 0; data->formula[i] != '\0'; i++){
		if(data->formula[i] == '['){
			if(data->formula[i + 1] == (par + 48)){	
				if(data->formula[i + 2] == ']'){
					for(int j = 0; j < i; j++)
						mod_formula[j] = data->formula[j];
					mod_formula[i] = 'x';
					int k = 1;
					for(int j = i + 3; data->formula[j] != '\0'; j++){
						mod_formula[j - 2] = data->formula[j];
						k++;
					}
					mod_formula[i + k] = '\0';
				}
			}
		}
	}
	return mod_formula;
}

void propagation_data_in_parser(propagation_data *data){
	if(!restore_propagation_data(data)){
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
		save_propagation_data(data);
	}
}

void propagation_data_calculus(propagation_data *data){
	TF1 *func;
	double partial_result = 0;
	char *mod_formula = NULL;
	for(int i = 0; i < data->num_par; i++){
		func = new TF1("func", replace_param(data, i, mod_formula));
		for(int j = 0; j < data->num_par; j++)
			if(i != j)
				func->SetParameter(j, data->parameters[j][0]);
		partial_result += pow(func->Derivative(data->parameters[i][0]), 2) * pow(data->parameters[i][1], 2);
		delete func;
	}
	delete[] mod_formula;
	data->result = sqrt(partial_result);
}

void propagation_data_output(propagation_data *data){
	char *mod_formula;
	TF1 *func = new TF1("func", replace_param(data, 0, mod_formula));
	for(int i = 1; i < data->num_par; i++)
		func->SetParameter(i, data->parameters[i][0]);
	cout << endl << "Result = " << func->Eval(data->parameters[0][0]) << " +- " << data->result << endl;
	delete func;
	delete[] mod_formula;
}
