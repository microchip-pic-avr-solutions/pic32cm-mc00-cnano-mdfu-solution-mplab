REM - @file: run.bat
REM - @description: Batch script that demonstrates the CLI call to initiate a
REM -               DFU using the pymdfu host tool
REM -
REM - @requirements:  python, pymdfu
REM ----------------------------------------------------------------------------

REM - Serial Transfer for v2 image
pymdfu update -v debug --tool serial --port <Port Name> --baudrate 115200 --image PIC32CM_TestApp_Binary_v2.img

REM - Serial Transfer - FAILURE due to Anti-Rollback support
REM pymdfu update -v debug --tool serial --port <Port Name> --baudrate 115200 --image PIC32CM_TestApp_Binary_v1.img