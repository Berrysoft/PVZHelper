Imports System.Collections.ObjectModel
Imports PVZHelper.Cli

Class MainViewMode
    Inherits DependencyObject

#Region "列表框"
    Public Shared ReadOnly PlantsListProperty As DependencyProperty = DependencyProperty.Register(NameOf(PlantsList), GetType(String()), GetType(MainViewMode))
    Public Property PlantsList As String()
        Get
            Return GetValue(PlantsListProperty)
        End Get
        Set(value As String())
            SetValue(PlantsListProperty, value)
        End Set
    End Property

    Public Shared ReadOnly ZombiesListProperty As DependencyProperty = DependencyProperty.Register(NameOf(ZombiesList), GetType(String()), GetType(MainViewMode))
    Public Property ZombiesList As String()
        Get
            Return GetValue(ZombiesListProperty)
        End Get
        Set(value As String())
            SetValue(ZombiesListProperty, value)
        End Set
    End Property

    Public Shared ReadOnly CellListProperty As DependencyProperty = DependencyProperty.Register(NameOf(CellList), GetType(String()), GetType(MainViewMode))
    Public Property CellList As String()
        Get
            Return GetValue(CellListProperty)
        End Get
        Set(value As String())
            SetValue(CellListProperty, value)
        End Set
    End Property

    Public Shared ReadOnly RowNumListProperty As DependencyProperty = DependencyProperty.Register(NameOf(RowNumList), GetType(Integer()), GetType(MainViewMode))
    Public Property RowNumList As Integer()
        Get
            Return GetValue(RowNumListProperty)
        End Get
        Set(value As Integer())
            SetValue(RowNumListProperty, value)
        End Set
    End Property

    Public Shared ReadOnly RowListProperty As DependencyProperty = DependencyProperty.Register(NameOf(RowList), GetType(String()), GetType(MainViewMode))
    Public Property RowList As String()
        Get
            Return GetValue(RowListProperty)
        End Get
        Set(value As String())
            SetValue(RowListProperty, value)
        End Set
    End Property

    Public Shared ReadOnly MixListProperty As DependencyProperty = DependencyProperty.Register(NameOf(MixList), GetType(String()), GetType(MainViewMode))
    Public Property MixList As String()
        Get
            Return GetValue(MixListProperty)
        End Get
        Set(value As String())
            SetValue(MixListProperty, value)
        End Set
    End Property

    Public Shared ReadOnly IZEListProperty As DependencyProperty = DependencyProperty.Register(NameOf(IZEList), GetType(String()), GetType(MainViewMode))
    Public Property IZEList As String()
        Get
            Return GetValue(IZEListProperty)
        End Get
        Set(value As String())
            SetValue(IZEListProperty, value)
        End Set
    End Property

    Public Shared ReadOnly CardNumListProperty As DependencyProperty = DependencyProperty.Register(NameOf(CardNumList), GetType(Integer()), GetType(MainViewMode))
    Public Property CardNumList As Integer()
        Get
            Return GetValue(CardNumListProperty)
        End Get

        Set(value As Integer())
            SetValue(CardNumListProperty, value)
        End Set
    End Property

    Public Shared ReadOnly CardZombiesListProperty As DependencyProperty = DependencyProperty.Register(NameOf(CardZombiesList), GetType(String()), GetType(MainViewMode))
    Public Property CardZombiesList As String()
        Get
            Return GetValue(CardZombiesListProperty)
        End Get

        Set(value As String())
            SetValue(CardZombiesListProperty, value)
        End Set
    End Property

    Public Shared ReadOnly FlowerListProperty As DependencyProperty = DependencyProperty.Register(NameOf(FlowerList), GetType(String()), GetType(MainViewMode))
    Public Property FlowerList As String()
        Get
            Return GetValue(FlowerListProperty)
        End Get
        Set(value As String())
            SetValue(FlowerListProperty, value)
        End Set
    End Property

    Public Shared ReadOnly FlowerSizeProperty As DependencyProperty = DependencyProperty.Register(NameOf(FlowerSize), GetType(String()), GetType(MainViewMode))
    Public Property FlowerSize As String()
        Get
            Return GetValue(FlowerSizeProperty)
        End Get
        Set(value As String())
            SetValue(FlowerSizeProperty, value)
        End Set
    End Property

    Public Shared ReadOnly FlowerPlaceProperty As DependencyProperty = DependencyProperty.Register(NameOf(FlowerPlace), GetType(String()), GetType(MainViewMode))
    Public Property FlowerPlace As String()
        Get
            Return GetValue(FlowerPlaceProperty)
        End Get
        Set(value As String())
            SetValue(FlowerPlaceProperty, value)
        End Set
    End Property
#End Region

#Region "常规"
    Public Shared ReadOnly InitStateProperty As DependencyProperty = DependencyProperty.Register(NameOf(InitState), GetType(InitErr), GetType(MainViewMode))
    Public Property InitState As InitErr
        Get
            Return GetValue(InitStateProperty)
        End Get
        Set(value As InitErr)
            SetValue(InitStateProperty, value)
        End Set
    End Property

    Public Shared ReadOnly SunProperty As DependencyProperty = DependencyProperty.Register(NameOf(Sun), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(9990))
    Public Property Sun As Integer
        Get
            Return GetValue(SunProperty)
        End Get
        Set(value As Integer)
            SetValue(SunProperty, value)
        End Set
    End Property

    Public Shared ReadOnly MoneyProperty As DependencyProperty = DependencyProperty.Register(NameOf(Money), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(99999))
    Public Property Money As Integer
        Get
            Return GetValue(MoneyProperty)
        End Get
        Set(value As Integer)
            SetValue(MoneyProperty, value)
        End Set
    End Property

    Public Shared ReadOnly AutoPickupProperty As DependencyProperty = DependencyProperty.Register(NameOf(AutoPickup), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf AutoPickupChangedCallback))
    Public Property AutoPickup As Boolean
        Get
            Return GetValue(AutoPickupProperty)
        End Get
        Set(value As Boolean)
            SetValue(AutoPickupProperty, value)
        End Set
    End Property
    Private Shared Sub AutoPickupChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.AutoPickup(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly IgnoreSunProperty As DependencyProperty = DependencyProperty.Register(NameOf(IgnoreSun), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf IgnoreSunChangedCallback))
    Public Property IgnoreSun As Boolean
        Get
            Return GetValue(IgnoreSunProperty)
        End Get
        Set(value As Boolean)
            SetValue(IgnoreSunProperty, value)
        End Set
    End Property
    Private Shared Sub IgnoreSunChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.IgnoreSun(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly NoDeleteProperty As DependencyProperty = DependencyProperty.Register(NameOf(NoDelete), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf NoDeleteChangedCallback))
    Public Property NoDelete As Boolean
        Get
            Return GetValue(NoDeleteProperty)
        End Get
        Set(value As Boolean)
            SetValue(NoDeleteProperty, value)
        End Set
    End Property
    Private Shared Sub NoDeleteChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.NoDelete(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly NoSaveProperty As DependencyProperty = DependencyProperty.Register(NameOf(NoSave), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf NoSaveChangedCallback))
    Public Property NoSave As Boolean
        Get
            Return GetValue(NoSaveProperty)
        End Get
        Set(value As Boolean)
            SetValue(NoSaveProperty, value)
        End Set
    End Property
    Private Shared Sub NoSaveChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.NoSave(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly AnywhereProperty As DependencyProperty = DependencyProperty.Register(NameOf(Anywhere), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf AnywhereChangedCallback))
    Public Property Anywhere As Boolean
        Get
            Return GetValue(AnywhereProperty)
        End Get
        Set(value As Boolean)
            SetValue(AnywhereProperty, value)
        End Set
    End Property
    Private Shared Sub AnywhereChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.Anywhere(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly NoPauseProperty As DependencyProperty = DependencyProperty.Register(NameOf(NoPause), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf NoPauseChangedCallback))
    Public Property NoPause As Boolean
        Get
            Return GetValue(NoPauseProperty)
        End Get
        Set(value As Boolean)
            SetValue(NoPauseProperty, value)
        End Set
    End Property
    Private Shared Sub NoPauseChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.NoPause(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly FastBeltProperty As DependencyProperty = DependencyProperty.Register(NameOf(FastBelt), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf FastBeltChangedCallback))
    Public Property FastBelt As Boolean
        Get
            Return GetValue(FastBeltProperty)
        End Get
        Set(value As Boolean)
            SetValue(FastBeltProperty, value)
        End Set
    End Property
    Private Shared Sub FastBeltChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.FastBelt(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly ShowHideProperty As DependencyProperty = DependencyProperty.Register(NameOf(ShowHide), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf ShowHideChangedCallback))
    Public Property ShowHide As Boolean
        Get
            Return GetValue(ShowHideProperty)
        End Get
        Set(value As Boolean)
            SetValue(ShowHideProperty, value)
        End Set
    End Property
    Private Shared Sub ShowHideChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.ShowHide(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly FullFogProperty As DependencyProperty = DependencyProperty.Register(NameOf(FullFog), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf FullFogChangedCallback))
    Public Property FullFog As Boolean
        Get
            Return GetValue(FullFogProperty)
        End Get
        Set(value As Boolean)
            SetValue(FullFogProperty, value)
        End Set
    End Property
    Private Shared Sub FullFogChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.FullFog(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly SeeFogProperty As DependencyProperty = DependencyProperty.Register(NameOf(SeeFog), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf SeeFogChangedCallback))
    Public Property SeeFog As Boolean
        Get
            Return GetValue(SeeFogProperty)
        End Get
        Set(value As Boolean)
            SetValue(SeeFogProperty, value)
        End Set
    End Property
    Private Shared Sub SeeFogChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.SeeFog(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly SeeVaseProperty As DependencyProperty = DependencyProperty.Register(NameOf(SeeVase), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf SeeVaseChangedCallback))
    Public Property SeeVase As Boolean
        Get
            Return GetValue(SeeVaseProperty)
        End Get
        Set(value As Boolean)
            SetValue(SeeVaseProperty, value)
        End Set
    End Property
    Private Shared Sub SeeVaseChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.SeeVase(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly RowProperty As DependencyProperty = DependencyProperty.Register(NameOf(Row), GetType(Integer), GetType(MainViewMode))
    Public Property Row As Integer
        Get
            Return GetValue(RowProperty)
        End Get
        Set(value As Integer)
            SetValue(RowProperty, value)
        End Set
    End Property


    Public Shared ReadOnly ColumnProperty As DependencyProperty = DependencyProperty.Register(NameOf(Column), GetType(Integer), GetType(MainViewMode))
    Public Property Column As Integer
        Get
            Return GetValue(ColumnProperty)
        End Get
        Set(value As Integer)
            SetValue(ColumnProperty, value)
        End Set
    End Property

    Public Shared ReadOnly CopyProperty As DependencyProperty = DependencyProperty.Register(NameOf(Copy), GetType(Boolean), GetType(MainViewMode))
    Public Property Copy As Boolean
        Get
            Return GetValue(CopyProperty)
        End Get
        Set(value As Boolean)
            SetValue(CopyProperty, value)
        End Set
    End Property

    Public Shared ReadOnly PlantingSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(PlantingSelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property PlantingSelectedIndex As Integer
        Get
            Return GetValue(PlantingSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(PlantingSelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly PutZombieSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(PutZombieSelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property PutZombieSelectedIndex As Integer
        Get
            Return GetValue(PutZombieSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(PutZombieSelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly CellTypeSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(CellTypeSelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property CellTypeSelectedIndex As Integer
        Get
            Return GetValue(CellTypeSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(CellTypeSelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly RowNumSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(RowNumSelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property RowNumSelectedIndex As Integer
        Get
            Return GetValue(RowNumSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(RowNumSelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly RowTypeSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(RowTypeSelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property RowTypeSelectedIndex As Integer
        Get
            Return GetValue(RowTypeSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(RowTypeSelectedIndexProperty, value)
        End Set
    End Property
#End Region

#Region "穿越"
    Public Shared ReadOnly JmpLevelNumProperty As DependencyProperty = DependencyProperty.Register(NameOf(JmpLevelNum), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(1))
    Public Property JmpLevelNum As Integer
        Get
            Return GetValue(JmpLevelNumProperty)
        End Get
        Set(value As Integer)
            SetValue(JmpLevelNumProperty, value)
        End Set
    End Property

    Public Shared ReadOnly MixModeSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(MixModeSelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property MixModeSelectedIndex As Integer
        Get
            Return GetValue(MixModeSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(MixModeSelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly MixModeLevelProperty As DependencyProperty = DependencyProperty.Register(NameOf(MixModeLevel), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(1))
    Public Property MixModeLevel As Integer
        Get
            Return GetValue(MixModeLevelProperty)
        End Get
        Set(value As Integer)
            SetValue(MixModeLevelProperty, value)
        End Set
    End Property

    Public Shared ReadOnly UnlockProperty As DependencyProperty = DependencyProperty.Register(NameOf(Unlock), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf UnlockChangedCallback))
    Public Property Unlock As Boolean
        Get
            Return GetValue(UnlockProperty)
        End Get
        Set(value As Boolean)
            SetValue(UnlockProperty, value)
        End Set
    End Property
    Private Shared Sub UnlockChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.Unlock(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly LockProperty As DependencyProperty = DependencyProperty.Register(NameOf(Lock), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf LockChangedCallback))
    Public Property Lock As Boolean
        Get
            Return GetValue(LockProperty)
        End Get
        Set(value As Boolean)
            SetValue(LockProperty, value)
        End Set
    End Property
    Private Shared Sub LockChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        Dim mode As MainViewMode = CType(d, MainViewMode)
        If mode.InitState = InitErr.Success Then
            Helper.Lock(e.NewValue, mode.IZESelectedIndex)
        End If
    End Sub

    Public Shared ReadOnly IZESelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(IZESelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property IZESelectedIndex As Integer
        Get
            Return GetValue(IZESelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(IZESelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly LittleProperty As DependencyProperty = DependencyProperty.Register(NameOf(Little), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf LittleChangedCallback))
    Public Property Little As Boolean
        Get
            Return GetValue(LittleProperty)
        End Get
        Set(value As Boolean)
            SetValue(LittleProperty, value)
        End Set
    End Property
    Private Shared Sub LittleChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.Little(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly RainProperty As DependencyProperty = DependencyProperty.Register(NameOf(Rain), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf RainChangedCallback))
    Public Property Rain As Boolean
        Get
            Return GetValue(RainProperty)
        End Get
        Set(value As Boolean)
            SetValue(RainProperty, value)
        End Set
    End Property
    Private Shared Sub RainChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.Rain(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly StormProperty As DependencyProperty = DependencyProperty.Register(NameOf(Storm), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf StormChangedCallback))
    Public Property Storm As Boolean
        Get
            Return GetValue(StormProperty)
        End Get
        Set(value As Boolean)
            SetValue(StormProperty, value)
        End Set
    End Property
    Private Shared Sub StormChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.Storm(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly ForceProperty As DependencyProperty = DependencyProperty.Register(NameOf(Force), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf ForceChangedCallback))
    Public Property Force As Boolean
        Get
            Return GetValue(ForceProperty)
        End Get
        Set(value As Boolean)
            SetValue(ForceProperty, value)
        End Set
    End Property
    Private Shared Sub ForceChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.Force(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly LikeColProperty As DependencyProperty = DependencyProperty.Register(NameOf(LikeCol), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf LikeColChangedCallback))
    Public Property LikeCol As Boolean
        Get
            Return GetValue(LikeColProperty)
        End Get
        Set(value As Boolean)
            SetValue(LikeColProperty, value)
        End Set
    End Property
    Private Shared Sub LikeColChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.LikeCol(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly RunFastProperty As DependencyProperty = DependencyProperty.Register(NameOf(RunFast), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf RunFastChangedCallback))
    Public Property RunFast As Boolean
        Get
            Return GetValue(RunFastProperty)
        End Get
        Set(value As Boolean)
            SetValue(RunFastProperty, value)
        End Set
    End Property
    Private Shared Sub RunFastChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.RunFast(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly StartDoorProperty As DependencyProperty = DependencyProperty.Register(NameOf(StartDoor), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf StartDoorChangedCallback))
    Public Property StartDoor As Boolean
        Get
            Return GetValue(StartDoorProperty)
        End Get
        Set(value As Boolean)
            SetValue(StartDoorProperty, value)
        End Set
    End Property
    Private Shared Sub StartDoorChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.StartDoor(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly StableDoorProperty As DependencyProperty = DependencyProperty.Register(NameOf(StableDoor), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf StableDoorChangedCallback))
    Public Property StableDoor As Boolean
        Get
            Return GetValue(StableDoorProperty)
        End Get
        Set(value As Boolean)
            SetValue(StableDoorProperty, value)
        End Set
    End Property
    Private Shared Sub StableDoorChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.StableDoor(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly Black1RowProperty As DependencyProperty = DependencyProperty.Register(NameOf(Black1Row), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(1))
    Public Property Black1Row As Integer
        Get
            Return GetValue(Black1RowProperty)
        End Get
        Set(value As Integer)
            SetValue(Black1RowProperty, value)
        End Set
    End Property

    Public Shared ReadOnly Black1ColumnProperty As DependencyProperty = DependencyProperty.Register(NameOf(Black1Column), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(3))
    Public Property Black1Column As Integer
        Get
            Return GetValue(Black1ColumnProperty)
        End Get
        Set(value As Integer)
            SetValue(Black1ColumnProperty, value)
        End Set
    End Property

    Public Shared ReadOnly Black2RowProperty As DependencyProperty = DependencyProperty.Register(NameOf(Black2Row), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(2))
    Public Property Black2Row As Integer
        Get
            Return GetValue(Black2RowProperty)
        End Get
        Set(value As Integer)
            SetValue(Black2RowProperty, value)
        End Set
    End Property

    Public Shared ReadOnly Black2ColumnProperty As DependencyProperty = DependencyProperty.Register(NameOf(Black2Column), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(10))
    Public Property Black2Column As Integer
        Get
            Return GetValue(Black2ColumnProperty)
        End Get
        Set(value As Integer)
            SetValue(Black2ColumnProperty, value)
        End Set
    End Property

    Public Shared ReadOnly White1RowProperty As DependencyProperty = DependencyProperty.Register(NameOf(White1Row), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(4))
    Public Property White1Row As Integer
        Get
            Return GetValue(White1RowProperty)
        End Get
        Set(value As Integer)
            SetValue(White1RowProperty, value)
        End Set
    End Property

    Public Shared ReadOnly White1ColumnProperty As DependencyProperty = DependencyProperty.Register(NameOf(White1Column), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(10))
    Public Property White1Column As Integer
        Get
            Return GetValue(White1ColumnProperty)
        End Get
        Set(value As Integer)
            SetValue(White1ColumnProperty, value)
        End Set
    End Property

    Public Shared ReadOnly White2RowProperty As DependencyProperty = DependencyProperty.Register(NameOf(White2Row), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(5))
    Public Property White2Row As Integer
        Get
            Return GetValue(White2RowProperty)
        End Get
        Set(value As Integer)
            SetValue(White2RowProperty, value)
        End Set
    End Property

    Public Shared ReadOnly White2ColumnProperty As DependencyProperty = DependencyProperty.Register(NameOf(White2Column), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(3))
    Public Property White2Column As Integer
        Get
            Return GetValue(White2ColumnProperty)
        End Get
        Set(value As Integer)
            SetValue(White2ColumnProperty, value)
        End Set
    End Property
#End Region

#Region "植物"
    Public Shared ReadOnly CornNoWaitProperty As DependencyProperty = DependencyProperty.Register(NameOf(CornNoWait), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf CornNoWaitChangedCallback))
    Public Property CornNoWait As Boolean
        Get
            Return GetValue(CornNoWaitProperty)
        End Get
        Set(value As Boolean)
            SetValue(CornNoWaitProperty, value)
        End Set
    End Property
    Private Shared Sub CornNoWaitChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.CornNoWait(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly MagnetNoWaitProperty As DependencyProperty = DependencyProperty.Register(NameOf(MagnetNoWait), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf MagnetNoWaitChangedCallback))
    Public Property MagnetNoWait As Boolean
        Get
            Return GetValue(MagnetNoWaitProperty)
        End Get
        Set(value As Boolean)
            SetValue(MagnetNoWaitProperty, value)
        End Set
    End Property
    Private Shared Sub MagnetNoWaitChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.MagnetNoWait(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly PotatoNoWaitProperty As DependencyProperty = DependencyProperty.Register(NameOf(PotatoNoWait), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf PotatoNoWaitChangedCallback))
    Public Property PotatoNoWait As Boolean
        Get
            Return GetValue(PotatoNoWaitProperty)
        End Get
        Set(value As Boolean)
            SetValue(PotatoNoWaitProperty, value)
        End Set
    End Property
    Private Shared Sub PotatoNoWaitChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.PotatoNoWait(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly MouthNoWaitProperty As DependencyProperty = DependencyProperty.Register(NameOf(MouthNoWait), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf MouthNoWaitChangedCallback))
    Public Property MouthNoWait As Boolean
        Get
            Return GetValue(MouthNoWaitProperty)
        End Get
        Set(value As Boolean)
            SetValue(MouthNoWaitProperty, value)
        End Set
    End Property
    Private Shared Sub MouthNoWaitChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.MouthNoWait(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly NoMarkProperty As DependencyProperty = DependencyProperty.Register(NameOf(NoMark), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf NoMarkChangedCallback))
    Public Property NoMark As Boolean
        Get
            Return GetValue(NoMarkProperty)
        End Get
        Set(value As Boolean)
            SetValue(NoMarkProperty, value)
        End Set
    End Property
    Private Shared Sub NoMarkChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.NoMark(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly NoSleepProperty As DependencyProperty = DependencyProperty.Register(NameOf(NoSleep), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf NoSleepChangedCallback))
    Public Property NoSleep As Boolean
        Get
            Return GetValue(NoSleepProperty)
        End Get
        Set(value As Boolean)
            SetValue(NoSleepProperty, value)
        End Set
    End Property
    Private Shared Sub NoSleepChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.NoSleep(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly StrongWindProperty As DependencyProperty = DependencyProperty.Register(NameOf(StrongWind), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf StrongWindChangedCallback))
    Public Property StrongWind As Boolean
        Get
            Return GetValue(StrongWindProperty)
        End Get
        Set(value As Boolean)
            SetValue(StrongWindProperty, value)
        End Set
    End Property
    Private Shared Sub StrongWindChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.StrongWind(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly StrongBeanProperty As DependencyProperty = DependencyProperty.Register(NameOf(StrongBean), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf StrongBeanChangedCallback))
    Public Property StrongBean As Boolean
        Get
            Return GetValue(StrongBeanProperty)
        End Get
        Set(value As Boolean)
            SetValue(StrongBeanProperty, value)
        End Set
    End Property
    Private Shared Sub StrongBeanChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.StrongBean(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly UmbrellaProperty As DependencyProperty = DependencyProperty.Register(NameOf(Umbrella), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf UmbrellaChangedCallback))
    Public Property Umbrella As Boolean
        Get
            Return GetValue(UmbrellaProperty)
        End Get
        Set(value As Boolean)
            SetValue(UmbrellaProperty, value)
        End Set
    End Property
    Private Shared Sub UmbrellaChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.Umbrella(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly NoCDProperty As DependencyProperty = DependencyProperty.Register(NameOf(NoCD), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf NoCDChangedCallback))
    Public Property NoCD As Boolean
        Get
            Return GetValue(NoCDProperty)
        End Get
        Set(value As Boolean)
            SetValue(NoCDProperty, value)
        End Set
    End Property
    Private Shared Sub NoCDChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.NoCD(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly LockScoopProperty As DependencyProperty = DependencyProperty.Register(NameOf(LockScoop), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf LockScoopChangedCallback))
    Public Property LockScoop As Boolean
        Get
            Return GetValue(LockScoopProperty)
        End Get
        Set(value As Boolean)
            SetValue(LockScoopProperty, value)
        End Set
    End Property
    Private Shared Sub LockScoopChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.LockScoop(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly AnyMoveProperty As DependencyProperty = DependencyProperty.Register(NameOf(AnyMove), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf AnyMoveChangedCallback))
    Public Property AnyMove As Boolean
        Get
            Return GetValue(AnyMoveProperty)
        End Get
        Set(value As Boolean)
            SetValue(AnyMoveProperty, value)
        End Set
    End Property
    Private Shared Sub AnyMoveChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.AnyMove(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly NoPurpleProperty As DependencyProperty = DependencyProperty.Register(NameOf(NoPurple), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf NoPurpleChangedCallback))
    Public Property NoPurple As Boolean
        Get
            Return GetValue(NoPurpleProperty)
        End Get
        Set(value As Boolean)
            SetValue(NoPurpleProperty, value)
        End Set
    End Property
    Private Shared Sub NoPurpleChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.NoPurple(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly FSBoomProperty As DependencyProperty = DependencyProperty.Register(NameOf(FSBoom), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf FSBoomChangedCallback))
    Public Property FSBoom As Boolean
        Get
            Return GetValue(FSBoomProperty)
        End Get
        Set(value As Boolean)
            SetValue(FSBoomProperty, value)
        End Set
    End Property
    Private Shared Sub FSBoomChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.FSBoom(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly FSSquashProperty As DependencyProperty = DependencyProperty.Register(NameOf(FSSquash), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf FSSquashChangedCallback))
    Public Property FSSquash As Boolean
        Get
            Return GetValue(FSSquashProperty)
        End Get
        Set(value As Boolean)
            SetValue(FSSquashProperty, value)
        End Set
    End Property
    Private Shared Sub FSSquashChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.FSSquash(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly FSMagnetProperty As DependencyProperty = DependencyProperty.Register(NameOf(FSMagnet), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf FSMagnetChangedCallback))
    Public Property FSMagnet As Boolean
        Get
            Return GetValue(FSMagnetProperty)
        End Get
        Set(value As Boolean)
            SetValue(FSMagnetProperty, value)
        End Set
    End Property
    Private Shared Sub FSMagnetChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.FSMagnet(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly FSSputterProperty As DependencyProperty = DependencyProperty.Register(NameOf(FSSputter), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf FSSputterChangedCallback))
    Public Property FSSputter As Boolean
        Get
            Return GetValue(FSSputterProperty)
        End Get
        Set(value As Boolean)
            SetValue(FSSputterProperty, value)
        End Set
    End Property
    Private Shared Sub FSSputterChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.FSSputter(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly FSMouthProperty As DependencyProperty = DependencyProperty.Register(NameOf(FSMouth), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf FSMouthChangedCallback))
    Public Property FSMouth As Boolean
        Get
            Return GetValue(FSMouthProperty)
        End Get
        Set(value As Boolean)
            SetValue(FSMouthProperty, value)
        End Set
    End Property
    Private Shared Sub FSMouthChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.FSMouth(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly FSPrickProperty As DependencyProperty = DependencyProperty.Register(NameOf(FSPrick), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf FSPrickChangedCallback))
    Public Property FSPrick As Boolean
        Get
            Return GetValue(FSPrickProperty)
        End Get
        Set(value As Boolean)
            SetValue(FSPrickProperty, value)
        End Set
    End Property
    Private Shared Sub FSPrickChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.FSPrick(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly FSSpoutProperty As DependencyProperty = DependencyProperty.Register(NameOf(FSSpout), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf FSSpoutChangedCallback))
    Public Property FSSpout As Boolean
        Get
            Return GetValue(FSSpoutProperty)
        End Get
        Set(value As Boolean)
            SetValue(FSSpoutProperty, value)
        End Set
    End Property
    Private Shared Sub FSSpoutChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.FSSpout(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly FSCatProperty As DependencyProperty = DependencyProperty.Register(NameOf(FSCat), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf FSCatChangedCallback))
    Public Property FSCat As Boolean
        Get
            Return GetValue(FSCatProperty)
        End Get
        Set(value As Boolean)
            SetValue(FSCatProperty, value)
        End Set
    End Property
    Private Shared Sub FSCatChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.FSCat(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly CatTouchProperty As DependencyProperty = DependencyProperty.Register(NameOf(CatTouch), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf CatTouchChangedCallback))
    Public Property CatTouch As Boolean
        Get
            Return GetValue(CatTouchProperty)
        End Get
        Set(value As Boolean)
            SetValue(CatTouchProperty, value)
        End Set
    End Property
    Private Shared Sub CatTouchChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.CatTouch(e.NewValue)
        End If
    End Sub
#End Region

#Region "僵尸"
    Public Shared ReadOnly NoSlowProperty As DependencyProperty = DependencyProperty.Register(NameOf(NoSlow), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf NoSlowChangedCallback))
    Public Property NoSlow As Boolean
        Get
            Return GetValue(NoSlowProperty)
        End Get
        Set(value As Boolean)
            SetValue(NoSlowProperty, value)
        End Set
    End Property
    Private Shared Sub NoSlowChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.NoSlow(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly NoButterProperty As DependencyProperty = DependencyProperty.Register(NameOf(NoButter), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf NoButterChangedCallback))
    Public Property NoButter As Boolean
        Get
            Return GetValue(NoButterProperty)
        End Get
        Set(value As Boolean)
            SetValue(NoButterProperty, value)
        End Set
    End Property
    Private Shared Sub NoButterChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.NoButter(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly No3zGroupProperty As DependencyProperty = DependencyProperty.Register(NameOf(No3zGroup), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf No3zGroupChangedCallback))
    Public Property No3zGroup As Boolean
        Get
            Return GetValue(No3zGroupProperty)
        End Get
        Set(value As Boolean)
            SetValue(No3zGroupProperty, value)
        End Set
    End Property
    Private Shared Sub No3zGroupChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.No3zGroup(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly NoIMPProperty As DependencyProperty = DependencyProperty.Register(NameOf(NoIMP), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf NoIMPChangedCallback))
    Public Property NoIMP As Boolean
        Get
            Return GetValue(NoIMPProperty)
        End Get
        Set(value As Boolean)
            SetValue(NoIMPProperty, value)
        End Set
    End Property
    Private Shared Sub NoIMPChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.NoIMP(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly NoICEProperty As DependencyProperty = DependencyProperty.Register(NameOf(NoICE), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf NoICEChangedCallback))
    Public Property NoICE As Boolean
        Get
            Return GetValue(NoICEProperty)
        End Get
        Set(value As Boolean)
            SetValue(NoICEProperty, value)
        End Set
    End Property
    Private Shared Sub NoICEChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.NoICE(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly ZEatableProperty As DependencyProperty = DependencyProperty.Register(NameOf(ZEatable), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf ZEatableChangedCallback))
    Public Property ZEatable As Boolean
        Get
            Return GetValue(ZEatableProperty)
        End Get
        Set(value As Boolean)
            SetValue(ZEatableProperty, value)
        End Set
    End Property
    Private Shared Sub ZEatableChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.ZEatable(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly StopZTimerProperty As DependencyProperty = DependencyProperty.Register(NameOf(StopZTimer), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf StopZTimerChangedCallback))
    Public Property StopZTimer As Boolean
        Get
            Return GetValue(StopZTimerProperty)
        End Get
        Set(value As Boolean)
            SetValue(StopZTimerProperty, value)
        End Set
    End Property
    Private Shared Sub StopZTimerChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.StopZTimer(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly StopZombiesProperty As DependencyProperty = DependencyProperty.Register(NameOf(StopZombies), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf StopZombiesChangedCallback))
    Public Property StopZombies As Boolean
        Get
            Return GetValue(StopZombiesProperty)
        End Get
        Set(value As Boolean)
            SetValue(StopZombiesProperty, value)
        End Set
    End Property
    Private Shared Sub StopZombiesChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.StopZombies(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly StopBossProperty As DependencyProperty = DependencyProperty.Register(NameOf(StopBoss), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf StopBossChangedCallback))
    Public Property StopBoss As Boolean
        Get
            Return GetValue(StopBossProperty)
        End Get
        Set(value As Boolean)
            SetValue(StopBossProperty, value)
        End Set
    End Property
    Private Shared Sub StopBossChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.StopBoss(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly BalloonBoomProperty As DependencyProperty = DependencyProperty.Register(NameOf(BalloonBoom), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf BalloonBoomChangedCallback))
    Public Property BalloonBoom As Boolean
        Get
            Return GetValue(BalloonBoomProperty)
        End Get
        Set(value As Boolean)
            SetValue(BalloonBoomProperty, value)
        End Set
    End Property
    Private Shared Sub BalloonBoomChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.BalloonBoom(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly ZTimerProperty As DependencyProperty = DependencyProperty.Register(NameOf(ZTimer), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf ZTimerChangedCallback))
    Public Property ZTimer As Boolean
        Get
            Return GetValue(ZTimerProperty)
        End Get
        Set(value As Boolean)
            SetValue(ZTimerProperty, value)
        End Set
    End Property
    Private Shared Sub ZTimerChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        Dim mode As MainViewMode = CType(d, MainViewMode)
        If mode.InitState = InitErr.Success Then
            If e.NewValue Then
                Helper.ZTimerSpeed(400 - mode.ZTimerSpeed)
            Else
                Helper.ZTimerSpeed(-1)
            End If
        End If
    End Sub

    Public Shared ReadOnly ZTimerSpeedProperty As DependencyProperty = DependencyProperty.Register(NameOf(ZTimerSpeed), GetType(Double), GetType(MainViewMode), New PropertyMetadata(0.0, AddressOf ZTimerSpeedChangedCallback))
    Public Property ZTimerSpeed As Double
        Get
            Return GetValue(ZTimerSpeedProperty)
        End Get
        Set(value As Double)
            SetValue(ZTimerSpeedProperty, value)
        End Set
    End Property
    Private Shared Sub ZTimerSpeedChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        Dim mode As MainViewMode = CType(d, MainViewMode)
        If mode.InitState = InitErr.Success AndAlso mode.ZTimer Then
            Helper.ZTimerSpeed(CInt(CDbl(e.NewValue)))
        End If
    End Sub

    Public Shared ReadOnly GatherZombieProperty As DependencyProperty = DependencyProperty.Register(NameOf(GatherZombie), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf GatherZombieChangedCallback))
    Public Property GatherZombie As Boolean
        Get
            Return GetValue(GatherZombieProperty)
        End Get
        Set(value As Boolean)
            SetValue(GatherZombieProperty, value)
        End Set
    End Property
    Private Shared Sub GatherZombieChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        Dim mode As MainViewMode = CType(d, MainViewMode)
        If mode.InitState = InitErr.Success Then
            Helper.GatherZombie(e.NewValue, mode.GatherZombiePosition)
        End If
    End Sub

    Public Shared ReadOnly GatherZombiePositionProperty As DependencyProperty = DependencyProperty.Register(NameOf(GatherZombiePosition), GetType(Double), GetType(MainViewMode), New PropertyMetadata(800.0, AddressOf GatherZombiePositionChangedCallback))
    Public Property GatherZombiePosition As Double
        Get
            Return GetValue(GatherZombiePositionProperty)
        End Get
        Set(value As Double)
            SetValue(GatherZombiePositionProperty, value)
        End Set
    End Property
    Private Shared Sub GatherZombiePositionChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        Dim mode As MainViewMode = CType(d, MainViewMode)
        If mode.InitState = InitErr.Success AndAlso mode.GatherZombie Then
            Helper.GatherZombie(True, CSng(CDbl(e.NewValue)))
        End If
    End Sub
#End Region

#Region "调节"
    Public Shared ReadOnly CardNumSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(CardNumSelectedIndex), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(9))
    Public Property CardNumSelectedIndex As Integer
        Get
            Return GetValue(CardNumSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(CardNumSelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly SetCardNumSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(SetCardNumSelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property SetCardNumSelectedIndex As Integer
        Get
            Return GetValue(SetCardNumSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(SetCardNumSelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly SetPCardNumSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(SetPCardNumSelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property SetPCardNumSelectedIndex As Integer
        Get
            Return GetValue(SetPCardNumSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(SetPCardNumSelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly SetPCardCopyProperty As DependencyProperty = DependencyProperty.Register(NameOf(SetPCardCopy), GetType(Boolean), GetType(MainViewMode))
    Public Property SetPCardCopy As Boolean
        Get
            Return GetValue(SetPCardCopyProperty)
        End Get
        Set(value As Boolean)
            SetValue(SetPCardCopyProperty, value)
        End Set
    End Property

    Public Shared ReadOnly SetZCardNumSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(SetZCardNumSelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property SetZCardNumSelectedIndex As Integer
        Get
            Return GetValue(SetZCardNumSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(SetZCardNumSelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly ZListSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(ZListSelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property ZListSelectedIndex As Integer
        Get
            Return GetValue(ZListSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(ZListSelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly ZListBoxProperty As DependencyProperty = DependencyProperty.Register(NameOf(ZListBox), GetType(ObservableCollection(Of Zombie)), GetType(MainViewMode), New PropertyMetadata(New ObservableCollection(Of Zombie)()))
    Public Property ZListBox As ObservableCollection(Of Zombie)
        Get
            Return GetValue(ZListBoxProperty)
        End Get
        Set(value As ObservableCollection(Of Zombie))
            SetValue(ZListBoxProperty, value)
        End Set
    End Property

    Public Shared ReadOnly ZListBoxSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(ZListBoxSelectedIndex), GetType(Integer), GetType(MainViewMode))
    Public Property ZListBoxSelectedIndex As Integer
        Get
            Return GetValue(ZListBoxSelectedIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(ZListBoxSelectedIndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly NaturalSeedProperty As DependencyProperty = DependencyProperty.Register(NameOf(NaturalSeed), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(True))
    Public Property NaturalSeed As Boolean
        Get
            Return GetValue(NaturalSeedProperty)
        End Get
        Set(value As Boolean)
            SetValue(NaturalSeedProperty, value)
        End Set
    End Property

    Public Shared ReadOnly ThiefLimitProperty As DependencyProperty = DependencyProperty.Register(NameOf(ThiefLimit), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf LimitPropertyChanged))
    Public Property ThiefLimit As Boolean
        Get
            Return GetValue(ThiefLimitProperty)
        End Get
        Set(value As Boolean)
            SetValue(ThiefLimitProperty, value)
        End Set
    End Property
    Private Shared Sub LimitPropertyChanged(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        Dim mode As MainViewMode = d
        If Not mode.NaturalSeed Then
            Dim zombies() As Integer = Helper.SeeLeftZombies().Select(Function(left, index) If(left, New Integer?(index), Nothing)).Where(Function(z) z IsNot Nothing).Select(Function(i) i.value()).ToArray()
            Helper.LimitSeed(zombies, mode.ThiefLimit, mode.RedLimit)
        End If
    End Sub

    Public Shared ReadOnly RedLimitProperty As DependencyProperty = DependencyProperty.Register(NameOf(RedLimit), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf LimitPropertyChanged))
    Public Property RedLimit As Boolean
        Get
            Return GetValue(RedLimitProperty)
        End Get
        Set(value As Boolean)
            SetValue(RedLimitProperty, value)
        End Set
    End Property
#End Region

#Region "花园"
    Public Shared ReadOnly FlowerNPKProperty As DependencyProperty = DependencyProperty.Register(NameOf(FlowerNPK), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf FlowerNPKChangedCallback))
    Public Property FlowerNPK As Boolean
        Get
            Return GetValue(FlowerNPKProperty)
        End Get
        Set(value As Boolean)
            SetValue(FlowerNPKProperty, value)
        End Set
    End Property
    Private Shared Sub FlowerNPKChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.FlowerNPK(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly MedicineProperty As DependencyProperty = DependencyProperty.Register(NameOf(Medicine), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf MedicineChangedCallback))
    Public Property Medicine As Boolean
        Get
            Return GetValue(MedicineProperty)
        End Get
        Set(value As Boolean)
            SetValue(MedicineProperty, value)
        End Set
    End Property
    Private Shared Sub MedicineChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.Medicine(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly ChocolateProperty As DependencyProperty = DependencyProperty.Register(NameOf(Chocolate), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf ChocolateChangedCallback))
    Public Property Chocolate As Boolean
        Get
            Return GetValue(ChocolateProperty)
        End Get
        Set(value As Boolean)
            SetValue(ChocolateProperty, value)
        End Set
    End Property
    Private Shared Sub ChocolateChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.Chocolate(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly TreeNPKProperty As DependencyProperty = DependencyProperty.Register(NameOf(TreeNPK), GetType(Boolean), GetType(MainViewMode), New PropertyMetadata(False, AddressOf TreeNPKChangedCallback))
    Public Property TreeNPK As Boolean
        Get
            Return GetValue(TreeNPKProperty)
        End Get
        Set(value As Boolean)
            SetValue(TreeNPKProperty, value)
        End Set
    End Property
    Private Shared Sub TreeNPKChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        If CType(d, MainViewMode).InitState = InitErr.Success Then
            Helper.TreeNPK(e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly TreeHeightProperty As DependencyProperty = DependencyProperty.Register(NameOf(TreeHeight), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(100))
    Public Property TreeHeight As Integer
        Get
            Return GetValue(TreeHeightProperty)
        End Get
        Set(value As Integer)
            SetValue(TreeHeightProperty, value)
        End Set
    End Property

    Public Shared ReadOnly FlowerNumProperty As DependencyProperty = DependencyProperty.Register(NameOf(FlowerNum), GetType(Integer), GetType(MainViewMode), New PropertyMetadata(32))
    Public Property FlowerNum As Integer
        Get
            Return GetValue(FlowerNumProperty)
        End Get
        Set(value As Integer)
            SetValue(FlowerNumProperty, value)
        End Set
    End Property

    Public Shared ReadOnly FlowerIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(FlowerIndex), GetType(Integer), GetType(MainViewMode))
    Public Property FlowerIndex As Integer
        Get
            Return GetValue(FlowerIndexProperty)
        End Get
        Set(value As Integer)
            SetValue(FlowerIndexProperty, value)
        End Set
    End Property
    'Private Shared Sub FlowerIndexChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
    '    Dim mode As MainViewMode = CType(d, MainViewMode)
    '    If mode.InitState = InitErr.Success Then
    '        Dim id As Integer = e.NewValue
    '        mode.FlowerListSelectedIndex = Helper.GetFlower(id)
    '        mode.FlowerSizeSelectedIndex = Helper.GetFSize(id)
    '        mode.FlowerPlaceSelectedIndex = Helper.GetFPlace(id)
    '    End If
    'End Sub

    'Public Shared ReadOnly FlowerListSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(FlowerListSelectedIndex), GetType(Integer), GetType(MainViewMode))
    'Public Property FlowerListSelectedIndex As Integer
    '    Get
    '        Return GetValue(FlowerListSelectedIndexProperty)
    '    End Get
    '    Set(value As Integer)
    '        SetValue(FlowerListSelectedIndexProperty, value)
    '    End Set
    'End Property

    'Public Shared ReadOnly FlowerSizeSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(FlowerSizeSelectedIndex), GetType(Integer), GetType(MainViewMode))
    'Public Property FlowerSizeSelectedIndex As Integer
    '    Get
    '        Return GetValue(FlowerSizeSelectedIndexProperty)
    '    End Get
    '    Set(value As Integer)
    '        SetValue(FlowerSizeSelectedIndexProperty, value)
    '    End Set
    'End Property

    'Public Shared ReadOnly FlowerPlaceSelectedIndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(FlowerPlaceSelectedIndex), GetType(Integer), GetType(MainViewMode))
    'Public Property FlowerPlaceSelectedIndex As Integer
    '    Get
    '        Return GetValue(FlowerPlaceSelectedIndexProperty)
    '    End Get
    '    Set(value As Integer)
    '        SetValue(FlowerPlaceSelectedIndexProperty, value)
    '    End Set
    'End Property

    Public Shared ReadOnly FlowersProperty As DependencyProperty = DependencyProperty.Register(NameOf(Flowers), GetType(ObservableCollection(Of Flower)), GetType(MainViewMode), New PropertyMetadata(New ObservableCollection(Of Flower)()))
    Public Property Flowers As ObservableCollection(Of Flower)
        Get
            Return GetValue(FlowersProperty)
        End Get
        Set(value As ObservableCollection(Of Flower))
            SetValue(FlowersProperty, value)
        End Set
    End Property
#End Region

End Class
