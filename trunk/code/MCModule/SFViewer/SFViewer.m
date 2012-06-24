close all;

[dim,v] = read_sf('..\Test\mesh\horse_sf_out.txt');
% [x,y,z] = ndgrid(1:dim(1),1:dim(2),1:dim(3));
[x,y,z] = meshgrid(1:dim(2),1:dim(1),1:dim(3));

%% the csv file can be viewed as points with scalars or structured grid in Paraview
write_csv(x,y,z,v, 'out.csv');
%http://paraview.org/Wiki/ParaView/Data_formats

% %%
% figure;
% p = patch(isosurface(x,y,z,v,0),'FaceColor','red',...
% 	'EdgeColor','none');
% p2 = patch(isocaps(v, 0),'FaceColor','interp',...
% 	'EdgeColor','none');
% isonormals(x,y,z,v,p);
% 
% daspect([1,1,1]);view(3); axis tight;
% camlight;lighting gouraud;
% view3d rot;
% % colormap(gray(100))

%%
figure;
xslice = 10:20:51; yslice = 10:80:81; zslice = 10:70:71;
slice(x,y,z,v,xslice,yslice,zslice);
colormap hsv;
daspect([1,1,1]);view(3); axis tight;
camlight;lighting gouraud;
view3d rot;