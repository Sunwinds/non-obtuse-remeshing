使用说明：

Test.exe文件和mesh文件夹要在同一目录下，
运行Test.exe，会显示armadillo_5k.smf，相关信息在命令提示符中显示
在OpenGL窗口上按c，当命令提示符显示done!后，会在mesh目录里生成scalar field文件armadillo_sf_out.txt
重命名armadillo_sf_out.txt成armadillo_sf_in.txt，然后在OpenGL窗口上按o，当命令提示符显示done!后，会将读入的armadillo_sf_in.txt生成marching cube，并在OpenGL窗口显示，再按s会保存marching cube，在mesh下生成armadillo_5k_mc.smf
将smf直接重命名成obj，就可以在一般网格显示软件上显示了。