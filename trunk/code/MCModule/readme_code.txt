============================================================
==========  ���Ĺ���˵��
============================================================
1. MC
nonObtuse��5��MC��Mid-point, interpolated, Nonobtuse-interpolated, Tilling Nonobtuse & Simple MC
ĿǰTilling Nonobtuse������ʱ�̴���������������ͨ�����Ҷ��ܲ���������ģ�ͣ�����ȱʧ����ӦΪgrid�ķֱ��ʲ�������
Mid-point & Simple MC �������޶۽ǵ���������Simple MC�����ǶԵģ�
interpolated, Nonobtuse-interpolated & Tilling Nonobtuse�����ɺ��۽ǵ���������
ǰ4�֣�Mid-point, interpolated, Nonobtuse-interpolated, Tilling Nonobtuse��������-1,1���볡��
���һ��ֱ�ӽ�������Ƭ��grid��cell���󽻡�

����ɷ�ǰ4�ֺ�m_TriTable�Ķ�Ӧ˵��һ�£��������Ǹ��Ե�pipeline��һ�£������ͬ�Ļ�����עһ�¼��ɡ�


2. -1,1���볡����
���ǵõ��Ĵ���ľ��볡������Ȼ�Ǵ�ģ���Ҫ����Ϊ���ж�һ��grid�ڵ�����⣨-1,1����ʱ��û�п������ߺ������ཻ�ĸ����˻�����������ߺ�����Ƭ�󽻵ĸ���������ǵ�����ȫ�ģ�


3. deform to fit ��optimization��
���д������Ե�����


============================================================
==========  �������ݽṹ˵��
============================================================
m_TriTable��3��class�У�CIsoSurface��CNonobtuseMC��TilingNonobtuseMC
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





============================================================
==========  ���ĺ���˵��
============================================================