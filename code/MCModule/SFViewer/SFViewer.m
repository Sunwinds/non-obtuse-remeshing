close all;

% [dim,v] = read_sf('..\Test\mesh\armadillo_sf_out.txt');
% [dim,v] = read_sf('..\Test\mesh\horse_sf_out.txt');
[dim,v] = read_sf('..\Test\mesh\test_sf.txt');

% Number of 1 and -1 of:
% armadillo_sf_out.txt: 820831, 71060.
% horse_sf_out.txt: 293096, 205. The ratio of number of 1 & -1 is obviously wrong!
% hand_sf_out.txt: 152388, 51013
[x,y,z] = meshgrid(1:dim(2),1:dim(1),1:dim(3));

%% the csv file can be viewed as points with scalars or structured grid in Paraview
write_csv(x,y,z,v, 'out.csv');
%http://paraview.org/Wiki/ParaView/Data_formats

%%
figure;
p = patch(isosurface(x,y,z,v,0),'FaceColor','red',...
	'EdgeColor','none');
p2 = patch(isocaps(v, 0),'FaceColor','interp',...
	'EdgeColor','none');
isonormals(x,y,z,v,p);

daspect([1,1,1]);view(3); axis tight;
camlight;lighting gouraud;
view3d rot;
% colormap(gray(100))

%%
figure;
xslice = 1:4:6; yslice = 1:5:8; zslice = 1:4:6;
slice(x,y,z,v,xslice,yslice,zslice);
colormap jet;
daspect([1,1,1]);view(3); axis tight;
camlight;lighting gouraud;
view3d rot;