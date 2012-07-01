============================================================
==========  核心数据结构说明
============================================================
关于m_TriTable在CIsoSurface，cnonobtusemc，tilingnonobtusemc
目前从网上得到的原始版本CIsoSurface的m_TriTable可以将所有的测试Model的SF生成完好的MC，但是钝角比较严重
现有的CIsoSurface，cnonobtusemc，tilingnonobtusemc的m_TriTable都不同程度地存在问题，cnonobtusemc的m_TriTable是目前SF生成MC，钝角最小也最少的
目前所有的m_TriTable已经放到了CIsoSurface.cpp中，可以通过宏#define作为开关，切换编译使用，对应关系如下：
USEISO：网上原始版本CIsoSurface的m_TriTable
USEIMC：CIsoSurface的m_TriTable
USENMC：cnonobtusemc的m_TriTable
USETMC：tilingnonobtusemc的m_TriTable
注意每次编译只能打开一个，注释掉其它！
对照文献JohnYungSanLiMSc.pdf的4.1中12c，可以在tilingnonobtusemc的m_TriTable中搜索到“-12, 5, 7, 11, 2, 1, 8, 0, 3”
在#ifdef USETMC下面，类型为12b，因此基本确定论文作者的实验是基于这个m_TriTable的
但是程序未必是论文的最终版本，因为tilingnonobtusemc中的CheckStiching函数直接导致程序生成MC时崩溃！
尝试屏蔽CheckStiching函数，生成网格的钝角问题也很严重！

============================================================
==========  核心函数说明
============================================================