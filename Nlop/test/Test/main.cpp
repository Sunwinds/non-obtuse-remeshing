
//C version

#include <math.h>
#include <stdio.h>
#include <nlopt.h>
#include <time.h>
#include <iostream>

#define M_PI 3.141592653589793

typedef struct {
	double x, y, z;
} my_constraint_data;

typedef struct {
	double x1, y1, z1, x2, y2, z2;
} my_constraint_data1;

double myfunc(unsigned n, const double *x, double *grad, void *idealPoints)
{
	double val;

    my_constraint_data *d = (my_constraint_data *)idealPoints; 

    val = (x[0] - d->x) * (x[0] - d->x) + (x[1] - d->y) * (x[1] - d->y) + (x[2] - d->z) * (x[2] - d->z);
	
	if (grad) {
		grad[0] = 2.0 * (x[0] - d->x);
		grad[1] = 2.0 * (x[1] - d->y);
		grad[2] = 2.0 * (x[2] - d->z);
	}

	return val;
}


double myconstraint(unsigned n, const double *x, double *grad, void *points)
{
	/*
	double x1,x2,y1,y2,z1,z2;
    double val;
    
	my_constraint_data1 *d = (my_constraint_data1 *)points;
    x1 = d->x1; x2 = d->x2;
	y1 = d->y1; y2 = d->y2;
	z1 = d->z1; z2 = d->z2;
    
    val = (x1 - x[0]) * (x2 - x[0]) + (y1 - x[1]) * (y2 - x[1]) + (z1 - x[2]) * (z2 - x[2]);
    if(grad)
	{
		grad[0] = (x1 - x[0]) + (x2 - x[0]);
		grad[1] = (y1 - x[1]) + (y2- x[1]);
		grad[2] = (z1- x[2]) + (z2 - x[2]);
	}

    return -1.0 * val;
    */
	
	double x1,x2,y1,y2,z1,z2;
	double dot,length1,length2;
	double val;

	my_constraint_data1 *d = (my_constraint_data1 *)points;
	x1 = d->x1; x2 = d->x2;
	y1 = d->y1; y2 = d->y2;
	z1 = d->z1; z2 = d->z2;
	
	//std::cout << x1 << " " << y1 << " " << z1 << "    " << x2 << " " << y2 << " " << z2 << std::endl; 
	dot = (x1 - x[0]) * (x2 - x[0]) + (y1 - x[1]) * (y2 - x[1]) + (z1 - x[2]) * (z2 - x[2]);
	length1 = sqrt((x1 - x[0]) * (x1 - x[0]) + (y1 - x[1]) * (y1 - x[1]) + (z1 - x[2]) * (z1 - x[2]));
	length2 = sqrt((x2 - x[0]) * (x2 - x[0]) + (y2 - x[1]) * (y2 - x[1]) + (z2 - x[2]) * (z2 - x[2]));

	if(grad)
	{   
		double term1,term2,term3;

		term1 = -1.0 * ((x1 - x[0]) + (x2 - x[0])) / (length1 * length2);
		term2 = dot / length2 * (x1 - x[0]) * pow(length1, -3.0);
		term3 = dot / length1 * (x2 - x[0]) * pow(length2, -3.0);
		grad[0] = -1.0 * (term1 + term2 + term3);

		term1 = -1.0 * ((y1 - x[1]) + (y2 - x[1])) / (length1 * length2);
		term2 = dot / length2 * (y1 - x[1]) * pow(length1, -3.0);
		term3 = dot / length1 * (y2 - x[1]) * pow(length2, -3.0);
		grad[1] = -1.0 * (term1 + term2 + term3);

		term1 = -1.0 * ((z1 - x[2]) + (z2 - x[2])) / (length1 * length2);
		term2 = dot / length2 * (z1 - x[2]) * pow(length1, -3.0);
		term3 = dot / length1 * (z2 - x[2]) * pow(length2, -3.0);
		grad[2] = -1.0 * (term1 + term2 + term3);
	}

	val = -1.0 * dot / (length1 * length2);
    
	//std::cout << val << std::endl;
	return val;
	
}


double myconstraint_1(unsigned n, const double *x, double *grad, void *points)
{
	double x1,x2,y1,y2,z1,z2;
	double dot,length1,length2;
	double val;

	my_constraint_data1 *d = (my_constraint_data1 *)points;
	x1 = d->x1; x2 = d->x2;
	y1 = d->y1; y2 = d->y2;
	z1 = d->z1; z2 = d->z2;

	dot = (x1 - x[0]) * (x2 - x[0]) + (y1 - x[1]) * (y2 - x[1]) + (z1 - x[2]) * (z2 - x[2]);
	length1 = sqrt((x1 - x[0]) * (x1 - x[0]) + (y1 - x[1]) * (y1 - x[1]) + (z1 - x[2]) * (z1 - x[2]));
	length2 = sqrt((x2 - x[0]) * (x2 - x[0]) + (y2 - x[1]) * (y2 - x[1]) + (z2 - x[2]) * (z2 - x[2]));
    
	if(grad)
	{   
		double term1,term2,term3;
        
		term1 = -1.0 * ((x1 - x[0]) + (x2 - x[0])) / (sqrt(length1) * sqrt(length2));
		term2 = dot / sqrt(length2) * (x1 - x[0]) * pow(length1, -3.0 / 2.0);
		term3 = dot / sqrt(length1) * (x2 - x[0]) * pow(length2, -3.0 / 2.0);
		grad[0] = term1 + term2 + term3;
        
		term1 = -1.0 * ((y1 - x[1]) + (y2 - x[1])) / (sqrt(length1) * sqrt(length2));
		term2 = dot / sqrt(length2) * (y1 - x[1]) * pow(length1, -3.0 / 2.0);
		term3 = dot / sqrt(length1) * (y2 - x[1]) * pow(length2, -3.0 / 2.0);
		grad[1] = term1 + term2 + term3;

		term1 = -1.0 * ((z1 - x[2]) + (z2 - x[2])) / (sqrt(length1) * sqrt(length2));
		term2 = dot / sqrt(length2) * (z1 - x[2]) * pow(length1, -3.0 / 2.0);
		term3 = dot / sqrt(length1) * (z2 - x[2]) * pow(length2, -3.0 / 2.0);
		grad[2] = term1 + term2 + term3;
	}
    
	val = dot / (sqrt(length1) * sqrt(length2)) - cos(M_PI / 6.0);
    //std::cout << val << std::endl;

	return val;
}


double myconstraint1(unsigned n, const double *x, double *grad, void *points)
{
	/*
	double x1,x2,y1,y2,z1,z2;
	double val;

	my_constraint_data1 *d = (my_constraint_data1 *)points;
	x1 = d->x1; x2 = d->x2;
	y1 = d->y1; y2 = d->y2;
	z1 = d->z1; z2 = d->z2;

	val = (x[0] - x1) * (x2 - x1) + (x[1] - y1) * (y2 - y1) + (x[2] - z1) * (z2 - z1);
	if(grad)
	{
		grad[0] = -1.0 * (x2 - x1);
		grad[1] = -1.0 * (y2 - y1);
		grad[2] = -1.0 * (z2 - z1);
	}

	return -1.0 * val;
   */
	
	double x1,x2,y1,y2,z1,z2;
	double dot,length1,length2;
	double val;

	my_constraint_data1 *d = (my_constraint_data1 *)points;
	x1 = d->x1; x2 = d->x2;
	y1 = d->y1; y2 = d->y2;
	z1 = d->z1; z2 = d->z2;

	dot = (x[0] - x1) * (x2 - x1) + (x[1] - y1) * (y2 - y1) + (x[2] - z1) * (z2 - z1);
	length1 = (x[0] - x1) * (x[0] - x1) + (x[1] - y1) * (x[1] - y1) + (x[2] - z1) * (x[2] - z1);
	length2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1);

	if(grad)
	{
		double term1,term2;

		term1 = (x2 - x1) / (sqrt(length1) * sqrt(length2));
		term2 = dot / sqrt(length2) * (x1 - x[0]) * pow(length1, -3.0 / 2.0);

		grad[0] = -1.0 * (term1 + term2);

		term1 = (y2 - y1) / (sqrt(length1) * sqrt(length2));
		term2 = dot / sqrt(length2) * (y1 - x[1]) * pow(length1, -3.0 / 2.0);

		grad[1] = -1.0 * (term1 + term2);

		term1 = (z2 - z1) / (sqrt(length1) * sqrt(length2));
		term2 = dot / sqrt(length2) * (z1 - x[2]) * pow(length1, -3.0 / 2.0);

		grad[2] = -1.0 * (term1 + term2);
	}
    
	val = -1.0 * dot / (sqrt(length1) * sqrt(length2));
    //std::cout << val << std::endl;

	return val;
	
}

double myconstraint1_1(unsigned n, const double *x, double *grad, void *points)
{
	double x1,x2,y1,y2,z1,z2;
	double dot,length1,length2;
	double val;

	my_constraint_data1 *d = (my_constraint_data1 *)points;
	x1 = d->x1; x2 = d->x2;
	y1 = d->y1; y2 = d->y2;
	z1 = d->z1; z2 = d->z2;

	dot = (x[0] - x1) * (x2 - x1) + (x[1] - y1) * (y2 - y1) + (x[2] - z1) * (z2 - z1);
	length1 = (x[0] - x1) * (x[0] - x1) + (x[1] - y1) * (x[1] - y1) + (x[2] - z1) * (x[2] - z1);
	length2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1);

	if(grad)
	{
		double term1,term2;

		term1 = (x2 - x1) / (sqrt(length1) * sqrt(length2));
		term2 = dot / sqrt(length2) * (x1 - x[0]) * pow(length1, -3.0 / 2.0);
		
		grad[0] = term1 + term2;

		term1 = (y2 - y1) / (sqrt(length1) * sqrt(length2));
		term2 = dot / sqrt(length2) * (y1 - x[1]) * pow(length1, -3.0 / 2.0);
		
		grad[1] = term1 + term2;

		term1 = (z2 - z1) / (sqrt(length1) * sqrt(length2));
		term2 = dot / sqrt(length2) * (z1 - x[2]) * pow(length1, -3.0 / 2.0);
		
		grad[2] = term1 + term2;
	}
    
	val = dot / (sqrt(length1) * sqrt(length2)) - cos(M_PI / 6.0);
    //std::cout << val << std::endl;

	return val;
}


int main()
{
	double lb[3] = {-1e+19, -1e+19, -1e+19}; // lower bounds 
	nlopt_opt opt;
    
	//opt = nlopt_create(NLOPT_LD_MMA, 3); // algorithm and dimensionality 
	//opt = nlopt_create(NLOPT_LN_COBYLA,3); //(don't work)
	//opt = nlopt_create(NLOPT_AUGLAG,3); 
	//opt = nlopt_create(NLOPT_LD_LBFGS, 3);  //(don't work)
	//opt = nlopt_create(NLOPT_LN_BOBYQA, 3);
	opt = nlopt_create(NLOPT_LD_SLSQP, 3); //faster
	//opt = nlopt_create(NLOPT_LN_COBYLA, 3);
	//opt = nlopt_create(NLOPT_LD_LBFGS, 3);

	nlopt_set_lower_bounds(opt, lb); 
	
	
	my_constraint_data data = {0.2176,   -0.1282,   -0.2309};
	nlopt_set_min_objective(opt,myfunc,&data);
	
	my_constraint_data1  data11[6];
	my_constraint_data1  data12[6];
	for(int i = 0;i < 6;i++)
	{
		//double d[6];
		//for(int j = 0;j < 6;j++)
			//std::cin >> d[j];
		//my_constraint_data1  data11 = {1.0, 0.0, 0.0, 0.5, sqrt(3.0) / 2.0, 0.0};
		//my_constraint_data1  data12 = {0.5, sqrt(3.0) / 2.0, 0.0, 1.0, 0.0, 0.0};
       
		std::cin >> data11[i].x1;
		std::cin >> data11[i].y1;
		std::cin >> data11[i].z1;
		std::cin >> data11[i].x2;
		std::cin >> data11[i].y2;
		std::cin >> data11[i].z2;

		data12[i].x1 = data11[i].x2;
		data12[i].y1 = data11[i].y2;
        data12[i].z1 = data11[i].z2;
		data12[i].x2 = data11[i].x1;
		data12[i].y2 = data11[i].y1;
		data12[i].z2 = data11[i].z1;
  
		nlopt_add_inequality_constraint(opt, myconstraint, &data11[i], 1e-6);
		nlopt_add_inequality_constraint(opt, myconstraint_1, &data11[i], 1e-6);

		nlopt_add_inequality_constraint(opt, myconstraint1, &data11[i], 1e-6);
		nlopt_add_inequality_constraint(opt, myconstraint1_1, &data11[i], 1e-6);

		nlopt_add_inequality_constraint(opt, myconstraint1, &data12[i], 1e-6);
		nlopt_add_inequality_constraint(opt, myconstraint1_1, &data12[i], 1e-6);

	}
	nlopt_set_xtol_rel(opt, 1e-8);
	nlopt_set_ftol_rel(opt, 1e-8);

	double x[3] = {0.2064,   -0.1266,   -0.2222};  // some initial guess
	double minf; // the minimum objective value, upon return
    
	clock_t start, end;

	start = clock();
	if (nlopt_optimize(opt, x, &minf) < 0) {
		printf("nlopt failed!\n");
	}
	else {
		printf("found minimum at f(%g,%g,%g) = %0.10g\n", x[0], x[1], x[2], minf);
	}
	end = clock();

	printf("Times: %g", (double)(end - start) / CLOCKS_PER_SEC);

    
	//std::cout << myconstraint(3, x, NULL, &data11);

	nlopt_destroy(opt);

	return 0;
}

/*
//C++ version
#include <nlopt.hpp>
#include <iostream>

typedef struct {
	double a, b;
} my_constraint_data;


double myvfunc(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data)
{
	if (!grad.empty()) {
		grad[0] = 0.0;
		grad[1] = 0.5 / sqrt(x[1]);
	}
	return sqrt(x[1]);
}

double myvconstraint(const std::vector<double> &x, std::vector<double> &grad, void *data)
{
	my_constraint_data *d = reinterpret_cast<my_constraint_data*>(data);
	double a = d->a, b = d->b;
	if (!grad.empty()) {
		grad[0] = 3 * a * (a*x[0] + b) * (a*x[0] + b);
		grad[1] = -1.0;
	}
	return ((a*x[0] + b) * (a*x[0] + b) * (a*x[0] + b) - x[1]);
}

int main()
{
	nlopt::opt opt(nlopt::LD_MMA, 2);

	std::vector<double> lb(2);
	lb[0] = -HUGE_VAL; lb[1] = 0;
	opt.set_lower_bounds(lb);

	opt.set_min_objective(myvfunc, NULL);

	my_constraint_data data[2] = { {2,0}, {-1,1} };
	opt.add_inequality_constraint(myvconstraint, &data[0], 1e-8);
	opt.add_inequality_constraint(myvconstraint, &data[1], 1e-8);

	opt.set_xtol_rel(1e-4);

	std::vector<double> x(2);
	x[0] = 1.234; x[1] = 5.678;
	double minf;
	nlopt::result result = opt.optimize(x, minf);

	if(result)
		std::cout << x[0] << " " << x[1] << std::endl;
	else
		std::cout << "Optimization failed!" << std::endl;

	return 1;
}
*/