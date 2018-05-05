Imports System.Runtime.InteropServices

Module Helper
    Enum InitErr
        Success
        ExeNotFound
        OpenFailed
        NotSupport
    End Enum

#Region "Lists"
    Public ReadOnly pList() As String = {"豌豆射手", "向日葵", "樱桃炸弹", "坚果", "土豆地雷",
"寒冰射手", "大嘴花", "双重射手", "小喷菇", "阳光菇", "大喷菇", "咬咬碑",
"迷糊菇", "胆小菇", "冰川菇", "末日菇", "莲叶", "窝瓜", "三重射手", "缠绕水草",
"火爆辣椒", "地刺", "火炬树桩", "高坚果", "水兵菇", "路灯花", "仙人掌",
"三叶草", "双向射手", "星星果", "南瓜头", "磁力菇", "卷心菜投手", "花盆",
"玉米投手", "咖啡豆", "大蒜", "萝卜伞", "金盏花", "西瓜投手", "机枪射手",
"双胞向日葵", "多嘴小蘑菇", "猫尾草", "冰西瓜", "吸金菇", "刚地刺", "玉米加农炮",
"反向双重射手", "红坚果", "巨坚果"}
    Public ReadOnly zList() As String = {"普通", "旗子", "路障", "撑杆", "铁桶", "读报",
"铁网门", "橄榄球", "舞者", "伴舞", "鸭子", "潜水", "雪橇车", "雪橇", "海豚",
"小丑", "气球", "矿工", "跳跳", "雪人", "飞贼", "梯子", "投石车", "巨人", "小鬼",
"僵博", "豌豆", "坚果", "辣椒", "机枪", "窝瓜", "高坚果", "红眼巨人"}
    Public ReadOnly bList() As String = {"草地", "裸地", "水池"}
    Public ReadOnly nList() As Integer = {1, 2, 3, 4, 5, 6}
    Public ReadOnly rList() As String = {"不出怪", "陆路", "水路"}
    Public ReadOnly mList() As String = {"冒险模式", "白天简单", "黑夜简单", "泳池简单",
"浓雾简单", "屋顶简单", "白天困难", "黑夜困难", "泳池困难", "浓雾困难",
"屋顶困难", "白天无限", "黑夜无限", "泳池无限", "浓雾无限", "屋顶无限",
"植物僵尸", "坚果保龄球", "拉霸", "种子雨", "僵尸迷阵", "隐形战争",
"观星", "僵尸水族馆", "僵尸迷阵 旋风", "小小大僵尸", "斗转星移", "排山倒海",
"全面冻结", "僵尸快跑", "打僵尸", "坚不可摧", "植物僵尸2", "坚果保龄球2",
"跳跳舞会", "僵尸博士的复仇", "当艺术遇上坚果", "晴天", "无草皮之地", "重要时间",
"当艺术遇上向日葵", "空袭", "急冻先锋", "牛顿的力量", "坟墓危险", "地道战",
"暴风雨之夜", "飞贼闪击战", "松鼠", "花瓶终结者无限", "僵尸公敌无限"}
    Public ReadOnly izeList() As String = {"豌豆阵", "大嘴地雷阵", "地刺杨桃阵", "大喷磁铁阵", "胆小阵",
"大杂烩阵1", "大杂烩阵2", "大杂烩阵3"}
    Public ReadOnly cnList() As Integer = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    Public ReadOnly czList() As String = {"普通", "路障", "撑杆", "铁桶", "扶梯", "矿工", "飞贼", "橄榄球", "气球", "铁网门", "雪橇车", "跳跳", "舞者", "巨人", "小鬼"}
    Public ReadOnly fList() As String = {"豌豆射手", "向日葵", "樱桃炸弹", "坚果", "土豆地雷",
"寒冰射手", "大嘴花", "双重射手", "小喷菇", "阳光菇", "大喷菇", "咬咬碑",
"迷糊菇", "胆小菇", "冰川菇", "末日菇", "莲叶", "窝瓜", "三重射手", "缠绕水草",
"火爆辣椒", "地刺", "火炬树桩", "高坚果", "水兵菇", "路灯花", "仙人掌",
"三叶草", "双向射手", "星星果", "南瓜头", "磁力菇", "卷心菜投手", "花盆",
"玉米投手", "咖啡豆", "大蒜", "萝卜伞", "金盏花", "西瓜投手", "机枪射手",
"双胞向日葵", "多嘴小喷菇", "猫尾草", "冰西瓜", "吸金菇", "钢地刺", "玉米加农炮", "变身茄子",
"红坚果", "巨坚果"}
    Public ReadOnly fsList() As String = {"幼苗", "小花", "中花", "大花"}
    Public ReadOnly fpList() As String = {"花园", "蘑菇园", "手推车", "水族馆"}
#End Region

#Region "常规"
    Public Declare Auto Function InitGame Lib "PVZHelper.dll" () As InitErr
    Public Declare Auto Sub SetSun Lib "PVZHelper.dll" (sun As Integer)
    Public Declare Auto Sub SetMoney Lib "PVZHelper.dll" (money As Integer)
    Public Declare Auto Sub AutoPickup Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub IgnoreSun Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub StartCar Lib "PVZHelper.dll" ()
    Public Declare Auto Sub ResetCar Lib "PVZHelper.dll" ()
    Public Declare Auto Sub NoDelete Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub NoSave Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub Anywhere Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub NoPause Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub FastBelt Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub ShowHide Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub FullFog Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub SeeFog Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub SeeVase Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub Planting Lib "PVZHelper.dll" (row As Integer, col As Integer, type As Integer, copy As Boolean)
    Public Declare Auto Sub PutZombie Lib "PVZHelper.dll" (row As Integer, col As Integer, type As Integer)
    Public Declare Auto Sub PutLadder Lib "PVZHelper.dll" (row As Integer, col As Integer)
    Public Declare Auto Sub PutGrave Lib "PVZHelper.dll" (row As Integer, col As Integer)
    Public Declare Auto Sub AutoLadder Lib "PVZHelper.dll" ()
    Public Declare Auto Sub SetBlock Lib "PVZHelper.dll" (row As Integer, col As Integer, type As Integer)
    Public Declare Auto Sub SetRow Lib "PVZHelper.dll" (row As Integer, type As Integer)
#End Region

#Region "穿越"
    Public Declare Auto Sub JmpLevel Lib "PVZHelper.dll" (level As Integer)
    Public Declare Auto Sub MixMode Lib "PVZHelper.dll" (mode As Integer, lv As Integer)
    Public Declare Auto Sub Unlock Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub Win Lib "PVZHelper.dll" ()
    Public Declare Auto Sub Lock Lib "PVZHelper.dll" (b As Boolean, mode As Integer)
    Public Declare Auto Sub Little Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub Rain Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub Storm Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub Force Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub LikeCol Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub RunFast Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub StartDoor Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub StableDoor Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub SetDoor Lib "PVZHelper.dll" (b1r As Integer, b1c As Integer, b2r As Integer, b2c As Integer, w1r As Integer, w1c As Integer, w2r As Integer, w2c As Integer)
#End Region

#Region "植物"
    Public Declare Auto Sub CornNoWait Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub MagnetNoWait Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub PotatoNoWait Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub MouthNoWait Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub PStrong Lib "PVZHelper.dll" ()
    Public Declare Auto Sub PNormal Lib "PVZHelper.dll" ()
    Public Declare Auto Sub PWeak Lib "PVZHelper.dll" ()
    Public Declare Auto Sub NoMark Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub NoSleep Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub StrongWind Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub StrongBean Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub Umbrella Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub NoCD Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub LockScoop Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub AnyMove Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub NoPurple Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub LockButter Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub LockCorn Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub FSBoom Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub FSSquash Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub FSMagnet Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub FSSputter Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub FSMouth Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub FSPrick Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub FSSpout Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub FSCat Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub CatTouch Lib "PVZHelper.dll" (b As Boolean)
#End Region

#Region "僵尸"
    Public Declare Auto Sub BoomIm Lib "PVZHelper.dll" ()
    Public Declare Auto Sub BoomNormal Lib "PVZHelper.dll" ()
    Public Declare Auto Sub BoomNever Lib "PVZHelper.dll" ()
    Public Declare Auto Sub VisiableNo Lib "PVZHelper.dll" ()
    Public Declare Auto Sub VisiableNormal Lib "PVZHelper.dll" ()
    Public Declare Auto Sub VisiableYes Lib "PVZHelper.dll" ()
    Public Declare Auto Sub ZStrong Lib "PVZHelper.dll" ()
    Public Declare Auto Sub ZNormal Lib "PVZHelper.dll" ()
    Public Declare Auto Sub ZWeak Lib "PVZHelper.dll" ()
    Public Declare Auto Sub NoSlow Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub NoButter Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub No3zGroup Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub NoIMP Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub NoICE Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub ZEatable Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub StopZTimer Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub StopZombies Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub StopBoss Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub BalloonBoom Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub ZTimerSpeed Lib "PVZHelper.dll" (value As Integer)
    Public Declare Auto Sub GatherZombie Lib "PVZHelper.dll" (b As Boolean, pos As Single)
    Public Declare Auto Sub KillAllZombies Lib "PVZHelper.dll" ()
    Public Declare Auto Sub ConfuseZombies Lib "PVZHelper.dll" ()
#End Region

#Region "调节"
    Public Declare Auto Sub SetCardNum Lib "PVZHelper.dll" (num As Integer)
    Public Declare Auto Sub SetCard Lib "PVZHelper.dll" (cid As Integer, type As Integer, copy As Boolean)
    Public Declare Auto Sub NaturalSeed Lib "PVZHelper.dll" (zombies() As Integer, znum As Integer)
    Public Declare Auto Sub LimitSeed Lib "PVZHelper.dll" (zombies() As Integer, znum As Integer, thief As Boolean, red As Boolean)
    Public Declare Auto Sub ResetSeed Lib "PVZHelper.dll" ()
    Public Declare Auto Sub SeeLeft Lib "PVZHelper.dll" ()
    Public Declare Auto Function SeeLeftZombie Lib "PVZHelper.dll" (index As Integer) As Boolean
#End Region

#Region "花园"
    Public Declare Auto Sub FlowerNPK Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub Medicine Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub Chocolate Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub TreeNPK Lib "PVZHelper.dll" (b As Boolean)
    Public Declare Auto Sub TreeHeight Lib "PVZHelper.dll" (h As Integer)
    Public Declare Auto Sub FlowerNum Lib "PVZHelper.dll" (n As Integer)
    Public Declare Auto Sub ChangeFlower Lib "PVZHelper.dll" (fid As Integer, type As Integer)
    Public Declare Auto Sub ChangeFSize Lib "PVZHelper.dll" (fid As Integer, size As Integer)
    Public Declare Auto Sub ChangeFDir Lib "PVZHelper.dll" (fid As Integer, dir As Integer)
    Public Declare Auto Sub ChangeFPlace Lib "PVZHelper.dll" (fid As Integer, place As Integer)
    Public Declare Auto Function GetFlowerNum Lib "PVZHelper.dll" () As Integer
    Public Declare Auto Function GetFlower Lib "PVZHelper.dll" (fid As Integer) As Integer
    Public Declare Auto Function GetFSize Lib "PVZHelper.dll" (fid As Integer) As Integer
    Public Declare Auto Function GetFDir Lib "PVZHelper.dll" (fid As Integer) As Integer
    Public Declare Auto Function GetFPlace Lib "PVZHelper.dll" (fid As Integer) As Integer
#End Region

End Module
