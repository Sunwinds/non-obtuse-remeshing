Laplace smoothing and deform-to-fit for non-obtuse remeshing

Request:
1. Software: MATLAB 2012, 32bit (The dll cannot work on 64 bit system)
2. Please add the folder "..Matlab codes"with subfolder the the MATLAB path

Demo:
Please run the 'demo.m' 


Files:
/Core: the core codes 
     //confunctions: the constraints for non-obtuse angles
          /// confun.m: the non-linear non-obtuse constraints based on dot product (current used)
          ///confun_planeAndSphere.m: the non-linear non-obtuse constraints based on two planes and a sphere of the original paper
          ///confun30Degree.m: angles bounded in [30, 90] degree based on dot product
          ///constraintPlanes.m: the linear constraints based on three planes used in the original paper         
                    
     //Deform to fiting
         ///deformeForFitting_withoutOrder.m: deform-to-fit
         ///optimalPositionOfPoint_point.m: compute the position of deform-to-fit for each vertex
         ///objfunPoint.m: the minimized function for deform to fit
 
     //Laplace smoothing
         ///Laplacesmoothing_withoutOrder.m: Laplace smoothing
         ///optimalPositionOfPoint_Laplace.m: compute the position of Laplace smoothing for each vertex 
         ///objfunLaplace.m:the minimized function for Laplace smoothing 

/Toolbox: some tool functions 
....