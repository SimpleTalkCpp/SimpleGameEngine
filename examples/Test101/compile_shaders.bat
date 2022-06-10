@REM #change current directory to this file
@%~d0
@cd %~dp0

set comp=..\..\build\SimpleGameEngine-x64-windows\src\render\shader_compiler\Debug\sge_shader_compiler.exe


%comp%

@rem pause