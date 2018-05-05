Imports System.Globalization

Public Class RangeRule
    Inherits ValidationRule

    Public Property Minimum As Integer
    Public Property Maximum As Integer

    Public Overrides Function Validate(value As Object, cultureInfo As CultureInfo) As ValidationResult
        Try
            Dim int As Integer = value
            If int >= Minimum AndAlso int <= Maximum Then
                Return ValidationResult.ValidResult
            Else
                Return New ValidationResult(False, $"数据应在{Minimum}与{Maximum}之间")
            End If
        Catch e As OverflowException
            Return New ValidationResult(False, $"数据应在{Minimum}与{Maximum}之间")
        Catch
            Return New ValidationResult(False, "数据错误")
        End Try
    End Function
End Class
