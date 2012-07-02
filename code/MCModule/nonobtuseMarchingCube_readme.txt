non-obtuse mesh 的生成的方法：给定一个网格，先生成标量场，然后再通过marching cube生成，也就是两步：
1. 标量场的生成
2. marching cube

1. 标量场的生成有两种方法
(1) report 的方法：见nonobtuse类中的getScalarField函数
(2) thesis的方法（只是一个符号场）：见tilingnonobtusemc类中的markIntersectedCubes和triangulate_simpleMC

2. marching cube
主要分为插值点的计算和由tritable的确定连接关系
(1) 插值点：线性插值和midpoint
(2) tritable: 主要是网上原来的tritable和作者的tritable(report和thesis的tritable是完全一样的)


目前我们non-obtuse remeshing的入口是：tilingnonobtusemc中的GenerateSurface_simpleMC()

Note: non-obtuse remeshing主要有三个要素：标量场的生成，插值点的计算，tritable, 而每一个要素在文中又有多个选择


 

