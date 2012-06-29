#linux
#for ooqp
 |-blas.tgz
 |-fd05-1.0.0.tar.gz
 |-id05-1.0.0.tar.gz
 |-ma27-1.0.0.tar.gz
 |-za02-1.0.0.tar.gz
 |-f2c.tar.gz
 |-libf2c.tar.gz
 |-extras-ampl.tar.gz
#for nonObtuse
 |-freeglut-2.8.0.tar.gz
 |-glui_v2_2.tgz
 |-OOQP-0.99.22.tar

#windows
#for ooqp
 |-f2c.zip
 |-libf2c.zip
#for nonObtuse
 |-OOQP_VS2005.zip

#matlab
OptiToolbox_v1.55.zip

#build f2c.exe
upzip f2c.zip
cd f2c
nmake -f makefile.vc f2c.exe

#build vcf2c.lib
unzip libf2c.zip
cd libf2c
nmake -f makefile.vc
