ʹ��˵����
1��ѡ��һ����Ҫ���Ե�SMF��ʽ�����񣬽�������Ϊtest.smf������mesh�ļ�����
2��Test.exe��mesh�ļ���Ҫ��ͬһĿ¼��
3������Test.exe������ʾtest.smf�������Ϣ��������ʾ����������ʾ
4����OpenGL�����ϰ�b����������ʾ��������ʾdone!�󣬻ᰴ��X��Y��Z��̵��Ǹ������з�50�ݵĲ�������marching cube
5����OpenGL�����ϰ�c����������ʾ��������ʾdone!�󣬻���mesh�ļ���������scalar field�ļ�test_sf.txt
6����OpenGL�����ϰ�o����������ʾ��������ʾdone!�󣬻Ὣ�����test_sf.txt����marching cube������OpenGL������ʾ
7����OpenGL�����ϰ�s����������ʾ��������ʾdone!�󣬻ᱣ��marching cube��mesh�ļ���������test_mc.smf
8����OpenGL�����ϰ�ESC�����˳�����
9����smf�ļ�ֱ����������obj���Ϳ�����һ��������ʾ�������ʾ��

����Ԥ��������
1����OpenGL�����ϰ�a����������ʾ��������ʾdone!�󣬻Ὣtest.smf���䵽����ϵ���ģ���С���ŵ�1����Ҫ���´򿪳���
2����OpenGL�����ϰ�n����������ʾ��������ʾdone!�󣬻Ὣtest.smf���������ζ���˳��ȡ��������ת������Ҫ���´򿪳���

��ݼ���
u������
j������
h������
k������
y����Զ
i���ƽ�
d����ʾ������model�����㿴������
֧����갴ס�����ת

======================================================
if you can not generate a distance field, the face orientation of the input mesh must be wrong:
1. Open the input mesh in MeshLab, if it is black, then use the following operation to invert face orientations:
MeshLab/Filters/Normals,Curvatures and Orientations/Invert Faces Orientations

2. Save the result to a *.off file.
3. Use off2smf.m convert and normalize it into a *smf
(after normailzing it, the GUI of testori will display result normal. see err_mc_sphere_4k.png)
4. Use test/testori!

=======================================================
If the face orientation is right, the generated distance field may be still wrong. see err_scalarField_sphere_4k.png
��ʹ��cube_602.smfҲ���ԭ����顣
���ȿ���onoobtuse.cpp��TODO: problems with cases where ray intersects vertex or edge

==========================================================
ʹ��ParaView�鿴������out.csv (SFViewer.m ������test_sf.txt�ϣ���������)
1. open "out.csv"
2. ��ѡ "Have Headers"��һ�Ȼ�󵥻���Apply����ť
3. �ο�http://paraview.org/Wiki/ParaView/Data_formats �� Displaying data as structured grid
4. Whole Extent ��д��0-nCellsX��0-nCellsY��0-nCellsZ
5. y columnѡField 1�� z columnѡField 2
6. Apply
7. Add a "Clip" filter to view the result.

========================================================
generate signed distance field from mesh:
http://techhouse.brown.edu/~dmorris/projects/voxelizer/ (��������������ԣ������������µĴ��룬���о���һ�����Լ���)
3D Distance Fields: A Survey of Techniques and Applications
Generating Signed Distance Fields From Triangle Meshes
http://www.cs.sfu.ca/~haoz/pubs/olson_zhang_cgf08.pdf