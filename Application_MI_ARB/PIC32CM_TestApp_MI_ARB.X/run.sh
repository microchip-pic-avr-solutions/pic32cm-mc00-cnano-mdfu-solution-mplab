# - @file: run.sh
# - @description: Shell script that demonstrates the CLI call to initiate a
# -               DFU using the pymdfu host tool
# -
# - @requirements:  python, pymdfu
# ----------------------------------------------------------------------------

# - Serial Transfer for v2 image
pymdfu update -v debug --tool serial --port <Port Name> --baudrate 115200 --image PIC32CM_TestApp_Binary_v2.img

# - Serial Transfer - FAILURE due to Anti-Rollback support
# pymdfu update -v debug --tool serial --port <Port Name> --baudrate 115200 --image PIC32CM_TestApp_Binary_v1.img