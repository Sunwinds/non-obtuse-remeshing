%Hui Wang, August 31, 2012

%Input data
load('gargoyle.mat');

%Laplace smoothing
V_dec_smooth = Laplacesmoothing_withoutOrder(V_dec, F_dec);

%Deform to fit
V_dec_result = deformeForFitting_withoutOrder(V_dec_smooth, F_dec, V_fit, F_fit, 0.0);

SaveOff('dec_result.OFF', V_dec_result, F_dec);