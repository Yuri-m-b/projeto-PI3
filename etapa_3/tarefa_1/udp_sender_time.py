"""Script para envio de dados em formato UDP com timestamp incluindo milissegundos."""

import socket
import time

SERVER_IP = "192.168.0.241"  # Endereço IP do Raspberry com VxWorks
SERVER_PORT = 21  # Port utilizada pelo VxWorks
MESSAGE = "Hello, VxWorks!"  # Mensagem/dado que será enviado
REPEAT = 10
DELAY = 1  # Delay entre mensagens em segundos


# Envia mensagem para o receptor
def main():
    """Main function"""
    # Cria um socket UDP
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    count = 0
    while count < REPEAT:
        # Obtém o tempo atual incluindo milissegundos
        current_time = time.time()  # Tempo atual em segundos com milissegundos
        # Formata o tempo como hh:mm:ss.mmm
        formatted_time = (
            time.strftime("%H:%M:%S.", time.gmtime(current_time))
            + f"{int((current_time % 1) * 1000):03d}"
        )
        # Cria a mensagem com timestamp
        message_with_time = f"{formatted_time}|{MESSAGE}"
        # Envia a mensagem
        sock.sendto(message_with_time.encode(), (SERVER_IP, SERVER_PORT))
        print(f"Mensagem Enviada às {formatted_time}: {MESSAGE}")
        count += 1

        # Aguarda antes de enviar a próxima mensagem
        time.sleep(DELAY)

    sock.close()

    return 0


if __name__ == "__main__":
    main()
