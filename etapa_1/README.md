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
