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
if you can not generate a distance field, the face orientation of the input mesh must be wrong:
1. Open the input mesh in MeshLab, if it is black, then use the following operation to invert face orientations:
MeshLab/Filters/Normals,Curvatures and Orientations/Invert Faces Orientations

2. Save the result to a *.off file.
3. Use off2smf.m convert and normalize it into a *smf
(after normailzing it, the GUI of testori will display result normal. see err_mc_sphere_4k.png)
4. Use test/testori!

============================================================
If the face orientation is right, the generated distance field may be still wrong. see err_scalarField_sphere_4k.png
��ʹ��cube_602.objҲ���ԭ����顣
���ȿ���onoobtuse.cpp��TODO: problems with cases where ray intersects vertex or edge

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

============================================================
����m_TriTable��CIsoSurface��cnonobtusemc��tilingnonobtusemc
Ŀǰ�����ϵõ���ԭʼ�汾CIsoSurface��m_TriTable���Խ����еĲ���Model��SF������õ�MC�����Ƕ۽ǱȽ�����
���е�CIsoSurface��cnonobtusemc��tilingnonobtusemc��m_TriTable����ͬ�̶ȵش������⣬cnonobtusemc��m_TriTable��ĿǰSF����MC���۽���СҲ���ٵ�
Ŀǰ���е�m_TriTable�Ѿ��ŵ���CIsoSurface.cpp�У�����ͨ����#define��Ϊ���أ��л�����ʹ�ã���Ӧ��ϵ���£�
USEISO������ԭʼ�汾CIsoSurface��m_TriTable
USEIMC��CIsoSurface��m_TriTable
USENMC��cnonobtusemc��m_TriTable
USETMC��tilingnonobtusemc��m_TriTable
ע��ÿ�α���ֻ�ܴ�һ����ע�͵�������
��������JohnYungSanLiMSc.pdf��4.1��12c��������tilingnonobtusemc��m_TriTable����������-12, 5, 7, 11, 2, 1, 8, 0, 3��
��#ifdef USETMC���棬����Ϊ12b����˻���ȷ���������ߵ�ʵ���ǻ������m_TriTable��
���ǳ���δ�������ĵ����հ汾����Ϊtilingnonobtusemc�е�CheckStiching����ֱ�ӵ��³�������MCʱ������
��������CheckStiching��������������Ķ۽�����Ҳ�����أ�