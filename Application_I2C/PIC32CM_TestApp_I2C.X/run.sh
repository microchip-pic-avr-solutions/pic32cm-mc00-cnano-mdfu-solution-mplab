# - @file: run.sh
# - @description: Shell script that demonstrates the CLI call to initiate a
# -               DFU using the pymdfu host tool
# -
# - @requirements:  python, pymdfu
# ----------------------------------------------------------------------------
pymdfu update -v debug --tool mcp2221a --interface i2c --clk-speed 100000 --image PIC32CM_TestApp_I2C.img --address 32