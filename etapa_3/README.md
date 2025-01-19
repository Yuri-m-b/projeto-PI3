# Tarefas

Para essa etapa do projeto a instalação de uma versão linux do Raspberry que tivesse suporte a RT preempt (Real Time Computing) foi necessaria. Após diversas tentativas falhas de conseguir instalar uma OS que funcionasse nas Raspberry 3B+, foi encontrado uma versão que tinha todos os suportes desejados, sendo ela a versão `6.6.51+rpt-rpi-v8`.
Os comandos a seguir comprovam que o software da Raspberry está com suporte ao RT Preempt:
![RTPREEMPT](/etapa_3/image_files/rtpreempt.png)

A configuração do kernel indica que o Raspberry Pi está executando um kernel totalmente preemptivo (CONFIG_PREEMPT = y), projetado para baixa latência e capacidade de resposta em tempo real.

## Tarefa 1
Seguindo o que já teria sido feito até o momento, foi criado dois códigos que realizam a comunicação entre as duas raspberry. Sendo um código feito para enviar pacotes com a informação de tempo de uma raspberry, enquanto que outro código criado para esperar e receber os pacotes enviados da outra raspberry. Tendo assim como resultado as informações no console de ambos os raspberry.
![tarefa1](/etapa_3/image_files/etapa3_tarefa1.png)

Ambos códigos recebem e enviam de forma automatica, e precisam ser encerrados manualmente.

## Tarefa 2

## Tarefa 3

Durante a realização das Tarefa 1 e Tarefa 2 foi possível acompanhar e rastrear o envio dos pacotes utilizando o software `Wireshark`, poden ser notado a comunicação e as informações dos pacotes.

![tarefa3](/etapa_3/image_files/wireshark_etapa3.png)
