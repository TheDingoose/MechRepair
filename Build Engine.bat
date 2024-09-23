@ECHO OFF

py -V | find /v "Python" >NUL 2>NUL && (
ECHO no python
PAUSE
exit)

cd %cd%\godot-4.2
scons dev_build=yes
scons target=editor vsproj=yes dev_build=yes

PAUSE
