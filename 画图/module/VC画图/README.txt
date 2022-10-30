============================================================
v3.0
自画直线，矩形，园

============================================================
v2.8-2.9
完成所有功能，包括全选，拖动等，参见文档HDraw2.9.doc

============================================================
v2.7
修正画布随滚动条滚动的Bug
完善MouseMove的直线，矩形和椭圆识别


============================================================
v2.5
增强版
增加相应菜单
把画图的Toolbar单独分离出来
在画布上对单张图片操作

============================================================
v2.4
完结版
画布,ScrollView, CRectTracker,调色板,保存成位图
加入中间层View的步骤
1.创建MFC类CHDrawPView:CScrollView
2.修改CHDrawApp的InitInstance方法，将CHDrawView改为CHDrawPView，并修改include
加入CRectTracker的步骤
1.增加成员变量CRectTracker m_tracker
2.在CHDrawPView的构造函数中设置Tracker的大小
3.在CHDrawPView的OnDraw中调用Tracker的Draw函数
4.override CHDrawPView的SetCursor方法 GetCursorPos->ScreenToClient->SetCursor
5.override CHDrawPView的LButtonDown方法 HitTest->Track
将CHDrawView加入CHDrawPView的
使用自定义消息防止ActiveView从画布跑到背景View


============================================================
v2.3
增加Bitmap背景功能

============================================================
v2.2
完成颜色和线宽功能

============================================================
v2.1
增加文本图形类型，增加删除功能，增加打开保存功能

============================================================
v2.0
完成所有基本功能