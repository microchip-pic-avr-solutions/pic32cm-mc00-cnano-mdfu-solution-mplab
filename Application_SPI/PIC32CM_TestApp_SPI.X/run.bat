REM - @file: run.bat
REM - @description: Batch script that demonstrates the CLI call to initiate a
REM -               DFU using the pymdfu host tool
REM -
REM - @requirements:  python, pymdfu
REM ----------------------------------------------------------------------------
@echo off
REM - Serial Command (UART)
REM pymdfu update -v debug --tool serial --port COM36 --baudrate 115200 --image PIC32CM_TestApp.img

REM - Serial Command (SPI)
pymdfu update --tool mcp2210 --image PIC32CM_TestApp.img --clk-speed 500000 --chip-select 1