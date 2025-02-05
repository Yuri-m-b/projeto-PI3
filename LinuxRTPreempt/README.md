## Etapa 3: Comunicação e monitoramento de pacotes de rede

**Objetivo**: Nessa etapa descobrimos que os objetivos anteriormente definidos para esse projeto, em relação a utilização do VxWorks para comunicação, envio e administração de tarefas não seria possível porque essas são funções apenas dísponiveis na versão paga do software. Após uma nova conversa com os professores foi definido então a utilização de uma versão linux com suporte a RT Preempt (Real Time Computing). Sendo assim a versão da Raspberry OS com suporte a Real time utilizado é a `6.6.51+rpt-rpi-v8`.


Tarefas:

- [x] Realizar a comunicação entre um par de tarefas em diferentes RPi3.
- [x] Criar dois pares de tarefas que se comunicam com períodos diferentes, cada uma em uma RPi3.
- [x] Monitorar envio e recebimento de pacotes.
- [x] Registrar e demonstrar por resultados e textos a comunicação entre pelo menos UM par de tarefas.
- [x] Registrar e demonstrar por resultados e textos a comunicação entre dois pares de tarefas com períodos diferentes.
- [x] Demonstrar resultados da monitoração dos pacotes sendo enviados e recebidos pela rede.

**Entrega**: [Etapa 3](/etapa_3/)

## Etapa 4: Comunicação e monitoramento de pacotes de rede

**Objetivo**: O objetivo desta etapa do projeto é avaliar o desempenho do sistema de comunicação em tempo real, analisando parâmetros temporais e a perda de pacotes em diferentes cenários de processamento. Para isso, serão realizadas alterações nas tarefas existentes para aumentar a carga de processamento, permitindo a análise comparativa entre o comportamento original e o ajustado. Adicionalmente, o comportamento das tarefas será registrado e descrito detalhadamente, com foco nos impactos das mudanças no desempenho do sistema, buscando fornecer resultados e insights sobre a estabilidade e eficiência da comunicação em tempo real.


Tarefas:

- [x] Realizar a análise de parâmetros temporais do sistema.
- [x] Alterar as tarefas para realizarem um processamento maior.
- [x] Analisar os parâmetros temporais com as novas tarefas de maior processamento.
- [x] Verificar e comparar a perda de pacote nos dois cenários
- [x] Registrar comportamento e descrever o comportamento das três tarefas criadas para analise da comunicação em tempo real.
- [x] Trazer resultados e analises para os resultados dos parâmetros em tempo reais.
- [x] Trazer resultados e analises para os resultados dos recebimentos e percas de pacotes

**Entrega**: [Etapa 4](/etapa_34)
