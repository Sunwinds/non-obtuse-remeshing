============================================================
============  Usage of \MCModule\Test
============================================================
����Ԥ��������
1����OpenGL�����ϰ�a����������ʾ��������ʾdone!�󣬻Ὣtest.obj���䵽����ϵ���ģ���С���ŵ�1����Ҫ���´򿪳���
2����OpenGL�����ϰ�n����������ʾ��������ʾdone!�󣬻Ὣtest.obj���������ζ���˳��ȡ��������ת������Ҫ���´򿪳���

����ʹ��˵����
SF��scalar field
MC��marching cube
1��ѡ��һ����Ҫ���Ե�SMF��ʽ�����񣬽�������Ϊtest.obj������mesh�ļ�����
2��Test.exe��mesh�ļ���Ҫ��ͬһĿ¼��
3������Test.exe������ʾtest.obj�������Ϣ��������ʾ����������ʾ
4����OpenGL�����ϰ�b����������ʾ��������ʾdone!�󣬻ᰴ��X��Y��Z��̵��Ǹ������з�40�ݵĲ�������SF
5����OpenGL�����ϰ�c����������ʾ��������ʾdone!�󣬻���mesh�ļ���������SF�ļ�test_sf.txt
6����OpenGL�����ϰ�o����������ʾ������ѡ��SFתMC�ķ�����
    0��MIDPOINT
    1��INTERPOLATE
    2��NONOBTUSE_INTERPOLATE
    Ȼ��س�����������ʾ��������ʾdone!�󣬳���Ὣ�����test_sf.txt����MC������OpenGL������ʾ
7����6��ǰ���£���OpenGL�����ϰ�m����������ʾ������ѡ����������MC�ķ�����
    0�������ɵ�MC��tiling��������һ���µ�MC
    1�������ɵ�MC��tiling�е�simple_mc������������һ���µ�MC
    Ȼ��س�����������ʾ��������ʾdone!�������ɵ�MC����OpenGL������ʾ
8����OpenGL�����ϰ�s����������ʾ��������ʾdone!�󣬻ᱣ��marching cube��mesh�ļ���������test_mc.obj
9����OpenGL�����ϰ�ESC�����˳�����

��ݼ���
u������
j������
h������
k������
y����Զ
i���ƽ�
d����ʾ������model�ͱ�������
    ʼ����ʾ�����ڲ�������
    ��ʾModelʱ�������ⲿ����������
    ����Modelʱ�������ⲿ��������ʾ
֧����갴ס�����ת

============================================================
//Hui Wang, June 30, 2012
//Deforme-to-fit testing
Ҫ��Test.exe��mesh�ļ���Ҫ��ͬһĿ¼�£�
���룺test_original.obj---ԭ��������
            test-nonobtuse.obj---�޶۽ǵ�����
���룺test_nonobtuse.obj---������������û��Laplace���Ż���Laplace���Ż�������ʱ����bug��

============================================================
Cai Yu 2012-07-01
1���ϲ�MCModule��nonObtuse����MCModule�����nonObtuse���̣�nonObtuse������ָ����ԭ����main.cpp���������ֵ���MarchingCube��MeshParser��ObtuseOpt
2���ϲ�MCModule��MCModule_test�����ݱȶ�
��MCModule_test�У�
  MarchingCube��ģ�renderer.h renderer.cpp
  nonObtuse��ģ�nonobtuseptn.cpp
  Test��ģ�testnmc.cpp testdeform.h testdeform.cpp main.cpp
������MCModule�еĶ�Ӧ�ļ�


============================================================
ʹ��ParaView�鿴������out.csv (SFViewer.m ������test_sf.txt�ϣ���������)
1. open "out.csv"
2. ��ѡ "Have Headers"��һ�Ȼ�󵥻���Apply����ť
3. �ο�http://paraview.org/Wiki/ParaView/Data_formats �� Displaying data as structured grid
4. Whole Extent ��д��0-nCellsX��0-nCellsY��0-nCellsZ
5. y columnѡField 1�� z columnѡField 2
6. Apply
7. Add a "Clip" filter to view the result.

============================================================
generate signed distance field from mesh:
http://techhouse.brown.edu/~dmorris/projects/voxelizer/ (��������������ԣ������������µĴ��룬���о���һ�����Լ���)
3D Distance Fields: A Survey of Techniques and Applications
Generating Signed Distance Fields From Triangle Meshes
http://www.cs.sfu.ca/~haoz/pubs/olson_zhang_cgf08.pdf


