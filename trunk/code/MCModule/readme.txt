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

======================================================
if you can not generate a distance field, the face orientation of the input mesh must be wrong:
1. Open the input mesh in MeshLab, if it is black, then use the following operation to invert face orientations:
MeshLab/Filters/Normals,Curvatures and Orientations/Invert Faces Orientations

2. Save the result to a *.off file.
3. Use off2smf.m convert and normalize it into a *smf
(after normailzing it, the GUI of testori will display result normal.)
4. Use test/testori!

=======================================================
If the face orientation is right, the generated distance field may be still wrong. see err_scalarField_sphere_4k.png