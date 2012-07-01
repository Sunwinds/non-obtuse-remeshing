1、nonObtuse工程用MCModule中的同名文件替换了nonObtuse中的原始文件
2、nonObtuse中仅保留main.cpp，且main中添加了MCModule中的q、a、n、b快捷键，其它快捷键没有添加，因为可以通过GLUI界面的按钮实现
3、glut_glui中的文件需要部署到系统环境中，然后编译nonObtuse就可以使用Windows版本了
4、与MCModule的Test一样，nonObtuse.exe和mesh文件夹要在同一目录下才能正常运行

============================================================
============  Usage of \MCModule\nonObtuse
============================================================
在OpenGL窗口上按q，退出程序
在OpenGL窗口上按a，当命令提示符窗口显示done!后，会将test.obj对其到坐标系中心，大小缩放到1，需要重新打开网格！如果看到的网格大小不合适，用之。
在OpenGL窗口上按n，当命令提示符窗口显示done!后，会将test.obj所有三角形顶点顺序取反，即翻转法向，需要重新打开网格！
在OpenGL窗口上按b，当命令提示符窗口显示done!后，会按照X、Y、Z最短的那个方向切分40份的步长构造SF
