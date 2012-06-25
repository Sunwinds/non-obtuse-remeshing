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