non-obtuse mesh �����ɵķ���������һ�����������ɱ�������Ȼ����ͨ��marching cube���ɣ�Ҳ����������
1. ������������
2. marching cube

1. �����������������ַ���
(1) report �ķ�������nonobtuse���е�getScalarField����
(2) thesis�ķ�����ֻ��һ�����ų�������tilingnonobtusemc���е�markIntersectedCubes��triangulate_simpleMC

2. marching cube
��Ҫ��Ϊ��ֵ��ļ������tritable��ȷ�����ӹ�ϵ
(1) ��ֵ�㣺���Բ�ֵ��midpoint
(2) tritable: ��Ҫ������ԭ����tritable�����ߵ�tritable(report��thesis��tritable����ȫһ����)


Ŀǰ����non-obtuse remeshing������ǣ�tilingnonobtusemc�е�GenerateSurface_simpleMC()

Note: non-obtuse remeshing��Ҫ������Ҫ�أ������������ɣ���ֵ��ļ��㣬tritable, ��ÿһ��Ҫ�����������ж��ѡ��


 

