#include <iostream>
#include "linear-fit/linear-fit.h"
#include "propagation-of-uncertainty-calculator/propagation-of-uncertainty-calculator.h"
#include "best/best.h"

#define LINEAR_FIT_SEL 'f'
#define P_O_U 'p'
#define BEST 'b'
#define HELP 'h'

using namespace std;

void help(void);

int main(int argc, char *argv[]){
  switch(argv[1][1]){
		case LINEAR_FIT_SEL: {
				linear_fit_parameters* fit = new linear_fit_parameters;
				if(linear_fit_data_in_parser(fit, argv[2]) == -1)
					return -1;
				if(linear_fit_calculus(fit) == -1)
					return -1;
				if(linear_fit_output(fit, argv[3], argv[4]) == -1)
					return -1;
				delete[] fit->data_in[0];
				delete[] fit->data_in[1];
				delete[] fit->data_in[2];
				delete[] fit->data_in[3];
				delete[] fit->data_in;
				delete[] fit->s_tot;
				delete fit;
			break;
		}
		case P_O_U: {
				propagation_data *prop_data = new propagation_data;
				propagation_data_in_parser(prop_data);
				propagation_data_calculus(prop_data);
				propagation_data_output(prop_data);
				for(int i = 0; i < prop_data->num_par; i++){
					delete[] prop_data->parameters_name[i];
					delete[] prop_data->parameters[i];
				}
				delete[] prop_data->parameters;
				delete[] prop_data->parameters_name;
				delete[] prop_data->formula;
				delete prop_data; 
			break;
	  }
		case BEST: {
				best_data *data = new best_data; 
				best_in_results_parser(data);
				avarage_calculus(data);
				output_compatibility_graph_best_approximation(data);
				for(int i = 0; i < data->num_data_in; i++)
					delete[] data->in_results[i];
				delete[] data->in_results;
				delete data;
			break;
		}
		case HELP: {
				help();
			break;
		}	
		default: {
				help();
				return -1;
			break;
		}
	}
	return 0;
}

void help(){
	cout << "Analisys: Physichist-friendly software for automated and repetitive calculus" << endl
			 << endl
			 << "Usage :" << endl
			 << "  analysis [option] [option-releated arguments]" << endl
			 << endl
			 << "OPTION" << endl
			 << "  -f\t\tfor linear fit of a list of point" << endl
			 << "  -p\t\tfor propagation of uncertainty through a mathematical relation" << endl
			 << "  -b\t\tfor compatibility check & best approximation" << endl
			 << endl
			 << "only one argument at a time is allowed if multiple arguments are insert the program" << endl
			 << "only take the first." << endl
			 << endl 
			 << "OPTION-RELEATED ARGUMENTS" << endl
			 << "  -f\t\t[path to a file contenent data in plain text] the format of the file must is" << endl
			 << "    \t\tthis:    x\ty\tsigma_x\t\tsigma_y" << endl 
			 << endl
			 << "NOTE: the -p & -b option trigger an interactive Q/A in order to parse input from user," << endl
			 << "so there aren't option-releated arguments for these"
			 << endl;
}
