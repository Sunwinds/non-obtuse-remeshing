1、nonObtuse工程用MCModule中的同名文件替换了nonObtuse中的原始文件
2、nonObtuse中仅保留main.cpp，且main中添加了MCModule中的q、a、n、b快捷键，其它快捷键没有添加，因为可以通过GLUI界面的按钮实现
3、glut_glui中的文件需要部署到系统环境中，然后编译nonObtuse就可以使用Windows版本了
4、与MCModule的Test一样，nonObtuse.exe和mesh文件夹要在同一目录下才能正常运行

============================================================
============  Usage of \MCModule\nonObtuse
============================================================
在OpenGL窗口上按ESC或q，退出程序
在OpenGL窗口上按a或按GLUI上窗口上File里的Adjust Size按钮，当命令提示符窗口显示done!后，会将test.obj对其到坐标系中心，大小缩放到1，需要重新打开网格！如果看到的网格大小不合适，用之。
在OpenGL窗口上按n或按GLUI上窗口上File里的Reverse Normal按钮，当命令提示符窗口显示done!后，会将test.obj所有三角形顶点顺序取反，即翻转法向，需要重新打开网格！
在OpenGL窗口上按b或按GLUI上窗口上的Auto Cut按钮，当命令提示符窗口显示done!后，会按照X、Y、Z最短的那个方向切分40份的步长构造SF
在OpenGL窗口上按c或按GLUI上窗口上的Save按钮，当命令提示符窗口显示done!后，会在mesh文件夹里生成SF文件test_sf.txt
在OpenGL窗口上按s或按GLUI上窗口上File里的Save按钮，当命令提示符窗口显示done!后，会保存marching cube到mesh文件夹下生成test_mc.obj
在显示SF时，在OpenGL窗口上按d或勾选GLUI上窗口上Show Model：显示或隐藏model和标量场：
    始终显示网格内部标量场
    显示Model时，网格外部标量场隐藏
    隐藏Model时，网格外部标量场显示
    技巧：可以把显示模式改成WireFrame，并勾上Show Model，就能看到MC内部的SF了！