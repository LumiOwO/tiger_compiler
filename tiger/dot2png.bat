@echo off

if {%1}=={} (
	echo Please specify the path of input file!
	echo Usage: %~n0 ^<dot file^>
) else (
	echo Using graphviz tools. Make sure to add it to PATH.
	echo dot -Tpng %1 -o %~n1.png 
	dot -Tpng %1 -o %~n1.png
)

pause