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
if cnt~=prod(dim)
    warning('Problem in reading scalers.');
end
v = reshape(v, dim(1), dim(2), dim(3));
    
fclose(fid);
return;