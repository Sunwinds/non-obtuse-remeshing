// Copyright (C) 2004, 2009 International Business Machines and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id: cpp_example.cpp 2005 2011-06-06 12:55:16Z stefan $
//
// Authors:  Carl Laird, Andreas Waechter     IBM    2004-11-05

#include "IpIpoptApplication.hpp"
#include "IpSolveStatistics.hpp"
#include "Solver.h"

#include <iostream>
using namespace Ipopt;

#include <cmath>
#include <vector>
typedef std::vector<double> Point;
typedef std::vector<std::vector<double>> Points; 

int main(int argv, char* argc[])
{
  Point p0;
  p0.push_back(0.0);
  p0.push_back(0.0);
  p0.push_back(0.0);

  Point q;
  q.push_back(2.0);
  q.push_back(2.0);
  q.push_back(2.0);

  Point p1,p2;
  p1.push_back(1.0);
  p1.push_back(0.0);
  p1.push_back(0.0);

  p2.push_back(0.5);
  p2.push_back(std::sqrt(3.0) / 2);
  p2.push_back(0.0);

  Points P1,P2;
  P1.push_back(p1);
  P2.push_back(p2);

  // Create an instance of your nlp...
  //MyNLP1 *mynlp = new MyNLP1();
  SmartPtr<TNLP> mynlp = new MyNLP1(p0, q, P1, P2);

  /*
  mynlp->setIdealPosition(q);
  mynlp->setStartingPoint(p0);
  mynlp->setConstraintEdge(P1, P2);
  */
  // Create an instance of the IpoptApplication
  //
  // We are using the factory, since this allows us to compile this
  // example with an Ipopt Windows DLL
  SmartPtr<IpoptApplication> app = IpoptApplicationFactory();
  app->Options()->SetStringValue("hessian_approximation","limited-memory");
  app->Options()->SetStringValue("print_user_options", "no");
  app->Options()->SetIntegerValue("print_level", 0);

  // Initialize the IpoptApplication and process the options
  ApplicationReturnStatus status;
  status = app->Initialize();
  if (status != Solve_Succeeded) {
    std::cout << std::endl << std::endl << "*** Error during initialization!" << std::endl;
    return (int) status;
  }

  status = app->OptimizeTNLP(mynlp);

  if (status == Solve_Succeeded) {
    // Retrieve some statistics about the solve
    Index iter_count = app->Statistics()->IterationCount();
    std::cout << std::endl << std::endl << "*** The problem solved in " << iter_count << " iterations!" << std::endl;

    Number final_obj = app->Statistics()->FinalObjective();
    std::cout << std::endl << std::endl << "*** The final value of the objective function is " << final_obj << '.' << std::endl;

  }

  return (int) status;
}
