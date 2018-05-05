Class FlowerDirButton
    Inherits Button

    Public Shared ReadOnly FlowerProperty As DependencyProperty = DependencyProperty.Register(NameOf(Flower), GetType(Flower), GetType(FlowerDirButton), New PropertyMetadata(Nothing, AddressOf FlowerChangedCallback))
    Public Property Flower As Flower
        Get
            Return GetValue(FlowerProperty)
        End Get
        Set(value As Flower)
            SetValue(FlowerProperty, value)
        End Set
    End Property
    Private Shared Sub FlowerChangedCallback(d As DependencyObject, e As DependencyPropertyChangedEventArgs)
        Dim button As Button = CType(d, Button)
        Dim f As Flower = TryCast(e.NewValue, Flower)
        If f IsNot Nothing Then
            button.Content = If(f.Dir = 0, "→", "←")
        End If
    End Sub

    Protected Overrides Sub OnClick()
        MyBase.OnClick()
        Flower.Dir = If(Flower.Dir = 0, 1, 0)
        Content = If(Flower.Dir = 0, "→", "←")
    End Sub
End Class
