#include <iostream>
#include "linear-fit/linear-fit.h"
#include "propagation-of-uncertainty-calculator/propagation-of-uncertainty-calculator.h"

#define LINEAR_FIT_SEL 'f'
#define P_O_U 'p'

using namespace std;

void help(void);

int main(int argc, char *argv[]){
  switch(argv[1][1]){
		case LINEAR_FIT_SEL: {
				linear_fit_parameters* fit = new linear_fit_parameters;
				if(data_in_parser(fit, argv[2]) == -1)
					return -1;
				//calculus
				//output
				delete[] fit->data_in[0];
				delete[] fit->data_in[1];
				delete[] fit->data_in[2];
				delete[] fit->data_in[3];
				delete[] fit->data_in;
				delete fit;
			break;
		}
		case P_O_U: {
				//argument parser
				//calculus
				//output
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
			 << endl
			 << "only one argument at a time is allowed if multiple arguments are insert the program" << endl
			 << "only take the first." << endl
			 << endl 
			 << "OPTION-RELEATED ARGUMENTS" << endl
			 << "  -f\t\t[path to a file contenent data in plain text] the format of the file must is" << endl
			 << "    \t\tthis:    x\ty\tsigma_x\t\tsigma_y" << endl 
			 << "  -p #da fare#" << endl;
}
