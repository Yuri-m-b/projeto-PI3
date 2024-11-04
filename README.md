# Análise de determinismo em comunicação de tempo real distribuída no RPi3 com o RTOS VxWorks
Alunos: Conrado Becker Gressler e Yuri Marques Barboza


Resumo: Realizar a análise de parâmetros temporais de um sistema distribuído utilizando RPi3 e VxWorks, com mensagens de tamanho fixas de 16 Mb utilizando protocolo UDP.

## Etapa 1: Setup e tarefas de exemplo

**Objetivo**: As tarefas para a primeira etapa do projeto foram pensadas com o intuito de serem focadas ao reconhecimento, instalação e criação de tarefas simples utilizando a ferramenta VxWorks.


Tarefas:

- [x] Criar uma tarefa no VxWorks.
- [x] Criar uma tarefa periódica.
- [x] Criar uma tarefa que envia pacotes para a rede.
- [x] Documentar passo-a-passo de como configurar ambiente para programar as tarefas em VxWorks.
- [x] Documentar código e mostrar resultado para a primeira tarefa criada.
- [x] Documentar código e mostrar resultado para a tarefa períodica.
- [x] Documentar código e mostrar resultado para a tarefa que envia para a rede.

**Entrega**: [Etapa 1](/etapa_1/)

## Etapa 2: Periodicidade, logs e sincronização de tarefas

**Objetivo**: As tarefas para a segunda etapa do projeto foram pensadas com o intuito de serem focadas ao aprendizado e testes para recebimento, log de mensagens, testes com o limitação de pacotes e sincronização de tarefas simples utilizando a ferramenta VxWorks.


Tarefas:

- [x] Criar uma tarefa que lê um pacote de rede.
- [x] Criar um log para as tarefas.
- [x] Limitar pacotes enviados e adequar envio e recebimento para usar o protocolo UDP.
- [x] Sincronizar tarefas periódicas.
- [ ] Documentar código e mostrar resultado para a tarefa que lê pacotes da rede.
- [ ] Registrar e analisar diferenças nas tarefas criadas até o momento utilizando logs e adequando as tarefas de testes para o escopo desejado.

**Entrega**: [Etapa 2](/etapa_2/)

# Setup

Como o setup para o VxWorks requer muitos passos e a documentação oficial para o Raspberry 3B+ está desatualizada, decidimos criar um tutorial que documenta todas as etapas e problemas que encontramos ao longo do processo de setup para iniciar o desenvolvimento. Esse tutorial pode ser encontrado em: [SETUP](/SETUP.md)

## Referências:
https://labs.windriver.com/downloads/wrsdk-vxworks7-docs/2103/README_raspberrypi3b.html

https://martsalz.medium.com/vxworks-21-03-on-raspberry-pi-4-e7ba6e1ae10e

https://www.pi4j.com/1.2/pins/model-3b-plus-rev1.html

https://github.com/jacobcalvert/vxw7-bsp-raspberry-pi

https://www.pi4j.com/1.2/pins/model-3b-plus-rev1.html

https://www.youtube.com/watch?v=-Xaf8RKmEFQ

https://learning.windriver.com/creating-your-own-layers-and-packages

https://wiki.emacinc.com/wiki/Getting_Started_With_Minicom

https://www.ee.torontomu.ca/~courses/ee8205/Data-Sheets/Tornado-VxWorks/vxworks/ref/libIndex.html

https://www.uio.no/studier/emner/matnat/fys/FYS4220/h11/undervisningsmateriale/forelesninger-rt/2011-6_VxWorks-6.2_Application_Programmers_Guide.pdf
