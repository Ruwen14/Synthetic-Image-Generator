@echo off
echo "Setup weights"

cd resources
cd weights
for /D %%d in (*) do (
    cd %%d
    setlocal EnableDelayedExpansion
    set list =
    for %%f in (0*.weights) do (
	set list=!list! %%f
	)
  
    echo concat !list:~1! in /%%d
    type !list:~1! > %%d.weights
    endlocal
    cd ..
)


echo "Done"
pause