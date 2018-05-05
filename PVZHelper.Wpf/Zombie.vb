Class Zombie
    Public Sub New(name As String, index As Integer)
        Me.Name = name
        Me.Index = index
    End Sub

    Public Property Name As String
    Public Property Index As Integer
    Public Overrides Function ToString() As String
        Return Name
    End Function
    Public Overrides Function Equals(obj As Object) As Boolean
        Dim z As Zombie = TryCast(obj, Zombie)
        If z IsNot Nothing Then
            Return z.Index = Me.Index
        Else
            Return False
        End If
    End Function
    Public Overrides Function GetHashCode() As Integer
        Return Index.GetHashCode()
    End Function
End Class
