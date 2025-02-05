# Tarefas

## Tarefa 1:

A tarefa 1 é uma tarefa com o intuito de apresentar um "hello world" no final de sua execução.
Rodando o arquivo compilado no VxWorks temos como resultado:
```
[vxWorks *]# hello_world.vxe                                                    
Launching process 'hello_world.vxe' ...                                         
Process 'hello_world.vxe' (process Id = 0xffff8000002bac10) launched.           
Iniciando a aplicação...                                                        
Task criada com sucesso, ID: 65571                                              
Hello, World! 
```

Mostrando que a tarefa foi executada com sucesso.

## Tarefa 2:

Para a tarefa 2, o objetivo principal é criar um arquivo source que tenha algum tipo de periodicidade. Enquanto desenvolviamos essa tarefa, foi notado que existe um comportamento "estranho" relacionado a loops nos arquivos compilados e executados em VxWorks. Em certos casos, quando se é utilizado mais de um `printf` ou `taskDelay` o loop é interrompido.

Dessa forma no momento foram criados dos arquivos `hello_world_period.c`e `hello_world_period2.c` que demonstram esse comportamento.

### hello_world_period
Este source file define uma função chamada repeat, que executa uma tarefa (task) um número específico de vezes. Tendo como tarefa periódica `callRepeatFunction` é uma tarefa periódica que imprime quantas vezes a exampleTask será executada e chama a função repeat para fazê-lo.

Tendo como resultado:
```
[vxWorks *]# hello_world_period.vxe                                             
Launching process 'hello_world_period.vxe' ...                                  
Process 'hello_world_period.vxe' (process Id = 0xffff8000002babe0) launched.    
Repetindo a task 5 vezes...                                                     
Hello World!                                                                    
Hello World!                                                                    
Hello World!                                                                    
Hello World!                                                                    
Hello World!  
```

### hello_world_period2
Este source file tenta criar uma tarefa periódica de forma mais simples, criando uma função chamada `helloWorldTask`, que utiliza como forma de loop um laço while que em teoria deveria se repetir 10 vezes até finalmente sair do loop. Porém como comentado anteriormente, esse loop está sendo interrompido antes de terminar o loop completamente. Ainda não sabemos o motivo desse comportamente, e planejamos continuar pesquisando sobre para as proxímas etapas do projeto.

Resultado desse source file:
```
[vxWorks *]# hello_world_period2.vxe                                            
Launching process 'hello_world_period2.vxe' ...                                 
Process 'hello_world_period2.vxe' (process Id = 0xffff8000002ba780) launched.   
Hello, World!                                                                   
[vxWorks *]# 
```

## Tarefa 3:
Para essa tarefa temos como objetivo conseguir utilizar uma tarefa para enviar um pacote para um IP e porta especificados no arquivo source. O pacote enviado é uma string `Olá, esse é um pacote de teste enviado pela rede!`.

Nesse arquivo source foram necessario utilizar libs especificas relacionadas a envio e comunicação UDP, definição de operações para internet entre outros.

Para verificar se o pacote foi realmente enviado, foi aberto outro terminal e utilizado o comando `sudo nc -u -l 21`, onde `21`é a porta utilizado no arquivo source `send_data.c`.

Tendo como resultado:
```
[vxWorks *]# send_data.vxe                                                      
Launching process 'send_data.vxe' ...                                           
Process 'send_data.vxe' (process Id = 0xffff8000002ba780) launched.             
Pacote enviado com sucesso, 51 bytes enviados.  
```
Terminal que recebe:
```
Olá, esse é um pacote de teste enviado pela rede!
```
![tarefa_3](/VxWorks//etapa_1/tarefa_3/tarefa_3.png)
