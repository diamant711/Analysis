#include <iostream>
#include "linear-fit/linear-fit.h"
#include "propagation-of-uncertainty-calculator/propagation-of-uncertainty-calculator.h"

using namespace std;

void startpage(void);

int main(int argc, char *argv[]){
	startpage();

	return 0;
}

void startpage(void){
	cout << "**********************************************************************" << endl
			 << "****************** Analysis Physichist-Friendly Hub ******************" << endl
			 << "**********************************************************************" << endl;
}
