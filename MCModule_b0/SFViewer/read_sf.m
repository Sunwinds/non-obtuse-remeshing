function [dim,v] = read_sf(filename)
dim = [0,0,0];
fid = fopen(filename,'r');
if( fid==-1 )
    error('Can''t open the file.');
    return;
end

str = fgetl(fid);
% str = fgetl(fid);
% tmp = sscanf(str,'%*s %d');
for i = 1:3
    dim(i) = fscanf(fid,'%*s %d',1)+1;
end

for i = 1:8
    str = fgetl(fid); 
end

[v,cnt] = fscanf(fid,'%d', prod(dim));
sum(v==1)
sum(v==-1)
if cnt~=prod(dim)
    warning('Problem in reading scalers.');
end
% v = [1:12]';
% v = reshape(v,3,2,2);
% v = reshape(v, 12, 1);
v = reshape(v, dim(1), dim(2), dim(3));
    
fclose(fid);
return;