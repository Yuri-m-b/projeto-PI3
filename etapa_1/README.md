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

O objetivo do arquivo source `hello_world_period.c` é apresentar em sua execução um `print`
de forma períodica a partir de um loop while e um delay para a task.
```
[vxWorks *]# hello_world_period.vxe                                             
Launching process 'hello_world_period.vxe' ...                                  
Process 'hello_world_period.vxe' (process Id = 0xffff8000002baad0) launched.    
Hello, World!                                                                   
Hello, World!                                                                   
Hello, World!                                                                   
Hello, World!                                                                   
Hello, World!                                                                   
Hello, World!                                                                   
Hello, World! 
```
Como o loop foi definido como infinito, a mensagem continuara sendo mandada até o encerramento do processo no VxWorks.
