echo %~dp0
ShaderCompiler.exe /Tps_5_0 NormalMesh_DirLight_NrmMapPS.fx
ShaderCompiler.exe /Tvs_5_0 NormalMesh_DirLight_NrmMapVS.fx
ShaderCompiler.exe /Tps_5_0 SkinMesh4_DirLight_ToonPS.fx
ShaderCompiler.exe /Tvs_5_0 SkinMesh4_DirLight_ToonVS.fx
ShaderCompiler.exe /Tps_5_0 NormalMesh_NoLight_FogPS.fx
ShaderCompiler.exe /Tvs_5_0 NormalMesh_NoLight_FogVS.fx
pause
