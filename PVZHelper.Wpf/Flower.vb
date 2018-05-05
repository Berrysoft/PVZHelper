Class Flower
    Inherits DependencyObject

    Private inited As Boolean
    Public Sub New(index As Integer, type As Integer, size As Integer, dir As Integer, place As Integer)
        Me.Index = index
        Me.Type = type
        Me.Size = size
        Me.Dir = dir
        Me.Place = place
        inited = True
    End Sub

    Public Shared ReadOnly IndexProperty As DependencyProperty = DependencyProperty.Register(NameOf(Index), GetType(Integer), GetType(Flower))
    Public Property Index As Integer
        Get
            Return GetValue(IndexProperty)
        End Get
        Set(value As Integer)
            SetValue(IndexProperty, value)
        End Set
    End Property

    Public Shared ReadOnly TypeProperty As DependencyProperty = DependencyProperty.Register(NameOf(Type), GetType(Integer), GetType(Flower), New PropertyMetadata(0, AddressOf TypeChangedCallback))
    Public Property Type As Integer
        Get
            Return GetValue(TypeProperty)
        End Get
        Set(value As Integer)
            SetValue(TypeProperty, value)
        End Set
    End Property
    Private Shared Sub TypeChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        Dim f As Flower = CType(d, Flower)
        If f.inited Then
            Helper.ChangeFlower(f.Index, e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly SizeProperty As DependencyProperty = DependencyProperty.Register(NameOf(Size), GetType(Integer), GetType(Flower), New PropertyMetadata(0, AddressOf SizeChangedCallback))
    Public Property Size As Integer
        Get
            Return GetValue(SizeProperty)
        End Get
        Set(value As Integer)
            SetValue(SizeProperty, value)
        End Set
    End Property
    Private Shared Sub SizeChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        Dim f As Flower = CType(d, Flower)
        If f.inited Then
            Helper.ChangeFSize(f.Index, e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly DirProperty As DependencyProperty = DependencyProperty.Register(NameOf(Dir), GetType(Integer), GetType(Flower), New PropertyMetadata(0, AddressOf DirChangedCallback))
    Public Property Dir As Integer
        Get
            Return GetValue(DirProperty)
        End Get
        Set(value As Integer)
            SetValue(DirProperty, value)
        End Set
    End Property
    Private Shared Sub DirChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        Dim f As Flower = CType(d, Flower)
        If f.inited Then
            Helper.ChangeFDir(f.Index, e.NewValue)
        End If
    End Sub

    Public Shared ReadOnly PlaceProperty As DependencyProperty = DependencyProperty.Register(NameOf(Place), GetType(Integer), GetType(Flower), New PropertyMetadata(0, AddressOf PlaceChangedCallback))
    Public Property Place As Integer
        Get
            Return GetValue(PlaceProperty)
        End Get
        Set(value As Integer)
            SetValue(PlaceProperty, value)
        End Set
    End Property
    Private Shared Sub PlaceChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        Dim f As Flower = CType(d, Flower)
        If f.inited Then
            Helper.ChangeFPlace(f.Index, e.NewValue)
        End If
    End Sub
End Class
