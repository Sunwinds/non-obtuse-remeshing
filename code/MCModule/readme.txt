============================================================
============  Usage of \MCModule\Test
============================================================
网格预处理方法：
1、在OpenGL窗口上按a，当命令提示符窗口显示done!后，会将test.obj对其到坐标系中心，大小缩放到1，需要重新打开程序！
2、在OpenGL窗口上按n，当命令提示符窗口显示done!后，会将test.obj所有三角形顶点顺序取反，即翻转法向，需要重新打开程序！

程序使用说明：
SF：scalar field
MC：marching cube
1、选择一个你要测试的SMF格式的网格，将其命名为test.obj，放在mesh文件夹里
2、Test.exe和mesh文件夹要在同一目录下
3、运行Test.exe，会显示test.obj，相关信息在命令提示符窗口中显示
4、在OpenGL窗口上按b，当命令提示符窗口显示done!后，会按照X、Y、Z最短的那个方向切分40份的步长构造SF
5、在OpenGL窗口上按c，当命令提示符窗口显示done!后，会在mesh文件夹里生成SF文件test_sf.txt
6、在OpenGL窗口上按o，在命令提示符窗口选择SF转MC的方法：
    0：MIDPOINT
    1：INTERPOLATE
    2：NONOBTUSE_INTERPOLATE
    然后回车，当命令提示符窗口显示done!后，程序会将读入的test_sf.txt生成MC，并在OpenGL窗口显示
7、在6的前提下，在OpenGL窗口上按m，在命令提示符窗口选择重新生成MC的方法：
    0：对生成的MC用tiling重新生成一个新的MC
    1：对生成的MC用tiling中的simple_mc方法重新生成一个新的MC
    然后回车，当命令提示符窗口显示done!后，新生成的MC会在OpenGL窗口显示
8、在OpenGL窗口上按s，当命令提示符窗口显示done!后，会保存marching cube到mesh文件夹下生成test_mc.obj
9、在OpenGL窗口上按ESC键，退出程序

快捷键：
u：上移
j：下移
h：左移
k：右移
y：移远
i：移近
d：显示或隐藏model和标量场：
    始终显示网格内部标量场
    显示Model时，网格外部标量场隐藏
    隐藏Model时，网格外部标量场显示
支持鼠标按住左键旋转

============================================================
//Hui Wang, June 30, 2012
//Deforme-to-fit testing
要求：Test.exe和mesh文件夹要在同一目录下，
输入：test_original.obj---原来的网格
            test-nonobtuse.obj---无钝角的网格
输入：test_nonobtuse.obj---最后的输入网格（没有Laplace的优化，Laplace的优化部分有时候有bug）

============================================================
Cai Yu 2012-07-01
1、合并MCModule和nonObtuse，在MCModule中添加nonObtuse工程，nonObtuse工程中指保留原来的main.cpp，其它部分调用MarchingCube、MeshParser和ObtuseOpt
2、合并MCModule和MCModule_test，根据比对
用MCModule_test中：
  MarchingCube里的：renderer.h renderer.cpp
  nonObtuse里的：nonobtuseptn.cpp
  Test里的：testnmc.cpp testdeform.h testdeform.cpp main.cpp
更新了MCModule中的对应文件


============================================================
使用ParaView查看标量场out.csv (SFViewer.m 作用在test_sf.txt上，即可生成)
1. open "out.csv"
2. 不选 "Have Headers"那一项，然后单击“Apply”按钮
3. 参考http://paraview.org/Wiki/ParaView/Data_formats 的 Displaying data as structured grid
4. Whole Extent 填写：0-nCellsX，0-nCellsY，0-nCellsZ
5. y column选Field 1； z column选Field 2
6. Apply
7. Add a "Clip" filter to view the result.

============================================================
generate signed distance field from mesh:
http://techhouse.brown.edu/~dmorris/projects/voxelizer/ (先用这个程序试试，再找以下文章的代码，不行就挑一个，自己编)
3D Distance Fields: A Survey of Techniques and Applications
Generating Signed Distance Fields From Triangle Meshes
http://www.cs.sfu.ca/~haoz/pubs/olson_zhang_cgf08.pdf


