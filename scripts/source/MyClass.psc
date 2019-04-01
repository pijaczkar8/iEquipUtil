ScriptName MyClass


Function PrintHello(String a_str) Global
	Debug.Trace("Hello " + a_str + "!")
EndFunction


String Function HelloWorld(Int[] a_arr) Global Native
