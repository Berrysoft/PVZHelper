Imports System.Globalization
Imports PVZHelper.Cli

Public Class InitErrToString
    Implements IValueConverter

    Public Function Convert(value As Object, targetType As Type, parameter As Object, culture As CultureInfo) As Object Implements IValueConverter.Convert
        Dim err As InitErr = value
        Select Case err
            Case InitErr.Success
                Return "游戏打开成功"
            Case InitErr.ExeNotFound
                Return "找不到游戏"
            Case InitErr.OpenFailed
                Return "打开游戏失败"
            Case InitErr.NotSupport
                Return "不支持的版本"
        End Select
        Return Nothing
    End Function

    Public Function ConvertBack(value As Object, targetType As Type, parameter As Object, culture As CultureInfo) As Object Implements IValueConverter.ConvertBack
        Throw New NotImplementedException()
    End Function
End Class

Public Class InitErrToBrush
    Implements IValueConverter

    Public Function Convert(value As Object, targetType As Type, parameter As Object, culture As CultureInfo) As Object Implements IValueConverter.Convert
        Dim err As InitErr = value
        Select Case err
            Case InitErr.Success
                Return Brushes.Transparent
            Case Else
                Return Brushes.Red
        End Select
    End Function

    Public Function ConvertBack(value As Object, targetType As Type, parameter As Object, culture As CultureInfo) As Object Implements IValueConverter.ConvertBack
        Throw New NotImplementedException()
    End Function
End Class

Public Class NotConverter
    Implements IValueConverter

    Public Function Convert(value As Object, targetType As Type, parameter As Object, culture As CultureInfo) As Object Implements IValueConverter.Convert
        Return Not CBool(value)
    End Function

    Public Function ConvertBack(value As Object, targetType As Type, parameter As Object, culture As CultureInfo) As Object Implements IValueConverter.ConvertBack
        Return Not CBool(value)
    End Function
End Class