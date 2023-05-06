# tv-bench-controller-firmware
TV Bench Controller LED Coprocessor Firmware

## Shell access
The shell is on the UART 1 Tx:PA 9, Rx: PA10. It's used for sending commands to
control the co-processor. The raspberry pi uses it, but it can also be used
for development with minicom. To ease the process the script `start-shell.sh`
can be run, but for it to work a minicom config file must be setup. To do so,
run the following commands:
```
sudo touch /etc/minicom/minirc.zephyr-shell
sudo echo -e "# Machine-generated file - use "minicom -s" to change parameters.\npu port             /dev/ttyUSB0\npu rtscts           No" > /etc/minicom/minirc.zephyr-shell
```
Then you can simply run the following:
```
./scripts/start-shell.sh
```
