function write_csv(x,y,z,v, filename)
fid = fopen(filename,'wt');
if( fid==-1 )
    error('Can''t open the file.');
    return;
end
rows = numel(x);
verts = [reshape(y,rows,1) reshape(x,rows,1) reshape(z,rows,1) reshape(v,rows,1)];
fprintf(fid, '%d,%d,%d,%d\n', verts');

fclose(fid);