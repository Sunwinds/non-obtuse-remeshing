使用说明：
1、选择一个你要测试的SMF格式的网格，将其命名为test.smf，放在mesh文件夹里
2、Test.exe和mesh文件夹要在同一目录下
3、运行Test.exe，会显示test.smf，相关信息在命令提示符窗口中显示
4、在OpenGL窗口上按b，当命令提示符窗口显示done!后，会按照X、Y、Z最短的那个方向切分50份的步长构造marching cube
5、在OpenGL窗口上按c，当命令提示符窗口显示done!后，会在mesh文件夹里生成scalar field文件test_sf.txt
6、在OpenGL窗口上按o，当命令提示符窗口显示done!后，会将读入的test_sf.txt生成marching cube，并在OpenGL窗口显示
7、在OpenGL窗口上按s，当命令提示符窗口显示done!后，会保存marching cube到mesh文件夹下生成test_mc.smf
8、在OpenGL窗口上按ESC键，退出程序
9、将smf文件直接重命名成obj，就可以在一般网格显示软件上显示了

网格预处理方法：
1、在OpenGL窗口上按a，当命令提示符窗口显示done!后，会将test.smf对其到坐标系中心，大小缩放到1，需要重新打开程序！
2、在OpenGL窗口上按n，当命令提示符窗口显示done!后，会将test.smf所有三角形顶点顺序取反，即翻转法向，需要重新打开程序！

快捷键：
u：上移
j：下移
h：左移
k：右移
y：移远
i：移近
d：显示或隐藏model，方便看标量场
支持鼠标按住左键旋转

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
即使用cube_602.smf也会错，原因待查。
首先考虑onoobtuse.cpp的TODO: problems with cases where ray intersects vertex or edge

==========================================================
使用ParaView查看标量场out.csv (SFViewer.m 作用在test_sf.txt上，即可生成)
1. open "out.csv"
2. 不选 "Have Headers"那一项，然后单击“Apply”按钮
3. 参考http://paraview.org/Wiki/ParaView/Data_formats 的 Displaying data as structured grid
4. Whole Extent 填写：0-nCellsX，0-nCellsY，0-nCellsZ
5. y column选Field 1； z column选Field 2
6. Apply
7. Add a "Clip" filter to view the result.

========================================================
generate signed distance field from mesh:
http://techhouse.brown.edu/~dmorris/projects/voxelizer/ (先用这个程序试试，再找以下文章的代码，不行就挑一个，自己编)
3D Distance Fields: A Survey of Techniques and Applications
Generating Signed Distance Fields From Triangle Meshes
http://www.cs.sfu.ca/~haoz/pubs/olson_zhang_cgf08.pdf