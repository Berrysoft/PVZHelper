Imports System.Reflection
Imports PVZHelper.Cli

Class MainWindow
    Public Sub New()
        InitializeComponent()

        Mode.PlantsList = pList
        Mode.ZombiesList = zList
        Mode.CellList = bList
        Mode.RowNumList = nList
        Mode.RowList = rList
        Mode.MixList = mList
        Mode.IZEList = izeList
        Mode.CardNumList = cnList
        Mode.CardZombiesList = czList
        Mode.FlowerList = fList
        Mode.FlowerSize = fsList
        Mode.FlowerPlace = fpList

        Title = String.Format("PVZ修改器v{0}", Assembly.GetExecutingAssembly().GetName().Version)
    End Sub

    Private Sub ShowClub(sender As Object, e As RoutedEventArgs)
        Process.Start("https://tieba.baidu.com/f?kw=%D6%B2%CE%EF%B4%F3%D5%BD%BD%A9%CA%AC&fr=ala0&tpl=5")
    End Sub

#Region "常规"
    Private Sub Window_InitGame(sender As Object, e As RoutedEventArgs)
        Mode.InitState = Helper.InitGame()
    End Sub

    Private Sub SetSun(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.SetSun(Mode.Sun)
        End If
    End Sub

    Private Sub SetMoney(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.SetMoney(Mode.Money)
        End If
    End Sub

    Private Sub StartCar(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.StartCar()
        End If
    End Sub

    Private Sub ResetCar(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.ResetCar()
        End If
    End Sub

    Private Sub ShowSaveDir(sender As Object, e As RoutedEventArgs)
        Dim dir As String = Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData) & "\PopCap Games\PlantsVsZombies\userdata"
        Process.Start("explorer", dir)
    End Sub

    Private Sub Planting(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.Planting(Mode.Row - 1, Mode.Column - 1, Mode.PlantingSelectedIndex, Mode.Copy)
        End If
    End Sub

    Private Sub PutZombie(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.PutZombie(Mode.Row - 1, Mode.Column - 1, Mode.PutZombieSelectedIndex)
        End If
    End Sub

    Private Sub PutLadder(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.PutLadder(Mode.Row - 1, Mode.Column - 1)
        End If
    End Sub

    Private Sub PutGrave(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.PutGrave(Mode.Row - 1, Mode.Column - 1)
        End If
    End Sub

    Private Sub AutoLadder(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.AutoLadder()
        End If
    End Sub

    Private Sub SetBlock(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.SetBlock(Mode.Row - 1, Mode.Column - 1, Mode.CellTypeSelectedIndex + 1)
        End If
    End Sub

    Private Sub SetRow(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.SetRow(Mode.RowNumSelectedIndex, Mode.RowTypeSelectedIndex)
        End If
    End Sub
#End Region

#Region "穿越"
    Private Sub JmpLevel(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.JmpLevel(Mode.JmpLevelNum)
        End If
    End Sub

    Private Sub MixMode(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.MixMode(Mode.MixModeSelectedIndex, Mode.MixModeLevel)
        End If
    End Sub

    Private Sub Win(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.Win()
        End If
    End Sub

    Private Sub SetDoor(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.SetDoor(Mode.Black1Row - 1, Mode.Black1Column - 1, Mode.Black2Row - 1, Mode.Black2Column - 1, Mode.White1Row - 1, Mode.White1Column - 1, Mode.White2Row - 1, Mode.White2Column - 1)
        End If
    End Sub
#End Region

#Region "植物"
    Private Sub PStrong(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.PStrong()
        End If
    End Sub

    Private Sub PNormal(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.PNormal()
        End If
    End Sub

    Private Sub PWeak(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.PWeak()
        End If
    End Sub

    Private Sub NoLockCorn(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.LockButter(False)
        End If
    End Sub

    Private Sub LockButter(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.LockButter(True)
        End If
    End Sub

    Private Sub LockCorn(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.LockCorn(True)
        End If
    End Sub
#End Region

#Region "僵尸"
    Private Sub BoomIm(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.BoomIm()
        End If
    End Sub

    Private Sub BoomNormal(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.BoomNormal()
        End If
    End Sub

    Private Sub BoomNever(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.BoomNever()
        End If
    End Sub

    Private Sub VisiableNo(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.VisiableNo()
        End If
    End Sub

    Private Sub VisiableNormal(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.VisiableNormal()
        End If
    End Sub

    Private Sub VisiableYes(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.VisiableYes()
        End If
    End Sub

    Private Sub ZStrong(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.ZStrong()
        End If
    End Sub

    Private Sub ZNormal(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.ZNormal()
        End If
    End Sub

    Private Sub ZWeak(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.ZWeak()
        End If
    End Sub

    Private Sub KillAllZombies(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.KillAllZombies()
        End If
    End Sub

    Private Sub ConfuseZombies(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.ConfuseZombies()
        End If
    End Sub
#End Region

#Region "调节"
    Private Sub ShowZombiesEditionDiscription(sender As Object, e As RoutedEventArgs)
        MessageBox.Show("1.自然刷怪只修改了出怪种子，出怪列表则是调用了游戏自身的函数来决定
2.僵尸并非<完全>由种子控制，也受自身属性影响
3.在某些情况下自然刷怪，修改器会进行某种干预以保证游戏不崩溃
4.极限刷怪平均填充出怪列表，不修改出怪种子，默认不对任何僵尸做特殊处理", "刷怪说明", MessageBoxButton.OK, MessageBoxImage.Information)
    End Sub

    Private Sub SetCardNum(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.SetCardNum(Mode.CardNumSelectedIndex)
        End If
    End Sub

    Private Sub SetPCard(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.SetCard(Mode.SetCardNumSelectedIndex, Mode.SetPCardNumSelectedIndex, Mode.SetPCardCopy)
        End If
    End Sub

    Private Sub SetZCard(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.SetCard(Mode.SetCardNumSelectedIndex, Mode.SetZCardNumSelectedIndex + 60, False)
        End If
    End Sub

    Private Sub AddZList(sender As Object, e As RoutedEventArgs)
        Dim item As New Zombie(zList(Mode.ZListSelectedIndex), Mode.ZListSelectedIndex)
        If Not Mode.ZListBox.Contains(item) Then
            Mode.ZListBox.Add(item)
        End If
    End Sub

    Private Sub RemoveZList(sender As Object, e As RoutedEventArgs)
        If Mode.ZListBoxSelectedIndex >= 0 Then
            Mode.ZListBox.RemoveAt(Mode.ZListBoxSelectedIndex)
        End If
    End Sub

    Private Sub ClearZList(sender As Object, e As RoutedEventArgs)
        Mode.ZListBox.Clear()
    End Sub

    Private Sub SetSeed(sender As Object, e As RoutedEventArgs)
        Dim znum As Integer = Mode.ZListBox.Count
        If Mode.InitState = InitErr.Success AndAlso znum > 0 Then
            Dim zombies() As Integer = Mode.ZListBox.Select(Function(z) z.Index).ToArray()
            If Mode.NaturalSeed Then
                Helper.NaturalSeed(zombies)
            Else
                If znum = 2 AndAlso ((zombies(0) = 20 AndAlso zombies(1) = 32) OrElse (zombies(0) = 32 AndAlso zombies(1) = 20)) Then
                    Mode.RedLimit = False
                End If
                Helper.LimitSeed(zombies, Mode.ThiefLimit, Mode.RedLimit)
            End If
        End If
    End Sub

    Private Sub ResetSeed(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.ResetSeed()
        End If
    End Sub

    Private Sub SeeLeftZombies(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Dim zombies = Helper.SeeLeftZombies().Select(Function(left, index) zList(index))
            Dim str As String = String.Join(vbCrLf, zombies.Select(Function(s, index) $"{index + 1}.{s}"))
            If String.IsNullOrWhiteSpace(str) Then
                MessageBox.Show("无", "剩余出怪种类")
            Else
                MessageBox.Show(str, "剩余出怪种类")
            End If
        End If
    End Sub

    Private Sub AddLeftZombies(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Dim zombies = Helper.SeeLeftZombies().Select(Function(index) New Zombie(zList(index), index))
            For Each z In zombies
                If Not Mode.ZListBox.Contains(z) Then
                    Mode.ZListBox.Add(z)
                End If
            Next
        End If
    End Sub
#End Region

#Region "花园"
    Private Sub TreeHeight(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.TreeHeight(Mode.TreeHeight)
        End If
    End Sub

    Private Sub FlowerNum(sender As Object, e As RoutedEventArgs)
        If Mode.InitState = InitErr.Success Then
            Helper.FlowerNum(Mode.FlowerNum)
        End If
    End Sub

    'Private Sub ChangeFlower(sender As Object, e As RoutedEventArgs)
    '    If Mode.InitState = InitErr.Success Then
    '        Dim id As Integer = Mode.FlowerIndex
    '        Helper.ChangeFlower(Mode.FlowerIndex, Mode.FlowerListSelectedIndex)
    '        RefreshF()
    '        Mode.FlowerIndex = id
    '    End If
    'End Sub

    'Private Sub ChangeFSize(sender As Object, e As RoutedEventArgs)
    '    If Mode.InitState = InitErr.Success Then
    '        Dim id As Integer = Mode.FlowerIndex
    '        Helper.ChangeFSize(Mode.FlowerIndex, Mode.FlowerSizeSelectedIndex)
    '        RefreshF()
    '        Mode.FlowerIndex = id
    '    End If
    'End Sub

    Private Sub ChangeFDir(sender As Object, e As RoutedEventArgs)
        Dim button = CType(e.Source, Button)
        If button.Content = 1 Then
            button.Content = 0
        Else
            button.Content = 1
        End If
    End Sub

    'Private Sub FlowerLeft(sender As Object, e As RoutedEventArgs)
    '    If Mode.InitState = InitErr.Success Then
    '        Dim id As Integer = Mode.FlowerIndex
    '        Helper.ChangeFDir(Mode.FlowerIndex, 1)
    '        RefreshF()
    '        Mode.FlowerIndex = id
    '    End If
    'End Sub

    'Private Sub FlowerRight(sender As Object, e As RoutedEventArgs)
    '    If Mode.InitState = InitErr.Success Then
    '        Dim id As Integer = Mode.FlowerIndex
    '        Helper.ChangeFDir(Mode.FlowerIndex, 0)
    '        RefreshF()
    '        Mode.FlowerIndex = id
    '    End If
    'End Sub

    'Private Sub ChangeFPlace(sender As Object, e As RoutedEventArgs)
    '    If Mode.InitState = InitErr.Success Then
    '        Dim id As Integer = Mode.FlowerIndex
    '        Helper.ChangeFPlace(Mode.FlowerIndex, Mode.FlowerPlaceSelectedIndex)
    '        RefreshF()
    '        Mode.FlowerIndex = id
    '    End If
    'End Sub

    Private Sub RefreshFlowers(sender As Object, e As RoutedEventArgs)
        'If RefreshF() Then
        '    Mode.FlowerIndex = -1
        '    Mode.FlowerIndex = 0
        'End If
        RefreshF()
    End Sub

    Private Function RefreshF() As Boolean
        If Mode.InitState = InitErr.Success Then
            Dim num As Integer = Helper.GetFlowerNum()
            Dim flowers = Enumerable.Range(0, num).Select(Function(id) New Flower(id, Helper.GetFlower(id), Helper.GetFSize(id), Helper.GetFDir(id), Helper.GetFPlace(id)))
            Mode.Flowers = New ObjectModel.ObservableCollection(Of Flower)(flowers)
            Return True
        Else
            Return False
        End If
    End Function
#End Region

End Class
