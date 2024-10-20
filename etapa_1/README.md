# Configurando setup para VxWorks Raspberry PI 3

## Instalar Debian no Windows
O ambiente Debian pode ser baixado direto pela loja oficial da Microsoft no Windows 10 e 11

## Configurar ambiente Debian
Com o ambiente Debian instalado, após abrir o terminal pela primeira vez um login e senha serão pedidos. Nesse caso é só escolher o login e senha para continuar os proxímos passos.

## Configurando o ambiente de desenvolvimento

Primeiramente deve ser rodado o seguinte comando:
`````
sudo apt install build-essential libc6:i386
`````
Em caso de erro, tentar a seguinte sequência:
`````
sudo dpkg --add-architecture i386
sudo apt update
sudo apt install libc6:i386
sudo apt install bison flex
sudo apt install libssl-dev
sudo apt install libgnutls28-dev
`````
Em seguida ter um servidor FTP instalado no seu host de desenvolvimento facilitará a implantação de aplicativos e permitirá o acesso ao sistema de arquivos do host a partir de um alvo VxWorks.

Para acomodar as diversas configurações de tempo de execução das imagens do kernel VxWorks incluídas nos SDKs, você pode estar interessado em usar uma opção de servidor FTP baseada no pyftpdlib.
`````
sudo apt install python3-pip
sudo pip install pyftpdlib
`````
Em caso de erro, tentar a seguinte sequência:
`````
sudo apt install python3-pyftpdlib
`````

## 1 To create the SD card, download the firmware from:

https://github.com/raspberrypi/firmware/archive/1.20200212.tar.gz

E.g. wget https://github.com/raspberrypi/firmware/archive/1.20200212.tar.gz

Format an SD card as a FAT32 file system and copy the contents of the "boot" directory from the downloaded firmware to the SD card.

## 2 Copy the files from /vxsdk/sdcard/ to the SD card.

Compile a u-boot binary for Raspberry Pi 3 and copy it to the SD card

E.g. on a Ubuntu/Debian host

```
$ sudo apt install gcc-aarch64-linux-gnu
$ git clone https://gitlab.denx.de/u-boot/u-boot.git  
$ cd u-boot  
$ CROSS_COMPILE=aarch64-linux-gnu- make rpi_3_b_plus_defconfig  
$ CROSS_COMPILE=aarch64-linux-gnu- make  
```
## 3 Copy u-boot.bin to the SD card as kernel8.img.

## 4


# Referencia Principal
https://labs.windriver.com/downloads/wrsdk-vxworks7-docs/2103/README_raspberrypi3b.html
