# AI-B6-Systemprogrammierung

## Driver

### Install

Either using
```
./install_driver.sh
```
or
```
cd driver
make all
sudo dtoverlay -d . spidev_disabler
sudo insmod fanctrl.ko
```

## App

### Install

```
cd app
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python app.py
```
