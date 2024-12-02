# Tarefas

## Tarefa 1:

A tarefa 1 é uma tarefa com o intuito de utilizar as tarefas desenvolvidas previamente para que duas tarefas, cada uma dentro de uma raspberry diferente, se comuniquem. Contudo, alguns detalhes, que antes não afetavam o desenvolvimento das outras tarefas, surgiram e defasaram o desenvolvimento da atividade.

Um deles sendo o fato de que ambas os dispositivos possuem o mesmo endereço MAC e estão configurados como clientes DHCP, de forma que quando os dois dispositivos são ligados, é necessário um servidor DHCP para fornecer um endereço de IP para os hosts, no entando, por conta do endereço MAC duplicado na rede, a tabela arp é mantida com apenas um endereço IP e um endereço MAC, de forma que se faz necessária a alteração tanto do endereço MAC quanto do endereço IP do dispositivo.

Usando alguns dos comandos do próprio sistema operacional, é possível realizar alterações de endereço IP, por exemplo:
```
ifconfig <NOME DA INTERFACE> inet <NOVO ENDEREÇO IP>
```

Também é possível fazer com que o equipamento não seja um cliente DHCP por meio da linha de comando:
```
ifconfig <NOME DA INTERFACE> -dhcp
```
No entanto, o fato de que o VxWorks realiza uma alteração no endereço MAC das raspberrys para o valor 00:11:22:33:44:55, um dos dispositivos pode ter o endereço MAC alterado por meio do seguinte comando:
```
ifconfig <NOME DA INTERFACE> lladdr <NOVO ENDEREÇO MAC>
```
Realizando ambas as alterações de IP e de endereço MAC a comunicação entre raspberrys deveria ser direta. Entretanto no decorrer do desenvolvimento diversos detalhes que antes não condiziam com as tarefas desenvolvidas nas etapas anteriores acabaram por afetar gravemente o desenvolvimento desta etapa.
O principal deles sendo o fato da fixação de endereço MAC pelo sistema operacional de forma que não é possível realizar uma conexão direta em um sistema onde esteja presente um servidor DHCP, será necessário configurar um dos dispositivos para depois conectar o próximo dispositivo para poder configura-lo e assim por diante.

Apesar das alterações serem simples, consistindo em alterações de IP, MAC e desativação de DHCP, realizar a alteração do endereço MAC do dispositivo torna-o inacessível para qualquer outro host conectado na rede, mesmo com a alteração do endereço MAC sendo visível por meio do comando ```arp -a```.



Porém o maior problema tem sido a troca de MAC feita por comando shell, no momento tentamos vários comandos para resolver esse problemas, porém não tivemos resultados positivos até o momento. Todos os testes feitos até o momento podem ser encontrados [aqui](#testes-feitos-para-tentar-resolver-o-problema-do-mac-address)

### Criamos tarefas que fazem o que seria esperado durante a comunicação entre as 2 Raspberries PI 3B+

A primeira tarefa foi desenvolvida utilizando como base o source code criado na etapa 2 [send_limited_data.c](/etapa_2/tarefa_3/send_limited_data.c), porém agora a tarefa envia junto com a mensagem o tempo atual da maquína, como pode ser visto na imagem abaixo.

![tarefa1](/etapa_3/image_files/send_data_with_time.png)

A tarefa executou exatamente o que era esperado, porém é preciso notar que como ela foi executada dentro da Raspberry PI 3B+, o tempo mostrado é referente ao tempo que a Raspberry estava ligada.

Para a proxíma etapa foi pensado em criar uma tarefa que recebe a informação de tempo de maquina, neste caso da minha maquina, e mostra a diferença do tempo que o dado teve entre envio e recebimento.

Seguindo o desenvolvimento utilizado na etapa 2, foi ajustado um script em Python que enviasse a informação do tempo a partir do protocolo UDP e porta 21.

Script: [udp_sender](/etapa_3/tarefa_1/udp_sender_time.py)

E então foi criado o source code [receiver_data_time](/etapa_3/tarefa_1/receiver_data_time.c), que define o tempo da Raspberry a partir do primeiro dado recebido, e a partir dele calcula a diferença de tempo do pacote enviado comparado com o pacote recebido.

![receiver](/etapa_3/image_files/receiver_data.png)

Entretando existe uma falha nesse codigo, o ideal é que o clock ou a utilização de um servidor NTP seja utilizado para sincronizar o tempo entre as maquinas conectadas na mesma rede. Possívelmente isso terá que ser utilizado nas proxímas tarefas do projeto.

## Tarefa 2:

A tarefa 2 segue o mesmo princípio da tarefa 1, de forma que se deseja realizar a comunicação entre as raspberrys em períodos diferentes.

## Tarefa 3:

Utilizando o software `Wireshark` e os scripts utilizados na [etapa2](/etapa_2/) sendo eles [send_limited_data](/etapa_2/tarefa_3/send_limited_data.c) e [receiver_data](/etapa_2/tarefa_1/receiver_data.c) , podemos verificar seus logs a partir do Wireshark. É facilmente possível limitar a filtragem dos pacotes enviados a partir do IP da maquína (192.168.0.113), o IP da Raspberry (192.168.0.241) e até o tipo de protocolo `udp`como pode ser visto nas imagens a seguir.

Quando estamos enviando dados(send_limited_data):

![wireshark1](/etapa_3/image_files/wireshark1_sending.png)

Quando estamos recebendo dados(send_limited_data):

![wireshark2](/etapa_3/image_files/wireshark_2receiver.png)

Pelo WireShark podemos ver varías informações que serão uteis nas proxímas etapas do projeto, como por exemplo o tempo, o tamanho do pacote, a porta utilizada, e até mesmo seu payload (a informação enviada).

O WireShark também tem sido muito útil para os processos de "debugging" do VxWorks, onde conseguimos descobrir o comportamento da comunicação das Raspberry's entre elas e entre a maquína.



## WORKFLOW DE COMANDOS PARA SETAR 1 RASPBERRY COM MAC E IP DIFERENTES
### CONECTAR RASP POR USB
ifconfig usb2End0 down

(ignore mensagens sobre falha DHCP)

ifconfig usb2End0 lladdr 00:11:22:33:44:54

ifconfig usb2End0 up

(esperar rasp ligar)

arp -d 192.168.0.241 (limpa arp table da maquina)

ifconfig usb2End0 192.168.0.240 netmask 255.255.255.0


## Conectando PC direto no Raspberry PI 3B+

- Comandos para Raspberry
```
# Configurar IP estatico na Raspberry:
cmd
ifconfig usb2End0 inet 192.168.1.1 netmask 255.255.255.0
# Desligar DHCP:
ifconfig usb2End0 -dhcp
```

- Comandos para Linux
```
# ifconfig -a para descobrir nome da interface
# Configurar IP estatico no Linux:
sudo ifconfig eth0 192.168.1.2 netmask 255.255.255.0
```

- Testar connectividade
```
ping 192.168.1.1
```

Restaurar Configuração da Raspberry:
```
ifconfig usb2End0 down
ifconfig usb2End0 up
dhcpc usb2End0
```

- Restaurar Configuração de Network do PC
```
sudo dhclient eth0
```


## Testes feitos para tentar resolver o problema do MAC Address.

### 1- Verificar e alterar o valor do MAC Address antes do boot automatico:
Ao acompanhar o boot do VxWorks através de um terminal com conexão via UART, é possível pausar o boot automatico do VxWorks e em teoria verificar e alterar o endereço MAC como pode ser visto a seguir:

```
U-Boot 2025.01-rc3-00027-g20f641987f83-dirty (Nov 30 2024 - 09:33:11 -0300)     
                                                                                
DRAM:  948 MiB                                                                  
RPI 3 Model B+ (0xa020d3)                                                       
Core:  79 devices, 13 uclasses, devicetree: board                               
MMC:   mmc@7e202000: 0, mmcnr@7e300000: 1                                       
Loading Environment from FAT... OK                                              
In:    serial,usbkbd                                                            
Out:   serial,vidconsole                                                        
Err:   serial,vidconsole                                                        
Net:   No ethernet found.                                                       
                                                                                
starting USB...                                                                 
Bus usb@7e980000: USB DWC2                                                      
scanning bus usb@7e980000 for devices...                                        
Warning: lan78xx_eth MAC addresses don't match:                                 
Address in DT is                b8:27:eb:15:90:74                               
Address in environment is       b8:27:eb:06:66:ec                               
4 USB Device(s) found                                                           
       scanning usb for storage devices... 0 Storage Device(s) found            
Hit any key to stop autoboot:  0                                                
U-Boot>
```
Utilizando o comando `printenv` podemos ver o endereço MAC para o `ethaddr` e `ethaddr0`.
```
ethaddr=b8:27:eb:06:66:ec
ethaddr0=b8:27:eb:06:66:ec 
```
Agora utilizando o comando `setenv ethaddr0 <novo MAC>` é possível alterar o endereço `ethaddr0`para o MAC escolhido.
```
U-Boot> setenv ethaddr0 00:11:22:33:44:52
U-Boot> saveenv
U-Boot> printenv
...
ethaddr=b8:27:eb:06:66:ec                                                       
ethaddr0=00:11:22:33:44:52  
...
U-Boot> boot
```
Porém após continuar com o boot do VxWorks é notado que o MAC da Respberry continua sendo endereço default de sempre, não resolvendo nosso problema.
```
Connecting to the Network...                                                    

Network configuration:                                                          
ifname usb2End0 inet 192.168.0.241 mac 00:11:22:33:44:55                        
                                                                                
This device is also accessible over telnet!                                     
        E.g. telnet 192.168.0.241  
```

### 2- Mudar o IP e o MAC ao mesmo tempo, ou de formas.

Buscando em referências, descobrimos que existe uma forma de alterar o MAC e o IP da Raspberry de forma relativamente fácil apenas usando essa sequência de comandos:

```
ifconfig usb2End0 lladdr 00:11:22:33:44:66
ifconfig usb2End0 inet 192.168.0.200
```
O ifconfig do VxWOrks mostra que tudo foi alterado corretamente, porém continuamos perdendo acesso por completo a Raspberry.
```
[vxWorks *]# ifconfig -a                                                        
lo0     Link type:Local loopback                                                
        inet 127.0.0.1  mask 255.255.255.255                                    
        inet6 unicast fe80::1%lo0  prefixlen 64  automatic                      
        inet6 unicast ::1  prefixlen 128                                        
        UP RUNNING LOOPBACK MULTICAST NOARP ALLMULTI                            
        MTU:1500  metric:1  VR:0  ifindex:1                                     
        RX packets:37 mcast:0 errors:0 dropped:0                                
        TX packets:37 mcast:0 errors:0                                          
        collisions:0 unsupported proto:0                                        
        RX bytes:5172  TX bytes:5172                                            
                                                                                
usb2End0        Link type:Ethernet  HWaddr 00:11:22:33:44:66                    
        inet 192.168.0.200  mask 255.255.255.0  broadcast 192.168.0.255         
        inet6 unicast fe80::211:22ff:fe33:4466%usb2End0  prefixlen 64  automatic
        UP RUNNING SIMPLEX BROADCAST MULTICAST                                  
        MTU:1500  metric:1  VR:0  ifindex:2                                     
        RX packets:4 mcast:0 errors:0 dropped:3                                 
        TX packets:25 mcast:12 errors:0                                         
        collisions:0 unsupported proto:0                                        
        RX bytes:816  TX bytes:4374   
```

Porém todas as tentativas, mesmo mudando a sequência de comandos, ou até mesmo desativando o acesso a rede antes de fazer as alterações a partir dos comandos:
```
ifconfig usb2End0 down
ifconfig usb2End0 up
```

### 3- Configurar o endereço MAC diretamente no arquivo config.txt da Raspberry.

O arquivo `config.txt`da Raspberry PI 3B+ é um dos arquivos utilizados no processo de boot da interface, e em tese existe uma linha de comando possível a se colocar nesse arquivo que alteraria o endereço MAC para o valor desejado, sendo esse comando o seguinto:
```
smsc95xx.macaddr=xx:xx:xx:xx:xx:xx
```

Infelizmente mesmo a Raspberry ligando normalmente, o endereço MAC contínua sendo alterado para o valor padrão duplicado, sendo mais uma pista que provavelmente esse valor está sendo configurado em algum dos arquivos de boot do VxWorks.

### 4- Utilizar um comando ao executar o boot do Vxworks.

Utilizando o comando `bootChange` no VxWorks shell temos essas informações:

```
-> bootChange

'.' = clear field;  '-' = go to previous field;  ^D = quit

boot device          : usb2End0 
processor number     : 0 
host name            : host 
file name            : vxWorks 
inet on ethernet (e) : 192.168.0.3:ffffff00 
inet on backplane (b): 
host inet (h)        : 192.168.0.2 
gateway inet (g)     : 192.168.0.1 
user (u)             : target 
ftp password (pw) (blank = use rsh): vxTarget 
flags (f)            : 0x0 
target name (tn)     : 
startup script (s)   : 
other (o)            : 
```
Foi tentado colocar o comando `ifconfig usb2End0 lladdr 00:11:22:33:44:52` que altera o valor do endereço MAC da Raspberry, no campo `other (o)` onde em teoria definiria um novo valor MAC para o VxWorks, porém esse valor além de não ser salvo após o reboot da Raspberry, o endereço MAC padrão não é alterado.
O uso do wireshark foi validado, de forma que foi possível visualizar o envio e recebimento de pacotes entre host que não eram de origem ou destinados à máquina que estava com a captura do wireshark rodando.


Por meio do wireshark também é possível realizar o monitoramento dos comandos realizados via telnet, como por exemplo:
![Screenshot from 2024-12-01 12-54-57](https://github.com/user-attachments/assets/4e5ef8bd-f36f-4460-848c-8fe1e87daa65)
